# CosmoLattice

## *A modern code for lattice simulations of scalar and gauge field dynamics in an expanding universe*
### Authors: Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg

### Documentation

- Please visit the official webpage for CosmoLattice at [cosmolattice.net](https://cosmolattice.net).
- To learn how to install and execute the code as well as how it works :  <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" > arXiv:2102.01031</a> .
- To learn about the underlying theoretical framework: <a href=https://arxiv.org/pdf/2006.15122.pdf target="_blank" rel="noopener noreferrer" > arXiv:2006.15122</a> .

### Basic installation

*Minimal requirements:* `CMake` version 3 or above, `g++` version 5 or above, `fftw3`.

```
git clone https://github.com/cosmolattice/cosmolattice.git
cd cosmolattice   
mkdir build                     
cd build                        
cmake -DMODEL=lphi4 ../
make cosmolattice
```

This will compile the ``lphi4`` model. To run it with the default input file, you can do

``
./lphi4 input=../src/models/parameter-files/lphi4.in
``

The above commands just represent a very brief guide for the installation and execution of CosmoLattice. 
For further information, see  Appendix A of the <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" >user-manual</a>.
All options of CosmoLattice, as well as how to activate them and how to install the optional external 
libraries are explained at length there.

### Credits

CosmoLattice is freely available to anyone who wants to use or modify it. However, whenever 
using CosmoLattice in your research, no matter how much (or little) you modify the code, 
<b>please cite both <a href=https://arxiv.org/pdf/2006.15122.pdf target="_blank" rel="noopener noreferrer" > arXiv:2006.15122</a> 
and <a href=https://arxiv.org/pdf/2102.01031.pdf target="_blank" rel="noopener noreferrer" > arXiv:2102.01031</a> in your papers</b>. 
