/**
 * \file
 *         Server COAP and serial line interface master
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
#include "resources/api_resources.h"

/*
 * Resources will be activated need to be imported.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern resource_t res_local_temp;
extern resource_t res_remote_temp;

extern resource_t res_local_volt;
extern resource_t res_remote_volt;

extern resource_t res_led_green;
extern resource_t res_led_blue;
extern resource_t res_led_red;
extern resource_t res_led_yellow;
extern resource_t res_led_all;

//Receive temp/volt measure
char temp_shared[TEMP_SIZE] = "FFF";
char volt_shared[VOLT_SIZE] = "FFFF";
char aux[AUX_SIZE] = "FFFFF";

PROCESS(server_COAP_master, "Server COAP and serial line interface master");
AUTOSTART_PROCESSES(&server_COAP_master);

//Server COAP master start
PROCESS_THREAD(server_COAP_master, ev, data)
{
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(batmon_sensor);
	rest_init_engine();

	//Will attend the response from slave
	cc26xx_uart_init();
	cc26xx_uart_set_input(serial_line_input_byte);

	rest_activate_resource(&res_remote_temp, "sen/remote temp");
	rest_activate_resource(&res_remote_volt, "sen/remote volt");
	rest_activate_resource(&res_local_temp, "sen/local temp");
	rest_activate_resource(&res_local_volt, "sen/local volt");
	
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
			memset(aux, '\0', AUX_SIZE);
			strncpy(aux, (char *)data, AUX_SIZE-1);
			if(aux[0] == 'T'){
				memset(temp_shared, '\0', TEMP_SIZE);
				strncpy(temp_shared, aux+sizeof(char), TEMP_SIZE-1);
			} else if (aux[0] == 'V'){
				memset(volt_shared, '\0', VOLT_SIZE);
				strncpy(volt_shared, aux+sizeof(char), VOLT_SIZE-1);
			}
		}

	}
	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}