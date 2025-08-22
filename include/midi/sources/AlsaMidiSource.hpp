
#ifndef ALSA_MIDI_SOURCE_HPP
#define ALSA_MIDI_SOURCE_HPP

#include "midi/sources/AMidiEventSource.hpp"
#include <alsa/asoundlib.h>
#include <string>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

class AlsaMidiSource : public AMidiEventSource {
public:
    AlsaMidiSource(const std::string& portName);
    ~AlsaMidiSource() override;

    void start() override;
    void stop() override;

    void listen();

private:

    snd_seq_t* _seq;
    int _port;
    std::thread _thread;
    std::atomic<bool> _isRunning;

    std::queue<MidiEvent> _eventQueue;
    std::mutex _queueMutex;
};

#endif // ALSA_MIDI_SOURCE_HPP