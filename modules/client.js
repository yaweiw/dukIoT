this.ConnectionString;
this.Protocol;
this.IoTHubClient;
this.IoTHubClientLLHandle;

this.fromConnectionString = function (connectionstring, protocol) {
    var obj = {
        ConnectionString : connectionstring,
        Protocol : protocol,
        IoTHubClient : new IoTHubClient(connectionstring, protocol), // call iothubclient_constructor
        open : function (cb) {
             this.IoTHubClientLLHandle = this.IoTHubClient.fromConnectionString(connectionstring, protocol); // call iothubclient_createfromconnectionstring
             cb();
        },
        sendEvent : function (message, cb) {
            if(!this.IoTHubClientLLHandle) {
                print("IoTHubClient not constructed");
            } else {
                this.IoTHubClient.sendeventasync(this.IoTHubClientLLHandle, message.getData()); // call iothubclient_sendeventasync
            }
            cb();
        },
        receiveEvent : function () {
            if(!this.IoTHubClientLLHandle) {
                print("IoTHubClient not constructed");
            } else {
                var message = this.IoTHubClient.receive(this.IoTHubClientLLHandle); // call iothubclient_receive
            }
            return message;
        },
        close : function(cb) {
            if(!this.IoTHubClientLLHandle) {
                print("IoTHubClient not constructed");
            } else {
                this.IoTHubClient.close(this.IoTHubClientLLHandle); // call iothubclient_destory
            }
            cb();
        }
    }
    return obj;
}
