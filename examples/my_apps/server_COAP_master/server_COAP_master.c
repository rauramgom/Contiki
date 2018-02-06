/**
 * \file
 *         Server COAP and serial line interface master
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
#include "resources/api_resources.h"

/*
 * Resources will be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern resource_t res_temp;
extern resource_t res_volt;
extern resource_t res_led_green;
extern resource_t res_led_blue;
extern resource_t res_led_red;
extern resource_t res_led_yellow;
extern resource_t res_led_all;

//#define TEMP_TIMER	CLOCK_SECOND*2
//static struct etimer et_temp;		//To get the last value stored
char shared_variable[BUFF_SIZE] = "FFFF";

PROCESS(server_COAP_master, "Server COAP and serial line interface master");
AUTOSTART_PROCESSES(&server_COAP_master);

//Server COAP master start
PROCESS_THREAD(server_COAP_master, ev, data)
{
	PROCESS_BEGIN();
	rest_init_engine();

	//Will attend the response from slave
	cc26xx_uart_init();
	cc26xx_uart_set_input(serial_line_input_byte);

#if WITH_OBSERVABLE
	res_temp.flags += IS_OBSERVABLE;
	res_volt.flags += IS_OBSERVABLE;
#endif
	rest_activate_resource(&res_temp, "sen/temp");
	//rest_activate_resource(&res_volt, "sen/volt");
	rest_activate_resource(&res_led_green, "led/green");
	rest_activate_resource(&res_led_blue, "led/blue");
	rest_activate_resource(&res_led_red, "led/red");
	rest_activate_resource(&res_led_yellow, "led/yellow");
	rest_activate_resource(&res_led_all, "led/all");

//	etimer_set(&et_temp, TEMP_TIMER);
	while(1) {
		//Waiting request..
		PROCESS_YIELD();
/*		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_temp))
		{
			leds_toggle(LEDS_GREEN);
			cc26xx_uart_write_byte(TEMP);
		}*/

		if(ev == serial_line_event_message && data != NULL){
			leds_toggle(LEDS_RED);
			strncpy(shared_variable, (char *)data, strlen(shared_variable));
			//etimer_restart(&et_temp);
		}

	}
	PROCESS_END();
}