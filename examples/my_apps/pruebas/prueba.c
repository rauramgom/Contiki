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
		if(ev == serial_line_event_message)
			printf("RECIBIDO: %s\n", (char*)data);
	}
	PROCESS_END();
}