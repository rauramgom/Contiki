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
#include <unistd.h>

static int toggle = 0;

PROCESS(serial_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&serial_slave);

static int uart_rx_callback(unsigned char c) {
  if (toggle == 0){
    leds_toggle(LEDS_YELLOW);
    toggle++;
    cc26xx_uart_write_byte('S');
  } else if(toggle == 60){
    leds_toggle(LEDS_RED);
    toggle++;
    cc26xx_uart_write_byte('S');
  } else if(toggle < 80){
    toggle++;
    cc26xx_uart_write_byte('S');
  } else {
    toggle=0;
    cc26xx_uart_write_byte('S');
  }
  return 1;
}

PROCESS_THREAD(serial_slave, ev, data)
{
  PROCESS_BEGIN();
  cc26xx_uart_init();

  cc26xx_uart_set_input(uart_rx_callback);
  //cc26xx_uart_write_byte('M');

  while(1) {
    //PROCESS_YIELD();  
  }

  PROCESS_END();
}