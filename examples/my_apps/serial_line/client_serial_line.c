#include "contiki.h"
#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include "dev/leds.h"

#include <stdio.h> /* For printf() */
#include <string.h>

PROCESS(test_serial, "Serial line test process");
AUTOSTART_PROCESSES(&test_serial);

static int uart_rx_callback(unsigned char c) {
  //uint8_t u;
  printf("\nReceived %c",c);
  //u = (uint8_t)c;
  //printf("\nReceived %u",u);

  return 1;
}

PROCESS_THREAD(test_serial, ev, data)
{
  PROCESS_BEGIN();
  //cc26xx_uart_init();
  serial_line_init();
  cc26xx_uart_set_input(serial_line_input_byte);
  //cc26xx_uart_set_input(uart_rx_callback);
  
  
  printf("Serial line interface\n");
  cc26xx_uart_write_byte('r');

  while(1) {
    PROCESS_YIELD();
    if(ev == serial_line_event_message) {
      printf("received line: %s\n", (char *)data);
      //if(strcmp(data,"ON")==0)
          leds_toggle(LEDS_GREEN);
          leds_toggle(LEDS_RED);
      //else if(strcmp(data,"OFF")==0)
      //    leds_off(LEDS_GREEN);
      //printf("ON\n");
      cc26xx_uart_write_byte('r');
    }
  }
  PROCESS_END();
}