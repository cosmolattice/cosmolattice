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
h_{ij}''-a^{-2(1-\alpha)}\nabla^2h_{ij}+(3-\alpha)\frac{a'}{a}h_{ij}=\frac{2}{m_{p}^2a^{2(1-\alpha)}}\left[\Pi_{ij}^{\rm eff}\right]^{\rm TT} \,, \hspace{5cm}\\
{\Pi}^{\text{eff}}_{ij} \equiv \partial_i {\phi}_{b} \partial_j {\phi}_{b} + \partial_i\theta \partial_j\theta + \left[(D^A_i {\varphi})^*(D^A_j {\varphi}) + (D_i {\Phi})^\dagger(D_j {\Phi}) + {\it c.c.}\right] - \left(a^{-2\alpha}E_i E_j + a^{-2} B_i B_j\right)\,,
%- \left(a^{-2\alpha} E_i^c E_j^c + a^{-2}  B_i^c B_j^c\right)\,.
\end{eqnarray}
```

