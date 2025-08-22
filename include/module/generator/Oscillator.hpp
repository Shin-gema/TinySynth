#ifndef OSCILLATOR_NODE_HPP
    #define OSCILLATOR_NODE_HPP

    #include "module/AModule.hpp"
    #include "utility/ADSR.hpp"
    #include "module/generator/Waveform.hpp"
    #include <cmath>


    class Oscillator : public AModule {
    public:
        Oscillator(float sampleRate);

        void setWaveform(Waveform waveform);
        void setFrequency(float frequency);
        void setNote(int noteNumber);
        void setDethune(float dethune);

        void process(int bufferSize) override;
    
    private:
        Waveform _waveform;
        float _frequency;
        float _sampleRate;
        float _phase;
        float _dethune;
    };
#endif // OSCILLATOR_NODE_HPP