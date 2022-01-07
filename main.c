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
#include "./jWrite.h"
#include "pattern.h"
#include "device.h"
#include "./log.h"
#include "util.h"
#include <stdio.h>

static int state;


int main(int argc, char *argv[])
{

    int payloadlen = 0;
    char uuid[40];
    char regTopic[64];
    char pubTopic[64];
    char dataTopic[256];
    int retained = 0;
    int rc = 0;
    clientId = malloc(sizeof(char) + 64);
    mqttPassword = malloc(sizeof(char) + 64);
    
    /* file format is simple, # and space at the begining is a comment
    non space lines look for a comma separated key value pair
    */
    if (!readConfigFile("device.cfg")){
        return false;
    }

    
    strcpy(clientId, "");
    
    strcpy(mqttPassword, "public");
    // Note - registration sequence begins with an empty clientId
    client = connectMqtt(clientId, mqttPassword);

    MQTTClient_deliveryToken dt;

    sprintf(pubTopic, "home/%s", getDeviceName());
    getUUID(uuid);
    snprintf(regTopic, sizeof(regTopic), "r/reg/%s", uuid);

    setState(DS_STARTING);
    while (true)
    {
        log_info("State: %d", state);

        switch (state)
        {
        case DS_STARTING:
            log_debug("-->Starting");
            if (strlen(getJwtToken() /* or expired */) < 10)
            {
                assembleJWTRequest();
                MQTTClient_publish(client, pubTopic, payloadlen, Buffer, qos, retained, &dt);
            }
            else
            {
                // Need to register each time
                assembleRegisterRequest();
                MQTTClient_publish(client, regTopic, payloadlen, Buffer, qos, retained, &dt);
                log_info("DS_STARTING -- isRegistered %d", IsRegistered());
                setState(DS_REGISTERING);
            }
            payloadlen = strlen(Buffer);

            break;
        case DS_REGISTERING:
            // Did we register?
            log_info("DS_REGISTERING -- isRegistered %d", IsRegistered());

            if (IsRegistered())
            {
                log_info("Subscribe");
                sprintf(dataTopic, "r/%s/iot_device/%s/%s", uuid, getAccountID(), getDeviceID());
                assembleSubscribeRequest();
                payloadlen = strlen(Buffer);
                MQTTClient_publish(client, dataTopic, payloadlen, Buffer, qos, retained, &dt);
                setState(DS_SUBSCRIBING);
                log_info("Disconnect from mqtt");
                disconnectFromMqtt();
                client = NULL;                
                // Note - This login uses derived username and password
                log_info("set new username and pw");
                log_info("Connect to mqtt");
                client = connectMqtt(getClientID(), getMQTTPassword());
            }
            else
            {
                log_info("assembleRegisterRequest");
                assembleRegisterRequest();
                payloadlen = strlen(Buffer);
                MQTTClient_publish(client, regTopic, payloadlen, Buffer, qos, retained, &dt);
            }
            break;
        case DS_SUBSCRIBING:
        {
            assembleSubscribeRequest();
            payloadlen = strlen(Buffer);
            MQTTClient_publish(client, dataTopic, payloadlen, Buffer, qos, retained, &dt);            
            delay(5000);
            break;
        }
        case DS_INIT:
            log_debug("-->Init"); /// TODO: provision
            assembleConfigRequest();
            payloadlen = strlen(Buffer);
            MQTTClient_publish(client, dataTopic, payloadlen, Buffer, qos, retained, &dt);
            log_debug("-->Provisioning");
            setState(DS_PROVISIONING);
            break;

        case DS_PROVISIONING:
            log_info("-->Provisioning");
            //if (now > (lastRequest + MSG_RETRY_PERIOD))
            {
                if (getNvRegisteredFlag())
                {
                    assembleConfigRequest();
                    MQTTClient_publish(client, regTopic, payloadlen, Buffer, qos, retained, &dt);
                }
                else
                {
                    //DeviceProvision(msg);
                }
                //lastRequest = now;
            }
            setState(DS_READY);
            break;

        case DS_READY:
            log_info("-->Ready");
            // Check for firmware update via "Setup" message...
            //DeviceSetup(msg);
            assembleDeviceSetupMessage();
            MQTTClient_publish(client, regTopic, payloadlen, Buffer, qos, retained, &dt);
            setState(DS_ONLINE);
            log_info("-->Online");
            break;

        case DS_ONLINE:
            // Handle normal idle processing
            //processOnlineState(msg, loopCounter);
            assembleConfigRequest();
            MQTTClient_publish(client, regTopic, payloadlen, Buffer, qos, retained, &dt);
            break;
        default:
            break;
        }
        delay(3000);
    }
    // after this time, unsubscribe, disconnect and destroy the client to terminate every process
    int timeout = 100;
    // MQTTClient_unsubscribe(client, topic1);
    MQTTClient_disconnect(client, timeout);
    MQTTClient_destroy(&client);

    return rc;
}

int getState()
{
    return state;
}
void setState(int s)
{
    log_debug("setting state: %d", s);
    state = s;
}