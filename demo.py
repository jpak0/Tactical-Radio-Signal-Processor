#!/usr/bin/env python3
"""
Signal Processing Pipeline Demonstration

This demo shows the complete signal processing workflow:
1. Generate a noisy test signal (simulates radio reception)
2. Apply low-pass filter to remove noise
3. Perform FFT to analyze frequencies
4. Visualize the results

These operations form the core of tactical radio signal processing.
"""

import sys

try:
    import signal_processor_cpp as sp
except ImportError as e:
    print("\nERROR: Could not import signal_processor_cpp module")
    print("This module must be built first.")
    print("\nTo build:")
    print("  chmod +x build.sh")
    print("  ./build.sh")
    print(f"\nDetails: {e}\n")
    sys.exit(1)

try:
    import numpy as np
    import matplotlib.pyplot as plt
except ImportError as e:
    print("\nERROR: Required Python packages not installed")
    print("\nTo install:")
    print("  pip3 install numpy matplotlib")
    print(f"\nDetails: {e}\n")
    sys.exit(1)

import time

def print_header(title):
    """Print a nicely formatted section header"""
    print("\n" + "=" * 60)
    print(title)
    print("=" * 60 + "\n")

def print_step(step_num, description):
    """Print a step description"""
    print(f"Step {step_num}: {description}")

def main():
    print_header("SIGNAL PROCESSING PIPELINE DEMONSTRATION")

    # ===========================================================================
    # STEP 1: Generate Test Signal
    # ===========================================================================
    print_step(1, "Generating test signal...")

    # Parameters for signal generation
    frequency = 10.0        # Hz - Our "data" frequency
    sample_rate = 1000.0    # Hz - How many samples per second
    duration = 1.0          # seconds
    noise_amplitude = 0.5   # How much noise to add

    # Generate clean reference signal (no noise)
    clean_signal = sp.generate_test_signal(frequency, sample_rate, duration, 0.0)

    # Generate noisy signal (what the radio actually receives)
    noisy_signal = sp.generate_test_signal(frequency, sample_rate, duration, noise_amplitude)

    # Calculate initial SNR
    initial_snr = sp.calculate_snr(clean_signal, noisy_signal)

    print(f"  - Creating {frequency}Hz sine wave with noise")
    print(f"  - Signal-to-Noise Ratio: {initial_snr:.2f} dB")

    # ===========================================================================
    # STEP 2: Apply Low-Pass Filter
    # ===========================================================================
    print_step(2, "Applying low-pass filter...")

    cutoff_freq = 0.1  # Keep only lowest 10% of spectrum
    num_taps = 51      # Filter coefficients (more = sharper cutoff)

    start_time = time.time()
    filtered_signal = sp.apply_lowpass_filter(noisy_signal, cutoff_freq, num_taps)
    filter_time = (time.time() - start_time) * 1000  # Convert to ms

    # Calculate SNR after filtering
    filtered_snr = sp.calculate_snr(clean_signal, filtered_signal)
    snr_improvement = filtered_snr - initial_snr

    print(f"  - Cutoff frequency: {cutoff_freq} (normalized)")
    print(f"  - Filter taps: {num_taps}")
    print(f"  - Processing time: {filter_time:.2f} ms")
    print(f"  - SNR improvement: {initial_snr:.2f} dB → {filtered_snr:.2f} dB")
    print(f"  - Noise reduction: {snr_improvement:.2f} dB")

    # ===========================================================================
    # STEP 3: Perform FFT Analysis
    # ===========================================================================
    print_step(3, "Performing FFT analysis...")

    start_time = time.time()
    fft_result = sp.compute_fft(filtered_signal)
    fft_time = (time.time() - start_time) * 1000

    detected_freq = sp.find_peak_frequency(fft_result, sample_rate)

    print(f"  - Processing time: {fft_time:.2f} ms")
    print(f"  - Detected peak frequency: {detected_freq:.1f} Hz")
    print(f"  - Expected frequency: {frequency:.1f} Hz")

    # ===========================================================================
    # STEP 4: Create Visualizations
    # ===========================================================================
    print_step(4, "Generating plots...")

    # Create time axis for plotting
    time_axis = np.arange(len(clean_signal)) / sample_rate

    # Create frequency axis for FFT plot
    freq_axis = np.fft.rfftfreq(len(clean_signal), 1/sample_rate)
    fft_magnitude = np.abs(fft_result)

    # Create figure with subplots
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    fig.suptitle('Tactical Radio Signal Processing Pipeline', fontsize=16, fontweight='bold')

    # Plot 1: Original noisy signal
    axes[0, 0].plot(time_axis[:200], noisy_signal[:200], 'b-', alpha=0.7, label='Noisy')
    axes[0, 0].plot(time_axis[:200], clean_signal[:200], 'g--', alpha=0.7, label='Clean')
    axes[0, 0].set_xlabel('Time (s)')
    axes[0, 0].set_ylabel('Amplitude')
    axes[0, 0].set_title(f'Raw Signal (SNR: {initial_snr:.1f} dB)')
    axes[0, 0].legend()
    axes[0, 0].grid(True, alpha=0.3)

    # Plot 2: Filtered signal
    axes[0, 1].plot(time_axis[:200], filtered_signal[:200], 'r-', alpha=0.7, label='Filtered')
    axes[0, 1].plot(time_axis[:200], clean_signal[:200], 'g--', alpha=0.7, label='Reference')
    axes[0, 1].set_xlabel('Time (s)')
    axes[0, 1].set_ylabel('Amplitude')
    axes[0, 1].set_title(f'After Low-Pass Filter (SNR: {filtered_snr:.1f} dB, +{snr_improvement:.1f} dB)')
    axes[0, 1].legend()
    axes[0, 1].grid(True, alpha=0.3)

    # Plot 3: FFT spectrum
    axes[1, 0].plot(freq_axis, fft_magnitude, 'b-', linewidth=2)
    axes[1, 0].axvline(detected_freq, color='r', linestyle='--', linewidth=2, label=f'Peak: {detected_freq:.1f} Hz')
    axes[1, 0].set_xlabel('Frequency (Hz)')
    axes[1, 0].set_ylabel('Magnitude')
    axes[1, 0].set_title('FFT: Frequency Domain Analysis')
    axes[1, 0].set_xlim(0, 50)  # Focus on low frequencies
    axes[1, 0].legend()
    axes[1, 0].grid(True, alpha=0.3)

    # Plot 4: Performance metrics
    axes[1, 1].axis('off')
    metrics_text = f"""
    PERFORMANCE METRICS
    ══════════════════════════════════════════

    Signal Parameters:
    • Frequency: {frequency} Hz
    • Sample Rate: {sample_rate} Hz
    • Duration: {duration} s
    • Samples: {len(clean_signal)}

    Processing Times:
    • Filter: {filter_time:.2f} ms
    • FFT: {fft_time:.2f} ms
    • Total: {filter_time + fft_time:.2f} ms

    Quality Metrics:
    • Initial SNR: {initial_snr:.2f} dB
    • Filtered SNR: {filtered_snr:.2f} dB
    • Improvement: {snr_improvement:.2f} dB

    Frequency Detection:
    • Expected: {frequency:.1f} Hz
    • Detected: {detected_freq:.1f} Hz
    • Error: {abs(detected_freq - frequency):.1f} Hz

    Filter Configuration:
    • Type: Low-pass FIR (Windowed-Sinc)
    • Cutoff: {cutoff_freq} (normalized)
    • Taps: {num_taps}

    Real-Time Capability:
    • Processing rate: {len(clean_signal) / ((filter_time + fft_time) / 1000):.0f} samples/sec
    • Throughput: {(len(clean_signal) / ((filter_time + fft_time) / 1000)) / sample_rate:.1f}x real-time
    """
    axes[1, 1].text(0.1, 0.5, metrics_text, fontfamily='monospace', fontsize=10, verticalalignment='center')

    plt.tight_layout()

    # Save the plot
    output_file = 'signal_processing_results.png'
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    print(f"  - Plot saved to: {output_file}")

    # Show the plot
    plt.show()

    # ===========================================================================
    # Summary
    # ===========================================================================
    print_header("PROCESSING COMPLETE")

    print("Pipeline summary:")
    print("  1. Generated 10 Hz sine wave with additive noise")
    print("  2. Applied low-pass filter to remove high-frequency noise")
    print(f"  3. Improved signal quality by {snr_improvement:.2f} dB")
    print("  4. Used FFT to detect signal frequency")
    print("  5. Generated visualization of results")
    print("")

if __name__ == "__main__":
    try:
        main()
    except ImportError as e:
        print("\nERROR: Could not import signal_processor_cpp module")
        print("Make sure to build the module first:")
        print("  chmod +x build.sh")
        print("  ./build.sh")
        print("")
        print(f"Details: {e}")
    except Exception as e:
        print(f"\nERROR: {e}")
        import traceback
        traceback.print_exc()
