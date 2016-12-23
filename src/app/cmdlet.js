var cstr = "HostName=yaweiIotHub.azure-devices.net;DeviceId=yaweiFirstDevice;SharedAccessKey=e3MIeWm8OvVL2zwqiVymLla1uIvsl3vQVO0cFUjoKb8=";
var pro = "HTTP";
var cstr = require('azure-iothub').ConnectionString;
var cn = cstr.parse("HostName=yaweiIotHub.azure-devices.net;DeviceId=yaweiFirstDevice;SharedAccessKey=e3MIeWm8OvVL2zwqiVymLla1uIvsl3vQVO0cFUjoKb8=");
print(cn.HostName);
print(cn.SharedAccessKeyName);
print(cn.SharedAccessKey);




