
#define True 0x01
#define False 0x00
//#define Disable 0x00
//#define Enable 0x01
#define HOLD  0x02


#define 	push_back 1
#define 	push_font 0

#define bufferFull 0x02

//typedef enum{
//	tDisable = 0,
//	tEnable,
//	tIdle
//}TimerTriState;


///define for CRC func
//CRC16
#define CRC_16        16
#define CRC16poly    0x00018005
#define CRC16modbus  0x00008005
//CRC7
#define CRC_7         7
#define CRC7poly     0x00000089

//#define MCU_Flash_memory_per_page 1024
//#define MAX_EEPROM								128
//#define APPoffset									0x3000
//#define ApplicationAddress    	0x08000000|APPoffset
//#define StartEeprom     				0x08080000  //infomations  //last page midle value line medium-density devices 1-64
//#define EndEeprom   						0x08080FFF //0x0800FFFF
//#define Pagesize    						0x00000100  //256 byte
//#define SNsectionadds 					StartEeprom

typedef void (*pFUNC)(void);
typedef void (*PFuncInit)(unsigned char *s);
typedef void (*pUartx)(unsigned char *s,unsigned short int size);


typedef struct
{
    unsigned char Status;
    unsigned int  counter;
    unsigned int  TMR;
    pFUNC       CBF;
}TimerCBF;

typedef struct
{
    unsigned char Status;
    unsigned int  counter;
    unsigned int  TMR;
//	unsigned int toggle;
}Solftimer;

typedef struct
{
	unsigned char SN[15];
	unsigned char Upgrading_status;
	unsigned char FileNameUpgrade[50];
}EEpromProperty;

typedef struct
{
	double Input;
  double Input_n_minus_1;
	double *pInput;
	double Output;
  double tmp;
	double Alpha;
	unsigned short int NumberSamplePoint;
	unsigned short int counterpiont;
	Solftimer Samplingrate;
}ClassFilter;

typedef struct
{
	Solftimer Timer;
	pFUNC Func;
}TimerToFunc;
typedef enum
{
buffIdle = 0,
}rxbuffer;

//provide Hard interface
typedef struct 
{
	unsigned char Flag_status;
	unsigned short int size;
	unsigned char *pdata;
}General_Buffer;


void usart1_putsSize(unsigned char *s,unsigned short int size);
void usart3_putsSize(unsigned char *s,unsigned short int size);
void usart2_putsSize(unsigned char *s,unsigned short int size);
void BytesToInt(unsigned char *BytesIn,unsigned short int *IntVal);
void BytesToLong(unsigned char* BytesIn,unsigned long* LongVal);//32bit;
void IntToBytes(unsigned short int IntVal,unsigned char* BytesOut);
void CRC16Gen(unsigned char *data,unsigned short int size,unsigned char* CRCResult);
void ModRTU_CRC(unsigned char *dat,unsigned short int size,unsigned char *res);
void ConvertByteInt(unsigned int* IntData,unsigned char ConvertDataType,unsigned char* BytesInOut);
void FloatToBytes(float fValues,unsigned char* BytesOut);
void BytesToFloat(unsigned char* BytesIn,float* FloatValOut);
void Longtobyte(unsigned long input,unsigned char *out);
void StringByteTolong(unsigned char *ByteArray,unsigned char Size,unsigned long *Result);
unsigned int DigitalLotateBit(unsigned int dataint,unsigned char bit);
unsigned char FindArraySet(unsigned char *datain,unsigned int sizein,unsigned char *dataref,unsigned int size);
int StringSearch(unsigned char *datain,unsigned int sizein,unsigned char *dataref,unsigned int size);
unsigned int attachdata(unsigned char *out,unsigned char *ag1,unsigned int size1,unsigned char *ag2,unsigned int size2,unsigned char *ag3,unsigned int size3,unsigned char *ag4,unsigned int size4);
unsigned char Comparearray(unsigned char *dat1,unsigned char *dat2,unsigned int size);
unsigned char SerialNOCRCCheck(unsigned char* SerialNumber,signed char Size);
void Readflash(unsigned int Address,unsigned char *dat,unsigned int size);
unsigned char Writeflash(unsigned int Address,unsigned char *dat,unsigned int size);
void Disassemblytobyte(unsigned char push_F_B,void *datain,unsigned int size,unsigned char *dataout);
void Set_array(unsigned char *data,unsigned short int size,unsigned char value);
void NullFuncvoid(void);
void NullFunc_1ag(void *ag);
void NullFunc_2ag(unsigned char *ag1,unsigned short int ag2);
unsigned char StringToHEX(unsigned char *str);
void CopyArrayCharFunc(unsigned char* DataOri,unsigned char* DataDes,unsigned int Datasize);
unsigned int Copy_String_Between_Text(char *Head,char *Tail,unsigned char *dat,unsigned int size,unsigned char *out);
unsigned int SearchEndData(unsigned char* DataIn,unsigned char EndChar,unsigned int MaxSize);
void FloatToBits(float FloatData,unsigned long* LongOutPut,unsigned char MaxPossearch);
void LEDBlink(void);
//void LEDBlink(void);
unsigned int Packprotocal_ACK(unsigned char CMD1,unsigned char CMD2,unsigned char *data,unsigned int size,unsigned char ACK,unsigned char *out);
//void HightpassFilter(ClassFilter *Instance);
unsigned char Writeflashconfig(unsigned int Address,unsigned char *dat,unsigned int size);
void ConvertStringtoHEX(unsigned char *String,unsigned char size,unsigned char *Output);


