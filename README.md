# CosmoLattice

## *A modern code for lattice simulations of scalar and gauge field dynamics in an expanding universe*

[![CosmoLattice Logo](https://cosmolattice.net/assets/CL_Icon.svg)](https://cosmolattice.net)

### Documentation

To learn how to install and execute the code, as well as how it works, please visit **the official webpage of CosmoLattice at [cosmolattice.net](https://cosmolattice.net)**. Besides that, you will find there in-depth pedagogical explanations about the underlying theoretical framework, as well as a user manual and developer informations.

The underlying theoretical framework has been also described in <a href=https://arxiv.org/pdf/2006.15122.pdf target="_blank" rel="noopener noreferrer" > arXiv:2006.15122</a> and <a href=https://arxiv.org/abs/2512.15627 target="_blank" rel="noopener noreferrer" > arXiv:2512.15627</a>.

### Citing CosmoLattice
If you use CosmoLattice in your research, no matter how much (or little), please cite it as explained [on the official webpage](https://cosmolattice.net/CLcitation.html).


### Basic installation

*Minimal requirements:* 
- `CMake` version 3.16 or above
- `clang++`, `g++` or another compiler with support for C++20

```bash
git clone https://github.com/cosmolattice/cosmolattice.git
cd cosmolattice   
mkdir build                     
cd build                        
cmake -DMODEL=lphi4 ../
make cosmolattice
```

This will compile the ``lphi4`` model. To run it with the default input file, you can do

```bash
./lphi4 input=../models/parameter-files/lphi4.in
```

The above commands just represent a very brief guide for the installation and execution of CosmoLattice. 
For further information, see the in-detail explanations on [the webpage](https://cosmolattice.net/Quick%20installation%20and%20execution.html).

## Backend

CosmoLattice uses

<a href="https://cosmolattice.github.io/templat/"><img src="https://github.com/cosmolattice/templat/raw/refs/heads/main/docs/logo/logo_red_nobg.svg" alt="TempLat" style="width: 200px; height: 100px;"></a>

 as a backend for the lattice and field operations. [TempLat](https://cosmolattice.github.io/templat/) is a C++ template library that provides a high-level interface for lattice computations, allowing for efficient and flexible implementations of various models.

We briefly explain some core configuration steps and refer to the [cosmolattice webpage](https://cosmolattice.net/Quick%20installation%20and%20execution.html) and the [TempLat repository](https://github.com/cosmolattice/templat) for further information.

### Choosing the device

By default, CosmoLattice will attempt to detect what devices are available on a given machine.
In that case, it will first check for GPU support, checking first CUDA and then HIP. 
On the CPU side, it will first check for OpenMP support and then for C++ threads. If none of these are available, it will fall back to a serial implementation.
If you want to force the use of a specific device, you can do so by setting the appropriate flag when configuring the project with CMake.
The available options are `-DCUDA=ON`, `HIP=ON`, `-DOPENMP=ON`, `-DPTHREADS=ON` and `-DNOTHREADING=ON`. To force serial for example, you can do
```bash
cmake -DMODEL=lphi4 -DNOTHREADING=ON ../
```

#### CUDA

Using Nvidia GPUs requires that you have the CUDA toolkit, or the NVIDIA HPC SDK installed on your machine, or loaded as a module on your cluster. To compile for NVIDIA GPUs using CUDA, you can enable the CUDA backend by setting
```bash
cmake -DMODEL=lphi4 -DCUDA=ON ../
```
Specifying the architecture is optional for CUDA, as Kokkos can usually detect it correctly. However, if you want to specify it manually, you can do so by passing the appropriate flag to Kokkos as described in the section [Offline compilation (Kokkos)](#offline-compilation-kokkos) below.

#### HIP

Using AMD GPUs requires that you have the ROCm toolkit installed on your machine, or loaded as a module on your cluster.
To compile for AMD GPUs using HIP, you can enable the HIP backend by setting
```bash
export CXX=hipcc
cmake -DMODEL=lphi4 -DHIP=ON ..
```
Specifying the HIP compiler (`hipcc`) is necessary for the correct detection of the HIP toolkit.
Specifying the architecture is optional for HIP, as Kokkos can usually detect it correctly. However, if you want to specify it manually, you can do so by passing the appropriate flag to Kokkos as described in the section [Offline compilation (Kokkos)](#offline-compilation-kokkos) below.

### Offline compilation (Kokkos)

To compile an application to be run on a different architecture, you can directly pass the target architecture to Kokkos. For a list of supported architectures, see [the Kokkos documentation](https://kokkos.org/kokkos-core-wiki/get-started/configuration-guide.html#gpu-architectures). For example, for an RTX 4070, you would pass 
```bash
cmake -DMODEL=lphi4 -DKokkos_ARCH_ADA89=ON ../
```
If no architecture is specified, Kokkos will attempt to detect the architecture of the machine. However, as compilation for GPU can take up to an hour, it is recommended on a cluster to queue the compilation separately on a node without a GPU, which requires offline compilation as described here.

### CMake Configuration Options

All custom CMake flags can be passed when configuring the project, e.g. `cmake -DMODEL=lphi4 -DMPI=ON -DHDF5=ON ../`.

| Flag                  | Description                             | Default                                       |
| --------------------- | --------------------------------------- | --------------------------------------------- |
| `MODEL`               | The model to compile                    | `lphi4`                                       |
| `COSMOINTERFACE_TEST` | Compile CosmoInterface tests            | `OFF`                                         |
| `NPROCESSES`          | MPI ranks used to run the tests         | `4`                                           |
| `PARAFAFT`            | ParaFaFT support for parallel FFTs      | `ON` when `MPI=ON`, else `OFF`                |
| `MPI`                 | MPI support                             | `OFF`                                         |
| `HDF5`                | HDF5 support                            | `OFF`                                         |
| `AUTOBUILD_HDF5`      | Build HDF5 from source if not found     | `OFF`                                         |
| `AUTOBUILD_FFTW`      | Build FFTW from source if not found     | `OFF`                                         |
| `FLOAT`               | Single-precision (float) FFTW support   | `OFF`                                         |
| `TEMPLAT_TEST`        | Enable TempLat's tests                  | `OFF`                                         |
| `TEMPLAT_BENCH`       | Build TempLat's benchmarks              | `OFF`                                         |
| `DEVICE_PROVIDER`     | Backend for parallelization             | `Kokkos`                                      |
| `CUDA`                | CUDA support for NVIDIA GPUs            | `OFF`                                         |
| `HIP`                 | HIP support for AMD GPUs                | `OFF`                                         |
| `OPENMP`              | OpenMP CPU parallelization              | `OFF`                                         |
| `PTHREADS`            | C++ threads CPU parallelization         | `OFF`                                         |
| `NOTHREADING`         | No parallelization                      | `OFF`                                         |
| `NATIVE`              | Pass `--march=native` to compiler       | `ON` (non-macOS), `OFF` (macOS)               |
| `KOKKOSFFT`           | KokkosFFT for single-node GPU FFTs      | `ON` when CUDA/HIP enabled, else `OFF`        |
| `TEMPLAT_REPO`        | TempLat repository, e.g. your fork      | `https://github.com/cosmolattice/templat.git` |
| `TEMPLAT_BRANCH`      | TempLat branch or tag to use (for devs) | `v1.0.0`                                      |

`PARAFAFT` requires `MPI=ON`; if it is requested without MPI, TempLat warns and disables it.
