this.ConnectionString = require('connectionstring');
this.Client = require('client');
this.Protocol = require('protocol');
this.Message = function(msg) {
	return require('message').Message(msg);
}
