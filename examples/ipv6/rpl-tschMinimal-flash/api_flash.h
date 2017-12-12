/******************************************************************************
*	Filename:		api_flash.h
*	Revised:			2017-10-24 13:04:16 (Tue, 24 Oct 2017)
*	Author:			Ramirez Gomez, Raul (raulramirezgomez@gmail.com)
*
*	Description:	API para interactuar con la Flash.
						 Describir el funcionamiento...
******************************************************************************/

#ifndef __API_FLASH_H__
#define __API_FLASH_H__


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
//				INCLUDES
/*************************************/
#include "contiki.h"
#include "sys/process.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "net/ip/uip-debug.h"


/*************************************/
//	DEFINES & ASSOCIATED VARIABLES
/*************************************/
//Values that can be returned from the erase functions
#define ERASE_SUCCESS		1
#define ERASE_ERROR			0

//FLASH
#define FLASH_ADDR_START	0x19000
#define FLASH_ADDR_END		FLASH_ADDR_START + 0x1000

//SOURCE ID - DICTIONARY
#define TEMP		0x01
#define VOLT		0x02
#define LED_ORANGE	0x03
#define LED_GREEN	0x04
#define LED_YELLOW	0x05
#define LED_RED		0x06
#define LED_ALL		0X07



//Struct used to store measuring information
typedef struct Measure {
	unsigned long sysUpTime;
	uint8_t ID;
	uint8_t measure;
} Measure;


/******************************************************************************
*	Función que resetea el bloque de 4K escogido para
* 	 almacenar los datos.
*	\params
*		- pos_flash: Pointer to flash position.
*	\return
*		- resul: Result of the erase
*			ERASE_SUCCESS
*			ERASE_ERROR
******************************************************************************/
extern int erase_flash(uint32_t *pos_flash);


/******************************************************************************
*	Función que escribe en la flash.
*	\params
*		- data_to_write: Struct storing the measure to write on flash.
*		- pos_flash: Pointer to flash position.
*	\return
*		- Void
******************************************************************************/
extern void write_flash(struct Measure data_to_write, uint32_t *pos_flash);


/******************************************************************************
*	Función que lee de la flash.
*	\params
*		- pos_flash: Flash address to read.
*	\return
*		- read_data
******************************************************************************/
extern struct Measure read_flash(uint32_t pos_flash);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __FLASH_H__