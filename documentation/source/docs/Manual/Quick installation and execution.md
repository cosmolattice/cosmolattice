Here, we provide a brief description of the steps to download, compile and run a job with CosmoLattice. As an example, we load a model called `lphi4`, with input parameters specified in the file `lphi4.in` (details of this model and of its parameters can be found in Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md), here we just use these files as a demonstration example). 

An appropriate version of FFTW needs to be installed in order to handle the code's Fourier transforms, either for serial jobs run on a single processor, or for parallelized jobs run on multiple processors. For serial jobs, that is all it is needed. For parallelized jobs, in addition, the appropriate MPI compiler needs also to be installed. 
Optionally, if one wants to print two- or three-dimensional snapshots of a simulation, or simply to stop/restart a run at a certain time, the HDF5 library also needs to be installed. 
Also optionally, the simulations can be parallelized in more than one spatial dimension, to speed up jobs of long duration or to run massive simulations. This requires the library PFFT (parallel Fast Fourier transforms) [@Pi13] to be installed. For further details on the installation and use of the code and the libraries, please see Appendix [Installation](Installation.md).

**Requirements:** `CMake` v3.0 (or above), `fftw3`, `g++` v$5.0$ (or above) or `clang++` v$3.4$ (or above). For parallel use: `MPI`. *Optional*: `HDF5` and `PFFT`.

**Download:** You can download CosmoLattice from [ http://www.cosmolattice.net](http://www.cosmolattice.net) , or alternatively use `git clone` as indicated below.

**Personal computer:**
```bash
git clone https://github.com/cosmolattice/cosmolattice
cd cosmolattice                  # Enter into main code folder
mkdir build                      # Create a new directory
cd build                         # and go inside it.
cmake -DMODEL=lphi4 ../          # Selects model phi^4 for serial runs
# OR
cmake -DMODEL=lphi4 -DMPI=ON ../ # Selects model phi^4 and activates parallelization
make cosmolattice                # Compiling
./lphi4 input=../src/models/parameter-files/lphi4.in # Executes serial run (input parameter file 'lphi4.in')
# OR
mpirun -n 8 lphi4 input=../src/models/parameter-files/lphi4.in # Parallelized run on 8 cores (input file 'lphi4.in')
```

**High-Performance cluster:** Assuming it uses Environment Modules, otherwise one needs to see how to load the required libraries.
```bash
git clone https://github.com/cosmolattice/cosmolattice
cd cosmolattice   # Enter into main code folder
mkdir build       # Create a new directory
cd build          # and go inside it.
module list       # Displays down the names of the libraries you need.
module load CMAKE # Here we will call C++, FFTW3, CMAKE and MPI.
module load C++   # The order is important, C++ before MPI before FFTW3
module load MPI   # Needed to run parallelized simulations
module load FFTW3 # Needed for Fourier transforms in parallelized simulations

cmake -DMODEL=lphi4 ../          # Selects model phi^4 for serial runs
# OR
cmake -DMODEL=lphi4 -DMPI=ON ../ # Selects model phi^4 and activates parallelization
make cosmolattice                # Compiling
# Now you can run lphi4. How you do it depends on the cluster.
```

*Optional:* Install `PFFT` and simply add the flag `-DPFFT=ON` when you call `CMake`:
```bash
cmake -DMODEL=lphi4 -DMPI=ON -DPFFT=ON ../ # For parallel use
```

*Optional:* Install `PFFT` and simply add the flag `-DPFFT=ON` when you call `CMake`:
```bash
cmake -DMODEL=lphi4 -DHDF5=ON ../          # For serial use
cmake -DMODEL=lphi4 -DMPI=ON -DHDF5=ON ../ # For parallel use (also works with PFFT)
```

**Installing `fftw3`, `PFFT` and `HDF5`**:
```bash
cd .../dependencies/
bash fetchall.sh MyLibs # Install everything. You are done if you do that.
bash fftw3.sh MyFFTW3 --parallel # Install only fftw3 in MyFFTW3. Can remove --parallel.
bash pfft.sh MyPFFT MyFFTW3 # Install only PFFT in MyPFFT. MyFFTW3 is the path to fftw3.
bash hdf5.sh MyHDF5 --parallel   # Install only hdf5 in MyHDF5. Can remove --parallel.
```

**Note:** Some of the above commands are only indicative, as they can change from machine to machine. For more detailed explanations about the installation of CosmoLattice and the libraries it uses (or can use), see Appendix [Installation](Installation.md).

