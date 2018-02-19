/**
 * \file
 *			COAP resource to manage local TEMP sensors
 * \author
 *			Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */

#include "api_resources.h"
#include "batmon-sensor.h"
#include "button-sensor.h"

static void temp_GET(void *request, void *response, uint8_t *buffer,
						uint16_t preferred_size, int32_t *offset);

static void temp_periodic_handler(void);

//Notify subscribers
char temp_old[TEMP_SIZE] = "FFF";

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function,
//			-period,
//			-function_handler
PERIODIC_RESOURCE(res_temp,
					"title=\"Temp\"",
					temp_GET, 
					NULL,
					NULL,
					NULL,
					TEMP_TIMER,
					temp_periodic_handler);

/********************************************************************************/
/*
* Handler function for the PERIODIC_RESOURCE
*/
static void
temp_periodic_handler()
{
	int temp;
	char temp_aux[TEMP_SIZE];

	temp = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
	memset(temp_aux, '\0', TEMP_SIZE);
	sprintf(temp_aux, "%d", temp);
	if(strcmp(temp_old, temp_aux) != 0){
		memset(temp_old, '\0', TEMP_SIZE);
		strncpy(temp_old, temp_aux, strlen(temp_aux));
		REST.notify_subscribers(&res_temp);
	}
}//End of temp_periodic_handler


static void
temp_GET(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	unsigned int accept = 100;

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	if(accept == 100 || accept == REST.type.APPLICATION_JSON) {
		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
			"{\"Temp\":{\"v\":%s,\"u\":\"C\"}}",
			(strncmp(temp_old, "FFF", TEMP_SIZE-1)!=0)?temp_old:"\"NaN\"");
		REST.set_response_payload(response, buffer, strlen((char *)buffer));

	} else if(accept == REST.type.TEXT_PLAIN) {
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Temp=%sC",
			(strncmp(temp_old, "FFF", TEMP_SIZE-1)!=0)?temp_old:"\"NaN\"");
		REST.set_response_payload(response, buffer, strlen((char *)buffer));

	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}

}//End of temp_GET
/********************************************************************************/