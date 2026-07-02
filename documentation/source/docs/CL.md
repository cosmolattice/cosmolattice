# $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ in a Nutshell

### **What is $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ?** { #subsec_WhatIsCL }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is a modern package for **lattice simulations of the non-linear dynamics of interactive fields in an expanding Universe**. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is being continuously developed to provide an up-to-date relevant numerical tool for the scientific community working on the **physics of the early Universe**. The current version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ (*v2.0*, released on July 2026), can simulate the dynamics of interactive (singlet) scalar fields, local $U(1)$ and/or $SU(2)$ charged scalar fields interacting with the corresponding Abelian and/or non-Abelian gauge fields, axion-like-particles (ALPs) interacting with Abelian gauge sectors, singlet scalar fields non-minimally coupled to gravity, and soon enough $--$ see [*Upcoming features*][subsec_CLupcoming] below $--$ non-minimal scalar kinetic interactions, ALPs interacting with SU(2) gauge sectors, and fluids coupled to scalar and/or gauge field sectors. At present, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ $\tt v2.0$ is ready to simulate the dynamics of field theories described by an action of the type
[](){ #eq_actionCL }
```math
\begin{eqnarray}
S = - \int d^4 x \sqrt{-g}&&\left\{\sum_b\frac{1}{2}\partial_{\mu} \phi_b \partial^{\mu} \phi_b + \frac{1}{2} \partial_{\mu}\chi\partial^{\mu}\chi + \frac{1}{2}\partial_\mu \theta\partial^\mu \theta + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) \right.\nonumber\\
\label{eq_actionCL}
&& ~~~~~~~~\left. + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} +\frac{1}{2}\xi R \chi^{2} - \frac{1}{4}\frac{\theta}{\Lambda} F_{\mu \nu}\,\tilde{F}^{\mu \nu} + V_{\rm int}(\lbrace \phi_c \rbrace,|\varphi|, |\Phi|, \chi, \theta)\right\}\,.
\end{eqnarray}
```
These sectors can be activated, either in isolation, or simultaneously with the others. 

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can simulate the dynamics of the above *matter* fields in flat space-time, or in an expanding background, given by the spatially-flat Friedmann-Lemaître-Robertson-Walker (FLRW) metric, with line element (here $\eta$ is the so called $\alpha$-time)
[](){ #eq_lineFLRW }
```math
\begin{eqnarray}\label{eq_lineFLRW}
ds^2 \equiv g_{\mu\nu}dx^\mu dx^\nu = - a^{2\alpha} (\eta)d\eta^2 + a^2 (\eta) \delta_{ij} dx^i dx^j\,.
\end{eqnarray}
```

In the expanding case, let this be during or after inflation, the matter fields can be either evolved over a **fixed background** (e.g. with fixed equation of state), or using a **self-consistent expansion of the Universe**, with the fields involved dictating themselves the expansion rate of the Universe via the Friedmann equations. Furthermore, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ allows to make predictions on the gravitational wave (GW) backgrounds produced by early Universe scenarios, by simultaneously simulating the emission of GWs throughout the dynamics of the above matter fields:
[](){ #eq_GWeomCL }
[](){ #eq_GWsourceCL }
```math
\begin{eqnarray}\label{eq_GWeomCL}
h_{ij}''-a^{-2(1-\alpha)}\nabla^2h_{ij}+(3-\alpha)\frac{a'}{a}h_{ij}=\frac{2}{m_{p}^2a^{2(1-\alpha)}}\left[\Pi_{ij}^{\rm eff}\right]^{\rm TT} \,, \hspace{5cm}\\
\label{eq_GWsourceCL}
{\Pi}^{\text{eff}}_{ij} \equiv \partial_i {\phi}_{b} \partial_j {\phi}_{b} + \partial_i\theta \partial_j\theta + \left[(D^A_i {\varphi})^*(D^A_j {\varphi}) 
%+ (D_i {\Phi})^\dagger(D_j {\Phi}) 
+ {\it c.c.}\right] - \left(a^{-2\alpha}E_i E_j + a^{-2} B_i B_j\right)\,.
%- \left(a^{-2\alpha} E_i^c E_j^c + a^{-2}  B_i^c B_j^c\right)\,.
\end{eqnarray}
```

### **Features & Capabilities of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$** { #subsec_CLfeatAndCapa }

The current version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ (version 2.0, released on July 2026), can simulate the dynamics of

1. *Singlet scalar-scalar interactions*, 
2. *Abelian $U(1)$ scalar-gauge interactions*, 
3. *Non-Abelian $SU(2)$ scalar-gauge interactions*, 
4. *Axion like particle - Abelian $U(1)$ gauge field interactions*, 
5. *Non-minimally coupled-to-gravity (singlet) scalar fields*, 
6. *Global cosmic defects (e.g. domain walls, strings, monopoles, or textures)*,
7. *Local topological defects (e.g. local strings)*,
8. *Gravitational waves from scalar field theories*, 
9. *Gravitational waves from Abelian $U(1)$ scalar-gauge theories*, 
10. [...] 

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ provides symplectic integrators with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, and non-symplectic integrators with accuracies from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{4})$ <!-- (see *e.g.* Sections 3.3, 3.4 and 3.5 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl], for a discussion on integrators and their properties) -->. Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for scalar-gauge theories, either Abelian or non-Abelian, always respect the Gauss constraint to machine precision, independently of the integrator, even in the case of self-consistent expansion. 

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is written in C++, and fully exploits the *object oriented  programming* nature of this language, with a modular structure that separates well all the ingredients involved. This allows $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to have a clear separation between the physics and the technical implementation details. **The code is designed so that the user can simulate a given scenario with different parameters, without requiring to re-compile each time that parameter values are changed**. Remarkably, the code allows for an easy implementation of new models with either scalar or gauge interactions.

**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is fully parallelized using *Message Passing Interface* (MPI), and uses a discrete Fourier Transform parallelized in multiple spatial dimensions**. This makes it ideal for probing physical problems with well-separated mass/length scales, running very high resolution simulations, or simply shortening the running time of long simulations. **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is actually a general package that defines field variables and their operations, by introducing its own symbolic language**. Once you become familiar with the basic ‘vocabulary’ of the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ language, editing the code or implementing your own model (resembling how you would write it in the continuum), should become a simple task.

### **What makes $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ different ?** { #subsec_CLspecial }

\CL is meant to be a **platform*** where one can implement any system of partial differential equations suitable for discretization on a lattice. That is, \CL is not meant as a code dedicated to one type of simulation, say with a given type of fields and a fixed integration technique, such as *e.g.* the dynamics of interacting (singlet) scalar fields in an expanding background solved by a Leapfrog algorithm. The idea of CL is rather something else: it is a package that introduces its own *symbolic language*, by defining field variables and operations over them. Therefore, once the user becomes familiar with the basic *vocabulary* of the new language, they can write their own code: be it for the time evolution of interactive fields in a model of interest with which ever suitable field content, or for some other operation, like a Monte-Carlo generator for thermal configurations, it is up to the user. 

One of the major advantages of \CL is that it separates the **physics*** (i.e.~fields living on a lattice and operations between them) from the **technical details***, such as the handling of the parallelization or the Fourier transforms. For example, let us imagine a beginner user with little experience in programming, and with no experience at all in parallelization techniques. With CL, they will be able to run a fully parallelized simulation of their favourite model (say using hundreds of processors in a cluster), while being completely oblivious to the technical details. They will just need to write a basic *model file* in the language of CL, containing the details of the model that they want to simulate. If, on the contrary, the user is rather an experienced one and wants to look inside the core routines of \CL and modify, for example, the MPI-implementation, they can always do so, and perhaps even contribute to improving them. 

Furthermore, \CL includes already a *library* of basic routines and field-theoretical operations. This constitutes a clear advantage of using \CL as a platform to implement a given scenario over writing your own code from scratch. In particular, \CL comes with symbolic scalar, complex [$U(1)$] and $SU(2)$ algebras, which allows the use of vectorial and matrix notations without sacrificing performances. Furthermore, \CL is MPI-based and uses a discrete Fourier Transform parallelized in multiple spatial dimensions **NAME**, making it very powerful for probing physical problems with **well-separated scales**, running very **high resolution simulations**, or simply **very long simulations**. \CL is publicly available, and it comes with a detailed manual explaining its whole structure and the basic instructions to start running your own simulations, see [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual**](./Manual/About This Manual.md)

### **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Versions** { #subsec_CLversions }



### **Upcoming features** { #subsec_CLupcoming }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is continuously evolving, with new physics modules and/or technical capabilities being constantly developed and made publicly available. At the time of writing (June 2026), we are enhancing the capabilities of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate 

10. *Fluids (fully relativistic and non-relativistic)*, 
11. *Fluid - Abelian $U(1)$ gauge field interactions*, 
12. *Fluid - scalar field interactions*, 
13. *Non-minimal kinetic scalar theories*,
14. *Gravitational waves from Non-Abelian $SU(2)$ scalar-gauge theories*,
15. *Axion like particle - Non-Abelian $SU(2)$ gauge field interactions*,
16. [...]

As all these new physics capabilities are being developed simultaneously, in parallel by different sub-teams working on the improvement of CL, it is difficult to predict which aspects will become publicly available first. It is therefore important to keep an eye on the [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Versions**](./CLversions.md) tab to check for new features (ranging from new variable definitions, options, evolver algorithms, field content, interactions, etc) incorporated in successive updated versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

