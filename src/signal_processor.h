#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include <vector>
#include <complex>

/**
 * Signal Processing Library for Tactical Radio Applications
 *
 * This header defines the interface for core DSP operations used in
 * software-defined radios (SDRs) for tactical communications systems.
 *
 * Key Operations:
 * 1. Low-pass filtering - Removes high-frequency noise
 * 2. FFT - Analyzes frequency content
 * 3. Signal generation - Creates test signals
 * 4. SNR calculation - Measures signal quality
 */

namespace signal_processor {

/**
 * Generate a test signal: sine wave + Gaussian noise
 *
 * Simulates radio signal reception conditions:
 * - Sine wave represents signal content (e.g., voice transmission)
 * - Gaussian noise represents interference and channel effects
 *
 * @param frequency Frequency of the sine wave (Hz)
 * @param sample_rate Sampling rate in samples per second (Hz)
 * @param duration Signal duration (seconds)
 * @param noise_amplitude Noise level (0 = clean signal, 1 = high noise)
 * @return Vector of signal samples
 *
 * Example: 10Hz sine wave sampled at 1000 Hz for 1 second
 *   generate_test_signal(10.0, 1000.0, 1.0, 0.5)
 *   Produces 1000 samples with moderate noise level
 */
std::vector<double> generate_test_signal(
    double frequency,
    double sample_rate,
    double duration,
    double noise_amplitude
);

/**
 * Apply a low-pass filter to remove high-frequency noise
 *
 * Implementation:
 * - Windowed-sinc FIR filter design
 * - Convolution with input signal
 * - Preserves low-frequency content while attenuating high frequencies
 *
 * Filter characteristics:
 * - Linear phase response (no signal distortion)
 * - Configurable cutoff frequency
 * - Adjustable filter order for sharpness/performance tradeoff
 *
 * @param input Input signal samples
 * @param cutoff_freq Normalized cutoff frequency (0-1)
 *                    Example: 0.1 = 10% of Nyquist frequency
 * @param num_taps Filter order (number of coefficients)
 *                 Typical values: 31, 51, 101 (odd numbers)
 * @return Filtered signal (same length as input)
 *
 * Application: Tactical radios may sample at 1 MHz and apply
 * a 20 kHz cutoff to isolate voice communication bandwidth
 */
std::vector<double> apply_lowpass_filter(
    const std::vector<double>& input,
    double cutoff_freq,
    int num_taps
);

/**
 * Compute Fast Fourier Transform (FFT)
 *
 * Converts signal from time-domain to frequency-domain representation,
 * revealing the spectral content of the input signal.
 *
 * Operation:
 * - Time-domain input (signal amplitude vs. time)
 * - Frequency-domain output (spectral power vs. frequency)
 * - O(N log N) complexity via FFT algorithm
 *
 * Radio applications:
 * - Spectrum scanning and signal detection
 * - Frequency-domain demodulation
 * - Interference analysis
 * - Channel sensing for frequency hopping
 *
 * @param input Real-valued signal samples
 * @return Complex frequency components (magnitude + phase)
 *
 * Implementation: Uses FFTW library (Fastest Fourier Transform in the West),
 * the industry standard for high-performance FFT computation in SDR systems.
 */
std::vector<std::complex<double>> compute_fft(
    const std::vector<double>& input
);

/**
 * Calculate Signal-to-Noise Ratio (SNR)
 *
 * Quantifies signal quality in decibels (dB):
 * - High SNR (>20 dB): Excellent signal quality, reliable decoding
 * - Medium SNR (10-20 dB): Good quality with some degradation
 * - Low SNR (<10 dB): Challenging decoding conditions
 * - Negative SNR: Noise power exceeds signal power
 *
 * Formula: SNR = 10 * log10(signal_power / noise_power)
 *
 * @param signal Reference signal (clean)
 * @param noisy Received signal (with noise)
 * @return SNR in decibels (dB)
 *
 * Tactical radio systems typically operate reliably down to -3 dB SNR,
 * where noise power is approximately twice the signal power.
 */
double calculate_snr(
    const std::vector<double>& signal,
    const std::vector<double>& noisy
);

/**
 * Find the frequency with maximum power in FFT output
 *
 * Identifies the dominant frequency component by locating
 * the spectral peak in the FFT magnitude spectrum.
 *
 * Applications:
 * - Signal frequency detection
 * - Interference source identification
 * - Channel tuning and acquisition
 * - Carrier frequency estimation
 *
 * @param fft_output Complex FFT result from compute_fft()
 * @param sample_rate Original sampling rate (Hz)
 * @return Detected peak frequency in Hz
 */
double find_peak_frequency(
    const std::vector<std::complex<double>>& fft_output,
    double sample_rate
);

} // namespace signal_processor

#endif // SIGNAL_PROCESSOR_H
