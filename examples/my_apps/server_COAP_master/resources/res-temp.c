/**
 * \file
 *			COAP resource to manage TEMP sensors
 * \author
 *			Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */

#include "api_resources.h"

//static void temp_GET(void *request, void *response, uint8_t *buffer,
//						uint16_t preferred_size, int32_t *offset);

//static void res_periodic_handler(void);

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
/*PERIODIC_RESOURCE(res_temp,
					"title=\"Temp\"",
					temp_GET, 
					NULL,
					NULL,
					NULL,
					OBSERVER_TIMER,
					res_periodic_handler);*/

/********************************************************************************/
/*
* Generic GET resource handler
*/
/*static void
res_GET_handler(void *request, void *response,
                    uint8_t *buffer, uint16_t preferred_size, int32_t *offset,
                    char temp)
{
	unsigned int accept = -1;
	char uart_response[BUFF_SIZE];

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	//Send request to slave
	cc26xx_uart_write_byte(temp);

	//Receive the response
	strncpy(uart_response, shared_variable, sizeof(uart_response));

	if(accept == -1 || accept == REST.type.APPLICATION_JSON) {
		strncpy(temp_old, uart_response, sizeof(temp_old));

		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
			"{\"Temp\":{\"v\":%s,\"u\":\"C\"}}",
			(uart_response!=NULL)?uart_response:"NaN");
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else if(accept == REST.type.TEXT_PLAIN) {
		strncpy(temp_old, uart_response, sizeof(temp_old));
		
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Temp=%sC",
			(uart_response!=NULL)?uart_response:"NaN");
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of res_GET_handler*/

/*
* Handler function for the PERIODIC_RESOURCE
*/
/*static void
res_periodic_handler()
{
	char aux_val[BUFF_SIZE];
	//Send request to slave
	cc26xx_uart_write_byte(TEMP);
	strncpy(aux_val, shared_variable, sizeof(aux_val));

	if(strncmp(aux_val, temp_old, sizeof(aux_val)) != 0){
		strncpy(temp_old, aux_val, sizeof(temp_old));
		REST.notify_subscribers(&res_temp);
	}

}//End of res_periodic_handler*/
/********************************************************************************/

/********************************************************************************/
static void
temp_GET(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	//res_GET_handler(request, response, buffer, preferred_size, offset,
    //                TEMP);
	measure_event_message = process_alloc_event();
	
	Res_handler temp_handler = {request, response, buffer, preferred_size, offset, TEMP};
	process_post(PROCESS_BROADCAST, measure_event_message, &temp_handler);
}//End of temp_GET

RESOURCE(res_temp,"title=\"Temp\"",
			temp_GET, NULL, NULL, NULL);
/********************************************************************************/