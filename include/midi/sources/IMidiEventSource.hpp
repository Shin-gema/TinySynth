#ifndef IMIDI_EVENT_SOURCE_HPP
#define IMIDI_EVENT_SOURCE_HPP

#include "midi/processors/IMidiProcessor.hpp"

class IMidiEventSource {
public:
    virtual ~IMidiEventSource() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void setOutput(IMidiProcessor::MidiEventCallback callback) = 0;
    
};

#endif // IMIDI_EVENT_SOURCE_HPP