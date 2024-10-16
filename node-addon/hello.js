const process = require('process');
console.log("Current working directory: ", process.cwd());

    try {
        var addon = require(process.cwd() + "/Release/node-addon.node");
    } catch(e) {
        var addon = require(process.cwd() + "/Debug/node-addon.node");
    }

console.log(addon.hello()); // 'world'
