#pragma once

#include <cstdint>


enum class MidiEventType {
    NOTE_OFF = 0x80,
    NOTE_ON = 0x90,
    CONTROL_CHANGE = 0xB0,
    PITCH_BEND = 0xE0,
    UNKNOWN = 0x00
};

struct MidiEvent {
    MidiEventType type = MidiEventType::UNKNOWN;
    uint8_t channel = 0;
    uint8_t note = 0;
    uint8_t velocity = 0;
    int value = 0; // For CC or Pitch Bend
};

