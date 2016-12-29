#include "../../duktapert/duktape.h"
#include "../../helper/helper.h"

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "iothub_client_ll.h"
#include "iothub_message.h"
#include "iothubtransporthttp.h"
#include <string.h>

#include "iothubclientlib.h"

#define DOWORK_LOOP_NUM     3

const char* cstr = "<your ConnectionString>";

duk_ret_t iothubclient_createfromconnectionstring(duk_context *ctx)
{
    // [cs protocol]
    const char* cs = duk_to_string(ctx, -2);
    const char* protocol = duk_to_string(ctx, -1);

    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    if (platform_init() != 0) {
        (void)printf("Failed to initialize the platform.\r\n");
        return 0;
    } else {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(cs, HTTP_Protocol)) == NULL) {
            (void)printf("ERROR: iotHubClientHandle is NULL!\r\n");
            return 0;
        }
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



static IOTHUBMESSAGE_DISPOSITION_RESULT receivemessage_callback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    const char* buffer;
    duk_context *ctx = userContextCallback;
    MESSAGE_DATA_HANDLE messageHandle = NULL;
    size_t buffersize;
    //MAP_HANDLE mapProperties;

    if (IoTHubMessage_GetByteArray(message, (const unsigned char**)&buffer, &buffersize) != IOTHUB_MESSAGE_OK) {
        (void)printf("unable to retrieve the message data\r\n");
        return IOTHUBMESSAGE_REJECTED;
    } else {
        if ((messageHandle = (MESSAGE_DATA_HANDLE)malloc(sizeof(MESSAGE_DATA))) != NULL) {
            messageHandle->size = buffersize;
            if((messageHandle->data = (char*)malloc(buffersize + 1)) != NULL) {
                memcpy(messageHandle->data, buffer, buffersize + 1);
                messageHandle->data[buffersize] = '\0';
                duk_push_global_object(ctx);
                duk_push_pointer(ctx, messageHandle);
                (void)duk_put_global_string(ctx, "messageHandle");
            }
            else {
                duk_push_global_object(ctx);
                duk_push_pointer(ctx, NULL);
                (void)duk_put_global_string(ctx, "messageHandle");
                return IOTHUBMESSAGE_REJECTED;
            }
        }
        else {
            duk_push_global_object(ctx);
            duk_push_pointer(ctx, NULL);
            (void)duk_put_global_string(ctx, "messageHandle");
            return IOTHUBMESSAGE_REJECTED;
        }
    }
/*
    // Retrieve properties from the message
    mapProperties = IoTHubMessage_Properties(message);
    if (mapProperties != NULL)
    {
        const char*const* keys;
        const char*const* values;
        size_t propertyCount = 0;
        if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
        {
            if (propertyCount > 0)
            {
                size_t index;

                printf("Message Properties:\r\n");
                for (index = 0; index < propertyCount; index++)
                {
                    printf("\tKey: %s Value: %s\r\n", keys[index], values[index]);
                }
                printf("\r\n");
            }
        }
    }

    // Some device specific action code goes here...
    (*counter)++;
    */
    return IOTHUBMESSAGE_ACCEPTED;
}

duk_ret_t iothubclient_receive(duk_context *ctx) {
    // args passe thru ctx
    IOTHUB_CLIENT_LL_HANDLE handle = duk_get_pointer(ctx, -1);  //iothubclienthandle

    //MESSAGE_DATA_HANDLE messageHandle = NULL;
    /* Setting Message call back, so we can receive Commands. */
    if (IoTHubClient_LL_SetMessageCallback(handle, receivemessage_callback, ctx) != IOTHUB_CLIENT_OK) {
        (void)printf("ERROR: IoTHubClient_LL_SetMessageCallback..........FAILED!\r\n");
    }
    for (size_t index = 0; index < DOWORK_LOOP_NUM; index++)
    {
        IoTHubClient_LL_DoWork(handle);
        ThreadAPI_Sleep(1);
    }

    duk_push_global_object(ctx);
    (void)duk_get_global_string(ctx, "messageHandle");
    MESSAGE_DATA_HANDLE messageHandle = duk_to_pointer(ctx, -1);
    if(messageHandle) {
        duk_pop(ctx);
        duk_push_string(ctx, messageHandle->data);
        return 1;
    }
    else {
        (void)printf("messageHandle is null\r\n");
        duk_pop(ctx);
        return 0;
    }
}

duk_ret_t iothubclient_setoption(duk_context *ctx) {
    // args passe thru ctx
    IOTHUB_CLIENT_LL_HANDLE handle = duk_get_pointer(ctx, -3);

    const char* optionName = duk_get_string(ctx, -2);
    const int* value = duk_get_pointer(ctx, -1);
    if (IoTHubClient_LL_SetOption(handle, optionName, value) != IOTHUB_CLIENT_OK) {
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
    IOTHUB_MESSAGE_HANDLE messagehandle = (IOTHUB_MESSAGE_HANDLE)userContextCallback;
    (void)printf("IoTHubClient_LL_SendEventAsync confirmed message has been transmitted to IoT Hub.\r\n");
    IoTHubMessage_Destroy(messagehandle);
}

duk_ret_t iothubclient_sendeventasync(duk_context *ctx) {
    IOTHUB_CLIENT_LL_HANDLE handle = duk_get_pointer(ctx, -2);
    const char* msg = duk_get_string(ctx, -1);

    IOTHUB_MESSAGE_HANDLE messageHandle;
    if ((messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msg, strlen(msg))) == NULL) {
        (void)printf("messageHandle is null\r\n");
        duk_push_string(ctx, "IOTHUB_CLIENT_ERROR");
    } else {
        if(IoTHubClient_LL_SendEventAsync(handle, messageHandle, sendconfirmation_callback, messageHandle) != IOTHUB_CLIENT_OK) {
            (void)printf("ERROR: IoTHubClient_LL_SendEventAsync..........FAILED!\r\n");
            duk_push_string(ctx, "IOTHUB_CLIENT_ERROR");
        } else {
            (void)printf("IoTHubClient_LL_SendEventAsync accepted message for transmission to IoT Hub.\r\n");
            duk_push_string(ctx, "IOTHUB_CLIENT_OK");
        }
        IoTHubClient_LL_DoWork(handle);
        ThreadAPI_Sleep(1);
    }
    return 1;
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

void iothubclient_test(duk_context *ctx) {
    iothubclient_init(ctx);

    (void)printf("Test creation of a new object from Ecmascript code.\n");
    /* Test creation of a new object from Ecmascript code. */
    char jsstr[1024];
    memset(jsstr, 0, 1024);
    strcpy(jsstr, "new IoTHubClient('");
    strcat(jsstr, cstr);
    strcat(jsstr, "', 'HTTP')");
    duk_eval_string(ctx, jsstr);
    /* ... stack top has result ... */ // [... iothubclientObj ] != IOTHUB_CLIENT_LL_HANDLE
    (void)printf("Check properties exist.\n");
    propExistsInstance(ctx, "connectionstring");
    propExistsInstance(ctx, "protocol");
    propExistsInstance(ctx, "receive");
    propExistsInstance(ctx, "sendeventasync");
    propExistsInstance(ctx, "setoption");
    propExistsInstance(ctx, "fromConnectionString");
    propExistsInstance(ctx, "close");

    (void)printf("\nCall object methods.\n");

    duk_get_prop_string(ctx, -1, "fromConnectionString"); // [... iothubclientObj fromConnectionString ]
    duk_dup(ctx, -2); // [... iothubclientObj fromConnectionString iothubclientObj(this) ]
    duk_push_string(ctx, cstr); // [... iothubclientObj fromConnectionString iothubclientObj(this) cstr ]
    duk_push_string(ctx, "HTTP"); // [... iothubclientObj fromConnectionString iothubclientObj(this) cstr "HTTP" ]
    duk_call_method(ctx, 2 /*nargs*/); // [... iothubclientObj iothubclienthandle ]
    /*stack top has the pointer to IOTHUB_CLIENT_LL_HANDLE*/
    IOTHUB_CLIENT_LL_HANDLE handle = (IOTHUB_CLIENT_LL_HANDLE)duk_get_pointer(ctx, -1);
    (void)printf("[fromConnectionString] handle = %p, \n", handle);
    duk_pop(ctx);

    duk_get_prop_string(ctx, -1, "setoption"); // [... iothubclientObj setoption ]
    duk_dup(ctx, -2); // [... iothubclientObj setoption iothubclientObj(this) ]
    duk_push_pointer(ctx, handle); // [... iothubclientObj setoption iothubclientObj(this) iothubclienthandle ]
    duk_push_string(ctx, "timeout"); // [... iothubclientObj setoption iothubclientObj(this) iothubclienthandle optionName ]
    unsigned int timeout = 241000;
    duk_push_pointer(ctx, &timeout); // [... iothubclientObj setoption iothubclientObj(this) iothubclienthandle optionName value ]
    duk_call_method(ctx, 3 /*nargs*/); // [... iothubclientObj IOTHUB_CLIENT_ERROR/OK ]
    (void)printf("[setoption] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);

    duk_get_prop_string(ctx, -1, "sendeventasync");
    duk_dup(ctx, -2);
    duk_push_pointer(ctx, handle);
    duk_push_string(ctx, "mymessage");
    duk_call_method(ctx, 2 /*nargs*/);
    (void)printf("[sendeventasync] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    (void)printf("send an cloud-to-device message now.");
    char c;
    scanf("%c", &c);
    duk_get_prop_string(ctx, -1, "receive");
    duk_dup(ctx, -2);
    duk_push_pointer(ctx, handle);
    duk_call_method(ctx, 1 /*nargs*/);
    // messagehandle is in global object labelled "messageHandle"
    (void)printf("[receive] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_eval_string(ctx, jsstr);
    duk_get_prop_string(ctx, -1, "close");
    duk_dup(ctx, -2);
    duk_push_pointer(ctx, handle);
    duk_call_method(ctx, 1 /*nargs*/);
    duk_pop(ctx);

    duk_pop(ctx);  /* pop instance */
}