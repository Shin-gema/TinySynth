#include "module/AModule.hpp"
#include "audio/AlsaWrapper.hpp"
#include <cmath>
#include <memory>

class AudioOutput : public AModule {
public:

    static AudioOutput& getInstance() {
        static AudioOutput instance;
        return instance;
    }

    AudioOutput(const AudioOutput&) = delete;
    AudioOutput& operator=(const AudioOutput&) = delete;

    void process(int bufferSize) override;
    

private:
    AudioOutput();
    ~AudioOutput();

    AlsaWrapper _alsaWrapper;
};