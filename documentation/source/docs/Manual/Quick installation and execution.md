Here, we provide a brief description of the steps to download, compile and run a job with CosmoLattice. As an example, we load a model called `lphi4`, with input parameters specified in the file `lphi4.in` (details of this model and of its parameters can be found in Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md), here we just use these files as a demonstration example). 

An appropriate version of FFTW needs to be installed in order to handle the code's Fourier transforms, either for serial jobs run on a single processor, or for parallelized jobs run on multiple processors. For serial jobs, that is all that is needed. For parallelized jobs, in addition, the appropriate MPI compiler needs also to be installed. 
Optionally, if one wants to print two- or three-dimensional snapshots of a simulation, or simply to stop/restart a run at a certain time, the HDF5 library also needs to be installed. 
Also optionally, the simulations can be parallelized in more than one spatial dimension, to speed up jobs of long duration or to run massive simulations. This is handled by the ParaFaFT library (parallel Fast Fourier transforms), which is automatically built and enabled when the code is compiled with MPI. For further details on the installation and use of the code and the libraries, please see [Installation](../code/Installation.md).

<div class="cl-facts cl-prereqs" markdown>
[CMake &ge; 3.16](../code/Installation.md#subsec_requirements){ .cl-fact }
[fftw3](../code/Installation.md#subsec_requirements){ .cl-fact }
[g++ &ge; 10.1](../code/Installation.md#subsec_requirements){ .cl-fact }
[clang++ &ge; 13](../code/Installation.md#subsec_requirements){ .cl-fact }
[MPI — parallel](../code/Installation.md#subsec_requirements){ .cl-fact }
[HDF5 — optional](../code/Installation.md#subsec_requirements){ .cl-fact }
</div>

**Download:** You can download CosmoLattice from [http://www.cosmolattice.net](http://www.cosmolattice.net), or alternatively use `git clone` as indicated below.

<div class="cl-hero" markdown>

=== "Personal computer"

    ```bash
    git clone https://github.com/cosmolattice/cosmolattice
    cd cosmolattice                  # Enter into main code folder
    mkdir build                      # Create a new directory
    cd build                         # and go inside it.
    cmake -DMODEL=lphi4 ../          # Selects model phi^4
    # OR
    cmake -DMODEL=lphi4 -DMPI=ON ../ # Selects model phi^4 and activates distributed parallelization
    make cosmolattice                # Compiling
    ./lphi4 input=../models/parameter-files/lphi4.in # Executes a run (input parameter file 'lphi4.in')
    # OR
    mpirun -n 8 lphi4 input=../models/parameter-files/lphi4.in # Parallelized run on 8 MPI ranks (input file 'lphi4.in')
    ```

=== "High-Performance cluster"

    Assuming it uses Environment Modules, otherwise one needs to see how to load the required libraries.
    ```bash
    git clone https://github.com/cosmolattice/cosmolattice
    cd cosmolattice   # Enter into main code folder
    mkdir build       # Create a new directory
    cd build          # and go inside it.
    module load CMAKE # Here we will call C++, FFTW3, CMAKE and MPI.
    module load C++   # The order is important, C++ before MPI before FFTW3
    module load MPI   # Needed to run parallelized simulations
    module load FFTW3 # Needed for Fourier transforms in parallelized simulations

    cmake -DMODEL=lphi4 ../          # Selects model phi^4 for serial runs
    # OR
    cmake -DMODEL=lphi4 -DMPI=ON ../ # Selects model phi^4 and activates distributed parallelization
    make cosmolattice                # Compiling
    # Now you can run lphi4. How you do it depends on the cluster.
    ```
    You will need to replace the CMAKE/C++/MPI/FFTW3 module names with the ones used in your cluster. These can be usually found by typing `module spider <library name>` in the command line. You can also check the cluster's documentation for more information.

</div>

*Optional:* Install `HDF5` and simply add the flag `-DHDF5=ON` when you call `CMake`:
```bash
cmake -DMODEL=lphi4 -DHDF5=ON ../          # For serial use
cmake -DMODEL=lphi4 -DMPI=ON -DHDF5=ON ../ # For parallel use (also works with ParaFaFT)
```

**Installing `fftw3` and `HDF5`:** These libraries can be installed with your system's package manager, or built automatically by CosmoLattice by adding the corresponding flag when you call `CMake`:
```bash
cmake -DMODEL=lphi4 -DAUTOBUILD_FFTW=ON ../           # Build fftw3 automatically
cmake -DMODEL=lphi4 -DHDF5=ON -DAUTOBUILD_HDF5=ON ../ # Build HDF5 automatically
```

!!! note

    Some of the above commands are only indicative, as they can change from machine to machine. For more detailed explanations about the installation of CosmoLattice and the libraries it uses (or can use), see [Installation](../code/Installation.md).
