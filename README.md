# Tactical Radio Signal Processing

A high-performance C++ signal processing library with Python bindings, demonstrating core DSP concepts used in tactical radio communications (like L3Harris systems).

## Overview

This project implements fundamental signal processing operations used in software-defined radios:

- **Low-pass filtering** - Removes high-frequency noise from signals
- **FFT (Fast Fourier Transform)** - Analyzes frequency content of signals
- **SNR calculation** - Measures signal quality
- **Test signal generation** - Creates realistic test scenarios

## Why This Matters for Tactical Radios

Modern tactical radios (like L3Harris Falcon series) are software-defined radios (SDRs) that:

1. **Receive noisy RF signals** - Real-world radio signals are always mixed with noise
2. **Filter to extract data** - Low-pass filters remove interference
3. **Analyze spectrum** - FFT identifies which frequencies contain signals
4. **Process in real-time** - Must handle millions of samples per second

This project demonstrates understanding of these core concepts at a fundamental level.

## Technical Architecture

```
┌─────────────────────────────────────────────────┐
│           Python Application Layer              │
│  (demo.py, benchmark.py, test_processor.py)     │
└─────────────────┬───────────────────────────────┘
                  │ pybind11 bindings
┌─────────────────▼───────────────────────────────┐
│         C++ Signal Processing Engine            │
│  • Low-pass FIR filter (windowed-sinc)          │
│  • FFT (using FFTW3 library)                    │
│  • Signal generation & SNR calculation          │
└──────────────────────────────────────────────────┘
```

## Key Features

- **Professional C++ implementation** - Clean, well-commented code
- **Python integration** - Easy to use from Python via pybind11
- **Comprehensive testing** - Automated test suite with pytest
- **Performance benchmarks** - Compares C++ vs Python implementations
- **Full documentation** - Explains concepts for interview prep
- **Production-ready build system** - CMake for cross-platform compilation

## Quick Start

### Option 1: Docker (Recommended)

```bash
# Build and run with Docker Compose
docker-compose up signal-processor  # Run tests
docker-compose up demo              # Run demonstration
docker-compose up benchmark         # Run benchmarks

# Or build and run directly
docker build -t tactical-radio-signal-processor .
docker run --rm tactical-radio-signal-processor
```

See [DOCKER.md](DOCKER.md) for detailed Docker instructions.

### Option 2: Native Build

#### Prerequisites

```bash
# macOS
brew install cmake fftw python3

# Linux (Ubuntu/Debian)
sudo apt-get install cmake libfftw3-dev python3 python3-pip

# Install Python dependencies
pip3 install numpy scipy matplotlib pytest pybind11
```

#### Build & Run

```bash
# Build the C++ module
chmod +x build.sh
./build.sh

# Run the demo
python3 demo.py

# Run benchmarks
python3 benchmark.py

# Run tests
python3 test_processor.py
```

## Project Structure

```
tactical-radio-signal-processing/
├── README.md                          # This file
├── DOCKER.md                          # Docker setup guide
├── CMakeLists.txt                     # Build configuration
├── build.sh                           # Build script
├── Dockerfile                         # Docker image definition
├── docker-compose.yml                 # Docker Compose configuration
├── .dockerignore                      # Docker build exclusions
├── src/
│   ├── signal_processor.h             # C++ header
│   ├── signal_processor.cpp           # C++ implementation
│   └── bindings.cpp                   # Python bindings
├── demo.py                            # Main demonstration
├── benchmark.py                       # Performance comparison
├── test_processor.py                  # Test suite
└── docs/
    ├── QUICK_START.md                 # Getting started guide
    ├── TECHNICAL_ARCHITECTURE.md      # Design documentation
    └── PROJECT_SUMMARY.md             # Technical summary
```

## Technical Concepts

This project implements and demonstrates:

1. **Signal Processing Fundamentals**
   - Digital filtering (convolution, frequency response)
   - Fast Fourier Transform (time-domain → frequency-domain conversion)
   - Signal quality metrics for communications systems

2. **C++ Performance Optimization**
   - Efficient array processing techniques
   - Integration with optimized libraries (FFTW)
   - Performance trade-offs between C++ and Python

3. **Software Engineering Practices**
   - Modern build systems (CMake)
   - Automated testing (pytest)
   - Performance benchmarking
   - Comprehensive documentation

4. **C++/Python Integration**
   - pybind11 binding implementation
   - Zero-copy array passing
   - Type conversion handling

## Example Output

```
============================================================
SIGNAL PROCESSING PIPELINE DEMONSTRATION
============================================================

Step 1: Generating test signal...
  - Creating 10Hz sine wave with noise
  - Signal-to-Noise Ratio: -3.90 dB

Step 2: Applying low-pass filter...
  - Cutoff frequency: 0.1 (normalized)
  - Filter taps: 51
  - Processing time: 1.12 ms
  - SNR improvement: -3.90 dB → -3.15 dB
  - Noise reduction: 0.75 dB

Step 3: Performing FFT analysis...
  - Processing time: 3.81 ms
  - Detected peak frequency: 10.0 Hz
  - Expected frequency: 10.0 Hz
```

## Application Context

This library implements core DSP operations essential for tactical radio communications:

**Signal Processing Pipeline**

The library provides a complete signal processing pipeline implementing operations fundamental to software-defined radios. Low-pass filtering and FFT analysis form the foundation of radio signal processing, enabling noise reduction and spectrum analysis.

**Performance Considerations**

Tactical radio systems process millions of samples per second in real-time. C++ provides the low-level control and performance necessary for these high-throughput applications. FFTW (Fastest Fourier Transform in the West) represents the industry standard for DSP operations. The pybind11 bindings enable rapid prototyping in Python while maintaining performance-critical operations in C++.

## Technical Highlights

- **Filter Design**: Windowed-sinc FIR filter with Hamming window
- **FFT Library**: FFTW3 (industry standard, used in real SDRs)
- **Build System**: Modern CMake with proper dependency management
- **Testing**: Comprehensive test suite covering edge cases
- **Documentation**: Extensive comments explaining every algorithm

## Performance

While NumPy/SciPy are highly optimized, this project demonstrates:

- Understanding of underlying algorithms
- Ability to work in C++ when needed
- Proper benchmarking methodology
- Real-world software engineering practices

## Extension Possibilities

Potential enhancements to expand functionality:

- Additional filter types (IIR, band-pass, notch filters)
- Real-time audio processing implementation
- SIMD optimizations (AVX2/AVX-512)
- Interactive GUI development
- Additional modulation schemes (BPSK, QPSK)

## License

MIT License - See LICENSE file for details

## Author

Joseph Pak

## Project Purpose

This project demonstrates practical implementation of signal processing concepts relevant to tactical radio communications systems and software-defined radio architectures.
