
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef use_test_runtime
#define use_test_runtime
#endif

#ifdef use_test_runtime
#include "../../duktapert/testrt/duktape.h"
#else
#include "../../duktapert/duktape.h"
#endif

#include "../../libs/iothubclientmqttlib/iothubclientmqttlib.h"
#include "../../helper/helper.h"

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "iothub_client_ll.h"
#include "iothub_message.h"
#include "iothubtransportmqtt.h"
#include <string.h>

void iothubclient_test(duk_context *ctx, const char* mode, const char* hn, const char* di, const char* sak) {
    IOTHUB_CLIENT_LL_HANDLE handle;
    iothubclient_init(ctx);
    char* cstr = (char*)malloc(sizeof(char)*(strlen(hn) + strlen(di) + strlen(sak) + 3));
    strcpy(cstr, hn);
    cstr[strlen(hn)] = ';';
    cstr[strlen(hn) + 1] = '\0';
    strcat(cstr, di);
    cstr[strlen(hn) + strlen(di) + 1] = ';';
    cstr[strlen(hn) + strlen(di) + 2] = '\0';
    strcat(cstr, sak);

    /* Test creation of a new object from Ecmascript code. */
    char jsstr[1024];
    (void)memset(jsstr, 0, 1024);
    (void)strcpy(jsstr, "new IoTHubClient('");
    (void)strcat(jsstr, cstr);
    (void)strcat(jsstr, "', 'MQTT')");
    duk_eval_string(ctx, jsstr);
    /* ... stack top has result ... */ // [... iothubclientObj ] != IOTHUB_CLIENT_LL_HANDLE
    if(strcmp(mode, "propertycheck_connectionstring") == 0) {
        propExistsInstance(ctx, "connectionstring");
    }

    if(strcmp(mode, "propertycheck_protocol") == 0) {
        propExistsInstance(ctx, "protocol");
    }

    if(strcmp(mode, "propertycheck_receive") == 0) {
        propExistsInstance(ctx, "receive");
    }

    if(strcmp(mode, "propertycheck_sendeventasync") == 0) {
        propExistsInstance(ctx, "sendeventasync");
    }

    if(strcmp(mode, "propertycheck_setoption") == 0) {
        propExistsInstance(ctx, "setoption");
    }

    if(strcmp(mode, "propertycheck_fromConnectionString") == 0) {
        propExistsInstance(ctx, "fromConnectionString");
    }

    if(strcmp(mode, "propertycheck_close") == 0) {
        propExistsInstance(ctx, "close");
    }
    duk_pop(ctx);


    /* ... stack top has result ... */ // [... iothubclientObj ] != IOTHUB_CLIENT_LL_HANDLE
    if(strcmp(mode, "fromconnectionstring") == 0) {
        #pragma region fromConnectionString
        duk_eval_string(ctx, jsstr);
        duk_get_prop_string(ctx, -1, "fromConnectionString"); // [... iothubclientObj fromConnectionString ]
        duk_dup(ctx, -2); // [... iothubclientObj fromConnectionString iothubclientObj(this) ]
        duk_push_string(ctx, cstr); // [... iothubclientObj fromConnectionString iothubclientObj(this) cstr ]
        duk_push_string(ctx, "MQTT"); // [... iothubclientObj fromConnectionString iothubclientObj(this) cstr "MQTT" ]
        duk_call_method(ctx, 2); // [... iothubclientObj iothubclienthandle ]
        //stack top has the pointer to IOTHUB_CLIENT_LL_HANDLE
        if(((IOTHUB_CLIENT_LL_HANDLE)duk_get_pointer(ctx, -1))) {
            (void)printf("fromConnectionString SUCCEEDED");
        } else {
            (void)printf("fromConnectionString FAILED");
        }
        duk_pop(ctx);
        return;
        #pragma endregion
    }

    if(strcmp(mode, "setoption") == 0) {
        #pragma region setoption
        duk_eval_string(ctx, jsstr);
        duk_get_prop_string(ctx, -1, "fromConnectionString");
        duk_dup(ctx, -2);
        duk_push_string(ctx, cstr);
        duk_push_string(ctx, "MQTT");
        duk_call_method(ctx, 2);
        handle = duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        if(handle) {
            duk_get_prop_string(ctx, -1, "setoption"); // [... iothubclientObj setoption ]
            duk_dup(ctx, -2); // [... iothubclientObj setoption iothubclientObj(this) ]
            duk_push_pointer(ctx, handle); // [... iothubclientObj setoption iothubclientObj(this) iothubclienthandle ]
            duk_push_string(ctx, "logtrace"); // [... iothubclientObj setoption iothubclientObj(this) iothubclienthandle optionName ]
            //unsigned int timeout = 241000;
            bool traceOn = true;
            duk_push_pointer(ctx, &traceOn); // [... iothubclientObj setoption iothubclientObj(this) iothubclienthandle optionName value ]
            duk_call_method(ctx, 3); // [... iothubclientObj IOTHUB_CLIENT_ERROR/OK ]
            (void)printf("%s", duk_to_string(ctx, -1));
        } else {
            (void)printf("NULL");
        }
        duk_pop(ctx);
        return;
        #pragma endregion
    }

    if(strcmp(mode, "sendeventasync") == 0) {
        #pragma region sendeventasync
        duk_eval_string(ctx, jsstr);
        duk_get_prop_string(ctx, -1, "fromConnectionString");
        duk_dup(ctx, -2);
        duk_push_string(ctx, cstr);
        duk_push_string(ctx, "MQTT");
        duk_call_method(ctx, 2);
        handle = duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        if(handle) {
            duk_get_prop_string(ctx, -1, "sendeventasync");
            duk_dup(ctx, -2);
            duk_push_pointer(ctx, handle);
            duk_push_string(ctx, "mymessage");
            duk_call_method(ctx, 2);
        }
        duk_pop(ctx);
        return;
        #pragma endregion
    }

    if(strcmp(mode, "receive") == 0) {
        #pragma region receive
        char c;
        scanf("%c", &c);
        //duk_get_prop_string(ctx, -1, "receive");
        //duk_dup(ctx, -2);
        //duk_push_pointer(ctx, handle);
        //duk_call_method(ctx, 1);
        char constr[1024];
        (void)memset(constr, 0, 1024);
        (void)strcpy(constr, "var cstr = '");
        (void)strcat(constr, cstr);
        (void)strcat(constr, "'");
        duk_eval_string(ctx, constr);
        duk_eval_string(ctx, "var iothubclient = new IoTHubClient(cstr, 'MQTT')");
        duk_eval_string(ctx, "var IoTHubClientLLHandle = iothubclient.fromConnectionString(cstr, 'MQTT');");
        duk_eval_string(ctx, "iothubclient.receive(IoTHubClientLLHandle);");

        // messagehandle is in global object labelled "messageHandle"
        (void)printf("%s", duk_to_string(ctx, -1));
        duk_pop(ctx);  /* pop call result */
        #pragma endregion
    }
}

int main(int argc, const char *argv[]) {
    duk_context *ctx = NULL;
    char line[4096];
    char idx;
    int ch;

    ctx = duk_create_heap_default();
    if (!ctx) {
        (void)printf("Failed to create a Duktape heap.");
        exit(1);
    }

    iothubclient_init(ctx);
    // argv[1] mode
    // argv[2] hostname
    // argv[3] devid
    // argv[4] sak
    iothubclient_test(ctx, (const char*)argv[1], (const char*)argv[2], (const char*)argv[3], (const char*)argv[4]);
    return 0;
}