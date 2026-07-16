<!-- ** Theory Monographs Guide. **  -->

The theoretical basis for the equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ are distributed in a series of monographs on lattice-cosmology techniques that we have written over the years, under the common title *"The art of simulating the early Universe"*. The series has currently (at the time of writing, July 2026) three parts: 

*Part I. Integration techniques and canonical cases.* (Ref. [@Figueroa_2020rrl])

*Part II. Non-canonical cases and gravitational waves.* (Ref. [@BaezaBallesteros_2025tme]) 

*Part III. Scalar-Gauge-Fluid dynamics.* (Ref. [@Figueroa_2026XYZ]) 

which we often refer to, colloquially, as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$, and $\mathtt{The~Art-III}$ monographs.

These reviews provide comprehensive discussions of lattice methods for scalar–gauge field theories in an expanding universe ($\mathtt{The~Art-I}$ [@Figueroa_2020rrl]), gravitational waves and non-canonical aspects of field theory - including interactions, initial conditions, dimensionality - ($\mathtt{The~Art-II}$ [@Baeza-Ballesteros:2025tme]), and fluid dynamics as a description of an ensemble of interacting scalar, gauge, and fermion particles ($\mathtt{The~Art-III}$ [@Figueroa_2026XYZ]). 

<!--
  space
-->

----------

<!--
  space
-->

<!-- [](){ #the_Art_I } -->
## $\mathtt{The~Art-I}.~$  { #subsec_ArtII }
<p style="margin-top:-2.2em; margin-left:6.5em; font-size:30px; font-weight:bold;">
Integration techniques and canonical cases
</p>

Download $\mathtt{The~Art-I}$ from: ArXiv-Link or as a PDF 

This monograph presents a comprehensive discussion of lattice techniques for simulating scalar and gauge field dynamics in an expanding universe. After reviewing the continuum formulation of scalar and gauge field interactions in both Minkowski and FLRW backgrounds, we introduce the fundamental tools for the discretization of field theories, including **lattice gauge-invariant** techniques.

We then discuss and classify numerical algorithms, ranging from methods with $\mathcal{O}(\delta t^2)$ accuracy, such as *staggered leapfrog* and *Verlet integration*, to *Runge--Kutta* methods with accuracy up to $\mathcal{O}(\delta t^4)$, as well as the higher-order *Yoshida* and *Gauss--Legendre* integrators, achieving accuracies up to $\mathcal{O}(\delta t^{10})$.

We adapt these methods for classical lattice simulations of the nonlinear dynamics of scalar and gauge fields on an expanding $3+1$-dimensional grid, including the case of **self-consistent** expansion sourced by the volume-averaged energy and pressure densities of the fields. We present lattice formulations for three canonical classes of theories:

1. *Interacting scalar fields*,
2. *Abelian* $U(1)$ *gauge theories*,
3. *Non-Abelian* $SU(2)$ *gauge theories*.

For each case, we build symplectic integrators with accuracies ranging from $\mathcal{O}(\delta t^2)$ to $\mathcal{O}(\delta t^{10})$. We also derive lattice expressions for the relevant observables, including the energy density components, field power spectra, and the Hubble constraint. Remarkably, all of our algorithms for gauge theories preserve the Gauss constraint to machine precision, even when self-consistent expansion is taken into account. As a numerical illustration, we analyze the post-inflationary dynamics of an oscillating inflaton charged under $SU(2)\times U(1)$.

This monograph serves as the theoretical foundation of $\texttt{\mathcal{C}osmo\mathcal{L}attice}~{\tt v1.0}$.

<!--
  space
-->

----------

<!--
  space
-->

## $\mathtt{The~Art-II}$: pepe { #subsec_ArtII }

<!--
  space
-->

----------

<!--
  space
-->


## $\mathtt{The~Art-III}$: pepe { #subsec_ArtIII }