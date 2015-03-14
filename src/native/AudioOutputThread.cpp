#include "AudioOutputThread.hpp"

using namespace v8;

AudioOutputThread::AudioOutputThread(int sampleRate, int framesPerBuffer, PaStreamParameters outputParameters)
    : framesPerBuffer(framesPerBuffer), shutdown(false) {
    int error;

    latestSamples = new float[framesPerBuffer];
//    buffers = deque<float*>();
    uv_mutex_init(&mutex);

    error = Pa_OpenStream(
        &stream,
        NULL,
        &outputParameters,
        sampleRate,
        framesPerBuffer,
        paClipOff,
        NULL, // PaCallback,
        NULL); // (void *) this);
    if (error) cout << "ERROR: Could not open read stream." << endl;

}

void AudioOutputThread::Start(void* arg) {
    cout << "Starting Audio Output Thread" << endl;
    AudioOutputThread* thiz = static_cast<AudioOutputThread*>(arg);

    thiz->Run();
}

void AudioOutputThread::Run() {
    float latestSamplesCopy[framesPerBuffer];
    int error;
    error = Pa_StartStream(stream);
    if (error) {
        cout << "ERROR: Could start read stream." << endl;
        return;
    }

    while (!shutdown) {
        uv_mutex_lock(&mutex);
        if (newSamples) {
            newSamples = false;
            memcpy((void*) latestSamplesCopy, (void*) latestSamples, framesPerBuffer * sizeof(float));
            uv_mutex_unlock(&mutex);
            Pa_WriteStream(stream, latestSamplesCopy, framesPerBuffer);
        } else {
            uv_mutex_unlock(&mutex);
        }
    }

    error = Pa_CloseStream(stream);
    if (error) cout << "ERROR: Could not close read stream." << endl;

    cout << "Shutdown Audio Output Thread" << endl;
}

void AudioOutputThread::SetLatestSamples(float* samples) {
//    Pa_WriteStream(stream, samples, framesPerBuffer);
    uv_mutex_lock(&mutex);
    memcpy((void*) latestSamples, (void*) samples, framesPerBuffer * sizeof(float));
    newSamples = true;
//    float* buffer = new float[framesPerBuffer];
//    memcpy((void*) buffer, (void*) samples, framesPerBuffer * sizeof(float));
//    buffers.push_back(buffer);
    uv_mutex_unlock(&mutex);
}

int AudioOutputThread::PaCallback(const void* inputBuffer, void *outputBuffer,
                                        unsigned long framesPerBuffer,
                                        const PaStreamCallbackTimeInfo* timeInfo,
                                        PaStreamCallbackFlags statusFlags,
                                        void* userData) {
//    cout << "PaCallback" << endl;
    AudioOutputThread* thiz = (AudioOutputThread*) userData;
    thiz->OutputStreamCallback((const float*) outputBuffer);
    return 0;
}

void AudioOutputThread::OutputStreamCallback(const float* buffer) {
//    cout << "InputStreamCallback" << endl;

//    uv_mutex_lock(&mutex);
//    if (buffers.empty()) {
//        cout << "No data ready..." << endl;
//        uv_mutex_unlock(&mutex);
//        return;
//    }
//    float* samples = buffers.front();
//    buffers.pop_front();
//    memcpy((void*) buffer, (void*) samples, framesPerBuffer * sizeof(float));
//
//    memcpy((void*) buffer, (void*) latestSamples, framesPerBuffer * sizeof(float));
//    newSamples = false;
//    uv_mutex_unlock(&mutex);
//    cout << "Copied Memory" << endl;

}

void AudioOutputThread::Stop() {
    shutdown = true;
}

