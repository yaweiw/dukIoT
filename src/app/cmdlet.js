var cstr = "abc";
var pro = "HTTP";
var message = "msg";
var IoTclient = this.ioTHubClientCreateFromConnectionString(cstr, pro);
print(IoTclient);
var MessageHandle = this.ioTHubMessageCreateFromByteArray(message, message.length);
print(MessageHandle);
print(this.ioTHubClientSendEventAsync(IoTclient, MessageHandle));
