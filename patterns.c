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

#include "./jWrite.h"
#include "pattern.h"
#include "util.h"
#include "device.h"
#include <time.h>
#include "jsmn.h"
#include "./log.h"


////////////////////////////////////// 
/// JSMN
static jsmntok_t *t = NULL;
static char tmp[4096];
//////////////////////////////////////


int getNvRegisteredFlag(){
    return true;
}
char* getFirmwareVersion(){
    return "v0.0.001";
}
char* getModel(){
    return "PTP-001";
}

void setMQTTPassword(char *password){
    log_info("set password: %s",password);
    strcpy(mqttPassword,password);
}

char* getMQTTPassword(){
    log_info("password: %s",mqttPassword);
    return mqttPassword;
}

void assembleMutableTemperatureUpdate(float temperature)
{
    char tempString[10]; 
    sprintf(tempString,"%.2f", temperature);
    memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);
    jwObj_object("state");    // object in the root object
        jwObj_object("reported");
        jwObj_string("messageType", "update");    
        jwObj_string("accountId", getAccountID());            
        jwObj_string("deviceId", getDeviceName() ); 
            jwObj_object("mutable");
                jwObj_object("temperatureState");
                    jwObj_string("tempvalue",tempString);                  
                    jwObj_string("displayType", "F");
                jwEnd();
            jwEnd();
        jwEnd();
    jwEnd();
    jwClose();
}

void assembleConfigUpdate()
{
    memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);
    jwObj_object("state");    // object in the root object
        jwObj_object("reported"); // object in the root object
            jwObj_string("messageType", "config");            
            jwObj_string("accountId", getAccountID());            
            jwObj_string("deviceId", getDeviceName() );
            jwObj_string("deviceType", getDeviceType());
                jwObj_object("settings");
                    jwObj_int("powerStateOn", 0);
                    jwObj_string("state", "connected");
                    jwObj_string("name",  getDeviceName());
                    jwObj_string("version", "v0.0.1");
               jwEnd();
            jwEnd();
        jwEnd();
    jwEnd();
    jwClose();
}

void assembleJWTRequest()
{
    memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);
    jwObj_object("state");    // object in the root object
        jwObj_object("reported"); // object in the root object
            jwObj_string("messageType", "requestJWT");            
            jwObj_string("accountId", getAccountID());            
            jwObj_string("deviceId", getDeviceName() );
            jwObj_string("deviceType", getDeviceType() );
            jwEnd();
        jwEnd();
    jwEnd();
    jwClose();
}

void assembleRegisterRequest()
{
    
    char path[64];

    log_info("-->Registering\n");
    snprintf(path, sizeof(path), "t.reg.%s", getDeviceName());

    memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);

    jwObj_string("version", MOVE_VERSION);
    jwObj_string("model", "registration");
    jwObj_string("operation", "register");
    jwObj_string("id", getClientID());
    jwObj_object("fields");
        jwObj_string("device_id", getDeviceID());
        jwObj_string("device_type", "iot_device");
        jwObj_string("id", getClientID());
        jwObj_string("token", getJwtToken());
        jwObj_string("username", "screen");
        jwObj_string("vendor", "bearerToken");
    jwEnd();
    jwObj_string("error_message", "");
    jwObj_array("path");
        jwArr_string(path);
    jwEnd();
    jwClose();

}



//Subscribe -
void assembleSubscribeRequest()
{
    char path[64];

    

    snprintf(path, sizeof(path), "t.%s", clientId);
    log_info("-->Subscribing %s", path);
    memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);


    jwObj_string("version", MOVE_VERSION);
    jwObj_string("model", "registration");
    jwObj_string("operation", "subscribe");
    jwObj_string("id", getClientID());
    jwObj_object("fields");
        jwObj_array("services");
            jwArr_object();
                jwObj_array("features");
                jwEnd();
                jwObj_string("id", getClientID());
                jwObj_string("source", "dynamic");
                jwObj_array("subscriptions");
                    jwArr_object();
                        jwObj_string("name", "data_text");
                        jwObj_string("status", "subscribed");
                    jwEnd();
                jwEnd();
                jwObj_string("type", "screen");
            jwEnd();
        jwEnd();
    jwEnd();
    jwObj_string("error_message", "");
    jwObj_array("path");
        jwArr_string(path);
    jwEnd();
    jwClose();

}

// GetDeviceConfig updates the configure stored in the device service
void assembleConfigRequest()
{
    char path[64];

    snprintf(path, sizeof(path), "t.%s", getClientID());

    memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);


    jwObj_string("version", MOVE_VERSION);
    jwObj_string("model", "device");
    jwObj_string("operation", "getConfig");
    jwObj_string("id", getClientID());
    jwObj_object("fields");
        jwObj_object("from");
            jwObj_string("account", getAccountID());
            jwObj_string("CID", getClientID());
            jwObj_string("device_id", getDeviceName());
            jwObj_string("location", "C-Client");
        jwEnd();
    jwEnd();
    jwObj_string("error_message", "");
    jwObj_array("path");
        jwArr_string(path);
    jwEnd();
    jwClose();
}

void assembleDeviceSetupMessage()
{
    char path[64];
    snprintf(path, sizeof(path), "t.%s", getClientID());

    memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);

    jwObj_string("version", MOVE_VERSION);
    jwObj_string("model", "device");
    jwObj_string("operation", "setup");
    jwObj_string("id", getClientID());
    jwObj_object("fields");
        jwObj_string("accepts", "headers");
        jwObj_string("device_id", getDeviceID());
        jwObj_string("firmware_build", "0");
        jwObj_string("firmware_type", getModel());
        jwObj_string("firmware_version", getFirmwareVersion());
    jwEnd();
    jwObj_string("error_message", "");
    jwObj_array("path");
        jwArr_string(path);
    jwEnd();
    jwClose();
}


void assembleMutableUpdate()
{


     memset(Buffer, 0, MAX_BUFFER_SIZE * sizeof (char));
    jwOpen(Buffer, MAX_BUFFER_SIZE, JW_OBJECT, JW_COMPACT);


    jwObj_object("state");    // object in the root object
        jwObj_object("reported");
            jwObj_object("mutable");
                jwObj_object("powerStateOn");
                    jwObj_int("value", 2);
                    jwObj_string("type", "P");
                jwEnd();
                // Clear out other data to avoid confusion in backend processing
                jwObj_raw("otaState", "null");
            jwEnd();
        jwEnd();
    jwEnd();
    jwClose();
}

void getRandomId(char *buf)
{
    uint32_t rand1;
    srand(time(NULL)); 
    rand1 = rand();
    sprintf(buf, "%08x", rand1);
}
void getUUID(char *buf)
{
    uint32_t rand1, rand2, rand3, rand4;
    srand(time(NULL)); 
    rand1 = rand();
    rand2 = rand();
    rand3 = rand();
    rand4 = rand();
    sprintf(buf, "%08x-%04x-%04x-%04x-%04x%08x", rand1,
                ((rand2 & 0xFFFF0000) >> 16), rand2 & 0x0FFFF,
                ((rand3 & 0xFFFF0000) >> 16), rand3 & 0x0FFFF, rand4);
}


void handleRegistrationMessage(char *message)
{
    char operation[32] = { 0 };
    char model[32] = { 0 };
    
    log_info("handleRegistrationMessage");
    

    char *ptr = getJsonValueFromBuffer("operation", message);
    if (ptr != NULL) {
         strncpy(operation, ptr, sizeof(operation));
    }
    
    ptr = getJsonValueFromBuffer("model", message);
    if (ptr != NULL) {
         strncpy(model, ptr, sizeof(model));
    }

    if ( (operation[0] == '\0') ||
         (model[0] == '\0')) {
            log_debug("model??: %s", message);
            log_info("Registration message missing model or operation!\n");
        return;
    }
     log_info("\nhandleRegistrationMessage %s", ptr);
    if (! strcmp(operation, "update")) {
         printf("Received an update from the registration\n");
    } else {
        if (! strcmp(model, "registration") &&
            ! strcmp(operation,"register")) {
            char *ptr = getJsonValueFromBuffer("result", message);
            char *results = NULL;

            if ((ptr != NULL) && (ptr[0] != '\0')) {
                results =  malloc( strlen(ptr) + 1);
                if (results != NULL) {
                     strcpy(results, ptr);
                }
            }
             log_info("\nresults");
            if (results != NULL) {
                char *id = getJsonValueFromBuffer("id", results);
                if ((id != NULL) && (id[0] != '\0')) {
                      log_info("Current CID: %s, New CID: %s\n",
                               clientId, id);
                    //strcpy(clientId, id);
                    setClientID(id);
                    log_info("\n********* getClientId %s",getClientID());
                    char *services = getJsonValueFromBuffer("services",results);
                    if ((services == NULL) || (services[0] == '\0')) {
                          log_info("No services on registration to subscribe\n");
                    }
                } else {
                      log_info("No ID found in result data!\n");
                }

                // Get the Move account_id
                 log_info("\ngetAccountID");
                char *account_id = getJsonValueFromBuffer("account_id", results);
                if ((account_id != NULL) && (account_id[0] != '\0')) {
                    setAccountID(account_id);
                    if (strcmp(account_id, "5cc893f1fd12670001b657de") == 0) 
                    {
                        setAccountID("");
                        setJWTToken("");
                        setState(DS_STARTING);
                        //return ;
                    }
                    log_info("Got move account_id: %s\n", account_id);
                }
                 log_info(" pass");
                // Get the password as well
                char *pw = getJsonValueFromBuffer("pass", results);
                if ((pw != NULL) && (pw[0] != '\0')) {

                    setMQTTPassword(pw);
                    log_info("Got password: %s\n", pw);
                }
                 free(results);
            } else {
                  log_info("Registration missing result!\n");
            }
        } else if (! strcmp(model, "registration") &&
                   ! strcmp(operation,"delete")) {
            //setDeviceName("");
            setMQTTPassword("");
              log_info("Unregistered\n");
            setState(DS_UNREGISTERED);
        } else if (! strcmp(model, "registration") &&
                   ! strcmp(operation,"subscribe")) {
              log_info("Got subscribe response...\n");

            // Go to init state to see if provisioned
            setState(DS_INIT);
        }
    }
    memset(message, 0, strlen(message) * sizeof (char));
}



static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)  strlen(s) == tok->end - tok->start &&
          strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}

char *getJsonValueFromBuffer(char *key, char *buffer)
{

    memset(tmp,0,sizeof(tmp));
    char *JSON_STRING = buffer;
    jsmn_parser p;

    jsmn_init(&p);
    t =   malloc(sizeof(jsmntok_t) * MAX_JSON_TOKENS);
    if (t == NULL) {
           log_info("Error allocating token array!\n");
        return NULL;
    }

    int r = jsmn_parse(&p, JSON_STRING,   strlen(JSON_STRING), t,
		       MAX_JSON_TOKENS);
    int i;
    
    for (i = 1; i < r; i++)
    {
        // We only want to match string keys!
        if (t[i].type != JSMN_STRING)
        {
            continue;
        }
        // Compare key
        if (jsoneq(JSON_STRING, &t[i++], key) == 0)
        {
            //   log_info("- %s: %.*s\n",key, t[i].end - t[i].start,
            //     JSON_STRING + t[i].start);
              strncpy(tmp, JSON_STRING + t[i].start, t[i].end - t[i].start);
            tmp[t[i].end - t[i].start] = '\0';
            // Exit if value found
            if (tmp[0] != '\0')
                break;
        }
    }
      free(t);
    t = NULL;
    return tmp;
}

//IsRegistered tells if the connection has an CID
bool IsRegistered(void)
{
    char *cid = getClientID();

    if ((cid == NULL) || (cid[0] == '\0')) {
        return false;
    }
    return true;
}