/*
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 *
 * \file
 *         6LBR Project Configuration
 */

#ifndef SIXLBR_CONF_CC26XX_H
#define SIXLBR_CONF_CC26XX_H

/*------------------------------------------------------------------*/
/* CC26XX 6LBR                                                    */
/*------------------------------------------------------------------*/

#define LOG6LBR_LEVEL_DEFAULT LOG6LBR_LEVEL_INFO

#define LOG6LBR_SERVICE_DEFAULT   LOG6LBR_SERVICE_ALL

#if WEBSERVER
#undef UIP_CONF_DS6_NBR_NBU
#define UIP_CONF_DS6_NBR_NBU     25

#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES   25

#undef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE                600

#else
#undef UIP_CONF_DS6_NBR_NBU
#define UIP_CONF_DS6_NBR_NBU     50

#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES   50

#endif

#define WEBSERVER_CONF_CFS_PATHLEN 600

#define WEBSERVER_CONF_CFS_URLCONV      1

#define WEBSERVER_CONF_CFS_CONNS        1

#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID   0xABCD

#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     csma_driver

#define CETIC_CSMA_STATS      0

#define CETIC_6LBR_LLSEC_WRAPPER        0

#undef CETIC_6LBR_IP64
#define CETIC_6LBR_IP64      0

/* Do not change lines below */

// IP packet must be 4 bytes aligned
#undef UIP_CONF_LLH_LEN
#define UIP_CONF_LLH_LEN 16

#define LOG6LBR_TIMESTAMP           0
#define LOG6LBR_STATIC              1

#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     nullrdc_driver

#endif /* SIXLBR_CONF_CC26XX_H */
