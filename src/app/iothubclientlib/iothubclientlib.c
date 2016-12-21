#include "iothubclientlib.h"

/* iothubclient.prototype.printName */
duk_ret_t iothubclient_print_connectionstring(duk_context *ctx) {
	/* Stack at entry is: [ ] */

	/* Get access to the 'this' binding. */
	duk_push_this(ctx);  /* -> stack: [ this ] */

	/* Read this.name */
	duk_get_prop_string(ctx, -1, "connectionstring");

	printf("connectionstring: %s\n", duk_safe_to_string(ctx, -1));

	return 0;  /* no return value (= undefined) */
}

/* iothubclient */
duk_ret_t iothubclient_constructor(duk_context *ctx) {
	/* Stack at entry is: [ connectionstring protocol ] */

	/* All Duktape/C functions can be called both as constructors
	 * ("new func()") and functions ("func()").  Sometimes objects
	 * allow both calls, sometimes not.  Here we reject a normal
	 * non-constructor call.
	 */
	if (!duk_is_constructor_call(ctx)) {
		return DUK_RET_TYPE_ERROR;
	}

	/* Get access to the default instance. */
	duk_push_this(ctx);  /* -> stack: [ connectionstring protocol this ] */

	/* Set this.connectionstring to connectionstring. */
	duk_dup(ctx, 0);  /* -> stack: [ connectionstring protocol this connectionstring ] */
	duk_put_prop_string(ctx, -2, "connectionstring");  /* -> stack: [ connectionstring protocol this ] */

    duk_dup(ctx, 1);  /* -> stack: [ connectionstring protocol this protocol ] */
	duk_put_prop_string(ctx, -2, "protocol");  /* -> stack: [ connectionstring protocol this ] */

	/* Return undefined: default instance will be used. */
	return 0;
}


duk_ret_t IoTHubClient_CreateFromConnectionString(duk_context *ctx) {
    // args passe thru ctx
    const char* constr = duk_get_string(ctx, 0);
    const char* protocol = duk_get_string(ctx, 1);
    duk_push_string(ctx, "IoTHubClient_CreateFromConnectionString is called!");
    return 0;
}

duk_ret_t IoTHubMessage_CreateFromByteArray(duk_context *ctx) {
    const char* message  = duk_get_string(ctx, 0);
    const int size = duk_get_int(ctx, 1);
    duk_push_string(ctx, "IoTHubMessage_CreateFromByteArray is called!");
    return 0;
}

duk_ret_t IoTHubClient_SendEventAsync(duk_context *ctx) {
    duk_push_string(ctx, "IoTHubClient_SendEventAsync is called!");
    return 0;
}