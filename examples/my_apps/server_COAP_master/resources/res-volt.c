/**
 * \file
 *			COAP resource to manage VOLT sensors
 * \author
 *			Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */
#include "contiki.h"
#include "rest-engine.h"
#include <string.h>

#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include "batmon-sensor.h"
#include "button-sensor.h"
#include "api_resources.h"

/********************************************************************************/
/*
* Generic GET resource handler
*/
static void
res_GET_handler(void *request, void *response,
                    uint8_t *buffer, uint16_t preferred_size, int32_t *offset,
                    char volt)
{
	unsigned int accept = -1;
	char uart_response = NULL;
	int flag = -1;

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	//Send request to slave
	cc26xx_uart_write_byte(volt);

	//Receive the response
//[ ******
	uart_response = receive(); //--------------------------------------------------FALTA!
// ***** ]
	if(accept == -1 || accept == REST.type.APPLICATION_JSON) {
		//Set the header content
		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
			"{\"Volt\":{\"v\":%s,\"u\":\"mV\"}}",
			uart_response);
		//Set the payload content 
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else if(accept == REST.type.TEXT_PLAIN && (flag == ON || flag == OFF)) {
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Volt=%smV",
			uart_response);
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of res_GET_handler
/********************************************************************************/

/********************************************************************************/
static void
volt_GET(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    VOLT);
}//End of volt_GET

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
RESOURCE(res_volt,"title=\"Volt\"",
				volt_GET, NULL, NULL, NULL);
/********************************************************************************/