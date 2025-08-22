#ifndef AMIDI_PROCESSOR_HPP
#define AMIDI_PROCESSOR_HPP

#include "midi/processors/IMidiProcessor.hpp"

class AMidiProcessor : public IMidiProcessor {
public:
    virtual ~AMidiProcessor() = default;
    void processEvent(const MidiEvent& event) override {
        // Default implementation does nothing
    }
    void setMidiEventCallback(MidiEventCallback callback) override {
        _midiEventCallback = callback;
    }

protected:
    MidiEventCallback _midiEventCallback;
};


#endif // AMIDI_PROCESSOR_HPP