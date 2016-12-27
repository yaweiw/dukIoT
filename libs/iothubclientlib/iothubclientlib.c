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

duk_ret_t iothubclient_createfromconnectionstring(duk_context *ctx)
{
    // [cs protocol]
    const char* cs = duk_to_string(ctx, -2);
    const char* protocol = duk_to_string(ctx, -1);

    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    if (platform_init() != 0) {
        printf("Failed to initialize the platform.\r\n");
        return 0;
    } else {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(cs, HTTP_Protocol)) == NULL) {
            (void)printf("ERROR: iotHubClientHandle is NULL!\r\n");
            return 0;
        } else {
            printf("iotHubClientHandle = %p\r\n", iotHubClientHandle);
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

/* IoTHubClient.prototype.getconnectionstring */
duk_ret_t iothubclient_getconnectionstring(duk_context *ctx) {
    /* Stack at entry is: [ ] */

    /* Get access to the 'this' binding. */
    duk_push_this(ctx);  /* -> stack: [ this ] */

    /* Read this.connectionstring */
    /* whatever last pushed is the return */
    duk_get_prop_string(ctx, -1, "connectionstring"); /* -> stack: [ this cstr ] */

    return 1;  /* 0 = no return value (= undefined) */
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


duk_ret_t iothubclient_setmessagecallback(duk_context *ctx) {
    // args passe thru ctx
    duk_push_string(ctx, "iothubclient_setmessagecallback is called!");
    return 1;
}

duk_ret_t iothubclient_setoption(duk_context *ctx) {
    // args passe thru ctx
    duk_push_string(ctx, "iothubclient_setoption is called!");
    return 1;
}

duk_ret_t iothubclient_sendeventasync(duk_context *ctx) {
    IOTHUB_CLIENT_LL_HANDLE handle = duk_get_pointer(ctx, -2);
    const char* msg = duk_get_string(ctx, -1);

    IOTHUB_MESSAGE_HANDLE messageHandle;
    if ((messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msg, strlen(msg))) == NULL) {
        printf("messageHandle is null\r\n");
        duk_push_string(ctx, "IOTHUB_CLIENT_ERROR");
    } else {
        if(IoTHubClient_LL_SendEventAsync(handle, messageHandle, NULL, NULL) != IOTHUB_CLIENT_OK) {
            printf("ERROR: IoTHubClient_LL_SendEventAsync..........FAILED!\r\n");
            duk_push_string(ctx, "IOTHUB_CLIENT_ERROR");
        } else {
            printf("IoTHubClient_LL_SendEventAsync accepted message for transmission to IoT Hub.\r\n");
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
    /* Set iothubclient.prototype.getconnectionstring. */
    duk_push_c_function(ctx, iothubclient_getconnectionstring, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "getconnectionstring");
    /* Set iothubclient.prototype.setmessagecallback. */
    duk_push_c_function(ctx, iothubclient_setmessagecallback, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "setmessagecallback");
    /* Set iothubclient.prototype.sendeventasync. */
    duk_push_c_function(ctx, iothubclient_sendeventasync, 2 /*nargs*/);
    duk_put_prop_string(ctx, -2, "sendeventasync");
    /* Set iothubclient.prototype.setoption. */
    duk_push_c_function(ctx, iothubclient_setoption, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "setoption");
    duk_push_c_function(ctx, iothubclient_createfromconnectionstring, 2 /*nargs*/);
    duk_put_prop_string(ctx, -2, "fromConnectionString");    
    duk_push_c_function(ctx, iothubclient_destory, 1 /*nargs*/);
    duk_put_prop_string(ctx, -2, "close");    
    /* Set iothubclient.prototype = proto */
    duk_put_prop_string(ctx, -2, "prototype");  /* -> stack: [ iothubclient_constructor ] */
    /* register iothubclient to the global object */
    duk_put_global_string(ctx, "IoTHubClient");  /* -> stack: [ ] */
}

void iothubclient_test(duk_context *ctx) {
    iothubclient_init(ctx);

    printf("Test creation of a new object from Ecmascript code.\n");
    /* Test creation of a new object from Ecmascript code. */
    duk_eval_string(ctx, "new IoTHubClient('HostName=yaweiIotHub.azure-devices.net;DeviceId=yaweiFirstDevice;SharedAccessKey=e3MIeWm8OvVL2zwqiVymLla1uIvsl3vQVO0cFUjoKb8=', 'HTTP')");
    /* ... stack top has result ... */ // [... iothubclientObj ] != IOTHUB_CLIENT_LL_HANDLE
    printf("Check properties exist.\n");
    propExistsInstance(ctx, "connectionstring");
    propExistsInstance(ctx, "protocol");
    propExistsInstance(ctx, "getconnectionstring");
    propExistsInstance(ctx, "setmessagecallback");
    propExistsInstance(ctx, "sendeventasync");
    propExistsInstance(ctx, "setoption");
    propExistsInstance(ctx, "fromConnectionString");
    propExistsInstance(ctx, "close");

    printf("\nCall object methods.\n");
    duk_get_prop_string(ctx, -1, "getconnectionstring");  /* call obj.getconnectionstring(); */ // [... iothubclientObj getconnectionstring ]
    duk_dup(ctx, -2); // [... iothubclientObj getconnectionstring iothubclientObj(this) ]
    duk_call_method(ctx, 0 /*nargs*/); // 0: [ ...iothubclientObj undefined ]; 1: [ ...iothubclientObj cstr ]
    printf("[getconnectionstring] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "setmessagecallback");  /* call obj.setmessagecallback(); */ // [... iothubclientObj setmessagecallback ]
    duk_dup(ctx, -2); // [... iothubclientObj setmessagecallback iothubclientObj(this) ]
    duk_call_method(ctx, 0 /*nargs*/); // 0: [ ...iothubclientObj undefined ]; 1: [ ...iothubclientObj cstr ]
    printf("[setmessagecallback] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "fromConnectionString");
    duk_dup(ctx, -2);
    duk_push_string(ctx, "test connection string");
    duk_push_string(ctx, "HTTP");
    duk_call_method(ctx, 2 /*nargs*/);
    /*stack top has the pointer to IOTHUB_CLIENT_LL_HANDLE*/
    IOTHUB_CLIENT_LL_HANDLE pointer = (IOTHUB_CLIENT_LL_HANDLE)duk_get_pointer(ctx, -1);
    printf("[fromConnectionString] handle = %p, \n", pointer);
    duk_pop(ctx);

    duk_get_prop_string(ctx, -1, "sendeventasync");
    duk_dup(ctx, -2);
    duk_push_pointer(ctx, pointer);
    duk_push_string(ctx, "mymessage");
    duk_call_method(ctx, 2 /*nargs*/);
    printf("[sendeventasync] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "close");
    duk_dup(ctx, -2);
    duk_push_pointer(ctx, pointer);
    duk_call_method(ctx, 1 /*nargs*/);
    duk_pop(ctx);

    duk_get_prop_string(ctx, -1, "setoption");
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0 /*nargs*/);
    printf("[setoption] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);
    duk_pop(ctx);  /* pop instance */
}