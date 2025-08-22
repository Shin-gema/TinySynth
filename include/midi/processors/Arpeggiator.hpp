
#ifndef ARPEGGIATOR_HPP
#define ARPEGGIATOR_HPP

#include "midi/processors/AMidiProcessor.hpp"
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>

class Arpeggiator : public AMidiProcessor {
public:
    Arpeggiator();
    ~Arpeggiator();

    void processEvent(const MidiEvent& event) override;

private:
    void runArpeggio();

    std::mutex _mutex;
    std::condition_variable _cv;
    std::vector<int> _heldNotes;
    size_t _currentIndex;

    std::atomic<bool> _stopThread;
    std::thread _workerThread;

    std::chrono::milliseconds _rate;
};

#endif // ARPEGGIATOR_HPP