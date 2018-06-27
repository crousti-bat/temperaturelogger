// my settings and global vars
#include "settings.h"
#include "globalVars.h"

/**************************** sensors init and measure order ********************/	
void TemperatureSensorsInit(void)
{
	STAILQ_INIT(&gSensorQueue);
	gTempSensors.begin();
	
	uint8_t sensorCount = gTempSensors.getDeviceCount();
	SensorQueueElem_t *pSensor = NULL;
	for(uint8_t i=0; i< sensorCount;i++)
	{
		pSensor = (SensorQueueElem_t *)malloc (sizeof(SensorQueueElem_t));
		pSensor->BusIndex = i;
		
		if (gTempSensors.getAddress(pSensor->Address, i)) {
			gTempSensors.setResolution(pSensor->Address, SENSOR_RESOLUTION);
			pSensor->Resolution = gTempSensors.getResolution(pSensor->Address);
		} else {
      pSensor->Resolution = 0;
		  memset(pSensor->Address,0,8);
		  }
		pSensor->Value = -127.0;
		// mqtt topic is "MQTT_PREFIX/MQTT_ROOM/MQTT_APPLIANCE/*index*". the +5 is for the 3 slashes, index and the terminal 0
		pSensor->MqttTopic = (char *)malloc (strlen(MQTT_PREFIX) + strlen(MQTT_ROOM) + strlen(MQTT_APPLIANCE) + 5);
    sprintf(pSensor->MqttTopic,"%s/%s/%s/%d",MQTT_PREFIX,MQTT_ROOM,MQTT_APPLIANCE, pSensor->BusIndex);
		STAILQ_INSERT_TAIL(&gSensorQueue, pSensor, sensorEntry);
	}
}
