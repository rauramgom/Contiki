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

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

int temp=0;
unsigned int volt=0;
char buf_temp[4];
char buf_volt[5];

PROCESS(prueba, "Prueba");
AUTOSTART_PROCESSES(&prueba);


PROCESS_THREAD(prueba, ev, data)
{
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(batmon_sensor);
	cc26xx_uart_init();
	//Will receive a measure_ID based on a dictionary
	cc26xx_uart_set_input(serial_line_input_byte);

	while(1)
	{
		PROCESS_YIELD();
		if(ev == serial_line_event_message){
			memset(buf_temp, '\0', 4);
			memset(buf_volt, '\0', 5);
			temp=batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
			sprintf(buf_temp, "%d", temp);
			volt=batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
			sprintf(buf_volt, "%u", volt);
			printf("\ntemp=%d | %s\nvolt=%u | %s\n", temp, buf_temp, volt, buf_volt);

		}
	}
	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}
