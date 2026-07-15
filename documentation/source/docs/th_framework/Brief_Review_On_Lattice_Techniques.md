<!-- The present Section [Field Discretization Primer](Brief_Review_On_Lattice_Techniques.md) --> 
In this section we introduce basic lattice concepts, following closely our discussions on these topics from Section 3 of $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]), Section 2 of $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]), and Section 3 of $\mathtt{The~Art-III}$ (Ref. [@Figueroa_2026XYZ]). If the user has already read either of those Sections or the $\mathtt{The~Art}$ monographs (available [here](MonographicReviews.md)), or they are simply familiar with scalar and/or gauge field lattice simulations, they can jump right ahead into the Sections [Scalar-Scalar Interactions](../Manual/My first model of (singlet) scalar fields.md) or [Scalar-Gauge Interations](../Manual/My first model of gauge fields.md) of the [$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual](../Manual/About This Manual.md), in order to set up their first scalar or scalar-gauge field simulations, respectively. If the reader is familiar with scalar field lattice simulations but not with gauge-invariant lattice  field theory, we recommend them to read our discussion below on [*Lattice gauge invariant techniques*][subsec_LGT], before jumping into [Scalar-Gauge Interations](My first model of gauge fields.md) in the [$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual](../Manual/About This Manual.md). 


!!! note "On the Number of Spatial Dimensions"
	While $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can simulate the dynamics of interacting fields in a regular hyper-cubic lattice of $N^{d}$ points, <!-- in total, with $N$ the number of lattice sites per dimension, and --> with $d$ the number of spatial dimensions, in this Section we will set $d = 3$, as $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ works by default in 3-spatial dimensions. After all... we live a three-dimenensional world ;) 

## Characterization of a lattice { #subsec_Lattice }

A regular cubic lattice in $d=3$ spatial dimensions is fully characterized by two parameters: the number of points per dimension $N$, and the length of each side $L$. The total number of lattice sites is therefore $N^3$. The ratio between $L$ and $N$ represents the *lattice spacing*,

[](){ #eq_auto_003 }
```math
\begin{eqnarray}
    dx \equiv {L\over N}\,,
\label{eq_auto_003}
\end{eqnarray}
```
which is the minimum distance resolved in a lattice. The different lattice sites can be tagged by a vector ${\bf n}$ as follows,

[](){ #eq_auto_004 }
```math
\begin{eqnarray}
    {\bf n} = (n_1,n_2,n_3),~~~~ {\rm with}~~ n_i = 0,1,...,N-1 \,,~~~i = 1,2,3\,.
\label{eq_auto_004}
\end{eqnarray}
```

A function ${\tt f}(\bf x)$ defined in the continuum can be represented on a lattice by a discrete function $f({\bf n})$, which takes the same value as the continuum function in the lattice sites, *i.e.* $f({\bf n}) \equiv {\tt f}({\bf x} = {\bf n} \, dx)$. Unless explicitly stated otherwise, we assume *periodic boundary conditions* through all three spatial directions, meaning that the function satisfies $f({\bf n} + \hat{\imath} N) = f({\bf n})$ for $i = 1,2,3$, with $ \hat{\imath}$ the unitary vector in the $i$-spatial direction. In general, spatially dependent functions ${f}({\bf n})$ on a lattice represent field amplitudes at a given time, so their value will change as the simulation progresses. Therefore, functions on a lattice will depend not only on spatial coordinates $\bf n$, <!-- (or reciprocal coordinates $\tilde{\bf n}$, as introduced in the next subsection), --> but also on a discrete time variable $n_0 = 0, 1, 2, \dots$, counting the number of evolution time-steps. A given moment in the evolution of fields is then indicated as $\eta = \eta_* + n_0 \delta \eta$, where $\delta \eta$ is the temporal step and $\eta_*$ an initial time. We will therefore treat fields as four-dimensional functions and write them as $f(n) = f(n_0,{\bf n})$. <!--  or $f(\tilde n) = f(n_0,\tilde{\bf n})$. --> Additionally, we denote a one-step time advance using $\hat{0}$, so that, for example, $f(n+\hat 0) = f(n_0+1,{\bf n})$. We will write $f(n+\hat{\mu})$ representing either $f(n_0+1,{\bf n})$ or $f(n_0,{\bf n}+\hat{\imath})$, depending on whether $\mu = 0$ or $\mu = i$. 

<!--
  space
-->


<!--
  space
-->


[](){ #sub_reciprocal }
**Reciprocal lattice**

For any lattice we can define a *reciprocal lattice* in momentum space, corresponding to a set of points tagged by a vector $\tilde{\bf n}$ with the following components,
[](){ #eq_auto_005 }
```math
\begin{eqnarray}
    \tilde{\bf n} = (\tilde n_1, \tilde n_2, \tilde n_3), ~~~~{\rm with}~~
    \tilde n_i = -\frac{N}{2}+1, -\frac{N}{2}+2, ... ,-1,0,1, ... , \frac{N}{2} - 1, \frac{N}{2}  \,,~~~ i  = 1,2,3\,.
\label{eq_auto_005}
\end{eqnarray}
```
We define the discrete Fourier transform (DFT) as follows,
[](){ #eq_FTdiscrete }
```math
\begin{eqnarray}\label{eq_FTdiscrete}
    f({\bf n}) \equiv {1\over N^3}\sum_{\tilde {\bf n}} e^{-i{2\pi\over N} {\bf \tilde n n}} f({\bf \tilde n}) ~~~~ \Longleftrightarrow ~~~~  f({\bf \tilde n}) \equiv \sum_{\bf n} e^{+i{2\pi\over N} {\bf n \tilde n} }f({\bf n})\,,
\end{eqnarray}
```
where the weight in the first expression is a consequence of the identity $\sum_{\bf n} e^{i{2\pi\over N} {\bf n} \tilde {\bf n} } = N^3\delta_{{\bf 0}, \tilde {\bf n}}$. As expected, the Fourier-transformed functions also exhibit periodicity in the reciprocal lattice, *i.e.* they satisfy periodic boundary conditions as $f({\bf\tilde{n}} + {\hat \imath} N) =  f({\bf\tilde{n}})$.

The *infrared* and *ultraviolet* cutoffs in a lattice, *i.e.* the minimum and maximum momentum resolved by the reciprocal lattice in each dimension, are
[](){ #eq_IRandUVmodes }
```math
\begin{equation} \label{eq_IRandUVmodes}
k_{\rm IR} \equiv \frac{2\pi}{L} = \frac{2\pi}{Ndx}\,, \hspace{0.7cm}  k_{\rm UV} \equiv {N\over2}k_{\rm IR} = {\pi\over dx} \ . 
\end{equation}
```
We note that $k_{\rm UV}$ is also known as the *Nyquist* frequency.

The reciprocal lattice thus captures a range of discrete momenta, 
[](){ #eq_lin_k_lattice }
```math
\begin{eqnarray}
{\bf k} = k_{\rm IR} (\tilde{n}_1,\tilde{n}_2,\tilde{n}_3)\,,
\label{eq_lin_k_lattice}
\end{eqnarray}
```
with the maximum modulus corresponding to the diagonal of the reciprocal lattice $k_{\rm max}  = \sqrt{3}{N \over 2}k_{\rm IR} = \sqrt{3}\pi / dx$. The modulus of momentum will be indicated as $k = k(\tilde n) \equiv k_{\rm IR}|\tilde {\bf n}|$, where $\tilde n = |\tilde {\bf n}| \equiv$ $({\tilde n}^2_1 +{\tilde n}^2_2 +{\tilde n}^2_3)^{1/2}$. We note that while the the number of modes with approximately the same modulus grows roughly as $\sim 4\pi |\tilde{\bf n}|^2$ for sub-Nyquist modes ($k < k_{\rm UV}$), this number  starts decaying abruptly for supra-Nyquist modes ($k > k_{\rm UV}$) as we approach $k_{\rm max}$.



[](){ #subsubsec_ProgramVariables }
**Program variables**

When simulating the evolution of interactive fields on a lattice, it is convenient to work with *program variables*, which are a set of dimensionless field and spacetime variables defined as follows,
[](){ #eq_ScalarGaugeProgramVar }
```math
\begin{align}
d\tilde\eta \equiv a^{- \alpha} \omega_* dt , \hspace{0.4cm}
d\tilde x^i \equiv \omega_* dx^i ,
\hspace{0.4cm}
\tilde\phi = \frac{\phi}{f_*}  , \hspace{0.4cm}
\tilde\varphi = \frac{\varphi}{f_*}  , \hspace{0.4cm} \widetilde{\Phi} = \frac{\Phi}{f_*}  , \hspace{0.4cm}  \widetilde{A}_\mu=\frac{A_\mu }{\omega_*}  , \hspace{0.4cm} \widetilde B_{\mu}^a = \frac{B_{\mu}^a}{\omega_*}  . %\tag{56}
\label{eq_ScalarGaugeProgramVar}
\end{align}
```
where $\delta t$ and $\delta x$ are the time-step and lattice spacing used for solving the field dynamics, respectively, and $f_*$ and $\omega_*$ are constants of dimension mass +1. For each problem, one can choose $f_*$ and $\omega_*$ appropriately so that the program variables take numerical values of order unity during the fields' evolution. See further discussion on the choice of program variables in [*Program variables*][subsec_LatticeScalars] from Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md) and [*Program variables*][subsec_LatticeScalarGauge] from Section [Scalar-Gauge Interactions](My first model of gauge fields.md).

It is also convenient to define the *program potential* as
[](){ #eq_ProgramPotMultiScalar }
```math
\begin{equation}\label{eq_ProgramPotMultiScalar}
\widetilde{V} (\tilde{\phi}, |\tilde{\varphi}|, |\widetilde{\Phi}|) \equiv \frac{1}{f_*^2 \omega_*^2} V(f_* \tilde \phi, f_* |\tilde \varphi|, f_* |\widetilde \Phi|  )\,, \  \end{equation}
```
as well as program variables for the field strengths and covariant derivatives as follows,
[](){ #eq_auto_007 }
```math
\begin{eqnarray}
\widetilde{F}_{\mu \nu} \equiv \frac{F_{\mu \nu}}{ \omega_*^2}\ , \hspace{0.4cm} \widetilde{G}_{\mu \nu}^a \equiv \frac{G_{\mu \nu}^a}{ \omega_*^2}\ , \hspace{0.4cm} \widetilde{D}_{\mu}^A \equiv \frac{D_{\mu}^A}{ \omega_*}\ , \hspace{0.4cm} \widetilde{D}_{\mu} \equiv \frac{D_{\mu}}{ \omega_*}\,.
\label{eq_auto_007}
\end{eqnarray}
```
Finally, we note that as the definition of linear momentum in Eq.$~$\eqref{eq_lin_k_lattice} scales as $k\propto {1/\delta x}$, we then naturally normalize linear momenta on the lattice using the inverse re-scaling for $\delta x$, *i.e.*
[](){ #eq_auto_008 }
```math
\begin{eqnarray}
    \kappa \equiv \frac{k}{\omega_*}\,.
\label{eq_auto_008}
\end{eqnarray}
```

<!-- [](){ #subsec_LatticeMomentum } -->
## Gradients and lattice momentum { #subsec_LatticeMomentum }

When writing the fields' equation of motion (EOM) on the lattice, we need to substitute continuum derivatives by discretized operations that must reproduce the continuum expressions up to some order of accuracy in the lattice spacing/time step. For example, the derivative of a continuous function ${\tt f}$ can be approximated by the following *neutral* or *centered* difference,
[](){ #eq_neutrald }
```math
\begin{equation}
    \label{eq_neutrald}
    [\nabla^{(0)}_\mu {f}] = \frac{{f}({n}+\hat\mu) - {f}({n}-\hat\mu)}{2dx ^\mu} ~~\longrightarrow ~~ \partial_{\mu} {f}({x})\big|_{{x}\,\equiv\, {\bf n}dx+n_0{d\eta}} + \mathcal{O}(dx_\mu^2)\,,
\end{equation}
```
where $\delta x^{\mu}$ represents either the time step $\delta \eta$ (for $\mu = 0$) or the lattice spacing $\delta x$ (for $\mu = i$). The expression is symmetric around the lattice point $n$, and recovers the continuum expression up to $\mathcal{O}(\delta x_\mu^2)$. We could also approximate the continuous derivative by the following *charged* difference,
[](){ #eq_forwardbackwardd }
```math
\begin{eqnarray}
    \label{eq_forwardbackwardd}
    [\nabla^\pm_\mu {f}] = \frac{\pm {f}({n}\pm \hat\mu) \mp {f}({n})}{dx^\mu} ~~\longrightarrow ~~ \left\lbrace\begin{array}{l}
        \partial_{\mu} {f}({x})\big|_{{x}\,\equiv\, {\bf n}dx+n_0{d\eta}} + \mathcal{O}(dx_\mu)\,,  \\[0.5em]
        \partial_{\mu} {f}({x})\big|_{{x}\,\equiv\, ({n} \pm \hat\mu/2)dx^\mu} + \mathcal{O}(dx_\mu^2)\,,
    \end{array}\right.
\end{eqnarray}
```
where $\nabla_{\mu}^+f$ and $\nabla_\mu^- {f}$ are called the *forward* and *backward* derivatives, respectively. Compared to the neutral derivative, they have the advantage of being sensitive to the minimum space interval captured by a lattice, *i.e.* to the lattice spacing. These expressions, if expanded around an actual lattice site ${\bf n}$,  only recover the continuum derivative up to $\mathcal{O}({\delta x}_\mu)$. However, if expanded in between the two lattice sites involved, they approximate the continuum expression to $\mathcal{O}({\delta x}_\mu^2)$. One can also implement discrete derivatives of higher order at either grid or half-grid points, involving field values of at more lattice points, see below Subsection [*Higher order derivatives and lattice momenta*][subsec_HigherOrder].

Associated to each spatial lattice derivative, we can define a *lattice momentum* ${\bf k_\text{L}}$ through the following relation in Fourier space,
  [](){ #eqn_latticemomentum }
```math
\begin{equation}  \label{eqn_latticemomentum}  [\nabla_i f]({\tilde{\bf n}}) = -i{\bf k}_{\text{L}}({\tilde{\bf n}}) f({\tilde{\bf n}}) \:. \end{equation}
```
For example, the cartesian components of the lattice momentum for the derivative defined in (\ref{eq_neutrald}) is
[](){ #eqn_k0 }
```math
\begin{equation}
    k^0_{\text{L}, i} = \dfrac{\sin (2\pi {\tilde n}_i/N)}{\delta x} \: .\label{eqn_k0}\\
\end{equation}
```
while for the charged derivative \eqref{eq_forwardbackwardd} we have instead
[](){ #eqn_kpm }
[](){ #eq_latticeMomentum0 }
```math
\begin{align}
    k^{\pm}_{\text{L}, i} &= 2 e^{\mp i\pi \tilde{n}_i/N} \dfrac{\sin(\pi \tilde{n}_i/N)}{\delta x} = \dfrac{\sin(2 \pi \tilde{n}_i/N)}{\delta x} \mp i \dfrac{1-\cos(2 \pi \tilde{n}_i/N)}{\delta x} \: \ , \label{eqn_kpm} \\ 
    k_{{\rm L},i}^{\pm} &= 2\frac{\sin (\pi \tilde{n}_i / N)}{ \delta x} \: \ , \label{eq_latticeMomentum0}
\end{align}
```
where the first expression applies when $\nabla^\pm_i {\tt f}$ is defined on integer lattice sites, and the second when it is defined on half-integer lattice sites.

**Note.** When one wishes to simulate scalar-gauge theories on a lattice, it is important to preserve gauge invariance. For such purpose, one needs to discretize the theory more carefully, in particular using *links* and *plaquettes*, which are quantities purposely defined to build gauge-invariant versions of discretized gauge theories. We refer the reader to [*Lattice gauge invariant techniques*][subsec_LatticeGaugeInv] for a discussion on lattice gauge-invariant techniques; see also Sect.~3.2 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl].


[](){ #subsec_HigherOrder }
**Higher-Order gradients and lattice momenta**

*To be added soon ...*

[](){ #subsec_PS }
## Power spectrum and initial conditions

Given a continuous function ${\tt f} ({\bf x})$, we are often interested on its power spectrum $\Delta_{\tt f}(k)$, which is defined in terms of its ensemble average $\langle {\tt f}^2 \rangle$ as follows,
[](){ #eq_continuumPS }
```math
\begin{equation}
\langle {\tt f}^2 \rangle = \int d\log k~\Delta_{\tt f}(k)~~, ~~~\Delta_{\tt f}(k) \equiv {k^3\over 2\pi^2}\mathcal{P}_{\tt f}(k)~~,~~~ \langle {\tt f}_{\bf k} {\tt f}_{{\bf k}^{\prime}}^* \rangle = (2\pi)^3 \mathcal{P}_{\tt f}(k) \delta (\mathbf{k}-\mathbf{k^{\prime}})~. \label{eq_continuumPS}
\end{equation}
```

On the lattice we substitute the previous ensemble average by an average over the lattice volume $V$, 
[](){ #eq_Averagef2 }
```math
\begin{equation}
\langle {f}^2 \rangle_V = \frac{dx^3}{V}\sum_{\bf n} {f}^2({\bf n}) = \frac{1}{N^3}\sum_{\bf n} {f}^2({\bf n})~\,, \label{eq_Averagef2}
\end{equation}
```
and using the definition of the discrete Fourier transform \eqref{eq_FTdiscrete}, re-write this expression as
[](){ #eq_discretePSaux }
```math
\begin{equation}
\langle {f}^2 \rangle_V = \frac{1}{2\pi}\sum_{|\tilde{\bf n}|}\Delta\log k(\tilde{\bf n}) ~k(\tilde{\bf n})\frac{\delta x}{N^5} \#_{R(\tilde{\bf n})} \big\langle \big|{f}(\tilde{\bf n})\big|^2\big\rangle_{R(\tilde{\bf n})}~\,,
\label{eq_discretePSaux}
\end{equation}
```
where we have defined $\Delta \log k(\tilde{\bf n}) \equiv k_{\rm IR}/k(\tilde{\bf n})$, and introduced $\langle ( ... ) \rangle_{R(\tilde{\bf n})} \equiv \frac{1}{\#_{R(\tilde{\bf n})}}\sum_{\tilde{\bf n}^{\prime}\in R(\tilde{\bf n})}( ... )$ representing an angular average over a spherical shell, $R(\tilde{\bf n})$, that contains all sites with radius $|\tilde{\bf n}^{\prime}| \in \big[|\tilde{\bf n}|,|\tilde{\bf n}|+ \Delta\tilde{n}\big)$, with $\Delta\tilde{n}$ a given radial binning, and $\#_{R(\tilde{\bf n})}$ the *multiplicity*, *i.e.* the number of sites contained within the spherical shell. Comparing Eqs.~(\ref{eq_continuumPS}) and (\ref{eq_discretePSaux}), we can define the lattice power spectrum as follows 
[](){ #eq_discretePST1 }
```math
\begin{eqnarray}\label{eq_discretePST1}
\Delta_{f}(k(|{\bf \tilde{n}}|)) \equiv {k(\tilde{\bf n})\over 2\pi}\frac{\delta x}{N^5} \#_{R(\tilde{\bf n})} \big\langle \big|{f}(\tilde{\bf n})\big|^2\big\rangle_{R(\tilde{\bf n})} = \frac{k^3(\tilde {\bf n})}{2\pi^2}\;{\Upsilon_{|\tilde{\bf n}|}}\;\left(\frac{\delta x}{N}\right)^3 \big\langle \big|{f}(\tilde{\bf n})\big|^2\big\rangle_{R(\tilde{\bf n})}\,,
\end{eqnarray}
```
where 
[](){ #eq_Upsilon }
```math
\begin{eqnarray}\label{eq_Upsilon}
    \Upsilon_{|\tilde{\bf n}|} \equiv \frac{\#_{R(\tilde{\bf n})}}{4\pi|\tilde{\bf n}|^2}\;.
\end{eqnarray}
```
While the most precise evaluation of Eq.~(\ref{eq_discretePST1}) requires to compute $\Upsilon_{|\tilde{\bf n}|}$ exactly (for each bin), *i.e.* using Eq.~(\ref{eq_Upsilon}), many works (specially in the past) often  used the multiplicity approximation $\#_{R(\tilde{\bf n})} \simeq 4\pi |\tilde{\bf n}|^2$. In that case $\Upsilon_{|\tilde{\bf n}|} \simeq 1$, and thus $\Upsilon_{|\tilde{\bf n}|}$ drops from Eq.~(\ref{eq_discretePST1}). While this is only an approximation, for historical reasons we still define two types of power spectra, depending on the multiplicity assumption, 
[](){ #eq_TypeIandIIPS }
```math
\begin{eqnarray}\label{eq_TypeIandIIPS}
\left\lbrace
    \begin{array}{ccll}
         \Upsilon_{|\tilde{\bf n}|} \equiv  \frac{\#_{R(\tilde{\bf n})}}{4\pi|\tilde{\bf n}|^2} \neq 1 & \Rightarrow & \Delta_{f}(k(|{\bf \tilde{n}}|)) \equiv {k(\tilde {\bf n})\delta x\over 2\pi N^5}\#_{|\tilde{\bf n}|}\big\langle\left|f (\tilde{\bf n})\right|^2\big\rangle_{R(\tilde{\bf n})} &  {\tt [Type-I]} \\[0.5em]
          \Upsilon_{|\tilde{\bf n}|} = 1 \;, & \Rightarrow & \Delta_{f}(k(|{\bf \tilde{n}}|)) \simeq \frac{k^3(\tilde {\bf n})}{2\pi^2}\left(\frac{\delta x}{N}\right)^3 \big\langle \big|{f}(\tilde{\bf n})\big|^2\big\rangle_{R(\tilde{\bf n})} & {\tt [Type-II]}
    \end{array}\right.\,.
\end{eqnarray}
```
The definition of ${\tt Type-I}$ spectrum naturally incorporates the exact multiplicity, and hence the actual lack of statistical sampling of supra-Nyquist frequencies $k > {N\over2}k_{\rm IR}$ on a lattice. ${\tt Type-II}$ can be actually seen as a good approximation of {\tt Type-I} spectra for many points in the (reciprocal) lattice, namely in the bins where $\#_{R(\tilde{\bf n})} \approx 4\pi |\tilde{\bf n}|^2$ holds. While such approximation is quite good at intermediate scales on a lattice, it fails onnly moderately for the most infrared modes, and most noticeably it fails significantly for the ultraviolet modes above the *Nyquist* frequency, *i.e.* $k > {N\over2}k_{\rm IR}$. 

!!! note "Spectrum-${\tt Type}$ in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$"
	 While $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ uses by default ${\tt Type-I}$ spectra for its output, it allows the user to switch to ${\tt Type-II}$ if desired. For further details on these aspects, see 
	 [*$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Technical Notes*](TechnicalNotes.md), and read
	 [*Observables*](../Manual/Observables.md) in the [$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual](../Manual/About This Manual.md) (<span style="color:red;">**CHECK !**</span>). 

The notion of power spectrum is particularly useful to initialize fundamental fields on a lattice. In the case of a scalar field, it is common to consider *e.g.* quantum vacuum fluctuations, characterized by a vacuum expectation value (continuum variance) as
[](){ #eq_auto_009 }
```math
\begin{eqnarray}
\mathcal{P}_{\tt f} (k) = \frac{1}{a^3} \frac{1}{2\omega_k}\,,~~~~~~ \mathcal{P}_{\tt f'} (k) = \frac{1}{a^{3-2\alpha}} {\omega_k\over 2}\,, ~~~~{\rm with}~~  \omega_k \equiv \left[(k/a)^2 + m_{\tt f}^2\right]^{1/2}\,,
\label{eq_auto_009}
\end{eqnarray}
```
where $m_{\tt f}^2 \equiv \frac{\partial^2 V({\tt f})}{\partial {\tt f}^2} > 0$. One initializes the field amplitudes ${f}(\tilde{\bf n})$ and time derivatives ${f}'(\tilde{\bf n})$ on the lattice by sampling from a Gaussian distributions $\mathcal{N}[\mu_k,\sigma_k]$ with vanishing mean ($\mu_k = 0$) and variance given by the power spectrum ($\sigma_k^2 \equiv \mathcal{P}_{\tt f}(k)$ or $\sigma_k^2 \equiv \mathcal{P}_{\tt f'}(k)$, respectively), so that
[](){ #eq_varICs }
```math
\begin{eqnarray}\label{eq_varICs}
|\tilde {f}(\tilde{\bf n})|^2  \equiv  {1\over \Upsilon_{|\tilde{\bf n}|}} \left(\frac{{N}}{\delta \tilde{x}}\right)^3 \mathcal{N}\big[0,\mathcal{P}_{\tt f}^{1/2}(k(|{\bf \tilde{n}}|))\big]^2\,,~~~~ |\tilde {f}'(\tilde{\bf n})|^2 \equiv {1\over \Upsilon_{|\tilde{\bf n}|}} \left(\frac{{N}}{\delta \tilde{x}}\right)^3 \mathcal{N}\big[0,\mathcal{P}_{\tt f'}^{1/2}(k(|{\bf \tilde{n}}|))\big]^2\;.
\end{eqnarray}
```
These expressions are, of course, valid for arbitrary power spectra $\mathcal{P}_{\tt f}(k), \mathcal{P}_{\tt f'}(k)$, that one wishes to impose on the initial fluctuations. For a broader discussion on initial conditions from a generic power spectrum, see [*Observables*](../Manual/Observables.md) in the [$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual](../Manual/About This Manual.md)

## Lattice gauge invariant techniques { #subsec_LatticeGaugeInv }

Discretizing a gauge theory requires a special care in order to preserve gauge invariance at the lattice level. It is not enough to recover gauge invariance in the continuum, i.e. in the limit of zero lattice spacing/time step, as gauge invariance is meant to remove spurious transverse degrees of freedom.  If we were to discretize a gauge theory by substituting all ordinary derivatives in the continuum EOM by finite differences like those in Eqs. $~$\eqref{eq_neutrald}, \eqref{eq_forwardbackwardd}, the gauge symmetry would not be preserved and the spurious degrees of freedom would propagate on the lattice. Lattice gauge invariant techniques are meant to avoid this type of trouble.

In order to build an action or EOM for any gauge theory that preserves a discretized version of the gauge symmetry, it is customary to define *link* variables as
[](){ #eq_LinkDef }
```math
\begin{eqnarray}\label{eq_LinkDef}
U_{0,n} \equiv P\exp\left\lbrace-ie\int_{x(n)}^{x(n+\hat0)}dt'A_0 \right\rbrace \approx  e^{-ie\delta t A_0}\,,~~~~
U_{i,n} \equiv P\exp\left\lbrace-ie\int_{x(n)}^{x(n+\hat\imath)} dxA_i \right\rbrace \approx e^{-ie\delta x A_i} ,
\end{eqnarray}
```
where $P\exp\lbrace...\rbrace$ means *path-ordered* along the integration trajectory, as the construction of links is based on the definition of a *parallel transporter*, connecting two points in space-time as $U(x,y) = P\exp\left\lbrace-ie\int_{x}^{y}dx^{\mu}A_\mu  \right\rbrace$. Above, the gauge field $A_\mu$, and hence the link $U_\mu$, is considered to live in the point $n + {\hat\mu\over2}$. We also define $U_{-\mu,n} = U_{\mu,n-\mu}^\dagger \equiv U_\mu^\dagger(n-{1\over 2}\hat\mu)$. In the continuum limit, the gauge fields can be recovered simply from $-i(\mathcal{I}- U_{\mu,n})/(e\delta x^\mu) \longrightarrow  A_\mu\big(n+{1\over2}\hat\mu\big) + \mathcal{O}(\delta x^\mu)$.

!!! note "**Important to know**"
	 To simplify the notation on the lattice, a scalar field living in a generic lattice site $n = (n_o,{\bf n}) = (n_o,n_1,n_2,n_3)$, i.e. $\phi_n = \phi(n)$, will be simply denoted as $\phi$. If the point is displaced in the $\mu-$direction by one unit lattice spacing/time step, $n + \hat\mu$, we use the notation $n+\mu$ or simply by $+\mu$ to indicate this, so that the field amplitude in the new point is expressed as $\phi_{+\mu} \equiv \phi(n+\hat\mu)$. In the case of gauge fields, whenever represented explicitly on the lattice, we will automatically understand that they live in the middle of lattice points, i.e. $A_{\mu} \equiv A_{\mu}(n+{1\over2}\hat\mu)$. It follows then that e.g. $A_{\mu,+\nu} \equiv A_{\mu}\big(n + {1\over2}\hat\mu +  \hat\nu\big)$. In the case of links, we will use the notation $U_\mu \equiv U_{\mu,n} \equiv U_\mu(n+{1\over2}\hat\mu)$, and hence $U_{\mu,\pm\nu} = U_{\mu,n\pm\nu} \equiv U_\mu(n + {1\over2}\hat\mu \pm \hat\nu)$.


One can actually build an action or EOM for any gauge theory, preserving a discretized version of the gauge symmetry, using only link variables and no gauge fields. That is known as the **compact formulation** of lattice gauge theories, and this can be applied to both Abelian and non-Abelian gauge theories. In the case of non-Abelian theories, compact formulations are actually the only way to discretize them while respecting gauge invariance on the lattice. For Abelian gauge theories, however, it is still possible to make use of an explicit representation of the  gauge fields, in the so called **non-compact formulation**. Below we provide both. We introduce standard definitions for *links*, *plaquettes* and *lattice covariant derivatives*, specialized to both Abelian and non-Abelian gauge groups, setting $e=g_A Q_A$. We provide also basic definitions, together with useful approximations and expressions (in the case of Abelian theories for both compact and non-compact formulations). All these ingredients, summarized in the $U(1)$ and $SU(2)$ toolkits below, represent all one needs to know in order to discretize gauge theories while preserving the gauge invariance at the lattice level.

[](){ #eq_U1toolkit }
!!! note "**U(1) toolkit**"
    
    ```math
    
    \begin{align*}\label{eq_U1toolkit}
    &{\rm Links:}  V_{\mu} \equiv e^{-i g_AQ_A \delta x_{\mu} A_{\mu}} = \cos(g_AQ_A\delta x_{\mu} A_{\mu}) - i \sin (g_AQ_A\delta x_{\mu} A_{\mu}) ;  V_{- \mu} \equiv V_{\mu,-\mu}^* ;  V_{\mu}^* V_{\mu} = 1 ;\\
    &{\rm Plaquettes}:  V_{\mu \nu} \equiv V_{\mu} V_{\mu,+\mu} V_{\mu, +\nu}^* V_\nu^* \simeq e^{-i g_AQ_A\delta x_{\mu} \delta x_{\nu} [ F_{\mu \nu} + \mathcal{O}(\delta x)] };  V_{\mu\nu}^* = V_{\nu\mu} ;
    \\
    &{\rm Covariant Derivs.}:  (D_{\mu}^\pm\varphi)({\bf l}) = \pm{1\over \delta x^\mu}(V_{\pm\mu}\varphi_{\pm\mu} - \varphi) , {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}
    \\
    &{\rm Expansions}:
    \left\lbrace
    \begin{array}{rcl}
    (D_{\mu}^\pm\varphi)({\bf l})  & \longrightarrow & (D_{\mu}\varphi)({\bf l}) + \mathcal{O}(\delta x^2)  {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}\\
    \mathcal{R}e\lbrace V_{\mu \nu} \rbrace  & \longrightarrow & 1 - \frac{1}{2} \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2 F_{\mu \nu}^2 + \mathcal{O}(\delta x^5) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}\\ \mathcal{I}m\lbrace V_{\mu \nu} \rbrace & \longrightarrow & - \delta x_{\mu} \delta x_{\nu} g_AQ_AF_{\mu \nu} + \mathcal{O}(\delta x^3) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}
    \end{array}\right.
    \\
    &{\rm Expressions}:
    \left\lbrace
    \begin{array}{l}
    \left.
    \begin{array}{l}
    \sum_n {1\over 4}F_{\mu \nu}^2 \cong -{1\over 2}\sum_n{\mathcal{R}e\lbrace V_{\mu \nu} \rbrace \over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} = -{1\over4}\sum_n {(V_{\mu \nu}+V_{\mu \nu}^*)\over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} + \mathcal{O}(\delta x^2)\\
    \sum_n {1\over4}F_{\mu \nu}^2 \simeq \sum_n {1\over4}{\mathcal{I}m^2\lbrace V_{\mu \nu} \rbrace \over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} = -\sum_n {1\over4}{(V_{\mu \nu}-V_{\mu \nu}^*)^2\over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} + \mathcal{O}(\delta x^2)
    \end{array}\right] ({\tt Compact})\\
    \left.
    \begin{array}{l}
    \sum_n {1\over4}F_{\mu \nu}^2 \simeq {1\over4}\sum_n (\Delta^+_\mu A_\nu - \Delta^+_\nu A_\mu)^2 + \mathcal{O}(\delta x^2)
    \end{array}\right] ({\tt Non-Compact})
    \end{array}\right.
    \\
    &{\rm Gauge Trans}
    \left.
    \left\lbrace
    \begin{array}{cll}
    \phi  & \longrightarrow & e^{+ig_AQ_A\alpha}\phi\\
    A_\mu & \longrightarrow & A_\mu - \Delta_\mu^+\alpha\\
    V_{\pm \mu}  & \longrightarrow & V_{\pm \mu}e^{ig_AQ_A(\alpha_{\pm\mu}-\alpha)}
    \end{array}\right.
    \right]  \Longrightarrow   \left\lbrace
    \begin{array}{cll}
    D_\mu^\pm\phi & \longrightarrow & e^{ig_AQ_A\alpha}(D_\mu^\pm\phi)\\
    V_{\mu\nu}  & \longrightarrow & V_{\mu\nu}  {\rm (gauge inv. !)}
    \end{array}\right.
    
    \end{align*}
    
    ```

<!-- [](){ #eq_SU2toolkit } -->
[](){ #eq_SUNtoolkit }
!!! note "**SU(N) toolkit**"    
    
    ```math
    
    \begin{align*}\label{eq_SUNtoolkit}
    &{\rm Links}:  U_{\mu} \equiv e^{-i g_B Q_B \delta x B_\mu} = e^{-i g_B Q_B \delta x B_{\mu}^a T_a} ;  U_{- \mu} \equiv U_{\mu,-\mu}^{\dagger} ;  U_{\mu}^{\dagger} U_{\mu} = \mathcal{I} \\
    &{\rm Plaquettes}:  U_{\mu \nu} \equiv U_{\mu} U_{\nu,+ \mu} U_{\mu, +\nu}^{\dagger} U_{\nu}^{\dagger} \simeq e^{-ig_B Q_B \delta x_{\mu} \delta x_{\nu} [ G_{\mu \nu}^a T_a + \mathcal{O} (\delta x_{\mu} ) ] } ;   U_{\mu \nu}^\dagger = U_{\nu\mu} \\
    &{\rm Covariant Derivs.}:  (D_{\mu}^\pm\Phi)({\bf l}) = \pm{1\over \delta x^\mu}(U_{\pm\mu}\Phi_{\pm\mu} - \Phi)  \longrightarrow  (D_{\mu}\Phi)({\bf l}) + \mathcal{O}(\delta x^2), {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}
    \\
    &{\rm Expansions}:
    \left\lbrace
    \begin{array}{ccl}
    (D_{\mu}^\pm\Phi)({\bf l}) & \longrightarrow & (D_{\mu}\Phi)({\bf l}) + \mathcal{O}(\delta x^2) , {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}\\
    (U_{\mu \nu} - U_{\mu \nu}^\dagger ) & \longrightarrow & -2ig_B Q_B\delta x_{\mu} \delta x_{\nu}G_{\mu \nu} + \mathcal{O} (\delta x_{\mu}^3) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu} \\ {\rm Tr} [ U_{\mu \nu} ] & \longrightarrow & 2 - \frac{\delta x_{\mu}^2 \delta x_{\nu}^2g_B^2 Q_B^2}{4}\sum_a (G_{\mu \nu}^a)^2 + \mathcal{O} (\delta x_{\mu}^5) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}
    \end{array}
    \right.
    \\
    &{\rm Expressions}:
    \left\lbrace
    \begin{array}{l}
    {1\over2}{\rm Tr}[G_{\mu\nu}G^{\mu\nu}] = {1\over4}\sum_a (G_{\mu\nu}^a)^2 \cong -{{\rm Tr} [ U_{\mu \nu} ]\over \delta x_{\mu}^2 \delta x_{\nu}^2 g_B^2 Q_B^2} + \mathcal{O}(\delta x^2) , \\
    G_{\mu \nu} = G_{\mu \nu}^aT_a \simeq \frac{i}{2\delta x_{\mu} \delta x_{\nu}g_B Q_B} (U_{\mu \nu} - U_{\mu \nu}^\dagger) + \mathcal{O}(\delta x^2)  ,\\
    G_{\mu \nu}^a \simeq \frac{1}{\delta x_{\mu} \delta x_{\nu}g_B Q_B} {\rm Tr} [ (i T_a)  (U_{\mu \nu} - U_{\mu \nu}^\dagger ) ] + \mathcal{O}(\delta x^2)
    \end{array}\right.
    \\
    &{\rm Gauge Trans.}
    \left.
    \left\lbrace
    \begin{array}{cll}
    \Phi  & \longrightarrow & \Omega \Phi ,  \Omega \equiv e^{+ig_B Q_B\alpha_aT_a}\\
    U_{\pm \mu}  & \longrightarrow & \Omega  U_{\pm \mu} \Omega^\dagger_{\pm \mu}
    \end{array}\right.
    \right]  \Longrightarrow   \left\lbrace
    \begin{array}{cll}
    D_\mu^\pm\Phi & \longrightarrow & \Omega (D_\mu^\pm\Phi)\\
    U_{\mu\nu}  & \longrightarrow & \Omega  U_{\mu\nu} \Omega^\dagger
    \\
    {\rm Tr}\lbrace U_{\mu\nu} \rbrace  & \longrightarrow & {\rm Tr}\lbrace U_{\mu\nu} \rbrace
    \end{array}\right.
    \end{align*}
    
    ```

For further discussion on lattice gauge-invariant techniques, see  Sect. 3.2 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ (Ref. [@Figueroa_2020rrl]).

[](){ #subsec_Algorithms }
## Evolution algorithms

The equations of motion (EOM) of standard relativistic fields [Eqs. ([*10*][eq_singlet-eom])–([*14*][eq_SU2eom]) in [*Continuum Field Theory*](Brief_Review_On_Continuum_Dynamics.md)], form a system of coupled second-order hyperbolic partial differential equations (PDE). As the fields propagate in an expanding background, we need to simultaneously consider the EOM for the evolution of the scale factor [Eq. ([*24*][eq_FriedmannD2a]) in [*Continuum Field Theory*](Brief_Review_On_Continuum_Dynamics.md)]. To solve all these equations on a lattice, we need to construct discretized versions of the EOM, and choose suitable integration schemes that satisfy the *Hubble constraint* [represented by Eq. ([*23*][eq_FriedmannHub]) in [*Continuum Field Theory*](Brief_Review_On_Continuum_Dynamics.md)], and in the case of gauge theories, the Gauss constraints [given by Eqs. ([*15*][eq_GaussU1-eom])–([*16*][eq_GaussSU2-eom]) in [*Continuum Field Theory*](Brief_Review_On_Continuum_Dynamics.md)]. During the evolution we need to track the field amplitudes $\{ f_i \}$ and their corresponding conjugate momenta $\{ \pi_i \}$ ($\pi_i \propto \dot f_i$), both of which are evaluated at each lattice site. We also need to track the scale factor amplitude $a(\eta)$ and its conjugate momentum $\pi_a \equiv a'(\eta)$, which contrary to the fields, are homogeneous functions. 

The number of field amplitudes $\{ f_i \}$ defines the number of degrees of freedom (*dof*) in the system. 
The EOM of the fields then take the general form
[](){ #eq_SchemeContVirgin1 }
[](){ #eq_SchemeContVirgin2 }
[](){ #eq_SchemeContVirgin3 }
[](){ #eq_SchemeContVirgin4 }
```math
\begin{eqnarray}
\pi_a(\eta) &=& a'(\eta)\,, \label{eq_SchemeContVirgin1} \\
\pi_a'(\eta) &=& \mathcal{K}_a[a(\eta), E_V(\eta), E_K(\eta), E_G(\eta)]\,, \label{eq_SchemeContVirgin2} \\
\pi_i({\bf x},\eta) &=& \mathcal{D}_i[f_i'({\bf x},\eta),a(\eta),\pi_a(\eta);\lbrace f_j({\bf x},\eta) \rbrace,\lbrace f'_{j\neq i}({\bf x},\eta) \rbrace]\,, \label{eq_SchemeContVirgin3} \\
\pi_i'({\bf x},\eta) &=& \mathcal{K}_i[f_i({\bf x},\eta),\pi_i({\bf x},\eta),a(\eta),\pi_a(\eta);\lbrace f_{j\neq i}({\bf x},\eta) \rbrace,\lbrace \pi_{j\neq i}({\bf x},\eta) \rbrace]\,. \label{eq_SchemeContVirgin4}
\end{eqnarray}
```
where primes denote differentiation with respect to $\alpha$-time. Here $\mathcal{D}_i[...]$ is a functional---the *drift*---that defines the conjugate momentum of the $i$th $dof$, and $\mathcal{K}_i[...]$ is another functional---the *kernel* or *kick*---, that determines the interactions of the $i$th $dof$ with the rest of $dof's$ (possibly including itself). The kernel of the scale factor,  $\mathcal{K}_a[...]$, is given by the $rhs$ of Eq. ([*24*][eq_FriedmannD2a]) from [*Continuum Field Theory*](Brief_Review_On_Continuum_Dynamics.md)], based on the volume average $\langle ... \rangle$ 
of the potential, kinetic and gradient energy densities of the $dof$ involved in the problem, namely $E_V \equiv \langle V \rangle$, $E_{K} \equiv \langle  \sum_j K_{j}\rangle$ and $E_{G} \equiv \langle \sum_j  G_{j}\rangle$.

Similarly, EOM of non-canonical field theory examples are given in the subsection [Non-Canonical Field Theories][subsec_eomNonCanonical] from [*Continuum Field Theory*](Brief_Review_On_Continuum_Dynamics.md)]. These equations take the same form as in Eqs.$~$\eqref{eq_SchemeContVirgin1}-\eqref{eq_SchemeContVirgin4}, with the major difference that their Kernels $\mathcal{K}_i$ contain explicitly a dependence on the conjugate momenta $\pi_i$, whereas in canonical field theory, we can choose variables such that $\mathcal{K}_i$ does not depend on  $\pi_i$ (nor on the other conjugate momenta $\pi_j$). 

We review below time-integration algorithms suitable for both canonical and non-canonical systems. While the algorithms we discuss can be adapted for any system of interactive fields, for clarity we illustrate each method adapting the algorithm to the the case of $N_s$ canonically normalized interacting scalar fields $\{\phi_i\}$.   These are characterized by an action $S = - \int d^4x\, \sqrt{-g}\left(\frac{1}{2}\partial_{\mu} \phi_i \partial^{\mu} \phi_i + V(\lbrace \phi_j \rbrace) \right)$, which, when specialized into a flat FLRW background [see Eq. ([*1*][eq_FLRWlineElem]) from [*Continuum Field Theory*](Brief_Review_On_Continuum_Dynamics.md)] and re-casted in terms of the program variables defined in Eqs.$~$\eqref{eq_ScalarGaugeProgramVar}, can be re-written as
[](){ #eq_ActionScalar }
```math
\begin{eqnarray}
\tilde S  = \left( \frac{\omega_*}{f_*}\right)^2 S = \int d^3\tilde x d \tilde\eta \left\{ \frac{1}{2} a^{3 - \alpha}\sum_i\left({\tilde\phi}_{i}\right)'^{\,2} - \frac{1}{2} a^{1 + \alpha} \sum_{i,k} (\tilde\partial_k \tilde\phi_{i})^2 - a^{3 + \alpha} \widetilde V(\lbrace \tilde\phi_{j} \rbrace) \right\} \, .
 \label{eq_ActionScalar}
\end{eqnarray}
```

While there is no unique way to obtain the discrete version of the EOM (see $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]) for discussion on this), here we adopt a *hybrid* scheme, where at the level of the action only spatial derivatives are discretized, while the temporal coordinate is treated as a continuous variable. We demand recovery of the continuum limit at the level of the action at least to order $\mathcal{O}(dx^2)$. Thus, the action for our reference example, using *e.g.* forward derivatives, reads
[](){ #eq_ActionScDiscHybrib }
```math
\begin{equation}
\widetilde S^{\rm L} = \int d\tilde\eta\sum_{\mathbf n} \delta \tilde x^{\,3} \left\{\frac{ 1}{2} a^{3 - \alpha}\sum_i( \tilde\phi_{i}')^2 - \frac{1}{2} a^{1 + \alpha}   \sum_{i,k} (\widetilde\nabla_k^+ \tilde\phi_{i})^2 - a^{3 + \alpha} \widetilde V (\lbrace \tilde\phi_j \rbrace ) \right\} \, , \label{eq_ActionScDiscHybrib}
\end{equation}
```
and the scalar fields EOM 
[](){ #eq_EOMScalar-Discr_Hybrid }
```math
\begin{equation}\label{eq_EOMScalar-Discr_Hybrid}
\left(a^{3 - \alpha} \tilde\phi_{i}' \right)' = a^{1 + \alpha} \sum_k \widetilde\nabla_k^- \widetilde\nabla_k^+ \tilde\phi_{i} - a^{3 + \alpha} \widetilde V_{,\tilde\phi_{i}}\,,\qquad i = 1, 2, ..., N_s\,.
\end{equation}
```
[](){ #eq_EOMScaleFactor-Discr_Hybrid }
```math
\begin{equation}\label{eq_EOMScaleFactor-Discr_Hybrid}
a'' = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2 a^{1+2\alpha}\Big[ (\alpha - 2){\widetilde E}_{K} + \alpha {{\widetilde E}_{G}} + (\alpha + 1 ) {{\widetilde E}_V} \Big] \,,
\end{equation}
```
with 
[](){ #eq_EK_EG_EV_Discrete }
```math
\begin{eqnarray}\label{eq_EK_EG_EV_Discrete}
{\widetilde E}_K \equiv \frac{1}{2 a^{2\alpha}}\sum_{i}\left\langle (\tilde \phi_i')^2 \right\rangle\,,~~~ {\widetilde E}_G \equiv \frac{1}{2 a^2 }\sum_{i,k} \left\langle (\widetilde \nabla_k^+ \tilde \phi_{i})^2 \right\rangle\,, ~~~{\widetilde E}_V \equiv \left\langle \tilde{V}(\lbrace \tilde\phi_i\rbrace) \right\rangle\,.
\end{eqnarray}
```
From here, one can choose a suitable evolution algorithm to solve Eqs.$~$\eqref{eq_EOMScalar-Discr_Hybrid}~\eqref{eq_EOMScaleFactor-Discr_Hybrid}. As we will see, the hybrid prescription is suitable for either canonical or non-canonical field theories, allowing for a flexible choice of the time-integrator. 

In the following we present a collection of algorithms, divided into *symplectic* and *non-symplectic* integrators. Symplectic integrators include the *Leapfrog* and *Position-* and *Velocity-Verlet* methods, which are very stable numerical algorithms for canonical field theories, allowing for large-time evolution. They can also be extended to higher-order accuracy evolvers, know as *Yoshida* integrators, through recursive compositions of sub-steps. Non-symplectic integrators, on the other hand, are suitable for more general applications, including systems with non-canonical kinetic terms, dissipative dynamics, or interactions containing canonical momenta in the kernels. These algorithms include explicit *Runge–Kutta* schemes of various orders and multi-stage algorithms, which naturally allow for adaptive time-stepping and the use of auxiliary fields to handle intermediate sub-steps. 

[](){ #subsubsec_SymplecticInt }
**Symplectic integrators**

Time integrators that are symplectic represent a class of algorithms tailored for the integration of Hamiltonian systems. The core principle of symplectic methods stems from *Liouville’s theorem*, which states that the phase-space volume must remain conserved throughout the system’s evolution. As a result, the field amplitudes and the corresponding conjugate momenta remain bounded, and they accurately preserve key constraints of the system, such as energy conservation. This property makes them particularly suitable for problems where a long-term dynamical behaviour is of primary importance.

One subtlety of these integrators lies in the importance of a wise choice of the conjugate momenta associated to the *dof*. An "improper" choice may lead to the loss of *symplecticity*, resulting in a degradation of the 
desired numerical stability. This situation arises, for instance, when the choice of conjugate momentum $\pi_i$ associated to the $dof$ $f_i$, leads to a kernel that contains such momentum. In those cases, the application of a symplectic algorithm will not lead to an accurate (or even stable) solution. 

Following with the example of interacting scalar fields introduced in the previous subsection, a convenient choice for the conjugate momenta is the canonical choice [see $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]) for a detailed discussion] 
[](){ #eq_auto_010 }
```math
\begin{equation}
\tilde\pi_{i} \equiv a^{3-\alpha}\tilde\phi_i'\, ,
\label{eq_auto_010}
\end{equation}
```
whereas for the scale factor we use
[](){ #eq_auto_011 }
```math
\begin{equation}
 b = \pi_a \equiv a' \, .
\label{eq_auto_011}
\end{equation}
```
The evolution kernels for our canonically normalised fields read therefore
[](){ #eq_EOMScalar-LatKernel }
[](){ #eq_EOMScaleFactor-LatKernel }
```math
\begin{eqnarray}
\mathcal{K}^{\rm L}_{i}[a,\lbrace \tilde\phi_j \rbrace] &=& a^{1 + \alpha} \sum_k \widetilde\nabla_k^- \widetilde\nabla_k^+ \tilde\phi_{i} - a^{3 + \alpha} \widetilde V_{,\tilde\phi_{i}}\,,\qquad i = 1, 2, ..., N_s\,, \label{eq_EOMScalar-LatKernel} \\
\mathcal{K}^{\rm L}_{a}[a,{\widetilde E}_K,{\widetilde E}_G,{\widetilde E}_V] &=& \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2 a^{1+2\alpha}\Big[ (\alpha - 2){\widetilde E}_{K} + \alpha {{\widetilde E}_{G}} + (\alpha + 1 ) {{\widetilde E}_V} \Big]\,. \label{eq_EOMScaleFactor-LatKernel}
\end{eqnarray}
```

We review now representative cases of symplectic integrators [for en extensive discussion on these, see $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl])]. We present below their concrete implementation for the reference case of scalar interactive singlets.


**I) (Staggered) Leapfrog.** The *leapfrog* algorithm is one of the simplest methods for solving second order differential equations that ensures order $\mathcal{O}(\delta \eta^2)$. It requires that the field amplitudes and their conjugate momenta are displaced between each other by a half-time step $\delta\eta/2$. The same applies to the scale-factor and its derivative. In our case of reference, a convenient choice of the conjugate momenta is
[](){ #eq_auto_012 }
```math
\begin{equation}
\tilde\pi_{i,+0/2}=a_{+0/2}^{3 - \alpha} \widetilde\nabla_0^+ \tilde\phi_{i}\, ,
\label{eq_auto_012}
\end{equation}
```
with the subindex $_{+0/2}$ indicating that the evaluation must be done half time-step ahead. The algorithm consists of a `kick-drift' scheme with discretized EOM as
[](){ #eq_EOMScalar-Discr }
```math
\begin{eqnarray}
\widetilde\nabla_0^- [\tilde\pi_{i,+0/2} ] & = & \mathcal{K}^{\rm L}_{i}[a,\lbrace \tilde\phi_j \rbrace]\, ,~~~~ i = 1, 2, ..., N_s\,,\\
\label{eq_EOMScalar-Discr}
b' & = &  \mathcal{K}^{\rm L}_{a}[a,\overline{{\widetilde E}_{K}},{\widetilde E}_G,{\widetilde E}_V] \,, ~~~~{\rm with}~~ \overline{{\widetilde E}_{K}} \equiv \left({\widetilde E}_{K} + {\widetilde E}_{K,-0/2} \right)/2\,,
\end{eqnarray}
```
where
[](){ #eq_EK_EG_EV_Discrete_2 }
```math
\begin{eqnarray}\label{eq_EK_EG_EV_Discrete_2}
{\widetilde E}_K \equiv \frac{1}{2 a^{2\alpha}_{+0/2} }\sum_{i}\left\langle (\widetilde\nabla_0^+\tilde \phi_i)^2 \right\rangle\,,~~~ {\widetilde E}_G \equiv \frac{1}{2 a^2 }\sum_{i,k} \left\langle (\widetilde \nabla_k^+ \tilde \phi_{i})^2 \right\rangle\,, ~~~{\widetilde E}_V \equiv \left\langle \tilde{V}(\lbrace \tilde\phi_i\rbrace) \right\rangle\, .
\end{eqnarray}
```
An iterative scheme is then written as
[](){ #eq_HCschemeIII }
```math
\begin{eqnarray}
&& IC  :  \lbrace \tilde\phi_i,a \rbrace {\rm ~at~} \tilde\eta_0, ~~~\lbrace \tilde\pi_{i,-{0}/2},b_{-{0}/2}\rbrace {\rm ~at~} \tilde\eta_0-0.5\delta\tilde\eta\, , \nonumber\\[1mm]
&& \left\lbrace
\begin{array}{rcl}
\tilde\pi_{i,+0/2} & = & \tilde\pi_{i,-0/2} + \delta\tilde\eta\mathcal{K}^{\rm L}_{i}[a,\lbrace \tilde\phi_j \rbrace]\, ,\\[1.5mm]
b_{+0/2} &=& b_{-0/2} + \delta\tilde\eta \mathcal{K}^{\rm L}_{a}[a,\overline{{\widetilde E}}_{K},{\widetilde E}_G,{\widetilde E}_V]\, ,\\[1.5mm]
a_{+0} &=&  a + \delta\tilde\eta\, b_{+0/2},\ ~~~~ \longrightarrow ~~~~ a_{+0/2} \equiv (a_{+0} + a)/2\,,\\[1.5mm]
\tilde\phi_{i,+0} &=& \tilde\phi_a + \delta\tilde\eta\,\tilde\pi_{i,+0/2}a_{+0/2}^{-(3-\alpha)}\,,
\end{array}
\right. \\[1mm]
&& HC : b_{+0/2}^2 = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2a_{+0/2}^{2(\alpha+1)} \Big({{\widetilde E}_{K}} + \overline{{\widetilde E}}_{G} + \overline{{\widetilde E}}_{V} \,\Big)\,,\nonumber
\label{eq_HCschemeIII}
\end{eqnarray}
```
where $\overline{{\widetilde E}}_{K} \equiv \left({\widetilde E}_{K, -0/2} + {\widetilde E}_{K,+0/2} \right)/2$, $\overline{{\widetilde E}}_{G} \equiv \left({\widetilde E}_{G} + {\widetilde E}_{G,+0} \right)/2$ and $\overline{{\widetilde E}}_{V} \equiv \left({\widetilde E}_{V} + {\widetilde E}_{V,+0} \right)/2$. Above $IC$ represents the *initial conditions*, whereas $HC$ stands for *Hubble Constraint*.

**II) Velocity- and Position-Verlet.** Verlet methods eliminate the half–time-step offset in the leapfrog method between field amplitudes and conjugate momenta, by either applying the velocity part of the leapfrog algorithm at two successive half–time steps but with a single position update in between, or by applying the coordinate part of the leapfrog algorithm at two successive half–time steps with one velocity update in between. The former prescription is known as the *Velocity-Verlet* (VV) or “kick–drift–kick”  scheme, whereas the latter is known as the *Position-Verlet* (PV) or “drift-kick–drift” scheme. Through the intermediate steps both position and velocity can be obtained after the three steps at integer times, with an accuracy up to order $\mathcal{O}(\delta \eta^2)$. For our reference example of singlet fields, the Verlet iterative schemes read

**II-1).** Velocity Verlet
**II-2).** Position Verlet
$IC: \{\tilde{\phi}_i,\tilde{\pi}_i,a,b\}\ \text{at}\ \tilde{\eta}_0\, ,$
$IC:  \{\tilde{\phi}_i,\tilde{\pi}_i,a,b\}\ \text{at}\ \tilde{\eta}_0\, ,$
[](){ #eq_auto_013 }
```math
\begin{equation}
\left\lbrace
\begin{array}{@{}l}
b_{+0/2} = b +{\dfrac{\delta\tilde\eta}{2}} 
\mathcal{K}^{\rm L}_{a}[a,{\widetilde E}_{K},{\widetilde E}_G,{\widetilde E}_V]\,,\\
\tilde\pi_{i,+0/2} = \tilde\pi^{(b)} + 
{\dfrac{\delta\tilde\eta}{2}} \mathcal{K}^{\rm L}_{i}[a,\{\tilde\phi_j\}]\,,~\\
a_{+0} =  a +  {\delta\tilde\eta}b_{+0/2}\,,~a_{+0/2} = \dfrac{a_{+0}+a}{2}\,,\\
\tilde\phi_{i,+0} = \tilde\phi_i + 
\delta\tilde\eta\,\tilde\pi_{i,+0/2}a_{+0/2}^{-(3-\alpha)}\,,\\
\tilde\pi_{i,+0} = \tilde\pi_{i,+0/2} +
{\dfrac{\delta\tilde\eta}{2}} \mathcal{K}^{\rm L}_{i}[a,\{\tilde\phi_j\}]\big|_{+0}\,,\\
b_{+0} = b_{+0/2} + {\dfrac{\delta\tilde\eta}{2}}  
\mathcal{K}^{\rm L}_{a}[a,{\widetilde E}_{K},{\widetilde E}_G,{\widetilde E}_V]\big|_{+0}\,,
\end{array}
\right.
\nonumber
\label{eq_auto_013}
\end{equation}
```
[](){ #eq_auto_014 }
```math
\begin{equation}
\left\lbrace
\begin{array}{@{}l}
a_{+0/2} =  a + {\dfrac{\delta\tilde\eta}{2}} b\,,\\
\tilde\phi_{i,+0/2} = \tilde\phi_i + 
{\dfrac{\delta\tilde\eta}{2}}\,\tilde\pi_i a^{-(3-\alpha)}\,,\\
\tilde\pi_{i,+0} = \tilde\pi_i +
{\delta\tilde\eta}\,\mathcal{K}^{\rm L}_{i}[a,\{\tilde\phi_j\}]\big|_{+0/2}\,,\\
b_{+0} = b +{\delta\tilde\eta}\,
\mathcal{K}^{\rm L}_{a}[a,\overline{\widetilde{E}}_{K},{\widetilde E}_G,{\widetilde E}_V]\big|_{+0/2}\,,\\
a_{+0} =  a_{+0/2} + {\dfrac{\delta\tilde\eta}{2}} b_{+0}\,,\\
\tilde\phi_{i,+0} = \tilde\phi_{i,+0/2} + 
{\dfrac{\delta\tilde\eta}{2}}\,\tilde\pi_{i,+0}a_{+0}^{-(3-\alpha)}\,,
\end{array}
\right.

\label{eq_auto_014}
\end{equation}
```
$HC: b^2 = \dfrac{1}{3}\!\left(\dfrac{ f_*}{m_p}\right)^2
a^{2(\alpha+1)} \big({\widetilde E}_{K} + {\widetilde E}_{G} + {\widetilde E}_{V}\big)\, ,$
$HC: b^2 = \dfrac{1}{3}\!\left(\dfrac{ f_*}{m_p}\right)^2
a^{2(\alpha+1)} \big({\widetilde E}_{K} + {\widetilde E}_{G} + {\widetilde E}_{V}\big)\, ,$
with $\overline{{\widetilde E}}_{K} \equiv \left({\widetilde E}_{K} + {\widetilde E}_{K,+0} \right)/2$.

**III) Yoshida: Verlet Integration of $\mathcal{O}(\delta\eta^n)$.** The Verlet integration methods can be used recursively to construct higher–order (even) integrators with accuracy $\mathcal{O}(\delta \eta^n)$, with $n = 4, 6, 8, ...$. A single time step $\delta \eta$ is decomposed into $s$ sub-steps, $\delta \eta_p = w_p \delta \eta$, with $\sum_{p=1}^s w_p = 1$, and the corresponding Verlet algorithm is applied sequentially in each sub-step. For instance, the Velocity-Verlet version of this scheme can be written as
[](){ #eq_auto_015 }
```math
\begin{equation}
IC  :  \lbrace \tilde \phi_i^{(0)},\tilde\pi_i^{(0)},a^{(0)},b^{(0)}\rbrace {\rm ~at~} \tilde\eta_0\,,\\\nonumber
\label{eq_auto_015}
\end{equation}
```
[](){ #eq_auto_016 }
```math
\begin{equation}
\left\lbrace
\begin{array}{rcl}
b^{(p)}_{1/2} &=& b^{(p-1)} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{ a}^{{\rm L},(p-1)}\, ,\\[1.5mm]
\tilde\pi^{(p)}_{i,1/2} &=& \tilde\pi_i^{(p-1)} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{i}^{{\rm L},(p-1)}\, ,\\[1.5mm]
a_{1/2}^{(p)} &=&  a^{(p-1)} + b_{1/2}^{(p)}\omega_p{\delta\tilde\eta\over2}\, ,\\[1.5mm]
\tilde\phi^{(p)}_{i} &=& \tilde\phi^{(p-1)}_i + \omega_p\delta\tilde\eta\,\tilde\pi_{i,1/2}^{(p)}(a_{1/2}^{(p)})^{-(3-\alpha)}\, ,\\[1.5mm]
a^{(p)} &=& a^{(p)}_{1/2} +  b^{(p)}_{1/2}\omega_p{\delta\tilde\eta\over2}\,,\\[1.5mm]
\tilde\pi_{i}^{(p)} & = & \tilde\pi^{(p)}_{i,1/2} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{i}^{{\rm L}, (p)}\, ,\\[1.5mm]

b^{(p)} &=& b^{(p)}_{1/2} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{a}^{{\rm L}, (p)}\, ,
\end{array}
\right\rbrace_{p\,=\,1,\, ...,\, s} 
\Longrightarrow
\left\lbrace
\begin{array}{rcl}
\tilde\phi_{i,+0} &=&  \tilde\phi_i^{(s)}\, ,\\[1.5mm]
a_{+0} &=& a^{(s)}\, ,\\[1.5mm]
\tilde\pi_{i,+0} &=& \tilde\pi_i^{(s)}\, ,\\[1.5mm]
b_{+0} &=& b^{(s)}\, , 
\end{array}
\right.
\label{eq_auto_016}
\end{equation}
```
[](){ #eq_auto_017 }
```math
\begin{equation}
HC : b^2 = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2a^{2(\alpha+1)} \Big({{\widetilde E}_{K}} + {{\widetilde E}_{G}} + {{\widetilde E}_{V}} \Big)\, ,\nonumber
\label{eq_auto_017}
\end{equation}
```
where we have compacted the notation introducing  $\mathcal{K}^{{\rm L}, (l)}_{a}=\mathcal{K}^{\rm L}_{a}[a^{(l)},{\widetilde E}_K^{(l)},{\widetilde E}_G^{(l)},{\widetilde E}_V^{(l)}]$ and $\mathcal{K}^{{\rm L}, (l)}_{i}= \mathcal{K}^{\rm L}_{i}[a^{(l)},\lbrace\tilde\phi_{j}^{(l)}\rbrace]$. The sub-index $_{1/2}$ represents intermediate updates of the variables at each iteration step and should not be confused with a half–time-step displacement. Using the appropriate coefficients $w_p$, see Table~\ref{tab_VVnCoeffs} of the Appendix, one achieves a cancellation of truncation errors up to order $\mathcal{O}(\delta \eta^{n})$, with $n = 4, 6, 8,$ and $10$, corresponding to $s = 3, 7, 15,$ and $31$ sub-steps, respectively.

[](){ #subsubsec_NonSymplecticInt }
**Non-symplectic integrators**

Non-symplectic integrators form a versatile set 
of methods with broad applicability, performing well across a variety of systems for which symplectic methods are less suitable, including non-Hamiltonian, dissipative, or stiff ones with canonical momenta appearing in the kernels. Non-symplectic methods can naturally accommodate adaptive time-stepping. 

In these  
schemes, both 
the field amplitudes and their 
conjugate momenta are defined 
at the same time step. 
On the other hand, because these methods involve the execution of intermediate sub-steps, *auxiliary fields* are required to store the information at each stage. In the case of interacting scalar fields, a simple choice for the conjugate momenta,  
[](){ #eq_auto_018 }
```math
\begin{equation}
\tilde{\pi}_i \equiv \tilde{\phi}_i' \, ,
\label{eq_auto_018}
\end{equation}
```
suffices. The evolution kernel takes then the form
[](){ #eq_scalar_singlet_eom }
```math
\begin{equation}\label{eq_scalar_singlet_eom}
\tilde{\pi}'_i = \mathcal{K}^{\rm L}_{i}[a,\lbrace\tilde\phi_{j}\rbrace, b, \tilde{\pi}_{i}] \equiv -(3 - \alpha)\frac{a'}{a}\tilde{\pi}_{i} + a^{-2 (1  - \alpha )} \sum_i \tilde{\nabla}_i^-\tilde{\nabla}_i^+ \tilde{\phi}_i - \widetilde V_{,\tilde\phi_{i}} \; , ~~~~ i = 1, 2, ..., N_s\,.
\end{equation}
```
which we note it depends explicitly on $\tilde{\pi}_{i}$.

**I) Runge-Kutta 2nd order (RK2).** These algorithms provide an evolution scheme accurate to $\mathcal{O}(\delta \eta^2)$ by introducing one intermediate step, whose information is stored in auxiliary fields, one per field *dof*. While there exist several implementations, here we review one of the most common ones, known as the *modified Euler* method,
[](){ #eq_auto_019 }
```math
\begin{equation}
IC  :  \lbrace \tilde \phi_i,\tilde\pi_i,a,b\rbrace {\rm ~at~} \tilde\eta_0\,,\\ \nonumber
\label{eq_auto_019}
\end{equation}
```
[](){ #eq_RK2algorithm_1 }
```math
\begin{equation}

\left\lbrace
\begin{array}{rcl}
\tilde{\phi_i}^{(1)} = \tilde{\phi_i}\,, & \tilde{\phi_i}^{(2)} = \tilde{\phi_i}^{(1)} + \delta\tilde{\eta}\tilde{\pi}^{(1)}_{i}\,,\\[1.5mm]
\tilde{\pi}^{(1)}_{i} = \tilde{\pi}_{i} & \tilde{\pi}^{(2)}_{i} = \tilde{\pi}^{(1)}_{i} + {\delta\tilde{\eta}}\mathcal{K}_{i}^{{\rm L},(1)} \,,\\[1.5mm]

a^{(1)} = a\,, & a^{(2)} = a^{(1)} + {\delta\tilde{\eta}}\tilde{\pi}^{(1)}_a\,,\\[1.5mm]
b^{(1)} = b\,, & b^{(2)} = b^{(1)} + {\delta\tilde{\eta}}\mathcal{K}_{a}^{{\rm L}, (1)}\,,
\end{array}
\right\rbrace\Longrightarrow
\left\lbrace
\begin{array}{rcl}
\tilde{\phi}_{i,+{0}} &=&
\tilde{\phi}^{(1)}_i
+ \frac{1}{2}\delta\tilde{\eta}
\left[\tilde{\pi}_i^{(1)}+\tilde{\pi}_i^{(2)}\right]\,, \\[2mm]
a_{+{0}} &=&
a^{(1)} + \frac{1}{2}\delta\tilde{\eta}
\left[b^{(1)}+b^{(2)}\right]\,, \\[2mm]
\tilde{\pi}_{i,+{0}} &=&
\tilde{\pi}^{(1)}_{i}
+ \frac{1}{2}\delta\tilde{\eta}
\left[\mathcal{K}_{i}^{{\rm L},(1)}+\mathcal{K}_{i}^{{\rm L},(2)}\right]\,, \\[2mm]
b_{+{0}} &=&
b^{(1)} + \frac{1}{2}\delta\tilde{\eta}
\left[\mathcal{K}_{a}^{{\rm L},(1)}+\mathcal{K}_{a}^{{\rm L},(2)}\right]\,,
\end{array}\label{eq_RK2algorithm_1}
\right.
\end{equation}
```
[](){ #eq_auto_020 }
```math
\begin{eqnarray}
HC : b^2 = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2a^{2(\alpha+1)} \Big({{\widetilde E}_{K}} + {{\widetilde E}_{G}} + {{\widetilde E}_{V}} \Big)\,,\nonumber
\label{eq_auto_020}
\end{eqnarray}
```
where again we use $\mathcal{K}^{{\rm L},(l)}_{ i} = \mathcal{K}_{i}^{\rm L}[a^{(l)},\lbrace\tilde\phi_{j}^{(l)}\rbrace,b^{(l)},\tilde{\pi}^{(l)}_{i}]$ and $\mathcal{K}_{a}^{{\rm L}, (l)}=\mathcal{K}_{a}^{\rm L}[a^{(l)},{\widetilde E}_K^{(l)},{\widetilde E}_G^{(l)},{\widetilde E}_V^{(l)}]$.

**II) Runge-Kutta 4th order (RK4).** The accuracy can be increased to $\mathcal{O}(\delta\eta^4)$ by adding a weighted average of four derivative stages in the previous Runge-Kutta algorithm of 2nd order. This leads to the renowned RK4 algorithm as
[](){ #eq_auto_021 }
```math
\begin{equation}
IC :  \lbrace \tilde \phi_i,\tilde\pi_i,a,b\rbrace {\rm ~at~} \tilde\eta_0\,,\\ \nonumber
\label{eq_auto_021}
\end{equation}
```
[](){ #eq_auto_022 }
```math
\begin{equation}
\left.

\begin{array}{c}
\left\lbrace
\begin{array}{llll}
\tilde{\phi}^{(1)}_i = \tilde{\phi}_i\,, & \tilde{\phi}^{(2)}_i = \tilde{\phi}^{(1)}_i + {{\delta\tilde{\eta}}\over2}\tilde{\pi}^{(1)}_{i}\,, & \tilde{\phi}^{(3)}_i = \tilde{\phi}^{(1)}_i + {{\delta\tilde{\eta}}\over2}\tilde{\pi}^{(2)}_{i}\,, & \tilde{\phi}^{(4)}_i = \tilde{\phi}^{(1)} + {\delta\tilde{\eta}}\tilde{\pi}^{(3)}_{i}\,,\\[1.5mm]
\tilde{\pi}^{(1)}_{i} = \tilde{\pi}_{i} & \tilde{\pi}^{(2)}_{i} = \tilde{\pi}^{(1)}_{i} + {{\delta\tilde{\eta}}\over2}\mathcal{K}^{{\rm L}, (1)}_{i}\,, & \tilde{\pi}^{(3)}_{i} = \tilde{\pi}^{(1)}_{i} + {{\delta\tilde{\eta}}\over2}\mathcal{K}^{{\rm L}, (2)}_{i}\,, & \tilde{\pi}^{(4)}_{i} = \tilde{\pi}^{(1)}_{i} + {\delta\tilde{\eta}}\mathcal{K}^{{\rm L}, (3)}_{i}\,,\\[1.5mm]
a^{(1)} = a\,, & a^{(2)} = a^{(1)} + {{\delta\tilde{\eta}}\over2}b^{(1)}\,, & a^{(3)} = a^{(1)} + {{\delta\tilde{\eta}}\over2}b^{(2)}\,, & a^{(4)} = a^{(1)} + {\delta\tilde{\eta}}b^{(3)}\,,\\[1.5mm]
b^{(1)} = b\,, & b^{(2)} = b^{(1)} + {{\delta\tilde{\eta}}\over2}\mathcal{K}_{a}^{{\rm L}, (1)}\,, & b^{(3)} = b^{(1)} + {{\delta\tilde{\eta}}\over2}\mathcal{K}_{a}^{{\rm L},(2)}\,, & b^{(4)} = b^{(1)} + {\delta\tilde{\eta}}\mathcal{K}_{a}^{{\rm L}, (3)}\,, 
\end{array}
\right\rbrace\Longrightarrow\nonumber
\end{array}
\right.
\label{eq_auto_022}
\end{equation}
```

[](){ #eq_RK4algorithm_1 }
```math
\begin{equation}
\Longrightarrow
\left\lbrace
\begin{array}{rcl}
\tilde{\phi}_{i,+0} &=& \tilde{\phi}^{(1)}_i + {1\over6}\delta\tilde{\eta}\left[\tilde{\pi}^{(1)}_{i}+2\tilde{\pi}^{(2)}_{i}+2\tilde{\pi}^{(3)}_{i}+\tilde{\pi}^{(4)}_{i}\right]\,,\\[1.5mm]
a_{+0} &=& a^{(1)} + {1\over6}\delta\tilde{\eta}\left[b^{(1)}+2b^{(2)}+2b^{(3)}+b^{(4)}\right]\,,\\[1.5mm]
\tilde{\pi}_{i,+0}&=&\tilde{\pi}^{(1)}_{i}+{1\over6}\delta\tilde{\eta}\left[\mathcal{K}^{{\rm L},(1)}_{i}+2\mathcal{K}^{{\rm L}, (2)}_{i}+2\mathcal{K}^{{\rm L}, (3)}_{i}+\mathcal{K}^{{\rm L}, (4)}_{i}\right]\,,\\[1.5mm]
b_{+0}&=&b^{(1)}+{1\over6}\delta\tilde{\eta}\left[\mathcal{K}_{a}^{{\rm L}, (1)}+2\mathcal{K}_{a}^{{\rm L}, (2)}+2\mathcal{K}_{a}^{{\rm L},(3)}+\mathcal{K}_{a}^{{\rm L}, (4)}\right]\,,
\end{array}\right. \label{eq_RK4algorithm_1}
\end{equation}
```

[](){ #eq_auto_023 }
```math
\begin{eqnarray}
HC : b^2 = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2a^{2(\alpha+1)} \Big({{\widetilde E}_{K}} + {{\widetilde E}_{G}} + {{\widetilde E}_{V}} \Big)\,.\nonumber
\label{eq_auto_023}
\end{eqnarray}
```

**III) Low-storage Runge-Kutta.** These methods represent a refined version of the previous schemes, in which the number of auxiliary fields is reduced while maintaining the integration accuracy of $\mathcal{O}(\delta \eta^n)$ \cite{Carpenter1994Thirdorder2R,Carpenter1994Fourthorder2R,Bazavov:2021pik,Bazavov:2025dzo,Bazavov:2025exj}. This is achieved by introducing $s$ intermediate sub-stages, each with its corresponding weight coefficient.
[](){ #eq_auto_024 }
```math
\begin{equation}
IC  :  \lbrace \tilde \phi_i^{(0)},\tilde\pi_i^{(0)},a^{(0)},b^{(0)}\rbrace {\rm ~at~} \tilde\eta_0\,,\\ \nonumber
\label{eq_auto_024}
\end{equation}
```

[](){ #eq_auto_025 }
```math
\begin{equation}
\left\lbrace
\begin{array}{rcl}
\Delta \tilde\phi^{(p)}_i
&=& A_p \Delta \tilde\phi^{(p-1)}_i
+ \delta \tilde \eta \tilde\pi_{\phi i}^{(p-1)} \, , \\[1mm]
\Delta\tilde\pi_{i}^{(p)}
&=& A_p\Delta\tilde\pi_{i}^{(p-1)}
+ \delta \tilde \eta \mathcal{K}^{{\rm L}, (p-1)}_{i} \, , \\[1mm]
\Delta a^{(p)}
&=& A_p \Delta a^{(p-1)}
+ \delta \tilde \eta b^{(p-1)} \, , \\[1mm]
\Delta b^{(p)}
&=& A_p\Delta b^{(p-1)}
+ \delta \tilde \eta \mathcal{K}^{{\rm L}, (p-1)}_{a} \, , 
 \end{array}
 \Longrightarrow
 \begin{array}{rcl}
       \tilde\phi^{(p)}_i &=& \tilde\phi^{(p-1)}_i + B_p  \Delta\tilde\phi^{(p)}_i\, ,\\[1.5mm]
        \tilde\pi_{i}^{(p)} &=&\tilde\pi_{i}^{(p-1)}+  B_p \Delta\tilde\pi_{i}^{(p)}\, ,\\[1.5mm]
        a^{(p)} &=&a^{(p-1)} +B_p  \Delta a^{(p)}\, ,\\[1.5mm]
        b^{(p)} &=&b^{(p-1)} +B_p  \Delta b^{(p)}\, , 
\end{array}
\right\rbrace_{p\,=\,1,\, ...,\, s} \Longrightarrow
\nonumber
\label{eq_auto_025}
\end{equation}
```
[](){ #eq_RKLSalgorithm_1 }
```math
\begin{equation}
\Longrightarrow
\left\lbrace
\begin{array}{rcl}
\tilde\phi_{i,+0} &=&  \tilde \phi_i^{(s)}\, ,\\[1.5mm]
a_{+0} &=& a^{(s)}\,,\\[1.5mm]
\tilde\pi_{i,+0} &=& \tilde\pi_i^{(s)}\, ,\\[1.5mm]
b_{+0} &=& b^{(s)}\, ,
\end{array}\label{eq_RKLSalgorithm_1}
\right.
\end{equation}
```

[](){ #eq_auto_026 }
```math
\begin{eqnarray}
HC : b^2 = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2a^{2(\alpha+1)} \Big({{\widetilde E}_{K}} + {{\widetilde E}_{G}} + {{\widetilde E}_{V}} \Big)\,.\nonumber
\label{eq_auto_026}
\end{eqnarray}
```

The auxiliary fields $\{\Delta\tilde{\phi}^{(p)}, \Delta\tilde{\pi}^{(p)}_{i}, \Delta a^{(p)}, \Delta b^{(p)}\}$ are updated with information at each stage $p$ up to a total of $s$ stages, without the need to define additional kernels.

For instance, using 2 intermediate stages and  $\{(A_p, B_p)\} = \{ (0, 1), (-1, 1/2) \}$, we recover the explicit 2nd order RK. Moreover, the accuracy can be increased to third order with 4 intermediate stages  using
$\{(A_p, B_p)\}~ = ~ \{ \,(\,0.0, 0.06688758201974097\,)\,,\,(\,-0.7825460361923583\,, \,2.876554598956719\,),\, ...$
\\$ ~(\,-2.042914325731225\,,\,0.5534657361343982\,),(\,-1.799337253940777\,,\, 0.3912730180961791\,)\,\}\,$, for example. We refer the reader to the Table \ref{tab_RKlsCoefficients} from the Appendix to find the necessary coefficients for other orders/stages.
