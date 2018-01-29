/**
 * \file
 *			COAP resource to manage VOLT sensors
 * \author
 *			Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */

#include "api_resources.h"

static void volt_GET(void *request, void *response, uint8_t *buffer,
						uint16_t preferred_size, int32_t *offset);

static void res_periodic_handler(void);

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
PERIODIC_RESOURCE(res_volt,
					"title=\"Volt\"",
					volt_GET,
					NULL,
					NULL,
					NULL,
					OBSERVER_TIMER,
					res_periodic_handler);

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
	char uart_response[BUFF_SIZE];

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	//Send request to slave
	cc26xx_uart_write_byte(volt);

	//Receive the response
	strncpy(uart_response, shared_variable, sizeof(uart_response));

	if(accept == -1 || accept == REST.type.APPLICATION_JSON) {
		strncpy(volt_old, uart_response, sizeof(volt_old));

		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
			"{\"Volt\":{\"v\":%s,\"u\":\"mV\"}}",
			uart_response);
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else if(accept == REST.type.TEXT_PLAIN) {
		strncpy(volt_old, uart_response, sizeof(volt_old));
		
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

/*
* Handler function for the PERIODIC_RESOURCE
*/
static void
res_periodic_handler()
{
	char aux_val[BUFF_SIZE];
	//Send request to slave
	cc26xx_uart_write_byte(VOLT);
	strncpy(aux_val, shared_variable, sizeof(aux_val));

	if(strncmp(aux_val, volt_old, sizeof(aux_val)) != 0){
		strncpy(volt_old, aux_val, sizeof(volt_old));
		REST.notify_subscribers(&res_volt);
	}

}//End of res_periodic_handler
/********************************************************************************/

/********************************************************************************/
static void
volt_GET(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    VOLT);
}//End of volt_GET
/********************************************************************************/