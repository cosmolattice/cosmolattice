# Download &amp; Installation

The code can be downloaded from our <a href="https://github.com/cosmolattice/cosmolattice_private" target="_blank">
**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ GitHub repository**
</a>, see also the installation instructions below on how to do this directly from the command line.

Here, we will present in detail how to build $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ for different platforms both on macOS and Linux. 
$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ requires almost nothing beyond standard development tools on a UNIX machine, see [*Requirements*][subsec_requirements].
We will also explain how to integrate it with external libraries and make some general comments on to make it work on High Performance Clusters (HPC).

<div class="cl-facts cl-prereqs" markdown>
[git](#subsec_requirements){ .cl-fact }
[make](#subsec_requirements){ .cl-fact }
[CMake &ge; 3.16](#subsec_requirements){ .cl-fact }
[C++20 compiler](#subsec_requirements){ .cl-fact }
[fftw3](#subsec_requirements){ .cl-fact }
</div>

## Download and Build

<div class="cl-steps" markdown>

1.  To download the code, open a terminal in a folder of your choice and run the following command:
    ```bash
    git clone https://github.com/cosmolattice/cosmolattice.git
    ```

2.  This will create a folder called `cosmolattice` in the current directory. To build the code, you will need to use the `CMake` build system. To do so, navigate to the new `cosmolattice` folder and run the following commands: (<span style="color:orange;">**Jorge comment: Isn't this missing a -DMODEL and make cosmolattice?**</span>)
    ```bash
    cd cosmolattice
    mkdir build
    cd build
    cmake ..
    make -j8
    ```
    The last command will compile the code using 8 threads. You can change this number to match the number of cores you want to use for the compilation. If everything goes well, you should now have a working installation of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and you can proceed to run your first simulation as explained in Section [*My first run*][sec_MyFirstRun].

</div>

!!! tip "FFTW3 not found?"

    If `fftw3` is not installed on your system, the `cmake` command will fail and notify you that it cannot find the library.
    In this case, you can either install it using your system's package manager, or pass `-DAUTOBUILD_FFTW=ON` to the `cmake` command, which will automatically download and build it for you:
    ```bash
    cmake -DAUTOBUILD_FFTW=ON ..   
    ```

## Requirements  { #subsec_requirements }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ requires a working `C++` compiler that fully supports the `C++20` standard, which is the case for all relatively modern compilers (see the table below). 
To obtain and set up the library, you will also need to have a working installation of `git`, `make` and `CMake`.
Typically, these tools can be easily installed on your system using the default package manager, see [*Installing Dependencies*][subsec_install_dependencies].

<div class="grid cl-req" markdown>

<div markdown>

**Required Tools/Libraries**
{: .cl-req-cap }

| Name  | Minimal Version | Notes                                 |
| ----- | --------------- | ------------------------------------- |
| git   | -               |                                       |
| make  | -               |                                       |
| CMake | 3.16            |                                       |
| fftw3 | 3               | Can be automatically built with CMake |

</div>

<div markdown>

**Supported compilers**
{: .cl-req-cap }

| Name    | Minimal Version | Notes                          |
| ------- | --------------- | ------------------------------ |
| g++     | 10.1            | Minimal version tested: 11.4   |
| clang++ | 13              | Minimal version tested: 20.1.8 |
| nvcc    | 12.0            | Minimal version tested: 12.4   |

</div>

</div>

!!! info "Kokkos is downloaded and built for you"

    $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ will always download and build [Kokkos](https://kokkos.org/) automatically. If a GPU architecture is used, it will also download and build [KokkosFFT](https://github.com/kokkos/kokkos-fft) to provide GPU-native fourier transformations.

## Device configuration

A device is a fixed pair of hardware and corresponding drivers that can be used to perform computations. In the context of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, a device can usually be either a CPU together with a parallelization framework (e.g. OpenMP or POSIX threads) or a GPU with a parallelization framework (e.g. CUDA or HIP), for more details see the [*Parallelization*](../Manual/Parallelization.md) section in the manual.

The library is designed to be able to run on a wide range of devices, and it can automatically detect what devices are available on a given machine.
In that case, it will first check for GPU support, checking first CUDA (used for Nvidia cards) and then HIP (for AMD cards).

A CPU device is always required. TempLat will always check first for OpenMP support and then for POSIX threads. If none of these are available, it will fall back to a serial implementation.

If you want to force the use of a specific device, you can do so by setting the appropriate flag when configuring the project with CMake.
The available options are `-DCUDA=ON`, `-DHIP=ON`, `-DOPENMP=ON`, `-DPTHREADS=ON` and `-DNOTHREADING=ON`. For example, to disable all shared-memory parallelization, you can do
```bash
cmake -DMODEL=lphi4 -DNOTHREADING=ON ../
```

<div class="grid cards" markdown>

-   __CUDA__ · NVIDIA GPUs

    ---

    To compile for NVIDIA GPUs using CUDA, you can enable the CUDA backend by setting
    ```bash
    cmake -DMODEL=lphi4 -DCUDA=ON ../
    ```
    Specifying the architecture is optional for CUDA, as Kokkos can usually detect it correctly. However, when compiling offline (e.g. on a HPC cluster) it may be necessary to specify it manually. 
    You can do so by passing the appropriate flag to Kokkos as described in the section [Offline compilation (Kokkos)](#offline-compilation) below.

-   __HIP__ · AMD GPUs

    ---

    To compile for AMD GPUs using HIP, you can enable the HIP backend by setting
    ```bash
    cmake -DMODEL=lphi4 -DHIP=ON ..
    ```
    Note that specifying the architecture is mandatory for HIP.

</div>

### Offline compilation

To compile an application to be run on a specific architecture, you can directly pass the target architecture to Kokkos.
For a list of supported architectures, see [the Kokkos documentation](https://kokkos.org/kokkos-core-wiki/get-started/configuration-guide.html#gpu-architectures). For example, for an RTX 4070, you would pass 
```bash
cmake -DMODEL=lphi4 -DKokkos_ARCH_ADA89 ../
```

## Optional features

Optionally, you can enable the following libraries:

| Name    <div style="width:150px"> | Minimal Version | Extra Features                                                                        | Notes                                                                                                                                                            |
| --------------------------------- | --------------- | ------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| MPI                               | -               | Distributed parallelization                                                           | Needs an implementation of `MPI` compatible with the compiler you chose. The most common open-source ones are `OpenMPI` and `MVAPICH` (for `g++` and `clang++`). |
| ParaFaFT                          | -               | Distributed fourier transformations.                                                  | External library for distributed CPU/GPU fourier transforms. Automatically built and enabled when `MPI=ON`.                                                      |
| HDF5                              | 5               | Alternative output format; saving whole simulations, restarting and automatic backup. | If `MPI=ON`, needs to be compiled against MPI. Can be built automatically.                                                                                       |

<div class="grid cards" markdown>

-   __MPI: Distributed Parallelization__

    ---

    Enabling `MPI` allows you to run simulations in parallel on multiple machines, which is particularly useful on HPC clusters and large simulations. For more details, see also the [*Parallelization*](../Manual/Parallelization.md) section in the manual.

     To enable it, you need to pass the `-DMPI=ON` flag to the `cmake` command:
    ```bash
    cmake -DMPI=ON -DHDF5=ON ..
    ```
    This requires that you have an `MPI` installation present on your system. OpenMPI can be easily installed on typical POSIX systems using the default package manager, see [*Installing Dependencies*][subsec_install_dependencies].

-   __HDF5: A Hierarchical Data Format__

    ---

    `HDF5` is a file format and set of tools for managing complex data. It is particularly useful for saving whole simulations, restarting them, and automatically backing them up. 
    Enabling `HDF5` allows you to use this format for all output files instead of the default text files. For more details, see also the [HDF5: Output and Backups](../Manual/HDF5: Output and Backups.md) section in the manual.

    To use `HDF5` with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, you can either install it using your system's package manager, or pass `-DAUTOBUILD_HDF5=ON` to the `cmake` command, which will automatically download and build it for you:
    ```bash
    cmake -DHDF5=ON -DAUTOBUILD_HDF5=ON ..
    ```

</div>

## Installing CosmoLattice on a HPC Cluster

!!! note "Every HPC cluster is different"

    By its nature, CosmoLattice is intended to be used to run parallel simulations on many cores, so it is perfectly suited to be used on High-Performance Computing (HPC) Clusters. As every single HPC cluster is different, it is impossible to write a generic explanation of how to install CosmoLattice on a cluster. We still provide the user with some general guidelines and information about commonly encountered features. If you encounter trouble using a specific cluster, or you are missing some libraries/tools, you should directly contact the IT team maintaining it.

Often, when connecting to a cluster, you do not have access to any libraries by default.
Typically, HPC clusters provide a large number of libraries and tools, but you need to enable them first.
Most clusters use for this purpose *Environment Modules*. 
In that case, to see which libraries/tools are available on your cluster, you can run the following command:
```bash
module list
```
This will show you a list of currently loaded modules. To see which modules are available, you can run:
```bash
module avail
```
or, for a more detailed list:
```bash
module spider
```
This will show a list of available packages, possibly with different versions. In order to run CosmoLattice, you need appropriate versions for the compiler, `CMake` and `fftw3`, as well as `MPI` and `HDF5` if you want to use the parallelization and backup features.
If these packages are missing or outdated, you should contact your IT team and ask for them to be installed.
Note that `fftw3` and `HDF5` can be automatically installed by the library, but HPC clusters usually offer optimized versions of these libraries.

If your cluster is actually using Environment Modules, you can load the appropriate libraries/tools by using the `load` option of the `module` command. In that case, you will typically need to type something along the following lines before compiling CosmoLattice,
```bash
module list
module load CMakeName
module load CompilerName
module load MPIName
module load FFTW3Name
module load HDF5Name # Only if you need it
```
where `CMakeName, CompilerName, MPIName, FFTW3Name` and `HDF5Name` refer to the appropriate name returned by `module spider`. Of course, you should not load `fftw3` or `hdf5` if you plan to have $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ build them locally.

At this point, you should be able to compile and run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ on your cluster. If you encounter any issues, please contact your IT team for assistance.

## Installing Dependencies { #subsec_install_dependencies }

=== "Ubuntu"

    **Ubuntu:** Ubuntu comes by default with the `apt-get` package manager, which makes the installation of all basic utilities easy. The following lines should be enough to install what you need
    ```bash
    sudo apt-get install make g++ cmake openmpi git
    ```
    The `sudo` command is necessary for the admin rights needed to install software ''globally" on your PC.

=== "Arch Linux"

    **Arch Linux:** On Arch Linux, `pacman` is the default package manager. 
    ```bash
    sudo pacman -S make g++ cmake openmpi git
    ```

=== "Fedora"

    **Fedora:** On Fedora, you can use `dnf` as a default package manager. 
    ```bash
    sudo dnf install make g++ cmake openmpi-devel git
    ```
    By default, `dnf` installs `openMPI` such that it is not globally accessible. To fix that, go to your `home` folder and edit or create the `.bashrc`
    ```bash
    cd
    res=$(find /usr -name "mpirun") #finds where openMPI was installed
    echo 'export PATH=$PATH:'${res} >> .bashrc # adds it to your path
    source .bashrc # reload it
    ```
    If you are using a different shell than `bash`, you will need to edit the corresponding configuration file (e.g. `.zshrc` for `zsh`).

=== "macOS"

    **macOS:** The first time you want to do something related to coding on your Mac, you need to start by opening a terminal and run:
    ```bash
    xcode-select --install
    ```
    This will install some basic utilities, like the Apple-Clang compiler and `make`.
    By default, macOS does not come with a package manager, so you will need to install one. We recommend using [`Homebrew`](https://brew.sh/), which can be installed as follows:
    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ``` 
    Once this is done, packages can simply be installed as 
    ```bash
    brew install gcc open-mpi cmake git 
    ``` 
