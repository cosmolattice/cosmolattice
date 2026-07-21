# $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Version Guide

<!-- <span style="font-size: 34px;">**Versions**</span> -->

A brief summary of the code versions, listing new features and bug corrections with respect previous versions, is provided below: 

## **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 2.X** { #subsec_CLv2pX }

<br>
[](){ #subsec_CLv2p0 }
<span style="font-size: 31px;">**Version 2.0**</span>

Latest version of the code, released on July 27th 2026. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}~\mathtt{v2.0}$ incorporates a significant number of new features compared to the previous [${\tt versions~ 1.X}$][subsec_CLv1pX]. Version 2.0 of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ includes lattice implementations of non-canonical interactions, like scalars with a non-minimal coupling to gravity, $\phi^2R$, or axion-like particle (ALP) interactions with gauge fields, $\phi F_{\mu\nu}\tilde F^{\mu\nu}$. It incorporates also methods to set up special field configurations, like cosmic defect networks close to *scaling* (*e.g.* cosmic strings and domain walls), or arbitrary field power spectra or spatial profiles. In addition, version 2.0 implementes non-symplectic evolution algorithms (suitable *e.g.* for non-minimal scalar kinetic theories of the type $\mathcal{G}_{ab}(\lbrace\phi_c\rbrace)\partial_\mu\phi^a\partial^\mu\phi^b$,), an optimized implementations of the GW dynamics on the lattice sourced by scalar and gauge fields, and the possibility to simulate scalar field dynamics in $d + 1$ dimensions with $d = 1, 2$. Furthermore, users can now run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ $\texttt{v2.0}$ on GPUs, gaining up to a factor $\sim 30$ in rapidity, as compared to CPUs. (<span style="color:orange;">**Jorge comment: Is the claim about spatial profiles? Also the claim about non-minimal kinetic theories seems out of place, as they are not implemented. What is rapidity here?**</span>)

**New features:**

- Simulation of scalar fields with non-minimal couplings to gravity of the form $\xi\phi^2R$.

- A new module for axion-like fields coupled to Abelian gauge fields through interactions of the form $\phi F_{\mu\nu}\widetilde{F}^{\mu\nu}$.

- New initialization procedures for cosmic-string and domain-wall networks, including configurations close to the scaling regime, together with dedicated defect observables.

- Support for scalar-field dynamics in $1+1$ and $2+1$ dimensions.

- Low-storage Runge–Kutta integrators suitable for non-canonical systems, including non-minimally coupled fields and axion–gauge interactions.

- A new lattice algorithm that evolves gravitational waves using five independent degrees of freedom instead of six.

- Support for initializing fields using externally supplied power spectra.

- Support for running simulations on GPUs.

- Combined MPI and OpenMP parallelization.

- Integration of Kokkos and updated parallel Fourier-transform and template libraries.

- Support for single-precision simulations, reducing field-memory requirements by approximately a factor of two.

- Support for outputting spectra without momentum binning.

- Improved lattice snapshots that can include scalar singlet amplitudes, absolute values of complex scalars and scalar doublets, as well as energy-density components.

- Snapshots covering arbitrary lattice subvolumes of size $N_x\times N_y\times N_z$, with the possibility of skipping lattice points at a user-defined interval.

- Backward compatibility with models developed for earlier CosmoLattice versions, requiring only a minimal change to the model file.

**Bug corrections:**

- <span style="color:red;">**Please Add**</span>

- <span style="color:red;">**Please Add**</span>

<br>


## **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 1.X** { #subsec_CLv1pX }

<br>
[](){ #subsec_CLv1p3 }
<span style="font-size: 31px;">**Version 1.3**</span> 

Released on Nov. 2023, this update addressed solely one issue related to memory excess. 

**Bugs detected and corrected:**

- The required RAM memory for a run was not scaling correctly with the number of field degrees of freedom $N_{\rm dof}$, depending on the architecture. After the update, the required RAM memory became roughly $10^{-9}\cdot2\cdot8\cdot N^3\cdot(N_{\rm dof}+3/4)$ [in Gygabytes], with $N$ the number of points/dimension. **NOTE**: This issue became obsolete and irrelevant once [$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}~{\tt v2.0}$][subsec_CLv2p0] was released on July 2026. 


<!--
  space
-->

-----

<!--
  space
-->


<!-- ## **Version 1.2** -->
<br>
[](){ #subsec_CLv1p2 }
<span style="font-size: 31px;">**Version 1.2**</span> 

Released on June 2023, this improved version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ represented mostly an update of the gravitational wave (GW) module, so that GWs sourced by U(1) scalar-gauge theories could be simulated. A number of bugs were also fixed.

**New features:**

- Gravitational waves sourced by U(1) gauge theories.

**Bugs detected and corrected:**

- Wrong counting of the modes used for the computation of the GW power spectra. This affects mainly the results on the lowest and highest bins, for which some modes were counted twice. This problem was pointed out by Toby Opferkuch and Josh Foster.

- Problem in the GW module that prevented the code to be compiled with NDim=2. While the architecture of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is prepared to run in an arbitrary number of spatial dimensions, the GW module is only prepared for NDim=3. An error is now printed if the user tries to simulate GWs for NDim different than 3.

- The code failed to compile when trying to compute a power of a ZeroType field. 

<!--
  space
-->

-----

<!--
  space
-->


<!-- ## **Version 1.1** -->
<!-- **Version 1.1** -->
<br>
[](){ #subsec_CLv1p1 }
<span style="font-size: 31px;">**Version 1.1**</span> 

Released on May 2022, this improved version added a gravitational wave (GW) module for singlet scalar fields. The main features and bug corrections that version 1.1 added with respect to version 1.0., were

**New features:**

- Gravitational waves sourced by singlet scalar sectors.
- New ways of computing the spectra (Type I and Type II, versions 1, 2, and 3). The new default way (Type II, version 1) improved the ultraviolet UV reconstruction of spectra.

**Bugs detected and corrected:**

- Wrong normalization factor in the occupation number of scalar fields.
- NaN outputs for the SU(2) magnetic power spectrum for extremely small amplitudes.
- The initial condition for the Leapfrog (LF) algorithm and the Velocity Verlet (VV2) did not conrrespond to the same physical time.
- The last bin of scalar spectra was printed twice.
- The 3D hdf5 output was not working on some architectures (mostly linux based). This problem was pointed out by Chenhuan Wang.
- Different runs had different random realizations even when using the same seed for the random number generation (this was not a problem by itself, but it made it impossible to reproduce simulations with the same random numbers). This problem was pointed out and corrected by Jorge Baeza-Ballesteros. 

<!-- ### **Version 1.0**  -->
<!-- **Version 1.0**  -->
<!-- ## **Version 1.0** -->
<br>
[](){ #subsec_CLv1p0 }
<span style="font-size: 31px;">**Version 1.0**</span> 

First version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, released on Feb. 2021. Written in ${\tt C++}$ and based on {\tt MPI}, $\mathcal{CL} {\tt 1.0}$ used discrete Fourier transforms parallelized in either one (FFTw3) or multiple (PFFT) spatial dimensions. It could simulate any model characterized by a scalar potential and a set of scalar fields, either singlets, or local $U(1)$ and/or $SU(2)$ charged scalar fields interacting with the corresponding Abelian and/or non-Abelian gauge fields. It introduces its own symbolic language, defining field variables and operations over them, so that one can introduce differential equations and operators on the lattice, written in a manner that resembles as much as possible the continuum formulation. It included a library of symplectic evolution algorithms for either scalar-scalar and scalar-gauge interactions, presenting $\mathcal{O}(\delta t^2)$ LeapFrog and Velocity Verlet methods, as well as Yoshida methods ranging from $\mathcal{O}(\delta t^4)$ to $\mathcal{O}(\delta t^{10})$. All algorithms were suitable for simulating field theories in an expanding grid, including the case of "self-consistent" expansion sourced by the fields themselves. Relevant observables were provided for each algorithm (e.g.~energy densities, field spectra, lattice snapshots) and remarkably, in the case of scalar-gauge interactions, the algorithms respect the Gauss constraint to machine precision, thanks to the use of lattice gauge-invariant techniques. 


