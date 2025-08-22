#include "utility/ADSR.hpp"
#include <cmath>

ADSR::ADSR() :
    _state(State::Idle),
    _sampleRate(44100.0f),
    _sustainLevel(0.7f),
    _output(0.0f),
    _velocityFactor(1.0f)
{
    setAttack(0.01f);
    setDecay(0.1f);
    setRelease(0.2f);
}

void ADSR::setSampleRate(float sampleRate) {
    _sampleRate = sampleRate;
}

void ADSR::setAttack(float attackSeconds) {
    _attackRate = (attackSeconds > 0) ? 1.0f / (attackSeconds * _sampleRate) : 1.0f;
}

void ADSR::setDecay(float decaySeconds) {
    _decayRate = (decaySeconds > 0) ? 1.0f / (decaySeconds * _sampleRate) : 1.0f;
}

void ADSR::setSustain(float sustainLevel) {
    _sustainLevel = sustainLevel;
}

void ADSR::setRelease(float releaseSeconds) {
    _releaseRate = (releaseSeconds > 0) ? 1.0f / (releaseSeconds * _sampleRate) : 1.0f;
}

void ADSR::gate(bool on, uint8_t velocity) {
    if (on) {
        _state = State::Attack;
        _velocityFactor = static_cast<float>(velocity) / 127.0f;
    } else {
        if (_state != State::Idle) {
            _state = State::Release;
        }
    }
}

float ADSR::process() {
    switch (_state) {
        case State::Idle:
            _output = 0.0f;
            break;
        case State::Attack:
            _output += _attackRate;
            if (_output >= 1.0f) {
                _output = 1.0f;
                _state = State::Decay;
            }
            break;
        case State::Decay:
            _output -= _decayRate;
            if (_output <= _sustainLevel) {
                _output = _sustainLevel;
                _state = State::Sustain;
            }
            break;
        case State::Sustain:
            // Output remains at sustain level
            _output = _sustainLevel;
            break;
        case State::Release:
            _output -= _releaseRate;
            if (_output <= 0.0f) {
                _output = 0.0f;
                _state = State::Idle;
            }
            break;
    }
    return _output * _velocityFactor;
}

