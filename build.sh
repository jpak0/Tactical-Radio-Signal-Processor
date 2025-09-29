#!/bin/bash

# Build script for Signal Processor module
# Compiles C++ code and creates Python module

echo "==================================="
echo "Building Signal Processor Module"
echo "==================================="
echo ""

# Check dependencies
echo "Checking dependencies..."
command -v cmake >/dev/null 2>&1 || { echo "ERROR: cmake not found. Install with: brew install cmake"; exit 1; }
command -v python3 >/dev/null 2>&1 || { echo "ERROR: python3 not found"; exit 1; }

# Check for FFTW3
if ! pkg-config --exists fftw3; then
    echo "ERROR: FFTW3 not found."
    echo "Install with: brew install fftw"
    exit 1
fi

# Check for pybind11
python3 -c "import pybind11" 2>/dev/null || {
    echo "ERROR: pybind11 not found."
    echo "Install with: pip3 install pybind11"
    exit 1
}

echo "✓ All dependencies found"
echo ""

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake
echo "Configuring with CMake..."
cmake .. || { echo "ERROR: CMake configuration failed"; exit 1; }

# Build
echo "Building..."
make -j$(sysctl -n hw.ncpu) || { echo "ERROR: Build failed"; exit 1; }

# Copy module to parent directory
echo "Installing module..."
cp signal_processor_cpp*.so ../ || { echo "ERROR: Could not copy module"; exit 1; }

echo ""
echo "==================================="
echo "✓ Build successful!"
echo "==================================="
echo ""
echo "Module created: signal_processor_cpp.so"
echo ""
echo "Next steps:"
echo "  1. Run demo:      python3 demo.py"
echo "  2. Run benchmark: python3 benchmark.py"
echo "  3. Run tests:     python3 test_processor.py"
echo ""
