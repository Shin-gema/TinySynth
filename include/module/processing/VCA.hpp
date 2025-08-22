#ifndef VCA_HPP
#define VCA_HPP

#include "module/AModule.hpp"

class VCA : public AModule {
public:
    constexpr static size_t AUDIO_IN = 0;
    constexpr static size_t MOD_IN = 1;
    VCA();
    ~VCA() override;
    void process(int bufferSize) override;
};
#endif // VCA_HPP