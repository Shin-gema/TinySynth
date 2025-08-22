#include "midi/sources/AlsaMidiSource.hpp"

#include <iostream>

AlsaMidiSource::AlsaMidiSource(const std::string& portName)
    : _seq(nullptr), _port(-1), _isRunning(false) {
    if (snd_seq_open(&_seq, portName == "" ? "default" : portName.c_str(), SND_SEQ_OPEN_INPUT, 0) < 0) {
        std::cerr << "Error: Could not open ALSA sequencer." << std::endl;
    }
    snd_seq_set_client_name(_seq, "TinySynth");
    _port = snd_seq_create_simple_port(_seq, "Input",
                                        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                                        SND_SEQ_PORT_TYPE_APPLICATION);
    if (_port < 0) {
        std::cerr << "Error: Could not create sequencer port." << std::endl;
        snd_seq_close(_seq);
        _seq = nullptr;
    }
}

AlsaMidiSource::~AlsaMidiSource() {
    stop();
    if (_seq) {
        snd_seq_close(_seq);
    }
}

void AlsaMidiSource::start() {
    if (_isRunning) return;
    _isRunning = true;
    _thread = std::thread(&AlsaMidiSource::listen, this);
}

void AlsaMidiSource::stop() {
    _isRunning = false;
    if (_thread.joinable()) {
        _thread.join();
    }
}

void AlsaMidiSource::listen() {
    snd_seq_event_t* ev;
    while (_isRunning) {
        if (snd_seq_event_input(_seq, &ev) >= 0) {
            MidiEvent event;
            event.type = static_cast<MidiEventType>(ev->type);
            event.channel = ev->data.control.channel;

            switch (ev->type) {
                case SND_SEQ_EVENT_NOTEON:
                    event.type = MidiEventType::NOTE_ON;
                    event.note = ev->data.note.note;
                    event.velocity = ev->data.note.velocity;
                    break;
                case SND_SEQ_EVENT_NOTEOFF:
                    event.type = MidiEventType::NOTE_OFF;
                    event.note = ev->data.note.note;
                    event.velocity = ev->data.note.velocity;
                    break;
                // Add other cases like CC, Pitch Bend here
                default:
                    event.type = MidiEventType::UNKNOWN;
                    break;
            }

            if (event.type != MidiEventType::UNKNOWN) {
                _midiEventCallback(event);
            }
            snd_seq_free_event(ev);
        }
    }
}