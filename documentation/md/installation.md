# Installation {#Installation}

To compile and run this project, there are very few requirements which you can easily install using your package manager on Linux or MacOS:

- [git](https://git-scm.com/) for external requirements and to clone this repository.
- [CMake](https://www.cmake.org/)>=3.10 for the build systems of DiFfRG, deal.ii and other libraries.
- [GNU Make](https://www.gnu.org/software/make/) or another generator of your choice.
- A compiler supporting at least the C++20 standard. This project is only tested using the [GCC](https://gcc.gnu.org/) compiler suite, as well as with `AppleClang` and `Clang`.

The following requirements are optional:
- [Doxygen](https://www.doxygen.org/) to build the documentation.
- [CUDA](https://developer.nvidia.com/cuda-toolkit) doing lattice simulations on the GPU, which gives a speedup for the calculation of the order 10 - 100x, depending on your hardware. In case you wish to use CUDA, make sure you have a compiler available on your system compatible with your version of `nvcc`, e.g. `g++`<=13.2 for CUDA 12.5

All other requirements are bundled and automatically built with CosmoLattice
The framework has been tested with the following systems:

#### Arch Linux
```bash
$ pacman -S git cmake gcc doxygen
```

#### Rocky Linux
```bash
$ dnf --enablerepo=devel install -y gcc-toolset-12 cmake git openblas-devel doxygen doxygen-latex python3 python3-pip gsl-devel
$ scl enable gcc-toolkit-12 bash
```
The second line is necessary to switch into a shell where `g++-12` is available

#### Ubuntu
```bash
$ apt-get update
$ apt-get install git cmake build-essential doxygen
```

#### MacOS
First, install xcode and homebrew, then run
```bash
$ brew install cmake doxygen
```

#### Windows

If using Windows, instead of running the project directly, it is recommended to use [WSL](https://learn.microsoft.com/en-us/windows/wsl/setup/environment) and then go through the installation as if on Linux (e.g. Arch or Ubuntu).

#### Docker and other container runtime environments

...

## Setup

```bash
$ git clone ...
```
and start the build after switching to the git directory.
```bash
$ cd cosmolattice
$ mkdir build
$ cd build
$ cmake ..
```