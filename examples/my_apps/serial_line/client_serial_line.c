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

#include <stdio.h> 
//#include <stdlib.h>
#include <string.h>

//DEFINES AND VARIABLES
#define END 0x0a
#define MEASURE_SIZE 2
#define ERROR_SIZE 5
int measure; 
int pos;
char buf_measure[MEASURE_SIZE];
char buf_error[ERROR_SIZE] = "ERR";

PROCESS(serial_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&serial_slave);


PROCESS_THREAD(serial_slave, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(batmon_sensor);
  cc26xx_uart_init();
  //Will receive a based-on dictionary measure ID
  cc26xx_uart_set_input(serial_line_input_byte);
  cc26xx_uart_write_byte('a');
  //printf("Serial line interface slave\n");

  while(1) {
    PROCESS_YIELD();
    if(ev == serial_line_event_message) {
      leds_toggle(LEDS_GREEN);
      leds_toggle(LEDS_BLUE);
      cc26xx_uart_write_byte('a');
      //Checkout type of data received
      //if(data == TEMP){
        //measure = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP); **
        //sprintf(buf_measure, "%d", measure); **
        //for(pos=0; pos<strlen(buf_measure); pos++) **
          //cc26xx_uart_write_byte((uint8_t)buf_measure[pos]); **
        //Send EOF character to be correctly understanded by serial_line
        //cc26xx_uart_write_byte(END); **
      /*} else if (data == VOLT) {
          [CONTINUAR...]
      }*/
      
      /*
      switch(data) {
        case TEMP
          measure = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
          sprintf(buf_measure, "%d", measure);
          for(pos=0; pos<strlen(buf_measure); pos++)
            cc26xx_uart_write_byte((uint8_t)buf_measure[pos]);
          cc26xx_uart_write_byte(END);
          break;
        case VOLT
          [FALTA]
          break;
        case LEDS_GREEN
          leds_toggle(LEDS_GREEN);
          break;
        case LEDS_BLUE
          leds_toggle(LEDS_BLUE);
          break;
        case LEDS_RED
          leds_toggle(LEDS_RED);
          break;
        case LEDS_YELLOW
          leds_toggle(LEDS_YELLOW);
          break;
        case LEDS_ALL
          leds_toggle(LEDS_ALL);
          break;
        default:
          for(pos=0; pos<strlen(buf_error); pos++)
            cc26xx_uart_write_byte((uint8_t)buf_error[pos]);
          cc26xx_uart_write_byte(END);
      }
      */      
    }
  }
  SENSORS_DEACTIVATE(batmon_sensor);
  PROCESS_END();
}