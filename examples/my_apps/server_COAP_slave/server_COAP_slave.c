/**
 * \file
 *         Serial line interface slave
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
//#include "contiki.h"

#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"

#include "dev/leds.h"
#include "batmon-sensor.h"
#include "button-sensor.h"

#include <stdio.h> 
#include <string.h>

//Dictionary
////////////////////////
#define TEMP				'1'
#define VOLT				'2'

#define LED_GREEN_POST_ON	'3'
#define LED_GREEN_POST_OFF	'4'

#define LED_BLUE_POST_ON	'5'
#define LED_BLUE_POST_OFF	'6'

#define LED_RED_POST_ON		'7'
#define LED_RED_POST_OFF	'8'

#define LED_YELLOW_POST_ON	'9'
#define LED_YELLOW_POST_OFF	'a'

#define LED_ALL_POST_ON		'b'
#define LED_ALL_POST_OFF	'c'

#define END			0x0a
////////////////////////


#define MEASURE_SIZE	4	//Negative value + \0. ex: -20\0
#define VOLT_SIZE		5	//4 cipher number + \0. ex: 2596\0

static int temp = 0; 
static int volt = 0;

char buf_temp[MEASURE_SIZE];
char buf_volt[VOLT_SIZE];
static char buf_error[] = "ERR";

PROCESS(server_COAP_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&server_COAP_slave);

/*
* Function called to send the requested data
*/
static void send_data_uart(char* buf)
{
	for(int pos=0; pos<strlen(buf); pos++)
		cc26xx_uart_write_byte((uint8_t)buf[pos]);
	cc26xx_uart_write_byte(END);
}

/*
* The callback function is called when the slave receives 
*	a dictionary-based request
*/
static int uart_rx_callback(unsigned char c) {
	//Checkout type of data received
	switch(c) {
		case TEMP:
			temp = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
			sprintf(buf_temp, "%d", temp);
			send_data_uart(buf_temp);
			break;

		case VOLT:
			volt = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
			sprintf(buf_volt, "%d", volt);
			send_data_uart(buf_volt);
			break;

		case LED_GREEN_POST_ON:
			leds_on(LED_GREEN);
			break;

		case LED_GREEN_POST_OFF:
			leds_off(LED_GREEN);
			break;

		case LED_BLUE_POST_ON:
			leds_on(LED_BLUE);
			break;
		case LED_BLUE_POST_OFF:
			leds_off(LED_BLUE);
			break;

		case LED_RED_POST_ON:
			leds_on(LED_RED);
			break;
		case LED_RED_POST_OFF:
			leds_off(LED_RED);
			break;

		case LED_YELLOW_POST_ON:
			leds_on(LED_YELLOW);
			break;
		case LED_YELLOW_POST_OFF:
			leds_off(LED_YELLOW);
			break;

		case LED_ALL_POST_ON:
			leds_on(LED_ALL);
			break;
		case LED_ALL_POST_OFF:
			leds_off(LED_ALL);
			break;

		default:
			//Error
			leds_blink();
			send_data_uart(buf_error);
	}
	return 1;
}

PROCESS_THREAD(server_COAP_slave, ev, data)
{
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(batmon_sensor);
	cc26xx_uart_init();
	//Will receive a measure_ID based on a dictionary
	cc26xx_uart_set_input(uart_rx_callback);

	while(1) {
		//Waiting request..
	}
	SENSORS_DEACTIVATE(batmon_sensor);
	PROCESS_END();
}