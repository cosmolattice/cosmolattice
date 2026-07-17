Content-Disposition: attachment; filename="Art_I.pdf"
Content-Type: ./

<!-- ** Theory Monographs Guide. **  -->

The theoretical basis for the equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ are presented through a series of monographic reviews on **lattice-cosmology** techniques, written over the years under the common title *"The art of simulating the early Universe"*. The series has at the time of writing (July 2026) three parts: 

*Part I. Integration techniques and canonical cases.* (Ref. [@Figueroa_2020rrl])

*Part II. Non-canonical cases and gravitational waves.* (Ref. [@BaezaBallesteros_2025tme]) 

*Part III. Scalar-Gauge-Fluid dynamics.* (Ref. [@Figueroa_2026XYZ]) 

which we often refer to, colloquially, as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$, and $\mathtt{The~Art-III}$ monographs.

These reviews provide comprehensive discussions of lattice-cosmology methods for scalar–gauge field theories in an expanding universe ($\mathtt{The~Art-I}$); gravitational waves and non-canonical aspects of field theory, including interactions, initial conditions, dimensionality ($\mathtt{The~Art-II}$); and fluid dynamics as a description of an ensemble of interacting scalar, gauge, and fermion particles ($\mathtt{The~Art-III}$). 


You can Download the monographs from:

| Article | INSPIRE-HEP | ArXiv Abstract | ArXiv PDF | Local PDF | Publication URL |
|:--------|:-----------:|:--------------:|:---------:|:---------:|:---------------:|
| $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]) | <a href="https://inspirehep.net/literature/1803326" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/abs/2006.15122" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/pdf/2006.15122" target="_blank" rel="noopener noreferrer">File</a> | <a href="./Art_I.pdf" target="_blank" rel="noopener noreferrer">Open</a> | <a href="https://doi.org/10.1088/1475-7516/2021/04/035" target="_blank" rel="noopener noreferrer">Link</a> |
| $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]) | <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">File</a> | <a href="./Art_II.pdf" target="_blank" rel="noopener noreferrer">Open</a> | <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer">Link</a> |
| $\mathtt{The~Art-III}$ (Ref. [@Figueroa_2026XYZ]) | <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">File</a> | <a href="./Art_III.pdf" target="_blank" rel="noopener noreferrer">Open</a> | <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer">Link</a> |

(<span style="color:red;">**CHANGE last row to ART-III **</span>)

<!--
  space
-->

----------

<!--
  space
-->

<!-- [](){ #the_Art_I } -->
## $\mathtt{The~Art-I}$  { #subsec_ArtI }
<p style="margin-top:-2.29em; margin-left:6.58em; font-size:32px; font-weight:bold;">
.  &nbsp; Integration techniques and canonical cases
</p>

You can Download $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]) from: &nbsp; <a href="https://inspirehep.net/literature/1803326" target="_blank" rel="noopener noreferrer">INSPIRE-HEP</a> ;  &nbsp; 
<a href="https://arxiv.org/abs/2006.15122" target="_blank" rel="noopener noreferrer">ArXiv Abstract</a> ; &nbsp; <a href="https://arxiv.org/pdf/2006.15122" target="_blank" rel="noopener noreferrer">ArXiv PDF</a> ; &nbsp;
<a href="./Art_I.pdf" target="_blank" rel="noopener noreferrer" download>Local PDF </a> ; &nbsp; <a href="https://doi.org/10.1088/1475-7516/2021/04/035" target="_blank" rel="noopener noreferrer"> Publication URL</a>


<!-- 
[INSPIRE-HEP](https://inspirehep.net/literature/1803326), [ArXiv](https://arxiv.org/abs/2006.15122)

[INSPIRE-HEP](https://inspirehep.net/literature/1803326), [PDF File](https://arxiv.org/abs/2006.15122),

<a href="./Art_I.pdf" target="_blank" rel="noopener noreferrer">

  Local PDF file
</a>
 -->

This monograph serves as the theoretical foundation of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}~{\tt v1.0}$ (released on Feb. 2021). It contains a comprehensive discussion of lattice techniques for simulating scalar and gauge field dynamics in an expanding universe. After reviewing the continuum formulation of scalar and gauge field interactions in both Minkowski and FLRW backgrounds, we introduce the fundamental tools for the discretization of field theories, including **lattice gauge-invariant** techniques. We then discuss and classify numerical algorithms, ranging from methods with $\mathcal{O}(\delta t^2)$ accuracy, such as *staggered leapfrog* and *Verlet integration*, to *Runge--Kutta* methods with accuracy up to $\mathcal{O}(\delta t^4)$, as well as the higher-order *Yoshida* and *Gauss--Legendre* integrators, achieving accuracies up to $\mathcal{O}(\delta t^{10})$. We adapt these methods for classical lattice simulations of the nonlinear dynamics of scalar and gauge fields on an expanding $3+1$-dimensional grid, including the case of **self-consistent** expansion sourced by the volume-averaged energy and pressure densities of the fields. We present lattice formulations for three canonical classes of theories:

1. *Interacting scalar fields*,
2. *Abelian* $U(1)$ *gauge theories*,
3. *Non-Abelian* $SU(2)$ *gauge theories*.

For each case, we build symplectic integrators with accuracies ranging from $\mathcal{O}(\delta t^2)$ to $\mathcal{O}(\delta t^{10})$. We also derive lattice expressions for the relevant observables, including the energy density components, field power spectra, and the Hubble constraint. Remarkably, all of our algorithms for gauge theories preserve the Gauss constraint to machine precision, even when self-consistent expansion is taken into account. As a numerical illustration, we analyze the post-inflationary dynamics of an oscillating inflaton charged under $SU(2)\times U(1)$.



<!--
  space
-->

----------

<!--
  space
-->

## $\mathtt{The~Art-II}$  { #subsec_ArtII }
<p style="margin-top:-2.29em; margin-left:7.3em; font-size:32px; font-weight:bold;">
.  &nbsp; Non-canonical cases and gravitational waves
</p>

You can Download $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]) from: &nbsp; <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">INSPIRE-HEP</a> ; &nbsp;
<a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv Abstract</a> ; &nbsp;
<a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv PDF</a> ; &nbsp;
<a href="./Art_II.pdf" target="_blank" rel="noopener noreferrer" download>Local PDF </a>; &nbsp; <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer"> Publication URL</a>

This monograph serves as the theoretical foundation for the non-canonical field-theory aspects—including interactions, initial conditions, and dimensionality— and the GW dynamics implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}~{\tt v2.0}$ (released on July 2026). It presents discussions on lattice techniques for the simulation of non-canonical field-theory scenarios, complementing our previous monograph on canonical cases $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]). We begin by reviewing basic aspects of lattice field theory, including symplectic and non-symplectic evolution algorithms. We then introduce lattice implementations of non-canonical interactions, considering 

- Scalars with a non-minimal coupling to gravity: $\phi^2 R$

- Non-minimal scalar kinetic theories: $\mathcal{G}_{ab}\big(\{\phi_c\}\big)\partial_\mu \phi^a \partial^\mu \phi^b$

- Axion-like particle (ALP) interactions with Abelian gauge fields: $\phi F_{\mu\nu}\widetilde{F}^{\mu\nu}$

Next, we discuss methods for setting up special field configurations, including:

- The creation of cosmic-defect networks approaching scaling, *e.g.*, cosmic strings and domain walls
- Field configurations based on arbitrary power spectra or spatial profiles
- Probabilistic methods, as required, *e.g.*, for thermal configurations

We further extend the notion of non-canonical theories by discussing the discretization of scalar-field dynamics in $d+1$ dimensions, with $d\neq 3$. Independently of these non-canonical aspects, we also discuss implementations of gravitational-wave (GW) dynamics on the lattice, including a new algorithm that improves the memory requirements of previous methods commonly used in the literature. 


<!--
  space
-->

----------

<!--
  space
-->


## $\mathtt{The~Art-III}$ { #subsec_ArtIII }
<p style="margin-top:-2.29em; margin-left:8.0em; font-size:32px; font-weight:bold;">
.  &nbsp; Scalar-Gauge-Fluid dynamics
</p>

You can Download $\mathtt{The~Art-III}$ (Ref. [@Figueroa_2026XYZ]) from: &nbsp; <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">INSPIRE-HEP</a> ; &nbsp;
<a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv Abstract</a> ; &nbsp;
<a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv PDF</a> ; &nbsp;
<a href="./Art_II.pdf" target="_blank" rel="noopener noreferrer" download>Local PDF </a>; &nbsp; <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer"> Publication URL</a>

(<span style="color:red;">**CHANGE to ART-III links**</span>)

This monograph serves as the theoretical foundation for the scalar-gauge-fluid dynamics implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}~{\tt v3.0}$ (to be released on 2027). 

discuss lattice methods for the simulation of fluid dynamics in the early Universe. This review represents a third entry in the monographic series on lattice cosmology techniques, Refs.\cite{Figueroa:2020rrl,Baeza-Ballesteros:2025tme}, which previously covered canonical %\cite{Figueroa:2020rrl} 
and non-canonical %~\cite{Baeza-Ballesteros:2025tme} 
%cases of interacting fields. 
field theory dynamics. Here, we first review the continuous theory of fluid dynamics in flat spacetime, and then in an FLRW background. We consider both conservation and non-conservation forms of the equations, fluids in isolation or coupled to scalar and/or gauge fields, and either
fully relativistic
% ultrarelativistic 
or 
subrelativistic
% non-relativistic
regimes of fluid bulk motion. %Next, after 
After reviewing %briefly 
basic lattice concepts, we introduce detailed discretization schemes for fluid dynamics in expanding backgrounds for: {\it i)} isolated perfect fluids, %neglecting their interaction with bosonic fields and/or deviations with respect to local thermal equilibrium (LTE), 
{\it ii)} isolated imperfect (viscous) fluids, % due to deviations with respect to LTE, 
{\it iii)} fluids coupled to gauge fields, % via Lorentz forces, 
and {\it iv)} fluids coupled to scalar fields. Our evolution algorithms %can 
accommodate self-consistent expansion %of the Universe 
sourced by all scalar, gauge, and fluid sectors, preserving gauge invariance to machine precision in some cases. %if needed. %in the case of gauge theories. 
We also review lattice methods to set up the initial conditions for a fluid, and the implementation of gravitational wave dynamics sourced by all scalar, gauge, and fluid degrees of freedom. This document represents the theoretical basis for scalar-gauge-fluid interacting systems in expanding backgrounds, as implemented in
{\color{blue}\ttfamily \href{http://www.cosmolattice.net}{${\mathcal C}$osmo${\mathcal L}$attice~v3.0}}, which will be publicly released after the publication of this monograph.
