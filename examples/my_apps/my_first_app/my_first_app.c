/*
My First App
3 Process: Main, auxiliary, auxiliary2
*/


#include "contiki.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "random.h"
#include "button-sensor.h"
#include "batmon-sensor.h"
#include "board-peripherals.h"
#include "rf-core/rf-ble.h"
#include "ti-lib.h"
#include <stdio.h>
#include <stdint.h>

//DEFINES

//Variables y estructuras globales
static struct etimer et1, et2, et3;
process_event_t event_from_process3;

PROCESS(process1,"Main Process 1");
PROCESS(process2,"Auxiliary Process 2");
PROCESS(process3,"Auxiliary Process 3");

AUTOSTART_PROCESSES(&process1 ,&process2);

//Definición de funciones

PROCESS_THREAD(process1, ev, data)
{
	PROCESS_BEGIN();
	static uint8_t count;
	printf("Process 1 started!\n");

	process_start(&process3, "Process 1");

	while(1){
		//Espera hasta que ocurra un evento cualquiera
		printf("[1] Espera hasta que ocurra evento...\n");
		PROCESS_YIELD();
		printf("[1] Evento capturado!\n");
		if(ev == event_from_process3){
			count = *((uint8_t *)data);
			printf("[1] Process 3 has requested shutdown in %u seconds\n", count);
			etimer_set(&et1, CLOCK_SECOND);
		}

		if(ev == PROCESS_EVENT_TIMER){
			if(count <= 0){
				process_exit(&process3);
				printf("[1] Proceso 3 exited!\n");
			} else {
				printf("[1] Process 3 will be terminated in: %u\n", count);
				count--;
				leds_toggle(LEDS_RED);
				etimer_reset(&et1);
			}
		}
	}
	PROCESS_END();
} //End of Process_Thread1
///////////////////////////////////////////////
PROCESS_THREAD(process2, ev, data)
{
	PROCESS_BEGIN();
	printf("Process 2 started!\n");

	while(1){
		printf("[2] Espera hasta que ocurra evento...\n");
		PROCESS_YIELD();
		if(ev == PROCESS_EVENT_EXITED){
			printf("[2]** Process 3 has been stopped by Process 1!\n");
			etimer_set(&et2, CLOCK_SECOND*4);
		}

		if(ev == PROCESS_EVENT_TIMER){
			printf("Process 2 is restarting Process 3\n");
			process_start(&process3, "Process2");
		}
	}
	PROCESS_END();
} //End of Process_Thread2
//////////////////////////////////////////////
PROCESS_THREAD(process3, ev, data)
{ 
	PROCESS_BEGIN();
	static char* parent;
	parent = (char*)data;
	static uint8_t counter;

	printf("Process 3 started by %s\n", parent);
	event_from_process3 = process_alloc_event();
	etimer_set(&et3, CLOCK_SECOND);

	counter = 0;

	while(1){
		//Captura el evento de 'expiración de timer' cuando se cumpla et3
		// Si expira cualquier etimer que no sea et3, no cogerá el evento por 
		//  ser una sentencia FALSE
		printf("[3] Estamos en el proceso 3, counter: %u\n", counter);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et3));
		counter++;
		leds_toggle(LEDS_GREEN);

		if(counter == 2){
			printf("[3] Contador ha llegado a %u\n", counter);
			process_post(&process1, event_from_process3, &counter);
		}
	}
	PROCESS_END();

} //End of Process_Thread3
