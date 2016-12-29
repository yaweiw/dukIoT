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
             cb();
        },
        sendEvent : function (message, cb) {
            if(!this.IoTHubClientHandle) {
                print("IoTHubClient not constructed");
            } else {
                this.IoTHubClient.sendeventasync(this.IoTHubClientHandle, message.getData());
            }
            cb();
        },
        receive : function (cb) {
            if(!this.IoTHubClientHandle) {
                print("IoTHubClient not constructed");
            } else {
                var message = this.IoTHubClient.receive(this.IoTHubClientHandle);
            }
            cb();
            return message;
        },
        close : function(cb) {
            if(!this.IoTHubClientHandle) {
                print("IoTHubClient not constructed");
            } else {
                this.IoTHubClient.close(this.IoTHubClientHandle);
            }
            cb();
        }
    }
    return obj;
}
