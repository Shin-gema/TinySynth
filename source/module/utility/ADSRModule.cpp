#include "module/utility/ADSRModule.hpp"
#include <iostream>

ADSRModule::ADSRModule(float sampleRate)
    : AModule(), _adsr() {
    _adsr.setSampleRate(sampleRate);
}

ADSRModule::~ADSRModule() = default;

void ADSRModule::setAttack(float attackSeconds) {
    _adsr.setAttack(attackSeconds);
}

void ADSRModule::setDecay(float decaySeconds) {
    _adsr.setDecay(decaySeconds);
}

void ADSRModule::setSustain(float sustainLevel) {
    _adsr.setSustain(sustainLevel);
}

void ADSRModule::setRelease(float releaseSeconds) {
    _adsr.setRelease(releaseSeconds);
}

void ADSRModule::gate(bool on, uint8_t velocity) {
    _adsr.gate(on, velocity);
}

void ADSRModule::reset() {
    _adsr.reset();
    _internalBuffer.clear();
}

void ADSRModule::process(int bufferSize) {
    _internalBuffer.resize(bufferSize, 0.0f);
    for (int i = 0; i < bufferSize; ++i) {
        // Process the ADSR envelope
        float envelopeValue = _adsr.process();
        // Apply the envelope to the output buffer
        _internalBuffer[i] = envelopeValue;
    }
}