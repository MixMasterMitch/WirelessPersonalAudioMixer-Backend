var Backend = require('../src/main');

Backend.initialize();
console.log(Backend.Hardware.getInputs());

// Start the transmitter and receiver
Backend.Transmitter.start();
Backend.Receiver.start();

// Make sure node keeps running
Backend.runAsyncTask();

// Register localhost with the transmitter to start audio flow
Backend.Transmitter.registerReceiver('127.0.0.1');

setInterval(function() {
    console.log(Backend.Transmitter.getAudioInputLevels());
    console.log(Backend.Receiver.getAudioOutputLevel());
}, 1000);
