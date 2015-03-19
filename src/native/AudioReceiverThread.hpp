#ifndef AUDIO_RECEIVER_THREAD_H
#define AUDIO_RECEIVER_THREAD_H

#include <stdio.h>
#include <node.h>
#include <nan.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include "OpusAudioDecoder.hpp"
#include "UdpReceiver.hpp"
#include "AudioOutputThread.hpp"

using namespace std;

/*
    Constantly receives UDP audio packets, decodes them, and passes them to the audioOutputThread.
*/
class AudioReceiverThread {
	public:
		AudioReceiverThread(int sampleRate, int framesPerBuffer, int port, AudioOutputThread* audioOutputThread);
		uv_thread_t thread;
		static void Start(void* arg);
		void Stop();
		float GetAudioOutputLevel();

	private:
	    void Run();
		int framesPerBuffer;
		bool shutdown;
		float level;
		AudioOutputThread* audioOutputThread;
		UdpReceiver* udpReceiver;
		OpusAudioDecoder* audioDecoder;
};

#endif
