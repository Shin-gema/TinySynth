#ifndef ADSR_MODULE_HPP
#define ADSR_MODULE_HPP

#include "module/AModule.hpp"
#include "core/IResetable.hpp"
#include "utility/ADSR.hpp"

class ADSRModule : public AModule, public IResetable {
public:
    ADSRModule(float sampleRate = 44100.0f);
    ~ADSRModule() override;

    void setAttack(float attackSeconds);
    void setDecay(float decaySeconds);
    void setSustain(float sustainLevel);
    void setRelease(float releaseSeconds);
    void gate(bool on, uint8_t velocity);
    void process(int bufferSize) override;
    void reset() override;
private:
    ADSR _adsr;
};
#endif // ADSR_MODULE_HPP