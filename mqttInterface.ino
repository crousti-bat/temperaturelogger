

/********************************** START CALLBACK ****************************/
void EnableOtaCallback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char *topicToTokenize = (char*) malloc (strlen(topic) + 1);
  strcpy(topicToTokenize, topic);

  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);

  //check topic validity: 
  // mqtt topic is "MQTT_PREFIX/MQTT_ROOM/MQTT_APPLIANCE". 
  char *token;

  /* get the first token */
  token = strtok(topicToTokenize, "/");

  // does it start with MQTT_PREFIX ?
  if (token == NULL ) {
    //no; leave
    Serial.println("No topic");
    return;
  }

  if (strcmp(token, MQTT_PREFIX) != 0) {
    //no; leave
    Serial.println("wrong MQTT prefix");
    return;
  }
  token = strtok(NULL, "/");
  if (token == NULL) {
    Serial.println("no MQTT room");
    return;
  }

   if (strcmp(MQTT_ROOM, token) != 0) {
    Serial.println("wrong MQTT room");
    return;
  }

  token = strtok(NULL, "/");
  if (token == NULL) {
    Serial.println("no MQTT appliance");
    return;
  }

  // does it end with MQTT_APPLIANCE ?
  if (strcmp(token , MQTT_APPLIANCE) != 0)  {
    Serial.println("wrong mqtt appliance");
    return;
  }

  // there should not be any data left
  token = strtok(NULL, "/");
  if (token != NULL) {
    Serial.println("Topic too long");
    return;
  }

  /* now check the value inside */
  StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return ;
  }

  const char* res = root["state"];
  if (res) {
    if (strcmp(root["state"], OTA_MQTT_KEYWORD) == 0)
		gUpdateOrder = true;
	}
}

void SendMeasuresOnMqtt(void)
{
    StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char* pBuffer = NULL;
    SensorQueueElem_t *pSensor = NULL;
  	STAILQ_FOREACH(pSensor, &gSensorQueue, sensorEntry) {
		root["sensorType"] = "Temperature";
		root["index"] = pSensor->BusIndex;
		root["addess"] = pSensor->Address;
		root["Resolution"] = pSensor->Resolution;
		root["value"] = pSensor->Value;
	
		char* pBuffer = (char *) malloc (root.measureLength() + 1);
		root.printTo(pBuffer, sizeof(pBuffer));

		gMqttClient.publish(pSensor->MqttTopic, pBuffer, true);
		free (pBuffer);
	}
}
