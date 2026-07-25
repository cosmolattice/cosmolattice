<!-- <div style="text-align: justify;"> -->

<!-- We are usually interested in studying the dynamics of a given scenario in three spatial dimensions. However, there are situations in which this is not feasible due to lack of computational power, so one might need to reduce the dimensionality of the simulations. For such purpose, the code allows one to solve the equations of motion of a scalar field sector in both $1+1$ dimensions and $2+1$ dimensions. More specifically, the code does solve the three-dimensional equations of motion of the scalar field sector but on either a $1+1$ or $2+1$ dimensional spatial slice. In this set-up, the discrete Laplacian operator at a given lattice site is only sourced by two and four neighbor points respectively. By modifying appropriately the initial spectrum of fluctuations, these simulations mimic the three-dimensional dynamics, see Section 7 of 2512.15627 for a more extensive discussion. -->

Simulating scalar fields on lattices with $d \neq 3$ spatial dimensions could be useful for two main reasons. First, one might wish to simulate a field theory intrinsically defined in $d \neq 3$ dimensions, with scalar fields propagating, *e.g.* in a $(2+1)$-dimensional expanding spacetime, $ds^2 = a^2(\eta)(-d\eta^2 + \delta_{ij}dx_i dx_j)$, with $i,j=1,2$. In this case, the corresponding EOM in $(2+1)$ dimensions can be discretized and solved using suitable evolution algorithms, in much the same way as for the theories in $(3+1)$ dimensions. Alternatively, one may wish to simulate a field theory defined in three spatial dimensions on a lower-dimensional lattice, thereby approximating its three-dimensional dynamics at a reduced computational cost. This approach is justified provided that the system is statistically isotropic, both in its initial conditions and throughout its evolution. This is typically the case for scalar-field interactions, as they can not create locally (neither globally) a preferred spatial direction, unless that was already imposed as an initial condition in the field gradients. Reducing the dimensionality from $d=3$ to $d=2$ can be therefore highly advantageous, accelerating simulations by a factor $\sim N$, with $N \approx 10^2-10^3$ in most applications. This enables investigations requiring very long evolution times, very large lattices, or extensive scans over model parameters, see *e.g.* Refs. [@Antusch_2020iyq], [@Antusch_2021aiw], [@Antusch_2025ewc]. The ability of lower-dimensional simulations to accurately reproduce three-dimensional dynamics, must be assessed anyways on a case-by-case basis.

In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we have considered the second circumstance, namely simulating $(3+1)$-dimensional physics on a reduced $(d+1)$-dimensional lattice, with $d=2$, or $d=1$. For such purpose, the code allows to solve the singlet scalar field EOM in both $(1+1)$ and $(2+1)$ dimensions. More specifically, the code solves standard EOM (*e.g.* of the form given in Eq. ([*6*][eq_scEOM]) and Eq. ([*9*][eq_sfEOM_II]) in [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md)), but on a $1-$ or $2-$dimensional spatial slice respectively. The discrete Laplacian operator in Eq. ([*6*][eq_scEOM]) from [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md), is then computed summing over the $d$-spatial directions, *i.e.* $\nabla^2 \phi = \sum_{i=1}^{d} \partial^2 \phi / \partial x_i^2$, with $d = 1$ or $2$. A similar change applies to the gradient component of the scalar field's energy density, ${G}_{\phi} = \frac{1}{2 a^2} \sum_{i=1}^d (\partial\phi / \partial x_i)^2$, *c.f.* Eq. ([*10*][eq_KG]) from [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md). By modifying appropriately the initial spectrum of fluctuations, these simulations mimic the three-dimensional dynamics, see Sect. 7.1 of $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]) for more extensive discussions.

### Runnin in d=1,2 spatial dimensions

For running a scalar field model in either 1+1 or 2+1 dimensions, you just need to add in the model file (e.g. `lphi4.h`) the following:

 `static constexpr size_t NDim = 1;`

 or 
 
 `static constexpr size_t NDim = 2;`
 

This must be added together with the number of fields of each species and the number of potential terms.

**Note:** For now, simulations in 1+1 or 2+1 dimensions only work with `PS_type = 2` spectra.

