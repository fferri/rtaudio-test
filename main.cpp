#include <cmath>
#include <chrono>
#include <thread>

#include "RtAudio.h"

RtAudio audio;

unsigned int numChannels = 1;
unsigned int sampleRate = 44100;
unsigned int bufferFrames = 256;

long t = 0;

int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData)
{
    float *out = reinterpret_cast<float*>(outputBuffer);
    float *in = reinterpret_cast<float*>(inputBuffer);

    std::cout << "audioCallback(" << outputBuffer << ", " << inputBuffer << ", " << nBufferFrames << ", " << streamTime << ", " << status << ", " << userData << ")" << std::endl;
    for(int i = 0; i < nBufferFrames; i++)
    {
        double v = sin(10. * double(t++) * M_PI * 2. / double(sampleRate));

        for(int j = 0; j < numChannels; j++)
            *out++ = v;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    audio.showWarnings();


    if(audio.getDeviceCount() == 0)
    {
        std::cout << "There are no available sound devices." << std::endl;
        exit(1);
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = audio.getDefaultOutputDevice();
    parameters.nChannels = numChannels;
    RtAudio::DeviceInfo info = audio.getDeviceInfo(parameters.deviceId);
    RtAudio::StreamOptions options;
    options.streamName = "rtaudio test";
    //options.flags = RTAUDIO_SCHEDULE_REALTIME; // | RTAUDIO_MINIMIZE_LATENCY;
    std::cout << "opening device '" << (info.probed ? info.name : "???") << "'..." << std::endl;
    auto e = audio.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, &audioCallback, NULL, &options);
    if(e != RtAudioErrorType::RTAUDIO_NO_ERROR) {
        std::cerr << "error in openStream: " << e << std::endl;
        exit(1);
    }
    e = audio.startStream();
    if(e != RtAudioErrorType::RTAUDIO_NO_ERROR) {
        std::cerr << "error in startStream: " << e << std::endl;
        exit(1);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    return 0;
}
