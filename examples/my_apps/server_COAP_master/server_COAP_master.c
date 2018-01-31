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
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
//extern resource_t res_temp;
//extern resource_t res_volt;
extern resource_t res_led_green;
//extern resource_t res_led_blue;
extern resource_t res_led_red;
//extern resource_t res_led_yellow;
//extern resource_t res_led_all;

//static struct etimer et_get;
//#define OBSERVER_TIMER	CLOCK_SECOND*7

PROCESS(server_COAP_master, "Server COAP and serial line interface master");
AUTOSTART_PROCESSES(&server_COAP_master);

/*
* The callback function is called when the master receives the response
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

PROCESS_THREAD(server_COAP_master, ev, data)
{
	PROCESS_BEGIN();
 	printf("Server COAP master started!\n");
	rest_init_engine();
	//SENSORS_ACTIVATE(batmon_sensor);
	cc26xx_uart_init();
	//Will attend the response from slave
	//cc26xx_uart_set_input(uart_rx_callback);

#if WITH_OBSERVABLE
	res_temp.flags += IS_OBSERVABLE;
	res_volt.flags += IS_OBSERVABLE;
#endif
	//rest_activate_resource(&res_temp, "sen/temp");
	//rest_activate_resource(&res_volt, "sen/volt");
	rest_activate_resource(&res_led_green, "led/green");
	//rest_activate_resource(&res_led_blue, "led/blue");
	rest_activate_resource(&res_led_red, "led/red");
	//rest_activate_resource(&res_led_yellow, "led/yellow");
	//rest_activate_resource(&res_led_all, "led/all");

	//etimer_set(&et_get, OBSERVER_TIMER);
	while(1) {
		//Waiting request..
		
		PROCESS_YIELD();
		/*if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_get))
		{
			REST.notify_subscribers(&res_temp);
			REST.notify_subscribers(&res_volt);
			etimer_restart(&et_get);
		}*/
	}
	//SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}