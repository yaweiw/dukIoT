var cstr = "IoTHub connection string";

var ConnectionString = require('azure-iot-device').ConnectionString;
var http = require('azure-iot-device').Protocol.HTTP;
var client = require('azure-iot-device').Client;
var Message = require('azure-iot-device').Message;

var iothubclient = client.fromConnectionString(cstr, http);
var msg = new Message("some msg data");

function connectCallback (err) {
    if (err) {
        print('[Device] Could not connect: ' + err);
    } else {
        print('[Device] Client connected\n');
    }
}

function sendCallback (err) {
    if (err) {
        print('[Device] sendEvent failed: ' + err);
    } else {
        print('[Device] sendEvent succeed\n');
    }
}

function closeCallback (err) {
    if (err) {
        print('[Handle] close failed: ' + err);
    } else {
        print('[Handle] close succeed\n');
    }
}


iothubclient.open(connectCallback);

iothubclient.sendEvent(msg, sendCallback);

print(msg.getData() + " has been sent!");

iothubclient.close(closeCallback);

var cn = ConnectionString.parse(cstr);
print(cn.HostName);
print(cn.SharedAccessKeyName);
print(cn.SharedAccessKey);
