#include "preset/bass.hpp"

#include "module/generator/Oscillator.hpp"
#include "module/utility/ADSRModule.hpp"
#include "module/processing/VCF.hpp"
#include "module/generator/LFO.hpp"
#include "module/IO/AudioOutput.hpp"
#include "module/processing/VCA.hpp"
#include "module/processing/Mixer.hpp"
#include "module/utility/Probe.hpp"

void BassPreset::load(bool debug) {
    float sampleRate = 44100.0f;

    auto osc1 = std::make_shared<Oscillator>(sampleRate);
    osc1->setWaveform(Waveform::Sawtooth);

    auto osc2 = std::make_shared<Oscillator>(sampleRate);
    osc2->setWaveform(Waveform::Square);
    osc2->setDethune(0.5f);

    auto lfo = std::make_shared<LFO>(sampleRate);
    lfo->setWaveform(Waveform::Sine);
    lfo->setFrequency(5.0f); // 5 Hz

    auto adsr = std::make_shared<ADSRModule>(sampleRate);
    adsr->setAttack(0.01f);
    adsr->setDecay(0.1f);
    adsr->setSustain(0.8f);
    adsr->setRelease(0.2f);

    auto vcf = std::make_shared<VCF>(sampleRate);
    vcf->setCutoff(200.0f);
    vcf->setResonance(0.7f);

    auto vca = std::make_shared<VCA>();
    
    auto mixer = std::make_shared<Mixer>();
    
    auto probe = std::make_shared<Probe>("bass_output", "bass_output.bin");

    auto audioOutput = std::shared_ptr<AudioOutput>(&AudioOutput::getInstance(), [](AudioOutput*){});

    // Connections
    _graphManager.addModule(osc1);
    _graphManager.addModule(osc2);
    _graphManager.addModule(lfo);
    _graphManager.addModule(adsr);
    _graphManager.addModule(vcf);
    _graphManager.addModule(vca);
    _graphManager.addModule(mixer);
    _graphManager.addModule(probe);
    _graphManager.addModule(audioOutput);

    _graphManager.connect(mixer, osc1, 0);
    _graphManager.connect(mixer, osc2, 1);

    _graphManager.connect(vcf, mixer, VCF::AUDIO_IN);
    _graphManager.connect(vcf, lfo, VCF::MOD_IN);

    _graphManager.connect(vca, vcf, VCA::AUDIO_IN);
    _graphManager.connect(vca, adsr, VCA::MOD_IN);

    _graphManager.connect(audioOutput, vca, 0);
    if (debug) {
    _graphManager.connect(probe, vca, 0);
    }
}