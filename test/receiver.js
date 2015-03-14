var Backend = require('../src/main');

Backend.initialize();
console.log(Backend.Hardware.getInputs());

// Start the receiver
Backend.Receiver.start();

// Make sure node keeps running
Backend.runAsyncTask();

