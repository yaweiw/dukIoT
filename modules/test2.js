var cstr = "<your ConnectionString>";

var http = require('azure-iot-device').Protocol.HTTP;
var client = require('azure-iot-device').Client;
var Message = require('azure-iot-device').Message;

var iothubclient = client.fromConnectionString(cstr, http);

function connectCallback (err) {
    if (err) {
        print('[Device] Could not connect: ' + err);
    } else {
        print('[Device] Client connected\n');
    }
}

function receiveCallback (err) {
    if (err) {
        print('[Handle] receive failed: ' + err);
    } else {
        print('[Handle] receive succeed\n');
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
var msg = iothubclient.receive(receiveCallback);
print("received message: " + msg);
iothubclient.close(closeCallback);
