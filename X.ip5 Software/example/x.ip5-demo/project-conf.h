/*
 * Copyright (c) 01/2018, Blueberry Tech srl
 *
 * Project-conf.h
 *
 */
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
#ifdef WITH_NON_STORING
#define WITH_NON_STORING_ 0
#endif /* WITH NON STORING */
/*---------------------------------------------------------------------------*/
#ifdef TEST_MORE_ROUTES
/* configure number of neighbors and routes */
#define NBR_TABLE_CONF_MAX_NEIGHBORS     10
#define UIP_CONF_MAX_ROUTES   30
#else
/* configure number of neighbors and routes */
#define NBR_TABLE_CONF_MAX_NEIGHBORS     10
#define UIP_CONF_MAX_ROUTES   10
#endif /* TEST_MORE_ROUTES */
/*---------------------------------------------------------------------------*/
#undef NULLRDC_CONF_802154_AUTOACK
#define NULLRDC_CONF_802154_AUTOACK       1

/* Define as minutes */
#define RPL_CONF_DEFAULT_LIFETIME_UNIT   60

/* 10 minutes lifetime of routes */
#define RPL_CONF_DEFAULT_LIFETIME        10

#define RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME 1
/*---------------------------------------------------------------------------*/
#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#undef UIP_CONF_MAX_ROUTES
/*---------------------------------------------------------------------------*/
/* Change to match your configuration */
#define IEEE802154_CONF_PANID            0xABCD
#define RF_CORE_CONF_CHANNEL                 25
#define RF_BLE_CONF_ENABLED                   1
/*---------------------------------------------------------------------------*/
/* Enable/Disable Components of this Demo */
#define CC26XX_WEB_DEMO_CONF_6LBR_CLIENT      0
#define CC26XX_WEB_DEMO_CONF_BROADCAST     1
/*
 * ADC sensor functionality. To test this, an external voltage source should be
 * connected to DIO23
 * Enable/Disable DIO23 ADC reading by setting CC26XX_WEB_DEMO_CONF_ADC_DEMO
 */
#define CC26XX_WEB_DEMO_CONF_ADC_DEMO         0
/*---------------------------------------------------------------------------*/
/* Enable the ROM bootloader */
#define ROM_BOOTLOADER_ENABLE                 1
/*---------------------------------------------------------------------------*/
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC nullrdc_driver
/*---------------------------------------------------------------------------*/
/*
 * Shrink the size of the uIP buffer, routing table and ND cache.
 * Set the TCP MSS
 */
#define UIP_CONF_BUFFER_SIZE                900
//#define NBR_TABLE_CONF_MAX_NEIGHBORS          5
//#define UIP_CONF_MAX_ROUTES                   5
#define UIP_CONF_TCP_MSS                    128
/*---------------------------------------------------------------------------*/
#if WITH_NON_STORING
#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM 40 /* Number of links maintained at the root. Can be set to 0 at non-root nodes. */
#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 0 /* No need for routes */
#undef RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/
#endif /* WITH_NON_STORING */
/*---------------------------------------------/-----------------------------*/
#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
