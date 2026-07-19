# What is $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ?

## **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ in a Nutshell** { #subsec_WhatIsCL }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ($\mathcal{CL}$) is a modern package for **lattice simulations of the non-linear dynamics of interactive fields in an expanding Universe**, providing an up-to-date numerical tool for investigating the physics of **the early Universe**. The current version ($\tt v2.0$) is ready to simulate the dynamics of field theories described by an action of the type
[](){ #eq_actionCL }
```math
\begin{eqnarray}
S = - \int d^4 x \sqrt{-g}&&\left\{\class{cl-eq-scalar}{\sum_b\frac{1}{2}\partial_{\mu} \phi_b \partial^{\mu} \phi_b} + \class{cl-eq-nmc-kin}{\frac{1}{2} \partial_{\mu}\chi\partial^{\mu}\chi} + \class{cl-eq-alp-kin}{\frac{1}{2}\partial_\mu \theta\partial^\mu \theta} + \class{cl-eq-u1-kin}{(D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi)} +  \class{cl-eq-su2-kin}{(D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi)} \right.\nonumber\\[1.1em]
\label{eq_actionCL}
&& ~~~~~~~~\left. + \class{cl-eq-u1-gauge}{\frac{1}{4} F_{\mu \nu} F^{\mu \nu}} + \class{cl-eq-su2-gauge}{\frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\}} +\class{cl-eq-nmc-coup}{\frac{1}{2}\xi R \chi^{2}} - \class{cl-eq-alp-coup}{\frac{1}{4}\frac{\theta}{\Lambda} F_{\mu \nu}\,\tilde{F}^{\mu \nu}} + V_{\rm int}(\lbrace \phi_c \rbrace,|\varphi|, |\Phi|, \chi, \theta)\right\}\,.
\end{eqnarray}
```
These sectors can be activated either in isolation or simultaneously with the others — *click on any highlighted term to jump to its manual chapter*. More is on its way: non-minimal kinetic terms, axion–$SU(2)$ interactions, and fluids coupled to scalar or gauge fields (see [*Upcoming Features*][subsec_CLupcoming]).

The fields can evolve in flat space-time, or in an expanding background given by the spatially-flat Friedmann-Lemaître-Robertson-Walker (FLRW) metric, with line element (here $\eta$ is the $\alpha$-time)
[](){ #eq_lineFLRW }
```math
\begin{eqnarray}\label{eq_lineFLRW}
ds^2 \equiv g_{\mu\nu}dx^\mu dx^\nu = - a^{2\alpha} (\eta)d\eta^2 + a^2 (\eta) \delta_{ij} dx^i dx^j\,.
\end{eqnarray}
```

In the expanding case, the fields can be evolved either over a **fixed background** (e.g. with fixed equation of state), or with a **self-consistent expansion of the Universe**, the fields themselves dictating the expansion rate via the Friedmann equations. Alongside the matter dynamics, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can simultaneously simulate the emission of the sourced **gravitational wave (GW) backgrounds**
[](){ #eq_GWeomCL }
[](){ #eq_GWsourceCL }
```math
\begin{eqnarray}\label{eq_GWeomCL}
\class{cl-gw-mark}{h_{ij}''-a^{-2(1-\alpha)}\nabla^2h_{ij}+(3-\alpha)\frac{a'}{a}h_{ij}=\frac{2}{m_{p}^2a^{2(1-\alpha)}}\left[\Pi_{ij}^{\rm eff}\right]^{\rm TT} \,,} \hspace{5cm}\\
\label{eq_GWsourceCL}
{\Pi}^{\text{eff}}_{ij} \equiv \partial_i {\phi}_{b} \partial_j {\phi}_{b} + \partial_i\theta \partial_j\theta + \left[(D^A_i {\varphi})^*(D^A_j {\varphi}) 
%+ (D_i {\Phi})^\dagger(D_j {\Phi}) 
+ {\it c.c.}\right] - \left(a^{-2\alpha}E_i E_j + a^{-2} B_i B_j\right)\,.
%- \left(a^{-2\alpha} E_i^c E_j^c + a^{-2}  B_i^c B_j^c\right)\,.
\end{eqnarray}
```

<div class="cl-manual-banner" markdown>

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is **publicly available**, and comes with a **detailed manual** which includes the basic instructions to start running your own simulations.

[$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual →](Manual/About This Manual.md){ .cl-manual-link }
[$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Installation →](code/Installation.md){ .cl-manual-link }

</div>

## **$\mathcal{CL}$ Features & Capabilities** { #subsec_CLfeatAndCapa }
<!-- of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$**  -->

The current version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ($v2.0$, released on July 2026) can simulate the dynamics of the following sectors. *Click on a box to jump to the corresponding section of the user manual:*

<div class="grid cards cl-sectors" markdown>

-   __Scalar–Scalar Interactions__

    ---

    Interacting singlet scalar fields with arbitrary potentials, *e.g.* inflaton–daughter field dynamics during preheating.

    [User Manual →](Manual/My first model of (singlet) scalar fields.md)

-   __Abelian Scalar–Gauge Interactions__

    ---

    $U(1)$-charged scalar fields coupled to Abelian gauge fields.

    [User Manual →](Manual/My first model of gauge fields.md)

-   __Non-Abelian Scalar–Gauge Interactions__

    ---

    $SU(2)$ scalar doublets coupled to non-Abelian gauge fields, alone or combined with a $U(1)$ sector.

    [User Manual →](Manual/My first model of gauge fields.md)

-   __Axion–Gauge Interactions__

    ---

    Axion-like particles (ALPs) coupled to Abelian $U(1)$ gauge sectors through a $\frac{\theta}{\Lambda} F\tilde{F}$ term.

    [User Manual →](Manual/ALP.md)

-   __Non-Minimal Couplings to Gravity__

    ---

    Singlet scalar fields non-minimally coupled to gravity via a $\xi R \chi^2$ interaction.

    [User Manual →](Manual/NMC.md)

-   __Cosmic Defects__

    ---

    Global defects (domain walls, strings, monopoles, textures) and local topological defects (*e.g.* local strings).

    [User Manual →](Manual/Defects.md)

-   __Gravitational Waves__

    ---

    GW backgrounds sourced by scalar field theories and by Abelian $U(1)$ scalar-gauge theories.

    [User Manual →](Manual/GW.md)

</div>

<div class="cl-facts" markdown>
[Symplectic integrators up to $\mathcal{O}(\delta t^{10})$](Manual/What CosmoLattice does in detail.md#subsec_Evolvers){ .cl-fact }
[Low-order Runge-Kutta up to $\mathcal{O}(\delta t^{4})$](Manual/What CosmoLattice does in detail.md#velocityverlet-positionverlet-and-rk2nstorage){ .cl-fact }
[Gauss constraint to machine precision](Manual/My first model of gauge fields.md#eq_GaussU1-eom){ .cl-fact }
[MPI + parallel FFTs](Manual/Parallelization.md#subsubsec_para2D){ .cl-fact }
[CMake](code/Installation.md#download-and-build){ .cl-fact }
[Kokkos](Manual/Parallelization.md#subsubsec_devices){ .cl-fact }
</div>

<div class="cl-claim" markdown>

**High-order integrators, machine-precision constraints**
{: .cl-minihead }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ provides symplectic integrators with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, and non-symplectic integrators with accuracies from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{4})$ <!-- (see *e.g.* Sections 3.3, 3.4 and 3.5 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl], for a discussion on integrators and their properties) -->. Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for scalar-gauge theories, either Abelian or non-Abelian, always respect the Gauss constraint to machine precision, independently of the integrator, even in the case of self-consistent expansion.

</div>

<div class="cl-claim" markdown>

**Change parameters, not code**
{: .cl-minihead }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is written in C++, and fully exploits the *object oriented  programming* nature of this language, with a modular structure that separates well all the ingredients involved. This allows $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to have a clear separation between the physics and the technical implementation details. **The code is designed so that the user can simulate a given scenario with different parameters, without requiring to re-compile each time that parameter values are changed**. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ allows for an easy implementation of new models with either scalar or gauge interactions.

</div>

<div class="cl-claim" markdown>

**Built to scale**
{: .cl-minihead }

**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is fully parallelized using *Message Passing Interface* (MPI), and uses a discrete Fourier Transform parallelized in multiple spatial dimensions** (<span style="color:red;">**CHANGE ?**</span>). This makes it ideal for probing physical problems with well-separated mass/length scales, running very high resolution simulations, or simply shortening the running time of long simulations. **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is actually a general package that defines field variables and their operations, by introducing its own symbolic language**. Once you become familiar with the basic ‘syntaxis’ defined in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, editing the code or implementing your own model--resembling as much as possible how you would write it in the continuum--, becomes a relatively straightforward task.

</div>

<span style="color:red;">**PARAGRAPH on GPU Capabilities [...]**</span>

<!-- 
!!! note "Cutting to the chase ..."
    If the reader is eager to start running their own simulations, they can install the code directly as described in  [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Installation**](./code/Installation.md), or jump into the code's manual at [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual**](./Manual/About This Manual.md). If the reader is however a newcomer to the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ world, we recommend them, instead, to keep on reading. 
 -->

## **What makes $\mathcal{CL}$ different ?**  {#subsec_CLspecial } 
<!-- ## **Is $\mathcal{CL}$ different from other codes?**  {#subsec_CLspecial } -->
<!-- $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ special ?** -->

<div class="cl-claim" markdown>

**A platform, not a single-purpose code**
{: .cl-minihead }

**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is a platform, not a code dedicated to one type of simulation**, such as *e.g.* the dynamics of interacting scalar fields in an expanding background solved by the Leapfrog algorithm. The idea is something else: **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is a framework where one can implement any system of partial differential equations suitable for discretization on a lattice, and the corresponding associated observables**. $\mathcal{CL}$ is a package that introduces its own **symbolic language**, defining field variables and operations over them. Once the user becomes familiar with the basic *vocabulary* of the $\mathcal{CL}$ language, they can write their own code: be it for the time evolution of interactive fields in a model of interest with whichever suitable field content, or for some other operation, like a Monte-Carlo generator for thermal configurations, it is up to the user.

</div>

<div class="cl-claim" markdown>

**Physics up front, machinery under the hood**
{: .cl-minihead }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ separates the **physics** (i.e.~fields living on a lattice and operations between them) from the **technical details**, such as the handling of the parallelization or the Fourier transforms. A beginner user with little experience in programming, and with no experience at all in parallelization techniques.
will be able to run a fully parallelized simulation of their favourite model (using hundreds of processors in a cluster if they wish), while being completely oblivious to the technical details. They will just need to write a basic **model file** in the language of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, containing the details of the model that they want to simulate. At the same time, an experienced user that wants to look inside the core routines of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ and modify, for example, the MPI-implementation, they can always do so, and perhaps even contribute to improving them.

</div>

<div class="cl-claim" markdown>

**Symbolic algebras, parallel FFTs, and more**
{: .cl-minihead }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ includes already a *library* of basic routines and field-theoretical operations. This constitutes a clear advantage when using $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ as a platform to implement a given scenario, over writing your own code from scratch. In particular, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ comes with symbolic scalar, complex [$U(1)$] and $SU(2)$ algebras, which allows the use of vectorial and matrix notations without sacrificing performances. Furthermore, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is MPI-based and uses a discrete Fourier Transform parallelized in multiple spatial dimensions (<span style="color:red;">**CHANGE ?**</span>), making it very powerful for probing physical problems with **well-separated scales**, running very **high resolution simulations**, or simply **very long simulations**.

</div>

## **$\mathcal{CL}$ Code Versions** { #subsec_CLversions }
<!-- **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Versions**  -->


$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is an ever-evolving package, and new or improved features are continuously being added. We typically release new versions of the code whenever new physics modules or new relevant lattice methods are added, though code structure improvements can also occur. Details of the different versions of the code can be found below in the [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Version Guide**](code/Versions.md). The latest version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is currently $\tt v2.0$, released on July 2026. It can be downloaded at [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Download**](code/Download.md).


<!-- 
A brief summary of the changes & improvement in the successive code versions can be found in the [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Version Guide**](code/Versions.md).
 -->

## **$\mathcal{CL}$ Upcoming Features** { #subsec_CLupcoming }
<!-- $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$  -->

As $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is continuously evolving, new physics and/or technical capabilities are being constantly developed, and eventually they are made publicly available. At the time of writing this (July 2026), we are enhancing the capabilities of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate

<div class="grid cards cl-upcoming" markdown>

-   __Fluid Dynamics__
-   __Fluid–Gauge Interactions__
-   __Fluid–Scalar Interactions__
-   __Higgs Bubble Nucleation__
-   __Non-Minimal Kinetic Terms__
-   __GWs from $SU(2)$ Theories__
-   __Axion–$SU(2)$ Interactions__

</div>

As all these new physics capabilities are being developed simultaneously in parallel by different teams, so it is difficult to predict which aspects will become publicly available first. It is therefore important to keep an eye on the tab [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Version Guide**](code/Versions.md), to check for new code releases, which may include new features such as new field variable definitions, options, evolution algorithms, and/or new interactions as those listed just above. All these changes are gradually incorporated in successive updated versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and whenever a new code release is made, this is announced in the [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ News**](CL_News/CLnews.md) tab. 
