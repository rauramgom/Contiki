/******************************************************************************
*	Filename:		api_resources.h
*	Revised:		2018-1-24
*	Author:			Ramirez Gomez, Raul (raulramirezgomez@gmail.com)
*
*	Description:	API to interact with the resources
******************************************************************************/

#ifndef __API_RESOURCES_H__
#define __API_RESOURCES_H__


//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"
#include "er-coap.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include "batmon-sensor.h"
#include "button-sensor.h"
#include "dev/leds.h"	

#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"

#include "net/mac/tsch/tsch.h"


// Dictionary
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

#define END					0x0a
////////////////////////

// Error in payload
#define not_supported_msg "Supported:application/json, text/plain"


// Shared global variables used to fill up the payload response of the resource
#define TEMP_SIZE	4	// worse case: - 2 9 \0
extern char temp_shared[TEMP_SIZE];

#define VOLT_SIZE	5	// worse case: 2 9 6 1 \0
extern char volt_shared[VOLT_SIZE];

#define AUX_SIZE	6	// worse case: V 2 9 6 1 \0

// Periodic resource timer
#define TEMP_TIMER		1*CLOCK_SECOND
#define VOLT_TIMER		1*CLOCK_SECOND

#define VOLT_PER_TIMER	2*CLOCK_SECOND
#define TEMP_PER_TIMER	2*CLOCK_SECOND


//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_RESOURCES_H__