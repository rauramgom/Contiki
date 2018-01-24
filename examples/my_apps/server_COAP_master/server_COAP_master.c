/**
 * \file
 *         Server COAP and serial line interface master
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
#include "contiki.h"
#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include <stdio.h> 
#include <string.h>
#include "lib/ringbuf.h"

//Dictionary
////////////////////////
#define TEMP				'1'
#define VOLT				'2'

#define LED_GREEN_POST_ON	'3'
#define LED_GREEN_POST_OFF	'4'

#define LED_BLUE_POST_ON	'5'
#define LED_BLUE_POST_OFF	'6'

#define LED_RED_POST_ON		'7'
#define LED_RED_POST_OFF	'8'

#define LED_YELLOW_POST_ON	'9'
#define LED_YELLOW_POST_OFF	'a'

#define LED_ALL_POST_ON		'b'
#define LED_ALL_POST_OFF	'c'

#define LED_ON				'1'
#define LED_OFF				'0'
#define END			0x0a
////////////////////////

#define BUFF_SIZE	6	// worse case: 3 2 1 1 END \0
static char buffer[BUFF_SIZE];

/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern resource_t
	res_temp,
	res_volt,
 	res_led_green,
	res_led_blue,
	res_led_red,
	res_led_yellow,
	res_led_all

static struct etimer et_get;
#define ETIMER_GET	CLOCK_SECOND*7

PROCESS(server_COAP_master, "Server COAP and serial line interface master");
PROCESS(aux_process, "Auxiliar process to fill in the response payload");
AUTOSTART_PROCESSES(&server_COAP_master);

/*
* The callback function is called when the master receives the response
*/
static int uart_rx_callback(unsigned char c) {
	//RELLENAR BUFFER

	//COPIAR BUFFER EN LA VARIABLE GLOBAL UTILIZADA POR EL RECURSO

	return 1;
}

PROCESS_THREAD(server_COAP_master, ev, data)
{
	PROCESS_BEGIN();
	//Initialize the REST engine. 
	rest_init_engine();
	SENSORS_ACTIVATE(batmon_sensor);

#if WITH_OBSERVABLE
	res_temp.flags += IS_OBSERVABLE;
	res_volt.flags += IS_OBSERVABLE;
#endif
	rest_activate_resource(&res_temp, "sen/temp");
	rest_activate_resource(&res_volt, "sen/volt");
	rest_activate_resource(&res_led_green, "led/green");
	rest_activate_resource(&res_led_blue, "led/blue");
	rest_activate_resource(&res_led_red, "led/red");
	rest_activate_resource(&res_led_yellow, "led/yellow");
	rest_activate_resource(&res_led_all, "led/all");

	cc26xx_uart_init();
	//Will receive the response from slave
	cc26xx_uart_set_input(uart_rx_callback);

	etimer_set(&et_get, ETIMER_GET);
	while(1) {
		//Waiting request..
		PROCESS_YIELD();
		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_get))
		{
			REST.notify_subscribers(&reading_resources);
			etimer_restart(&et_get);
		}
	}
	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}

/*PROCESS_THREAD(aux_process, ev, data)
{
	PROCESS_BEGIN();
	PROCESS_PAUSE()
	int flag_serial = 0;
	int flag_posted = 0;

	while (flag_posted==0 || flag_serial==0)
	{
		PROCESS_YIELD();
		if(ev == event_posted_by_resource && data != NULL)
		{
			flag_posted=1;
			//Recibe la estructura response
		}

		if(ev == serial_line_event_message && data != NULL)
		{
			flag_serial=1;
			//Relleno los campos de la response
		}
	}
	PROCESS_END();
}*/