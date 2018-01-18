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
      data[packetbuf_datalen()] = 0;
      if(strcmp(data, "APRE") == 0) {
      leds_on(LEDS_RED);
      printf("Sta Aprendo");
    } else if(strcmp(data, "CHID") == 0) {
      leds_off(LEDS_RED);
      printf("Sta chiudendo");
    } else if(strcmp(data, "APER") == 0) {
      leds_on(LEDS_YELLOW);
      printf("Porta Aperta");
    } else if(strcmp(data, "CHIU") == 0) {
      leds_off(LEDS_YELLOW);
      printf("Porta Chiusa");
    } else if(strcmp(data, "BLOC") == 0) {
      leds_on(LEDS_RED);
      printf("Porta in errore");
    } else if(strcmp(data, "STOP") == 0) {
      leds_on(LEDS_RED);
      printf("Porta bloccata");
       }
      }
    //process_poll(&example_broadcast_process);
  } 


static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
  static struct etimer et;
  static char command[3+1];

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  cc26xx_uart_set_input(serial_line_input_byte);
  broadcast_open(&broadcast, 129, &broadcast_call);
  
  while(1) {
    PROCESS_YIELD();
    if(ev == serial_line_event_message && data != NULL) {
      if(strcmp(data,"ON")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        //cmd[0]=0x4F; 
        //cmd[1]=0x4E;
        leds_on(LEDS_RED);
      } else if(strcmp(data,"OFF")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        //cmd[0]=0x41;
        //cmd[1]=0x42;
        leds_off(LEDS_RED);
      } else if(strcmp(data,"PE")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        //cmd[0]=0x41;
        //cmd[1]=0x42;
        leds_on(LEDS_YELLOW);
      } else if(strcmp(data,"ST")==0) {
        //for(int i=0;i<5;i++)cmd[i]=0x0;
        //cmd[0]=0x41;
        //cmd[1]=0x42;
        leds_off(LEDS_YELLOW);
      }  else {
        continue;
      }

      strcpy(command, data);

      etimer_set(&et, CLOCK_SECOND * 1 + random_rand() % (CLOCK_SECOND * 1));
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

      packetbuf_copyfrom(command, strlen(command) + 1);  
      broadcast_send(&broadcast);
      //printf("Comando inviato\n");
    }
  }

  PROCESS_END();
}
