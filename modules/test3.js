//var cstr = "<your ConnectionString>";
var cstr = "HostName=yaweiIotHub.azure-devices.net;DeviceId=yaweiFirstDevice;SharedAccessKey=e3MIeWm8OvVL2zwqiVymLla1uIvsl3vQVO0cFUjoKb8=";
var mqtt = require('azure-iot-device').Protocol.MQTT;
var Message = require('azure-iot-device').Message;
var iothubclient = new IoTHubClient(cstr, mqtt);
var IoTHubClientLLHandle = iothubclient.fromConnectionString(cstr, mqtt);
var msg = new Message("some msg data");
//iothubclient.sendeventasync(IoTHubClientLLHandle, msg.getData());

var message = iothubclient.receive(IoTHubClientLLHandle);
print(message);
iothubclient.close(IoTHubClientLLHandle);