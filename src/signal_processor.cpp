#include "signal_processor.h"
#include <cmath>
#include <random>
#include <stdexcept>
#include <fftw3.h>
#include <algorithm>

namespace signal_processor {

// ============================================================================
// SIGNAL GENERATION
// ============================================================================

std::vector<double> generate_test_signal(
    double frequency,
    double sample_rate,
    double duration,
    double noise_amplitude
) {
    /**
     * Creates a realistic test signal for radio simulation
     *
     * Step-by-step:
     * 1. Calculate how many samples we need
     * 2. For each sample:
     *    a. Calculate time position
     *    b. Generate sine wave value: sin(2π * frequency * time)
     *    c. Add random Gaussian noise
     * 3. Return the complete signal
     *
     * Why this matters:
     * - Real radio signals are always noisy
     * - We need realistic test data to validate our filters
     * - This simulates what an SDR would actually receive
     */

    int num_samples = static_cast<int>(sample_rate * duration);
    std::vector<double> signal(num_samples);

    // Random number generator for Gaussian noise
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> noise_dist(0.0, noise_amplitude);

    // Generate each sample
    for (int i = 0; i < num_samples; ++i) {
        double t = i / sample_rate;  // Current time in seconds

        // Pure sine wave (this is our "data")
        double sine_value = std::sin(2.0 * M_PI * frequency * t);

        // Add noise (this is interference we want to remove)
        double noise = noise_dist(gen);

        signal[i] = sine_value + noise;
    }

    return signal;
}

// ============================================================================
// LOW-PASS FILTER (Removes High Frequencies)
// ============================================================================

std::vector<double> apply_lowpass_filter(
    const std::vector<double>& input,
    double cutoff_freq,
    int num_taps
) {
    /**
     * FIR (Finite Impulse Response) Low-Pass Filter
     * Uses the "windowed-sinc" method - industry standard
     *
     * How filtering works:
     * 1. Design filter coefficients (the "weights")
     * 2. Slide filter across signal (convolution)
     * 3. Each output is weighted average of nearby inputs
     *
     * Example with 3-tap filter [0.25, 0.5, 0.25]:
     *   Input:  [5, 10, 6, 12, 7, ...]
     *   Output: [-, 7.5, 9, 9.25, ...] (reduced variation)
     *
     * Tactical radio use case:
     * - Voice signal: 0-4 kHz (low frequency)
     * - Noise: 10+ kHz (high frequency)
     * - Filter keeps voice, removes noise
     */

    // Step 1: Design the filter coefficients using windowed-sinc method
    std::vector<double> filter_coeffs(num_taps);

    // Calculate the center of the filter
    int center = num_taps / 2;

    for (int i = 0; i < num_taps; ++i) {
        // Distance from center
        int offset = i - center;

        if (offset == 0) {
            // Special case: at center, sinc function = 1
            filter_coeffs[i] = 2.0 * cutoff_freq;
        } else {
            // Sinc function: sin(x)/x
            // This creates the ideal low-pass filter shape
            double sinc_value = std::sin(2.0 * M_PI * cutoff_freq * offset) /
                               (M_PI * offset);

            // Hamming window: tapers the edges to reduce ringing
            // Formula: 0.54 - 0.46 * cos(2πn/N)
            double window = 0.54 - 0.46 * std::cos(2.0 * M_PI * i / (num_taps - 1));

            filter_coeffs[i] = sinc_value * window;
        }
    }

    // Normalize the filter (make sure sum = 1)
    // This preserves signal amplitude
    double sum = 0.0;
    for (double coeff : filter_coeffs) {
        sum += coeff;
    }
    for (double& coeff : filter_coeffs) {
        coeff /= sum;
    }

    // Step 2: Apply filter via convolution
    // Convolution = sliding weighted average
    int input_size = input.size();
    std::vector<double> output(input_size, 0.0);

    for (int i = 0; i < input_size; ++i) {
        double sum = 0.0;

        // Multiply and accumulate (MAC) operation
        // This is the core of digital filtering
        for (int j = 0; j < num_taps; ++j) {
            int input_idx = i - center + j;

            // Handle edges by zero-padding
            if (input_idx >= 0 && input_idx < input_size) {
                sum += input[input_idx] * filter_coeffs[j];
            }
        }

        output[i] = sum;
    }

    return output;
}

// ============================================================================
// FFT (Fast Fourier Transform)
// ============================================================================

std::vector<std::complex<double>> compute_fft(
    const std::vector<double>& input
) {
    /**
     * Converts signal from time-domain to frequency-domain
     * Uses FFTW library (Fastest Fourier Transform in the West)
     *
     * Conceptual understanding:
     * - ANY signal can be represented as sum of sine waves
     * - FFT finds which sine waves (frequencies) are present
     * - Each output bin represents a specific frequency
     *
     * Example:
     *   Input: Complex audio waveform
     *   Output: Shows peaks at 440Hz (A note), 880Hz (octave above)
     *
     * SDR use case:
     * - Scan 2.4 GHz Wi-Fi band
     * - FFT shows which channels are occupied
     * - Tactical radio picks empty channel
     */

    int N = input.size();

    // Allocate aligned memory for FFTW (faster performance)
    double* in = fftw_alloc_real(N);
    fftw_complex* out = fftw_alloc_complex(N);

    // Copy input data
    for (int i = 0; i < N; ++i) {
        in[i] = input[i];
    }

    // Create FFTW plan (this analyzes best algorithm for this size)
    // FFTW_ESTIMATE is fast planning, good for one-time use
    // Production code might use FFTW_MEASURE for repeated FFTs
    fftw_plan plan = fftw_plan_dft_r2c_1d(
        N,
        in,
        out,
        FFTW_ESTIMATE
    );

    // Execute the FFT (this is the actual transform)
    fftw_execute(plan);

    // Convert FFTW complex format to C++ complex
    // FFTW uses array[0] = real, array[1] = imag
    std::vector<std::complex<double>> result(N / 2 + 1);
    for (int i = 0; i < N / 2 + 1; ++i) {
        result[i] = std::complex<double>(out[i][0], out[i][1]);
    }

    // Clean up FFTW resources
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return result;
}

// ============================================================================
// SNR (Signal-to-Noise Ratio) CALCULATION
// ============================================================================

double calculate_snr(
    const std::vector<double>& signal,
    const std::vector<double>& noisy
) {
    /**
     * Measures signal quality in decibels (dB)
     *
     * How it works:
     * 1. Calculate "noise" = noisy signal - clean signal
     * 2. Compute power of signal: sum of squares
     * 3. Compute power of noise: sum of squares
     * 4. SNR = 10 * log10(signal_power / noise_power)
     *
     * Interpretation:
     * - SNR = 20 dB: Signal is 100x more powerful than noise (excellent)
     * - SNR = 10 dB: Signal is 10x more powerful (good)
     * - SNR = 0 dB: Signal and noise equal power (poor)
     * - SNR = -3 dB: Noise is 2x more powerful (challenging)
     *
     * Tactical radio specifications typically require operation at -3 dB or lower.
     */

    if (signal.size() != noisy.size()) {
        throw std::invalid_argument("Signal and noisy vectors must have same size");
    }

    int N = signal.size();

    // Calculate signal power: P_signal = Σ(signal[i]²)
    double signal_power = 0.0;
    for (int i = 0; i < N; ++i) {
        signal_power += signal[i] * signal[i];
    }

    // Calculate noise power: P_noise = Σ((noisy[i] - signal[i])²)
    double noise_power = 0.0;
    for (int i = 0; i < N; ++i) {
        double noise = noisy[i] - signal[i];
        noise_power += noise * noise;
    }

    // Avoid division by zero
    if (noise_power == 0.0) {
        return 100.0;  // Perfect signal (infinite SNR, capped at 100 dB)
    }

    // Convert to decibels: 10 * log10(ratio)
    double snr_db = 10.0 * std::log10(signal_power / noise_power);

    return snr_db;
}

// ============================================================================
// PEAK FREQUENCY DETECTION
// ============================================================================

double find_peak_frequency(
    const std::vector<std::complex<double>>& fft_output,
    double sample_rate
) {
    /**
     * Finds the dominant frequency in FFT output
     *
     * Process:
     * 1. Calculate magnitude of each FFT bin: sqrt(real² + imag²)
     * 2. Find bin with maximum magnitude
     * 3. Convert bin number to frequency: freq = bin * (sample_rate / N)
     *
     * Example:
     *   Sample rate = 1000 Hz, 1000 samples
     *   FFT bins represent: 0 Hz, 1 Hz, 2 Hz, ..., 500 Hz
     *   If bin 10 has max magnitude → frequency is 10 Hz
     *
     * SDR application:
     * - Identifies signal location in spectrum
     * - Enables automatic frequency tuning
     * - Essential for frequency-hopping radio systems
     */

    if (fft_output.empty()) {
        return 0.0;
    }

    // Find the bin with maximum magnitude
    int max_bin = 0;
    double max_magnitude = 0.0;

    for (size_t i = 0; i < fft_output.size(); ++i) {
        // Magnitude = sqrt(real² + imaginary²)
        double magnitude = std::abs(fft_output[i]);

        if (magnitude > max_magnitude) {
            max_magnitude = magnitude;
            max_bin = i;
        }
    }

    // Convert bin number to actual frequency
    // Each bin represents (sample_rate / total_bins) Hz
    int total_bins = (fft_output.size() - 1) * 2;  // Account for real FFT
    double frequency = max_bin * sample_rate / total_bins;

    return frequency;
}

} // namespace signal_processor
