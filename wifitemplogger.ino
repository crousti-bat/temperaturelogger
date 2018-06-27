// for wifi
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//for MQTT
#include <ArduinoJson.h>
#include <PubSubClient.h>

//for OTA updates
#include <ArduinoOTA.h>

// my settings and global vars
#include "settings.h"
#include "globalVars.h"

/********************************** PIN SETUP   *******************************/
void pinSetup() {
	// set D0 to output (wakeup) and D1 to input (1wire)
  
	//pinMode(D0, OUTPUT);       				
	//pinMode(TEMP_SENSORS_DATA_PIN, INPUT);  

	// onboard led
	pinMode(LED_CARTE, OUTPUT);
}


/********************************** WIFI SETUP ********************************/
void setup_wifi(void) {

  Serial.print("Connecting to ");
  Serial.println(gConfSsid);

  delay(10);
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(gConfSsid, gConfPassword);
  delay(40);
  wl_status_t retCon = WiFi.status() ;

  while (retCon != WL_CONNECTED) {
    WiFi.printDiag(Serial);
    Serial.print("Wifi status: ");
    switch (retCon) {
      case  WL_CONNECTED: 
		Serial.println(" WL_CONNECTED"); break;
      case  WL_NO_SSID_AVAIL: 
		Serial.println(" WL_NO_SSID_AVAIL"); break;
      case  WL_CONNECT_FAILED: 
		Serial.println(" WL_CONNECT_FAILED (maybe wrong pwd)"); break;
      case  WL_IDLE_STATUS: 
		Serial.println(" WL_IDLE_STATUS"); break;
      default: 
		Serial.println("WL_DISCONNECTED"); break;
    }
    delay(10);
    retCon = WiFi.status() ;
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/********************************** OTA SETUP *********************************/
void setup_OTA(void) {

	ArduinoOTA.setPort(OTAPORT);
    ArduinoOTA.setHostname(DEVICENAME);

    // authentication by default
    ArduinoOTA.setPassword(OTAPASSWORD);

    ArduinoOTA.onStart([]() {
      Serial.println("OTA start");
    });
    
    ArduinoOTA.onEnd([]() {
		gUpdateOrder = false;
		Serial.println("OTA end, reboot in 3s");
		delay(3000);
		ESP.restart();
	});
	
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
	
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}
	
/********************************** SETUP *************************************/
void setup() {

	  // serial
	  Serial.begin(115200);

	  //wifi not starting fix
	  WiFi.mode(WIFI_OFF);

	  // setup pins
	  pinSetup();

	  digitalWrite(LED_CARTE, HIGH); // Turn on the on-board LED
	  Serial.println("Starting / waking");
    
/********************************** SENSORS INIT ******************************/
	TemperatureSensorsInit();

/********************************** WIFI SETUP ********************************/
	//setup_wifi();

/********************************** MQTT SETUP ********************************/

	//Serial.println("Wifi connected, reaching for MQTT server");
	//gMqttClient.setServer(gConfMqttServerIp, gConfMqttPort);
	//gMqttClient.setCallback(EnableOtaCallback);

/********************************** OTA SETUP *********************************/
	//setup_OTA();
	
/********************************** WAIT FOR DS18B20 CONVERSION ***************/
	//delay(gTimeForConversion[SENSOR_RESOLUTION - MIN_SENSOR_RESOLUTION]);
  //Serial.print("time to wait for conversion: ");
  //Serial.println(gTimeForConversion[SENSOR_RESOLUTION - MIN_SENSOR_RESOLUTION]);

/********************************** READ SENSORS ******************************/
  gTempSensors.setWaitForConversion(true);
	SensorQueueElem_t *pSensor = NULL;
	STAILQ_FOREACH(pSensor, &gSensorQueue, sensorEntry) {
   
    gTempSensors.requestTemperaturesByAddress(pSensor->Address);

    pSensor->Value = gTempSensors.getTempC(pSensor->Address);
		Serial.print("Sensor ");
		Serial.print(pSensor->MqttTopic);
		Serial.print(" (address: ");
    Serial.print(pSensor->Address[0],HEX);
    Serial.print(pSensor->Address[1],HEX);
    Serial.print(pSensor->Address[2],HEX);
    Serial.print(pSensor->Address[3],HEX);
    Serial.print(pSensor->Address[4],HEX);
    Serial.print(pSensor->Address[5],HEX);
    Serial.print(pSensor->Address[6],HEX);
    Serial.print(pSensor->Address[7],HEX);
    Serial.print(") parasitic power mode ");
    if (gTempSensors.isParasitePowerMode())
      Serial.println("ON ");
    else
      Serial.println("OFF ");
    Serial.print("temperature reading: ");
    Serial.println(pSensor->Value);
	} 

/********************************** SEND DATA TO MQTT SERVER ******************/	
	//SendMeasuresOnMqtt();
	
/********************************** GO DEEP SLEEP IF NO UPDATE*****************/
	if (!gUpdateOrder) {	
		digitalWrite(LED_CARTE,LOW); // Turn off the on-board LED
		//delay (SLEEP_TIME_S * 1000);
		ESP.deepSleep( SLEEP_TIME_S * 1000000);
	}  
} 

/********************************** SETUP END *********************************/
	
/********************************** START MAIN LOOP****************************/
void loop() {

/* we only enter main loop for OTA */
    ArduinoOTA.handle();
}

