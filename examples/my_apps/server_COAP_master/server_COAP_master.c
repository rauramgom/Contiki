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

//Receive temp/volt measure
char temp_shared_variable[TEMP_SIZE] = "FFFF";
char volt_shared_variable[VOLT_SIZE] = "FFFFF";
char aux[VOLT_SIZE] = "FFFFF";


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

	while(1) {
		//Waiting request..
		PROCESS_YIELD();

		if(ev == serial_line_event_message && data != NULL){
			//Check if data received is temp(T) or volt(V)
			strncpy(aux, (char *)data, strlen(aux));
			if(aux[0] == 'T'){
				leds_toggle(LEDS_RED);
				strncpy(temp_shared_variable, aux+sizeof(char), strlen(temp_shared_variable));
			} else if (aux[0] == 'V'){
				leds_toggle(LEDS_BLUE);
				strncpy(volt_shared_variable, aux+sizeof(char), strlen(volt_shared_variable));
			}
		}

	}
	PROCESS_END();
}