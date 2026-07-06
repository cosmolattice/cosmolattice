# 

<span style="font-size: 34px;">**Download**</span>

The code can be downloaded from our <a href="https://github.com/cosmolattice/cosmolattice_private" target="_blank">
**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ GitHub repository**. 
</a>

<span style="font-size: 34px;">**Installation**</span>

Here, we will present in detail how to build $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ for different platforms both on macOS and Linux. 
$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ requires almost nothing beyond standard development tools on a UNIX machine, see [*Requirements*][subsec_requirements].
We will also explain how to integrate it with external libraries and make some general comments on to make it work on High Performance Clusters (HPC).

### Requirements  { #subsec_requirements }

#### Required Tools/Libraries

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ requires a working `C++` compiler that fully supports the `C++20` standard, which is the case for all relatively modern compilers (see the table below). 
To obtain and set up the library, you will also need to have a working installation of `git`, `make` and `CMake`.
Typically, these tools can be easily installed on your system using the default package manager, see [*Installing Dependencies*][subsec_install_dependencies].

| Name  | Minimal Version | Notes                                 |
| ----- | --------------- | ------------------------------------- |
| git   | -               |                                       |
| make  | -               |                                       |
| CMake | 3.16            |                                       |
| fftw3 | 3               | Can be automatically built with CMake |

#### Supported compilers

| Name    | Minimal Version | Notes                          |
| ------- | --------------- | ------------------------------ |
| g++     | 10.1            | Minimal version tested: 11.4   |
| clang++ | 13              | Minimal version tested: 20.1.8 |
| nvcc    | 12.0            | Minimal version tested: 12.4   |


$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ will always download and build [Kokkos](https://kokkos.org/) automatically. If a GPU architecture is used, it will also download and build [KokkosFFT](https://github.com/kokkos/kokkos-fft) to provide GPU-native fourier transformations.

### Download and Build

To download the code, open a terminal in a folder of your choice and run the following command:
```bash
git clone https://github.com/cosmolattice/cosmolattice.git
```
This will create a folder called `cosmolattice` in the current directory. To build the code, you will need to use the `CMake` build system. To do so, navigate to the new `cosmolattice` folder and run the following commands:
```bash
cd cosmolattice
mkdir build
cd build
cmake ..
make -j8
```
The last command will compile the code using 8 threads. You can change this number to match the number of cores you want to use for the compilation. If everything goes well, you should now have a working installation of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and you can proceed to run your first simulation as explained in Section [*My first run*][sec_MyFirstRun].

If `fftw3` is not installed on your system, the `cmake` command will fail and notify you that it cannot find the library.
In this case, you can either install it using your system's package manager, or pass `-DAUTOBUILD_FFTW=ON` to the `cmake` command, which will automatically download and build it for you:
```bash
cmake -DAUTOBUILD_FFTW=ON ..   
```

### Device configuration

### Optional features

Optionally, you can enable the following libraries:

| Name    <div style="width:150px"> | Minimal Version | Extra Features                                                                        | Notes                                                                                                                                                            |
| --------------------------------- | --------------- | ------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| MPI                               | -               | Distributed parallelization                                                           | Needs an implementation of `MPI` compatible with the compiler you chose. The most common open-source ones are `OpenMPI` and `MVAPICH` (for `g++` and `clang++`). |
| ParaFaFT                          | -               | Distributed fourier transformations.                                                  | External library for distributed CPU/GPU fourier transforms. Automatically built and enabled when `MPI=ON`.                                                      |
| HDF5                              | 5               | Alternative output format; saving whole simulations, restarting and automatic backup. | If `MPI=ON`, needs to be compiled against MPI. Can be built automatically.                                                                                       |

#### MPI: Distributed Parallelization

Enabling `MPI` allows you to run simulations in parallel on multiple machines, which is particularly useful on HPC clusters and large simulations. For more details, see also the [*Parallelization*](Parallelization.md) section in the manual.

 To enable it, you need to pass the `-DMPI=ON` flag to the `cmake` command:
```bash
cmake -DMPI=ON -DHDF5=ON ..
```
This requires that you have an `MPI` installation present on your system. OpenMPI can be easily installed on typical POSIX systems using the default package manager, see [*Installing Dependencies*][subsec_install_dependencies].

### HDF5: A Hierarchical Data Format

`HDF5` is a file format and set of tools for managing complex data. It is particularly useful for saving whole simulations, restarting them, and automatically backing them up. 
Enabling `HDF5` allows you to use this format for all output files instead of the default text files. For more details, see also the [Backing up simulations with HDF5](Backing up simulations with HDF5.md) section in the manual.

To use `HDF5` with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, you can either install it using your system's package manager, or pass `-DAUTOBUILD_HDF5=ON` to the `cmake` command, which will automatically download and build it for you:
```bash
cmake -DHDF5=ON -DAUTOBUILD_HDF5=ON ..
```

### Installing CosmoLattice on a HPC Cluster

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

**Ubuntu:** Ubuntu comes by default with the `apt-get` package manager, which makes the installation of all basic utilities easy. The following lines should be enough to install what you need
```bash
sudo apt-get install make g++ cmake openmpi git
```
The `sudo` command is necessary to give you the admin rights, which you need to have to install software ''globally" on your PC.

**Fedora:** On Fedora, you can use `dnf` as a default package manager. Then the command are the same than on Ubuntu.
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
