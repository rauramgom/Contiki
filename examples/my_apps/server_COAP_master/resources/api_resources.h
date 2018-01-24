/******************************************************************************
*	Filename:		api_flash.h
*	Revised:			2017-10-24 13:04:16 (Tue, 24 Oct 2017)
*	Author:			Ramirez Gomez, Raul (raulramirezgomez@gmail.com)
*
*	Description:	API para interactuar con la Flash.
						 Describir el funcionamiento...
******************************************************************************/

#ifndef __API_RESOURCES_H__
#define __API_RESOURCES_H__


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/*************************************/
//	DEFINES & ASSOCIATED VARIABLES
/*************************************/
//Dictionary
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

//Create the payload
#define ON		1
#define OFF		0
const char *not_supported_msg = "Supported:text/plain,application/json";



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_RESOURCES_H__