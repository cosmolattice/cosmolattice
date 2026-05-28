# $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ in a Nutshell

### What is CosmoLattice ? { #subsec_WhatIsCL }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is a modern package for lattice simulations of the dynamics of interactive fields in an expanding Universe. We have developed CosmoLattice to provide an up-to-date relevant numerical tool for the scientific community working on early Universe scenarios, including the interactions of scalar fields (singlets), scalar fields charged under a $U(1)$ and/or $SU(2)$ gauge symmetry/ies and their interactions with the corresponding Abelian and/or non-Abelian gauge vector fields, axion-like-particles (ALP) interacting with gauge sectors through $\phi F\tilde F$ terms, non-minimally coupled scalar fields, and soon enough (see below) a fluid sector coupled to scalar and/or gauge fields. In other words, CosmoLattice is ready to simulate the dynamics of field theories described by an action of the type:
```math
\begin{eqnarray}
S = - \int d^4 x \sqrt{-g}&&\left\{\sum_b\frac{1}{2}\partial_{\mu} \phi_b \partial^{\mu} \phi_b + \frac{1}{2} \partial_{\mu}\chi\partial^{\mu}\chi + \frac{1}{2}\partial_\mu \theta\partial^\mu \theta + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) \right.\\
&& ~~~~~~~~\left. + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} +\frac{1}{2}\xi R \chi^{2} - \frac{1}{4}\frac{\theta}{\Lambda} F_{\mu \nu}\,\tilde{F}^{\mu \nu} + V_{\rm int}(\lbrace \phi_c \rbrace,|\varphi|, |\Phi|, \chi, \theta)\right\}\,.
\end{eqnarray}
```

CosmoLattice can simulate the dynamics of the above fields in a flat space-time, or in an expanding background (including inflationary and post-inflationary eras), characterized by a spatially-flat Friedmann-Lemaître-Robertson-Walker (FLRW) metric, with line element:
```math
\begin{eqnarray}
ds^2 \equiv g_{\mu\nu}dx^\mu dx^\nu = - a^{2\alpha} (\eta)d\eta^2 + a^2 (\eta) \delta_{ij} dx^i dx^j\,.
\end{eqnarray}
```

In the expanding case, the fields can evolve either over a fixed background (e.g. with a power-law scale factor), or with **self-consistent expansion of the Universe**, i.e. with the fields involved in a simulation determining themselves the expansion rate through the Friedmann equations. Furthermore, CosmoLattice allow to make predictions on the gravitational wave (GW) backgrounds produced by early Universe scenarios, by simultaneously simulating the emission of GWs throughout the dynamics of the above matter fields:
```math
\begin{eqnarray}
h_{ij}''-a^{-2(1-\alpha)}\nabla^2h_{ij}+(3-\alpha)\frac{a'}{a}h_{ij}=\frac{2}{m_{p}^2a^{2(1-\alpha)}}\left[\Pi_{ij}^{\rm eff}\right]^{\rm TT} \,, \hspace{5cm}\\
{\Pi}^{\text{eff}}_{ij} \equiv \partial_i {\phi}_{b} \partial_j {\phi}_{b} + \partial_i\theta \partial_j\theta + \left[(D^A_i {\varphi})^*(D^A_j {\varphi}) + (D_i {\Phi})^\dagger(D_j {\Phi}) + {\it c.c.}\right] - \left(a^{-2\alpha}E_i E_j + a^{-2} B_i B_j\right)\,,
%- \left(a^{-2\alpha} E_i^c E_j^c + a^{-2}  B_i^c B_j^c\right)\,.
\end{eqnarray}
```

### Features & Capabilities of CosmoLattice { #subsec_CLfeatAndCapa }

The current version of CosmoLattice (version 2.0, released on June 2026), can simulate

1. *Singlet scalar field dynamics*, 
2. *Abelian $U(1)$ scalar-gauge interactions*, 
3. *Non-Abelian $SU(2)$ scalar-gauge interactions*, 
4. *Axion like particle - Abelian $U(1)$ gauge field interactions*, 
5. *Non-minimally coupled-to-gravity scalar field dynamics*, 
6. *Global cosmic defects (domain walls, strings, monopoles, textures)*,
7. *Local topological defects (local strings)*,
8. *Gravitational waves from scalar field theories*, 
9. *Gravitational waves from Abelian $U(1)$ scalar-gauge theories*, 
10. [...] 

CosmoLattice provides symplectic integrators with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, and non-symplectic integrators with accuracies from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{4})$ (See Sections 3.3, 3.4 and 3.5 of $\mathtt{The~Art-I}$[@Figueroa_2020rrl], for a discussion on integrators and their properties). Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for scalar-gauge theories, either Abelian or non-Abelian, always respect the Gauss constraint to machine precision, independently of the integrator, even in the case of self-consistent expansion. 

CosmoLattice is written in C++, and fully exploits the *object oriented  programming* nature of this language, with a modular structure that separates well all the ingredients involved. This allows CosmoLattice to have a clear separation between the physics and the technical implementation details. **The code is designed so that the user can simulate a given scenario with different parameters, without requiring to re-compile each time that parameter values are changed**. <!-- More importantly, the code allows for an easy implementation of new models with either scalar or gauge interactions.  -->

**CosmoLattice is fully parallelized using *Message Passing Interface* (MPI), and uses a discrete Fourier Transform parallelized in multiple spatial dimensions**. This makes it ideal for probing physical problems with well-separated mass/length scales, running very high resolution simulations, or simply shortening the running time of long simulations. **CosmoLattice is actually a general package that defines field variables and their operations, by introducing its own symbolic language. Once you become familiar with the basic ‘vocabulary’ of the CosmoLattice language, editing the code or implementing your own model (resembling how you would write it in the continuum), should become a simple task.**

### What makes CosmoLattice different ? { #subsec_CLspecial }

### Upcoming features { #subsec_CLupcoming }