/*
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 *       
 */

#ifndef CETIC_6LBR_H_
#define CETIC_6LBR_H_

#include "net/rpl/rpl.h"
#include "net/ip64/ip64-dhcpc.h"

PROCESS_NAME(cetic_6lbr_process);

extern process_event_t cetic_6lbr_restart_event;
extern process_event_t cetic_6lbr_reload_event;

extern void cetic_6lbr_set_prefix(uip_ipaddr_t * prefix, unsigned len,
                                  uip_ipaddr_t * ipaddr);

extern void cetic_6lbr_ip64_dhcpc_configured(const struct ip64_dhcpc_state *s);

typedef uint8_t ethaddr_t[6];

//Initialisation flags
extern int ethernet_ready;
extern int eth_mac_addr_ready;
extern int radio_ready;
extern int radio_mac_addr_ready;

// WSN Side
extern uip_lladdr_t wsn_mac_addr;
extern uip_ipaddr_t wsn_ip_addr;
extern uip_ipaddr_t wsn_ip_local_addr;  //Created from wsn_mac_addr

extern uip_ip6addr_t wsn_net_prefix;
extern uint8_t wsn_net_prefix_len;

extern rpl_dag_t *cetic_dag;

// Ethernet side
extern ethaddr_t eth_mac_addr;
extern uip_lladdr_t eth_mac64_addr;     //Created from eth_mac_addr

extern uip_ipaddr_t eth_ip_addr;
extern uip_ipaddr_t eth_ip_local_addr;  //Created from eth_mac_addr

extern uip_ipaddr_t eth_net_prefix;

extern uip_ipaddr_t eth_dft_router;

extern uip_ip4addr_t eth_ip64_addr;
extern uip_ip4addr_t eth_ip64_netmask;
extern uip_ip4addr_t eth_ip64_gateway;

// Misc
extern unsigned long cetic_6lbr_startup;

enum cetic_6lbr_restart_type_t {
  CETIC_6LBR_NO_RESTART,
  CETIC_6LBR_RESTART,
  CETIC_6LBR_REBOOT,
  CETIC_6LBR_HALT
};

extern enum cetic_6lbr_restart_type_t cetic_6lbr_restart_type;
#endif
