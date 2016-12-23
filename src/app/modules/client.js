this.ConnectionString;
this.Protocol;

this.fromConnectionString = function (connectionstring, protocol) {
    var obj = {
        ConnectionString : connectionstring,
        Protocol : protocol,
        open : function (cb) {
            print("open is called!");
            cb();
        },
        sendEvent : function (message, cb) {
            print("sendEven is called!");
            cb();
        }
    }
    return obj;
}

this.open = function (cb) {
    print("open is called!");
    cb();
}

this.sendEvent = function (message, cb) {
    print("sendEven is called!");
    cb();
}
