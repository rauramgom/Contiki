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

/********************************************************************************/
/*
* Generic GET resource handler for any led
*/
static void
res_GET_handler(void *request, void *response,
                    uint8_t *buffer, uint16_t preferred_size, int32_t *offset,
                    char led_get, char *color)
{
	unsigned int accept = -1;
	char *uart_response = NULL;
	int flag = -1;

	if(request != NULL) {
		REST.get_header_accept(request, &accept);
	}

	//Send request to slave
	cc26xx_uart_write_byte(led_get);

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
			"{\"Led\":{\"%s\":%s}}",
			color, (flag==ON)?"on":"off");
		//Set the payload content 
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else if(accept == REST.type.TEXT_PLAIN && (flag == ON || flag == OFF)) {
		REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Led %s=%s",
			color, (flag==ON)?"on":"off");
		REST.set_response_payload(response, buffer, strlen((char *)buffer));
	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of res_GET_handler

/*
* Generic POST resource handler for any led
*/
static void
res_POST_handler(void *request, void *response,
                    uint8_t *buffer, uint16_t preferred_size, int32_t *offset,
                    char led_on, char led_off)
{
	char *mode = NULL;

	REST.get_post_variable(request, "mode", &mode);
	if(strncmp(mode, "on", sizeof("on")) == 0)
		cc26xx_uart_write_byte(led_on);
	else if(strncmp(mode, "off", sizeof("off")) == 0)
		cc26xx_uart_write_byte(led_off);
	else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of res_POST_handler
/********************************************************************************/

/********************************************************************************/
/*
* Get the status of the green led
*/
static void
led_GET_green(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    LED_GREEN_GET, "Green");
}//End of led_GET_green

/*
* Post a request to power on/off green led
*/
static void
led_POST_green(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_GREEN_POST_ON, LED_GREEN_POST_OFF);
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
/********************************************************************************/

/********************************************************************************/
/*
* Get the status of the blue led
*/
static void
led_GET_blue(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    LED_BLUE_GET, "Blue");
}//End of led_GET_blue

/*
* Post a request to power on/off blue led
*/
static void
led_POST_blue(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_BLUE_POST_ON, LED_BLUE_POST_OFF);
}//End of led_POST_blue

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
RESOURCE(led_blue,"title=\"led\";blue",
				led_GET_blue, led_POST_blue, NULL, NULL);
/********************************************************************************/

/********************************************************************************/
/*
* Get the status of the red led
*/
static void
led_GET_red(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    LED_RED_GET, "Red");
}//End of led_GET_red

/*
* Post a request to power on/off red led
*/
static void
led_POST_red(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_RED_POST_ON, LED_RED_POST_OFF);
}//End of led_POST_red

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
RESOURCE(led_red,"title=\"led\";red",
				led_GET_red, led_POST_red, NULL, NULL);
/********************************************************************************/

/********************************************************************************/
/*
* Get the status of the yellow led
*/
static void
led_GET_yellow(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    LED_YELLOW_GET, "Yellow");
}//End of led_GET_yellow

/*
* Post a request to power on/off yellow led
*/
static void
led_POST_yellow(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_YELLOW_POST_ON, LED_YELLOW_POST_OFF);
}//End of led_POST_yellow

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
RESOURCE(led_yellow,"title=\"led\";yellow",
				led_GET_yellow, led_POST_yellow, NULL, NULL);
/********************************************************************************/

/********************************************************************************/
/*
* Get the status of all leds
*/
static void
led_GET_all(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    LED_ALL_GET, "All");
}//End of led_GET_all

/*
* Post a request to power on/off all leds
*/
static void
led_POST_all(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_ALL_POST_ON, LED_ALL_POST_OFF);
}//End of led_POST_all

//Creation of the associated resource. Valid to make it OBSERVABLE or be activated
//	\params
//			-name, 
//			-atribute,
//			-GET function, 
//			-POST function, 
//			-PUT function, 
//			-DELETE function
RESOURCE(led_all,"title=\"led\";all",
				led_GET_all, led_POST_all, NULL, NULL);
/********************************************************************************/