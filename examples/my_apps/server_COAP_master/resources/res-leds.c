/**
 * \file
 *			COAP resource to manage LEDs
 * \author
 *			Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */
#include "contiki.h"
#include "rest-engine.h"
#include "dev/leds.h"
#include <string.h>

#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include "api_resources.h"


/*
* Get the status of the green led
*/
static void
led_GET_green(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	unsigned int accept = -1;
	char *uart_response = NULL;
	int flag = -1;

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	//Send request to slave
	cc26xx_uart_write_byte(LED_GREEN_GET);

	//Receive the response
//[ ******
	uart_response = receive(); //--------------------------------------------------FALTA!
// ***** ]

	if(strncmp(uart_response, LED_OFF, sizeof(LED_OFF)) == 0)
		flag = OFF;
	else if(strncmp(uart_response, LED_ON, sizeof(LED_ON)) == 0)
		flag = ON;

	if((accept == -1 || accept == REST.type.APPLICATION_JSON) && 
		(flag == ON || flag == OFF)) {
		//Set the header content
		REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE,
			"{\"Led\":{\"Green\":%d}}",
			flag);
		//Set the payload content 
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else if(accept == REST.type.TEXT_PLAIN && (flag == ON || flag == OFF)) {
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Led Green=%d",
			flag);
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of led_GET_green

/*
* Post a request to on/off green led
*/
static void
led_POST_green(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	char *mode = NULL;

	REST.get_post_variable(request, "mode", &mode);
	if(strncmp(mode, "on", sizeof("on")) == 0){
		cc26xx_uart_write_byte(LED_GREEN_POST_ON);
	} else if(strncmp(mode, "off", sizeof("off")) == 0){
		cc26xx_uart_write_byte(LED_GREEN_POST_OFF);
	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of led_POST_green

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
RESOURCE(led_green,"title=\"led\";green",
				led_GET_green, led_POST_green, NULL, NULL);