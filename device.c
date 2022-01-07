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

#include "device.h"
#include "log.h"
#include "pattern.h"
#include "util.h"


char deviceName[64] = {0};
char deviceId[64] = {0};
char deviceType[64] = {0};
char accountId[ACCOUNT_ID_SIZE];

char* getClientID(){
    return clientId;
}
void setClientID(char* cid){    
    strcpy(clientId, cid);
}
char* getDeviceID(){
    return deviceId;
}
void setDeviceID(char* id){
    log_info("setDevicID: %s",id);
    strcpy(deviceId,id);
}

void setDeviceType(char* dt){
    log_info("set deviceType: %s",dt);
    strcpy(deviceType,dt);
}

void setAccountID(char* aId){
    strcpy(accountId,aId);
}

char* getAccountID(){
    return accountId;
}

char* getJwtToken(){
    return JWTToken;
}

void setJWTToken(char* val){
    strcpy(JWTToken, val);
}
char* getDeviceName(){
    return deviceName;
}
void setDeviceName(char* dName){
    log_debug("setDeviceName: %d %s ",strlen(dName),dName);
    strncpy(deviceName,dName, strlen(dName));

}

char* getDeviceType(){
    return deviceType;
}
