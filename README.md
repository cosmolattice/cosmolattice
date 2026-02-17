
# CosmoLattice

## *A modern code for lattice simulations of scalar and gauge field dynamics in an expanding universe*
### Authors: Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg

### Documentation

- Please visit the official webpage for CosmoLattice at [cosmolattice.net](https://cosmolattice.net).
- To learn how to install and execute the code as well as how it works :  <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" > arXiv:2102.01031</a> .
- To learn about the underlying theoretical framework: <a href=https://arxiv.org/pdf/2006.15122.pdf target="_blank" rel="noopener noreferrer" > arXiv:2006.15122</a> .

### Basic installation

*Minimal requirements:* 
- `CMake` version 3.10 or above
- `g++` version 8 or above
- `fftw3`

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
For further information, see  Appendix A of the <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" >user-manual</a>.
All options of CosmoLattice, as well as how to activate them and how to install the optional external libraries are explained at length there.

### Offline compilation (Kokkos)

To compile an application to be run on a different architecture, you can directly pass the target architecture to Kokkos. For a list of supported architectures, see [the Kokkos documentation](https://kokkos.org/kokkos-core-wiki/get-started/configuration-guide.html#gpu-architectures). For example, for an RTX 4070, you would pass 
```bash
cmake -DMODEL=lphi4 -DKokkos_ARCH_ADA89 ../
```
If no architecture is specified, Kokkos will attempt to detect the architecture of the machine. However, as compilation for GPU can take up to an hour, it is recommended on a cluster to queue the compilation separately on a node without a GPU, which requires offline compilation as described here.

### Credits

CosmoLattice is freely available to anyone who wants to use or modify it. However, whenever 
using CosmoLattice in your research, no matter how much (or little) you modify the code, 
<b>please cite both <a href=https://arxiv.org/pdf/2006.15122.pdf target="_blank" rel="noopener noreferrer" > arXiv:2006.15122</a> 
and <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" > arXiv:2102.01031</a> in your papers</b>. 
