#ifndef IOTHUBCLIENTLIB_H
#define IOTHUBCLIENTLIB_H

#ifdef __cplusplus
extern "C" {
#endif
    duk_ret_t iothubclient_getconnectionstring(duk_context *ctx);
    duk_ret_t iothubclient_constructor(duk_context *ctx);
    duk_ret_t iothubclient_setmessagecallback(duk_context *ctx);
    duk_ret_t iothubclient_setoption(duk_context *ctx);
    duk_ret_t iothubclient_sendeventasync(duk_context *ctx);
    duk_ret_t iothubclient_destory(duk_context *ctx);

    void iothubclient_init(duk_context *ctx);
    void iothubclient_test(duk_context *ctx);

#ifdef __cplusplus
}
#endif

#endif /* IOTHUBCLIENTLIB_H */