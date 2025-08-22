# TinySynth Module Guide

This document provides a detailed description of the core processing modules available in TinySynth. Each module is a node in the audio processing graph.

## Module Categories

Modules are organized into several categories based on their function:

*   **Generator Modules**: Create signals (e.g., audio waves, low-frequency modulations).
*   **Processing Modules**: Modify incoming audio signals (e.g., filtering, amplification).
*   **Utility Modules**: Provide control signals or perform other helper tasks (e.g., envelopes).
*   **I/O Modules**: Handle the interface with the outside world (e.g., audio output).

---

## Generator Modules

### Oscillator

*   **Source**: `module/generator/Oscillator.hpp`
*   **Role**: The primary sound source. It generates periodic waveforms at a specific frequency.
*   **Inputs**:
    *   `Frequency (Control)`: Determines the pitch of the generated sound, typically driven by MIDI note information.
    *   `Waveform (Parameter)`: Selects the waveform shape (e.g., Sine, Square, Sawtooth, Triangle).
    *   `Frequency Modulation (Control)`: An optional input, often from an LFO, to create vibrato effects.
*   **Outputs**:
    *   `Audio Out (Audio)`: The generated audio signal.

### LFO (Low-Frequency Oscillator)

*   **Source**: `module/generator/LFO.hpp`
*   **Role**: Generates waveforms at very low frequencies, not for hearing, but for modulating other parameters.
*   **Inputs**:
    *   `Frequency (Parameter)`: The speed of the LFO (e.g., 0.1 Hz to 20 Hz).
    *   `Waveform (Parameter)`: The shape of the modulation signal.
*   **Outputs**:
    *   `Control Out (Control)`: The modulation signal, which can be routed to parameters like an oscillator's frequency or a filter's cutoff.

---

## Processing Modules

### VCF (Voltage-Controlled Filter)

*   **Source**: `module/processing/VCF.hpp`
*   **Role**: Sculpts the timbre of the sound by removing or boosting certain frequencies. This is a core component of subtractive synthesis.
*   **Inputs**:
    *   `Audio In (Audio)`: The audio signal to be filtered.
    *   `Cutoff Frequency (Control)`: The primary parameter of the filter, determining which frequency is the focal point of the filtering action.
    *   `Resonance (Control)`: Emphasizes frequencies near the cutoff point.
    *   `Cutoff Modulation (Control)`: An optional input, often from an LFO or an ADSR envelope, to create filter sweeps.
*   **Outputs**:
    *   `Audio Out (Audio)`: The filtered audio signal.

### VCA (Voltage-Controlled Amplifier)

*   **Source**: `module/processing/VCA.hpp`
*   **Role**: Controls the volume (amplitude) of an audio signal based on a control signal. It acts like an automated volume knob.
*   **Inputs**:
    *   `Audio In (Audio)`: The audio signal whose volume is to be controlled.
    *   `Amplitude Modulation (Control)`: The control signal that dictates the volume. This is typically connected to an ADSR envelope to shape the note's dynamics.
*   **Outputs**:
    *   `Audio Out (Audio)`: The audio signal with its amplitude adjusted.

### Mixer

*   **Source**: `module/processing/Mixer.hpp`
*   **Role**: Combines multiple audio signals into a single output.
*   **Inputs**:
    *   `Audio In 1 (Audio)`: First audio signal.
    *   `Audio In 2+ (Audio)`: Additional audio signals.
*   **Outputs**:
    *   `Audio Out (Audio)`: The sum of all input signals.

---

## Utility Modules

### ADSR Module

*   **Source**: `module/utility/ADSRModule.hpp` & `utility/ADSR.hpp`
*   **Role**: Generates a time-based control signal, the ADSR (Attack, Decay, Sustain, Release) envelope. This is essential for shaping the dynamics of a note.
*   **Inputs**:
    *   `Gate (Event)`: A trigger signal (like MIDI Note On/Off) that starts and releases the envelope.
    *   `Attack Time (Parameter)`
    *   `Decay Time (Parameter)`
    *   `Sustain Level (Parameter)`
    *   `Release Time (Parameter)`
*   **Outputs**:
    *   `Control Out (Control)`: The envelope signal, typically routed to a VCA's amplitude input or a VCF's cutoff input.

---

## I/O Modules

### AudioOutput

*   **Source**: `module/IO/AudioOutput.hpp`
*   **Role**: The terminal node of the graph. It takes the final audio signal and sends it to the audio interface (via the ALSA wrapper).
*   **Inputs**:
    *   `Audio In (Audio)`: The final, mixed audio signal to be played.
*   **Outputs**: None (sends to hardware).
