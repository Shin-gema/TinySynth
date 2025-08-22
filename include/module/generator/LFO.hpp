#ifndef LFO_HPP
#define LFO_HPP

#include "module/AModule.hpp"
#include "module/generator/Waveform.hpp"

class LFO : public AModule {
public:
    LFO(float sampleRate);
    ~LFO() override = default;

    void setWaveform(Waveform waveform);
    void setFrequency(float frequency);
    void process(int bufferSize) override;

private:
    Waveform _waveform;
    float _frequency;
    float _sampleRate;
    float _phase;
};

#endif // LFO_HPP