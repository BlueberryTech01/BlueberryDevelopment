#include <stdio.h>
#include <string.h>

#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/serial-line.h"
#include "dev/cc26xx-uart.h"

#include "dev/button-sensor.h"
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);
//char cmd[5];
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  char *data;

      if((data = packetbuf_dataptr()) !=NULL){
      //data[packetbuf_datalen()] = 0;
      if(strcmp(data, "ON") == 0) {
      leds_on(LEDS_RED);
      printf("aAPRIz");
    } else if(strcmp(data, "OFF") == 0) {
      leds_off(LEDS_RED);
      printf("aCHIUz");
    } else if(strcmp(data, "PE") == 0) {
      leds_on(LEDS_YELLOW);
      printf("aPEDOz");
    } else if(strcmp(data, "ST") == 0) {
      leds_off(LEDS_YELLOW);
      printf("aSTOPz");
      }
    process_poll(&example_broadcast_process);
  } 
} 

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
   static char command[3+1];

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  cc26xx_uart_set_input(serial_line_input_byte);
  broadcast_open(&broadcast, 129, &broadcast_call);

  while(1) {
    PROCESS_YIELD();
    if(ev == serial_line_event_message && data != NULL) {
      if(strcmp(data,"APRE")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        leds_on(LEDS_RED);
      } else if(strcmp(data,"CHID")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        leds_off(LEDS_RED);
      } else if(strcmp(data,"APER")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        leds_on(LEDS_YELLOW);
      } else if(strcmp(data,"CHIU")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        leds_off(LEDS_YELLOW);
      } else if(strcmp(data,"BLOC")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        leds_on(LEDS_RED);
      } else if(strcmp(data,"STOP")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        leds_off(LEDS_RED);
      } 
 
      strcpy(command, data);     
 
      packetbuf_copyfrom(command, strlen(command) + 1);  /* (copy '\0' also) */
      broadcast_send(&broadcast);
      //printf("Comando inviato\n");
    } 
} 
    PROCESS_END();
}
