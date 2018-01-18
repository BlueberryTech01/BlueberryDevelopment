/*
 * Copyright (c) 01/2018, Blueberry Tech srl
 *
 * 
 * 6LBR Web Server per Gateway X.IP5 
 */

#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/netstack.h"
#include "net/rpl/rpl.h"

#include "sicslow-ethernet.h"
#include "cetic-6lbr.h"
#include "nvm-config.h"
#include "rio.h"
#if CETIC_NODE_INFO
#include "node-info.h"
#endif

#include <stdio.h>              /* For printf() */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

extern uip_ds6_nbr_t uip_ds6_nbr_cache[];
extern uip_ds6_prefix_t uip_ds6_prefix_list[];

extern rpl_instance_t instance_table[RPL_MAX_INSTANCES];

int count = 0;

/*---------------------------------------------------------------------------*/
#include "httpd-simple.h"

#ifndef WEBSERVER_CONF_LOADTIME
#define WEBSERVER_CONF_LOADTIME 0
#endif
#ifndef WEBSERVER_CONF_FILESTATS
#define WEBSERVER_CONF_FILESTATS 0
#endif

#ifndef WEBSERVER_CONF_INFO
#define WEBSERVER_CONF_INFO 1
#endif
#ifndef WEBSERVER_CONF_SENSORS
#define WEBSERVER_CONF_SENSORS 0
#endif
#ifndef WEBSERVER_CONF_RPL
#define WEBSERVER_CONF_RPL 0
#endif
#ifndef WEBSERVER_CONF_NETWORK
#define WEBSERVER_CONF_NETWORK 1
#endif
#ifndef WEBSERVER_CONF_CONFIG
#define WEBSERVER_CONF_CONFIG 1
#endif

#define BUF_USES_STACK 0
/*---------------------------------------------------------------------------*/
PROCESS(httpd_process, "Web server");
PROCESS_THREAD(httpd_process, ev, data)
{
  PROCESS_BEGIN();

  httpd_init();

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    httpd_appcall(data);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
webserver_init(void)
{
  process_start(&httpd_process, NULL);
}
/*---------------------------------------------------------------------------*/
#define BUF_SIZE 256
static const char *TOP =
  "<html><head><title>GATEWAY BLUEBERRY TECH</title><style type=\"text/css\">"
  "body{font-family:Verdana;color:#000080;padding:20px;}"
  "h1,h2{margin:40px 0 0;padding:0;font-weight:bold;}"
  "h1{font-size:16px;line-height:18px;}"
  "h2{font-size:14px;color:#87ceeb;line-height:16px;}"
  "h3{font-size:12px;font-weight:bold;line-height:14px;}"
  "#h{margin:0;}"
  "</style></head>";
static const char *BODY = "<body>";
static const char *BOTTOM =
  "<hr/>"
  "<small>6LBR By Blueberry Tech srl (<a href=\"http://www.blueberryonline.eu/\">doc</a>)</small>"
  "</body></html>";

#if BUF_USES_STACK
static char *bufptr, *bufend;
#else
static char buf[BUF_SIZE];
static int blen;
#endif
/*---------------------------------------------------------------------------*/
/*OPTIMIZATIONS to gain space : prototypes*/
static void add(char *str, ...);
void add_network_cases(const uint8_t state);
static void reset_buf();

/*End optimizations*/
/*---------------------------------------------------------------------------*/
static void
ipaddr_add(const uip_ipaddr_t * addr)
{
  uint16_t a;
  int i, f;

  for(i = 0, f = 0; i < sizeof(uip_ipaddr_t); i += 2) {
    a = (addr->u8[i] << 8) + addr->u8[i + 1];
    if(a == 0 && f >= 0) {
      if(f++ == 0)
        add("::");
    } else {
      if(f > 0) {
        f = -1;
      } else if(i > 0) {
        add(":");
      }
      add("%x", a);
    }
  }
}
static void
lladdr_add(const uip_lladdr_t * addr)
{
  int i;

  for(i = 0; i < sizeof(uip_lladdr_t); i++) {
    if(i > 0) {
      add(":");
    }
    add("%x", addr->addr[i]);
  }
}
static void
ethaddr_add(ethaddr_t * addr)
{
  int i;

  for(i = 0; i < 6; i++) {
    if(i > 0) {
      add(":");
    }

    add("%x", (*addr)[i]);
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(generate_index(struct httpd_state *s))
{
  static int i;
  //static int j;
  static uip_ds6_nbr_t *nbr;
  static uip_ds6_route_t *r;
  static uip_ds6_defrt_t *dr;

#if BUF_USES_STACK
  char buf[BUF_SIZE];
#endif
#if WEBSERVER_CONF_LOADTIME
  static clock_time_t numticks;

  numticks = clock_time();
#endif

  PSOCK_BEGIN(&s->sout);

  SEND_STRING(&s->sout, TOP);
  SEND_STRING(&s->sout, BODY);
  reset_buf();
  add("<h1 id=\"h\">IoT Gateway <i>Blueberry Tech</i></h1><h2 id=\"h\">Gateway Status</h2><hr>");
#if WEBSERVER_CONF_INFO
  add("<h1>Info</h1>");
  add("<h2>6LBR</h2>");
  add("<br>");
  i = clock_seconds() - cetic_6lbr_startup;
  add("Operating time : %dh %dm %ds<br>", i / 3600, (i / 60) % 60, i % 60);
  SEND_STRING(&s->sout, buf);
  reset_buf();

  add("<h2>Ethernet</h2>");
#if CETIC_6LBR_ROUTER
  add("Address : ");
  ipaddr_add(&eth_ip_addr);
  add("<br>");
  add("Gateway IPv6 address : ");
  ipaddr_add(&eth_ip_local_addr);
  add("<br>");
#endif
#endif

/*#if WEBSERVER_CONF_SENSORS
#if CETIC_NODE_CONFIG
  node_config_t *  my_config = node_config_find_by_lladdr(&uip_lladdr);
  if (my_config) {
    add("%s;", node_config_get_name(my_config));
  } else {
   add("_%04x;",
     (uip_lladdr.addr[6] << 8) + uip_lladdr.addr[7]);
  }
#else
  add("_%04x;",
    (uip_lladdr.addr[6] << 8) + uip_lladdr.addr[7]);
#endif
  for(i = 0; i < UIP_DS6_ROUTE_NB; i++) {
    if(node_info_table[i].isused) {
      if(! uip_is_addr_unspecified(&node_info_table[i].ip_parent)) {
#if CETIC_NODE_CONFIG
        node_config_t * node_config = node_config_find_by_ip(&node_info_table[i].ipaddr);
        node_config_t * parent_node_config = node_config_find_by_ip(&node_info_table[i].ip_parent);
        if ( node_config ) {
          if ( parent_node_config ) {
            add("%s->%s;",
                node_config_get_name(node_config),
                node_config_get_name(parent_node_config));
          } else {
            add("%s->_%04hx;",
                node_config_get_name(node_config),
                (node_info_table[i].ip_parent.u8[14] << 8) +
                node_info_table[i].ip_parent.u8[15]);
          }
        } else {
          if (parent_node_config) {
            add("_%04hx->%s;",
                (node_info_table[i].ipaddr.u8[14] << 8) +
                node_info_table[i].ipaddr.u8[15],
                node_config_get_name(parent_node_config));
          } else {
            add("_%04hx->_%04hx;",
                (node_info_table[i].ipaddr.u8[14] << 8) +
                node_info_table[i].ipaddr.u8[15],
                (node_info_table[i].ip_parent.u8[14] << 8) +
                node_info_table[i].ip_parent.u8[15]);
          }
        }
#else
        add("_%04hx->_%04hx;",
            (node_info_table[i].ipaddr.u8[14] << 8) +
            node_info_table[i].ipaddr.u8[15],
            (node_info_table[i].ip_parent.u8[14] << 8) +
            node_info_table[i].ip_parent.u8[15]);
#endif
      }
    }
  }
  add("}\"alt=\"\" /></center>");
  SEND_STRING(&s->sout, buf);
  reset_buf();
#endif */
#if WEBSERVER_CONF_NETWORK
  add("</pre><h2>X.IP5 Device</h2><pre>");
  SEND_STRING(&s->sout, buf);
  reset_buf();
  for(r = uip_ds6_route_head(), i = 0; r != NULL;
      r = uip_ds6_route_next(r), ++i) {
    ipaddr_add(&r->ipaddr);
    add("/%u via ", r->length);
    ipaddr_add(uip_ds6_route_nexthop(r));
    if(1 || (r->state.lifetime < 600)) {
      add("%lu s\n", r->state.lifetime);
    } else {
      add("\n");
    }
    SEND_STRING(&s->sout, buf);
    reset_buf();
  }
#endif

#if WEBSERVER_CONF_FILESTATS
  static uint16_t numtimes;

  add("<br><i>This page sent %u times</i>", ++numtimes);
#endif

#if WEBSERVER_CONF_LOADTIME
  numticks = clock_time() - numticks + 1;
  add(" <i>(%u.%02u sec)</i>", numticks / CLOCK_SECOND,
      (100 * (numticks % CLOCK_SECOND) / CLOCK_SECOND));
  SEND_STRING(&s->sout, buf);
  reset_buf();
#endif

  SEND_STRING(&s->sout, BOTTOM);
  PSOCK_END(&s->sout);
}

/*---------------------------------------------------------------------------*/
httpd_simple_script_t
httpd_simple_get_script(const char *name)
{
  //static uip_ds6_route_t *r;
  //static int i;

  if(strcmp(name, "index.html") == 0 || strcmp(name, "") == 0) {
    return generate_index;
  } else {
    return NULL;
  }
}
/*---------------------------------------------------------------------------*/
void
add_network_cases(const uint8_t state)
{
  switch (state) {
  case NBR_INCOMPLETE:
    add("I");
    break;
  case NBR_REACHABLE:
    add("R");
    break;
  case NBR_STALE:
    add("S");
    break;
  case NBR_DELAY:
    add("D");
    break;
  case NBR_PROBE:
    add("P");
    break;
  }
}
/*---------------------------------------------------------------------------*/
/*Macro redefined : RESET_BUF()*/
/*---------------------------------------------------------------------------*/
static void
reset_buf()
{
#if BUF_USES_STACK
  bufptr = buf;
  bufend = bufptr + sizeof(buf);
#else
  blen = 0;
#endif
}
/*---------------------------------------------------------------------------*/
/*Macro redefined : ADD()*/
/*---------------------------------------------------------------------------*/
static void
add(char *str, ...)
{
  va_list arg;

  va_start(arg, str);
  //ADD(str, arg); //TODO : bug while formating
#if BUF_USES_STACK
  bufptr += vsnprintf(bufptr, bufend - bufptr, str, arg);
#else
  blen += vsnprintf(&buf[blen], sizeof(buf) - blen, str, arg);
#endif
  va_end(arg);
}
/*---------------------------------------------------------------------------*/
