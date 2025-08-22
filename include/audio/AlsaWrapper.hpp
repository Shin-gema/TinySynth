#ifndef ALSA_WRAPPER_HPP
    #define ALSA_WRAPPER_HPP

    #include <alsa/asoundlib.h>
    #include <string>
    #include <vector>

    class AlsaWrapper {
    public:
    
        AlsaWrapper(const std::string& deviceName);
        ~AlsaWrapper();

        bool open();
        void close();
        bool configure(unsigned int& sampleRate, snd_pcm_format_t& format);
        void write(const std::vector<float>& buffer);
        
    private:
        std::string _deviceName;
        snd_pcm_t* _pcmHandle;
        snd_pcm_hw_params_t* _hwParams;
    };
#endif // ALSA_WRAPPER_HPP