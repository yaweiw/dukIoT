#include "../../duktapert/duktape.h"
#include "../../helper/helper.h"

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "iothub_client_ll.h"
#include "iothub_message.h"
#include "iothubtransportmqtt.h"
#include "iothubtransport_amqp_common.h"
#include <string.h>

#include "iothubclientmqttlib.h"

#define DOWORK_LOOP_NUM     3
#define MAX_IOTHUB_MESSAGE_SIZE 1024 * 64

const char* cstr = "<your ConnectionString>";

static IOTHUBMESSAGE_DISPOSITION_RESULT receivemessage_callback(IOTHUB_MESSAGE_HANDLE iothubmessage, void* userContextCallback)
{
    const char* buffer;
    duk_context *ctx = userContextCallback;
    MESSAGE_DATA_HANDLE messageHandle = NULL;
    size_t buffersize;

    if (IoTHubMessage_GetByteArray(iothubmessage, (const unsigned char**)&buffer, &buffersize) != IOTHUB_MESSAGE_OK) {
        (void)printf("unable to retrieve the message data\r\n");
        return IOTHUBMESSAGE_REJECTED;
    } else {
        if ((messageHandle = (MESSAGE_DATA_HANDLE)malloc(sizeof(MESSAGE_DATA))) != NULL) {
            messageHandle->length = buffersize;
            if((messageHandle->message = (char*)malloc(buffersize + 1)) != NULL) {
                memcpy(messageHandle->message, buffer, buffersize + 1);
                messageHandle->message[buffersize] = '\0';
                // get properties
                MAP_HANDLE mapProperties = IoTHubMessage_Properties(iothubmessage);
                if (mapProperties != NULL)
                {
                    const char*const* keys;
                    const char*const* values;
                    size_t propertyCount = 0;
                    if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
                    {
                        if (propertyCount > 0)
                        {
                            messageHandle->count = propertyCount;
                            // allocate space for properties
                            messageHandle->properties = (PROPERTY_HANDLE*)malloc(sizeof(PROPERTY_HANDLE));

                            for (size_t index = 0; index < propertyCount; index++)
                            {
                                // allocate space for properties[index]
                                messageHandle->properties[index] = (PROPERTY_HANDLE)malloc(sizeof(PROPERTY));

                                // allocate space for properties[index]->key
                                size_t keyLen = strlen(keys[index]) + 1;
                                messageHandle->properties[index]->key = (char*)malloc(keyLen);
                                memcpy(messageHandle->properties[index]->key, keys[index], keyLen);
                                messageHandle->properties[index]->key[keyLen] = '\0';

                                // allocate space for properties[index]->value
                                size_t valLen = strlen(values[index]) + 1;
                                messageHandle->properties[index]->value = (char*)malloc(valLen);
                                memcpy(messageHandle->properties[index]->value, values[index], valLen);
                                messageHandle->properties[index]->value[valLen] = '\0';
                            }
                        }
                    }
                }

                duk_push_global_object(ctx);
                duk_push_pointer(ctx, messageHandle);
                (void)duk_put_global_string(ctx, "MESSAGE_DATA_HANDLE");
            }
            else {
                duk_push_global_object(ctx);
                duk_push_pointer(ctx, NULL);
                (void)duk_put_global_string(ctx, "MESSAGE_DATA_HANDLE");
                return IOTHUBMESSAGE_REJECTED;
            }
        }
        else {
            duk_push_global_object(ctx);
            duk_push_pointer(ctx, NULL);
            (void)duk_put_global_string(ctx, "MESSAGE_DATA_HANDLE");
            return IOTHUBMESSAGE_REJECTED;
        }
    }
    return IOTHUBMESSAGE_ACCEPTED;
}

duk_ret_t iothubclient_createfromconnectionstring(duk_context *ctx)
{
    // [cs protocol]
    const char* constr = duk_to_string(ctx, -2);
    const char* protocol = duk_to_string(ctx, -1);

    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    if (platform_init() != 0) {
        (void)printf("Failed to initialize the platform.\r\n");
        return 0;
    } else {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(constr, MQTT_Protocol)) == NULL) {
            (void)printf("ERROR: iotHubClientHandle is NULL!\r\n");
            return 0;
        }
        /* Setting Message call back, so we can receive Commands. */
        if (IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, receivemessage_callback, ctx) != IOTHUB_CLIENT_OK) {
            (void)printf("ERROR: IoTHubClient_LL_SetMessageCallback..........FAILED!\r\n");
            return 0;
        }
        // set message call back
        IoTHubClient_LL_DoWork(iotHubClientHandle);
        // set callback needs time 1s is required
        ThreadAPI_Sleep(1000);
        // set MESSAGE_DATA_HANDLE prop NULL
        duk_push_global_object(ctx);
        duk_push_pointer(ctx, NULL);
        (void)duk_put_global_string(ctx, "MESSAGE_DATA_HANDLE");
    }
    duk_push_pointer(ctx, iotHubClientHandle);
    return 1;
}

duk_ret_t iothubclient_destory(duk_context *ctx)
{
    IOTHUB_CLIENT_LL_HANDLE handle = duk_get_pointer(ctx, -1);
    IoTHubClient_LL_Destroy(handle);
    platform_deinit();
    return 0;
}

/* IoTHubClient */
duk_ret_t iothubclient_constructor(duk_context *ctx) {
    /* Stack at entry is: [ connectionstring protocol ] */

    /* All Duktape/C functions can be called both as constructors
     * ("new func()") and functions ("func()").  Sometimes objects
     * allow both calls, sometimes not.  Here we reject a normal
     * non-constructor call.
     */
    if (!duk_is_constructor_call(ctx)) {
        return DUK_RET_TYPE_ERROR;
    }

    /* Get access to the default instance. */
    duk_push_this(ctx);  /* -> stack: [ connectionstring protocol this ] */

    /* Set this.connectionstring to connectionstring. */
    duk_dup(ctx, 0);  /* -> stack: [ connectionstring protocol this connectionstring ] */
    duk_put_prop_string(ctx, -2, "connectionstring");  /* -> stack: [ connectionstring protocol this ] */

    duk_dup(ctx, 1);  /* -> stack: [ connectionstring protocol this protocol ] */
    duk_put_prop_string(ctx, -2, "protocol");  /* -> stack: [ connectionstring protocol this ] */
    return 0;
}

duk_ret_t iothubclient_receive(duk_context *ctx) {
    // args passe thru ctx
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle = duk_get_pointer(ctx, -1);  //iothubclienthandle

    MESSAGE_DATA_HANDLE messageHandle;
    // make sure message is received
    for (size_t index = 0; index < DOWORK_LOOP_NUM; index++)
    {
        IoTHubClient_LL_DoWork(iotHubClientHandle);
        ThreadAPI_Sleep(1000);
        duk_push_global_object(ctx);
        (void)duk_get_global_string(ctx, "MESSAGE_DATA_HANDLE");
        messageHandle = duk_to_pointer(ctx, -1);
        if(messageHandle) {
            break;
        }
        duk_pop_2(ctx);
    }

    if(messageHandle) {
        // received the message
        duk_push_global_object(ctx);
        duk_push_pointer(ctx, NULL);
        (void)duk_put_global_string(ctx, "MESSAGE_DATA_HANDLE");

        // max cloud to device message size 64k
        char* messageinJson = (char*)malloc(sizeof(char) * MAX_IOTHUB_MESSAGE_SIZE);
        memset(messageinJson, 0, sizeof(char) * MAX_IOTHUB_MESSAGE_SIZE);
        //{	"message": "abab",
        strcpy(messageinJson, "{\"message\":\"");
        strcat(messageinJson, messageHandle->message);
        strcat(messageinJson, "\",\"properties\": {");
        // add all properties key value
        for(size_t index = 0; index < messageHandle->count; index++)
        {
            strcat(messageinJson, "\"");
            // key
            strcat(messageinJson, messageHandle->properties[index]->key);
            strcat(messageinJson, "\":\"");
            // value
            strcat(messageinJson, messageHandle->properties[index]->key);
            if(index == messageHandle->count - 1) {
                strcat(messageinJson, "\"}");
            } else {
                strcat(messageinJson, "\",");
            }
        }
        //"}"
        strcat(messageinJson, "}");
        duk_push_string(ctx, messageinJson);
        return 1;
    }
    else {
        (void)printf("MESSAGE_DATA_HANDLE is null\r\n");
        return 0;
    }
}

duk_ret_t iothubclient_setoption(duk_context *ctx) {
    // args passe thru ctx
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle = duk_get_pointer(ctx, -3);

    const char* optionName = duk_get_string(ctx, -2);
    const int* value = duk_get_pointer(ctx, -1);
    if (IoTHubClient_LL_SetOption(iotHubClientHandle, optionName, value) != IOTHUB_CLIENT_OK) {
        (void)printf("setoption failed\r\n");
        duk_push_string(ctx, "IOTHUB_CLIENT_ERROR");
    }
    else {
        duk_push_string(ctx, "IOTHUB_CLIENT_OK");
    }
    return 1;
}

static void sendconfirmation_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    IOTHUB_MESSAGE_HANDLE iotHubMessage = (IOTHUB_MESSAGE_HANDLE)userContextCallback;
    (void)printf("IoTHubClient_LL_SendEventAsync confirmed message has been transmitted to IoT Hub.\r\n");
    IoTHubMessage_Destroy(iotHubMessage);
}

duk_ret_t iothubclient_sendeventasync(duk_context *ctx) {
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle = duk_get_pointer(ctx, -2);
    const char* msg = duk_get_string(ctx, -1);

    IOTHUB_MESSAGE_HANDLE iotHubMessage;
    if ((iotHubMessage = IoTHubMessage_CreateFromByteArray((const unsigned char*)msg, strlen(msg))) == NULL) {
        (void)printf("iotHubMessage is null\r\n");
        duk_push_string(ctx, "IOTHUB_CLIENT_ERROR");
    } else {
        if(IoTHubClient_LL_SendEventAsync(iotHubClientHandle, iotHubMessage, sendconfirmation_callback, (void *)iotHubMessage) != IOTHUB_CLIENT_OK) {
            (void)printf("ERROR: IoTHubClient_LL_SendEventAsync..........FAILED!\r\n");
        } else {
            (void)printf("IoTHubClient_LL_SendEventAsync accepted message for transmission to IoT Hub.\r\n");
        }
        // make sure message is sent
        IOTHUB_CLIENT_STATUS status;
        while ((IoTHubClient_LL_GetSendStatus(iotHubClientHandle, &status) == IOTHUB_CLIENT_OK) && (status == IOTHUB_CLIENT_SEND_STATUS_BUSY))
        {
            IoTHubClient_LL_DoWork(iotHubClientHandle);
            ThreadAPI_Sleep(100);
        }
    }
    return 0;
}

void iothubclient_init(duk_context *ctx) {
    // register constructor
    duk_push_c_function(ctx, iothubclient_constructor, 2 /*nargs*/); /* -> stack: [ iothubclient_constructor ] */
    /* Push iothubclient.prototype object. */
    duk_push_object(ctx); /* -> stack: [ iothubclient_constructor prototypeObject(empty) ] */
    /* Set iothubclient.prototype.sendeventasync. */
    duk_push_c_function(ctx, iothubclient_sendeventasync, 2 /*nargs*/);
    duk_put_prop_string(ctx, -2, "sendeventasync");
    /* Set iothubclient.prototype.setoption. */
    duk_push_c_function(ctx, iothubclient_setoption, 3 /*nargs*/);
    duk_put_prop_string(ctx, -2, "setoption");
    /* Set iothubclient.prototype.receive. */
    duk_push_c_function(ctx, iothubclient_receive, 1 /*nargs*/);
    duk_put_prop_string(ctx, -2, "receive");
    /* Set iothubclient.prototype.fromConnectionString. */
    duk_push_c_function(ctx, iothubclient_createfromconnectionstring, 2 /*nargs*/);
    duk_put_prop_string(ctx, -2, "fromConnectionString");
    /* Set iothubclient.prototype.close. */
    duk_push_c_function(ctx, iothubclient_destory, 1 /*nargs*/);
    duk_put_prop_string(ctx, -2, "close");
    /* Set iothubclient.prototype = proto */
    duk_put_prop_string(ctx, -2, "prototype");  /* -> stack: [ iothubclient_constructor ] */
    /* register iothubclient to the global object */
    duk_put_global_string(ctx, "IoTHubClient");  /* -> stack: [ ] */
}

