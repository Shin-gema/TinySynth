#include "preset/pianoSynth.hpp"

#include "module/generator/Oscillator.hpp"
#include "module/IO/AudioOutput.hpp"
#include "module/processing/Mixer.hpp"
#include "module/utility/Probe.hpp"
#include "module/utility/ADSRModule.hpp"
#include "module/processing/VCA.hpp"
#include "module/processing/VCF.hpp"
#include "module/generator/LFO.hpp"

void PianoSynthPreset::load(bool debug) {
    float sampleRate = 44100.0f;


    // Create modules
    auto oscillator = std::make_shared<Oscillator>(sampleRate);
    oscillator->setWaveform(Waveform::Sawtooth);

    auto oscillator2 = std::make_shared<Oscillator>(sampleRate);
    oscillator2->setWaveform(Waveform::Sawtooth);
    oscillator2->setDethune(1.5f);

    auto audioOutput = std::shared_ptr<AudioOutput>(&AudioOutput::getInstance(), [](AudioOutput*){});
    
    auto adsrModule = std::make_shared<ADSRModule>(sampleRate);
    adsrModule->setAttack(0.1f);
    adsrModule->setDecay(1.5f);
    adsrModule->setSustain(0.01f);
    adsrModule->setRelease(0.4f);

    auto adsrModuleVCF = std::make_shared<ADSRModule>(sampleRate);
    adsrModuleVCF->setAttack(0.01f);
    adsrModuleVCF->setDecay(0.200f);
    adsrModuleVCF->setSustain(0.01f);
    adsrModuleVCF->setRelease(0.2f);

    auto lfo = std::make_shared<LFO>(sampleRate);
    lfo->setWaveform(Waveform::Sine);


    auto mixer = std::make_shared<Mixer>();
    auto probe = std::make_shared<Probe>("MainOutput", "output.bin");
    auto vca = std::make_shared<VCA>();

    auto vcf = std::make_shared<VCF>(sampleRate);
    vcf->setCutoff(750.0f);
    vcf->setResonance(2.0f);
    vcf->setModulationAmount(0.6f);

    // Add modules to the graph

    _graphManager.addModule(oscillator);
    _graphManager.addModule(oscillator2);
    _graphManager.addModule(mixer);
    _graphManager.addModule(vca);
    _graphManager.addModule(vcf);
    _graphManager.addModule(lfo);
    _graphManager.addModule(adsrModuleVCF);
    _graphManager.addModule(adsrModule);
    _graphManager.addModule(audioOutput);
    _graphManager.addModule(probe);

    _graphManager.connect(mixer, oscillator, 0);
    _graphManager.connect(mixer, oscillator2, 1);

    _graphManager.connect(vcf, mixer, VCF::AUDIO_IN);
    _graphManager.connect(vcf, lfo, VCF::MOD_IN);

    _graphManager.connect(vca, vcf, VCA::AUDIO_IN);
    _graphManager.connect(vca, adsrModule, VCA::MOD_IN);
    _graphManager.connect(audioOutput, vca, 0);
    _graphManager.connect(probe, vca, 0);

}