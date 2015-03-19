#include "AudioInputAndTransmitterThread.hpp"

using namespace v8;

AudioInputAndTransmitterThread::AudioInputAndTransmitterThread(int sampleRate, int framesPerBuffer, int port, PaStreamParameters inputParameters)
    : framesPerBuffer(framesPerBuffer), port(port), inputParameters(inputParameters) {
    int error, i;

    audioEncoder = new OpusAudioEncoder(sampleRate, framesPerBuffer);
    udpSender = new UdpSender();

    for (i = 0; i < inputParameters.channelCount; i++) {
        levels.push_back(0.0f);
    }

    error = Pa_OpenStream(
        &stream,
        &inputParameters,
        NULL,
        sampleRate,
        framesPerBuffer,
        paClipOff,
        PaCallback,
        (void *) this);
    if (error) cout << "ERROR: Could not open read stream." << endl;
}

void AudioInputAndTransmitterThread::Start(void* arg) {
    cout << "Starting Audio Input and Transmitter Thread" << endl;
    AudioInputAndTransmitterThread* thiz = static_cast<AudioInputAndTransmitterThread*>(arg);
    thiz->Run();
}

void AudioInputAndTransmitterThread::Run() {
    int error;
    error = Pa_StartStream(stream);
    if (error) cout << "ERROR: Could not start read stream." << endl;
}

void AudioInputAndTransmitterThread::Stop() {
    int error;
    error = Pa_CloseStream(stream);
    if (error) cout << "ERROR: Could not close read stream." << endl;
    cout << "Stopped Audio Input and Transmitter Thread" << endl;
}

int AudioInputAndTransmitterThread::PaCallback(const void* inputBuffer, void *outputBuffer,
                                        unsigned long framesPerBuffer,
                                        const PaStreamCallbackTimeInfo* timeInfo,
                                        PaStreamCallbackFlags statusFlags,
                                        void* userData) {
    AudioInputAndTransmitterThread* thiz = static_cast<AudioInputAndTransmitterThread*>(userData);
    thiz->OnSamples((const float*) inputBuffer);
    return 0;
}

void AudioInputAndTransmitterThread::OnSamples(const float* samples) {
    int i, j;
    float mixedSamples[framesPerBuffer];
    unsigned char encodedSamples[framesPerBuffer * sizeof(float)];

    for (i = 0; i < framesPerBuffer; i++) {
        mixedSamples[i] = 0.0f;
        for (j = 0; j < inputParameters.channelCount; j++) {
            mixedSamples[i] += samples[i * inputParameters.channelCount + j];
        }
    }

    int encodedSamplesLength = audioEncoder->Encode(mixedSamples, encodedSamples, framesPerBuffer * sizeof(float));

    for (vector<char*>::iterator it = receivers.begin() ; it != receivers.end(); ++it) {
        udpSender->Send(*it, port, encodedSamples, encodedSamplesLength);
    }


    for (i = 0; i < inputParameters.channelCount; i++) {
        float sum = 0.0f;
        for (j = 0; j < framesPerBuffer; j++) {
            sum += samples[j * inputParameters.channelCount + i] * samples[j * inputParameters.channelCount + i];
        }
        float rms = sqrt(sum / framesPerBuffer);
        float db = 20.0f * log10(rms/0.1f) + 80.0f;
        levels[i] = db * 0.2f + levels[i] * 0.8f;
    }
}

void AudioInputAndTransmitterThread::RegisterReceiver(char* ip) {
    receivers.push_back(ip);

    cout << "Receivers:" << endl;
    for (vector<char*>::iterator it = receivers.begin() ; it != receivers.end(); ++it) {
        cout << ' ' << *it << endl;
    }
}

vector<float> AudioInputAndTransmitterThread::GetAudioInputLevels() {
    return levels;
}

