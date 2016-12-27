 'use strict';

 var iothub = require('azure-iothub');
 var connectionString = 'HostName=yaweiIotHub.azure-devices.net;DeviceId=yaweiFirstDevice;SharedAccessKey=e3MIeWm8OvVL2zwqiVymLla1uIvsl3vQVO0cFUjoKb8=';

 var registry = iothub.Registry.fromConnectionString(connectionString);
 var device = new iothub.Device(null);
 device.deviceId = 'myFirstNodeDevice';
 registry.create(device, function(err, deviceInfo, res) {
   if (err) {
     registry.get(device.deviceId, printDeviceInfo);
   }
   if (deviceInfo) {
     printDeviceInfo(err, deviceInfo, res)
   }
 });

 function printDeviceInfo(err, deviceInfo, res) {
     print("deviceInfo");
 }
