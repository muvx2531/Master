/*
 * STM32CustomBootloader.h
 *
 *  Created on: Feb 15, 2023
 *      Author: USER
 */

#ifndef INC_STM32CUSTOMBOOTLOADER_H_
#define INC_STM32CUSTOMBOOTLOADER_H_

#include "main.h"



typedef  void (*pFunction)(void);



/************Bootloader configuration************/

#define NVIC_VectTab_FLASH  ((uint32_t)0x08000000) //default
#define APPoffset			((uint32_t)0x0000F000)
#define ApplicationAddress  (NVIC_VectTab_FLASH | APPoffset)

/******************36
******************************/

uint8_t HexIntelDecodeFlashProgramming(uint8_t *data,uint32_t size);
void ConvertStringtoHEX(uint8_t *String,uint8_t size,uint8_t *Output);
void Bootloader_JumpToApplication(void);
void GOTO_APP(void);
uint8_t program_verify_fanuc(uint8_t *data,uint32_t size);
uint8_t Get_linetotal(uint8_t *String,uint16_t size,uint32_t *LineTotal);
#endif /* INC_STM32CUSTOMBOOTLOADER_H_ */
