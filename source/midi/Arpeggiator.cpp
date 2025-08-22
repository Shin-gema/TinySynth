
#include "midi/processors/Arpeggiator.hpp"
#include "core/MidiEvent.hpp"
#include <algorithm>
#include <iostream>

Arpeggiator::Arpeggiator()
    : _currentIndex(0),
      _stopThread(false),
      _rate(125) // Tempo par défaut : 120 BPM (croches)
{
    _workerThread = std::thread(&Arpeggiator::runArpeggio, this);
}

Arpeggiator::~Arpeggiator() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stopThread = true;
    }
    _cv.notify_one();
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

void Arpeggiator::processEvent(const MidiEvent& event) {
    bool should_notify = false;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        switch (event.type) {
            case MidiEventType::NOTE_ON: {
                if (std::find(_heldNotes.begin(), _heldNotes.end(), event.note) == _heldNotes.end()) {
                    _heldNotes.push_back(event.note);
                    std::sort(_heldNotes.begin(), _heldNotes.end());
                    should_notify = true;
                }
                break;
            }
            case MidiEventType::NOTE_OFF: {
                auto it = std::find(_heldNotes.begin(), _heldNotes.end(), event.note);
                if (it != _heldNotes.end()) {
                    _heldNotes.erase(it);
                    should_notify = true;
                }
                break;
            }
            default: {
                if (_midiEventCallback) {
                    _midiEventCallback(event);
                }
                break;
            }
        }
    }

    if (should_notify) {
        _cv.notify_one(); // Notifie le thread de l'arpégiateur qu'un changement a eu lieu
    }
}

void Arpeggiator::runArpeggio() {
    while (!_stopThread.load()) {
        MidiEvent noteOnEvent, noteOffEvent;
        bool shouldPlay = false;
        int noteToPlay = 0;

        {
            std::unique_lock<std::mutex> lock(_mutex);
            // Attend qu'on lui signale de s'arrêter OU qu'il y ait des notes à jouer
            _cv.wait(lock, [this] { return _stopThread.load() || !_heldNotes.empty(); });

            if (_stopThread.load()) {
                return; // Sortie du thread
            }

            // Si nous sommes réveillés et qu'il y a des notes, nous en jouons une
            if (!_heldNotes.empty()) {
                if (_currentIndex >= _heldNotes.size()) {
                    _currentIndex = 0;
                }
                noteToPlay = _heldNotes[_currentIndex];
                _currentIndex++;
                shouldPlay = true;
            }
        } // Le verrou est libéré ici

        if (shouldPlay) {
            if (_midiEventCallback) {
                noteOnEvent = {MidiEventType::NOTE_ON, 0, static_cast<unsigned char>(noteToPlay), static_cast<unsigned char>(127)};
                _midiEventCallback(noteOnEvent);

                std::this_thread::sleep_for(_rate / 2);

                noteOffEvent = {MidiEventType::NOTE_OFF, 0, static_cast<unsigned char>(noteToPlay), 0};
                _midiEventCallback(noteOffEvent);

                // Petite pause avant la prochaine note pour respecter le tempo
                std::this_thread::sleep_for(_rate / 2);
            }
        }
    }
}
