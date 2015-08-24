var aida64 = require("./index.js");

// Asynchronous read
aida64.readAIDA64(function(err, data){
	if(err)
		return console.log("Error while reading data: " + err);

	console.log("Asynchronous read");
	console.log(JSON.stringify(data));
});

// Synchronous read
try {
	console.log("Synchronous read");
	console.log(JSON.stringify(aida64.readAIDA64Sync()));
} catch(e) {
	console.log("Error while reading data: " + e);
}
