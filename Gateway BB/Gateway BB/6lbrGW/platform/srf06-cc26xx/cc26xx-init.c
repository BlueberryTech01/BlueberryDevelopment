/* 
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 */

#define LOG6LBR_MODULE "CC26XX"

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "dev/watchdog.h"

#include "platform-init.h"
#include "cetic-6lbr.h"
#include "sicslow-ethernet.h"
#include "nvm-config.h"
#include "log-6lbr.h"

void
platform_init(void)
{
}

void
platform_finalize(void)
{
}

void
platform_load_config(config_level_t level)
{
  switch(level) {
  case CONFIG_LEVEL_LOAD:
    load_nvm_config();
    break;
  default:
    break;
  }
}
void
platform_radio_init(void)
{
  NETSTACK_RADIO.set_value(RADIO_PARAM_CHANNEL, nvm_data.channel);
  NETSTACK_RADIO.set_value(RADIO_PARAM_PAN_ID, nvm_data.pan_id);
  radio_ready = 1;
  radio_mac_addr_ready = 1;
}

void
platform_set_wsn_mac(linkaddr_t * mac_addr)
{
  linkaddr_set_node_addr(mac_addr);
}

void
platform_restart(void)
{
  LOG6LBR_INFO("Rebooting...\n");
  watchdog_reboot();
}
