/******************************************************************************
*	Filename:		api_flash.c
*	Revised:			2017-10-20 12:34:02 (Fry, 20 Oct 2017)
*	Author:			Ramirez Gomez, Raul (raulramirezgomez@gmail.com)
*
*	Description:	API para interactuar con la Flash.
						 Describir el funcionamiento...
******************************************************************************/

/*************************************/
//				INCLUDES
/*************************************/
#include "contiki.h"
#include "sys/process.h"
#include "rest-engine.h"
#include "er-coap.h"
#include "batmon-sensor.h"
#include "clock.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//FLASH
#include "driverlib/flash.h"
#include "api_flash.h"


/*************************************/
//	DEFINES & ASSOCIATED VARIABLES
/*************************************/
//FLASH
#define BLOCK_LEN	12
//static uint32_t pos_flash = FLASH_ADDR_START;


/*************************************/
//		FUNCTIONS DEFINITION
/*************************************/
/******************************************************************************
*	Función que resetea el bloque de 4K escogido para
* 	 almacenar los datos.
******************************************************************************/
int
erase_flash(uint32_t *pos_flash)
{
	int resul = ERASE_ERROR;
	uint32_t erase;
	//Comprobamos que no se ha detectado error y la Flash esta lista 
	// para recibir nuevos comandos
	if(FlashCheckFsmForError() == FAPI_STATUS_SUCCESS &&
			FlashCheckFsmForReady() == FAPI_STATUS_FSM_READY)
	{
		FlashIntDisable(FLASH_INT_FSM_DONE);
		//Reseteamos..
		erase = FlashSectorErase(FLASH_ADDR_START); 
		if(erase == FAPI_STATUS_SUCCESS && 
			FlashCheckFsmForReady() == FAPI_STATUS_FSM_READY)
		{
			resul = ERASE_SUCCESS;
			*pos_flash = FLASH_ADDR_START;
			printf("[%lu] ERASE -- OK.\n", clock_seconds());
		} else if (erase == FAPI_STATUS_FSM_ERROR) {
			printf("[%lu] [**ERASE ERROR**] A programming error is encountered.\n",
				clock_seconds());
		} else {
			//Cuando no se introduce una dirección de inicio de bloque
			printf("[%lu] [**ERASE ERROR**] Invalid argument.\n",
				clock_seconds());
		}
		FlashIntEnable(FLASH_INT_FSM_DONE);
	} else {
		printf("[%lu] [**ERROR**] Flash is not ready or there is A FSM error.\n", 
				clock_seconds());
	}

	return resul;
} //End of erase_flash()


/******************************************************************************
*	Función que escribe en la flash.
******************************************************************************/
void
write_flash(struct Measure data_to_write, uint32_t *pos_flash)
{
	uint8_t write_buffer[3];
			//write_buffer[0] = sysUpTime
			//write_buffer[1] = ID
			//write_buffer[2] = measure
	uint32_t write_resul;

	//Antes de escribir comprueba que no se hayan completado los 4k
	if(*pos_flash < FLASH_ADDR_START+BLOCK_LEN/*FLASH_ADDR_END*/ || 
		((*pos_flash >= FLASH_ADDR_START+BLOCK_LEN) &&
			erase_flash(pos_flash) == ERASE_SUCCESS))
	{
		while(FlashCheckFsmForError() != FAPI_STATUS_SUCCESS &&
				FlashCheckFsmForReady() != FAPI_STATUS_FSM_READY){
			printf("[%lu] Waiting until Fsm is Ready...\n", clock_seconds());
		}

		write_buffer[0] = (uint8_t)data_to_write.sysUpTime;
		write_buffer[1] = data_to_write.ID;
		write_buffer[2] = data_to_write.measure;
		//Escribimos en la memoria Flash
		FlashIntDisable(FLASH_INT_FSM_DONE);
		write_resul = FlashProgram(write_buffer, *pos_flash,
								sizeof(write_buffer));
		FlashIntEnable(FLASH_INT_FSM_DONE);
		if (write_resul == FAPI_STATUS_SUCCESS)
		{
			printf("[%lu] Flash written correctly.\n", clock_seconds());
			*pos_flash += sizeof(write_buffer);
			printf("pos_flash: %lu\n", *pos_flash);
		} else if (write_resul == FAPI_STATUS_FSM_ERROR) {
			//Cuando intenta escribir 1 donde hay 0 sin Erase() previo
			printf("[%lu] [** WRITE ERROR**] A programming error is encountered.\n",
				clock_seconds());
			printf("POS_FLASH ESCRITA2: %d\n\n", (int)*pos_flash);
		} else {
			printf("[%lu] [**WRITE ERROR**] Too many bytes were requested.\n",
				clock_seconds());
		}
	} 
} //End of write_flash()


/******************************************************************************
*	Función que lee de la flash.
******************************************************************************/
struct Measure
read_flash(uint32_t pos_flash)
{
	uint8_t read_buffer[1];
	uint8_t buffer_total[BLOCK_LEN];
	struct Measure readed_measure;
	char IDmeasure[4];
	
	while(FlashCheckFsmForError() != FAPI_STATUS_SUCCESS &&
			FlashCheckFsmForReady() != FAPI_STATUS_FSM_READY) {
		printf("[%lu] Waiting until Fsm is Ready...\n", clock_seconds());
	}

	//Lee el último valor almacenado en la flash
	if(FlashCheckFsmForError() == FAPI_STATUS_SUCCESS &&
			FlashCheckFsmForReady() == FAPI_STATUS_FSM_READY){
		FlashIntDisable(FLASH_INT_FSM_DONE);
		//SysUpTime
		FlashRead(read_buffer, pos_flash-3, sizeof(read_buffer));
		readed_measure.sysUpTime = *read_buffer;
		//ID
		FlashRead(read_buffer, pos_flash-2, sizeof(read_buffer));
		readed_measure.ID = *read_buffer;
		//measure
		FlashRead(read_buffer, pos_flash-1, sizeof(read_buffer));
		readed_measure.measure = *read_buffer;
		switch(readed_measure.ID)
		{
			case TEMP:
				strcpy(IDmeasure, "TEMP");
				break;
			case VOLT:
				strcpy(IDmeasure, "VOLT");
				break;
		}
		printf("[%lu] Measure data:\r -sysUpTime: %lu\r -ID: %s\r -value: %i\n", 
					clock_seconds(), readed_measure.sysUpTime,
					IDmeasure, readed_measure.measure);
//[ ....
		FlashRead(buffer_total, FLASH_ADDR_START, sizeof(buffer_total));
		printf("BUFFER: ");
		for(int i=0; i<BLOCK_LEN; i++)
			printf("%i-", buffer_total[i]);
		printf("\n\n");
// .... ]
		FlashIntEnable(FLASH_INT_FSM_DONE);
	} else {
		printf("[%lu] [**ERROR**] Flash is not ready or there is a FSM error.\n",
			clock_seconds());
	}

	return readed_measure;
} //End of read_flash()