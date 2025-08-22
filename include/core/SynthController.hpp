#ifndef SYNTH_CONTROLLER_HPP
#define SYNTH_CONTROLLER_HPP

#include "core/MidiEvent.hpp"
#include "core/GraphManager.hpp"

class SynthController {
public:
    SynthController(GraphManager& graphManager);

    void onMidiEvent(const MidiEvent& event);

private:
    GraphManager& _graphManager; // Reference to the GraphManager for module management

};

#endif // SYNTH_CONTROLLER_HPP