const process = require('process');
console.log("Current working directory: ",
    process.cwd());

    try {
        var addon = require("./../build/node-addon/Release/node-addon.node");
    } catch(e) {
        var addon = require("./../build/node-addon/Debug/node-addon.node");
    }

console.log(addon.hello()); // 'world'
