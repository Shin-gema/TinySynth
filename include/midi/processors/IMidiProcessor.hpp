#ifndef IMIDI_PROCESSOR_HPP
    #define IMIDI_PROCESSOR_HPP

    #include "core/MidiEvent.hpp"
    #include <functional>

    class IMidiProcessor {
    public:
        virtual ~IMidiProcessor() = default;

        virtual void processEvent(const MidiEvent& event) = 0;
        
        using MidiEventCallback = std::function<void(const MidiEvent&)>;
        virtual void setMidiEventCallback(MidiEventCallback callback) = 0;
    };

#endif // IMIDI_PROCESSOR_HPP