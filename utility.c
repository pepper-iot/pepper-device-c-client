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

#include <stdio.h>
////////////// utility ////////////////////
#include <time.h>
#include <stdbool.h>
#include "./log.h"
#include "pattern.h"
#include "device.h"

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

char* getMqttServerUrl(){
    return MQTTServerUrl;
}
int setMqttServerUrl(char* url){
    log_debug("setMqttServer %s", url);
    strcpy(MQTTServerUrl,url);
    return true;
}
int getValue(char *str, char *value)
{
    // very specific get value looking for a simple split string
    // we only care about the second part key,value
    // rules: no spaces to begin the line , if there is no comma or second part return false
    
    char *token = strtok(str, ",");
    if (token != NULL)
        token = strtok(NULL, ",");

    while (token != NULL)
    {
        strcpy(value, token);
        token = strtok(NULL, ",");
        break;
    }
    // check for carriage return in last position
    int len = strlen(value);
    if (len <= 0)
        return false;
    if (value[len - 1] == '\n')
    {
        value[len - 1] = '\0';
    }
    return true;
}
int readConfigFile(char *filename)
{

    FILE *file = fopen(filename, "r");
    if( file == NULL ) return false;
    log_info("Reading config file %s", filename);
    char buffer[256];    
    int ret;
    char value[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (buffer[0] != '#' && buffer[0] != ' ' && strlen(buffer) > 1)
        {

            if (strstr(buffer, "deviceId") != NULL)
            {
                ret = getValue(buffer, value);
                printf("deviceId %s \n", value);
                setDeviceID(value);
            }
            if (strstr(buffer, "accountId") != NULL)
            {
                ret = getValue(buffer, value);
                printf("accountId %s \n", value);
                setAccountID(value);
            }
            if (strstr(buffer, "deviceName") != NULL)
            {
                ret = getValue(buffer, value);
                printf("deviceName %s \n", value);
                setDeviceName(value);
            }
            if (strstr(buffer, "JWT") != NULL)
            {
                ret = getValue(buffer, value);
                printf("JWT %s \n", value);
                setJWTToken(value);
            }
            if (strstr(buffer, "mqtturl") != NULL)
            {
                log_info("mqtturl");
                ret = getValue(buffer, value);
                log_info("mqtturl %d", ret);
                printf("URL %s \n", value);
                setMqttServerUrl(value);
            }
        }
    }
    fclose(file);

    return true;
}
////////// end utility functions //////////