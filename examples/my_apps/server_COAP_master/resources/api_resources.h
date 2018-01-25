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
#include "rest-engine.h"
#include <string.h>
#include <stdio.h>

#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"
#include "batmon-sensor.h"
#include "button-sensor.h"

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

#define LED_ON				'1'
#define LED_OFF				'0'
#define END			0x0a
////////////////////////

// Create the payload
#define ON		1
#define OFF		0
#define not_supported_msg "Supported:text/plain,application/json"
//const char *not_supported_msg = "Supported:text/plain,application/json";

// Shared global variable used to fill up the resource payload response
#define BUFF_SIZE	5	// worse case: 3 2 8 1 \0
extern char shared_variable[BUFF_SIZE];

//Used to compare with new measures
#define OBSERVER_TIMER	CLOCK_SECOND*7
char temp_old[BUFF_SIZE];
char volt_old[BUFF_SIZE];


//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_RESOURCES_H__