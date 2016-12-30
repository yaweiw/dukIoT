//var cstr = "<your ConnectionString>";
var cstr = "HostName=yaweiIotHub.azure-devices.net;DeviceId=yaweiFirstDevice;SharedAccessKey=e3MIeWm8OvVL2zwqiVymLla1uIvsl3vQVO0cFUjoKb8=";
var mqtt = require('azure-iot-device').Protocol.MQTT;
var client = require('azure-iot-device').Client;

function connectCallback (err) {
    if (err) {
        print('[Device] Could not connect: ' + err);
    } else {
        print('[Device] Client connected\n');
    }
}

function closeCallback (err) {
    if (err) {
        print('[Handle] close failed: ' + err);
    } else {
        print('[Handle] close succeed\n');
    }
}

var iothubclient = client.fromConnectionString(cstr, mqtt);
iothubclient.open(connectCallback);
var msg = iothubclient.receiveEvent();
print("received message: " + msg);
iothubclient.close(closeCallback);
