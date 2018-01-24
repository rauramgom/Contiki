/*
 * Copyright (c) 2015, SICS Swedish ICT.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
/**
 * \file
 *         A RPL+TSCH node able to act as either a simple node (6ln),
 *         DAG Root (6dr) or DAG Root with security (6dr-sec)
 *         Press use button at startup to configure.
 *
 * \author Simon Duquennoy <simonduq@sics.se>
 */

/*************************************/
//        INCLUDES
/*************************************/
#include "contiki.h"
#include "sys/process.h"
#include "rest-engine.h"
#include "er-coap.h"
#include "batmon-sensor.h"
#include "button-sensor.h"
// IPv6/RPL Stack
#include "net/netstack.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"

//#include "sys/etimer.h"
#include "dev/leds.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// API FLASH
#include "api_flash.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#include "node-id.h"
#include "net/mac/tsch/tsch.h"



/*************************************/
//  DEFINES & ASSOCIATED VARIABLES
/*************************************/
//TIMERS
#define ETIMER_STORE CLOCK_SECOND*5
#define ETIMER_GET  CLOCK_SECOND*7000

static struct etimer et_store;				//To store data in Flash
static struct etimer et_get;				//To get the last value stored


//FLASH
static uint32_t pos_flash = FLASH_ADDR_START;
struct Measure temp_measure;  //Create a Measure struct to store the value

//COAP
const char *not_supported_msg = "Supported:text/plain,application/json";

/*---------------------------------------------------------------------------*/
PROCESS(node_process, "RPL Node");
AUTOSTART_PROCESSES(&node_process);

/*
* Show all the available resources
*/
static void
reading_resources_GET_handler(void *request, void *response, uint8_t *buffer,
								uint16_t preferred_size, int32_t *offset)
{
	unsigned int accept = -1;
	struct Measure measure_temp;
	int voltage;
	char measureID[4];

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	//Value of sensor is rescued
	measure_temp = read_flash(pos_flash);
	//measure_temp.measure = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);;
	voltage = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);

	if(accept == -1 || accept == REST.type.APPLICATION_JSON) {
		//Set the header content
		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		switch(measure_temp.ID)
		{
			case TEMP:
				strcpy(measureID, "TEMP");
				break;
			case VOLT:
				strcpy(measureID, "VOLT");
				break;
			default:
				strcpy(measureID, "ERRO");
				break;
		}
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
//{"info":{"ID":255,"sysUpTime":255},"temp":{"v":255,"u":"C"},"voltage":{"v":3000,"u":"mV"}}
			//"{\"info\":{\"ID\":%s,\"sysUpTime\":%lu},"
			"{\"temp\":{\"v\":%d,\"u\":\"C\"},"
			"\"voltage\":{\"v\":%d,\"u\":\"mV\"}}",
			//measureID, measure_temp.sysUpTime,
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
	printf("[%lu] APP: READING RESOURCE \n", clock_seconds());
} //End of reading_resources_GET_handler

//Creation of the associated resource. Valid to make it OBSERVABLE
// or to be activated
//  \params
//      -name, 
//      -atribute,
//      -GET function, 
//      -POST function, 
//      -PUT function, 
//      -DELETE function
RESOURCE(reading_resources,"title=\"Sensor Readings\";obs",
				reading_resources_GET_handler, NULL, NULL, NULL);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(node_process, ev, data)
{

	PROCESS_BEGIN();

	printf("My demo App started!\n");
	//Initialize the REST engine.
	rest_init_engine();
	reading_resources.flags += IS_OBSERVABLE;
	rest_activate_resource(&reading_resources, "sen/readings");
	//Activate comun sensors
	SENSORS_ACTIVATE(batmon_sensor);

	printf("[%lu] Erasing the flash for first time...\n\n", clock_seconds());
	erase_flash(&pos_flash);

	/* Print out routing tables every minute */
	etimer_set(&et_store, ETIMER_STORE);
	etimer_set(&et_get, ETIMER_GET);

	while(1) {
		PROCESS_YIELD();

		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_store))
		{
			//Save the data on Flash
			printf("[%lu]APP: Storing value ...\n", clock_seconds()); 
			//leds_toggle(LEDS_ORANGE); //blue
			leds_toggle(LEDS_YELLOW); //NARANJA 
			//leds_toggle(LEDS_RED);
			leds_toggle(LEDS_BLUE); //blue
			//Fill up every Measure struct field
			temp_measure.measure = rand()%10;//batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
			temp_measure.ID = TEMP;
			temp_measure.sysUpTime = clock_seconds();
			write_flash(temp_measure, &pos_flash);
			etimer_restart(&et_store);
		}
		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_get)) 
		{
			//Get the last value stored on the Flash
			leds_toggle(LEDS_GREEN);
			printf("[%lu]APP: Getting value ...\n", clock_seconds());
			read_flash(pos_flash);
			REST.notify_subscribers(&reading_resources);
			etimer_restart(&et_get);
		}
	}

	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/