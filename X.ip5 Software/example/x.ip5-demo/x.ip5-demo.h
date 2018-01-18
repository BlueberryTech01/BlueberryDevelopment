/*
 * Copyright (c) 11/2017, Blueberrytech srl
 * All rights reserved.
 *
/**
 * \addtogroup X.IP5-examples
 * @{
 *
 * \defgroup X-IP5-web-demo X.IP5 Web Demo
 * @{
 *
 *   An example demonstrating:
 *   * how to use a X.IP5-powered node in a deployment driven by a 6LBR
 *   * how to build a small web page which reports networking and sensory data
 *   * how to configure functionality through the aforementioned web page using
 *
 * \file
 *   Main header file for the X.IP5 web demo.
 */
/*---------------------------------------------------------------------------*/
#ifndef CC26XX_WEB_DEMO_H_
#define CC26XX_WEB_DEMO_H_
/*---------------------------------------------------------------------------*/
#include "dev/leds.h"
#include "sys/process.h"
#include <stdint.h>
/*---------------------------------------------------------------------------*/
#ifdef CC26XX_WEB_DEMO_CONF_6LBR_CLIENT
#define CC26XX_WEB_DEMO_6LBR_CLIENT CC26XX_WEB_DEMO_CONF_6LBR_CLIENT
#else
#define CC26XX_WEB_DEMO_6LBR_CLIENT 1
#endif

#ifdef CC26XX_WEB_DEMO_CONF_ADC_DEMO
#define CC26XX_WEB_DEMO_ADC_DEMO CC26XX_WEB_DEMO_CONF_ADC_DEMO
#else
#define CC26XX_WEB_DEMO_ADC_DEMO 0
#endif

#ifdef CC26XX_WEB_DEMO_CONF_BROADCAST
#define CC26XX_WEB_DEMO_BROADCAST CC26XX_WEB_DEMO_CONF_BROADCAST
#else
#define CC26XX_WEB_DEMO_BROADCAST 1
#endif
/*---------------------------------------------------------------------------*/
/* User configuration */
/* Take a sensor reading on button press */
#define CC26XX_WEB_DEMO_SENSOR_READING_TRIGGER &button_left_sensor

/* Payload length of ICMPv6 echo requests used to measure RSSI with def rt */
#define CC26XX_WEB_DEMO_ECHO_REQ_PAYLOAD_LEN   20

/*---------------------------------------------------------------------------*/
/* A timeout used when waiting to connect to a network */
#define CC26XX_WEB_DEMO_NET_CONNECT_PERIODIC        (CLOCK_SECOND >> 3)
/*---------------------------------------------------------------------------*/
/*
 * You normally won't have to change anything from here onwards unless you are
 * extending the example
 */
/*---------------------------------------------------------------------------*/
/* Sensor types */
#define CC26XX_WEB_DEMO_SENSOR_BATMON_TEMP   0
#define CC26XX_WEB_DEMO_SENSOR_BATMON_VOLT   1
/*---------------------------------------------------------------------------*/;
extern process_event_t cc26xx_web_demo_config_loaded_event;
extern process_event_t cc26xx_web_demo_load_config_defaults;
/*---------------------------------------------------------------------------*/
#define CC26XX_WEB_DEMO_UNIT_TEMP     "C"
#define CC26XX_WEB_DEMO_UNIT_VOLT     "mV"
#define CC26XX_WEB_DEMO_UNIT_PRES     "hPa"
#define CC26XX_WEB_DEMO_UNIT_HUMIDITY "%RH"
/*---------------------------------------------------------------------------*/
/* A data type for sensor readings, internally stored in a linked list */
#define CC26XX_WEB_DEMO_CONVERTED_LEN        12

typedef struct cc26xx_web_demo_sensor_reading {
  struct cc26xx_web_demo_sensor_reading *next;
  int raw;
  int last;
  const char *descr;
  const char *xml_element;
  const char *form_field;
  char *units;
  uint8_t type;
  uint8_t publish;
  uint8_t changed;
  char converted[CC26XX_WEB_DEMO_CONVERTED_LEN];
} cc26xx_web_demo_sensor_reading_t;
/*---------------------------------------------------------------------------*/
/* Global configuration */
typedef struct cc26xx_web_demo_config_s {
  uint32_t magic;
  int len;
  uint32_t sensors_bitmap;
  int def_rt_ping_interval;
} cc26xx_web_demo_config_t;

extern cc26xx_web_demo_config_t cc26xx_web_demo_config;
/*---------------------------------------------------------------------------*/
/**
 * \brief Performs a lookup for a reading of a specific type of sensor
 * \param sens_type CC26XX_WEB_DEMO_SENSOR_BATMON_TEMP...
 * \return A pointer to the reading data structure or NULL
 */
const cc26xx_web_demo_sensor_reading_t *cc26xx_web_demo_sensor_lookup(int sens_type);

/**
 * \brief Returns the first available sensor reading
 * \return A pointer to the reading data structure or NULL
 */
const cc26xx_web_demo_sensor_reading_t *cc26xx_web_demo_sensor_first(void);

/**
 * \brief Print an IPv6 address into a buffer
 * \param buf A pointer to the buffer where this function will print the IPv6
 *        address
 * \param buf_len the length of the buffer
 * \param addr A pointer to the IPv6 address
 * \return The number of bytes written to the buffer
 *
 * It is the caller's responsibility to allocate enough space for buf
 */
int cc26xx_web_demo_ipaddr_sprintf(char *buf, uint8_t buf_len,
                                   const uip_ipaddr_t *addr);

/**
 * \brief Resets the example to a default configuration
 */
void cc26xx_web_demo_restore_defaults(void);
/*---------------------------------------------------------------------------*/
#endif /* CC26XX_WEB_DEMO_H_ */
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */
