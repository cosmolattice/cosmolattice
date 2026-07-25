# $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ in a Nutshell

### What is CosmoLattice ? { #subsec_WhatIsCL }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is a modern package for **lattice simulations of the dynamics of interactive fields in an expanding Universe**. CosmoLattice is being continuously developed to provide an up-to-date relevant numerical tool for the scientific community working on the **physics of the early Universe**. The current version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ($\tt v2.0$, released on July 2026), can handle the dynamics of interactive scalar fields (singlets), scalar fields charged under a U(1) and/or SU(2) gauge symmetries interacting with the corresponding Abelian and/or non-Abelian gauge fields, axion-like-particles (ALP) interacting with gauge sectors, scalar fields non-minimally coupled to gravity, and soon enough (see below) a fluid sector coupled to scalar and/or gauge fields. In other words, CosmoLattice is ready to simulate the dynamics of field theories described by an action of the type:
```math
\begin{eqnarray}
S = - \int d^4 x \sqrt{-g}&&\left\{\sum_b\frac{1}{2}\partial_{\mu} \phi_b \partial^{\mu} \phi_b + \frac{1}{2} \partial_{\mu}\chi\partial^{\mu}\chi + \frac{1}{2}\partial_\mu \theta\partial^\mu \theta + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) \right.\\
&& ~~~~~~~~\left. + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} +\frac{1}{2}\xi R \chi^{2} - \frac{1}{4}\frac{\theta}{\Lambda} F_{\mu \nu}\,\tilde{F}^{\mu \nu} + V_{\rm int}(\lbrace \phi_c \rbrace,|\varphi|, |\Phi|, \chi, \theta)\right\}\,.
\end{eqnarray}
```
Any of these sectors can be activated, either in isolation, or simultaneously with others. 

CosmoLattice can simulate the dynamics of the above *matter* fields in a flat space-time, or in an expanding background (including inflationary and post-inflationary eras), given by a spatially-flat Friedmann-Lemaître-Robertson-Walker (FLRW) metric, characterized by the line element (here $\eta$ is the so called $\alpha$-time):
```math
\begin{eqnarray}
ds^2 \equiv g_{\mu\nu}dx^\mu dx^\nu = - a^{2\alpha} (\eta)d\eta^2 + a^2 (\eta) \delta_{ij} dx^i dx^j\,.
\end{eqnarray}
```

In the expanding case, the matter fields can be either evolved over a **fixed background** (e.g. with a power-law scale factor), or using the option of **self-consistent expansion of the Universe**, i.e. with the fields themselves determining the expansion rate of the Universe through the Friedmann equations. Furthermore, CosmoLattice allow to make predictions on the gravitational wave (GW) backgrounds produced by early Universe scenarios, by simultaneously simulating the emission of GWs throughout the dynamics of the above matter fields:
```math
\begin{eqnarray}
h_{ij}''-a^{-2(1-\alpha)}\nabla^2h_{ij}+(3-\alpha)\frac{a'}{a}h_{ij}'=\frac{2}{m_{p}^2a^{2(1-\alpha)}}\left[\Pi_{ij}^{\rm eff}\right]^{\rm TT} \,, \hspace{5cm}\\
{\Pi}^{\text{eff}}_{ij} \equiv \partial_i {\phi}_{b} \partial_j {\phi}_{b} + \partial_i\theta \partial_j\theta + \left[(D^A_i {\varphi})^*(D^A_j {\varphi}) + (D_i {\Phi})^\dagger(D_j {\Phi}) + {\it c.c.}\right] - \left(a^{-2\alpha}E_i E_j + a^{-2} B_i B_j\right)\,,
%- \left(a^{-2\alpha} E_i^c E_j^c + a^{-2}  B_i^c B_j^c\right)\,.
\end{eqnarray}
```

### Features & Capabilities of CosmoLattice { #subsec_CLfeatAndCapa }

The current version of CosmoLattice (version 2.0, released on July 2026), can simulate

1. *Singlet scalar field dynamics*, 
2. *Abelian U(1) scalar-gauge interactions*,
3. *Non-Abelian SU(2) scalar-gauge interactions*,
4. *Axion like particle - Abelian U(1) gauge field interactions*,
5. *Non-minimally coupled-to-gravity scalar field dynamics*, 
6. *Global cosmic defects (e.g. domain walls, strings, monopoles, or textures)*,
7. *Local topological defects (e.g. local strings)*,
8. *Gravitational waves from scalar field theories*, 
9. *Gravitational waves from Abelian U(1) scalar-gauge theories*,
10. [...] 

CosmoLattice provides symplectic integrators with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, and non-symplectic integrators with accuracies from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{4})$ [see *e.g.* Sections 3.3, 3.4 and 3.5 of $\mathtt{The~Art-I}$[@Figueroa_2020rrl], for a discussion on integrators and their properties]. Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for scalar-gauge theories, either Abelian or non-Abelian, always respect the Gauss constraint to machine precision, independently of the integrator, even in the case of self-consistent expansion. 

CosmoLattice is written in C++, and fully exploits the *object oriented  programming* nature of this language, with a modular structure that separates well all the ingredients involved. This allows CosmoLattice to have a clear separation between the physics and the technical implementation details. **The code is designed so that the user can simulate a given scenario with different parameters, without requiring to re-compile each time that parameter values are changed**. Remarkably, the code allows for an easy implementation of new models with either scalar or gauge interactions.

**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is fully parallelized using both shared memory parallelization (*OpenMP*, *POSIX Threads* or GPU acceleration) and distributed parallelization (*Message Passing Interface* (MPI)) for use in high-performance clusters, and uses a discrete Fourier Transform parallelized in multiple spatial dimensions**. This makes it ideal for probing physical problems with well-separated mass/length scales, running very high resolution simulations, or simply shortening the running time of long simulations. To provide these capabilities, **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ relies on `TempLat`, a general package for lattice simulations that defines field variables and their operations, by introducing its own symbolic language, and managing all aspects of dispatching workload to the available computational resources. Once you become familiar with the basic ‘vocabulary’ of the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ and `TempLat` language, editing the code or implementing your own model (resembling how you would write it in the continuum), should become a simple task.**

With **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$** 2.0, through the `TempLat` library, a particularly powerful new feature is the native support for **GPU acceleration**. This is achieved by using the `Kokkos` library, which allows for a single code base to be compiled and run on different architectures, including NVIDIA GPUs (via CUDA), AMD GPUs (via HIP), and multi-core CPUs (via OpenMP or Pthreads). This means that users can take advantage of the computational power of modern GPUs without having to write GPU-specific code, making it easier to run large-scale simulations efficiently. `TempLat` also handles the MPI distribution of the workload across many GPUs, allowing for simulations that require more memory than a single GPU can provide. Using GPUs for lattice simulations is as simple as switching a single flag in the `CMake` configuration, and the code will automatically handle the rest, including memory management and parallelization, see also [**Installation - Full**](../code/Installation.md) for more details.

### What makes CosmoLattice different ? { #subsec_CLspecial }

\CL is meant to be a **platform*** where one can implement any system of partial differential equations suitable for discretization on a lattice. That is, \CL is not meant as a code dedicated to one type of simulation, say with a given type of fields and a fixed integration technique, such as *e.g.* the dynamics of interacting (singlet) scalar fields in an expanding background solved by a Leapfrog algorithm. The idea of CL is rather something else: it is a package that introduces its own *symbolic language*, by defining field variables and operations over them. Therefore, once the user becomes familiar with the basic *vocabulary* of the new language, they can write their own code: be it for the time evolution of interactive fields in a model of interest with which ever suitable field content, or for some other operation, like a Monte-Carlo generator for thermal configurations, it is up to the user. 

One of the major advantages of \CL is that it separates the **physics*** (i.e. fields living on a lattice and operations between them) from the **technical details***, such as the handling of the parallelization or the Fourier transforms. For example, let us imagine a beginner user with little experience in programming, and with no experience at all in parallelization techniques. With CL, they will be able to run a fully parallelized simulation of their favourite model (say using hundreds of processors in a cluster), while being completely oblivious to the technical details. They will just need to write a basic *model file* in the language of CL, containing the details of the model that they want to simulate. If, on the contrary, the user is rather an experienced one and wants to look inside the core routines of \CL and modify, for example, the MPI-implementation, they can always do so, and perhaps even contribute to improving them.

Furthermore, \CL includes already a *library* of basic routines and field-theoretical operations. This constitutes a clear advantage of using \CL as a platform to implement a given scenario over writing your own code from scratch. In particular, \CL comes with symbolic scalar, complex [U(1)] and SU(2) algebras, which allows the use of vectorial and matrix notations without sacrificing performances. Furthermore, \CL is MPI-based and uses a discrete Fourier Transform parallelized in multiple spatial dimensions **NAME**, making it very powerful for probing physical problems with **well-separated scales**, running very **high resolution simulations**, or simply **very long simulations**. \CL is publicly available, and it comes with a detailed manual explaining its whole structure and the basic instructions to start running your own simulations, see [**CosmoLattice Manual**](../Manual/About This Manual.md)

### CosmoLattice Versions { #subsec_CLversions }



### Upcoming features { #subsec_CLupcoming }

CosmoLattice is continuously evolving, with new physics modules and/or technical capabilities being constantly developed and made publicly available. At the time of writing (June 2026), we are enhancing the capabilities of CosmoLattice to simulate 

10. *Fluids (fully relativistic and non-relativistic)*, 
11. *Fluid - Abelian U(1) gauge field interactions*,
12. *Fluid - scalar field interactions*, 
13. *Non-minimal kinetic scalar theories*,
14. *Gravitational waves from Non-Abelian SU(2) scalar-gauge theories*,
15. *Axion like particle - Non-Abelian SU(2) gauge field interactions*,
16. [...]

As all these new physics capabilities are being developed simultaneously, in parallel by different sub-teams working on the improvement of CL, it is difficult to predict which aspects will become publicly available first. It is therefore important to keep an eye on the [**CosmoLattice Versions**](./CLversions.md) tab to check for new features (ranging from new variable definitions, options, evolver algorithms, field content, interactions, etc) incorporated in successive updated versions of CosmoLattice.

