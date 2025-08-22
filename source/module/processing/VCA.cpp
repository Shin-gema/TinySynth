#include "module/processing/VCA.hpp"
#include <iostream>

VCA::VCA() : AModule() {
}

VCA::~VCA() {
}

void VCA::process(int bufferSize) {
    _internalBuffer.assign(bufferSize, 0.0f);

    if (_input.size() < 2 || !_input[AUDIO_IN] || !_input[MOD_IN]) {
        return;
    }

    auto& audioBuffer = _input[AUDIO_IN]->getInternalBuffer();
    auto& modBuffer = _input[MOD_IN]->getInternalBuffer();

    for (int i = 0; i < bufferSize; ++i) {
        if (i < audioBuffer.size() && i < modBuffer.size()) {
            float modValue = modBuffer[i];
            _internalBuffer[i] = audioBuffer[i] * modValue;
        }
    }
}