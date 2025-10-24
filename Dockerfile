# Tactical Radio Signal Processing - Docker Image
# Multi-stage build for optimized final image

# Build stage
FROM python:3.11-slim as builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libfftw3-dev \
    pkg-config \
    pybind11-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Install pybind11 via pip for the build script check
RUN pip install --no-cache-dir pybind11

# Set working directory
WORKDIR /app

# Copy source files
COPY CMakeLists.txt build.sh ./
COPY src/ ./src/

# Build the C++ extension
RUN chmod +x build.sh && ./build.sh

# Runtime stage
FROM python:3.11-slim

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libfftw3-double3 \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy built extension from builder
COPY --from=builder /app/build/*.so ./

# Copy Python scripts and documentation
COPY demo.py benchmark.py test_processor.py ./
COPY README.md GETTING_STARTED.txt ./
COPY docs/ ./docs/

# Install Python dependencies
RUN pip install --no-cache-dir \
    numpy \
    scipy \
    matplotlib \
    pytest

# Set Python path to find the compiled module
ENV PYTHONPATH=/app

# Default command: run tests
CMD ["python3", "test_processor.py"]
