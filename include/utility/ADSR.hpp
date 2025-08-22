#pragma once

#include <cstdint>

class ADSR {
public:
    ADSR();

    void setAttack(float attackSeconds);
    void setDecay(float decaySeconds);
    void setSustain(float sustainLevel);
    void setRelease(float releaseSeconds);

    void gate(bool on, uint8_t velocity);
    float process();

    void setSampleRate(float sampleRate);

    void reset() {
        _state = State::Idle;
        _output = 0.0f;
        _velocityFactor = 1.0f;
    }

private:
    enum class State {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };

    State _state;
    float _sampleRate;
    float _attackRate;
    float _decayRate;
    float _releaseRate;
    float _sustainLevel;
    float _output;
    float _velocityFactor;
};

