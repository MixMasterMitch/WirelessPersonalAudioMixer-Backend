#include "OpusAudioDecoder.hpp"

OpusAudioDecoder::OpusAudioDecoder(int sampleRate, int framesPerBuffer)
        : framesPerBuffer(framesPerBuffer) {
    int error;
    this->decoder = opus_decoder_create(sampleRate, 1, &error);
    if (error) printf("ERROR: Could not create OPUS decoder.");
}

int OpusAudioDecoder::Decode(unsigned char* encodedBuffer, int encodedBufferSize, float* decodedSamples) {
    return opus_decode_float(decoder, encodedBuffer, encodedBufferSize, decodedSamples, framesPerBuffer, 0);
}
