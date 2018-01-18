/*
 * Copyright (c) 01/2018, Blueberry Tech srl
 *
 *
 * Comunicazione seriale via radio tra device Blueberry
 *
 *
 *
*/
#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/serial-line.h"
#include "dev/cc26xx-uart.h"

#include "simple-udp.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234

static struct simple_udp_connection broadcast_connection;

/*---------------------------------------------------------------------------*/
PROCESS(broadcast_example_process, "UDP broadcast example process");
AUTOSTART_PROCESSES(&broadcast_example_process);
/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

     char *appdata;

      if((appdata = data) !=NULL){
      if(strcmp(appdata, "ON") == 0) {
      leds_on(LEDS_RED);
      printf("aAPRIz");
    } else if(strcmp(appdata, "OFF") == 0) {
      leds_off(LEDS_RED);
      printf("aCHIUz");
    } else if(strcmp(appdata, "PE") == 0) {
      leds_on(LEDS_YELLOW);
      printf("aPEDOz");
    } else if(strcmp(appdata, "ST") == 0) {
      leds_off(LEDS_YELLOW);
      printf("aSTOPz");
       }
    }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(broadcast_example_process, ev, data)
{
  static struct etimer et;
  static char command[3+1];
  uip_ipaddr_t addr;

  PROCESS_BEGIN();

  cc26xx_uart_set_input(serial_line_input_byte);

  simple_udp_register(&broadcast_connection, UDP_PORT,
                      NULL, UDP_PORT,
                      receiver);

  while(1) {
    PROCESS_YIELD();
    if(ev == serial_line_event_message && data != NULL) {
      if(strcmp(data,"APRE")==0) {
        leds_on(LEDS_RED);
      } else if(strcmp(data,"CHID")==0) {
        leds_off(LEDS_RED);
      } else if(strcmp(data,"APER")==0) {
        leds_on(LEDS_YELLOW);
      } else if(strcmp(data,"CHIU")==0) {
        leds_off(LEDS_YELLOW);
      } else if(strcmp(data,"BLOC")==0) {
        leds_on(LEDS_RED);
      } else if(strcmp(data,"STOP")==0) {
        leds_off(LEDS_RED);
      } else {
        continue;
      }
   
    strcpy(command, data);

      etimer_set(&et, CLOCK_SECOND * 1 + random_rand() % (CLOCK_SECOND * 1));
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
 
    //printf("Sending broadcast\n");
    uip_create_linklocal_allnodes_mcast(&addr);
    simple_udp_sendto(&broadcast_connection, command, strlen(command) + 1, &addr);
  }
}
  PROCESS_END();
}
