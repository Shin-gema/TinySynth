#include "core/GraphManager.hpp"

#include "midi/sources/AlsaMidiSource.hpp"
#include "core/SynthController.hpp"
#include "midi/processors/Arpeggiator.hpp"

#include "preset/bass.hpp"
#include "preset/pianoSynth.hpp"

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // 1. Initialize and start the MIDI Manager
    AlsaMidiSource midiSource("default");
    std::cout << "MIDI Manager started. Please connect your MIDI device." << std::endl;


    midiSource.start();
    // --- Graph Setup ---
    float sampleRate = 44100.0f;
    //GraphManager graphManager(512);
    // BassPreset bassPreset;
    // bassPreset.load(true);
    PianoSynthPreset pianoSynthPreset;
    pianoSynthPreset.load(true);

    SynthController synthController(pianoSynthPreset.getGraphManager());
    Arpeggiator arpeggiator;

    // Set the MIDI event callback
    auto syntSink = [&](const MidiEvent& event) {
        synthController.onMidiEvent(event);
    };

    arpeggiator.setMidiEventCallback(syntSink);

    midiSource.setOutput([&](const MidiEvent& event) {
        arpeggiator.processEvent(event);
    });

    std::cout << "Synth running... Press Ctrl+C to exit." << std::endl;
    while (true) {
        pianoSynthPreset.getGraphManager().processBlock();
        // The audio is now handled by the AudioOutput module and ALSA
        // We can just sleep briefly.
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Cleanup
    midiSource.stop();
    return 0;
}