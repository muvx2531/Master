//#include "GeneralFunction.h"
#include "main.h"

#define XbeeModeMasterNode

#define xBee_Buffer_size 512
#define xBeeHeader 0x7E

//#define xBeeModeMasterNode



/********* cmd slave-node query master-node in see *************/
//7E 00 04 08 03 4E 44 62 cmd slave-node query master-node in see
//7E 00 1E 88 03 4E 44 00 00 00 00 13 A2 00 41 B6 D7 B0 20 4E 6F 64 65 31 00 FF FE 00 00 C1 05 10 1E E7
//7E 00 1E 88 03 4E 44 00 00 00 00 13 A2 00 41 B6 D7 B0 20 4E 6F 64 65 31 00 FF FE 00 00 C1 05 10 1E E7
/**************************************************************/

typedef struct{
	unsigned char status;
	unsigned char *pDat;
	
}xBeetransmittKeeper;

typedef enum
{
	idle,
	inprocess,
	success,
	failure,
	error,
	timeout
}xBeestatus;

typedef enum
{
	initparam = 0,
	initmodule,
	NetworkRestart,
	NetworkDiscovery,
	Datareport
}xBeeslaveState;

typedef struct{
	xBeestatus status;
	General_Buffer dat;
}xBeepackage;



#ifndef xBeeModeMasterNode
#define xBeeModeSlaveNode

typedef struct
{
	volatile General_Buffer *RX;
	unsigned char *TX;
	pFUNC xBeePowerOn;
	pFUNC xBeePowerOff;
	xBeeslaveState stateSlaveNode;
	unsigned char MasternodeStatus;
	unsigned char MasternodeID[8];
}InstanceSlavenode;

#endif


void xBeeFunction(void);
void initxBee(void);
void xBeeAPIhandle(uint8_t *dat,uint16_t size);
void xBeeTransmit(xBeepackage *pPackage);
uint8_t xBeeChecksum(uint8_t *pdat,uint16_t size);
void xBeeTestsentdata(uint8_t *pdata,uint16_t size);
void xBeeintervalRSSI(void);
void xBeeTimer(void);
void mainslavenode(void);
//void PowerXbeecontrol_OFF(void);
//void PowerXbeecontrol_ON(void);
xBeestatus xBeeAPIcmdNT(uint16_t timeout);
xBeestatus xBeeAPIcmdNR(uint16_t timeout);
xBeestatus xBeeAPIcmdDIS(uint16_t Timeout);
xBeestatus xBeeAPIcmdSentData(uint8_t *pdata,uint16_t size);




