#include "module/generator/Oscillator.hpp"
#include <cmath>

Oscillator::Oscillator(float sampleRate)
    :   _waveform(Waveform::Sine),
        _frequency(440.0f),
        _sampleRate(sampleRate),
        _phase(0.0f),
        _dethune(0.0f) 
{
}

void Oscillator::setWaveform(Waveform waveform) {
    _waveform = waveform;
}

void Oscillator::setFrequency(float frequency) {
    _frequency = frequency;
}

void Oscillator::setNote(int noteNumber) {
    // MIDI note to frequency conversion
    // A4 = 440 Hz, MIDI note 69
    _frequency = 440 * std::pow(2.0f, (noteNumber - 69.0f) / 12.0f);
    _frequency += _dethune;
}

void Oscillator::setDethune(float dethune) {
    _dethune = dethune;
}

void Oscillator::process(int bufferSize) {
    _internalBuffer.resize(bufferSize, 0.0f);

    float phaseIncrement = (2.0f * M_PI * _frequency) / _sampleRate;

    for (int i = 0; i < bufferSize; ++i) {
        float amp = 1.0f; 
        switch (_waveform) {
            case Waveform::Sine:
                _internalBuffer[i] = amp * std::sin(_phase);
                break;
            case Waveform::Square:
                _internalBuffer[i] = amp * (_phase < M_PI ? 1.0f : -1.0f);
                break;
            case Waveform::Triangle:
                _internalBuffer[i] = amp * (2.0f * std::abs(2.0f * (_phase / (2.0f * M_PI)) - 1.0f) - 1.0f);
                break;
            case Waveform::Sawtooth:
                _internalBuffer[i] = amp * (2.0f * (_phase / (2.0f * M_PI)) - 1.0f);
                break;
        }

        _phase += phaseIncrement;
        if (_phase >= 2.0f * M_PI) {
            _phase -= 2.0f * M_PI;
        }
    }
}