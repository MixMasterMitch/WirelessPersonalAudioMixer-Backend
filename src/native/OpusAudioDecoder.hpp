#ifndef OPUS_AUDIO_DECODER_H
#define OPUS_AUDIO_DECODER_H

#include <stdio.h>
#include <iostream>
#include "opus.h"

using namespace std;

/*
    A simple interface for decoding audio using the OPUS audio encoding.
    Ensure that the sampleRate and the framesPerBuffer values are the same as the audio encoding.
    See: https://www.opus-codec.org/
    See: https://mf4.xiph.org/jenkins/view/opus/job/opus/ws/doc/html/group__opus__decoder.html
*/
class OpusAudioDecoder {
	public:
		OpusAudioDecoder(int sampleRate, int framesPerBuffer);
		int Decode(unsigned char* encodedBuffer, int encodedBufferSize, float* decodedSamples);

	private:
		OpusDecoder* decoder;
		int framesPerBuffer;
};

#endif
