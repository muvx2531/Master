#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
//#include "stm32f4xx_nucleo_144.h"
#include "xBee.h"
#include "WeatherSensor.h"




typedef struct{
	uint8_t networkstatus;
	uint16_t RSSI;
}xBeeinfo;


extern UART_HandleTypeDef huart3;
extern IWDG_HandleTypeDef hiwdg;
//extern RTC_HandleTypeDef hrtc;
//extern float temperature, humidity;
/***************** xBee module Hardware *******************/
unsigned char xBee_RX_buffer[xBee_Buffer_size];
unsigned char xBee_TX_buffer[xBee_Buffer_size];
//volatile General_Buffer HAL_xBeeRX;
xBeepackage HAL_xBeeTX;

UxSerial_Handle xBeeSerial={.Serial = &huart3,.RX = xBee_RX_buffer,.Maxsize = xBee_Buffer_size,.size = 0,.RXSplitters ={.state = Idle,.tmr = 10,.counter = 0}};
/*********************************************************/


/******************* Genneral timer **********************/
Solftimer xBee_Test_intevalsent={DISABLE,0,5000};
volatile Solftimer xBeeAPICMDTimeout={DISABLE,0,3000};
TimerToFunc xBeeRSSItimer={{DISABLE,0,5000},&xBeeintervalRSSI};


Solftimer tResetAll={ENABLE,0,60000};
/*********************************************************/


/**********************************************************/
extern char Nodename[10];
extern 	double temperature;
extern 	double humd;
extern 	uint16_t press;
extern 	uint32_t UV;
extern 	uint16_t dust;
extern 	double windsp;
extern 	uint16_t windi;
extern 	uint16_t rainrate;
/**********************************************************/

#ifndef XbeeModeMasterNode
/****************** slave node parameter *****************/
//InstanceSlavenode Nodesenseor=.RX = &HAL_xBeeRX,.TX = xBee_TX_buffer,.xBeePowerOff = &PowerXbeecontrol_OFF ,.xBeePowerOn = &PowerXbeecontrol_ON,.MasternodeStatus = DISABLE};
InstanceSlavenode Nodesenseor;
/*********************************************************/
#else

#endif

/***************** xBee RSSI****************/
xBeeinfo xBeenodeinfo;
/*******************************************/


/***************** xBee Global variable****************/
extern Nodeweatherdata NodeSensordata[Maxnodesize];

uint8_t Flagcmd_NT = DISABLE;
uint8_t Flagcmd_NR = DISABLE;
uint8_t Flagcmd_DIS = DISABLE;
uint8_t Flagcmd_Sent = DISABLE;
/******************************************************/


void initxBee(void)
{
	
	HAL_xBeeTX.status = idle;
	HAL_xBeeTX.dat.pdata = xBee_TX_buffer;
	HAL_xBeeTX.dat.size  = 0;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);// Enable 3.3V xbee
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);// Enable 3.3V xbee
	//HAL_Delay(200);

	//Nodesenseor.stateSlaveNode = initparam;
}

void xBeeConnectionKeeper(void)
{
	if((CheckNodeCaonnected(1) == True) /*&& (CheckNodeCaonnected(2) == True)*/ && (CheckNodeCaonnected(3) == True))
	{
		tResetAll.counter = 0;
		tResetAll.Status = ENABLE;
		//HAL_IWDG_Refresh(&hiwdg);
	}
	//else {if(tResetAll.Status == 0x02)while(1);}
}

void xBeeFunction(void)
{
	uint16_t i;
  __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE); //Enable Uart flag RXNE
	
	//xBee recieved handle
	if(xBeeSerial.RXSplitters.state ==  RxReady)
	{
		//HAL_UART_Transmit(&huart3,xBeeSerial.RX,xBeeSerial.size,100);
		xBeeAPIhandle(xBeeSerial.RX,xBeeSerial.size);
		xBeeSerial.size = 0;
		xBeeSerial.RXSplitters.state =  Idle;
		for(i=0;i<xBee_Buffer_size;i++)xBeeSerial.RX[i]= 0;
	}
	
	//xBeeConnectionKeeper();
	
	
//	HAL_UART_Transmit(&UART_xBee_Handle,"test\r\n",6,100);
#ifndef XbeeModeMasterNode
	mainslavenode();
#endif
//	HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);
	
//	/************* xBee  process ***********/
//	jsonsize = sprintf((char*)jsonbuffer,"\r\n{\"ID\" : \"N0001\",\"ver\" :1,\"DT\" : \"%02d/%02d/20%02d %02d:%02d:%02d.%03d\",\"Temp\" :%.1f,\"Hum\" :%.1f,\"RSSI\" :-%d dBm\"text buck data \" : \"THIS IS TEST BUCK DATA 0123456789ABCDEF message message message kainaoa mouyjung longdear\"}\r\n",
//	currDate.Date,
//	currDate.Month,
//	currDate.Year+2000,
//	currTime.Hours,
//	currTime.Minutes,
//	currTime.Seconds,
//	1000 * (currTime.SecondFraction - currTime.SubSeconds) / (currTime.SecondFraction + 1),
//	temperature,
//	humidity,
//	xBeenodeinfo.RSSI
//	);
//	if(xBee_Test_intevalsent.Status == DISABLE)
//	{
//		jsonsize = sprintf((char*)jsonbuffer,"Test %d\r\n",xBeenodeinfo.RSSI);
//		//TRACE_INFO("%s",jsonbuffer);
//		xBeeTestsentdata(jsonbuffer,jsonsize);
//		xBee_Test_intevalsent.Status = ENABLE;
//	}
	//HAL_Delay(500);
//	/***************************************/
	
	//xBee trancmitt handle
	//xBeeTransmit(&HAL_xBeeTX);
}


#ifndef XbeeModeMasterNode

void mainslavenode(void)
{
	xBeestatus result=idle;
	static uint8_t error = 0;
	uint16_t jsonsize;
	uint8_t jsonbuffer[100];
	switch(Nodesenseor.stateSlaveNode)
	{
	case initparam:
		//init API AT
		Nodesenseor.xBeePowerOff(); //Enable xbee power
		Nodesenseor.stateSlaveNode = initmodule;
		HAL_Delay(5000);
		Nodesenseor.xBeePowerOn(); //Enable xbee power
		Nodesenseor.MasternodeStatus = DISABLE;
		error = 0;
		break;

	case initmodule:
		result = xBeeAPIcmdNT(3000); //simple AT cmd
		if(result == success)Nodesenseor.stateSlaveNode = NetworkRestart;
		else if(result == timeout)Nodesenseor.stateSlaveNode = initparam;
		break;

	case NetworkRestart:
		result = xBeeAPIcmdNR(3000);//cmd restart network
		if(result == success)Nodesenseor.stateSlaveNode = NetworkDiscovery;
		else if(result == timeout)Nodesenseor.stateSlaveNode = initparam;
		break;

	case NetworkDiscovery:
		result = xBeeAPIcmdDIS(5000);//cmd Discovery network
		if(result == timeout)
		{
			error++;
			if(error >= 5){
				error = 0;
				Nodesenseor.stateSlaveNode = initmodule;
			}
		}

		if(Nodesenseor.MasternodeStatus == ENABLE){
			error = 0;
			Nodesenseor.stateSlaveNode = Datareport;
		}
		break;

	case Datareport:
			if(xBee_Test_intevalsent.Status == DISABLE)
			{
				jsonsize = sprintf((char*)jsonbuffer,"Test %d\r\n",xBeenodeinfo.RSSI);
				xBeeAPIcmdSentData(jsonbuffer,jsonsize);
				xBee_Test_intevalsent.Status = ENABLE;//
			}
			if(xBee_Test_intevalsent.Status == 0x02)
			{
				if(Flagcmd_Sent != ENABLE)
				{
					error++;
					if(error >= 30)
					{
						error = 0;
						Nodesenseor.stateSlaveNode = initmodule;
					}
				}
				else
				{
					error = 0;
					Flagcmd_Sent = DISABLE;
				}

				xBee_Test_intevalsent.Status = DISABLE;
			}
			//Flagcmd_Sent
		break;

	default:
		break;
	}
}
#endif


void xBeeAPIhandle(uint8_t *dat,uint16_t size)
{
	uint16_t i;
	uint8_t checksum;
	uint16_t cmdsize;
	//uint8_t getFlag_

	//uint8_t checksum=0;
	for(i=0;i<size;i++)
	{
		if(dat[i] == xBeeHeader)
		{
			cmdsize = (uint16_t)dat[i+1]<<8|(uint16_t)dat[i+2];
			if(cmdsize < xBee_Buffer_size)
			{
				checksum=xBeeChecksum(dat+i+3,cmdsize);
				if(dat[i+3+cmdsize] == checksum)
				{
	//				xBeeTestsentdata("stm32 acho",10);
					if(dat[i+3] == 0x90)//Receive Packet - 0x90 .,data start at 15
					{
					  DecodeWeatherNodesensor(dat+i+15,cmdsize,NodeSensordata);
					}
					else if(dat[i+3] == 0x88)//Local AT Command Response - 0x88 //cmd sent >>7E 00 04 08 01 4E 54 54
					{
						if((dat[i+5] == 0x4E) && (dat[i+6] == 0x44))
						{

#ifndef XbeeModeMasterNode
							if(dat[i+7] == 0x00){
								for(i=0;i<8;i++)Nodesenseor.MasternodeID[i] = dat[i+10];
								Nodesenseor.MasternodeStatus = ENABLE;
							}
#endif
						}
						else if((dat[i+5] == 0x4E) && (dat[i+6] == 0x54))
						{
							if(dat[i+7] == 0x00)Flagcmd_NT = ENABLE;
						}
						else if((dat[i+5] == 0x4E) && (dat[i+6] == 0x52))
						{
							if(dat[i+7] == 0x00)Flagcmd_NR = ENABLE;
						}
						else if((dat[i+5] == 0x44) && (dat[i+6] == 0x42))
						{
							xBeenodeinfo.RSSI = (uint16_t)dat[i+7]<<8|dat[i+8];
						}
					}
					else if(dat[i+3] == 0x8A) //Device associate
					{

#ifndef XbeeModeMasterNode
						if(dat[i+3] == 0x43)//device Open join window
						{
							Nodesenseor.stateSlaveNode = NetworkDiscovery;
							Nodesenseor.MasternodeStatus = DISABLE;
							xBeeAPICMDTimeout.Status = DISABLE;
							xBeeAPICMDTimeout.counter = 0;
						}
						else if(dat[i+3] == 0x44)// closed and join window
						{
							Nodesenseor.stateSlaveNode = NetworkRestart;
							Nodesenseor.MasternodeStatus = DISABLE;
							xBeeAPICMDTimeout.Status = DISABLE;
							xBeeAPICMDTimeout.counter = 0;
						}
#endif
					}
					else if(dat[i+3] == 0x8B) //transmitted data status
					{
						if(dat[i+8] == 0x00)//device Open join window
						{
							Flagcmd_Sent = ENABLE;
						}
					}
					else
					{
						if((dat[i+5] == 0x4E) && (dat[i+6] == 0x44))
						{

						}
						else if((dat[i+5] == 0x44) && (dat[i+6] == 0x42))
						{

						}
					}
				}
			}
		}
	}
}

void xBeeTransmit(xBeepackage *pPackage)
{
	if(pPackage->status == inprocess)
	{
		HAL_UART_Transmit(&huart3,pPackage->dat.pdata,pPackage->dat.size,100);
		pPackage->status = idle;
	}
}

	
uint8_t xBeeChecksum(uint8_t *pdat,uint16_t size)
{
	uint16_t i;
  uint8_t sum=0;
	for(i=0;i<size;i++)
	{
		sum = sum+pdat[i];
	}
	sum = 0xFF-sum;
	return sum;
}

void xBeeTestsentdata(uint8_t *pdata,uint16_t size)
{
	uint16_t i;
	uint8_t packagebuffer[512];
	uint8_t Id_destination[8]={0x00,0x13,0xA2,0x00,0x41,0xB6,0xD7,0xB0};
	uint16_t sumsize;
//    uint8_t checksum=0;
//	static uint8_t packageID=0;
	sumsize = size+14;
	
	packagebuffer[0] = 0x7E;
	packagebuffer[1] = sumsize>>8;
	packagebuffer[2] = sumsize&0x00FF;
	packagebuffer[3] = 0x10;
	packagebuffer[4] = 0x01;
	for (i=0;i<8;i++)
	{
		packagebuffer[5+i] = Id_destination[i];
	}
	packagebuffer[13] = 0xFF;
	packagebuffer[14] = 0xFE;
	packagebuffer[15] = 0x00;
	packagebuffer[16] = 0x00;
	for (i=0;i<size;i++)
	{
		packagebuffer[17+i] = pdata[i];
	}
	packagebuffer[sumsize+3] = xBeeChecksum(&packagebuffer[3],sumsize);
	sumsize+=4;
	
	HAL_UART_Transmit(&huart3,packagebuffer,sumsize,100);
}


void xBeeintervalRSSI(void)
{
	//7e 00 04 08 52 44 42 1F
	uint8_t cmd[]={0x7e,0x00,0x04,0x08,0x52,0x44,0x42,0x1F};
	HAL_UART_Transmit(&huart3,cmd,8,100);
}

void PowerXbeecontrol_OFF(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);// Disable 3.3V xbee
}

void PowerXbeecontrol_ON(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);// Enable 3.3V xbee
}

void xBeechecknetwork(void)
{

}

xBeestatus xBeeAPIcmdNT(uint16_t Timeout){

	uint8_t protocol[] = {0x7E,0x00,0x04,0x08,0x83,0x4E,0x54,0xD2};
	if(Flagcmd_NT == DISABLE){
		if(xBeeAPICMDTimeout.Status == DISABLE){
			xBeeAPICMDTimeout.TMR = Timeout;
			xBeeAPICMDTimeout.counter = 0;
			xBeeAPICMDTimeout.Status = ENABLE;
			HAL_UART_Transmit(&huart3,protocol,8,100);
		}
		else if(xBeeAPICMDTimeout.Status == 0x02){
			xBeeAPICMDTimeout.Status = DISABLE;
			xBeeAPICMDTimeout.counter = 0;
			return timeout;
		}
	}
	else if(Flagcmd_NT == ENABLE){
		xBeeAPICMDTimeout.Status = DISABLE;
		xBeeAPICMDTimeout.counter = 0;
		Flagcmd_NT = DISABLE;
		return success;
	}
	return inprocess;
}


xBeestatus xBeeAPIcmdNR(uint16_t Timeout){

	uint8_t protocol[] = {0x7E,0x00,0x04,0x08,0x01,0x4E,0x52,0x56};
	if(Flagcmd_NR == DISABLE){
		if(xBeeAPICMDTimeout.Status == DISABLE){
			xBeeAPICMDTimeout.Status = ENABLE;
			xBeeAPICMDTimeout.TMR = Timeout;
			xBeeAPICMDTimeout.counter = 0;
			HAL_UART_Transmit(&huart3,protocol,8,100);
		}
		else if(xBeeAPICMDTimeout.Status == 0x02){
			xBeeAPICMDTimeout.Status = DISABLE;
			xBeeAPICMDTimeout.counter = 0;
			return timeout;
		}
	}
	else if(Flagcmd_NR == ENABLE){
		xBeeAPICMDTimeout.Status = DISABLE;
		xBeeAPICMDTimeout.counter = 0;
		Flagcmd_NR = DISABLE;
		return success;
	}
	return inprocess;
}

xBeestatus xBeeAPIcmdDIS(uint16_t Timeout){

	uint8_t protocol[8] = {0x7E,0x00,0x04,0x08,0x01,0x4E,0x44,0x64};

		if(xBeeAPICMDTimeout.Status == DISABLE)
		{
			xBeeAPICMDTimeout.Status = ENABLE;
			xBeeAPICMDTimeout.TMR = Timeout;
			xBeeAPICMDTimeout.counter = 0;
			HAL_UART_Transmit(&huart3,protocol,8,100);
		}
		else if(xBeeAPICMDTimeout.Status == 0x02){
			xBeeAPICMDTimeout.Status = DISABLE;
			xBeeAPICMDTimeout.counter = 0;
			return timeout;
		}
	return inprocess;
}

#ifndef XbeeModeMasterNode
xBeestatus xBeeAPIcmdSentData(uint8_t *pdata,uint16_t size)
{
	uint16_t i;
	uint8_t packagebuffer[512];
	//uint8_t Id_destination[8]={0x00,0x13,0xA2,0x00,0x41,0xB6,0xD7,0xB0};
	uint16_t sumsize;
    uint8_t checksum=0;
	static uint8_t packageID=0;
	sumsize = size+14;



	packagebuffer[0] = 0x7E;
	packagebuffer[1] = sumsize>>8;
	packagebuffer[2] = sumsize&0x00FF;
	packagebuffer[3] = 0x10;
	packagebuffer[4] = 0x01;
	for (i=0;i<8;i++)
	{
		packagebuffer[5+i] = Nodesenseor.MasternodeID[i];
	}
	packagebuffer[13] = 0xFF;
	packagebuffer[14] = 0xFE;
	packagebuffer[15] = 0x00;
	packagebuffer[16] = 0x00;
	for (i=0;i<size;i++)
	{
		packagebuffer[17+i] = pdata[i];
	}
	packagebuffer[sumsize+3] = xBeeChecksum(&packagebuffer[3],sumsize);
	sumsize+=4;

	HAL_UART_Transmit(&UART_xBee_Handle,packagebuffer,sumsize,100);
}
#endif

void xBeeTimer(void)
{
	if(xBeeRSSItimer.Timer.Status == ENABLE)
	{
		xBeeRSSItimer.Timer.counter++;
		if(xBeeRSSItimer.Timer.counter >= xBeeRSSItimer.Timer.TMR)
		{
			xBeeRSSItimer.Timer.counter = 0;
			xBeeRSSItimer.Func();
		}
	}
	if(xBee_Test_intevalsent.Status == ENABLE)
	{
		xBee_Test_intevalsent.counter++;
		if(xBee_Test_intevalsent.counter >= xBee_Test_intevalsent.TMR)
		{
			xBee_Test_intevalsent.counter = 0;
			xBee_Test_intevalsent.Status = 0x02;
		}
	}

	if(xBeeAPICMDTimeout.Status == ENABLE)
	{
		xBeeAPICMDTimeout.counter++;
		if(xBeeAPICMDTimeout.counter >= xBeeAPICMDTimeout.TMR)
		{
			xBeeAPICMDTimeout.counter = 0;
			xBeeAPICMDTimeout.Status = 0x02;
		}
	}
	
	if(xBeeSerial.RXSplitters.state == Recieving)
	{
		xBeeSerial.RXSplitters.counter++;
		if(xBeeSerial.RXSplitters.counter >= xBeeSerial.RXSplitters.tmr)
		{
			xBeeSerial.RXSplitters.counter = 0;
			xBeeSerial.RXSplitters.state = RxReady;
		}
	}
	
	if(tResetAll.Status == ENABLE)
	{
		tResetAll.counter++;
		if(tResetAll.counter >= tResetAll.TMR)
		{
			tResetAll.counter = 0;
			tResetAll.Status = 0x02;
		}
	}
}


void xBee_IRQHandler(void)
{
		if(((xBeeSerial.Serial->Instance->SR & USART_SR_RXNE) != RESET) && ((xBeeSerial.Serial->Instance->CR1 & USART_CR1_RXNEIE) != RESET))
	{
		if(xBeeSerial.RXSplitters.state == Idle)xBeeSerial.RXSplitters.state = Recieving;
		if(xBeeSerial.RXSplitters.state == Recieving)
		{
			if(xBeeSerial.size < xBeeSerial.Maxsize)
			{
				xBeeSerial.RX[xBeeSerial.size] = (unsigned char)READ_REG(xBeeSerial.Serial->Instance->DR);
				xBeeSerial.size++;
				xBeeSerial.RXSplitters.counter = 0;
			}
			(void)READ_REG(xBeeSerial.Serial->Instance->DR);

		}
	}
	(void)READ_REG(xBeeSerial.Serial->Instance->DR);
}

