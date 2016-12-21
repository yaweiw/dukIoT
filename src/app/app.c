#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duktapert/duktape.h"
#include "iothubclientlib/iothubclientlib.h"
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

    // register duktape c API

    duk_push_c_function(ctx, iothubclient_constructor, 1 /*nargs*/);
    //todo : http://wiki.duktape.org/HowtoNativeConstructor.html
    // http://duktape.org/guide.html#programming
    duk_register(ctx, IoTHubClient_CreateFromConnectionString, "ioTHubClientCreateFromConnectionString");
    duk_register(ctx, IoTHubMessage_CreateFromByteArray, "ioTHubMessageCreateFromByteArray");
    duk_register(ctx, IoTHubClient_SendEventAsync, "ioTHubClientSendEventAsync");

    if (duk_peval_file(ctx, "cmdlet.js") != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        goto finished;
    }
    duk_pop(ctx);  /* ignore result */
/*
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
            propExists(ctx, line);
*/

/*
            duk_push_global_object(ctx);
            //duk_get_prop_string(ctx, -1, "processLine");
            duk_get_prop_string(ctx, -1, "connect"); //-1
            duk_push_string(ctx, line);
            if (duk_pcall(ctx, 1) != 0) {
                printf("connect: Error: %s\n", duk_safe_to_string(ctx, -1));
            } else {
                printf("connect: %s\n", duk_safe_to_string(ctx, -1));
            }

            //get the new line
            memcpy(line, duk_get_string(ctx, -1), strlen(duk_get_string(ctx, -1)));
            duk_pop(ctx);

            duk_push_global_object(ctx);
            duk_get_prop_string(ctx, -1, "send");
            duk_push_string(ctx, line);
            if (duk_pcall(ctx, 1) != 0) {
                printf("send: Error: %s\n", duk_safe_to_string(ctx, -1));
            } else {
                printf("send: %s\n", duk_safe_to_string(ctx, -1));
            }

            //get the new line
            memcpy(line, duk_get_string(ctx, -1), strlen(duk_get_string(ctx, -1)));
            duk_pop(ctx);

            duk_push_global_object(ctx);
            duk_get_prop_string(ctx, -1, "receive");
            duk_push_string(ctx, line);
            if (duk_pcall(ctx, 1) != 0) {
                printf("receive: Error: %s\n", duk_safe_to_string(ctx, -1));
            } else {
                printf("receive: %s\n", duk_safe_to_string(ctx, -1));
            }
            duk_pop(ctx);
*/
/*
            idx = 0;
        } else if (ch == EOF) {
            break;
        } else {
            line[idx++] = (char) ch;
        }
    }
*/
 finished:
    duk_destroy_heap(ctx);

    exit(0);
}