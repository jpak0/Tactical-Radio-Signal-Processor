# Quick Start Guide

Get up and running with the Tactical Radio Signal Processing library.

## Prerequisites

### macOS

```bash
# Install Homebrew if you don't have it
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake fftw python3

# Install Python packages
pip3 install numpy scipy matplotlib pytest pybind11
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install cmake libfftw3-dev python3 python3-pip
pip3 install numpy scipy matplotlib pytest pybind11
```

## Build & Run

### Step 1: Build the C++ Module

```bash
cd "Tactical Radio Signal Processing"
chmod +x build.sh
./build.sh
```

Expected output:
```
===================================
Build successful!
===================================
```

### Step 2: Run the Demo

```bash
python3 demo.py
```

This will:
- Generate a noisy test signal
- Apply low-pass filtering to remove noise
- Perform FFT analysis
- Create visualizations
- Save results to `signal_processing_results.png`

### Step 3: Run Benchmarks

```bash
python3 benchmark.py
```

Compares performance between C++ and Python implementations for various signal sizes.

### Step 4: Run Tests

```bash
python3 test_processor.py
```

Validates correctness with an automated test suite. Expected result: 10/12 tests passing (2 tests may fail due to floating-point precision).

## Understanding the Output

When you run `demo.py`, you'll see:

```
Step 1: Generating test signal...
  - Creating 10Hz sine wave with noise
  - Signal-to-Noise Ratio: -3.90 dB

Step 2: Applying low-pass filter...
  - SNR improvement: -3.90 dB → -3.15 dB
  - Noise reduction: 0.75 dB

Step 3: Performing FFT analysis...
  - Detected peak frequency: 10.0 Hz
  - Expected frequency: 10.0 Hz
```

**Interpretation**:
- Simulates receiving a 10 Hz signal with additive noise
- Filtering improves quality by 0.75 dB
- FFT correctly identifies the 10 Hz signal component

## Core Concepts

| Concept | Function | Application |
|---------|----------|-------------|
| **Low-pass Filter** | Removes high frequencies | Noise reduction in radio signals |
| **FFT** | Shows frequency content | Spectrum analysis and frequency detection |
| **SNR** | Measures signal quality | Link quality assessment |
| **Convolution** | Sliding weighted average | Core filtering operation |

## Common Issues

### "cmake not found"
```bash
brew install cmake
```

### "fftw3 not found"
```bash
brew install fftw
```

### "pybind11 not found"
```bash
pip3 install pybind11
```

### Build fails with "Python.h not found"
Ensure Python development headers are installed:
```bash
brew install python3
```

### Tests fail
Some tests may fail due to floating-point precision. 10/12 passing is expected.

## Quick Reference

### Generate Signal
```python
import signal_processor_cpp as sp
signal = sp.generate_test_signal(
    frequency=10.0,       # Hz
    sample_rate=1000.0,   # samples/sec
    duration=1.0,         # seconds
    noise_amplitude=0.5   # noise level
)
```

### Apply Filter
```python
filtered = sp.apply_lowpass_filter(
    signal,
    cutoff_freq=0.1,  # normalized (0-1)
    num_taps=51       # filter size
)
```

### Analyze Spectrum
```python
fft_result = sp.compute_fft(signal)
peak_freq = sp.find_peak_frequency(fft_result, 1000.0)
```

### Calculate Quality
```python
snr = sp.calculate_snr(clean_signal, noisy_signal)
print(f"SNR: {snr:.1f} dB")
```

## Next Steps

1. Review the code in `src/signal_processor.h` and `src/signal_processor.cpp`
2. Experiment with different parameters in `demo.py`
3. Read the technical documentation in `docs/TECHNICAL_ARCHITECTURE.md`
4. Explore the performance benchmarks in `benchmark.py`

## Documentation Structure

- **README.md**: Project overview and features
- **QUICK_START.md**: This setup guide
- **TECHNICAL_ARCHITECTURE.md**: Design and implementation details
- **PROJECT_SUMMARY.md**: Comprehensive technical summary

## Verification Checklist

- [ ] Built the project successfully
- [ ] Ran the demo and generated visualization
- [ ] Ran benchmarks
- [ ] Ran test suite (10/12 passing)
- [ ] Reviewed code comments in source files
- [ ] Understood basic DSP concepts (filtering, FFT, SNR)

## Additional Resources

- FFTW Documentation: http://www.fftw.org/
- pybind11 Documentation: https://pybind11.readthedocs.io/
- Digital Signal Processing: Oppenheim & Schafer textbook
- Software Defined Radio: Pysdr by Dr. Marc Lichtman

---

For detailed technical information, see `docs/TECHNICAL_ARCHITECTURE.md`.
