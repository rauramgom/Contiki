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
#include "sys/process.h"
#include "rest-engine.h"
#include "er-coap.h"
#include "batmon-sensor.h"
#include "button-sensor.h"
#include "dev/leds.h"

#include "dev/cc26xx-uart.h"
#include "dev/serial-line.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


// IPv6/RPL Stack
//#include "net/netstack.h"
//#include "net/ipv6/uip-ds6-nbr.h"
//#include "net/ipv6/uip-ds6-route.h"
//#include "net/rpl/rpl.h"
//#include "net/rpl/rpl-private.h"

// Dictionary
////////////////////////

#define LED_GREEN_POST_ON	'3'

#define LED_RED_POST_ON		'7'

#define END			0x0a
////////////////////////


//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_RESOURCES_H__