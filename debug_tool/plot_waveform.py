#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

# Path to the raw file
file_path = '../build/bass_output.bin'

# Read the raw float data
data = np.fromfile(file_path, dtype=np.float32)

# Plot the waveform
plt.figure(figsize=(12, 6))
plt.plot(data)
plt.title('Waveform from Probe')
plt.xlabel('Sample Number')
plt.ylabel('Amplitude')
plt.grid(True)
plt.show()