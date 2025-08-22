#include "module/processing/Mixer.hpp"

#include <iostream>

Mixer::Mixer() : AModule() {
}

Mixer::~Mixer() {
}

void Mixer::process(int bufferSize) {
     // This part is good. It clears your mixer's internal buffer.
     _internalBuffer.assign(bufferSize, 0.0f);

     if (_input.empty()) {
         std::cerr << "Mixer has no inputs." << std::endl;
         return;
     }
 
    // This part is also good. It pulls from the inputs and sums them.
     for (const auto& inputModule : _input) {
         if (inputModule) {
             // Assuming getInternalBuffer() exists and returns the input's buffer
             std::vector<float>& inputBuffer = inputModule->getInternalBuffer();
             for (int i = 0; i < bufferSize; ++i) {
                 if (i < inputBuffer.size()) {
                    _internalBuffer[i] += inputBuffer[i];
                 }
             }
         }
     }
     float gain = 1.0f / static_cast<float>(_input.size());
     for (int i = 0; i < bufferSize; ++i) {
         _internalBuffer[i] *= gain; // Normalize the mix
     }
}