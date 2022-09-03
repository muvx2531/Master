#include "main.h"
#include <stdint.h>

/*{"name":"Node1","Bat":3.65,"RSSI":51,"Temp":29.170000,"Hum":33.002998,"Prs":306,"ALS":0,"PM25":0,"WS":1,"WD":291,"Rain":0}*/


//KEY json weather
#define Name 						"name"
//#define gpsDate						"dat"
//#define gpsTime						"Tim"
#define Batterry 					"Bat"
#define nodeRSSI					"RSSI"
#define Temperature 				"Temp"
#define Humidity 					"Hum"
#define Pressture  					"Prs"
#define Ambient						"ALS"
#define DustPM25					"PM25"
#define WindSpeed					"WS"
#define WindDirection				"WD"
#define Rainrate					"Rain"

#define JsonNodeMaxsize	512	

#define Maxnodesize 3
//#define NodeMask_1 "{\"name\":\"Node1\""
//#define NodeMask_2 "{\"name\":\"Node2\""
//#define NodeMask_3 "{\"name\":\"Node3\""

typedef struct
{
	uint16_t MarkNode;
	char JSON[JsonNodeMaxsize];
	uint16_t size;
	
	Solftimer tNode;
	
//	float nTemperature;
//	float nHumidity;
//	uint32_t nPressure;
	
	
}Nodeweatherdata;


void Initweather(void);
void main_weatherdataprocess(void);
uint8_t DecodeWeatherNodesensor(uint8_t *pdata,uint16_t size,Nodeweatherdata *node);
void WeatherProcesstimer(void);


















