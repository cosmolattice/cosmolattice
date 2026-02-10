/**
 * Standalone test for parafaft FFT integration
 *
 * This test isolates the parafaft FFT backend from CosmoLattice abstractions
 * to identify issues with memory layout, data handling, or roundtrip accuracy.
 *
 * Compile with:
 *   mpicxx -O2 -I/path/to/parafaft -I/path/to/fftw3 parafaft_standalone_test.cpp -lfftw3 -lm -o test_parafaft
 *
 * Run with:
 *   mpirun -n 4 ./test_parafaft
 */

#include <mpi.h>
#include <parafaft_r2c.hpp>
#include <fftw3.h>
#include <vector>
#include <complex>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <numeric>

// Test parameters
constexpr int N0 = 16; // Grid size in dimension 0
constexpr int N1 = 16; // Grid size in dimension 1
constexpr int N2 = 16; // Grid size in dimension 2
constexpr double TOLERANCE = 1e-10;

#ifdef CL_CUDA
using ParaFaFT_Backend = parafaft::CuFFTBackend;
#elif defined(CL_HIP)
using ParaFaFT_Backend = parafaft::HipFFTBackend;
#else
using ParaFaFT_Backend = parafaft::FFTWBackend;
#endif

// Helper to check if two doubles are approximately equal
bool approx_equal(double a, double b, double tol = TOLERANCE)
{
  return std::abs(a - b) < tol * (1.0 + std::abs(a) + std::abs(b));
}

// Test 1: Basic roundtrip test (forward then backward should give original * N)
bool test_roundtrip(MPI_Comm comm)
{
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  if (rank == 0) {
    std::cout << "\n=== Test 1: Roundtrip (forward + backward) ===" << std::endl;
  }

  int global_shape[3] = {N0, N1, N2};
  parafaft::ParaFaFT_R2C<3, ParaFaFT_Backend> fft(global_shape, comm);

  // Get local sizes
  int local_real_shape[3], real_start[3];
  int local_complex_shape[3], complex_start[3];
  fft.get_local_real_shape(local_real_shape);
  fft.get_real_global_start(real_start);
  fft.get_local_complex_shape(local_complex_shape);
  fft.get_complex_global_start(complex_start);

  int local_real_size = fft.get_local_real_size();
  int local_complex_size = fft.get_local_complex_size();

  if (rank == 0) {
    std::cout << "Global shape: [" << N0 << ", " << N1 << ", " << N2 << "]" << std::endl;
    std::cout << "Number of MPI ranks: " << size << std::endl;
  }

  MPI_Barrier(comm);
  std::cout << "Rank " << rank << ": real shape = [" << local_real_shape[0] << ", " << local_real_shape[1] << ", "
            << local_real_shape[2] << "]"
            << ", real start = [" << real_start[0] << ", " << real_start[1] << ", " << real_start[2] << "]"
            << ", complex shape = [" << local_complex_shape[0] << ", " << local_complex_shape[1] << ", "
            << local_complex_shape[2] << "]"
            << ", complex start = [" << complex_start[0] << ", " << complex_start[1] << ", " << complex_start[2] << "]"
            << std::endl;
  MPI_Barrier(comm);

  // Allocate buffers
  std::vector<double> real_input(local_real_size);
  std::vector<double> real_output(local_real_size);
  std::vector<std::complex<double>> complex_buffer(local_complex_size);

  // Initialize with a simple pattern: f(x,y,z) = sin(2*pi*x/N0) * cos(2*pi*y/N1) + z
  for (int i = 0; i < local_real_shape[0]; ++i) {
    for (int j = 0; j < local_real_shape[1]; ++j) {
      for (int k = 0; k < local_real_shape[2]; ++k) {
        int idx = i * local_real_shape[1] * local_real_shape[2] + j * local_real_shape[2] + k;
        int gi = real_start[0] + i;
        int gj = real_start[1] + j;
        int gk = real_start[2] + k;

        double x = (double)gi / N0;
        double y = (double)gj / N1;
        double z = (double)gk / N2;

        real_input[idx] = std::sin(2.0 * M_PI * x) * std::cos(2.0 * M_PI * y) + z;
      }
    }
  }

  // Copy input for later comparison
  std::vector<double> original_input = real_input;

  // Forward FFT
  fft.forward(real_input.data(), complex_buffer.data());

  // Backward FFT
  fft.backward(complex_buffer.data(), real_output.data());

  // Check roundtrip: output should equal input * N (unnormalized FFT)
  double total_N = (double)(N0 * N1 * N2);
  double max_error = 0.0;
  int error_count = 0;

  for (int i = 0; i < local_real_size; ++i) {
    double expected = original_input[i] * total_N;
    double error = std::abs(real_output[i] - expected);
    max_error = std::max(max_error, error);

    if (error > TOLERANCE * total_N) {
      if (error_count < 5) { // Only print first 5 errors
        std::cout << "Rank " << rank << ": error at index " << i << ": got " << real_output[i] << ", expected "
                  << expected << ", error = " << error << std::endl;
      }
      error_count++;
    }
  }

  // Gather max error across all ranks
  double global_max_error;
  MPI_Reduce(&max_error, &global_max_error, 1, MPI_DOUBLE, MPI_MAX, 0, comm);

  int global_error_count;
  MPI_Reduce(&error_count, &global_error_count, 1, MPI_INT, MPI_SUM, 0, comm);

  if (rank == 0) {
    std::cout << "Roundtrip max error: " << global_max_error << std::endl;
    std::cout << "Total error count: " << global_error_count << std::endl;

    if (global_max_error < TOLERANCE * total_N) {
      std::cout << "PASSED: Roundtrip test" << std::endl;
      return true;
    } else {
      std::cout << "FAILED: Roundtrip test" << std::endl;
      return false;
    }
  }

  return true; // Non-root ranks just return true
}

// Test 2: Known Fourier transform - constant function should give delta at k=0
bool test_constant_function(MPI_Comm comm)
{
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  if (rank == 0) {
    std::cout << "\n=== Test 2: Constant function FFT ===" << std::endl;
  }

  int global_shape[3] = {N0, N1, N2};
  parafaft::ParaFaFT_R2C<3, ParaFaFT_Backend> fft(global_shape, comm);

  int local_real_shape[3], real_start[3];
  int local_complex_shape[3], complex_start[3];
  fft.get_local_real_shape(local_real_shape);
  fft.get_real_global_start(real_start);
  fft.get_local_complex_shape(local_complex_shape);
  fft.get_complex_global_start(complex_start);

  int local_real_size = fft.get_local_real_size();
  int local_complex_size = fft.get_local_complex_size();

  // Initialize with constant = 1.0
  std::vector<double> real_input(local_real_size, 1.0);
  std::vector<std::complex<double>> complex_output(local_complex_size);

  // Forward FFT
  fft.forward(real_input.data(), complex_output.data());

  // Check: FFT of constant 1.0 should give N*delta(k)
  // Only the k=(0,0,0) mode should be non-zero, with value = N0*N1*N2
  double total_N = (double)(N0 * N1 * N2);
  bool passed = true;

  for (int i = 0; i < local_complex_shape[0]; ++i) {
    for (int j = 0; j < local_complex_shape[1]; ++j) {
      for (int k = 0; k < local_complex_shape[2]; ++k) {
        int idx = i * local_complex_shape[1] * local_complex_shape[2] + j * local_complex_shape[2] + k;
        int gi = complex_start[0] + i;
        int gj = complex_start[1] + j;
        int gk = complex_start[2] + k;

        std::complex<double> expected(0.0, 0.0);
        if (gi == 0 && gj == 0 && gk == 0) {
          expected = std::complex<double>(total_N, 0.0);
        }

        double error = std::abs(complex_output[idx] - expected);

        if (error > TOLERANCE * total_N) {
          if (rank == 0 || (gi == 0 && gj == 0 && gk == 0)) {
            std::cout << "Rank " << rank << ": k=(" << gi << "," << gj << "," << gk << "): got " << complex_output[idx]
                      << ", expected " << expected << ", error = " << error << std::endl;
          }
          passed = false;
        }
      }
    }
  }

  // Gather results
  int local_passed = passed ? 1 : 0;
  int global_passed;
  MPI_Reduce(&local_passed, &global_passed, 1, MPI_INT, MPI_MIN, 0, comm);

  if (rank == 0) {
    if (global_passed) {
      std::cout << "PASSED: Constant function FFT test" << std::endl;
      return true;
    } else {
      std::cout << "FAILED: Constant function FFT test" << std::endl;
      return false;
    }
  }

  return true;
}

// Test 3: Single sine wave - should give delta at k=(1,0,0) and k=(-1,0,0)
bool test_sine_wave(MPI_Comm comm)
{
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  if (rank == 0) {
    std::cout << "\n=== Test 3: Sine wave FFT ===" << std::endl;
  }

  int global_shape[3] = {N0, N1, N2};
  parafaft::ParaFaFT_R2C<3, ParaFaFT_Backend> fft(global_shape, comm);

  int local_real_shape[3], real_start[3];
  int local_complex_shape[3], complex_start[3];
  fft.get_local_real_shape(local_real_shape);
  fft.get_real_global_start(real_start);
  fft.get_local_complex_shape(local_complex_shape);
  fft.get_complex_global_start(complex_start);

  int local_real_size = fft.get_local_real_size();
  int local_complex_size = fft.get_local_complex_size();

  // Initialize with sin(2*pi*x/N0)
  std::vector<double> real_input(local_real_size);
  for (int i = 0; i < local_real_shape[0]; ++i) {
    for (int j = 0; j < local_real_shape[1]; ++j) {
      for (int k = 0; k < local_real_shape[2]; ++k) {
        int idx = i * local_real_shape[1] * local_real_shape[2] + j * local_real_shape[2] + k;
        int gi = real_start[0] + i;

        double x = (double)gi / N0;
        real_input[idx] = std::sin(2.0 * M_PI * x);
      }
    }
  }

  std::vector<std::complex<double>> complex_output(local_complex_size);

  // Forward FFT
  fft.forward(real_input.data(), complex_output.data());

  // Check: FFT of sin(2*pi*x/N0) should give:
  // -i*N/2 at k=(1,0,0) and +i*N/2 at k=(N0-1,0,0)
  // Due to Hermitian symmetry, only k=(1,0,0) is stored in r2c output
  double total_N = (double)(N0 * N1 * N2);
  double expected_magnitude = total_N / 2.0;

  // Print some Fourier coefficients for debugging
  for (int i = 0; i < local_complex_shape[0]; ++i) {
    for (int j = 0; j < local_complex_shape[1]; ++j) {
      for (int k = 0; k < local_complex_shape[2]; ++k) {
        int idx = i * local_complex_shape[1] * local_complex_shape[2] + j * local_complex_shape[2] + k;
        int gi = complex_start[0] + i;
        int gj = complex_start[1] + j;
        int gk = complex_start[2] + k;

        double mag = std::abs(complex_output[idx]);
        if (mag > TOLERANCE * total_N) {
          std::cout << "Rank " << rank << ": k=(" << gi << "," << gj << "," << gk << "): " << complex_output[idx]
                    << " (magnitude = " << mag << ")" << std::endl;
        }
      }
    }
  }

  if (rank == 0) {
    std::cout << "Expected non-zero mode at k=(1,0,0) with magnitude " << expected_magnitude << std::endl;
    std::cout << "(Visual inspection required for this test)" << std::endl;
  }

  return true;
}

// Test 4: In-place padded buffer roundtrip (matches CosmoLattice memory layout)
// CosmoLattice uses padded real arrays: last dimension is 2*(N/2+1)
bool test_inplace_padded_roundtrip(MPI_Comm comm)
{
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  if (rank == 0) {
    std::cout << "\n=== Test 4: In-place padded buffer roundtrip ===" << std::endl;
  }

  int global_shape[3] = {N0, N1, N2};
  parafaft::ParaFaFT_R2C<3, ParaFaFT_Backend> fft(global_shape, comm);

  int local_real_shape[3], real_start[3];
  fft.get_local_real_shape(local_real_shape);
  fft.get_real_global_start(real_start);

  // CosmoLattice padded layout: last dimension is 2*(N/2+1)
  int padded_last_dim = 2 * (N2 / 2 + 1);
  int padded_size = local_real_shape[0] * local_real_shape[1] * padded_last_dim;

  // Allocate padded buffer (as CosmoLattice does)
  std::vector<double> padded_buffer(padded_size, 0.0);

  // Fill with test data (only the non-padding part)
  for (int i = 0; i < local_real_shape[0]; ++i) {
    for (int j = 0; j < local_real_shape[1]; ++j) {
      for (int k = 0; k < local_real_shape[2]; ++k) {
        int idx = i * local_real_shape[1] * padded_last_dim + j * padded_last_dim + k;
        int gi = real_start[0] + i;
        int gj = real_start[1] + j;
        int gk = real_start[2] + k;

        double x = (double)gi / N0;
        double y = (double)gj / N1;
        double z = (double)gk / N2;

        padded_buffer[idx] = std::sin(2.0 * M_PI * x) * std::cos(2.0 * M_PI * y) + z;
      }
    }
  }

  // Save original for comparison
  std::vector<double> original = padded_buffer;

  // Forward in-place (operates directly on padded buffer)
  fft.forward_in_place(padded_buffer.data());

  // Backward in-place
  fft.backward_in_place(padded_buffer.data());

  // Check roundtrip: output should equal input * N
  double total_N = (double)(N0 * N1 * N2);
  double max_error = 0.0;
  int error_count = 0;

  for (int i = 0; i < local_real_shape[0]; ++i) {
    for (int j = 0; j < local_real_shape[1]; ++j) {
      for (int k = 0; k < local_real_shape[2]; ++k) {
        int idx = i * local_real_shape[1] * padded_last_dim + j * padded_last_dim + k;
        double expected = original[idx] * total_N;
        double error = std::abs(padded_buffer[idx] - expected);
        max_error = std::max(max_error, error);

        if (error > TOLERANCE * total_N) {
          error_count++;
        }
      }
    }
  }

  double global_max_error;
  MPI_Reduce(&max_error, &global_max_error, 1, MPI_DOUBLE, MPI_MAX, 0, comm);

  int global_error_count;
  MPI_Reduce(&error_count, &global_error_count, 1, MPI_INT, MPI_SUM, 0, comm);

  if (rank == 0) {
    std::cout << "In-place padded roundtrip max error: " << global_max_error << std::endl;
    std::cout << "Total error count: " << global_error_count << std::endl;

    if (global_max_error < TOLERANCE * total_N) {
      std::cout << "PASSED: In-place padded buffer roundtrip" << std::endl;
      return true;
    } else {
      std::cout << "FAILED: In-place padded buffer roundtrip" << std::endl;
      return false;
    }
  }

  return true;
}

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    std::cout << "========================================" << std::endl;
    std::cout << "Parafaft Standalone Test Suite" << std::endl;
    std::cout << "Grid size: " << N0 << " x " << N1 << " x " << N2 << std::endl;
    std::cout << "========================================" << std::endl;
  }

  bool all_passed = true;

  all_passed &= test_roundtrip(MPI_COMM_WORLD);
  all_passed &= test_constant_function(MPI_COMM_WORLD);
  all_passed &= test_sine_wave(MPI_COMM_WORLD);
  all_passed &= test_inplace_padded_roundtrip(MPI_COMM_WORLD);

  if (rank == 0) {
    std::cout << "\n========================================" << std::endl;
    if (all_passed) {
      std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
      std::cout << "SOME TESTS FAILED" << std::endl;
    }
    std::cout << "========================================" << std::endl;
  }

  MPI_Finalize();
  return all_passed ? 0 : 1;
}
