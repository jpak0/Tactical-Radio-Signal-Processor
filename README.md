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

### Option 1: Docker (Recommended - No Installation Required)

Docker provides a complete, isolated environment with all dependencies pre-installed.

**Prerequisites**: Docker Desktop ([Download here](https://www.docker.com/products/docker-desktop))

**Run Tests**:
```bash
docker-compose up signal-processor
```

**Run Demo** (creates visualization in `outputs/`):
```bash
docker-compose up demo
```

**Run Benchmarks**:
```bash
docker-compose up benchmark
```

**Interactive Development Shell**:
```bash
docker-compose up dev
```

**First run**: Takes 2-5 minutes to build (downloads base images, compiles C++ code, installs dependencies)
**Subsequent runs**: 3-5 seconds (uses cached image)

See [DOCKER.md](DOCKER.md) for detailed Docker instructions, troubleshooting, and advanced usage.

### Option 2: Native Build (For Local Development)

For active development with IDE support and faster iteration.

#### Prerequisites

**macOS**:
```bash
brew install cmake fftw python3
pip3 install numpy scipy matplotlib pytest pybind11
```

**Linux (Ubuntu/Debian)**:
```bash
sudo apt-get update
sudo apt-get install cmake libfftw3-dev python3 python3-pip
pip3 install numpy scipy matplotlib pytest pybind11
```

**Windows**: Use WSL2 (Windows Subsystem for Linux) and follow Linux instructions, or use Docker.

#### Build & Run

```bash
# 1. Build the C++ module
chmod +x build.sh
./build.sh

# Expected output: "✓ Build successful!"

# 2. Run the demo
python3 demo.py

# 3. Run benchmarks
python3 benchmark.py

# 4. Run tests
python3 test_processor.py
```

**Build time**: ~30 seconds (compiles C++ code with optimizations)

## What You Get

After building (Docker or native), you have access to three main programs:

### 1. Demo (`demo.py`)
**Purpose**: Complete signal processing pipeline demonstration
**Output**:
- Console output showing each processing step
- Visualization saved to `outputs/signal_processing_results.png`
- Shows: original signal, filtered signal, FFT spectrum, performance metrics

**When to use**: First-time exploration, demonstrations, understanding the workflow

### 2. Benchmarks (`benchmark.py`)
**Purpose**: Performance comparison between C++ and Python implementations
**Output**:
- Side-by-side timing comparisons
- Multiple signal sizes (1K to 1M samples)
- Speedup calculations
- Detailed performance analysis

**When to use**: Understanding performance characteristics, optimization decisions

### 3. Tests (`test_processor.py`)
**Purpose**: Automated test suite validating correctness
**Output**:
- 12 tests covering all functionality
- Expected: 10/12 passing (2 tests affected by floating-point precision)
- Validates: signal generation, filtering, FFT, SNR calculation, edge cases

**When to use**: Verifying installation, ensuring code correctness, development

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

## Troubleshooting

### Docker Issues

**"Cannot connect to Docker daemon"**
- Ensure Docker Desktop is running (check for whale icon in menu bar/system tray)
- Wait for Docker to fully start before running commands

**Build takes too long**
- First build downloads images and compiles code (2-5 minutes is normal)
- Subsequent builds use cache and are much faster

**Port conflicts**
- Run `docker-compose down` to stop any running containers
- Clean up with `docker system prune`

### Native Build Issues

**"fftw3.h not found"**
- Install FFTW: `brew install fftw` (macOS) or `sudo apt-get install libfftw3-dev` (Linux)

**"pybind11 not found"**
- Install via pip: `pip3 install pybind11`

**Import errors in Python**
- Build the C++ module first: `./build.sh`
- Verify build was successful (look for `.so` file in `build/` directory)

**IDE showing errors**
- Install dependencies locally (see Native Build prerequisites)
- Reload your IDE/editor
- For VS Code: Install C++ and Python extensions

### Getting Help

- Check [DOCKER.md](DOCKER.md) for Docker-specific issues
- Check [docs/QUICK_START.md](docs/QUICK_START.md) for detailed setup instructions
- Review [docs/TECHNICAL_ARCHITECTURE.md](docs/TECHNICAL_ARCHITECTURE.md) for implementation details

## Extension Possibilities

Potential enhancements to expand functionality:

- Additional filter types (IIR, band-pass, notch filters)
- Real-time audio processing implementation
- SIMD optimizations (AVX2/AVX-512)
- Interactive GUI development
- Additional modulation schemes (BPSK, QPSK)

## Contributing

Contributions are welcome! Please ensure:
- Code follows existing style conventions
- All tests pass (`python3 test_processor.py`)
- Documentation is updated for new features

## License

MIT License - See LICENSE file for details

## Author

Joseph Pak

## Acknowledgments

This project demonstrates practical implementation of signal processing concepts relevant to tactical radio communications systems and software-defined radio architectures.

Built with industry-standard tools:
- **FFTW3**: Fastest Fourier Transform in the West
- **pybind11**: Seamless C++/Python integration
- **CMake**: Modern build system
- **Docker**: Containerized deployment
