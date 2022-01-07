//////////////////////////////////////////////////////////////////////////////
//
// SmartHome Ventures Confidential
//---------------------------------------
// Copyright 2019-2022 SmartHome Ventures, LLC. All rights reserved.
//
// NOTICE: All information contained herein is and remains the property of
// SmartHome Ventures, LLC. The technical concepts contained herein are
// proprietary to SmartHome Ventures LLC. and may be covered by U.S. and
// foreign patents, or patents in process, and are protected by trade secret
// or copyright law. Dissemination or reproduction of this material is
// strictly forbidden unless prior written permission is obtained from
// SmartHome Ventures, LLC.
//
//////////////////////////////////////////////////////////////////////////////

#include "MQTTClient.h"


int getState();
void setState(int state);
MQTTClient connectMqtt(char*, char*);
void disconnectFromMqtt();
MQTTClient client;
int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message);
int readConfigFile(char *filename);
char* getMqttServerUrl();
int qos;
void delay(int number_of_seconds);