#ifndef IOTHUBCLIENTLIB_H
#define IOTHUBCLIENTLIB_H

#ifdef __cplusplus
extern "C" {
#endif
    duk_ret_t iothubclient_constructor(duk_context *ctx);
    duk_ret_t iothubclient_createfromconnectionstring(duk_context *ctx);
    duk_ret_t iothubclient_receive(duk_context *ctx);
    duk_ret_t iothubclient_setoption(duk_context *ctx);
    duk_ret_t iothubclient_sendeventasync(duk_context *ctx);
    duk_ret_t iothubclient_destory(duk_context *ctx);

    void iothubclient_init(duk_context *ctx);
    void iothubclient_test(duk_context *ctx);

    typedef struct MESSAGE_DATA_TAG
    {
        char* data;
        size_t size;
    } MESSAGE_DATA, *MESSAGE_DATA_HANDLE;
#ifdef __cplusplus
}
#endif

#endif /* IOTHUBCLIENTLIB_H */