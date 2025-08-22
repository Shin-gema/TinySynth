#include "module/processing/VCF.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

VCF::VCF(float sampleRate)
    : _sampleRate(sampleRate), _baseCutoff(1000.0f), _resonance(0.5f), _modAmount(0.0f),
      _x1(0.0f), _x2(0.0f), _y1(0.0f), _y2(0.0f),
      _b0(0.0f), _b1(0.0f), _b2(0.0f), _a1(0.0f), _a2(0.0f)
{
    calculateCoefficients(_baseCutoff, _resonance);
}

void VCF::setCutoff(float cutoff) {
    _baseCutoff = cutoff;
    calculateCoefficients(_baseCutoff, _resonance);
}

void VCF::setResonance(float resonance) {
    _resonance = resonance;
    calculateCoefficients(_baseCutoff, _resonance);
}

void VCF::setModulationAmount(float modulationAmount) {
    _modAmount = modulationAmount;
}

void VCF::calculateCoefficients(float cutoff, float resonance) {
     float omega = 2.0f * M_PI * cutoff / _sampleRate;
     float alpha = std::sin(omega) / (2.0f * resonance);
     float cos_omega = std::cos(omega);
 
     // The raw coefficients
     float a0_raw = 1.0f + alpha;
     float a1_raw = -2.0f * cos_omega;
     float a2_raw = 1.0f - alpha;
     float b0_raw = (1.0f - cos_omega) / 2.0f;
     float b1_raw = 1.0f - cos_omega;
     float b2_raw = (1.0f - cos_omega) / 2.0f;
 
     // We normalize by a0 to simplify the processing loop
     _b0 = b0_raw / a0_raw;
     _b1 = b1_raw / a0_raw;
     _b2 = b2_raw / a0_raw;
     _a1 = a1_raw / a0_raw;
     _a2 = a2_raw / a0_raw;
}

void VCF::process(int bufferSize) {
    _internalBuffer.assign(bufferSize, 0.0f);

    if (_input.size() < 2 || !_input[AUDIO_IN] || !_input[MOD_IN]) {
        return;
    }

    auto& audioBuffer = _input[AUDIO_IN]->getInternalBuffer();
    auto& modBuffer = _input[MOD_IN]->getInternalBuffer();

    for (int i = 0; i < bufferSize; ++i) {
        if (i < audioBuffer.size() && i < modBuffer.size()) {
            float modValue = modBuffer[i] * _modAmount;
            float cutoff = _baseCutoff + modValue;
            calculateCoefficients(cutoff, _resonance);

            float x0 = audioBuffer[i];
            float y0 = _b0 * x0 + _b1 * _x1 + _b2 * _x2 - _a1 * _y1 - _a2 * _y2;

            _x2 = _x1;
            _x1 = x0;
            _y2 = _y1;
            _y1 = y0;

            _internalBuffer[i] = y0;
        }
    }
}