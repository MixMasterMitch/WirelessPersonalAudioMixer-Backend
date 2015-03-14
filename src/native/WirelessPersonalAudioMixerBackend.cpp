#include <v8.h>
#include <node.h>
#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include "AudioInputAndTransmitterThread.hpp"
#include "AudioOutputThread.hpp"
#include "AudioReceiverThread.hpp"
#include "UselessTask.hpp"

#define SAMPLE_RATE (48000) // 48kHz sample rate
#define FRAMES_PER_BUFFER (120) // 2.5 milliseconds of buffering
#define PORT (5006) // Transmission port

using namespace v8;
using namespace node;

static AudioOutputThread* audioOutputThread;
static AudioReceiverThread* audioReceiverThread;
static bool startedReceivingAudio = false;

static AudioInputAndTransmitterThread* audioInputAndTransmitterThread;
static bool startedTransmittingAudio = false;

// Initializes Port Audio (must be called before other functions)
NAN_METHOD(InitializePA) {
    NanScope();
    Pa_Initialize();
    NanReturnUndefined();
}

// Shuts down Port Audio
NAN_METHOD(TerminatePA) {
    NanScope();
    Pa_Terminate();
    NanReturnUndefined();
}

// Prints to the console information about all of the audio inputs and outputs.
NAN_METHOD(PrintAudioIO) {
    NanScope();

    PaStreamParameters inputParameters, outputParameters;

    int numDevices = Pa_GetDeviceCount();
    printf("Number of devices = %d\n", numDevices);

    for(int i=0; i < numDevices; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        printf("--------------------------------------- device #%d\n", i);
        printf("Name = %s\n", deviceInfo->name);
        printf("Max inputs = %d\n", deviceInfo->maxInputChannels);
        printf("Max outputs = %d\n", deviceInfo->maxOutputChannels);

        printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
        printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
        printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
        printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );

        inputParameters.device = i;
        inputParameters.channelCount = deviceInfo->maxInputChannels;
        inputParameters.sampleFormat = paInt16;
        inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
        inputParameters.hostApiSpecificStreamInfo = NULL;
        
        outputParameters.device = i;
        outputParameters.channelCount = deviceInfo->maxOutputChannels;
        outputParameters.sampleFormat = paInt16;
        outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
        outputParameters.hostApiSpecificStreamInfo = NULL;
    }


    printf("----------------------------------------------\n");

    NanReturnUndefined();
}

// Converts the device info into a javascript object.
NAN_METHOD(GetAudioInputInfo) {
    NanScope();

    Local<Object> obj = NanNew<Object>();

    int numDevices = Pa_GetDeviceCount();

    for(int i=0; i < numDevices; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);

        Local<Object> info = NanNew<Object>();
        info->Set(NanNew<String>("name"), NanNew<String>(deviceInfo->name));
        info->Set(NanNew<String>("deviceNumber"), NanNew<Number>(i));
        info->Set(NanNew<String>("numInputChannels"), NanNew<Number>(deviceInfo->maxInputChannels));
        info->Set(NanNew<String>("latency"), NanNew<Number>(deviceInfo->defaultLowInputLatency * 1000));

        obj->Set(NanNew<Number>(i), info);
    }

    NanReturnValue(obj);
}

// Starts sampling audio from the input device and transmitting over the network to the registered receivers.
NAN_METHOD(StartTransmittingAudio) {
    NanScope();

    if (!startedTransmittingAudio) {
        startedTransmittingAudio = true;

        // input parameters for the default input device
        int device = Pa_GetDefaultInputDevice();
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(device);
        PaStreamParameters inputParameters;
        inputParameters.device = device;
        inputParameters.channelCount = deviceInfo->maxInputChannels;
        inputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
        inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = NULL;

        // start the audio input and transmitter thread
        audioInputAndTransmitterThread = new AudioInputAndTransmitterThread(SAMPLE_RATE, FRAMES_PER_BUFFER, PORT, inputParameters);
        uv_thread_create(&(audioInputAndTransmitterThread->thread), audioInputAndTransmitterThread->Start, audioInputAndTransmitterThread);
    }

    NanReturnUndefined();
}

// Stops the thread for transmitting audio
NAN_METHOD(StopTransmittingAudio) {
    NanScope();

    if (startedTransmittingAudio) {
        startedTransmittingAudio = false;

        audioInputAndTransmitterThread->Stop();
    }

    NanReturnUndefined();
}

NAN_METHOD(RunAsyncTask) {
    NanScope();

    NanAsyncQueueWorker(new UselessTask());
    NanReturnUndefined();
}

// Starts receiving audio over the network and outputs it to the default speakers
NAN_METHOD(StartReceivingAudio) {
    NanScope();

    if (!startedReceivingAudio) {
        startedReceivingAudio = true;

        // Play audio out on the default output device
        PaStreamParameters outputParameters;
        outputParameters.device = Pa_GetDefaultOutputDevice();
        outputParameters.channelCount = 1; /* mono output */
        outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        // Start the receiver and output threads
        audioOutputThread = new AudioOutputThread(SAMPLE_RATE, FRAMES_PER_BUFFER, outputParameters);
        audioReceiverThread = new AudioReceiverThread(SAMPLE_RATE, FRAMES_PER_BUFFER, PORT, audioOutputThread);
        uv_thread_create(&(audioOutputThread->thread), audioOutputThread->Start, audioOutputThread);
        uv_thread_create(&(audioReceiverThread->thread), audioReceiverThread->Start, audioReceiverThread);
    }

    NanReturnUndefined();
}

// Stops the threads responsible for receiving and playing audio
NAN_METHOD(StopReceivingAudio) {
    NanScope();

    if (startedReceivingAudio) {
        startedReceivingAudio = false;

        audioOutputThread->Stop();
        audioReceiverThread->Stop();
    }

    NanReturnUndefined();
}

// Registers the IP address of a new receiver to transmit audio to.
NAN_METHOD(RegisterReceiver) {
    NanScope();

    if (startedTransmittingAudio) {
        NanUtf8String* str = new NanUtf8String(args[0]);
        audioInputAndTransmitterThread->RegisterReceiver(**str);
    }

    NanReturnUndefined();
}


void Initialize(Handle<Object> exports) {
    exports->Set(NanNew<String>("initializePA"), NanNew<FunctionTemplate>(InitializePA)->GetFunction());
    exports->Set(NanNew<String>("terminatePA"), NanNew<FunctionTemplate>(TerminatePA)->GetFunction());
    exports->Set(NanNew<String>("printAudioIO"), NanNew<FunctionTemplate>(PrintAudioIO)->GetFunction());
    exports->Set(NanNew<String>("startTransmittingAudio"), NanNew<FunctionTemplate>(StartTransmittingAudio)->GetFunction());
    exports->Set(NanNew<String>("stopTransmittingAudio"), NanNew<FunctionTemplate>(StopTransmittingAudio)->GetFunction());
    exports->Set(NanNew<String>("startReceivingAudio"), NanNew<FunctionTemplate>(StartReceivingAudio)->GetFunction());
    exports->Set(NanNew<String>("stopReceivingAudio"), NanNew<FunctionTemplate>(StopReceivingAudio)->GetFunction());
    exports->Set(NanNew<String>("runAsyncTask"), NanNew<FunctionTemplate>(RunAsyncTask)->GetFunction());
    exports->Set(NanNew<String>("getAudioInputInfo"), NanNew<FunctionTemplate>(GetAudioInputInfo)->GetFunction());
    exports->Set(NanNew<String>("registerReceiver"), NanNew<FunctionTemplate>(RegisterReceiver)->GetFunction());
}

NODE_MODULE(WirelessPersonalAudioMixerBackend, Initialize);
