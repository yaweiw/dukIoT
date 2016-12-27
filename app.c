#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duktapert/duktape.h"
#include "libs/iothubclientlib/iothubclientlib.h"
#include "helper/helper.h"

//#define DO_TEST

int main(int argc, const char *argv[]) {
    duk_context *ctx = NULL;
    char line[4096];
    char idx;
    int ch;

    ctx = duk_create_heap_default();
    if (!ctx) {
        printf("Failed to create a Duktape heap.\n");
        exit(1);
    }

    // register native funcs here
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, duk_readFile, 1 /*nargs*/);
    duk_put_prop_string(ctx, -2, "readFile");
    duk_pop(ctx);
    //duk_register(ctx, duk_readFile, "readFile");

    iothubclient_init(ctx);
    #ifdef DO_TEST
    iothubclient_test(ctx);
    printf("\nFinish test.\n\n");
    #endif

    printf("Register module search function.\n\n");
    if (duk_peval_file(ctx, "modulesearch.js") != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        goto finished;
    }
    duk_pop(ctx);  /* ignore result */

    printf("Interpret and execute test.js.\n\n");
    if (duk_peval_file(ctx, "test.js") != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        goto finished;
    }
    duk_pop(ctx);  /* ignore result */

finished:
    duk_destroy_heap(ctx);

    exit(0);
}