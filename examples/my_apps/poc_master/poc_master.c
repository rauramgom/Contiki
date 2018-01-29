/**
 * \file
 *         Server COAP and serial line interface master
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
#include "resources/api_resources.h"

/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern resource_t res_led_green;
extern resource_t res_led_red;

PROCESS(server_COAP_master, "Server COAP and serial line interface master");
AUTOSTART_PROCESSES(&server_COAP_master);


PROCESS_THREAD(server_COAP_master, ev, data)
{
	PROCESS_BEGIN();
 	printf("Server COAP master started!\n");
	rest_init_engine();

	rest_activate_resource(&res_led_green, "led/green");
	rest_activate_resource(&res_led_red, "led/red");

	//cc26xx_uart_init();

	while(1) {
		//Waiting request..
	}
	PROCESS_END();
}