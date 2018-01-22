/**
 * \file
 *         Serial line interface slave
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
//#include "contiki.h"

#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"

#include "dev/leds.h"
#include "batmon-sensor.h"
#include "button-sensor.h"
#include "srf06/als-sensor.h"

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

static struct etimer et;
static int value = 20;
static int value_n = -20;
char buf_value[4];
#define END 0x0a

PROCESS(prueba, "Prueba");
AUTOSTART_PROCESSES(&prueba);

/*
* Function called to send the requested data
*/
static void send_data_uart(char* buf)
{
	for(int pos=0; pos<strlen(buf); pos++)
		cc26xx_uart_write_byte((uint8_t)buf[pos]);
	cc26xx_uart_write_byte(END);
}

PROCESS_THREAD(prueba, ev, data)
{
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(batmon_sensor);
	etimer_set(&et, 3*CLOCK_SECOND);
	while(1) {
		//Waiting request..
		PROCESS_YIELD();
		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et))
		{
			//leds_toggle(LEDS_GREEN);
			if(rand()%10 < 5){
				sprintf(buf_value, "%d", value);
				printf("**\npos:%s\n", buf_value);
				send_data_uart(buf_value);
				printf("**\n\n");
				//
			} else {
				sprintf(buf_value, "%d", value_n);
				printf("**\nneg:%s\n**\n", buf_value);
				send_data_uart(buf_value);
				printf("**\n\n");
			}
			etimer_restart(&et);
		}
	}
	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}