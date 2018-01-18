/*
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 *
 /**
 * \file
 *         NVM Interface for the C6XX platform
 */

#define LOG6LBR_MODULE "NVM"

#include "contiki.h"
#include "contiki-lib.h"

#include "nvm-config.h"
#include "nvm-itf.h"
#include "board-peripherals.h"

#include "log-6lbr.h"

#define CETIC_6LBR_NVM_SIZE 2048
#define CETIC_6LBR_NVM_ADDRESS 0

void
nvm_data_read(void)
{
  LOG6LBR_INFO("Reading 6LBR NVM\n");
  int rv = ext_flash_open();

  if(!rv) {
    LOG6LBR_ERROR("Could not open external flash\n");
    ext_flash_close();
    return;
  }

  rv = ext_flash_read(CETIC_6LBR_NVM_ADDRESS, sizeof(nvm_data_t),
                      (uint8_t *)&nvm_data);

  ext_flash_close();

  if(!rv) {
    printf("Error loading config\n");
    return;
  }
}

void
nvm_data_write(void)
{
  LOG6LBR_INFO("Writing 6LBR NVM\n");
  int rv;
  rv = ext_flash_open();

  if(!rv) {
    LOG6LBR_ERROR("Could not open external flash\n");
    ext_flash_close();
    return;
  }

  rv = ext_flash_erase(CETIC_6LBR_NVM_ADDRESS, CETIC_6LBR_NVM_SIZE);

  if(!rv) {
    LOG6LBR_ERROR("Error erasing flash\n");
  } else {
    rv = ext_flash_write(CETIC_6LBR_NVM_ADDRESS, sizeof(nvm_data_t),
                         (uint8_t *)&nvm_data);
    if(!rv) {
      LOG6LBR_ERROR("Error writing flash\n");
    }
  }

  ext_flash_close();
}
