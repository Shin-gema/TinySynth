#ifndef MIXER_HPP
    #define MIXER_HPP

#include "module/AModule.hpp"

class Mixer : public AModule {
public:
    Mixer();
    ~Mixer() override;
    void process(int bufferSize) override;
};

#endif // MIXER_HPP 