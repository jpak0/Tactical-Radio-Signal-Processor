cmake_minimum_required(VERSION 3.15)
project(SignalProcessor CXX)

# C++17 for modern features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python and pybind11
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Find FFTW3 library
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED fftw3)

# Create the Python module
pybind11_add_module(signal_processor_cpp
    src/signal_processor.cpp
    src/bindings.cpp
)

# Link FFTW3
target_include_directories(signal_processor_cpp PRIVATE ${FFTW3_INCLUDE_DIRS})
target_link_libraries(signal_processor_cpp PRIVATE ${FFTW3_LIBRARIES})

# Compiler optimizations
target_compile_options(signal_processor_cpp PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-O3 -march=native -Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/O2 /W4>
)

# Set version
target_compile_definitions(signal_processor_cpp PRIVATE VERSION_INFO="1.0.0")
