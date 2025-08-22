#ifndef BASS_PRESET_HPP
#define BASS_PRESET_HPP

#include "preset/APreset.hpp"

class BassPreset : public APreset {
public:
    BassPreset() = default;
    ~BassPreset() override = default;
    void load(bool debug = false) override;
};
#endif // BASS_PRESET_HPP