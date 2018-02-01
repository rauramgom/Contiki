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
//#include "batmon-sensor.h"
#include "button-sensor.h"

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

char tabla[3];

PROCESS(prueba, "Prueba");
AUTOSTART_PROCESSES(&prueba);


PROCESS_THREAD(prueba, ev, data)
{
	PROCESS_BEGIN();
	cc26xx_uart_init();
	//Will receive a measure_ID based on a dictionary
	cc26xx_uart_set_input(serial_line_input_byte);

	while(1)
	{
		PROCESS_YIELD();
		if(ev == serial_line_event_message){
			printf("RECIBIDO: %s\n", (tabla!=NULL)?"SI":"NO");
			leds_toggle(LEDS_ALL);
			cc26xx_uart_write_byte('M');
			cc26xx_uart_write_byte(0x0a);
		}
	}
	PROCESS_END();
}
