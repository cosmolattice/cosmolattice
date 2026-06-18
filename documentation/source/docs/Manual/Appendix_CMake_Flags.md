We collect here the different CMake flags the user can pass to influence the compilation process.

### Model selection

| **Flag** | **Explanation** |
| --- | --- |
| `-DMODEL` | Takes `modelname` as an argument, where `modelname` is the name of the model you want to compile. |

### Device backends

By default, CosmoLattice will auto-detect available backends. GPU backends are checked first (CUDA, then HIP), then CPU backends (OpenMP, then pthreads). If nothing is found, it falls back to no threading. You can override auto-detection by explicitly setting one of the flags below.

| **Flag** | **Explanation** |
| --- | --- |
| `-DCUDA` | `ON` or `OFF`. Enable NVIDIA CUDA GPU backend. Default: `OFF` (auto-detected). |
| `-DHIP` | `ON` or `OFF`. Enable AMD HIP GPU backend. Default: `OFF` (auto-detected). |
| `-DOPENMP` | `ON` or `OFF`. Enable OpenMP CPU backend. Default: `OFF` (auto-detected). |
| `-DPTHREADS` | `ON` or `OFF`. Enable C++ threads (pthreads) CPU backend. Default: `OFF` (auto-detected). |
| `-DNOTHREADING` | `ON` or `OFF`. Disable threading entirely (serial execution). Default: `OFF`. |
| `-DDEVICE_PROVIDER` | Selects the device provider framework. Currently only `Kokkos` is supported. Default: `Kokkos`. |

### Libraries and features

| **Flag** | **Explanation** |
| --- | --- |
| `-DMPI` | `ON` or `OFF`. Enable MPI parallelization. Default: `OFF`. |
| `-DHDF5` | `ON` or `OFF`. Enable HDF5 support for I/O (saving/loading simulations, 3D snapshots). Default: `OFF`. |
| `-DPARAFAFT` | `ON` or `OFF`. Enable ParaFaFT for parallel FFTs (requires MPI). Replaces the old PFFT library. Default: `OFF`. |
| `-DFLOAT` | `ON` or `OFF`. Enable single-precision (float) FFTW support. Automatically enabled when using KokkosFFT. Default: `OFF`. |
| `-DKOKKOSFFT` | `ON` or `OFF`. Enable KokkosFFT for single-node GPU FFTs. Automatically set to `ON` when CUDA or HIP is enabled. Default: `OFF`. |

### Auto-building dependencies

| **Flag** | **Explanation** |
| --- | --- |
| `-DAUTOBUILD_FFTW` | `ON` or `OFF`. Automatically download and build FFTW from source if not found on the system. Default: `OFF`. |
| `-DAUTOBUILD_HDF5` | `ON` or `OFF`. Automatically download and build HDF5 from source if not found on the system. Default: `OFF`. |

### Compiler optimization flags

| **Flag** | **Explanation** |
| --- | --- |
| `-DNATIVE` | `ON` or `OFF`. Pass `-march=native` to the compiler for architecture-specific optimizations. Default: `ON` on Linux, `OFF` on macOS. |
| `-DSSE` | `ON` or `OFF`. Enable SSE instructions (`-msse -msse2 -msse3 -msse4`). Default: `OFF`. |
| `-DAVX` | Can be set to `OFF`, `mavx`, `mavx2`, or `mavx512f`. Enables AVX instructions up to the specified level. Default: `OFF`. |

### Testing

| **Flag** | **Explanation** |
| --- | --- |
| `-DTEMPLAT_TESTS` | `ON` or `OFF`. Build the test suite. Default: `OFF`. |
| `-DNPROCESSES` | Number of MPI processes to use when running tests. Default: `4`. |

### Deprecated flags

The following flags from CosmoLattice 1.0 are still accepted for backward compatibility, but will emit a warning. They will be removed in a future version.

| **Old flag** | **Replacement** |
| --- | --- |
| `-DOpenMP` | `-DOPENMP` |
| `-DThreads` | `-DPTHREADS` |
| `-DSerial` | `-DNOTHREADING` |
| `-DFloat` | `-DFLOAT` |
| `-DPFFT` | Removed. Use `-DPARAFAFT=ON` instead. |
| `-DMYPFFT_PATH` | Removed. Use `-DPARAFAFT=ON` instead. |
| `-DMYFFTW3_PATH` | No longer needed. Use `-DAUTOBUILD_FFTW=ON` or set `CMAKE_PREFIX_PATH`. |
| `-DMYHDF5_PATH` | No longer needed. Use `-DAUTOBUILD_HDF5=ON` or set `CMAKE_PREFIX_PATH`. |
| `-DG++OPT` | Use `CMAKE_BUILD_TYPE=Release` or `Debug` instead. |
| `-DG++SSE` | Use `-DSSE=ON` instead. |
| `-DG++AVX` | Use `-DAVX=mavx2` (or `mavx`, `mavx512f`) instead. |
| `-DTESTING` | Use `-DTEMPLAT_TESTS=ON` instead. |
