var WirelessPersonalAudioMixerBackend = require('../build/Release/WirelessPersonalAudioMixerBackend');
var _ = require('lodash');

module.exports = {
    Transmitter : {
        start : WirelessPersonalAudioMixerBackend.startTransmittingAudio,
        registerReceiver : WirelessPersonalAudioMixerBackend.registerReceiver,
        stop : WirelessPersonalAudioMixerBackend.stopTransmittingAudio
    },
    Receiver : {
        start : WirelessPersonalAudioMixerBackend.startReceivingAudio,
        stop : WirelessPersonalAudioMixerBackend.stopReceivingAudio
    },
    Hardware : {
        print : WirelessPersonalAudioMixerBackend.printAudioIO,
        getInputs : function() {
            var inputInfo = _.values(WirelessPersonalAudioMixerBackend.getAudioInputInfo());
            var inputs = [];
            _.each(inputInfo, function(input) {
                for (var i = 0; i < input.numInputChannels; i++) {
                    inputs.push({
                        name : input.name,
                        deviceNumber : input.deviceNumber,
                        channelNumber : i,
                        latency : input.latency
                    })
                }
            });
            return inputs;
        }
    },
    initialize : WirelessPersonalAudioMixerBackend.initializePA,
    terminate : WirelessPersonalAudioMixerBackend.terminatePA,
    runAsyncTask : WirelessPersonalAudioMixerBackend.runAsyncTask
};
