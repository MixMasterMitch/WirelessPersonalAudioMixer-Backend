#ifndef AUDIO_OUTPUT_THREAD_H
#define AUDIO_OUTPUT_THREAD_H

#include <stdio.h>
#include <node.h>
#include <nan.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <deque>
#include "portaudio.h"

using namespace std;

/*
    Constantly tries to output the latest samples that it is received. Old samples are not played and if no new samples
    are available, the same samples will be replayed. Audio is outputted using Port Audio
    See: See: http://portaudio.com/
*/
class AudioOutputThread {
	public:
		AudioOutputThread(int sampleRate, int framesPerBuffer, PaStreamParameters outputParameters);
		static void Start(void* arg);
		uv_thread_t thread;

        float* latestSamples;
        deque<float*> buffers;

        bool newSamples;
        uv_mutex_t mutex;

        void SetLatestSamples(float* samples);
		void Stop();

	private:
		int framesPerBuffer;
		PaStream* stream;
		bool shutdown;
		static int PaCallback(const void* inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData);
		void OutputStreamCallback(const float* buffer);
		void Run();
};

#endif
