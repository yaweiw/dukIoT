var cstr = "test connectionstring";
var pro = "HTTP";
var client = new IoTHubClient(cstr, pro);
print(client.getconnectionstring());
print(client.sendeventasync());
print(client.dowork());
print(client.dispose());



