/**
 * \file
 *			COAP resource to manage remote VOLT sensors
 * \author
 *			Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */

#include "api_resources.h"

static void volt_periodic_GET(void *request, void *response, uint8_t *buffer,
						uint16_t preferred_size, int32_t *offset);

static void volt_periodic_handler(void);

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
//			-period,
//			-function_handler
PERIODIC_RESOURCE(res_periodic_volt,
					"title=\"Volt\"",
					volt_periodic_GET,
					NULL,
					NULL,
					NULL,
					VOLT_PER_TIMER,
					volt_periodic_handler);

/********************************************************************************/
/*
* Handler function for PERIODIC_RESOURCE
*/
static void
volt_periodic_handler()
{
	REST.notify_subscribers(&res_periodic_volt);
	cc26xx_uart_write_byte(VOLT);
}//End of volt_periodic_handler


static void
volt_periodic_GET(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	unsigned int accept = 100;

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	if(accept == 100 || accept == REST.type.APPLICATION_JSON) {
		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
			"{\"Volt\":{\"v\":%s,\"u\":\"mV\"}}",
			(strncmp(volt_shared, "FFFF", VOLT_SIZE-1)!=0)?volt_shared:"\"NaN\"");
		REST.set_response_payload(response, buffer, strlen((char *)buffer));

	} else if(accept == REST.type.TEXT_PLAIN) {
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Volt=%smV",
			(strncmp(volt_shared, "FFFF", VOLT_SIZE-1)!=0)?volt_shared:"\"NaN\"");
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
		
	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of volt_periodic_GET
/********************************************************************************/