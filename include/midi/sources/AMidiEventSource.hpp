#ifndef AMIDI_EVENT_SOURCE_HPP
#define AMIDI_EVENT_SOURCE_HPP

#include "midi/sources/IMidiEventSource.hpp"

class AMidiEventSource : public IMidiEventSource {
public:
    virtual ~AMidiEventSource() = default;

    void start() override {
        // Default implementation does nothing
    }

    void stop() override {
        // Default implementation does nothing
    }

    void setOutput(IMidiProcessor::MidiEventCallback callback) override {
        _midiEventCallback = callback;
    }

protected:
    IMidiProcessor::MidiEventCallback _midiEventCallback;
};

#endif // AMIDI_EVENT_SOURCE_HPP