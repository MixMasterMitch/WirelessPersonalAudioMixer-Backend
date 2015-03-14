var Backend = require('../src/main');

Backend.initialize();
console.log(Backend.Hardware.getInputs());

// Start the transmitter
Backend.Transmitter.start();

// Make sure node keeps running
Backend.runAsyncTask();

// Register the receiver with the transmitter
Backend.Transmitter.registerReceiver(process.argv[2]);
