#include "module/utility/Probe.hpp"
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>

Probe::Probe(const std::string& name, const std::string& filePath) : _name(name) {
    _file.open(filePath, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!_file.is_open()) {
        std::cerr << "Error: Could not open file for probe: " << filePath << std::endl;
    }
}

Probe::~Probe() {
    if (_file.is_open()) {
        _file.close();
    }
}

void Probe::process(int bufferSize) {
    if (_input.empty()) {
        std::cerr << "Error: No input connected to Probe module." << std::endl;
        return;
    }

    auto& inputBuffer = _input[0]->getInternalBuffer();
    if (inputBuffer.size() < bufferSize) {
        std::cerr << "Error: Input buffer size is smaller than expected." << std::endl;
        return;
    }

    writeData(inputBuffer);

    _internalBuffer = inputBuffer;
}

void Probe::writeData(const std::vector<float>& buffer) {
    _file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(float));

    // Calculate and print statistics
    float peak = 0.0f;
    for (float sample : buffer) {
        if (std::abs(sample) > peak) {
            peak = std::abs(sample);
        }
    }

    double sum_sq = std::inner_product(buffer.begin(), buffer.end(), buffer.begin(), 0.0);
    float rms = std::sqrt(sum_sq / buffer.size());

    int zero_crossings = 0;
    for (size_t i = 1; i < buffer.size(); ++i) {
        if ((buffer[i] > 0 && buffer[i-1] <= 0) || (buffer[i] < 0 && buffer[i-1] >= 0)) {
            zero_crossings++;
        }
    }

    std::cout << "Probe '" << _name << "': "
                << "Peak=" << peak << ", "
                << "RMS=" << rms << ", "
                << "ZeroCrossings=" << zero_crossings
                << std::endl;
}

