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
//#include "batmon-sensor.h"
//#include "button-sensor.h"

//#include "sys/etimer.h"

#include <stdio.h> 
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int toggle = 0;

#define TEMP  '3'
#define VOLT  '7'
#define ETIMER_GET  CLOCK_SECOND*3
static struct etimer et;
static struct etimer et2;

PROCESS(serial_slave, "Serial line interface slave");
AUTOSTART_PROCESSES(&serial_slave);

static int uart_rx_callback(unsigned char c) {
  if (toggle == 10){
    leds_toggle(LEDS_ALL);
    toggle++;
    //cc26xx_uart_write_byte('M');
  } else if(toggle == 70){
    leds_toggle(LEDS_YELLOW);
    toggle++;
    //cc26xx_uart_write_byte('M');
  } else if(toggle < 150){
    toggle++;
    //cc26xx_uart_write_byte('M');
  } else {
    toggle=0;
    //cc26xx_uart_write_byte('M');
  }
  return 1;
}

PROCESS_THREAD(serial_slave, ev, data)
{
  PROCESS_BEGIN();
  cc26xx_uart_init();
  //serial_line_init();
  //cc26xx_uart_set_input(serial_line_input_byte);
  cc26xx_uart_set_input(uart_rx_callback);
  //cc26xx_uart_write_byte('M');

  etimer_set(&et, ETIMER_GET);
  etimer_set(&et2, ETIMER_GET*73/30);
  while(1) {
    PROCESS_YIELD(); 
    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et))
    {
      cc26xx_uart_write_byte(TEMP);
      etimer_restart(&et);
      leds_toggle(LEDS_GREEN);
    }
    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et2))
    {
      cc26xx_uart_write_byte(VOLT);
      etimer_restart(&et2);
      leds_toggle(LEDS_BLUE);
    }
  }

  PROCESS_END();
}