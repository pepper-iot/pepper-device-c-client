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

char* getDeviceName();
char* getDeviceType();
char* getDeviceID();
char* getFirmwareVersion();
char* getModel();
char* getAccountID();
char* getClientID();

void setJWTToken(char* val);
void setAccountID(char* aId);
void setDeviceType(char* type);
void setDeviceID(char* id);
void setDeviceName(char* dName);
int setDeviceValues(char* deviceType);
void setClientID(char*);




typedef enum DevType {
    TEMPEST = 1,
    WEATHERMAN,
    LIGHT,
    PUMP
} DevType;