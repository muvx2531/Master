#include "WeatherSensor.h"
#include "string.h"

#define raw_json "{\"name\":\"Node1\",\"Bat\":3.65,\"RSSI\":51,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":1,\"WD\":291,\"Rain\":0}{\"name\":\"Node2\",\"Bat\":4.65,\"RSSI\":51,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":1,\"WD\":291,\"Rain\":0}{\"name\":\"Node3\",\"Bat\":5.65,\"RSSI\":51,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":1,\"WD\":291,\"Rain\":0}"



//const char *JsonHeaderdatasensor[]={Name,Batterry,RSSI,Temperature,Humidity,Pressure,AmbientLight,Dustsensor,WindSpeed,WindDirection,Rainrate};



Nodeweatherdata NodeSensordata[Maxnodesize]={{.MarkNode = NodeMask_1,.JSON = "{\"name\":\"Node1\",\"Bat\":3.65,\"RSSI\":51,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":0,\"WD\":0,\"Rain\":0}\0"},
																						{.MarkNode = NodeMask_2,.JSON =  "{\"name\":\"Node2\",\"Bat\":4.65,\"RSSI\":49,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":0,\"WD\":0,\"Rain\":0}\0"},
																						{.MarkNode = NodeMask_3,.JSON =  "{\"name\":\"Node3\",\"Bat\":3.70,\"RSSI\":48,\"Temp\":29.170000,\"Hum\":33.002998,\"Prs\":306,\"ALS\":0,\"PM25\":0,\"WS\":0,\"WD\":0,\"Rain\":0}\0"}};



void Initweather(void)
{
	uint16_t i,n;
	for (i=0;i<3;i++)
	{
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
	//DecodeWeatherNodesensor((uint8_t*)raw_json,strlen(raw_json),NodeSensordata);
}


uint8_t DecodeWeatherNodesensor(uint8_t *pdata,uint16_t size,Nodeweatherdata *node){
	
	uint8_t i;
	
	for(i=0;i<Maxnodesize;i++){
		int16_t index=0;
		index = StringSearch(pdata,size,(unsigned char*)NodeSensordata[i].MarkNode,strlen((char*)NodeSensordata[i].MarkNode));
		if(index != -1){
			NodeSensordata[i].size = 0;
			uint16_t sub_index=0;
			int16_t braces=0;
			for(sub_index=0;sub_index<JsonNodeMaxsize;sub_index++){
				NodeSensordata[i].JSON[sub_index] = pdata[index+sub_index];
				NodeSensordata[i].JSON[sub_index+1] = 0;
				if(NodeSensordata[i].JSON[sub_index] == '{')braces++;
				else if(NodeSensordata[i].JSON[sub_index] == '}')braces--;
				NodeSensordata[i].size++;
				if(braces == 0)break;
			}
			
			/* 
			//Convert string to number
			//sub_ind ex = StringSearch(pdata+index,size+index,(unsigned char*)Batterry,strlen((char*)Batterry));
			*/
		}
	}
}


uint16_t SearchString(uint8_t *in,uint16_t size,char *ref,uint16_t sizeref)
{
	
}




