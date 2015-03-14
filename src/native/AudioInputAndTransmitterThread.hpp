#ifndef AUDIO_INPUT_THREAD_H
#define AUDIO_INPUT_THREAD_H

#include <node.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "portaudio.h"
#include "OpusAudioEncoder.hpp"
#include "UdpSender.hpp"

using namespace std;

/*
    Constantly processes audio input, mixes it, encodes it, and transmits it to the receivers. Audio is sampled using Port Audio.
    Port Audio recommends doing the minimum amount of work in the callback to make sure there is not a delay, but when I separated
    mixing, encoding, and transmitting to a different thread, the overhead of signaling the other thread using condition variables was
    too high.
    See: http://portaudio.com/
*/
class AudioInputAndTransmitterThread {
	public:
		AudioInputAndTransmitterThread(int sampleRate, int framesPerBuffer, int port, PaStreamParameters inputParameters);
		uv_thread_t thread;
		static void Start(void* arg);
		void Stop();
		void RegisterReceiver(char* ip);

	private:
		int framesPerBuffer;
		int port;
		vector<char*> receivers;
		PaStreamParameters inputParameters;
		PaStream* stream;
		UdpSender* udpSender;
		OpusAudioEncoder* audioEncoder;
		static int PaCallback(const void* inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData);
		void OnSamples(const float* samples);
		void Run();
};

#endif
