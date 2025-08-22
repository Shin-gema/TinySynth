#include "module/generator/LFO.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

LFO::LFO(float sampleRate)
    : _waveform(Waveform::Sine), _frequency(1.0f), _sampleRate(sampleRate), _phase(0.0f) {
}

void LFO::setWaveform(Waveform waveform) {
    _waveform = waveform;
}

void LFO::setFrequency(float frequency) {
    _frequency = frequency;
}

void LFO::process(int bufferSize) {
    _internalBuffer.resize(bufferSize, 0.0f);

    float phaseIncrement = (2.0f * M_PI * _frequency) / _sampleRate;

    for (int i = 0; i < bufferSize; ++i) {
        switch (_waveform) {
            case Waveform::Sine:
                _internalBuffer[i] = std::sin(_phase);
                break;
            case Waveform::Square:
                _internalBuffer[i] = (_phase < M_PI) ? 1.0f : -1.0f;
                break;
            case Waveform::Triangle:
                _internalBuffer[i] = 2.0f * std::abs(2.0f * (_phase / (2.0f * M_PI)) - 1.0f) - 1.0f;
                break;
            case Waveform::Sawtooth:
                _internalBuffer[i] = 2.0f * (_phase / (2.0f * M_PI)) - 1.0f;
                break;
        }

        _phase += phaseIncrement;
        if (_phase >= 2.0f * M_PI) {
            _phase -= 2.0f * M_PI;
        }

        _internalBuffer[i] = (_internalBuffer[i] + 1.0f) * 0.5f; // Normalize to [0, 1]
    }
}