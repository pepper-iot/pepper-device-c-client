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

#include "util.h"
#include "log.h"
#include "pattern.h"
#include "device.h"

const char *_username;
const char *_password;
static char * pPayload;    
static char _clientId[25];

MQTTClient connectMqtt(char* username, char* password){
    int rc = 1; 
    qos = 1;

    char* serverUrl = getMqttServerUrl();
    log_info("connectMqtt %s ", serverUrl);
    //log_info("username: %s , password: %s", (char*)username, (char*)password);
    // the serverURI has to be in the format "protocol://name:port", in this case it should be "ssl://name:8883"
    
    getRandomId(_clientId);
    MQTTClient_create(&client, serverUrl, _clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // you can set optional callbacks for context, connectionLost, messageArrived and deliveryComplete
    int i = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
    log_info("callback: %d \n", i); // callback 0 signalizes a successful connection

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    ssl_opts.enableServerCertAuth = 0;

    ssl_opts.verify = 0;
    ssl_opts.CApath = "NULL";
    ssl_opts.keyStore = NULL;
    ssl_opts.trustStore = "./ca.pem";
    ssl_opts.privateKey = NULL;
    ssl_opts.privateKeyPassword = NULL;
    ssl_opts.enabledCipherSuites = NULL;

    conn_opts.ssl = &ssl_opts;
    conn_opts.keepAliveInterval = 10;
    conn_opts.cleansession = 1;
    
    _username = &username[i];
    _password = &password[i];
    
    conn_opts.username = username;
    conn_opts.password = password;
    //setClientID("");

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(0);
    }

    log_info("is connected %d \n", rc);

    char topic1[200];
    char topic2[200];
    // todo fix this subscription path
    strcpy(topic1, "t/#"); //, getDeviceName());
    MQTTClient_subscribe(client,topic1 , qos);
    log_info("subscribed to - -%s \n", topic1);

    sprintf(topic2, "home/%s/JWT", getDeviceName());
    MQTTClient_subscribe(client, topic2, qos);
    log_info("subscribed to -- %s \n", topic2);

    return client;
}

void disconnectFromMqtt(){
    int rc = 1;
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS){
                                    printf("Failed to disconnect, return code %d\n", rc);
                                    exit(0);
                                }
}



int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    /** TODO:   Validate
     *          PARSE 
     *          Evaluate
     *          Update
     *              desired  - handle
     *              reported - ignore
     */ 
    // just print for now
    pPayload = malloc(message->payloadlen* sizeof(char));
    strncpy(pPayload, message->payload,message->payloadlen);
    log_info("messageArrived: %s  %d %d", pPayload, strlen(message->payload), message->payloadlen);

    char* ret = strstr(pPayload, "reported");
    if (ret != NULL){
        //printf("ignor reported message %p\n", ret);
        log_info("reported message - do not handle ");
    } else {
        if (strstr(topicName,"JWT") > 0) {
            log_info("<== %s \n ==%s \n",(char*) topicName, (char*)message->payload);            
            strcpy(JWTToken,(char*)pPayload);
        }
        else{
            log_info("^== %s ",(char*) topicName);
            //log_info("^== %s ", (char*)message->payload);
            handleRegistrationMessage(pPayload);
        }
    }
    
    return 1;
}