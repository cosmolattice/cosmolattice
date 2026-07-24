We collect here the different CMake flags the user can pass to influence the compilation process.

### Model selection

| **Flag**  | **Explanation**                                                                                   |
| --------- | ------------------------------------------------------------------------------------------------- |
| `-DMODEL` | Takes `modelname` as an argument, where `modelname` is the name of the model you want to compile. |

### Device backends

By default, CosmoLattice will auto-detect available backends. GPU backends are checked first (CUDA, then HIP), then CPU backends (OpenMP, then pthreads). If OpenMP is not found, it falls back to pthreads. You can override auto-detection by explicitly setting one of the flags below.

| **Flag**            | **Explanation**                                                                                 |
| ------------------- | ----------------------------------------------------------------------------------------------- |
| `-DCUDA`            | `ON` or `OFF`. Enable NVIDIA CUDA GPU backend. Default: `OFF` (auto-detected).                  |
| `-DHIP`             | `ON` or `OFF`. Enable AMD HIP GPU backend. Default: `OFF` (auto-detected).                      |
| `-DOPENMP`          | `ON` or `OFF`. Enable OpenMP CPU backend. Default: `OFF` (auto-detected).                       |
| `-DPTHREADS`        | `ON` or `OFF`. Enable C++ threads (pthreads) CPU backend. Default: `OFF` (auto-detected).       |
| `-DNOTHREADING`     | `ON` or `OFF`. Disable threading entirely (serial execution). Default: `OFF`.                   |
| `-DDEVICE_PROVIDER` | Selects the device provider framework. Currently only `Kokkos` is supported. Default: `Kokkos`. |

### Libraries and features

| **Flag**      | **Explanation**                                                                                                                  |
| ------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| `-DMPI`       | `ON` or `OFF`. Enable MPI parallelization. Default: `OFF`.                                                                       |
| `-DHDF5`      | `ON` or `OFF`. Enable HDF5 support for I/O (saving/loading simulations, 3D snapshots). Default: `OFF`.                           |
| `-DPARAFAFT`  | `ON` or `OFF`. Enable ParaFaFT for parallel FFTs (requires MPI; automatically enabled when `-DMPI=ON`). Replaces the old PFFT library. Default: `OFF`.                  |
| `-DFLOAT`     | `ON` or `OFF`. Enable single-precision (float) FFTW support. Automatically enabled when using KokkosFFT. Default: `OFF`.         |
| `-DKOKKOSFFT` | `ON` or `OFF`. Enable KokkosFFT for single-node GPU FFTs. Automatically set to `ON` when CUDA or HIP is enabled. Default: `OFF`. |

### Auto-building dependencies

| **Flag**           | **Explanation**                                                                                              |
| ------------------ | ------------------------------------------------------------------------------------------------------------ |
| `-DAUTOBUILD_FFTW` | `ON` or `OFF`. Automatically download and build FFTW from source if not found on the system. Default: `OFF`. |
| `-DAUTOBUILD_HDF5` | `ON` or `OFF`. Automatically download and build HDF5 from source if not found on the system. Default: `OFF`. |

### TempLat source

TempLat is downloaded automatically at configure time. These flags let you point the build at a different source, for instance your own fork.

| **Flag**           | **Explanation**                                                                                              |
| ------------------ | ------------------------------------------------------------------------------------------------------------ |
| `-DTEMPLAT_REPO`   | Git repository to fetch TempLat from. Can be any URL git understands, including a local path. Default: `https://github.com/cosmolattice/templat.git`. |
| `-DTEMPLAT_BRANCH` | Branch, tag or commit of TempLat to check out. Default: `main`.                                              |

For example, to build against your own fork:

```bash
cmake -S . -B build -DTEMPLAT_REPO=https://github.com/yourname/templat.git -DTEMPLAT_BRANCH=my-feature
```

Note that these are cached variables, so changing them in an existing build directory requires re-running CMake with the new value (or configuring from a clean build directory).

### Compiler optimization flags

| **Flag**   | **Explanation**                                                                                                                                                                       |
| ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `-DNATIVE` | `ON` or `OFF`. Pass `-march=native` to the compiler for architecture-specific optimizations. This automatically enables AVX/SSE if available. Default: `ON` on Linux, `OFF` on macOS. |
| `-DSSE`    | `ON` or `OFF`. Enable SSE instructions (`-msse -msse2 -msse3 -msse4`). Default: `OFF`.                                                                                                |
| `-DAVX`    | Can be set to `OFF`, `mavx`, `mavx2`, or `mavx512f`. Enables AVX instructions up to the specified level. Default: `OFF`.                                                              |

### Testing

| **Flag**               | **Explanation**                                                            |
| ---------------------- | -------------------------------------------------------------------------- |
| `-DCOSMOINTERFACE_TEST` | `ON` or `OFF`. Build the CosmoInterface test suite. Default: `OFF`.       |
| `-DTEMPLAT_TEST`       | `ON` or `OFF`. Build TempLat's own test suite. Default: `OFF`.             |
| `-DNPROCESSES`         | Number of MPI processes to use when running tests. Default: `4`.          |

### Deprecated flags

Unless noted otherwise, the following flags from CosmoLattice 1.0 are still accepted for backward compatibility, but will emit a warning. They will be removed in a future version. See also [Migrating from v1.x](Migration.md).

| **Old flag**     | **Replacement**                                                                                 |
| ---------------- | ------------------------------------------------------------------------------------------------- |
| `-DFLOATFFT`     | **Silently ignored.** Use `-DFLOAT=ON` instead.                                                 |
| `-DPFFT`         | **Configure error.** PFFT support has been discontinued, use `-DPARAFAFT=ON` instead.           |
| `-DMYPFFT_PATH`  | **Configure error.** PFFT support has been discontinued, use `-DPARAFAFT=ON` instead.           |
| `-DMYFFTW3_PATH` | Appended to `CMAKE_PREFIX_PATH`. Use `-DAUTOBUILD_FFTW=ON` or set `CMAKE_PREFIX_PATH` directly. |
| `-DMYHDF5_PATH`  | Appended to `CMAKE_PREFIX_PATH`. Use `-DAUTOBUILD_HDF5=ON` or set `CMAKE_PREFIX_PATH` directly. |
| `-DG++OPT`       | Use `CMAKE_BUILD_TYPE=Release` or `Debug` instead.                                              |
| `-DG++SSE`       | Use `-DSSE=ON` instead.                                                                         |
| `-DG++AVX`       | Use `-DAVX=mavx2` (or `mavx`, `mavx512f`) instead.                                              |
| `-DTESTING`      | **Silently ignored.** Use `-DCOSMOINTERFACE_TEST=ON` instead.                                   |
| `-DOpenMP`       | Use `-DOPENMP=ON` instead.                                                                      |
| `-DThreads`      | Use `-DPTHREADS=ON` instead.                                                                    |
| `-DSerial`       | Use `-DNOTHREADING=ON` instead.                                                                 |
| `-DFloat`        | Use `-DFLOAT=ON` instead.                                                                       |
