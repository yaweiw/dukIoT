#ifndef IOTHUBCLIENTLIB_H
#define IOTHUBCLIENTLIB_H

#include "../duktapert/duktape.h"

#ifdef __cplusplus
extern "C" {
#endif
    duk_ret_t iothubclient_print_connectionstring(duk_context *ctx);
    duk_ret_t iothubclient_constructor(duk_context *ctx);
    duk_ret_t IoTHubClient_CreateFromConnectionString(duk_context *ctx);
    duk_ret_t IoTHubMessage_CreateFromByteArray(duk_context *ctx);
    duk_ret_t IoTHubClient_SendEventAsync(duk_context *ctx);

#ifdef __cplusplus
}
#endif

#endif /* IOTHUBCLIENTLIB_H */