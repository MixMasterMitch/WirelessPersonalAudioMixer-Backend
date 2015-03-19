#include "AudioReceiverThread.hpp"

using namespace v8;

AudioReceiverThread::AudioReceiverThread(int sampleRate, int framesPerBuffer, int port, AudioOutputThread* audioOutputThread)
        : framesPerBuffer(framesPerBuffer), shutdown(false), level(0.0f), audioOutputThread(audioOutputThread) {
    audioDecoder = new OpusAudioDecoder(sampleRate, framesPerBuffer);
    udpReceiver = new UdpReceiver("0.0.0.0", port);
}

void AudioReceiverThread::Start(void* arg) {
    cout << "Starting Audio Receiver Thread" << endl;
    AudioReceiverThread* thiz = static_cast<AudioReceiverThread*>(arg);

    thiz->Run();
}

void AudioReceiverThread::Stop() {
    shutdown = true;
}

void AudioReceiverThread::Run() {
    unsigned char encodedSamples[framesPerBuffer * sizeof(float)];
    float decodedSamples[framesPerBuffer];

    while (!shutdown) {

//        int encodedSamplesLength = udpReceiver->Receive(decodedSamples, framesPerBuffer * sizeof(float));
        int encodedSamplesLength = udpReceiver->Receive(encodedSamples, framesPerBuffer * sizeof(float));

        audioDecoder->Decode(encodedSamples, encodedSamplesLength, decodedSamples);

        audioOutputThread->SetLatestSamples(decodedSamples);

        int i;
        float sum = 0.0f;
        for (i = 0; i < framesPerBuffer; i++) {
            sum += decodedSamples[i] * decodedSamples[i];
        }
        float rms = sqrt(sum / framesPerBuffer);
        float db = 20.0f * log10(rms/0.1f);
        level = 0.8f * level + 0.2f * (db + 80);
    }
    cout << "Shutdown Audio Receiver Thread" << endl;
}

float AudioReceiverThread::GetAudioOutputLevel() {
    return level;
}



