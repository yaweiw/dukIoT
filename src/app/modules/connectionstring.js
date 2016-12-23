this.HostName;
this.SharedAccessKeyName;
this.SharedAccessKey;

this.parse = function (constr) {
    var obj = {
        HostName : constr.split(";")[0].split("=")[1],
        SharedAccessKeyName : constr.split(";")[1].split("=")[1],
        SharedAccessKey : constr.split(";")[2].split("=")[1]
    }
    return obj;
}