//var cstr = "HostName=yaweiIotHub.azure-devices.net;DeviceId=yaweiFirstDevice;SharedAccessKey=e3MIeWm8OvVL2zwqiVymLla1uIvsl3vQVO0cFUjoKb8=";
var cstr = "<your ConnectionString>";
var ConnectionString = require('azure-iot-device').ConnectionString;
var mqtt = require('azure-iot-device').Protocol.MQTT;
var client = require('azure-iot-device').Client;
var Message = require('azure-iot-device').Message;

var cn = ConnectionString.parse(cstr);
print(cn.HostName);
print(cn.SharedAccessKeyName);
print(cn.SharedAccessKey);

var iothubclient = client.fromConnectionString(cstr, mqtt);
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
iothubclient.close(closeCallback);
