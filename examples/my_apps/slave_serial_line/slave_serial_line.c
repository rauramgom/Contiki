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

//DEFINES AND VARIABLES

//Dictionary
////////////////////////
#define TEMP		'1'
#define VOLT		'2'
#define LED_GREEN	'3'
#define LED_BLUE	'4'
#define LED_RED		'5'
#define LED_YELLOW	'6'
#define LED_ALL		'7'
#define END			0x0a
////////////////////////


#define MEASURE_SIZE	2
#define VOLT_SIZE		4

static int measure = 0; 
static int volt = 0;

char buf_measure[MEASURE_SIZE];
char buf_volt[VOLT_SIZE];
static char buf_error[] = "ERR";
static char buf_ack[] = "ACK";

PROCESS(serial_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&serial_slave);

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
			measure = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
			sprintf(buf_measure, "%d", measure);
			send_data_uart(buf_measure);
			break;
		case VOLT:
			volt = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
			sprintf(buf_volt, "%d", volt);
			send_data_uart(buf_volt);
			break;
		case LED_GREEN:
			leds_toggle(LED_GREEN);
			send_data_uart(buf_ack);
			break;
		case LED_BLUE:
			leds_toggle(LED_BLUE);
			send_data_uart(buf_ack);
			break;
		case LED_RED:
			leds_toggle(LED_RED);
			send_data_uart(buf_ack);
			break;
		case LED_YELLOW:
			leds_toggle(LED_YELLOW);
			send_data_uart(buf_ack);
			break;
		case LED_ALL:
			leds_toggle(LED_ALL);
			send_data_uart(buf_ack);
			break;
		default:
			//Error
			send_data_uart(buf_error);
	}
	return 1;
}

PROCESS_THREAD(serial_slave, ev, data)
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