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
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//static int toggle = 0;
#define TEMP  '3'
#define VOLT  '7'
  
PROCESS(serial_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&serial_slave);

static int uart_rx_callback(unsigned char c) {
  switch((char)c) {
    case TEMP:
      leds_toggle(LEDS_GREEN);
      break;
    case VOLT:
      leds_toggle(LEDS_BLUE);
      break;
  }
/*
  if (toggle == 1 || c=='T'){
    leds_toggle(LEDS_GREEN);
    toggle++;
    //cc26xx_uart_write_byte('S');
  } else if(toggle == 5 || c=='S'){
    leds_toggle(LEDS_BLUE);
    toggle=0;
    //cc26xx_uart_write_byte('S');
  } else if(toggle < 8){
    toggle++;
    //cc26xx_uart_write_byte('S');
  } else {
    toggle=0;
    //cc26xx_uart_write_byte('S');
  }*/
  return 1;
}

PROCESS_THREAD(serial_slave, ev, data)
{
  PROCESS_BEGIN();
  cc26xx_uart_init();
  //serial_line_init();
  //cc26xx_uart_set_input(serial_line_input_byte);
  cc26xx_uart_set_input(uart_rx_callback);

  while(1) {
//    PROCESS_YIELD();
  }

  PROCESS_END();
}