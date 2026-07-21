<!-- ** Theory Monographs Guide. **  -->

The theoretical basis for the equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ are presented through a series of monographic reviews on **lattice-cosmology** techniques, written over the years under the common title *"The Art of Simulating the Early Universe"*. At the time of writing (July 2026), the series has three parts: 

- *Part I. Integration techniques and canonical cases.* (Ref. [@Figueroa_2020rrl])

- *Part II. Non-canonical cases and gravitational waves.* (Ref. [@BaezaBallesteros_2025tme]) 

- *Part III. Scalar-Gauge-Fluid dynamics.* (Ref. [@Figueroa_2026XYZ]) 

which will often refer to as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$ and $\mathtt{The~Art-III}$ monographs. These reviews provide comprehensive discussions of lattice-cosmology methods for scalar–gauge field theories in an expanding universe ($\mathtt{The~Art-I}$); gravitational waves and non-canonical aspects of field theory, including interactions, initial conditions, dimensionality ($\mathtt{The~Art-II}$); and fluid dynamics as a description of an ensemble of interacting scalar, gauge, and fermion particles ($\mathtt{The~Art-III}$). 

You can download the monographs from:

| Article | INSPIRE-HEP | ArXiv Abstract | ArXiv PDF | Local PDF | Publication URL |
|:--------|:-----------:|:--------------:|:---------:|:---------:|:---------------:|
| $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]) | <a href="https://inspirehep.net/literature/1803326" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/abs/2006.15122" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/pdf/2006.15122" target="_blank" rel="noopener noreferrer">File</a> | <a href="./pdfs/Art_I.pdf" target="_blank" rel="noopener noreferrer">Open</a> | <a href="https://doi.org/10.1088/1475-7516/2021/04/035" target="_blank" rel="noopener noreferrer">Link</a> |
| $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]) | <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">File</a> | <a href="./pdfs/Art_II.pdf" target="_blank" rel="noopener noreferrer">Open</a> | <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer">Link</a> |
| $\mathtt{The~Art-III}$ (Ref. [@Figueroa_2026XYZ]) | <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">Link</a> | <a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">File</a> | <a href="./pdfs/Art_III.pdf" target="_blank" rel="noopener noreferrer">Open</a> | <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer">Link</a> |

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


<!-- ## $\mathtt{The~Art-I}$. **Integration techniques & canonical cases** { #subsec_ArtI } -->

This monograph provides the theoretical foundation for $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}~{\tt version~1.0}$, released in Feb. 2021. It contains a comprehensive discussion of lattice techniques for simulating scalar and gauge field dynamics in an expanding universe. After reviewing the continuum formulation of scalar and gauge field interactions in both Minkowski and FLRW backgrounds, we introduce the fundamental tools for the discretization of field theories, including **lattice gauge-invariant** techniques. We then discuss and classify numerical algorithms, ranging from methods with $\mathcal{O}(\delta t^2)$ accuracy, such as *staggered leapfrog* and *Verlet integration*, to *Runge--Kutta* methods with accuracy up to $\mathcal{O}(\delta t^4)$, as well as the higher-order *Yoshida* and *Gauss--Legendre* integrators, achieving accuracies up to $\mathcal{O}(\delta t^{10})$. We adapt these methods for classical lattice simulations of the nonlinear dynamics of scalar and gauge fields on an expanding $3+1$-dimensional grid, including the case of **self-consistent** expansion sourced by the volume-averaged energy and pressure densities of the fields. We present lattice formulations for three canonical classes of theories:

1. *Interacting scalar fields*,
2. *Abelian* U(1) *scalar-gauge theories*,
3. *Non-Abelian* SU(2) *scalar-gauge theories*.

For each case, we build symplectic integrators with accuracies ranging from $\mathcal{O}(\delta t^2)$ to $\mathcal{O}(\delta t^{10})$. We also derive lattice expressions for the relevant observables, including the energy density components, field power spectra, and the Hubble constraint. Remarkably, all of our algorithms for gauge theories preserve the Gauss constraint to machine precision, even when self-consistent expansion is taken into account. As a numerical illustration, we analyze the post-inflationary dynamics of an oscillating field charged under $SU(2)\times U(1)$.

You can Download $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]) from: &nbsp; <a href="https://inspirehep.net/literature/1803326" target="_blank" rel="noopener noreferrer">INSPIRE-HEP</a> ;  &nbsp; 
<a href="https://arxiv.org/abs/2006.15122" target="_blank" rel="noopener noreferrer">ArXiv Abstract</a> ; &nbsp; <a href="https://arxiv.org/pdf/2006.15122" target="_blank" rel="noopener noreferrer">ArXiv PDF</a> ; &nbsp;
<a href="./pdfs/Art_I.pdf" target="_blank" rel="noopener noreferrer" download>Local PDF </a> ; &nbsp; <a href="https://doi.org/10.1088/1475-7516/2021/04/035" target="_blank" rel="noopener noreferrer"> Publication URL</a>


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

<!-- ## $\mathtt{The~Art-II}$. **Non-canonical cases and gravitational waves** { #subsec_ArtII } -->

This monograph provides the theoretical foundation for the non-canonical field-theory aspects (including interactions, initial conditions, and dimensionality) and gravitational-wave (GW) dynamics implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ since ${\tt version~2.0}$ (released in July 2026). It presents discussions on lattice techniques for the simulation of non-canonical field-theory scenarios, complementing our previous monograph on canonical cases $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]). We begin by reviewing basic aspects of lattice field theory, including symplectic and non-symplectic evolution algorithms. We then introduce lattice implementations of non-canonical interactions, considering: 

- Scalars with a non-minimal coupling to gravity: $\phi^2 R$

- Non-minimal scalar kinetic theories: $\mathcal{G}_{ab}\big(\{\phi_c\}\big)\partial_\mu \phi^a \partial^\mu \phi^b$

- Axion-like particle (ALP) interactions with Abelian gauge fields: $\phi F_{\mu\nu}\widetilde{F}^{\mu\nu}$

Next, we discuss methods for setting up special field configurations, including:

- Cosmic-defect networks approaching scaling, *e.g.*, cosmic strings and domain walls
- Field configurations based on arbitrary power spectra or spatial profiles
- Probabilistic methods, as required, *e.g.*, for thermal configurations

We further extend the notion of non-canonical theories by discussing the discretization of scalar-field dynamics in $d+1$ dimensions, with $d\neq 3$. Independently of non-canonical aspects, we also discuss implementations of GW dynamics on the lattice, including a new algorithm that improves the memory requirements of previous methods commonly used in the literature. 

You can Download $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]) from: &nbsp; <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">INSPIRE-HEP</a> ; &nbsp;
<a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv Abstract</a> ; &nbsp;
<a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv PDF</a> ; &nbsp;
<a href="./pdfs/Art_II.pdf" target="_blank" rel="noopener noreferrer" download>Local PDF </a>; &nbsp; <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer"> Publication URL</a>


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

<!-- ## $\mathtt{The~Art-III}$. **Scalar-gauge-fluid dynamics** { #subsec_ArtIII } -->

This monograph serves as the theoretical basis for scalar–gauge–fluid interacting systems in expanding backgrounds, as implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ since ${\tt version~3.0}$ (to be released on 2027). It presents lattice methods for the simulation of fluid dynamics in the early Universe, representing a third entry in our monographic series on lattice cosmology techniques, $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]) and $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]), which previously covered canonical and non-canonical field theories, and gravitational waves (GWs). In this monograph, we first review the continuous theory of fluid dynamics in flat spacetime, and then in an FLRW background. We consider both conservation and non-conservation forms of the equations, fluids in isolation or coupled to scalar and/or gauge fields, and either fully relativistic or subrelativistic regimes of fluid bulk motion. After reviewing basic lattice concepts, we introduce detailed discretization schemes for fluid dynamics in expanding backgrounds for: 

- Isolated perfect fluids 
- Isolated imperfect (viscous) fluids 
- Fluids coupled to gauge fields
- Fluids coupled to scalar fields

Our evolution algorithms accommodate self-consistent expansion sourced by all scalar, gauge, and fluid sectors, preserving gauge invariance to machine precision in some cases. We also review lattice methods to set up the initial conditions for a fluid and the implementation of GW dynamics sourced by all scalar, gauge, and fluid degrees of freedom. 

You can Download $\mathtt{The~Art-III}$ (Ref. [@Figueroa_2026XYZ]) from: &nbsp; <a href="https://inspirehep.net/literature/3093619" target="_blank" rel="noopener noreferrer">INSPIRE-HEP</a> ; &nbsp;
<a href="https://arxiv.org/abs/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv Abstract</a> ; &nbsp;
<a href="https://arxiv.org/pdf/2512.15627" target="_blank" rel="noopener noreferrer">ArXiv PDF</a> ; &nbsp;
<a href="./pdfs/Art_II.pdf" target="_blank" rel="noopener noreferrer" download>Local PDF </a>; &nbsp; <a href="https://doi.org/10.1088/1475-7516/2026/06/087" target="_blank" rel="noopener noreferrer"> Publication URL</a>

(<span style="color:red;">**CHANGE to ART-III links**</span>)
