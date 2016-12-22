#include "helper.h"

duk_ret_t propExistsGlobal(duk_context *ctx, char* line) {
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, line);
    printf("obj.propertyName = %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);
    return 0;
}

duk_ret_t propExistsInstance(duk_context *ctx, char* line) {
    /* ... stack top has the object ... */
    duk_get_prop_string(ctx, -1, line);
    printf("obj.propertyName = %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);
    return 0;
}


duk_ret_t duk_register(duk_context *ctx, DUKAPIHANDLE cFuncHandle, const char* propName) {
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, cFuncHandle, 2 /*nargs*/);
    duk_put_prop_string(ctx, -2, propName);
    duk_pop(ctx);
    return 0;
}