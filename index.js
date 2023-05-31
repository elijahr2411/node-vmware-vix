const vmware = require("./build/Release/vmware-vix");

var vix = new vmware.VIXClient();
console.log(vix.Test());