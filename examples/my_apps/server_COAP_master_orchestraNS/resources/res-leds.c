/**
 * \file
 *			COAP resource to manage LEDs
 * \author
 *			Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 */
#include "api_resources.h"

/********************************************************************************/
/*
* Generic POST resource handler for power on/off any led
*/
static void
res_POST_handler(void *request, void *response,
                    uint8_t *buffer, uint16_t preferred_size, int32_t *offset,
                    char led_on, char led_off)
{
	const char *mode = NULL;
	size_t len = 0;

	//cc26xx_uart_write_byte(led_on);
	len = REST.get_post_variable(request, "mode", &mode);
	if(strncmp(mode, "on", len) == 0){

		cc26xx_uart_write_byte(led_on);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "%s", "Led on");
		REST.set_response_payload(response, (uint8_t*)buffer,
			strlen((char *)buffer));

	} else if(strncmp(mode, "off", len) == 0) {

		cc26xx_uart_write_byte(led_off);
		snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "%s", "Led off");
		REST.set_response_payload(response, (uint8_t*)buffer,
			strlen((char *)buffer));

	} else {
		//ERROR
		REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
		REST.set_response_payload(response, not_supported_msg,
			strlen(not_supported_msg));
	}
}//End of res_POST_handler
/********************************************************************************/

/********************************************************************************/
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
RESOURCE(res_led_green,"title=\"led green\"",
			NULL,led_POST_green, led_POST_green, NULL);
/********************************************************************************/

/********************************************************************************/
static void
led_POST_blue(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_BLUE_POST_ON, LED_BLUE_POST_OFF);
}//End of led_POST_blue

//Creation of the associated resource.
RESOURCE(res_led_blue,"title=\"led blue\"",
			NULL, led_POST_blue, led_POST_blue, NULL);
/********************************************************************************/

/********************************************************************************/
static void
led_POST_red(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_RED_POST_ON, LED_RED_POST_OFF);
}//End of led_POST_red

//Creation of the associated resource. 
RESOURCE(res_led_red,"title=\"led red\"",
			NULL, led_POST_red, led_POST_red, NULL);
/********************************************************************************/

/********************************************************************************/
static void
led_POST_yellow(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_YELLOW_POST_ON, LED_YELLOW_POST_OFF);
}//End of led_POST_yellow

//Creation of the associated resource.
RESOURCE(res_led_yellow,"title=\"led yellow\"",
			NULL, led_POST_yellow, led_POST_yellow, NULL);
/********************************************************************************/

/********************************************************************************/
static void
led_POST_all(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_POST_handler(request, response, buffer, preferred_size, offset,
                    LED_ALL_POST_ON, LED_ALL_POST_OFF);
}//End of led_POST_all

//Creation of the associated resource.
RESOURCE(res_led_all,"title=\"leds all\"",
			NULL, led_POST_all, led_POST_all, NULL);
/********************************************************************************/