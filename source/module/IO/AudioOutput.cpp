#include "module/IO/AudioOutput.hpp"

#include <iostream>

AudioOutput::AudioOutput()
    : _alsaWrapper("default"){ // Default   ALSA device{
    unsigned int sampleRate = 44100; // Default sample rate
    snd_pcm_format_t format = SND_PCM_FORMAT_FLOAT; // Default format
    if (!_alsaWrapper.open()) {
        throw std::runtime_error("Failed to open ALSA device");
    }
    if (!_alsaWrapper.configure(sampleRate, format)) {
        _alsaWrapper.close();
        throw std::runtime_error("Failed to configure ALSA device");
    }
}

AudioOutput::~AudioOutput() {
}


void AudioOutput::process(int bufferSize) {

    _internalBuffer.resize(bufferSize, 0.0f);

    for (const auto& inputModule : _input) {
        if (inputModule) {
            const std::vector<float>& inputBuffer = inputModule->getInternalBuffer();
            for (int i = 0; i < bufferSize; ++i) {
                if (i < inputBuffer.size()) {
                    _internalBuffer[i] += inputBuffer[i];
                } else {
                    std::cerr << "Warning: Input buffer size is smaller than expected." << std::endl;
                }
            }
        }
    }
    if (_internalBuffer.empty()) {
        std::cerr << "Warning: Internal buffer is empty, nothing to write." << std::endl;
        return;
    }

    _alsaWrapper.write(_internalBuffer);
    _internalBuffer.clear(); 
}

