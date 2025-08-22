#ifndef PIANO_SYNTH_PRESET_HPP
#define PIANO_SYNTH_PRESET_HPP

#include "preset/APreset.hpp"

class PianoSynthPreset : public APreset {
public:
    PianoSynthPreset() = default;
    ~PianoSynthPreset() override = default;
    void load(bool debug = false) override;
};
#endif // PIANO_SYNTH_PRESET_HPP