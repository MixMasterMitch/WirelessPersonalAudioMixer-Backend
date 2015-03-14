#include "OpusAudioEncoder.hpp"

OpusAudioEncoder::OpusAudioEncoder(int sampleRate, int framesPerBuffer)
        : framesPerBuffer(framesPerBuffer) {
    int error;
    this->encoder = opus_encoder_create(sampleRate, 1, OPUS_APPLICATION_RESTRICTED_LOWDELAY, &error);
    if (error) printf("ERROR: Could not create OPUS encoder.");
}

int OpusAudioEncoder::Encode(const float* buffer, unsigned char* encodedBuffer, int encodedBufferSize) {
    return opus_encode_float(encoder, buffer, framesPerBuffer, encodedBuffer, encodedBufferSize);
}
