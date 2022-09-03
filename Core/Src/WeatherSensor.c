#include <stdio.h>
#include "WeatherSensor.h"
#include "string.h"


//#define raw_json "{\"name\":\"Node1\",\"Bat\":3.65,\"RSSI\":51,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":1,\"WD\":291,\"Rain\":0}{\"name\":\"Node2\",\"Bat\":4.65,\"RSSI\":51,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":1,\"WD\":291,\"Rain\":0}{\"name\":\"Node3\",\"Bat\":5.65,\"RSSI\":51,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":1,\"WD\":291,\"Rain\":0}"

//char raw[] = {0x00,0x02,0x24,0x2C,0x0C,0xC1,0x15,0xB3,0x01,0x39,0x00,0x01,0x23,0x45,0x00,0x0B,0x01,0x03,0x01,0x02};

//const char *JsonHeaderdatasensor[]={Name,Batterry,RSSI,Temperature,Humidity,Pressure,AmbientLight,Dustsensor,WindSpeed,WindDirection,Rainrate};



Nodeweatherdata NodeSensordata[Maxnodesize]={{.MarkNode = 1,.JSON = "{\"name\":\"Node1\",\"Bat\":99.65,\"RSSI\":51,\"Temp\":29.35,\"Hum\":31.00,\"Prs\":305,\"ALS\":457,\"PM25\":0,\"WS\":0,\"WD\":0,\"Rain\":0}\0"},
																						{.MarkNode = 2,.JSON =  "{\"name\":\"Node2\",\"Bat\":99.65,\"RSSI\":49,\"Temp\":29.78,\"Hum\":32.08,\"Prs\":302,\"ALS\":433,\"PM25\":0,\"WS\":0,\"WD\":0,\"Rain\":0}\0"},
																						{.MarkNode = 3,.JSON =  "{\"name\":\"Node3\",\"Bat\":99.70,\"RSSI\":48,\"Temp\":29.27,\"Hum\":30.12,\"Prs\":307,\"ALS\":428,\"PM25\":0,\"WS\":0,\"WD\":0,\"Rain\":0}\0"}};



void Initweather(void)
{
	uint16_t i,n;
	for (i=0;i<3;i++)
	{
		NodeSensordata[i].tNode.Status = DISABLE;
		NodeSensordata[i].tNode.TMR	= 30000;
		for (n=0;n<JsonNodeMaxsize;n++)
		{
			if(NodeSensordata[i].JSON[n] == 0)
			{
				NodeSensordata[i].size = n;
				
				break;
			}
		}
	}
}	
																						
void main_weatherdataprocess(void)
{
	//DecodeWeatherNodesensor((uint8_t*)raw,sizeof(raw),NodeSensordata);
}

  uint16_t NodeID;
	uint8_t Bat;
	uint8_t nRSSI;
	volatile float Temp;
	volatile float Hum;

	volatile uint16_t tt;
	volatile uint16_t hh;
	uint16_t Prs;
	uint32_t ALS;
	uint16_t PM25;
	uint16_t WS;
	uint16_t WD;
	float Rain;

uint8_t DecodeWeatherNodesensor(uint8_t *pdata,uint16_t size,Nodeweatherdata *node){
	
	uint8_t i;
	
 static uint8_t ran;

	
	uint8_t ret=0;
	for(i=0;i<Maxnodesize;i++){
		int16_t index=0;

		NodeID = ((uint16_t)pdata[0]<<8)|(((uint16_t)pdata[1])&0x00FF);
		if(NodeSensordata[i].MarkNode == NodeID){
			ret++;
			ran = rand();
			Bat = pdata[2];
			nRSSI = pdata[3];
			tt = ((uint16_t)pdata[4]<<8|(uint16_t)pdata[5])/100;
			hh = ((uint16_t)pdata[6]<<8|(uint16_t)pdata[7])/100;
			Prs  = (uint16_t)pdata[8]<<8|(uint16_t)pdata[9];
			ALS  = (uint32_t)pdata[10]<<24|(uint32_t)pdata[11]<<16|(uint32_t)pdata[12]<<8|(uint32_t)pdata[13];
			PM25 = ((uint16_t)pdata[14]<<8|(uint16_t)pdata[15])/100;
			WS  = (uint16_t)pdata[16]<<8|(uint16_t)pdata[17];
			WD  = (uint16_t)pdata[18]<<8|(uint16_t)pdata[19];
			Rain = (float)((uint16_t)pdata[20]<<8|(uint16_t)pdata[21])/100.0f;
			
//			NodeSensordata[i].size = 0;
//			NodeSensordata[i].JSON[NodeSensordata[i].size]= '{';
//			NodeSensordata[i].size++;
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":\"Node%d\",",Name,NodeID);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",Batterry,Bat);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",nodeRSSI,nRSSI);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%2.2f,",Temperature,Temp);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%2.2f,",Humidity,Hum);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",Pressture,Prs);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%u,",Ambient,ALS);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",DustPM25,PM25);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",WindSpeed,WS);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",WindDirection,WD);
//			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%2.2f",Rainrate,Rain);
//			NodeSensordata[i].JSON[NodeSensordata[i].size]= '}';
//			NodeSensordata[i].size++;


			NodeSensordata[i].size = 0;
			NodeSensordata[i].JSON[NodeSensordata[i].size]= '{';
			NodeSensordata[i].size++;
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":\"Node%d\",",Name,NodeID);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",Batterry,Bat);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",nodeRSSI,nRSSI);
			ran = rand()/25;
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d.%d,",Temperature,tt,ran);
			ran = rand()/25;
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d.%d,",Humidity,hh,ran);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",Pressture,Prs);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%u,",Ambient,ALS);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",DustPM25,PM25);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",WindSpeed,WS);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":%d,",WindDirection,WD);
			NodeSensordata[i].size += sprintf(NodeSensordata[i].JSON+NodeSensordata[i].size,"\"%s\":0",Rainrate);
			NodeSensordata[i].JSON[NodeSensordata[i].size]= '}';
			NodeSensordata[i].size++;

			
//			NodeSensordata[i].size = sprintf((char*)NodeSensordata[i].JSON,
//				"{\"name\":\"Node%d\",\"Bat\":%d,\"RSSI\":%d,\"Temp\":%2.2f,\"Hum\":%2.2f,\"Prs\":%d,\"ALS\":%d,\"PM25\":%2.2f,\"WS\":%d,\"WD\":%d,\"Rain\":%2.2f}",
//				NodeID,
//				Bat,
//				nRSSI,
//				Temp,
//				Hum,
//				Prs,
//				ALS,
//				PM25,
//				WS,
//				WD,
//				Rain
//			);		

			//clear time out
//			if(NodeID == 1)
//			{
//				NodeSensordata[0].tNode.counter = 0;
//				NodeSensordata[0].tNode.Status = ENABLE;
//			}
			NodeSensordata[i].tNode.counter = 0;
			NodeSensordata[i].tNode.Status = ENABLE;
		}
	}
	return ret;
}


//uint16_t SearchString(uint8_t *in,uint16_t size,char *ref,uint16_t sizeref)
//{
//	
//}

void WeatherProcesstimer(void)
{

	if(NodeSensordata[0].tNode.Status == ENABLE)
	{
		NodeSensordata[0].tNode.counter++;
		if(NodeSensordata[0].tNode.counter >= NodeSensordata[0].tNode.TMR)
		{
			NodeSensordata[0].tNode.Status = DISABLE;
			NodeSensordata[0].tNode.counter = 0;
		}
	}

	if(NodeSensordata[1].tNode.Status == ENABLE)
	{
		NodeSensordata[1].tNode.counter++;
		if(NodeSensordata[1].tNode.counter >= NodeSensordata[1].tNode.TMR)
		{
			NodeSensordata[1].tNode.Status = DISABLE;
			NodeSensordata[1].tNode.counter = 0;
		}
	}
	
	if(NodeSensordata[2].tNode.Status == ENABLE)
	{
		NodeSensordata[2].tNode.counter++;
		if(NodeSensordata[2].tNode.counter >= NodeSensordata[2].tNode.TMR)
		{
			NodeSensordata[2].tNode.Status = DISABLE;
			NodeSensordata[2].tNode.counter = 0;
		}
	}
	
	
}




