#include <stdio.h>
#include <stdlib.h>

#ifndef use_test_runtime
#define use_test_runtime
#endif

#ifdef use_test_runtime
#include "../duktapert/testrt/duktape.h"
#define use_test_on
#else
#include "../duktapert/duktape.h"
#endif

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

// warning C4113: 'duk_ret_t (__cdecl *)(duk_context *,const char *)' differs in parameter lists from 'duk_c_function'
duk_ret_t duk_readFile(duk_context *ctx) {
    duk_push_string_file(ctx, duk_to_string(ctx, -1));
    return 1;
}

