#ifndef HELPER_H
#define HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DUKAPIHANDLE
#define DUKAPIHANDLE void*
#endif

    int duk_register(duk_context *ctx, DUKAPIHANDLE cFuncName, const char* propName);
    duk_ret_t propExistsGlobal(duk_context *ctx, char* line);
    duk_ret_t propExistsInstance(duk_context *ctx, char* line);
    duk_ret_t duk_readFile(duk_context *ctx);


#ifdef __cplusplus
}
#endif

#endif /* HELPER_H */