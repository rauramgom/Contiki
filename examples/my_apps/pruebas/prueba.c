/**
 * \file
 *         Serial line interface slave
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
//#include "contiki.h"

//#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"

#include "dev/leds.h"
//#include "batmon-sensor.h"
#include "button-sensor.h"

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


PROCESS(prueba, "Prueba");
AUTOSTART_PROCESSES(&prueba);

char src[10];
char dst1[10];
char dst2[10];

PROCESS_THREAD(prueba, ev, data)
{
	PROCESS_BEGIN();
	//cc26xx_uart_init();
	//Will receive a measure_ID based on a dictionary
	//cc26xx_uart_set_input(serial_line_input_byte);

	while(1)
	{
		strcpy(src, "");
		strncpy(dst1, src, sizeof(src));
		printf("[%lu] DEST1: %s\n", clock_seconds()%CLOCK_SECOND, dst1);
		strcpy(src, "23");
		strncpy(dst2, src, sizeof(src));
		printf("DEST2: %s\n", dst2);
		int ret = strncmp(dst1, dst2, sizeof(dst1));
		if(ret < 0) {
      		printf("str1 is less than str2");
   		} else if(ret > 0) {
     		printf("str2 is less than str1");
   		} else {
      		printf("str1 is equal to str2");
   		}

		PROCESS_YIELD();
		if(ev == serial_line_event_message)
			printf("RECIBIDO: %s\n", (char*)data);
	}
	PROCESS_END();
}
