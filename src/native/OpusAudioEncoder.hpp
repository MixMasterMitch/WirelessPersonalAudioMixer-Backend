#ifndef OPUS_AUDIO_ENCODER_H
#define OPUS_AUDIO_ENCODER_H

#include <stdio.h>
#include <iostream>
#include "opus.h"

using namespace std;

/*
    A simple interface for encoding audio using the OPUS audio encoding.
    Ensure that the sampleRate and the framesPerBuffer values are supported by OPUS.
    See: https://www.opus-codec.org/
    See: https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__encoder.html
*/
class OpusAudioEncoder {
	public:
		OpusAudioEncoder(int sampleRate, int framesPerBuffer);
		int Encode(const float* buffer, unsigned char* encodedBuffer, int encodedBufferSize);

	private:
		OpusEncoder* encoder;
		int framesPerBuffer;
};

#endif
