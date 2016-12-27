this.ConnectionString;
this.Protocol;
this.IoTHubClient;
this.IoTHubClientHandle;

this.fromConnectionString = function (connectionstring, protocol) {
    var obj = {
        ConnectionString : connectionstring,
        Protocol : protocol,
        IoTHubClient : new IoTHubClient(connectionstring, protocol),
        open : function (cb) {
             this.IoTHubClientHandle = this.IoTHubClient.fromConnectionString(connectionstring, protocol);
             print("open: " + this.IoTHubClientHandle);
             print("open is called!");
            cb();
        },
        sendEvent : function (message, cb) {
            if(!this.IoTHubClientHandle) {
                print("IoTHubClient not constructed");
            } else {
                print("sendEvent: " + this.IoTHubClientHandle);
                this.IoTHubClient.sendeventasync(this.IoTHubClientHandle, message.getData());
                print("sendEvent is called!");
            }
            cb();
        },
        close : function(cb) {
            if(!this.IoTHubClientHandle) {
                print("IoTHubClient not constructed");
            } else {
                print("close: " + this.IoTHubClientHandle);
                this.IoTHubClient.close(this.IoTHubClientHandle);
                print("close is called!");
            }
            cb();
        }
    }
    return obj;
}
