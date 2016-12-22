#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duktapert/duktape.h"
#include "libs/iothubclientlib/iothubclientlib.h"
#include "helper/helper.h"

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

    iothubclient_init(ctx);
    iothubclient_test(ctx);
    printf("\nFinish test.\n\n");

    printf("Interpret and execute cmdlet.js.\n\n");
    if (duk_peval_file(ctx, "cmdlet.js") != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        goto finished;
    }
    duk_pop(ctx);  /* ignore result */

finished:
    duk_destroy_heap(ctx);

    exit(0);
}