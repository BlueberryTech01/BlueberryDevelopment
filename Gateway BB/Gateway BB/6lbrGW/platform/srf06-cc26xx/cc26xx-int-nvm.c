/*
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 * \file
 *         NVM Interface for the CC26XX platform
 */

#define LOG6LBR_MODULE "NVM"

#include "contiki.h"
#include "contiki-lib.h"

#include "cetic-6lbr.h"
#include "nvm-config.h"
#include "nvm-itf.h"
#include "log-6lbr.h"

#include "ti-lib.h"

#define CETIC_6LBR_NVM_SIZE 2048
// We use the penultimate flash page as our nvm
#define CETIC_6LBR_NVM_ADDRESS (0x00020000 - (2*CETIC_6LBR_NVM_SIZE))

void
nvm_data_read(void)
{
  LOG6LBR_INFO("Reading 6LBR NVM\n");
  memcpy( (void *)&nvm_data,
   (void *)CETIC_6LBR_NVM_ADDRESS, sizeof(nvm_data_t));
}

void
nvm_data_write(void)
{
  long err;
  int retry = 4;
  while (retry > 0 ) {
    LOG6LBR_INFO("Flashing 6LBR NVM\n");
    err = HapiSectorErase(CETIC_6LBR_NVM_ADDRESS);
    if ( err != 0 ) {
      LOG6LBR_ERROR("erase error : %ld\n", err);
    }
    err = HapiProgramFlash( (uint8_t*)&nvm_data,
     CETIC_6LBR_NVM_ADDRESS, (sizeof(nvm_data_t)/4+1)*4);
    if ( err != 0 ) {
      LOG6LBR_ERROR("write error : %ld\n", err);
    }
    if(memcmp( (void *)&nvm_data, (void *)CETIC_6LBR_NVM_ADDRESS, sizeof(nvm_data_t)) == 0) {
      break;
    }
    LOG6LBR_ERROR("verify NVM failed, retry\n");
    retry--;
  }
  if(retry == 0) {
    LOG6LBR_FATAL("Could not program 6LBR NVM !\n");
  }
}
