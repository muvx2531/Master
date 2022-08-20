#include "main.h"
#include <stdint.h>

/*{"name":"Node1","Bat":3.65,"RSSI":51,"Temp":29.170000,"Hum":33.002998,"Prs":306,"ALS":0,"PM25":0,"WS":1,"WD":291,"Rain":0}*/

#define Name "name"
#define Batterry "Bat"
#define RSSI	"RSSI"
#define Temperature "Temp"
#define Humidity "Hum"
#define Pressure "Prs"
#define AmbientLight "ALS"
#define Dustsensor	"PM25"
#define WindSpeed		"WS"
#define WindDirection "WD"
#define Rainrate		"Rain"


#define JsonNodeMaxsize	150	

#define Maxnodesize 3
#define NodeMask_1 "{\"name\":\"Node1\""
#define NodeMask_2 "{\"name\":\"Node2\""
#define NodeMask_3 "{\"name\":\"Node3\""
typedef struct
{
	const char *MarkNode;
	int8_t JSON[JsonNodeMaxsize];
	uint16_t size;
	
//	float nTemperature;
//	float nHumidity;
//	uint32_t nPressure;
	
	
}Nodeweatherdata;


void Initweather(void);
void main_weatherdataprocess(void);
uint8_t DecodeWeatherNodesensor(uint8_t *pdata,uint16_t size,Nodeweatherdata *node);



















