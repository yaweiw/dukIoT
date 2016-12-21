#include "helper.h"


int propExists(duk_context *ctx, char* line) {
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, line);
    printf("obj.propertyName = %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);
    return 0;
}


int duk_register(duk_context *ctx, DUKAPIHANDLE cFuncName, const char* propName) {
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, cFuncName, 2 /*nargs*/);
    duk_put_prop_string(ctx, -2, propName);
    duk_pop(ctx);
    return 0;
}