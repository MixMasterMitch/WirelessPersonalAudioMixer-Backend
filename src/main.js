var WirelessPersonalAudioMixerBackend = require('../build/Release/WirelessPersonalAudioMixerBackend');
var _ = require('lodash');

module.exports = {
    Transmitter : {
        start : WirelessPersonalAudioMixerBackend.startTransmittingAudio,
        registerReceiver : WirelessPersonalAudioMixerBackend.registerReceiver,
        getAudioInputLevels : WirelessPersonalAudioMixerBackend.getAudioInputLevels,
        stop : WirelessPersonalAudioMixerBackend.stopTransmittingAudio
    },
    Receiver : {
        start : WirelessPersonalAudioMixerBackend.startReceivingAudio,
        getAudioOutputLevel : WirelessPersonalAudioMixerBackend.getAudioOutputLevel,
        stop : WirelessPersonalAudioMixerBackend.stopReceivingAudio
    },
    Hardware : {
        print : WirelessPersonalAudioMixerBackend.printAudioIO,
        getInputs : function() {
            var inputInfo = _.values(WirelessPersonalAudioMixerBackend.getAudioInputInfo());
            var inputs = [];
            _.each(inputInfo, function(input) {
                if (input.numInputChannels <= 0) { return; }
                // for (var i = 0; i < input.numInputChannels; i++) {
                    inputs.push({
                        name : input.name,
                        deviceNumber : input.deviceNumber,
                        numChannels : input.numInputChannels,
                        latency : input.latency,
                        isDefault : input.isDefault
                    });
                // }
            });
            return inputs;
        }
    },
    initialize : WirelessPersonalAudioMixerBackend.initializePA,
    terminate : WirelessPersonalAudioMixerBackend.terminatePA,
    runAsyncTask : WirelessPersonalAudioMixerBackend.runAsyncTask
};
