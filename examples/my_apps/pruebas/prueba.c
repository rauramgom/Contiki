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
#include <string.h>

static struct etimer et;

static int amb_light = 0;


PROCESS(prueba, "Prueba");
AUTOSTART_PROCESSES(&prueba);

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
			leds_toggle(LEDS_GREEN);
			amb_light = 1;//als_sensor.value(0);
			printf("Medida: %d\n", amb_light);
			//etimer_restart(&et);
		}
	}
	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}