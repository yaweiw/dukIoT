#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef use_test_runtime
#define use_test_runtime
#endif

#ifndef do_test
#define do_test
#endif

#ifdef use_test_runtime
#include "duktapert/testrt/duktape.h"
#else
#include "duktapert/duktape.h"
#endif

#include "libs/iothubclientlib/iothubclientlib.h"
#include "helper/helper.h"



int main(int argc, const char *argv[]) {
    #ifdef use_test_runtime
    (void)printf("use_test_runtime on\r\n");
    #endif
    duk_context *ctx = NULL;
    char line[4096];
    char idx;
    int ch;

    ctx = duk_create_heap_default();
    if (!ctx) {
        (void)printf("Failed to create a Duktape heap.\n");
        exit(1);
    }

    // register native funcs here
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, duk_readFile, 1 /*nargs*/);
    duk_put_prop_string(ctx, -2, "readFile");
    duk_pop(ctx);

    iothubclient_init(ctx);
    #ifdef do_test
    iothubclient_test(ctx);
    (void)printf("\nFinish test.\n\n");
    #endif

    if (duk_peval_file(ctx, "modulesearch.js") != 0) {
        (void)printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        goto finished;
    }
    duk_pop(ctx);  // ignore result

    (void)printf("Input JS file to execute.\n\n");
    memset(line, 0, sizeof(line));
    idx = 0;
    for (;;) {
        if (idx >= sizeof(line)) {
            printf("Line too long\n");
            exit(1);
        }

        ch = fgetc(stdin);
        if (ch == 0x0a) {
            line[idx++] = '\0';
            if(strcmp(line, "quit") == 0) {
                goto finished;
            }
            if (duk_peval_file(ctx, line) != 0) {
                (void)printf("Error: %s\n", duk_safe_to_string(ctx, -1));
                duk_pop(ctx);  // ignore result
            }
            idx = 0;
        } else {
            line[idx++] = (char) ch;
        }
    }
finished:
    duk_destroy_heap(ctx);

    exit(0);
}