/*
 * STM32CustomBootloader.c
 *
 *  Created on: Feb 15, 2023
 *      Author: USER
 */
//#include "main.h"
#include <stdlib.h>
#include "STM32CustomBootloader.h"
#include "string.h"




pFunction Jump_To_Application;




uint8_t HexIntelDecodeFlashProgramming(uint8_t *data,uint32_t size)
{
	//ref from this
	//http://www.dlwrr.com/electronics/tools/hexview/hexview.html
		uint32_t i=0,x;
		uint8_t LL;
		uint32_t AAAA,BuffStore;
		uint8_t TT;
	    uint8_t StringtoHexbuffer[16];
	    uint8_t checksum[2];
	    volatile uint32_t Address_Mask=0x08000000;//= ApplicationAddress;
//	    uint8_t checksumdatasize;
	    volatile uint32_t Address;//= ApplicationAddress;
	    HAL_StatusTypeDef Flashresult;

			while(i<size)
			{
				if(*(data+i) == ':')
				{
					ConvertStringtoHEX(data+i+1,2,&LL);
					ConvertStringtoHEX(data+i+3,4,StringtoHexbuffer);
					AAAA = ((uint16_t)StringtoHexbuffer[0]<<8)|((uint16_t)StringtoHexbuffer[1]);
					ConvertStringtoHEX(data+i+7,2,&TT);
					ConvertStringtoHEX(data+i+9+(LL*2),2,&checksum[0]);

					ConvertStringtoHEX(data+i+9,LL*2,StringtoHexbuffer);
					checksum[1] = LL+TT+((uint8_t)(AAAA>>8))+((uint8_t)(AAAA&0x00FF));
					for(x=0;x<LL;x++)checksum[1] = checksum[1]+StringtoHexbuffer[x];
					checksum[1]--;
					checksum[1] ^= 0xFF;
					if(checksum[1] == checksum[0])//check HEX CRC
					{
						switch(TT)
						{
							case 0x00:
								HAL_FLASH_Unlock();
								__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGAERR| FLASH_FLAG_PGPERR | FLASH_FLAG_WRPERR);
								ConvertStringtoHEX(data+i+9,LL*2,StringtoHexbuffer);
								for(x=0;x<LL;x+=4)
								{
									BuffStore = ((uint32_t)StringtoHexbuffer[x+3] << 24)|
															((uint32_t)StringtoHexbuffer[x+2] << 16)|
															((uint32_t)StringtoHexbuffer[x+1] << 8)|
															((uint32_t)StringtoHexbuffer[x]);
									if((Address|(uint32_t)AAAA) < ApplicationAddress)return 0;
									Address = (Address&Address_Mask)|AAAA;
									Flashresult=HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,BuffStore);

									if(Flashresult == HAL_ERROR){
										;//Do process try out;
									}
									//Flashresult = FLASH_ProgramWord(0x08003C08,BuffStore);
									AAAA+=4;
								}
								HAL_FLASH_Lock();
								break;
							case 0x01: //End of file
								break;


							case 0x04://goto address MSB
								ConvertStringtoHEX(data+i+9,4,StringtoHexbuffer);
								Address = ((uint32_t)StringtoHexbuffer[0]<<24)|((uint32_t)StringtoHexbuffer[1]<<16);
								Address_Mask = Address;
								break;


							case 0x05://jump to address;
								//Bootloader_JumpToApplication();
								GOTO_APP();
								break;
							default:
								break;
						}
					}
					else
					{
						return 0;
					}
				}
				i++;
			}
			return 1;
}

void EraseAPP(void)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError
	;
		HAL_FLASH_Unlock();
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGAERR| FLASH_FLAG_PGPERR | FLASH_FLAG_WRPERR);
	
//	  uint32_t StartPage = GetPage(StartPageAddress);
//	  uint32_t EndPageAdress = StartPageAddress + numberofwords*4;
//	  uint32_t EndPage = GetPage(EndPageAdress);

	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase   = FLASH_TYPEERASE_SECTORS;
	   EraseInitStruct.Sector = 6;
	   EraseInitStruct.NbSectors    = 6;
		 EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	   if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	   {
	     /*Error occurred while page erase.*/
		  //return HAL_FLASH_GetError ();
	   }
	
	
		HAL_FLASH_Lock();
	
	
}

uint8_t Get_linetotal(uint8_t *String,uint16_t size,uint32_t *LineTotal)
{
	uint16_t i;
	char stringtemp[10]={0,0,0,0,0,0,0,0,0,0},st=0,ch=0;
	uint8_t str_xor[2],xorref=0;
	char sumxor=0x0;

	if((String[0] == 'H') && (String[1] == 'E') && (String[2] == 'X')&& (String[3] == '='))
	{
		for(i=0;i<size;i++)
		{
			if(String[i] != '*')
			{
				if((String[i] >= '0') && (String[i] <= '9'))
				{
					stringtemp[ch] = String[i];
					stringtemp[ch+1] = 0;
					ch++;
				}
				sumxor = sumxor^String[i];
			}
			else
			{
				sumxor = sumxor^String[i];
				str_xor[0] = String[i+1];
				str_xor[1] = String[i+2];
				break;
			}
		}
		
		ConvertStringtoHEX(str_xor,2,&xorref);
		if(sumxor == xorref)
		{
			*LineTotal=(uint32_t)atoi(stringtemp);
			return True;
		}
	}
	return False;
}

void ConvertStringtoHEX(uint8_t *String,uint8_t size,uint8_t *Output)
{
		 uint8_t i,j;
		 uint8_t OutputCounter=0;
	   uint8_t buff[2];
		 for(i=0;i<size;i+=2)
		 {
				for(j=0;j<2;j++)
				{
					if((*(String+i+j) >= 'A') && (*(String+i+j) <= 'F'))
					{
						buff[j]=*(String+i+j)-0x37;
					}
					else if((*(String+i+j) >= 'a') && (*(String+i+j) <= 'f'))
					{
						buff[j]=*(String+i+j)-0x57;
					}
					else if((*(String+i+j) >= '0') && (*(String+i+j) <= '9'))
					{
						buff[j]=*(String+i+j)-0x30;
					}
			  }
				*(Output+OutputCounter) = buff[0]<<4|buff[1];
				OutputCounter++;
		 }
}


void Bootloader_JumpToApplication(void)
{
    uint32_t JumpAddress = *(__IO uint32_t*)(ApplicationAddress + 4);
    pFunction Jump       = (pFunction)JumpAddress;

    HAL_RCC_DeInit();
    HAL_DeInit();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

#if(SET_VECTOR_TABLE)
    SCB->VTOR = APP_ADDRESS;
#endif

    __set_MSP(*(__IO uint32_t*)ApplicationAddress);
    Jump();
}

uint8_t program_verify_fanuc(uint8_t *data,uint32_t size)
{
		uint32_t i=0,x;
		uint8_t LL;
		uint32_t AAAA,BuffStore;
		uint8_t TT;
	    uint8_t StringtoHexbuffer[16];
	    uint8_t checksum[2];
	    volatile uint32_t Address_Mask=0x08000000;//= ApplicationAddress;
//	    uint8_t checksumdatasize;
	    volatile uint32_t Address;//= ApplicationAddress;
	    HAL_StatusTypeDef Flashresult;

			while(i<size)
			{
				if(*(data+i) == ':')
				{
					ConvertStringtoHEX(data+i+1,2,&LL);
					ConvertStringtoHEX(data+i+3,4,StringtoHexbuffer);
					AAAA = ((uint16_t)StringtoHexbuffer[0]<<8)|((uint16_t)StringtoHexbuffer[1]);
					ConvertStringtoHEX(data+i+7,2,&TT);
					ConvertStringtoHEX(data+i+9+(LL*2),2,&checksum[0]);

					ConvertStringtoHEX(data+i+9,LL*2,StringtoHexbuffer);
					checksum[1] = LL+TT+((uint8_t)(AAAA>>8))+((uint8_t)(AAAA&0x00FF));
					for(x=0;x<LL;x++)checksum[1] = checksum[1]+StringtoHexbuffer[x];
					checksum[1]--;
					checksum[1] ^= 0xFF;
					if(checksum[1] != checksum[0])//check HEX CRC
					{
						return 0;
					}
				}
			}
			return 1;
}



void GOTO_APP(void)
{
	//GOTO APP
	/* Set system control register SCR->VTOR  */
	uint32_t JumpAddress = *(__IO uint32_t*)(ApplicationAddress + 4);
	pFunction Jump       = (pFunction)JumpAddress;

	HAL_RCC_DeInit();
	HAL_DeInit();

	SCB->VTOR = NVIC_VectTab_FLASH| APPoffset;

	__set_MSP(*(__IO uint32_t*)ApplicationAddress);
	Jump();
}


void disable_interrupts()
{
  // Disable all interrupts by setting the interrupt priority mask to the maximum level
  __disable_irq();
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

  // Clear any pending interrupts
  NVIC_ClearPendingIRQ(SysTick_IRQn);
  NVIC_ClearPendingIRQ(SVCall_IRQn);
  NVIC_ClearPendingIRQ(PendSV_IRQn);

  // Disable all interrupts at the interrupt controller level
  for (uint32_t i = 0; i < 8; i++) {
    NVIC->ICER[i] = 0xFFFFFFFF;
  }
}







