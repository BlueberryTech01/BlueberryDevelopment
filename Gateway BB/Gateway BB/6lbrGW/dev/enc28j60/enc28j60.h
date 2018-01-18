/*
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 *
 */

#ifndef ENC28J60_H
#define ENC28J60_H
#include <inttypes.h>

void enc28j60_init(uint8_t *mac_addr);
int enc28j60_send(uint8_t *data, uint16_t datalen);
int enc28j60_read(uint8_t *buffer, uint16_t bufsize);

#endif
