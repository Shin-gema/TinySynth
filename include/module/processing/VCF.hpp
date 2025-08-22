#ifndef VCF_HPP
#define VCF_HPP

#include "module/AModule.hpp"
#include "core/IResetable.hpp"

class VCF : public AModule, public IResetable {
public:
    constexpr static size_t AUDIO_IN = 0;
    constexpr static size_t MOD_IN = 1;

    VCF(float sampleRate);
    ~VCF() override = default;

    void setCutoff(float cutoff);
    void setResonance(float resonance);
    void setModulationAmount(float modulationAmount);
    void process(int bufferSize) override;
    void reset() override {
        _x1 = _x2 = _y1 = _y2 = 0.0f;
    }

private:
    void calculateCoefficients(float cuttoff, float resonance);

    float _sampleRate;
    float _baseCutoff;
    float _resonance;
    float _modAmount;

    //filter coefficients
    float _b0, _b1, _b2, _a1, _a2;

    //state variables
    float _x1, _x2, _y1, _y2;
}; 
#endif // VCF_HPP