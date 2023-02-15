#include "TCP_protocol_Handle.h"

void Protocol_Handle(uint8_t *dat,uint16_t size)
{
	uint16_t PackageSize=0;
	uint8_t crcres[2];
	for(uint16_t index = 0;index<size;index++)
	{
		if((dat[index+0] == Protocol_Header[0] ) && (dat[index+1] == Protocol_Header[1] ) && (dat[index+2] == Protocol_Header[2] ))
		{
			PackageSize = ((uint16_t)dat[index+3])<<8|((uint16_t)dat[index+4]);
			CRC16Gen(dat+index,PackageSize+7,crcres);
			if((crcres[0] == 0) && (crcres[1] == 0))
			{
				switch(dat[index+5])
				{
					case 0x01:
						while(1);
						break;
					
					default: break;
				}
			}
		}
	}
}

uint8_t CheckXORsum(uint8_t *p,uint16_t size)
{
	
}




