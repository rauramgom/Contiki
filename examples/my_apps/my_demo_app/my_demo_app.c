/******************************************************************************
*	Filename:		my_demo_app.c
*	Revised:			2017-10-19 09:09:02 (Thu, 19 Oct 2017)
*	Author:			Ramirez Gomez, Raul (raulramirezgomez@gmail.com)
*
*	Description:	Demo de una aplicación práctica.
						 Describir el funcionamiento...
******************************************************************************/

/*************************************/
//				INCLUDES
/*************************************/
#include "contiki.h"
#include "sys/process.h"
#include "rest-engine.h"
//#include "er-coap.h"
#include "batmon-sensor.h"
// IPv6/RPL Stack
#include "net/netstack.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"

#include "sys/etimer.h"
#include "dev/leds.h"
#include "button-sensor.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// API FLASH
#include "api_flash.h"


/*
#include "sys/stimer.h"

#include "dev/watchdog.h"

#include "board-peripherals.h"

#include "ti-lib.h"

	#include "contiki-lib.h"
	#include "contiki-net.h"
#include "net/ip/resolv.h" 
*/

// COMMON RESOURCES
/*extern resource_t res_leds;
extern resource_t res_toggle_red;
extern resource_t res_toggle_green;
extern resource_t res_toggle_orange;
extern resource_t res_toggle_yellow;
extern resource_t res_device_uptime;
*/

/*************************************/
//	DEFINES & ASSOCIATED VARIABLES
/*************************************/
//TIMERS
#define ETIMER_SAVE	CLOCK_SECOND*5
#define ETIMER_GET	CLOCK_SECOND*7
static struct etimer et_store;	//To store data in Flash
static struct etimer et_get;		//To get the last value stored

//FLASH
static uint32_t pos_flash = FLASH_ADDR_START;
struct Measure temp_measure;	//Create a Measure struct to store the value

//COAP
const char *not_supported_msg = "Supported:text/plain,application/json";


PROCESS(my_demo_app, "MY DEMO APP");
AUTOSTART_PROCESSES(&my_demo_app);


/*************************************/
//		FUNCTIONS DEFINITION
/*************************************/
/*
* Función que mostrará todos los recursos disponibles
*/
static void
reading_resources_GET_handler(void *request, void *response, uint8_t *buffer,
								uint16_t preferred_size, int32_t *offset)
{
	unsigned int accept = -1;
	struct Measure measure_temp;
	int voltage;

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	//Value of sensor is rescued
	//measure_temp = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
	measure_temp = read_flash(pos_flash);
	voltage = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);

	if(accept == -1 || accept == REST.type.APPLICATION_JSON) {
		//Set the header content
		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
			"{\"temp\":{\"v\":%d,\"u\":\"C\"},"
			"\"voltage\":{\"v\":%d,\"u\":\"mV\"}}",
			measure_temp.measure, (voltage * 125) >> 5);

		//Set the payload content 
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else if(accept == REST.type.TEXT_PLAIN) {
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Temp=%dC, Voltage=%dmV",
			measure_temp.measure, (voltage * 125) >> 5);

		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}

} //End of reading_resources_GET_handler

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
RESOURCE(reading_resources,"title=\"Sensor Readings\";obs",
				reading_resources_GET_handler, NULL, NULL, NULL);


/*************************************/
//			APP START
/*************************************/
PROCESS_THREAD(my_demo_app, ev, data)
{
	PROCESS_BEGIN();

	printf("My demo App started!\n");

	//Initialize the REST engine. 
	rest_init_engine();
	//Activate comun sensors
	SENSORS_ACTIVATE(batmon_sensor);
	//Activate the created resources
	reading_resources.flags += IS_OBSERVABLE;
	rest_activate_resource(&reading_resources, "sen/readings");
/*	rest_activate_resource(&res_toggle_green, "lt/g");
	rest_activate_resource(&res_toggle_red, "lt/r");
	rest_activate_resource(&res_toggle_yellow, "lt/y");
	rest_activate_resource(&res_toggle_orange, "lt/o");
	rest_activate_resource(&res_leds, "lt");
	rest_activate_resource(&res_device_uptime, "dev/uptime");
*/
	printf("Erasing the flash for first time...\n\n");
	erase_flash(&pos_flash);

	etimer_set(&et_store, ETIMER_SAVE);
	etimer_set(&et_get, ETIMER_GET);
	while(1){
		PROCESS_YIELD();
		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_get))
		{
			//Get the last value stored on the Flash
			leds_toggle(LEDS_GREEN);
			read_flash(pos_flash);
			REST.notify_subscribers(&reading_resources);
			etimer_restart(&et_get);
		}

		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_store))
		{
			//Save the data on Flash
			leds_toggle(LEDS_RED);
			printf("Storing value ...\n");
			//Fill up every Measure struct field
			temp_measure.measure = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
			temp_measure.ID = TEMP;
			temp_measure.sysUpTime = clock_seconds();
			write_flash(temp_measure, &pos_flash);
			etimer_restart(&et_store);
		}
	}

	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
} //End of PROCESS_THREAD