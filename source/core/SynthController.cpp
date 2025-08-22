
#include "core/SynthController.hpp"

#include <iostream>

SynthController::SynthController(GraphManager& graphManager)
    : _graphManager(graphManager) {
}

void SynthController::onMidiEvent(const MidiEvent& event) {
    switch (event.type) {
    case MidiEventType::NOTE_ON:
        // Handle note on event
        _graphManager.broadcastReset(); // Reset all modules in the graph
        _graphManager.setNote(event.note, event.velocity); // Set note and velocity
        _graphManager.triggerEnvelope(true, event.velocity); // Trigger envelope with velocity
        break;
    case MidiEventType::NOTE_OFF:
        // Handle note off event
        _graphManager.triggerEnvelope(false, 0.0f); // Trigger envelope release
        break;
    default:
        // Handle other MIDI events if necessary
        break;
    }
}