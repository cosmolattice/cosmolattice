<!-- <div style="text-align: justify;"> -->

We are usually interested in studying the dynamics of a given scenario in three spatial dimensions. However, there are situations in which this is not feasible due to lack of computational power, so one might need to reduce the dimensionality of the simulations. For such purpose, the code allows to solve the equations of motion of a scalar field sector in both $1+1$ dimensions and $2+1$ dimensions. More specifically, the code does solve the three-dimensional equations of motion of the scalar field sector but on either a $1+1$ or $2+1$ dimensional spatial slice. In this set-up, the discrete Laplacian operator at a given lattice site is only sourced by two and four neighbor points respectively. By modifying appropriately the initial spectrum of fluctuations, these simulations mimic the three-dimensional dynamics, see Section 7 of 2512.15627 for a more extensive discussion.

###  { How to run a 1+1 or 2+1 dimensional model }

For running a scalar field model in either 1+1 or 2+1 dimensions, you just need to add in the model file (e.g. `lphi4.h`) the following:

 `static constexpr size_t NDim = 1;`

 or 
 
 `static constexpr size_t NDim = 2;`
 

This must be added together with the number of fields of each species and the number of potential terms.

**Note:** For now, simulations in 1+1 or 2+1 dimensions only work with `PS_Type=2` spectra.

