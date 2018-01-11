#include "contiki.h"
#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include "dev/leds.h"
#include "batmon-sensor.h"
#include "button-sensor.h"

#include <stdio.h> /* For printf() */
#include <stdlib.h>
#include <string.h>

//DEFINES AND VARIABLES
#define END 0x0a
#define MEASURE_SIZE 2
int measure;
char buf_measure[MEASURE_SIZE]; 
int i;

PROCESS(serial_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&serial_slave);


PROCESS_THREAD(serial_slave, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(batmon_sensor);
  cc26xx_uart_init();
  //Will receive a based-on dictionary measure ID
  cc26xx_uart_set_input(serial_line_input_byte);
  //serial_line_init();
  
  printf("Serial line interface slave\n");

  while(1) {
    PROCESS_YIELD();
    if(ev == serial_line_event_message) {
      //Checkout data received
      //if(data == TEMP){
        measure = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
        sprintf(buf_measure, "%d", measure);
      
        for(i=0; i<strlen(buf_measure); i++){
          //printf("letter: %c -- ASCII: %d -- serial: ",buf_measure[i], (uint8_t)buf_measure[i]);
          cc26xx_uart_write_byte((uint8_t)buf_measure[i]);
        }
      //}
      //Send 
      cc26xx_uart_write_byte(END);
    }
  }
  SENSORS_DEACTIVATE(batmon_sensor);
  PROCESS_END();
}