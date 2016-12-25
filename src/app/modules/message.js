this.data;

this.Message = function (message) {
    var obj = {
        data : message,
        getData : function () { return this.data; }
    };
    return obj;
}