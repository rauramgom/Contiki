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
res_GET_handler(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset,
                char led_on)
{
	leds_toggle(LEDS_GREEN);
	//cc26xx_uart_write_byte(led_on);
	REST.set_response_payload(response, &led_on,
			sizeof(char));
}//End of res_GET_handler
/********************************************************************************/




/********************************************************************************/
static void
led_GET_green(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    LED_GREEN_POST_ON);
}

RESOURCE(res_led_green,"title=\"led green\"",
				led_GET_green, NULL, NULL, NULL);
/********************************************************************************/

/********************************************************************************/
static void
led_GET_red(void *request, void *response, uint8_t *buffer,
				uint16_t preferred_size, int32_t *offset)
{
	res_GET_handler(request, response, buffer, preferred_size, offset,
                    LED_RED_POST_ON);
}

RESOURCE(res_led_red,"title=\"led red\"",
				led_GET_red, NULL, NULL, NULL);
/********************************************************************************/