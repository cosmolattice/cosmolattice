# CosmoLattice Version Guide

<span style="font-size: 34px;">**Versions**</span>

A brief summary of the code versions is provided below: 

<!-- ### **Version 1.0** { #subsec_CLv1p0 } -->
<!-- **Version 1.0**  -->
## **Version 1.0** <!-- { #subsec_CLv1p0 } -->

Original release of the code on Feb. 2nd 2021. Written in ${\tt C++}$ and based on {\tt MPI}, it used discrete Fourier transforms parallelized either in one (FFTw3) or multiple (PFFT) spatial dimensions. It could already simulate any model characterized by a scalar potential and a set of scalar fields, either singlets, or local $U(1)$ and/or $SU(2)$ charged scalar fields interacting with the corresponding Abelian and/or non-Abelian gauge fields. It introduces its own symbolic language, defining field variables and operations over them, so that one can introduce differential equations and operators on the lattice, written in a manner that resembles as much as possible the continuum formulation. It included a library of symplectic evolution algorithms for either scalar-scalar and scalar-gauge interactions, presenting $\mathcal{O}(\delta t^2)$ LeapFrog and Velocity Verlet methods, as well as Yoshida methods ranging from $\mathcal{O}(\delta t^4)$ to $\mathcal{O}(\delta t^{10})$. All algorithms were suitable for simulating field theories in an expanding grid, including the case of "self-consistent" expansion sourced by the fields themselves. Relevant observables were provided for each algorithm (e.g.~energy densities, field spectra, lattice snapshots) and remarkably, in the case of scalar-gauge interactions, the algorithms respect the Gauss constraint to machine precision, thanks to the use of lattice gauge-invariant techniques. 

## **Version 1.1** <!-- { #subsec_CLv1p1 } -->
<!-- **Version 1.1** -->

Released on May 6th 2022, this new version added a gravitational wave (GW) module for (singlet) scalar fields. The main features and bug corrections that version 1.1 added with respect to version 1.0.

**New features:**

- Gravitational waves sourced by singlet scalar sectors.
- New ways of computing the spectra (Type I and Type II, versions 1, 2, and 3). The new default way (Type II, version 1) improved the ultraviolet UV reconstruction of spectra.

**Bug corrections:**

- Corrected a wrong normalization factor in the occupation number.
- The SU(2) magnetic power spectrum was outputing NaN instead of machine precision for extremely small amplitudes.
- The initial conditions for the Leapfrog algorithm and the Velocity Verlet one did not conrrespond to the same physical time.
- The last spectra bin was printed twice.
- The 3D hdf5 output was ot working on some architectures (mostly linux based). Special thanks to Chenhuan Wang for pointing this out!
- When using a random seed, every process had a different random seed. This is not a problem per se but makes it impossible to reproduce the simulation with the single seed printed in the info file. Now all the process have the same random seed. Thanks to Jorge Baeza-Ballesteros for pointing this out! 

## **Version 1.2** <!-- { #subsec_CLv1p2 } -->

## **Version 1.3** <!-- { #subsec_CLv1p3 } -->

## **Version 2.0** <!-- { #subsec_CLv2p0 } -->
