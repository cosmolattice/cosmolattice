In Section [*My first run*][sec_MyFirstRun] we explain how to compile and run CosmoLattice assuming everything is setup correctly. In this Appendix, we will explain how to achieve such a set-up. We first present in detail how to install the relevant tools and libraries, including both the required and optional ones, on a UNIX (macOS and Linux)\footnote{CosmoLattice is written in `C++` and uses only standard packages, so it should work with minimum trouble on Windows. However, no one has verified this statement for now.} personal computer. These are summarized in Table \ref{tab:requirements}. We then make general comments on to make it work on High Performance Clusters (HPC).

In the following we assume that you have already a working `C++` compiler compatible with CosmoLattice. The main requirement is for it to fully support the `C++14` standard, which is the case of all relatively modern compilers (see Table \ref{tab:requirements} for which ones are). We also assume that you have a working installation of `make`, `CMake` as well as a compatible `MPI` installation. These tools and libraries are very standard and easy to install. For completeness, we explain a way of installing these in Box \ref{Box:basicinstall}.

### `fftw3`
#### Serial Version: All you Need for CosmoLattice without Parallelization

The first installation we will go through is the one of `fftw3`. It is very likely that you have some version of it already installed on your laptop, as many applications rely on it. However, it is also probable that such installation is not complete enough to work with CosmoLattice. For example, the installation required by CosmoLattice needs to have been compiled both for `double` and `float` precision. The easiest way of achieving this is to install `fftw3` from the source. To simplify the user's life, we provide a script which does that (almost) automatically; we will explain now how to use it. If you want to do it by yourself, you can skip to Box \ref{Box:libsinstall}, where all the appropriate compilation flags for the external libraries are summarized. If you do this and choose a ''local" installation, do not forget to specify the path where you install `fftw3` to the CosmoLattice `CMake`, as explained in Section [*My first run*][sec_MyFirstRun].

In the root folder of CosmoLattice, there is a folder called `dependencies`, which contains different scripts that help with the installation of external libraries. The script `fftw3.sh` deals with the installation of `fftw3`. We will install the library locally inside `dependencies`, in a new subfolder which we will call `MyFFTW3`. To do this, we simply need to run `fftw3.sh` with `MyFFTW3` as an argument.
```bash
cd dependencies
bash fftw3.sh MyFFTW3
```

The lines above will download, unpack, compile and install `fftw3`. The compilation may take up to several minutes to complete. An advantage of using this script is that it automatically provides `CMake` with the correct path to the newly installed library, by writing it in `src/cmake/auto_install_paths.txt`. If the installation has gone well, everything is ready to run CosmoLattice in serial mode, as explained in Section [*My first run*][sec_MyFirstRun].

However, it is possible that the script does not manage to download the source. In this case, you can open the script and correct the url, by checking what the current one is at \url{http://www.fftw.org/download.html}. You can also create the directory `MyFFTW3` folder, download the archive yourself, put it there, and then run the `fftw3.sh` script.

`dependencies/fftw3.sh:` \inputminted[ firstline=8, lastline=10,linenos, frame=single]{bash}{code_files/fftw3.sh}
The variable \mintinline{bash}{CURURL} contains the current URL. The variable \mintinline{bash}{CURNAME} should match the name of the folder extracted from the archive, as should the variable \mintinline{bash}{CURFOLDER} (but without double quotes). It is important not to add spaces around the equal signs.

If you cannot make this script work, you can proceed to Box \ref{Box:libsinstall}, where we give instruction on how to compile all the libraries without the automated scripts.

#### Parallel Version: All you Need for CosmoLattice with Parallelization in One Direction

Assuming you have a working installation of `MPI`, the installation of the parallel version of `fftw3` proceeds exactly in the same way by using the same script. You simply need to pass an extra argument that specifies that you want the parallel installation:
```bash
cd dependencies
bash fftw3.sh MyFFTW3 --parallel
```
We also provide relevant information for the installation of the parallel version of `fftw3` in  Box \ref{Box:libsinstall}.

### `PFFT`: Required for Parallelization in $(N-1)$ Directions

If you want to perform simulations parallelized in $N-1$ directions (e.g. two for three dimensional simulations), you need to install the extra library `PFFT` [@Pi13]. You need to do this after installing the parallel `fftw3`, as `PFFT` is based on it. To do so, we also provide an automated script `pfft.sh` that installs it, which is also located in the `dependencies` folder. It takes two arguments. First, the path where you want it to be installed; which in this example will be in a subfolder called `MyPFFT` inside `dependencies`. Second, the folder where the parallel `fftw3` is located; `MyFFTW3` in this example.
```bash
cd dependencies
bash pfft.sh MyPFFT MyFFTW3
```
For the sake of compatibility and stability, we use a specific version of `PFFT`: version `1.0.8.alpha`, which is hosted at \url{https://www-user.tu-chemnitz.de/ potts/workgroup/pippig/software.php.en#pfft}. Also in this case, the script writes the path of this local installation  in the file `src/cmake/auto_install_paths.txt`. Again, instructions about how to manually install it are provided in Box \ref{Box:libsinstall}.

### `HDF5`: To Backup your Simulations and Save $N$ Dimensional Field Distributions

In order to save $N$ dimensional field distributions, we use the external library `HDF5`, which provides a standardized binary format which can be read by most data analysis tools/languages. To install it, you can use the script `hdf5.sh` provided in `dependencies`. This library needs to be compiled separately for serial and parallel usage. Contrary to `fftw3`, the serial and parallel version of `HDF5` are mutually incompatible, so if we want to use both of them, we will need to install them in separate folders. This is done using the `hdf5.sh` script as follows. The script first takes as an argument   the location where we want it to be installed, and second, an optional `--parallel` flag to specify we want the parallel version.
```bash
cd dependencies
bash hdf5.sh MyHDF5 # Only if you also want the serial version
bash hdf5.sh MyHDF5 --parallel # Only if you also want the parallel version
```
As for the other scripts, the relevant path is directly set in the CosmoLattice `CMake`, in the file `src/cmake/auto_install_paths.txt`. In this case, if you install both the serial and parallel versions, it is important you comment out in this file the version you do not need when compiling CosmoLattice. Namely, if you want to compile CosmoLattice in serial mode, you should comment out the link to the parallel `HDF5` version and vice versa. The same is true if you install the library following the instructions given in Box \ref{Box:libsinstall}; to avoid conflict, `CMake` should be aware only of one `HDF5` version. Also, as in the case of `fftw3`, if the script has trouble downloading the archive, you can open the script and correct the url, by checking what the current one is at \url{https://www.hdfgroup.org/downloads/hdf5/}. You can also create the directory `MyHDF5` (or MyHDF5Parallel), download the archive yourself, put it there and then run the `hdf5.sh` script.

`dependencies/hdf5.sh:` \inputminted[ firstline=7, lastline=9,linenos, frame=single,breaklines=true]{bash}{code_files/hdf5.sh}
Again, the variable \mintinline{bash}{CURURL} contains the current URL. The variable \mintinline{bash}{CURNAME} should match the name of the folder extracted from the archive, as should the variable \mintinline{bash}{CURFOLDER}, but without double quotes. Its important not to add spaces around the equal signs.

### All of It
In case you want all of the libraries installed automatically, you can also run the `fetchall.sh` script in dependencies. It takes a single argument, the folder where you want the libraries to be installed, and an optional argument equal to `--no-pfft` or `--no-hdf5` if you want the script not to install `PFFT` or `HDF5`. It detects whether or not you have `MPI` installed. If so, it install the parallel version of the libraries. Otherwise, it asks you whether you want to proceed with the serial installation or not. The following execution would install all the libraries in a `MyLibs` folder inside `dependencies`.
```bash
cd dependencies
bash fetchall.sh MyLibs
```

### Installing CosmoLattice on a HPC Cluster

By its nature, CosmoLattice is intended to be used to run parallel simulations on many cores, so it is perfectly suited to be used on High-Performance Computing (HPC) Clusters. As every single HPC cluster is different, it is impossible to write a generic explanation of how to install CosmoLattice on a cluster. We still provide the user with some general guidelines and information about commonly encountered features. If you encounter trouble using a specific cluster, or you are missing some libraries/tools, you should directly contact the IT team maintaining it.

Often, when you connect to a cluster, you do not have access to any libraries by default; you need to first specify which ones you want to use from a predefined list of installed libraries. A common way of implementing this is through the use of Environment Modules. If this is the case of the cluster you are using, you will have access to the `module` command. In that case, you can see what libraries/tools are available by calling it with the `list` argument:
```bash
module list
```

This will show you a list of available packages, possibly with different versions. In order to run CosmoLattice, you need appropriate version for the compiler, `CMake` and `MPI` distribution (see Table  \ref{tab:requirements}). If this is not the case, you should contact your IT team. You can also expect to have `fftw3` available, both serial and parallel, and similarly for `HDF5`. If this is not the case, you can either ask for them to be installed (easiest option), or install them locally as explained in the previous sections (likely fastest option). You will need the parallel `fftw3` anyhow, but you will want to have `HDF5` installed only if you use such feature.

In any case, `PFFT` will not be installed by default and you will have to install it locally as explained above. If you do not have direct access to the internet from your cluster, you can download the archive somewhere else and upload it to the cluster, in the folder where you want to install the library. As explained above, you need to know where the appropriate version of `fftw3` is located in order to perform the installation of `PFFT`, which may not be clear if you use the version installed on the cluster. A useful trick to find its path is to run the CosmoLattice `CMake` with the `MPI=ON` and `PFFT=OFF` options. The `CMake` will automatically locate `fftw3`, and you will be able to recover the appropriate path by opening the generated `CMakeCache.txt` file. By looking for instance at `FFTW_LIB`, you will see where the `fftw3` libraries are located. The path you want is the full path before the `lib` folder: for instance, if you see `/user/home/johndoe/fftw/lib/`, you must only give `/user/home/johndoe/fftw/`) to the `pfft.sh` script.

If your cluster is actually using Environment Modules, you can load the appropriate libraries/tools by using the `load` option of the `module` command. In that case, you will typically need to type something along the following lines before compiling CosmoLattice,
```bash
module list
module load CMakeName
module load CompilerName
module load MPIName
module load FFTW3Name
module load HDF5Name # Only if you need it
```
where `CMakeName, CompilerName, MPIName, FFTW3Name` and `HDF5Name` refer to the appropriate name returned by `module list`. Of course, you should not load `fftw3` or `hdf5` if you have loaded them locally.

At this point, you should be able to compile and run CosmoLattice with as many cores as you want.

\begin{framed}
*Installing a* `C++` *compiler,* `make`, `CMake`, `MPI` and `git` on your PC
\bigskip

**Ubuntu:** Ubuntu comes by default with the `apt-get` package manager, which makes the installation of all basic utilities easy. The following lines should be enough to install what you need
```bash
sudo apt-get install make
sudo apt-get install g++
sudo apt-get install cmake
sudo apt-get install openmpi
sudo apt-get install git
```
The `sudo` command is necessary to give you the admin rights, which you need to have to install software ''globally" on your PC.

**Fedora:** On Fedora, you can use `dnf` as a default package manager. Then the command are the same than on Ubuntu.
```bash
sudo dnf install make
sudo dnf install g++
sudo dnf install cmake
sudo dnf install openmpi-devel
sudo dnf install git
```

By default, `dnf` does not install `openMPI` somewhere which is globally accessible. To fix that, go to your `home` folder and edit or create the `.bashrc`
```bash
cd
res='find /usr -name "mpirun"' #finds where openMPI was installed
echo 'export PATH=$PATH:'"${res
source  /.bashrc # reload it
```

**macOS:** The first time you want to do something related to coding on your mac, you need to start by opening a terminal and run:
```bash
xcode-select --install
```
This will enable your command line to be used to code and install some basic utilities. Then, to install the remaining missing software, it will be convenient to first install a package manager, which does not come by default on mac. Here we will use `Homebrew`, which can be installed as:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" ``` Once this is done, remaining packages can simply be installed as ```bash brew install gcc brew install open-mpi brew install cmake brew install git ``` \end{framed} \captionof{boxfloat}{Summary of how to install basic utilities on a PC to code in \texttt{C++}.}   \begin{framed} \textit{Installing the external libraries without CosmoLattice installation scripts} \bigskip  \texttt{fftw3}:  First, download the the source code (\url{http://www.fftw.org/download.html}) and extract the archive. Inside the extracted folder, which we will refer to as \texttt{fftw-3}, do the following:  ```bash cd fftw-3  ./configure --prefix=/path/where/to/install/ --enable-threads --enable-sse2 --enable-avx --disable-shared --enable-static make -j make install  ```  \texttt{fftw3}, parallel:  First, download the source code and extract the archive (it is the same than the ''normal" \texttt{fftw3}, so maybe you already did it). Inside the extracted folder, which we will refer to as \texttt{fftw-3}, do the following: ```bash cd fftw-3  ./configure --prefix=/path/where/to/install/ --enable-threads --enable-sse2 --enable-avx --disable-shared --enable-static --enable-mpi make -j make install  ``` With this, you should have a functional \texttt{fftw3} installation to work with CosmoLattice.  \texttt{PFFT}:  Download version \texttt{1.0.8-alpha} from \url{https://www-user.tu-chemnitz.de/ potts/workgroup/pippig/software.php.en#pfft}.  ```bash cd pfft-1.0.8-alpha  FFTWPATH=/path/where/fftw3/is/installed/  export LDFLAGS="-L${FFTWPATH}/lib"
export DYLD_LIBRARY_PATH="${FFTWPATH}/lib" export LIBS="-lfftw3_mpi -lfftw3" export CXX=mpic++ export CC=mpicc export CFLAGS="-g3"  ./configure --prefix=/path/where/to/install/ --with-fftw3=${FFTWPATH} --disable-fortran --disable-shared --enable-static
make -j
make install
```

`hdf5`:

First, download the source code (\url{https://www.hdfgroup.org/downloads/hdf5/}) and extract the archive. Inside the extracted folder, which we will refer to as `hdf5`, do the following:
```bash
cd hdf5

export CC=gcc
./configure --prefix=/path/where/to/install/
make -j
make install
```

`hdf5`, parallel:

First, download the the source code and extract the archive (it is the same than the ''normal" `HDF5`, so maybe you already did it). Inside the extracted folder, which we will refer to as `hdf5Parallel`, do the following:
```bash
cd hdf5

export CC=mpicc
./configure --prefix=/path/where/to/install/ --enable-parallel
make -j
make install
```
If you want to have both serial and parallel `HDF5`, it is important you do not install them in the same folder.

\end{framed}

\captionof{boxfloat}{Installation of the external libraries without using the automated scripts provided with CosmoLattice. As explained in the main text, while `fftw3` and its parallel version can perfectly be installed in the same place, one has to be careful to install `hdf5` and its parallel version in separate folders. Also, we are assuming here that you want to compile the libraries with `gcc`. If you want to use another compiler, replace the `CC` and `CXX` flags appropriately.}

#### Required Tools

| Name  | Minimal Version |     | Notes |
| ----- | --------------- | --- | ----- |
| make  | -               |     |       |
| CMake | 3.0             |     |       |

#### Required Compiler (one of the following)

| Name    | Minimal Version |     | Notes                       |
| ------- | --------------- | --- | --------------------------- |
| g++     | 5.0             |     | Minimal version tested: 5.5 |
| clang++ | 3.4             |     | Minimal version tested: 3.9 |

#### Required Libraries

| Name  | Minimal Version |     | Notes                          |
| ----- | --------------- | --- | ------------------------------ |
| fftw3 | 3               |     | Minimal version tested: 3.3.6. |

#### Optional Libraries 

| Name    <div style="width:150px"> | Minimal Version | Extra Features                                                                                                | Notes                                                                                                                                                                                                                                                       |
| --------------------------------- | --------------- | ------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| MPI                               | -               | Parallelization                                                                                               | Needs an implementation of `MPI` compatible with the compiler you chose. The most common open-source ones are `OpenMPI` and `MVAPICH` (for `g++` and `clang++`). Works only with one of the parallel Fourier transforms libraries, see below and main text. |
| fftw3, mpi version                | 3               | Parallelization in one dimension                                                                              | `fftw3` compiled for parallel use. Allows only for parallelization in one dimension.                                                                                                                                                                        |
| PFFT                              | -               | Parallelization in $n-1$ dimensions.                                                                          | External library based on the parallel `fftw3` library. Needs the parallel `fftw3`.                                                                                                                                                                         |
| HDF5                              | 5               | Saving of 3D distributions to file. Saving of whole simulations. Restarting simulations and automatic backup. | Needs to be separately compiled to work in parallel, see bulk text.                                                                                                                                                                                         |

#### Optional Tools

| Name | Minimal Version | Extra Features                                                    | Notes |
| ---- | --------------- | ----------------------------------------------------------------- | ----- |
| git  | -               | Easy access to the code and easy way to update your code version. |       |

