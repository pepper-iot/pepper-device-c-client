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

//HELPER METHODS
#ifndef PATTERN_H
#define PATTERN_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 


#define MAX_MESSAGE_SIZE 1024
#define MAX_BUFFER_SIZE 2048
#define JWT_TOKEN_SIZE 256
#define MAX_URL_SIZE 256
#define MOVE_VERSION          "1.0"
#define MAX_JSON_TOKENS 200
#define ACCOUNT_ID_SIZE 50

char Buffer[MAX_BUFFER_SIZE];
char JWTToken[JWT_TOKEN_SIZE];
char MQTTServerUrl[MAX_URL_SIZE];
char *clientId;
char *mqttPassword;



char* getThingName();
char* getJwtToken();
void getUUID(char *buf);

char *getJsonValueFromBuffer(char *key, char *buffer);

char* getMQTTPassword();
int getNvRegisteredFlag();
void getRandomId(char *buf);
void assembleMutableTemperatureUpdate(float temperature);
void assembleConfigUpdate();
void assembleDeviceSetupMessage();
void assembleJWTRequest();
void assembleMutableUpdate();
void assembleRegisterRequest();
void assembleSubscribeRequest();
void assembleConfigRequest();
void assembleJWTRequest();
void handleRegistrationMessage();

////////////////////////////////////////////////////////////////
bool IsRegistered(void);
void setState(int state);

typedef enum DevState {
    DS_STARTING = 10,
    DS_REGISTERING,
    DS_SUBSCRIBING,
    DS_INIT,
    DS_PROVISIONING,
    DS_READY,
    DS_ONLINE,
    DS_OFFLINE,
    DS_UPGRADING,
    DS_UNREGISTER,
    DS_UNREGISTERING,
    DS_UNREGISTERED,
    DS_RESET
} DevState;

#endif 