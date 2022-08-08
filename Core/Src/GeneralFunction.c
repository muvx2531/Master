//#include "GeneralFunction.h"

#include "GeneralFunction.h"


//extern UART_HandleTypeDef huart4;
//extern UART_HandleTypeDef huart2;

//void usart4_putsSize(unsigned char *s,unsigned short int size)
//{
//	HAL_UART_Transmit(&huart4,s,size,100);
//}
//void usart2_putsSize(unsigned char *s,unsigned short int size)
//{
//	HAL_UART_Transmit(&huart2,s,size,100);
//}
//
//void usart3_putsSize(unsigned char *s,unsigned short int size)
//{
//	//HAL_UART_Transmit(&huart3,s,size,100);
//}

void BytesToInt(unsigned char *BytesIn,unsigned short int *IntVal)
{
    unsigned char *IntP;

    IntP=(unsigned char *)IntVal;
    *IntP=*(BytesIn+1);
    *(IntP+1)=*BytesIn;
}

void BytesToLong(unsigned char* BytesIn,unsigned long* LongVal)//32bit
{
    unsigned char* LongP;
    signed char i;
    unsigned char j=0;
    LongP=(unsigned char *)LongVal;
    for(i=3;i>=0;i--)
    {
        *(LongP+i)=*(BytesIn+j);
        j++;
    }
}

void IntToBytes(unsigned short int IntVal,unsigned char* BytesOut)
{
    unsigned char* IntP;
    IntP=(unsigned char *)&IntVal;
    *BytesOut=*(IntP+1);
    *(BytesOut+1)=*IntP;

}


void ModRTU_CRC(unsigned char *dat,unsigned short int size,unsigned char *res)
{
  unsigned int len = size;
  unsigned char buf[1024];
  unsigned int crc = 0xFFFF;
  unsigned int pos,i,j;
//  QByteArray result;

  for(j=0;j<len;j++)buf[j]=dat[j];

  for (pos = 0; pos < len; pos++) {
    crc ^= (unsigned int)buf[pos];          // XOR byte into least sig. byte of crc

    for (i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
	res[0]=(unsigned char)((crc>>8)&0x00FF);
	res[1]=(unsigned char)crc;
//  result.append((quint8)crc);
  //qCRCresult.append((quint8)result);
  //qDebug()<<qCRCresult.toHex();
}

void CRC16Gen(unsigned char *data,unsigned short int size,unsigned char* CRCResult)
{
    unsigned int sizeofbit=(size*8)+CRC_16;
    unsigned long result=0;
    unsigned long check=(CRC16modbus>>CRC_16)<<CRC_16;
    unsigned char Feeddata=0;
    unsigned int i=0;
    unsigned char currentBit=8;
	
    while(sizeofbit != 0x00)
    {
        if(sizeofbit<=CRC_16)Feeddata=0x00;
        else
        {
            Feeddata=(*(data+i)<<(8-currentBit));
            Feeddata=Feeddata>>7;
        }

        result=result<<1|Feeddata;

        if(result >= check)result=result^CRC16modbus;

        currentBit--;  //point to next bit
        if(currentBit==0)
        {
            currentBit=8;
            i++;      //point to next data
        }
        sizeofbit--;  //totalsize -1
    }
    IntToBytes((int)result,CRCResult);
}

void ConvertByteInt(unsigned int* IntData,unsigned char ConvertDataType,unsigned char* BytesInOut)
{
	if(ConvertDataType == 0)
	{
		*BytesInOut = 0x00;
		*BytesInOut = *IntData>>8;
		*(BytesInOut+1) = *IntData;	
	}
	else
	{
		*IntData = 0x0000;
		*IntData |= (unsigned char) * (BytesInOut);
		*IntData = *IntData << 8;
		*IntData |= (unsigned char) * (BytesInOut+1);
	}
}

void FloatToBytes(float fValues,unsigned char* BytesOut)
{
    unsigned char* FloatP;
    signed char i;
    unsigned char j=0;
    FloatP=(unsigned char *)&fValues;

    for(i=3;i>=0;i--)
    {
     *(BytesOut+j)=*(FloatP+i);
     j++;
    }
}

void BytesToFloat(unsigned char* BytesIn,float* FloatValOut)
{
    unsigned char* FloatP;
    signed char i;
    unsigned char j=0;
    FloatP=(unsigned char *)FloatValOut;
    for(i=3;i>=0;i--)
    {
        *(FloatP+j)=*(BytesIn+i);
        j++;
    }
}

void Longtobyte(unsigned long input,unsigned char *out)
{
    //unsigned long input=*(unsigned int*)in;
		unsigned char i,j=4;
		unsigned char *p;
	    p=(unsigned char*)&input;
		for(i=0;i<4;i++)
		{
			j--;
			*(out+i)= *(p+j);
		}
}
//bit = only 8 16 32
unsigned int DigitalLotateBit(unsigned int dataint,unsigned char bit)
{
		unsigned int result=0;
		char i;
//	  j=bit-1;
		for(i=0;i<32;i++)
		{
				result |= ((dataint>>i)<<(31-i))&((0x80000000)>>i);
		}
	return result;
}

unsigned char FindArraySet(unsigned char *datain,unsigned int sizein,unsigned char *dataref,unsigned int size)
{
	unsigned short int i,j=0;
	for(i=0;i<sizein;i++)
	{
		if(*(datain+i)==*(dataref+j))
		{
			j++;
		}
		else 
		{
			j=0;
			if(*(datain+i)==*(dataref+j))
			{
				j++;
			}
		}
		if(j>size-1)
		{
			return 1;
		}
	}
	return 0;
}

unsigned char FindArray_toPosition(unsigned char *datain,unsigned int sizein,unsigned char *dataref,unsigned char size)
{
	unsigned short int i,j=0;
	for(i=0;i<sizein;i++)
	{
		if(*(datain+i)==*(dataref+j))
		{
			j++;
		}
		else 
		{
			j=0;
			if(*(datain+i)==*(dataref+j))
			{
				j++;
			}
		}
		if(j>size-1)
		{
			return i;
		}
	}
	return 0;
}

unsigned int attachdata(unsigned char *out,unsigned char *ag1,unsigned int size1,unsigned char *ag2,unsigned int size2,unsigned char *ag3,unsigned int size3,unsigned char *ag4,unsigned int size4)
{
	unsigned int i,j=0;
	for(i=0;i<size1;i++)
	{
		*(out+j)=*(ag1+i);
		j++;
	}
	for(i=0;i<size2;i++)
	{
		*(out+j)=*(ag2+i);
		j++;
	}
	for(i=0;i<size3;i++)
	{
		*(out+j)=*(ag3+i);
		j++;
	}
	for(i=0;i<size4;i++)
	{
		*(out+j)=*(ag4+i);
		j++;
	}
	return j;
}


unsigned char Comparearray(unsigned char *dat1,unsigned char *dat2,unsigned int size)
{
	unsigned int i;
	for(i=0;i<size;i++)
	{
		if(*(dat1+i) != *(dat2+i))return 0;
	}
	return 1;
}

unsigned char SerialNOCRCCheck(unsigned char* SerialNumber,signed char Size)
{
    signed char i,j=1;
    unsigned int CRCResult=0;

    for(i=Size-2;i>=0;i--)
    {
        //m=(*(SerialNumber+i)*j);
        if((*(SerialNumber+i)>=0x30)&&(*(SerialNumber+i)<=0x39))CRCResult+=((*(SerialNumber+i)-0x30)*j);
        else CRCResult+=(*(SerialNumber+i)*j);
        j++;
    }
    if((CRCResult%9)==SerialNumber[Size-1]-0x30)return 0x01;
    return 0x00;
}

void Disassemblytobyte(unsigned char push_F_B,void *datain,unsigned int size,unsigned char *dataout)
{
	unsigned char *pp;
	int i,j;
	pp = (unsigned char*)datain;
	if(push_F_B == push_font)for(i=0;i<size;i++)*(dataout+i) = *(pp+i);
	else
	{
		j=0;
		for(i=(size-1);i>=0;i--)
		{
			*(dataout+j) = *(pp+i);
			j++;
		}
	}
}

void Set_array(unsigned char *data,unsigned short int size,unsigned char value)
{
	unsigned short i;
	for(i=0;i<size;i++)*(data+i)=value;
}

void StringByteTolong(unsigned char *ByteArray,unsigned char Size,unsigned long *Result)
{
    unsigned char i,j;
    unsigned long Multiplier;

    *Result=0;
    for (i=0;i<Size;i++)
    {
        Multiplier=1;
        for(j=0;j<Size-(i+1);j++)
        {
           Multiplier*=10;
        }
        *Result+=(*(ByteArray+i)-0x30)*Multiplier;
    }
}

void CopyArrayCharFunc(unsigned char* DataOri,unsigned char* DataDes,unsigned int Datasize)
{
    unsigned int i=0;
    for(i=0;i<Datasize;i++)
    {
        *(DataDes+i)=*(DataOri+i);
    }
}

unsigned char StringToHEX(unsigned char *str)
{
    unsigned char i;
    unsigned char tmp[2]={0,0};
    unsigned char ret=0;
    for(i=0;i<2;i++)
    {
        if((*(str+i) >= 0x30) && (*(str+i) <= 0x39))
        {
            tmp[i] = *(str+i) - 0x30;
        }
        else if((*(str+i) >= 'A') && (*(str+i) <= 'F'))
        {
            tmp[i] = *(str+i) - 0x37;
        }
        else if((*(str+i) >= 'a') && (*(str+i) <= 'f'))
        {
            tmp[i] = *(str+i) - 0x57;
        }
    }
    ret =(tmp[0]<<4)|tmp[1];
    return ret;
}


unsigned int Copy_String_Between_Text(char *Head,char *Tail,unsigned char *dat,unsigned int size,unsigned char *out)
{
    unsigned int i,H=0,T=0,stop=0,start=0;
    unsigned int res;
    
    for(i=0;i<size;i++)
    {
        if((*(Head+H) == *(dat+i)) && (*(Head+H) != 0x00))H++;
        if((H > 0) && (*(Head+H) == 0x00))
        {
            start = i;
            break;
        }
        if((i+1)>=size)return 0;
    }
    
    for(i=start;i<size;i++)
    {
        if((*(Tail+T) == *(dat+i)) && (*(Tail+T) != 0x00))T++;
        if((T > 0) && (*(Tail+T) == 0x00))
        {
            stop = i-T;
            break;
        }
        if((i+1)>=size)return 0;
    }
    
    H=0;
    for(i=(start+1);i<=stop;i++)
    {
        *(out+H) = *(dat+i);
        *(out+H+1) = 0;
        H++;
    }
    res = stop -start;
    return res;
}

unsigned int SearchEndData(unsigned char* DataIn,unsigned char EndChar,unsigned int MaxSize)
{
    unsigned int i;
    for(i=0;i<MaxSize;i++)
    {
        if(*(DataIn+i)==EndChar)
        {
            return i;
        }
    }
    return i;
}

void FloatToBits(float FloatData,unsigned long* LongOutPut,unsigned char MaxPossearch)
{
    //#define MaxA2dByteSearch        4
    //#define MaxA2DSensorEquVal      2047
    unsigned char strBuff[15];
    unsigned char SearchResult;
    unsigned long LongBuff;
    unsigned char CharBuff;
		//int DataSize;
    //unsigned int FloatBuff;
    //FloatData=0;
    *LongOutPut=0;
		
    sprintf((char*)strBuff,"%0.01f",FloatData);
		//sprintf((char*)strBuff,"%f",FloatData);
		//to_string(3.12);
		//DataSize=sprintf((char*)strBuff,"%f",FloatData);
		//print((char*)strBuff,"%0.01f",FloatData);
    SearchResult=SearchEndData(strBuff,'.',MaxPossearch+1);
    if(SearchResult<=MaxPossearch)
    {
        StringByteTolong(strBuff,SearchResult,LongOutPut);
        LongBuff=((*LongOutPut)<<4);
        CharBuff=strBuff[SearchResult+1]-0x30;
        *LongOutPut=CharBuff|LongBuff;

    }
    //}
}


void NullFuncvoid(void)
{
	;
}
void NullFunc_1ag(void *ag)
{
	;
}
void NullFunc_2ag(unsigned char *ag1,unsigned short int ag2)
{
	;
}

/*
Equation for high-pass filter:
y[n]=(1-alpha)y[n-1]+(1-alpha)(x[n]-x[n-1])    
//use this for angles obtained from gyroscopes
x[n] is the pitch/roll/yaw that you get from the gyroscope
y[n] is the filtered final pitch/roll/yaw which you must feed into the next phase of your program

n is the current sample indicator.
alpha is related to time-constant. 
it defines the boundary where the accelerometer readings stop and the gyroscope readings take over and vice-versa. 
It controls how much you want the output to depend on the current value or a new value that arrives. 
Both the alpha�s have to be the same. alpha is usually > 0.5 using the definitions above.
*/ 

void HightpassFilter(ClassFilter *Instance)
{
	Instance->Output = ((1-Instance->Alpha)*Instance->Output)+((1-Instance->Alpha)*(Instance->Input - Instance->Input_n_minus_1));
	Instance->Input_n_minus_1 = Instance->Input;
}

unsigned char Writeflashconfig(unsigned int Address,unsigned char *dat,unsigned int size)
{
	unsigned char Buffer[256];
	unsigned char *pBuffer;
//	HAL_StatusTypeDef res;
	unsigned int i;
//  uint32_t PageError = 0;
//	uint32_t *pProgramer=(uint32_t*)Buffer;
//	FLASH_EraseInitTypeDef f;
	
//	Readflash(Address,Buffer,256);
//	for(i=0;i<size;i++)Buffer[i]= *(dat+i);
//	
////	f.TypeErase = FLASH_TYPEERASE_PAGES;
////	f.PageAddress = Address;
////	f.NbPages = 1;

//	if(((Address >= StartEeprom)&&(Address <= StartEeprom)) && (size <= ((EndEeprom+1)- Address)) && (size != 0))
//	{
//		HAL_FLASH_Unlock();
//		res = HAL_FLASHEx_Erase(&f, &PageError);//Erase
//		for(i=0;i<(Pagesize/4);i+=4)
//		{
//			res = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,StartEeprom+i,*pProgramer);
//			pProgramer++;
//			if(res != HAL_OK)
//			{
				return 0;
//			}
//		}
//		HAL_FLASH_Lock();
//		return 1;
//	}
//	else return 0;
}

void Readflash(unsigned int Address,unsigned char *dat,unsigned int size)
{
	unsigned int i;
	for(i=0;i<size;i++)*(dat+i) = (unsigned char)*((unsigned int*)(Address+i));
}

void ConvertStringtoHEX(unsigned char *String,unsigned char size,unsigned char *Output)
{
		 unsigned char i,j;
		 unsigned char OutputCounter=0;
	   unsigned char buff[2];
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

unsigned char Writeflash(unsigned int Address,unsigned char *dat,unsigned int size)
{
//		HAL_StatusTypeDef res;
//		uint16_t i;
//	  uint32_t *pProgramer=(uint32_t*)dat;
//		HAL_FLASH_Unlock();
//		FLASH_PageErase(0x08080000);
//		HAL_FLASH_Lock();
//	
//		HAL_FLASH_Unlock();
//		for(i=0;i<size;i+=4)
//		{
//			res = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address+i,*pProgramer);
//			pProgramer++;
//			if(res != HAL_OK)
//			{
				return 0;
//			}
//		}
//		HAL_FLASH_Lock();
}


