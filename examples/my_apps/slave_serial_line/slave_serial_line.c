/**
 * \file
 *         Serial line interface slave
 * \author
 *         Raul Ramirez Gomez <raulramirezgomez@gmail.com>
 *
 */
#include "contiki.h"
#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include "dev/leds.h"
#include "batmon-sensor.h"
#include "button-sensor.h"

//#include <stdio.h> 
//#include <stdlib.h>
//#include <string.h>

//DEFINES AND VARIABLES
#define END						0x0a
#define MEASURE_SIZE	2
#define VOLT_SIZE			4
#define ERROR_SIZE		3
static int measure = 0; 
static int volt = 0;
int pos = 0;

char buf_measure[MEASURE_SIZE];
char buf_volt[VOLT_SIZE];
char buf_error[ERROR_SIZE] = "ERR";

PROCESS(serial_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&serial_slave);

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
			for(pos=0; pos<strlen(buf_measure); pos++)
				cc26xx_uart_write_byte((uint8_t)buf_measure[pos]);
			cc26xx_uart_write_byte(END);
			break;
		case VOLT:
			volt = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
			sprintf(buf_volt, "%d", volt);
			for(pos=0; pos<strlen(buf_volt); pos++)
				cc26xx_uart_write_byte((uint8_t)buf_volt[pos]);
			cc26xx_uart_write_byte(END);
			break;
		case LEDS_GREEN:
			leds_toggle(LEDS_GREEN);
			break;
		case LEDS_BLUE:
			leds_toggle(LEDS_BLUE);
			break;
		case LEDS_RED:
			leds_toggle(LEDS_RED);
			break;
		case LEDS_YELLOW:
			leds_toggle(LEDS_YELLOW);
			break;
		case LEDS_ALL:
			leds_toggle(LEDS_ALL);
			break;
		default:
			//Error
			for(pos=0; pos<strlen(buf_error); pos++)
				cc26xx_uart_write_byte((uint8_t)buf_error[pos]);
			cc26xx_uart_write_byte(END);
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