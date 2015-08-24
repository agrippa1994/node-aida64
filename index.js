var aida64 = require("./build/Release/node-aida64");
var xmlParser = require("xml2js").parseString;

module.exports = {
	readAIDA64Sync: function() {
		var obj = {};
		xmlParser("<root>" + aida64.readAIDA64Sync() + "</root>", { explicitRoot: false }, function(err, result) {
			if(err)
				throw err;

			obj = result;
		});

		return obj;
	},

	readAIDA64: function(callback) {
		return aida64.readAIDA64(function(err, data) {
			if(err)
				return callback(err, null);

			xmlParser("<root>" + data + "</root>", { explicitRoot: false }, function(err, result) {
				if(err)
					return callback(err, null);

				return callback(null, result);
			});
		});
	}
}
