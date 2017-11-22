/*
My Third App
2 Processes:
El proceso principal inicia un ctime, que al cumplirse inicia el
segundo proceso, este enciende un LED ROJO si el evento proviene del Main
sino, enciende un LED AZUL
*/


#include "contiki.h"
#include "sys/etimer.h"
#include "sys/stimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "random.h"
#include "button-sensor.h" 
#include <stdio.h>
#include <stdint.h>

//DEFINES

//Variables y estructuras globales
static struct ctimer ct;
static struct etimer et;
process_event_t event_from_processMain;
static int aux = 0;


PROCESS(my_third_app_main, "My Third App Main");
PROCESS(my_third_app_auxiliary, "My Third App Auxiliary");

static void
ctimer_callback_example(void *ptr)
{
	int *datos_rcv = (int *)ptr;
	printf("*** [CALLBACK1] count: %d\n", *datos_rcv);
	*datos_rcv = 0;

	
	event_from_processMain = process_alloc_event();
	process_post(&my_third_app_auxiliary, event_from_processMain, datos_rcv);

	ctimer_restart(&ct);
}


AUTOSTART_PROCESSES(&my_third_app_main, &my_third_app_auxiliary);

//Main process
PROCESS_THREAD(my_third_app_main, ev, data)
{
	PROCESS_BEGIN();
	//static int *ptr_cont = NULL;
	printf("Process Main started\n");
	//*ptr_cont = 0;
	printf("[MAINNN] count: %d\n", 3);
	int a = 1;
	//a++;
	ctimer_set(&ct, 5, ctimer_callback_example, &a);

	while(1)
		PROCESS_YIELD();
		//PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
		
	PROCESS_END();
}


//Auxiliary process
PROCESS_THREAD(my_third_app_auxiliary, ev, data)
{
	PROCESS_BEGIN();
	
	printf("Process Auxiliary started\n");
	etimer_set(&et, CLOCK_SECOND);
	
	while(1){
		//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		PROCESS_YIELD();
		if(ev == event_from_processMain){
			if (aux==40){
				//Parpadean los leds apagados
				leds_blink();
				aux=0;
			}
			aux++;
		} else if(ev == PROCESS_EVENT_TIMER) {
			printf("[AUXILIARY] ERROR!!\n");
			leds_toggle(LEDS_BLUE);
		}
	}
	
	PROCESS_END();
	
}
