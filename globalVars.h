#ifndef _CGE_GLOBAL_VARS_H
#define _CGE_GLOBAL_VARS_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
// DS18B20 sensor libs
#include <OneWire.h> 
#include <DallasTemperature.h>

/******************************** structures**************************/
typedef struct SensorQueueElem_s {
  DeviceAddress 	Address;
  uint8_t 			  BusIndex;
  uint8_t			    Resolution;
  float Value;
  char *MqttTopic;
  STAILQ_ENTRY(SensorQueueElem_s) sensorEntry;
} SensorQueueElem_t;

/******************************** queue system ***********************/
typedef STAILQ_HEAD(, SensorQueueElem_s) SensorQueueHead_t;

/******************************** 1-wire sensors *********************/
OneWire gOneWire(TEMP_SENSORS_DATA_PIN); 
DallasTemperature gTempSensors(&gOneWire);
SensorQueueHead_t gSensorQueue;
// whitepaper says :from 9 to 12 bits resolution, max time is: 93.75,187.5,375,750
// add some to be sure the command times are accounted for 
uint16_t gTimeForConversion[MAX_SENSOR_RESOLUTION - MIN_SENSOR_RESOLUTION + 1]= {150,250,450,800};
/******************************** hardware ***************************/
bool gUpdateOrder = false;

/******************************** network ****************************/
WiFiClient   gEspClient;
PubSubClient gMqttClient(gEspClient);

/* CONST VARS */
/**************************************** JSON ************************/
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(10);
#endif
