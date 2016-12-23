#include "constr.h"

/* IoTHubClient.prototype.printName */
duk_ret_t iothubclient_getconnectionstring(duk_context *ctx) {
    /* Stack at entry is: [ ] */

    /* Get access to the 'this' binding. */
    duk_push_this(ctx);  /* -> stack: [ this ] */

    /* Read this.connectionstring */
    /* whatever last pushed is the return */
    duk_get_prop_string(ctx, -1, "connectionstring"); /* -> stack: [ this cstr ] */

    return 1;  /* 0 = no return value (= undefined) */
}

/* ConnectionString */
duk_ret_t constr_constructor(duk_context *ctx) {
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

    return 0;
}


duk_ret_t iothubclient_setmessagecallback(duk_context *ctx) {
    // args passe thru ctx
    duk_push_string(ctx, "iothubclient_setmessagecallback is called!");
    return 1;
}

duk_ret_t iothubclient_setoption(duk_context *ctx) {
    // args passe thru ctx
    duk_push_string(ctx, "iothubclient_setoption is called!");
    return 1;
}

duk_ret_t iothubclient_sendeventasync(duk_context *ctx) {
    duk_push_string(ctx, "iothubclient_sendeventasync is called!");
    return 1;
}

duk_ret_t iothubclient_dowork(duk_context *ctx) {
    // args passe thru ctx
    duk_push_string(ctx, "iothubclient_dowork is called!");
    return 1;
}

duk_ret_t iothubclient_dispose(duk_context *ctx) {
    // args passe thru ctx
    duk_push_string(ctx, "iothubclient_dispose is called!");
    return 1;
}

void iothubclient_init(duk_context *ctx) {
    // register constructor
    duk_push_c_function(ctx, iothubclient_constructor, 1 /*nargs*/); /* -> stack: [ iothubclient_constructor ] */
    /* Push iothubclient.prototype object. */
    duk_push_object(ctx); /* -> stack: [ iothubclient_constructor prototypeObject(empty) ] */
    /* Set iothubclient.prototype.getconnectionstring. */
    duk_push_c_function(ctx, iothubclient_getconnectionstring, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "getconnectionstring");
    /* Set iothubclient.prototype.setmessagecallback. */
    duk_push_c_function(ctx, iothubclient_setmessagecallback, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "setmessagecallback");
    /* Set iothubclient.prototype.sendeventasync. */
    duk_push_c_function(ctx, iothubclient_sendeventasync, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "sendeventasync");
    /* Set iothubclient.prototype.setoption. */
    duk_push_c_function(ctx, iothubclient_setoption, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "setoption");
    /* Set iothubclient.prototype.dowork. */
    duk_push_c_function(ctx, iothubclient_dowork, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "dowork");
    /* Set iothubclient.prototype.dispose. */
    duk_push_c_function(ctx, iothubclient_dispose, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "dispose");
    /* Set iothubclient.prototype = proto */
    duk_put_prop_string(ctx, -2, "prototype");  /* -> stack: [ iothubclient_constructor ] */
    /* register iothubclient to the global object */
    duk_put_global_string(ctx, "IoTHubClient");  /* -> stack: [ ] */
}

void iothubclient_test(duk_context *ctx) {
    iothubclient_init(ctx);

    printf("Test creation of a new object from Ecmascript code.\n");
    /* Test creation of a new object from Ecmascript code. */
    duk_eval_string(ctx, "new IoTHubClient('myIoTHubConnectionString', 'HTTP')");
    /* ... stack top has result ... */ // [... iothubclientObj ]
    printf("Check properties exist.\n");
    propExistsInstance(ctx, "connectionstring");
    propExistsInstance(ctx, "protocol");
    propExistsInstance(ctx, "getconnectionstring");
    propExistsInstance(ctx, "setmessagecallback");
    propExistsInstance(ctx, "sendeventasync");
    propExistsInstance(ctx, "setoption");
    propExistsInstance(ctx, "dowork");
    propExistsInstance(ctx, "dispose");

    printf("\nCall object methods.\n");
    duk_get_prop_string(ctx, -1, "getconnectionstring");  /* call obj.getconnectionstring(); */ // [... iothubclientObj getconnectionstring ]
    duk_dup(ctx, -2); // [... iothubclientObj getconnectionstring iothubclientObj(this) ]
    duk_call_method(ctx, 0 /*nargs*/); // 0: [ ...iothubclientObj undefined ]; 1: [ ...iothubclientObj cstr ]
    printf("[getconnectionstring] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "setmessagecallback");  /* call obj.setmessagecallback(); */ // [... iothubclientObj setmessagecallback ]
    duk_dup(ctx, -2); // [... iothubclientObj setmessagecallback iothubclientObj(this) ]
    duk_call_method(ctx, 0 /*nargs*/); // 0: [ ...iothubclientObj undefined ]; 1: [ ...iothubclientObj cstr ]
    printf("[setmessagecallback] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "sendeventasync");
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0 /*nargs*/);
    printf("[sendeventasync] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "setoption");
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0 /*nargs*/);
    printf("[setoption] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);

    duk_pop(ctx);  /* pop instance */

    /* Test creation of a new object from C code. */
    printf("\nTest creation of a new object from C code.\n");
    duk_get_global_string(ctx, "IoTHubClient");
    duk_push_string(ctx, "myIoTHubConnectionString");  /* cstr argument */
    duk_push_string(ctx, "HTTP");  /* protocol argument */
    duk_new(ctx, 2 /*nargs*/);
    /* ... stack top has result ... */

    duk_get_prop_string(ctx, -1, "getconnectionstring");  /* call obj.getconnectionstring(); */
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0 /*nargs*/);
    printf("[getconnectionstring] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "setmessagecallback");
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0 /*nargs*/);
    printf("[setmessagecallback] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "sendeventasync");
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0 /*nargs*/);
    printf("[sendeventasync] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);  /* pop call result */

    duk_get_prop_string(ctx, -1, "setoption");
    duk_dup(ctx, -2);
    duk_call_method(ctx, 0 /*nargs*/);
    printf("[setoption] %s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);

    duk_pop(ctx);  /* pop instance */
}