
# CosmoLattice

* This branch is under active development, NO GUARANTEES WHATSOEVER.*

## *A modern code for lattice simulations of scalar and gauge field dynamics in an expanding universe*
### Authors: Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg

### Documentation

- Please visit the official webpage for CosmoLattice at [cosmolattice.net](https://cosmolattice.net).
- To learn how to install and execute the code as well as how it works :  <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" > arXiv:2102.01031</a> .
- To learn about the underlying theoretical framework: <a href=https://arxiv.org/pdf/2006.15122.pdf target="_blank" rel="noopener noreferrer" > arXiv:2006.15122</a> .

### Basic installation

*Minimal requirements:* 
- `CMake` version 3.16 or above
- `clang`, `g++` or another compiler with support for C++20

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
./lphi4 input=../src/models/parameter-files/lphi4.in
```

The above commands just represent a very brief guide for the installation and execution of CosmoLattice. 
For further information, see Appendix A of the <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" >user-manual</a>.
All options of CosmoLattice, as well as how to activate them and how to install the optional external libraries are explained at length there.

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

### Device Providers

CosmoLattice fully abstracts away the management and dispatching to actual devices, which is handled by device providers. The default device provider is Kokkos, which supports a wide range of devices and architectures. Currently, only Kokkos is implemented as a device provider.

### CMake Configuration Options

All custom CMake flags can be passed when configuring the project, e.g. `cmake -DMODEL=lphi4 -DMPI=ON -DHDF5=ON ../`.

| Flag                   | Description                        | Default                                |
| ---------------------- | ---------------------------------- | -------------------------------------- |
| `MODEL`                | The model to compile               | `lphi4`                                |
| `COSMOINTERFACE_TEST`  | Compile CosmoInterface tests       | `OFF`                                  |
| `PARAFAFT`             | ParaFaft support for parallel FFTs | `OFF`                                  |
| `MPI`                  | MPI support                        | `OFF`                                  |
| `HDF5`                 | HDF5 support                       | `OFF`                                  |
| `TEMPLAT_TEST`         | Enable TempLat's tests             | `OFF`                                  |
| `DEVICE_PROVIDER`      | Backend for parallelization        | `Kokkos`                               |
| `CUDA`                 | CUDA support for NVIDIA GPUs       | `OFF`                                  |
| `HIP`                  | HIP support for AMD GPUs           | `OFF`                                  |
| `OPENMP`               | OpenMP CPU parallelization         | `OFF`                                  |
| `PTHREADS`             | C++ threads CPU parallelization    | `OFF`                                  |
| `NOTHREADING`          | No parallelization                 | `OFF`                                  |
| `NATIVE`               | Pass `--march=native` to compiler  | `ON` (non-macOS), `OFF` (macOS)        |
| `KOKKOSFFT`            | KokkosFFT for single-node GPU FFTs | `ON` when CUDA/HIP enabled, else `OFF` |
| `TEMPLAT_BRANCH`       | Used branch of TempLat (for devs)  | `main`                                 |

### Credits

CosmoLattice is freely available to anyone who wants to use or modify it. However, whenever 
using CosmoLattice in your research, no matter how much (or little) you modify the code, 
<b>please cite both <a href=https://arxiv.org/pdf/2006.15122.pdf target="_blank" rel="noopener noreferrer" > arXiv:2006.15122</a> 
and <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" > arXiv:2102.01031</a> in your papers</b>. 
