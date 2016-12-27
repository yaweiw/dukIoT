this.fromConnectionString = function (connectionstring) {
    var obj = {
        create : function (device, cb) {
            print("device.deviceId = " + device.deviceId);
            cb();
        },
        get : function(deviceId, cb) {
            print("deviceID = " + deviceId);
            cb();
        }
    }
    return obj;
}