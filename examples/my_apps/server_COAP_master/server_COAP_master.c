/**
 * \file
 *         Server COAP and serial line interface master
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
#include "resources/api_resources.h"

//static char buf_rx_uart[BUFF_SIZE];		//BUFF_SIZE	5
//char shared_variable[BUFF_SIZE] = "";	//necessary initiation

/*
 * Resources will be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern resource_t res_temp;
//extern resource_t res_volt;
extern resource_t res_led_green;
extern resource_t res_led_blue;
extern resource_t res_led_red;
extern resource_t res_led_yellow;
extern resource_t res_led_all;

/***/
unsigned int accept = -1;
static Res_handler *str_resource;

PROCESS(server_COAP_master, "Server COAP and serial line interface master");
AUTOSTART_PROCESSES(&server_COAP_master);

/*
* The callback function called when the master receives a response
*/
/*
static int uart_rx_callback(unsigned char c) {
	int pos = 0;
	//Fill up the buffer
	for(pos=0; (pos<sizeof(buf_rx_uart))&&(c != END); pos++)
	{
		buf_rx_uart[pos] = c;
	}
	if (pos<sizeof(buf_rx_uart) && (c == END))
		buf_rx_uart[pos] = '\0';
	
	//Copy the buffer to the global variable used by the resource
	strncpy(shared_variable, buf_rx_uart, sizeof(buf_rx_uart));

	return 1;
}*/

//Server COAP master start
PROCESS_THREAD(server_COAP_master, ev, data)
{
	PROCESS_BEGIN();
	rest_init_engine();
	//SENSORS_ACTIVATE(batmon_sensor);
	cc26xx_uart_init();
	//Will attend the response from slave
	cc26xx_uart_set_input(serial_line_input_byte);
	//cc26xx_uart_set_input(uart_rx_callback);

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

		if(ev == measure_event_message && data != NULL){
			if(((Res_handler *) data)->res_request != NULL) {
				REST.get_header_accept(((Res_handler *) data)->res_request, &accept);
			}
			str_resource = (Res_handler *)data;
		}

		if(ev == serial_line_event_message && str_resource != NULL){
			leds_toggle(LEDS_ALL);
			if(accept == -1 || accept == REST.type.APPLICATION_JSON) {
				//strncpy(temp_old, data, sizeof(temp_old));

				REST.set_header_content_type(str_resource->res_response, REST.type.APPLICATION_JSON);
				snprintf((char *)str_resource->res_buffer, REST_MAX_CHUNK_SIZE,
					"{\"Temp\":{\"v\":%s,\"u\":\"C\"}}",
					(data!=NULL)?(char *)data:"NaN");
				REST.set_response_payload(str_resource->res_response,
					str_resource->res_buffer, strlen((char *)str_resource->res_buffer));
			} else if(accept == REST.type.TEXT_PLAIN) {
				//strncpy(temp_old, data, sizeof(temp_old));
		
				REST.set_header_content_type(str_resource->res_response, REST.type.TEXT_PLAIN);
				snprintf((char *)str_resource->res_buffer, REST_MAX_CHUNK_SIZE, "Temp=%sC",
					(data!=NULL)?(char *)data:"NaN");
				REST.set_response_payload(str_resource->res_response,
					str_resource->res_buffer, strlen((char *)str_resource->res_buffer));
			} else {
				//ERROR
				REST.set_response_status(str_resource->res_response, REST.status.NOT_ACCEPTABLE);
				REST.set_response_payload(str_resource->res_response, not_supported_msg,
					strlen(not_supported_msg));
			}
			str_resource = NULL;
		}

	}
	//SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}