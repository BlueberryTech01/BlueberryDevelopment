/*
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 */

#ifndef ENC28J60_ARCH_H
#define ENC28J60_ARCH_H

#include <inttypes.h>

/* ENC28J60 architecture-specific SPI functions that are called by the
   enc28j60 driver and must be implemented by the platform code */

void enc28j60_arch_spi_init(void);
void enc28j60_arch_spi_write(uint8_t data);
uint8_t enc28j60_arch_spi_read(void);
void enc28j60_arch_spi_select(void);
void enc28j60_arch_spi_deselect(void);

#endif
