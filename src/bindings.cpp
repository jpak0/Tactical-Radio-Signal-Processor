#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include "signal_processor.h"

namespace py = pybind11;

/**
 * Python Bindings for Signal Processor
 *
 * This file creates the bridge between C++ and Python using pybind11
 *
 * What pybind11 does:
 * 1. Automatically converts Python lists ↔ C++ vectors
 * 2. Handles complex numbers between NumPy and C++
 * 3. Creates proper Python module that can be imported
 * 4. Generates function signatures and docstrings
 *
 * Key features:
 * - Performance-critical code in C++
 * - Natural Python interface
 * - Zero-copy array passing (high performance)
 * - Automatic type conversion and error handling
 *
 * After compilation, you can do in Python:
 *   import signal_processor_cpp
 *   signal = signal_processor_cpp.generate_test_signal(10.0, 1000.0, 1.0, 0.5)
 */

PYBIND11_MODULE(signal_processor_cpp, m) {
    // Module docstring - shows up in Python help()
    m.doc() = R"pbdoc(
        Signal Processing C++ Module

        High-performance signal processing operations for tactical radio applications.

        This module provides:
        - Low-pass filtering using windowed-sinc FIR design
        - FFT analysis using FFTW library
        - Signal generation and quality metrics

        Typical usage:
            import signal_processor_cpp as sp

            # Generate test signal
            signal = sp.generate_test_signal(10.0, 1000.0, 1.0, 0.5)

            # Apply filter
            filtered = sp.apply_lowpass_filter(signal, 0.1, 51)

            # Analyze frequencies
            fft_result = sp.compute_fft(signal)
            peak_freq = sp.find_peak_frequency(fft_result, 1000.0)
    )pbdoc";

    // Bind generate_test_signal function
    m.def("generate_test_signal",
          &signal_processor::generate_test_signal,
          py::arg("frequency"),
          py::arg("sample_rate"),
          py::arg("duration"),
          py::arg("noise_amplitude"),
          R"pbdoc(
              Generate a test signal (sine wave + Gaussian noise)

              Args:
                  frequency (float): Frequency of sine wave in Hz
                  sample_rate (float): Sampling rate in Hz
                  duration (float): Duration in seconds
                  noise_amplitude (float): Standard deviation of noise

              Returns:
                  list[float]: Signal samples

              Example:
                  >>> signal = generate_test_signal(10.0, 1000.0, 1.0, 0.5)
                  >>> len(signal)
                  1000
          )pbdoc");

    // Bind apply_lowpass_filter function
    m.def("apply_lowpass_filter",
          &signal_processor::apply_lowpass_filter,
          py::arg("input"),
          py::arg("cutoff_freq"),
          py::arg("num_taps"),
          R"pbdoc(
              Apply a low-pass FIR filter to remove high frequencies

              Uses windowed-sinc method with Hamming window.

              Args:
                  input (list[float]): Input signal
                  cutoff_freq (float): Normalized cutoff frequency (0-1)
                                      0.1 = keep lowest 10% of spectrum
                  num_taps (int): Number of filter coefficients (31, 51, 101 typical)
                                 More taps = sharper cutoff, more computation

              Returns:
                  list[float]: Filtered signal

              Example:
                  >>> noisy = [1, 5, 2, 6, 3, 7, 4, 8]
                  >>> smooth = apply_lowpass_filter(noisy, 0.3, 5)
                  # smooth will be less jagged
          )pbdoc");

    // Bind compute_fft function
    m.def("compute_fft",
          &signal_processor::compute_fft,
          py::arg("input"),
          R"pbdoc(
              Compute Fast Fourier Transform using FFTW

              Converts time-domain signal to frequency-domain representation.

              Args:
                  input (list[float]): Real-valued signal samples

              Returns:
                  list[complex]: Complex frequency components
                                Length is (N/2 + 1) where N = len(input)

              Example:
                  >>> signal = [math.sin(2*math.pi*10*t/1000) for t in range(1000)]
                  >>> fft = compute_fft(signal)
                  >>> # Peak will be at bin 10 (10 Hz)
          )pbdoc");

    // Bind calculate_snr function
    m.def("calculate_snr",
          &signal_processor::calculate_snr,
          py::arg("signal"),
          py::arg("noisy"),
          R"pbdoc(
              Calculate Signal-to-Noise Ratio in decibels

              Measures quality of signal reception.

              Args:
                  signal (list[float]): Clean reference signal
                  noisy (list[float]): Signal with noise added

              Returns:
                  float: SNR in dB
                        >20 dB: Excellent
                        10-20 dB: Good
                        0-10 dB: Poor
                        <0 dB: Noise dominates

              Example:
                  >>> clean = [1, 2, 3, 4, 5]
                  >>> noisy = [1.1, 2.2, 2.9, 4.1, 5.0]
                  >>> snr = calculate_snr(clean, noisy)
                  >>> print(f"SNR: {snr:.1f} dB")
          )pbdoc");

    // Bind find_peak_frequency function
    m.def("find_peak_frequency",
          &signal_processor::find_peak_frequency,
          py::arg("fft_output"),
          py::arg("sample_rate"),
          R"pbdoc(
              Find the frequency with maximum power in FFT output

              Detects dominant frequency in signal.

              Args:
                  fft_output (list[complex]): Output from compute_fft()
                  sample_rate (float): Original sampling rate in Hz

              Returns:
                  float: Detected frequency in Hz

              Example:
                  >>> signal = generate_test_signal(10.0, 1000.0, 1.0, 0.1)
                  >>> fft = compute_fft(signal)
                  >>> freq = find_peak_frequency(fft, 1000.0)
                  >>> print(f"Detected: {freq} Hz")  # Should be ~10.0
          )pbdoc");

    // Version information
    #ifdef VERSION_INFO
        m.attr("__version__") = VERSION_INFO;
    #else
        m.attr("__version__") = "dev";
    #endif
}
