#ifndef _CGE_SETTINGS_EMPTY_H
#define _CGE_SETTINGS_EMPTY_H

/* DEFINES */
/* remove this line to be able to compile. Please do it once 
 *  mac address is set */



/**************************** OTA **************************************************/
#define DEVICENAME "wifitemplogger" //change this to whatever you want to call your device
#define OTAPASSWORD "otapwd" //the gConfPassword you will need to enter to upload remotely via the ArduinoIDE
#define OTAPORT 8800
#define OTA_MQTT_KEYWORD "UPDATE_ORDER"

/******************************* MQTT **********************************************/
#define MQTT_MAX_PACKET_SIZE 512

/******************************** HARDWARE *******************************************/
#define LED_CARTE 				  BUILTIN_LED
#define WAKEUP_PIN			  	  D0
#define TEMP_SENSORS_DATA_PIN 	  D1

/******************************** SENSORS ******************************************/
#define NOMINAL_SENSOR_COUNT 3
#define MIN_SENSOR_RESOLUTION 9
#define MAX_SENSOR_RESOLUTION 12
#define SENSOR_RESOLUTION 10

/************ WIFI and MQTT Information (CHANGE THESE FOR YOUR SETUP) **************/
const char* gConfSsid = "homesensors"; //type your WIFI information inside the quotes
const char* gConfPassword = "pwd";
const char* gConfMqttServerIp = "192.168.10.2";
const char* gConfMqttUsername = "mqttuser";
const char* gConfMqttPassword = "mqttpwd";
const int   gConfMqttPort = 1883;

/************* MQTT TOPICS (change these topics as you wish)  **********************/
#define MQTT_PREFIX "TempSensor"
#define MQTT_ROOM "kitchen"
#define MQTT_APPLIANCE "fridge"


/**************************************** NETWORK **********************************/

// the media access control (ethernet hardware) address for the shield:
const byte gConfMacAddress[] = { 0xFA, 0xE1, 0x90, 0x00, 0x00, 0x01 }; 


/**************************************** SLEEP **********************************/
#define SLEEP_TIME_S 10


#endif
