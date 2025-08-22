#include "audio/AlsaWrapper.hpp"
#include <iostream>
#include <unistd.h>

AlsaWrapper::AlsaWrapper(const std::string& deviceName)
    :   _deviceName(deviceName),
        _pcmHandle(nullptr),
        _hwParams(nullptr)
        {}

AlsaWrapper::~AlsaWrapper() {
    close();
}

bool AlsaWrapper::open() {
    int err = snd_pcm_open(&_pcmHandle, _deviceName.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        std::cerr << "Error opening PCM device: " << snd_strerror(err) << std::endl;
        return false;
    }
    return true;
}

void AlsaWrapper::close() {
    if (_pcmHandle) {
        snd_pcm_close(_pcmHandle);
        _pcmHandle = nullptr;
    }
}

bool AlsaWrapper::configure(unsigned int& sampleRate, snd_pcm_format_t& format) {
    snd_pcm_hw_params_alloca(&_hwParams);
    int err = snd_pcm_hw_params_any(_pcmHandle, _hwParams);
    if (err < 0) {
        std::cerr << "Error initializing hardware parameters: " << snd_strerror(err) << std::endl;
        return false;
    }

    err = snd_pcm_hw_params_set_access(_pcmHandle, _hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        std::cerr << "Error setting access type: " << snd_strerror(err) << std::endl;
        return false;
    }

    err = snd_pcm_hw_params_set_format(_pcmHandle, _hwParams, format);
    if (err < 0) {
        std::cerr << "Error setting format: " << snd_strerror(err) << std::endl;
        return false;
    }

    // Set number of channels (mono)
    err = snd_pcm_hw_params_set_channels(_pcmHandle, _hwParams, 1);
    if (err < 0) {
        std::cerr << "Error setting channels: " << snd_strerror(err) << std::endl;
        return false;
    }

    err = snd_pcm_hw_params_set_rate_near(_pcmHandle, _hwParams, &sampleRate, nullptr);
    if (err < 0) {
        std::cerr << "Error setting sample rate: " << snd_strerror(err) << std::endl;
        return false;
    }

    // Set buffer size (in frames)
    snd_pcm_uframes_t bufferSize = 4096;
    err = snd_pcm_hw_params_set_buffer_size_near(_pcmHandle, _hwParams, &bufferSize);
    if (err < 0) {
        std::cerr << "Error setting buffer size: " << snd_strerror(err) << std::endl;
        return false;
    }

    // Set period size (in frames)
    snd_pcm_uframes_t periodSize = 1024;
    err = snd_pcm_hw_params_set_period_size_near(_pcmHandle, _hwParams, &periodSize, nullptr);
    if (err < 0) {
        std::cerr << "Error setting period size: " << snd_strerror(err) << std::endl;
        return false;
    }

    err = snd_pcm_hw_params(_pcmHandle, _hwParams);
    if (err < 0) {
        std::cerr << "Error applying hardware parameters: " << snd_strerror(err) << std::endl;
        return false;
    }

    // Prepare the PCM for use
    err = snd_pcm_prepare(_pcmHandle);
    if (err < 0) {
        std::cerr << "Error preparing PCM: " << snd_strerror(err) << std::endl;
        return false;
    }

    return true;
}

void AlsaWrapper::write(const std::vector<float>& buffer) {
    if (!_pcmHandle) {
        std::cerr << "PCM handle is not open." << std::endl;
        return;
    }

    if (buffer.empty()) {
        return;
    }

    snd_pcm_sframes_t frames = snd_pcm_writei(_pcmHandle, buffer.data(), buffer.size());
    
    if (frames < 0) {
        // Handle underrun or other errors
        if (frames == -EPIPE) {
            std::cerr << "Underrun occurred, recovering..." << std::endl;
            snd_pcm_prepare(_pcmHandle);
            // Try to write again after recovery
            frames = snd_pcm_writei(_pcmHandle, buffer.data(), buffer.size());
            if (frames < 0) {
                std::cerr << "Error writing to PCM device after recovery: " << snd_strerror(frames) << std::endl;
            }
        } else if (frames == -ESTRPIPE) {
            std::cerr << "Stream suspended, waiting for resume..." << std::endl;
            while ((frames = snd_pcm_resume(_pcmHandle)) == -EAGAIN) {
                sleep(1); // Wait for resume
            }
            if (frames < 0) {
                std::cerr << "Error resuming stream: " << snd_strerror(frames) << std::endl;
                snd_pcm_prepare(_pcmHandle);
            }
        } else {
            std::cerr << "Error writing to PCM device: " << snd_strerror(frames) << std::endl;
        }
    } else if (frames < static_cast<snd_pcm_sframes_t>(buffer.size())) {
        std::cerr << "Short write, expected " << buffer.size() << " frames, wrote " << frames << " frames." << std::endl;
    }
}

