!!! note
    The present tab [Field Discretization Primer](Brief_Review_On_Lattice_Techniques.md) contains discussions about basic lattice concepts based on Section 3 of $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]), Section 2 of $\mathtt{The~Art-I}$ (Ref. [@BaezaBallesteros_2025tme]), and Section 3 of $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2026XYZ]). If the reader has already read either of those Sections, or they are simply familiar with scalar and/or gauge field lattice simulations, they can jump right ahead into Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md) or [Scalar-Gauge Interations](My first model of gauge fields.md) of the [$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Manual](About This Manual.md), in order to set up their first singlet scalar or scalar-gauge field lattice simulations, respectively. If the reader is familiar with scalar field lattice simulations but not with gauge lattice field theories, we still recommend them to read the Section [*Lattice gauge invariant techniques*][subsec_LGT], before jumping into [Scalar-Gauge Interations](My first model of gauge fields.md). 

## Canonical field theory in the continuum { #subsec_eomCont }

Throughout this monograph, we consider 
a homogeneous and isotropic universe described by the Friedmann-Lemaitre-Robertson-Walker (FLRW) line element, which we write as
[](){ #eq_FLRWmetric }
```math
\begin{equation}
d s^2 \equiv g_{\mu\nu}d x^\mud x^\nu = - d t^2 + a^2(t) \delta_{ij} d x^i d x^j \ , \label{eq_FLRWmetric}
\end{equation}
```
with $t$ the cosmic time and $a(t)$ the scale factor. It is useful to define a new time coordinate $\eta$ through the relation
[](){ #eq_auto_001 }
```math
\begin{equation} d \eta \equiv a^{-\alpha} (t) dt \ ,\label{eq_auto_001} \end{equation}
```
where $\alpha$ is a real number conveniently chosen for a given problem at hand. We denote it as $\alpha$\textit{-time}, and it represents cosmic and conformal time for $\alpha=0,1$, respectively. We will often let $\alpha$ be an unspecified real number and write all the relevant equations in terms of $\alpha$-time. 

The evolution of the scale factor is dictated by the stress-energy tensor of the matter fields, which in order to be compatible with homogeneity and isotropy, must take the following \textit{perfect fluid} form,
[](){ #eq_stresstensor }
```math
\begin{equation} {\bar T}_{\mu \nu} \equiv (\bar\rho + \bar p )u_{\mu} u_{\nu} + \bar p g_{\mu \nu}  \ , \hspace{0.4cm} g_{\mu \nu} u^{\mu} u^{\nu} = -1 \hspace{0.4cm} \Longrightarrow \hspace{0.4cm}   \begin{cases}
    \bar\rho \equiv a^{-2 \alpha}\,{\bar T}_{00} \ , \vspace{0.2cm}\\
    \bar p \equiv {1\over 3a^2} \sum_j {\bar T}_{jj} \ ,
\end{cases} \label{eq_stresstensor} \end{equation}
```
where $\bar{p}$ and $\bar{\rho}$ are the background values of the total pressure and energy densities of the matter sector, and $u_{\mu} = (a^{\alpha},0,0,0)$
is the four-velocity of a fluid at rest. The evolution of the scale factor is determined by the first and second Friedmann equations,
[](){ #eq_Friedmann-full }
```math
\begin{eqnarray}
    \mathcal{H}^2 \equiv \left({a'\over a}\right)^2 = a^{2 \alpha} \frac{\bar {\rho}}{3 m_p^2} 
    \,,~~~~~~~
    {a''\over a} = \frac{a^{2 \alpha}}{6 m_p^2}\Big[ (2 \alpha - 1) \bar{\rho} - 3 \bar{ p} \Big]\,, \label{eq_Friedmann-full}
\end{eqnarray}
```
where we remind the reader that in our notation ${f}' \equiv \text{d} f/ \text{d} \eta$ and $m_p$ is the reduced Planck mass. 

In this work we present the lattice formulation of different theories involving scalar and/or gauge fields. In order to introduce notation, we review here the equations of motion of a gauge-invariant theory formed by three kinds of canonically-normalized scalar fields: a real scalar singlet $\phi$, a $U(1)$-charged complex scalar field $\varphi$, and a $[SU(N) \times U(1)]$-charged scalar field $\Phi$. In the last two cases, there are also present, respectively, Abelian and non-Abelian gauge fields, $A_{\mu}$ and $C_{\mu} \equiv C_{\mu}^a T_a$, with $T_a$ the $N^2-1$ group generators of $SU(N)$. The scalar fields can be explicitly written in terms of real components as follows,
[](){ #eq_ChargedScalars }
```math
\begin{eqnarray} \label{eq_ChargedScalars}
    \begin{array}{ccccc}
        \phi \in \mathcal{R}e & , &  \varphi \equiv {1\over\sqrt{2}}(\varphi_1 +i\varphi_2) & , & \Phi = \left(
        \begin{array}{c}
            \varphi^{(1)} \\ \varphi^{(2)} \\ \vdots \\ \varphi^{(N)}
        \end{array}
        \right) =
        {1\over\sqrt{2}}
        \left(
        \begin{array}{c}
            \varphi_1 +i\varphi_2 \vspace*{0.1cm}\\ \varphi_3 +i\varphi_3 \\ \vdots \\ \varphi_{2N -1} +i\varphi_{2N}
        \end{array}
        \right) \,.
    \end{array}
\end{eqnarray}
```

More specifically, for canonical scalar-gauge theories we consider the action $S = \int d^4x  \sqrt{-g}\, \mathcal{L}$, with $g \equiv |{\rm det} (g_{\mu \nu})|$, and the Lagrangian
[](){ #eq_lagrangian }
```math
\begin{align} 
-\mathcal{L} = \frac{1}{2}\partial_{\mu} \phi \partial ^{\mu}\phi + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} + V \ ,
\label{eq_lagrangian} 
\end{align}
```
with $V \equiv V(\phi,|\varphi|, |\Phi|)$ the potential describing the interactions between the scalar fields, and where the {\it covariant derivatives} and {\it field strength tensors} associated to the gauge fields, are defined as
[](){ #eq_AbCovDerivCont }
[](){ #eq_CovDerivCont }
```math
\begin{eqnarray}
D_{\mu}^{\rm A}  &\equiv &  \partial _{\mu} - i  g_A Q_AA_\mu \ , \hspace{4cm} F_{\mu \nu}\equiv  \partial_{\mu}  A_{\nu} - \partial_{\nu} A_{\mu} \ , \label{eq_AbCovDerivCont} \\
D_{\mu} & \equiv  &
\mathcal{I}D^{\rm A}_\mu
- i g_C Q_C C_{\mu}^a \,T_a
\ , \hspace{3.13cm}  G_{\mu \nu} \equiv \partial_{\mu} C_{\nu} - \partial_{\nu} C_{\mu} - i[C_\mu,C_\nu]\,,  \label{eq_CovDerivCont}  
\end{eqnarray}
```
with $g_{A}$ and $g_C$ the Abelian and non-Abelian gauge couplings, $Q_{A}$ and $Q_C$ the Abelian and non-Abelian charges of the scalar fields, $\mathcal{I}$ the $N\times N$ identity matrix and $[\cdot,\cdot]$ the usual matrix commutator. The gauge-invariant electric and magnetic fields associated to the Abelian and non-Abelian fields can be written, respectively, as follows,
[](){ #eq_ElectricMagneticDefs }
```math
\begin{equation}\label{eq_ElectricMagneticDefs}
E_i \equiv F_{0i} , \,\,\,\,\,\,\,\,  B_i \equiv \frac{1}{2} \epsilon_{i j k} F^{j k} , \,\,\,\,\,\,\,\,   E_i^a \equiv G_{0i}^a , \,\,\,\,\,\,\,\,  B_i^a \equiv \frac{1}{2} \epsilon_{i j k} G^{j k}_a \ , \end{equation}
```
where $\epsilon_{ijk}$ is the Levi-Civita symbol in three dimensions with normalization $\epsilon_{123}=+1$, and $G_{\mu \nu}^a \equiv {\rm Tr}(2G_{\mu \nu} T_a)$.

The equations of motion for the matter fields and the scale factor have been derived in more detail in {\tt The Art\,I}. Here we simply quote their resulting form, which reads
[](){ #eq_singlet-eom }
[](){ #eq_higgsU1-eom }
[](){ #eq_higgsSU2-eom }
[](){ #eq_U1eom }
[](){ #eq_SU2eom }
[](){ #eq_GaussU1-eom }
[](){ #eq_GaussSU2-eom }
```math
\begin{eqnarray}
    \phi'' - a^{-2(1 - \alpha)} {\vv\nabla}^{\,2} \hspace{-1mm}\phi + (3 - \alpha)\mathcal{H} {\phi'} &=& - a^{2 \alpha} V_{,\phi} \ , \label{eq_singlet-eom} \\
    \varphi'' - a^{-2(1 - \alpha)} {\vv D}_{\hspace{-0.5mm}A}^{\,2}\varphi + (3 - \alpha) \mathcal{H}  {\varphi'} &=& - \frac{a^{2 \alpha}V_{,|\varphi|} }{2} \frac{\varphi}{|\varphi |} \ , \label{eq_higgsU1-eom}\\
    \Phi'' - a^{-2(1 - \alpha)} {\vv D}^{\,2}\Phi + (3 - \alpha) \mathcal{H}  {\Phi'} &=& - \frac{a^{2 \alpha} V_{,|\Phi|}}{2} \frac{\Phi}{|\Phi |} \ , \label{eq_higgsSU2-eom}
    \\
    \partial_0 F_{0i} - a^{-2(1 - \alpha )}\partial_j F_{ji} + (1 - \alpha) \mathcal{H} F_{0i} &=&
    a^{2 \alpha}J^A_i \ , \label{eq_U1eom}
    \\
    (\mathcal{D}_0 )_{a b} (G_{0i})^b - a^{-2(1 - \alpha )} ( \mathcal{D}_j )_{a b} (G_{ji} )^b + (1 - \alpha) \mathcal{H} (G_{0i} )^b &=& a^{2 \alpha}(J_i)_a \ , \label{eq_SU2eom}
    \\
    \partial_i F_{0i} &=& a^2J^A_0 \ , \label{eq_GaussU1-eom}\\
    (\mathcal{D}_i )_{a b} (G_{0i})^b &=& a^2(J_0)_a \ , \label{eq_GaussSU2-eom}
\end{eqnarray}
```
where we have introduced the derivative operators $(\mathcal{D}_{\nu}O)_a = (\mathcal{D}_{\nu})_{a b}O_b \equiv ( \delta_{a b}  \partial_{\nu} - f_{abc} C_{\nu}^c ) O_b$, and defined the matter currents
[](){ #eq_AbelianCurrent }
[](){ #eq_NonAbelianCurrent }
```math
\begin{eqnarray}
    \label{eq_AbelianCurrent}
    J_A^\mu & \equiv & 2g_A Q_A^{(\varphi)} \mathcal{I}m [ \varphi^{*} ( D_A^{\mu} \varphi )] + 2g_A Q_A^{(\Phi)} \mathcal{I}m [ \Phi^\dag (D^{\mu} \Phi  )]\,,\\
    \label{eq_NonAbelianCurrent}
    J_a^\mu & \equiv & 2g_C Q_C\mathcal{I}m [ \Phi^{\dag} T_a( D^{\mu} \Phi )]\,.
\end{eqnarray}
```
Note that Eqs.~\eqref{eq_GaussU1-eom} and \eqref{eq_GaussSU2-eom} are the Gauss constraint of the Abelian and non-Abelian sectors, respectively, which must be preserved at all times during the evolution.

The energy-momentum tensor of a system characterized by a lagrangian $\mathcal{L}$, is given by 
[](){ #eq_auto_002 }
```math
\begin{equation}
T_{\mu \nu} \equiv -\frac{2}{\sqrt{g}}\frac{\delta(\sqrt{g} \mathcal{L})}{\delta g^{\mu \nu}}\,.
\label{eq_auto_002}
\end{equation}
```
This definition leads, using Eqs.~(\ref{eq_stresstensor}) and~(\ref{eq_lagrangian}), to {\it local} expressions for the field's energy and pressure densities, 
[](){ #eq_rhoLocal }
[](){ #eq_pLocal }
```math
\begin{eqnarray}
    \hspace*{-0.5cm}\rho &=& {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} + {G}_{\phi} + {G}_{\varphi} + {G}_{\Phi} + {K}_{U(1)} + {G}_{U(1)} + {K}_{SU(2)} + {G}_{SU(2)} + {V},  \label{eq_rhoLocal}\\
    \hspace*{-0.5cm}p &=& {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} -{1\over3}({G}_{\phi} + {G}_{\varphi} + {G}_{\Phi}) + {1\over3}({K}_{U(1)} + {G}_{U(1)}) + {1\over3}({K}_{SU(2)} + {G}_{SU(2)}) - {V},  \label{eq_pLocal}
\end{eqnarray}
```
with the different energy density contributions given by
[](){ #eq_energy-contrib }
```math
\begin{eqnarray}
\begin{array}{l} \label{eq_energy-contrib}
    {K}_{\phi} = \frac{1}{2 a^{2\alpha} } {\phi'}^2 \vspace{0.1cm}\\
    {K}_{\varphi} = \frac{1}{a^{2\alpha} } (D_0^A \varphi)^*(D_0^A \varphi)
    \vspace{0.1cm}\\
    {K}_{\Phi} = \frac{1}{a^{2\alpha} } (D_0 \Phi )^\dag(D_0 \Phi)
    \vspace{0.1cm}\\
\end{array}
\hspace{0.1cm};\hspace{0.4cm}
\begin{array}{l}
    {G}_{\phi} = \frac{1}{2 a^2} \sum_i (\partial_i \phi)^2
    \vspace{0.1cm}\\
    {G}_{\varphi} = \frac{1}{a^2} \sum_i (D_i^A \varphi)^*(D_i^A \varphi)
    \vspace{0.1cm}\\
    {G}_{\Phi} = \frac{1}{a^2} \sum_i (D_i \Phi)^\dag(D_i \Phi)
    \vspace{0.1cm}\\
\end{array}
\hspace{0.1cm};\hspace{0.4cm}
\begin{array}{l}
    {K}_{U(1)} = \frac{1}{2 a^{2 + 2 \alpha}}  \sum_{i} F_{0i}^2
    \vspace{0.1cm}\\
    {K}_{SU(2)} = \frac{1}{2 a^{2 + 2 \alpha}}  \sum_{a,i} (G_{0i}^a)^2
    \vspace{0.1cm}\\
    {G}_{U(1)} = \frac{1}{2 a^4}  \sum_{i,j<i} F_{ij}^2
    \vspace{0.1cm}\\
    {G}_{SU(2)} = \frac{1}{2 a^4}  \sum_{a,i,j<i}  (G_{ij}^a)^2  \, . \vspace*{0.2cm}\\
\end{array}
\nonumber\\
{\rm(Kinetic-Scalar)} \hspace*{2.15cm} {\rm(Gradient-Scalar)} \hspace*{2.00cm} {\rm (Electric ~\&~ Magnetic)} \hspace{1.0cm} \nonumber\\
\end{eqnarray}
```
If the fields dominate the energy budget of the Universe, the expansion rate can be determined through the Friedmann equations~\eqref{eq_Friedmann-full}, which in our case can be written as
[](){ #eq_FriedmannHubble }
[](){ #eq_FriedmannDDa }
```math
\begin{eqnarray}\label{eq_FriedmannHubble}
    &&\hspace*{-1.0cm}\left({a'\over a}\right)^2 =  \frac{a^{2 \alpha}}{3 m_p^2}\left\langle {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} + {G}_{\phi} + {G}_{\varphi} + {G}_{\Phi} + {K}_{U(1)} + {G}_{U(1)} + {K}_{SU(2)} + {G}_{SU(2)} + {V}\right\rangle\hspace*{-0.5mm},
    \\
    \label{eq_FriedmannDDa}
    &&\hspace*{-0.33cm}{a''\over a} = \frac{a^{2 \alpha}}{3 m_p^2}\big\langle (\alpha-2)({K}_{\phi} + {K}_{\varphi} + {K}_{\Phi}) + \alpha({G}_{\phi} + {G}_{\varphi} + {G}_{\Phi}) + (\alpha + 1)V \\
    &&\hspace*{5.75cm} \left. +~ (\alpha-1)({K}_{U(1)} + {G}_{U(1)} + {K}_{SU(2)} + {G}_{SU(2)}) \right\rangle,\nonumber
\end{eqnarray}
```
with $\langle \dots \rangle$ denoting an average over a sufficiently large volume to encompass all relevant wavelengths of the fields. Typically, we use Eq.~(\ref{eq_FriedmannDDa}) to solve for the scale factor, while monitoring that the constraint equation (\ref{eq_FriedmannHubble}) is verified throughout the evolution to some desired accuracy, see Sect.~\ref{subsec_Algorithms}. 



## Characterization of a lattice { #subsec_Lattice }

A regular cubic lattice in $d=3$ spatial dimensions is fully characterized by two parameters: the number of points per dimension $N$, and the length of each side $L$. The total number of lattice sites 
is therefore $N^3$. 
The ratio between $L$ and $N$ represents the $lattice spacing$,

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

A function ${\tt f}(\bf x)$ defined in the continuum can be represented on a lattice by a discrete function $f({\bf n})$, which takes the same value as the continuum function 
{\it i.e.}~$f({\bf n}) \equiv {\tt f}({\bf x} = {\bf n} \, dx)$. Unless explicitly stated otherwise, we assume {\it periodic boundary conditions} through all three spatial directions, meaning that the function satisfies $f({\bf n} + \hat{\imath} N) = f({\bf n})$ for $i = 1,2,3$, with $ \hat{\imath}$ the unitary vector in the $i$-spatial direction. 
In general, spatially dependent functions ${f}({\bf n})$ on a lattice represent field amplitudes at a given time, so their value will change as the simulation progresses. Therefore, functions on a lattice will depend not only on spatial coordinates $\bf n$ (or reciprocal coordinates $\tilde{\bf n}$, as introduced in the next subsection), but also on a discrete time variable $n_0 = 0, 1, 2, \dots$, counting the number of evolution time-steps. A given time is thus indicated as $\eta = \eta_* + n_0 \delta \eta$, where $\delta \eta$ is the temporal step and $\eta_*$ an initial time. We will therefore treat fields as four-dimensional functions and write them as $f(n) = f(n_0,{\bf n})$ or $f(\tilde n) = f(n_0,\tilde{\bf n})$. Additionally, we denote a one-step time advance using $\hat{0}$, so that, for example, $f(n+\hat 0) = f(n_0+1,{\bf n})$. We will write $f(n+\hat{\mu})$ representing either $f(n_0+1,{\bf n})$ or $f(n_0,{\bf n}+\hat{\imath})$, depending on whether $\mu = 0$ or $\mu = i$. 

[](){ #sub_reciprocal }
**Reciprocal lattice**

For any lattice we can define its \textit{reciprocal lattice} in momentum space, which is also regular and whose points can be tagged by a vector $\tilde{\bf n}$ with the following components,
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
where the weight in the first expression is a consequence of the identity $ \sum_{\bf n} e^{i{2\pi\over N} {\bf n} \tilde {\bf n} } = N^3\delta_{{\bf 0}, \tilde {\bf n}} $. As expected, the Fourier-transformed functions exhibit periodicity in the reciprocal lattice, {\it i.e.}~satisfy periodic boundary condition $ f({\bf\tilde{n}} + {\hat \imath} N) =  f({\bf\tilde{n}}) $.

The \textit{infrared} and \textit{ultraviolet} cutoffs in a lattice, {\it i.e.}~the minimum and maximum momentum resolved by the reciprocal lattice in each dimension, are
[](){ #eq_IRandUVmodes }
```math
\begin{equation} \label{eq_IRandUVmodes}
k_{\rm IR} \equiv \frac{2\pi}{L} = \frac{2\pi}{Ndx}\,, \hspace{0.7cm}  k_{\rm UV} \equiv {N\over2}k_{\rm IR} = {\pi\over dx} \ . 
\end{equation}
```
We note that $k_{\rm UV}$ is also known as the {\it Nyquist} frequency.

The reciprocal lattice hence captures a range of discrete momenta, 
[](){ #eq_auto_006 }
```math
\begin{eqnarray}
{\bf k} = k_{\rm IR} (\tilde{n}_1,\tilde{n}_2,\tilde{n}_3)\,,
\label{eq_auto_006}
\end{eqnarray}
```
with the maximum modulus corresponding to the diagonal of the reciprocal lattice $k_{\rm max}  = \sqrt{3}{N \over 2}k_{\rm IR} = \sqrt{3}\pi / dx$. The modulus of momentum will be indicated as $k = k(\tilde n) 
\equiv k_{\rm IR}|\tilde {\bf n}|$, where $\tilde n = |\tilde {\bf n}| \equiv (\tilde{n}_1^2+\tilde{n}_2^2+\tilde{n}_3^2)^{1/2}$. We note that while the
the number of modes with approximately the same modulus 
grows roughly as $4\pi |\tilde{\bf n}|^2$ for sub-Nyquist modes ($k < k_{\rm UV}$), 
this number  starts decaying abruptly for supra-Nyquist modes ($k > k_{\rm UV}$) as we approach $k_{\rm max}$.

[](){ #subsubsec_ProgramVariables }
**Program variables**

When simulating the evolution of interactive fields on a lattice, it is convenient to work with \textit{program variables}, which are a set of dimensionless field and spacetime variables defined as follows,
[](){ #eq_GaugeProgramVar }
```math
\begin{align}
    \hspace*{-0.4cm}\delta\tilde\eta \equiv a^{- \alpha} \omega_* \delta t\ , \hspace{0.4cm}
    \delta\tilde x^i \equiv \omega_* \delta x\ ,
    \hspace{0.4cm}
    \tilde\phi = \frac{\phi}{f_*} \ , \hspace{0.4cm}
    \tilde\varphi = \frac{\varphi}{f_*} \ , \hspace{0.4cm} \widetilde{\Phi} = \frac{\Phi}{f_*} \ , \hspace{0.4cm}  \widetilde{A}_\mu=\frac{A_\mu }{\omega_*} \ , \hspace{0.4cm} \widetilde C_{\mu}^a = \frac{C_{\mu}^a}{\omega_*} \ , \label{eq_GaugeProgramVar}
\end{align}
```
where $\delta t$ and $\delta x$ are the time-step and lattice spacing used for solving the field dynamics, respectively, and $f_*$ and $\omega_*$ are constants of dimension mass +1. For each problem, one can choose $f_*$ and $\omega_*$ appropriately so that the program variables take numerical values of order unity during the fields' evolution. It is also convenient to define the \textit{program potential} as
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
Finally, we note that as the definition of linear momentum in Sect.~\ref{sub_reciprocal} scales as $k\propto {1/\delta x}$, we naturally normalize the linear momentum on the lattice using the inverse re-scaling for $\delta x$, {\it i.e.}
[](){ #eq_auto_008 }
```math
\begin{eqnarray}
    \kappa \equiv \frac{k}{\omega_*}\,.
\label{eq_auto_008}
\end{eqnarray}
```

[](){ #subsec_LatticeMomentum }
**Gradients and lattice momentum**

When writing the fields' equation of motion (EOM) on the lattice, we need to substitute continuum derivatives by discretized operations that must reproduce the continuum expressions up to some order of accuracy in the lattice spacing/time step. For example, the derivative of a continuous function {\tt f}  
can be approximated by the following \textit{neutral} or \textit{centered} difference,
[](){ #eq_neutrald }
```math
\begin{equation}
    \label{eq_neutrald}
    [\nabla^{(0)}_\mu {\tt f}] = \frac{{\tt f}({n}+\hat\mu) - {\tt f}({n}-\hat\mu)}{2dx ^\mu} ~~\longrightarrow ~~ \partial_{\mu} {\tt f}({x})\big|_{{x}\,\equiv\, {\bf n}dx+n_0\deta} + \mathcal{O}(dx_\mu^2)\,,
\end{equation}
```
where $\delta x^{\mu}$ represents either the time step $\delta \eta$ (for $\mu = 0$) or the lattice spacing $\delta x$ (for $\mu = i$). The expression is symmetric around the lattice point $n$, and recovers the continuum expression up to $\mathcal{O}(\delta x_\mu^2)$. We could also approximate the continuous derivative by the following \textit{charged} difference,
[](){ #eq_forwardbackwardd }
```math
\begin{eqnarray}
    \label{eq_forwardbackwardd}
    [\nabla^\pm_\mu {\tt f}] = \frac{\pm {\tt f}({n}\pm \hat\mu) \mp {\tt f}({n})}{dx^\mu} ~~\longrightarrow ~~ \left\lbrace\begin{array}{l}
        \partial_{\mu} {\tt f}({x})\big|_{{x}\,\equiv\, {\bf n}dx+n_0\deta} + \mathcal{O}(dx_\mu)\,,  \vspace*{0.2cm}\\
        \partial_{\mu} {\tt f}({x})\big|_{{x}\,\equiv\, ({n} \pm \hat\mu/2)dx^\mu} + \mathcal{O}(dx_\mu^2)\,,
    \end{array}\right.
\end{eqnarray}
```
where $\nabla^+_\mu {\tt f}$ and $\nabla^-_\mu {\tt f}$ are called the \textit{forward} and \textit{backward} derivatives, respectively. Compared to the neutral derivative, they have the advantage of being sensitive to the minimum space interval captured by a lattice, {\it i.e.}~to the lattice spacing. These expressions, if expanded around an actual lattice site ${\bf n}$,  only recover the continuum derivative up to $\mathcal{O}({\delta x}_\mu)$. However, if expanded in between the two lattice sites involved, they approximate the continuum expression to $\mathcal{O}({\delta x}_\mu^2)$. One can also implement discrete derivatives of higher order at either grid or half-grid points, involving field values of at more lattice points \cite{DiscreteDerivatives}. 

Associated to each spatial lattice derivative, we can define a {\it lattice momentum} ${\bf k_\text{L}}$ through the following relation in Fourier space,
  [](){ #eqn_latticemomentum }
```math
\begin{equation}  \label{eqn_latticemomentum}  [\nabla_i f]({\tilde{\bf n}}) = -i{\bf k}_{\text{L}}({\tilde{\bf n}}) f({\tilde{\bf n}}) \:. \end{equation}
```
For example, the cartesian components of the lattice momentum for the derivative defined in (\ref{eq_neutrald}) is
[](){ #eqn_k0 }
```math
\begin{equation}
    k^0_{\text{L}, i} = \dfrac{\sin (2\pi \Tilde{n}_i/N)}{\delta x} \: .\label{eqn_k0}\\
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

Finally, we mention that when one wishes to simulate scalar-gauge theories on a lattice, 
it is important to preserve gauge invariance. For such purpose, one needs to discretize the theory more carefully, in particular using {\it links} and {\it plaquettes}, which are quantities purposely defined to build gauge-invariant versions of discretized gauge theories. In this review, only the particular case of local strings 
in Sect.~\ref{sec_DefectsV}, involve scalar-gauge interactions that require to use these techniques, so we do not dwell on them here. We refer the reader to Sect.~3.2 of {\tt The Art\,I} for a discussion on lattice gauge-invariant techniques. 

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
where we have defined $\Delta \log k(\tilde{\bf n}) \equiv k_{\rm IR}/k(\tilde{\bf n})$, and introduced $\langle ( ... ) \rangle_{R(\tilde{\bf n})} \equiv \frac{1}{\#_{R(\tilde{\bf n})}}\sum_{\tilde{\bf n}^{\prime}\in R(\tilde{\bf n})}( ... )$ representing an angular average over a spherical shell, $R(\tilde{\bf n})$, that contains all sites with radius $|\tilde{\bf n}^{\prime}| \in \big[|\tilde{\bf n}|,|\tilde{\bf n}|+ \Delta\tilde{n}\big)$, with $\Delta\tilde{n}$ a given radial binning, and $\#_{R(\tilde{\bf n})} $ the {\it multiplicity}, {\it i.e.}~the number of sites contained within the spherical shell. Comparing Eq.~(\ref{eq_discretePSaux}) with Eq.~(\ref{eq_continuumPS}), we can define the lattice power spectrum as follows 
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
While the most precise evaluation of Eq.~(\ref{eq_discretePST1}) 
requires to compute $\Upsilon_{|\tilde{\bf n}|}$ exactly (for each bin) according to Eq.~(\ref{eq_Upsilon}), many works in the past commonly used the multiplicity approximation $\#_{R(\tilde{\bf n})} \simeq 4\pi |\tilde{\bf n}|^2$, so that $\Upsilon_{|\tilde{\bf n}|} \simeq 1$, hence dropping $\Upsilon_{|\tilde{\bf n}|}$ from Eq.~(\ref{eq_discretePST1}). While this is only an approximation, for historical reasons we define two types of power spectra, depending on the multiplicity assumption, 
[](){ #eq_TypeIandIIPS }
```math
\begin{eqnarray}\label{eq_TypeIandIIPS}
\left\lbrace
    \begin{array}{ccll}
         \Upsilon_{|\tilde{\bf n}|} \equiv  \frac{\#_{R(\tilde{\bf n})}}{4\pi|\tilde{\bf n}|^2} \neq 1 & \Rightarrow & \Delta_{f}(k(|{\bf \tilde{n}}|)) \equiv {k(\tilde {\bf n})\delta x\over 2\pi N^5}\#_{|\tilde{\bf n}|}\big\langle\left|f (\tilde{\bf n})\right|^2\big\rangle_{R(\tilde{\bf n})} &  \text{\tt [Type-I]} \vspace*{4mm}\\
          \Upsilon_{|\tilde{\bf n}|} = 1 \;, & \Rightarrow & \Delta_{f}(k(|{\bf \tilde{n}}|)) \simeq \frac{k^3(\tilde {\bf n})}{2\pi^2}\left(\frac{\delta x}{N}\right)^3 \big\langle \big|{f}(\tilde{\bf n})\big|^2\big\rangle_{R(\tilde{\bf n})} &  \text{\tt [Type-II]}
    \end{array}\right.\,.
\end{eqnarray}
```
The definition of {\tt Type-I} spectrum naturally incorporates the exact multiplicity, and hence the actual lack of statistical sampling of supra-Nyquist frequencies $k > {N\over2}k_{\rm IR}$ on a lattice\footnote{\CL uses by default {\tt Type-I} spectra for its output, but allows the user to switch to {\tt Type-II} if desired.}.
{\tt Type-II} can be actually seen as a good approximation of {\tt Type-I} spectra for many points in the (reciprocal) lattice, namely in the bins where $\#_{R(\tilde{\bf n})} \approx 4\pi |\tilde{\bf n}|^2$ holds. While such approximation is quite good at intermediate scales on a lattice, it fails moderately for the most infrared modes, and most noticeably it fails significantly for the ultraviolet modes above the {\it Nyquist} frequency, {\it i.e.}~$k > {N\over2}k_{\rm IR}$. For further details on these aspects, see~\cite{TechnicalNoteI}. 

The notion of power spectrum is particularly useful to initialize fundamental fields on a lattice. In the case of a scalar field, it is common to consider quantum vacuum fluctuations, characterized by a vacuum expectation value (continuum variance) as
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
For a broader discussion on initial conditions from a generic power spectrum, see Sect.~\ref{subsec_ArbitrarySpectrum}.

[](){ #subsec_Algorithms }
## Evolution algorithms

The equations of motion (EOM) of canonical relativistic fields, Eqs.~(\ref{eq_singlet-eom})–(\ref{eq_SU2eom}), form a system of coupled second-order hyperbolic partial differential equations (PDE). As the fields propagate in an expanding background, we need to simultaneously consider  the EOM for the scale factor, Eq.~(\ref{eq_FriedmannDDa}). To solve all these equations on a lattice, we need to construct discretized versions of the EOM, and choose suitable integration schemes that satisfy the Hubble constraint in Eq.~(\ref{eq_FriedmannHubble}), and in the case of gauge theories, the Gauss constraints in Eqs.~(\ref{eq_GaussU1-eom})-(\ref{eq_GaussSU2-eom}). 
During the evolution 
we need to track 
the field amplitudes $\{ f_i \}$ and their corresponding conjugate momenta $\{ \pi_i \}$ ($\pi_i \propto \dot f_i$), both of which are evaluated at each lattice site. We also need to track 
the scale factor amplitude $a(\eta)$ and its conjugate momentum $\pi_a \equiv a'(\eta)$, which contrary to the fields, are homogeneous functions. 

The number of field amplitudes $\{ f_i \}$ defines the number of degrees of freedom ({\it dof}) in the system. 
The EOM of the fields then take the general form
[](){ #eq_SchemeContVirgin1 }
[](){ #eq_SchemeContVirgin2 }
[](){ #eq_SchemeContVirgin3 }
[](){ #eq_SchemeContVirgin4 }
```math
\begin{eqnarray}\label{eq_SchemeContVirgin1}
\pi_a(\eta) &=& a'(\eta)\,,\\
\label{eq_SchemeContVirgin2}
\pi_a'(\eta) &=& \mathcal{K}_a[a(\eta), E_V(\eta), E_K(\eta), E_G(\eta)]\,,\\
\label{eq_SchemeContVirgin3}
\pi_i({\bf x},\eta) &=& \mathcal{D}_i[f_i'({\bf x},\eta),a(\eta),\pi_a(\eta);\lbrace f_{j}({\bf x},\eta) \rbrace, \lbrace f'_{j\neq i}({\bf x},\eta) \rbrace]\,,\\
\label{eq_SchemeContVirgin4}
\pi_i'({\bf x},\eta) &=& \mathcal{K}_i[f_i({\bf x},\eta),\pi_i({\bf x},\eta),a(\eta),\pi_a(\eta);\lbrace f_{j\neq i}({\bf x},\eta) \rbrace, \lbrace \pi_{j\neq i}({\bf x},\eta) \rbrace]\,,
\end{eqnarray}
```
where primes denote differentiation with respect to $\alpha$-time. Here $\mathcal{D}_i[...]$ is a functional---the {\it drift}---that defines the conjugate momentum of the $i$th $dof$, and $\mathcal{K}_i[...]$ is another functional---the {\it kernel} or {\it kick}---, that determines the interactions of the $i$th $dof$ with the rest of $dof's$ (possibly including itself). The kernel of the scale factor,  $\mathcal{K}_a[...]$, is given by the $rhs$ of Eq.~(\ref{eq_FriedmannDDa}), based on the volume average $\langle ... \rangle$ 
of the potential, kinetic and gradient energy densities of the $dof$ involved in the problem, namely $E_V \equiv \langle V \rangle$, $E_{K} \equiv \langle  \sum_j K_{j}\rangle $ and $E_{G} \equiv \langle \sum_j  G_{j}\rangle$.

This section reviews time-integration algorithms suitable for both canonical and non-canonical systems. While the algorithms we discuss can be adapted for any system of interactive fields, for clarity we illustrate each method adapting the algorithm to the the case of $N_s$ 
canonically normalized interacting scalar fields $\{\phi_i\}$.  
This are characterized by an action $S = - \int d^4x\, \sqrt{-g}\left(\frac{1}{2}\partial_{\mu} \phi_i \partial^{\mu} \phi_i + V(\lbrace \phi_j \rbrace) \right)$,
which, when specialized into a flat FLRW background given in Eq.~(\ref{eq_FLRWmetric}) and 
re-casted in terms of the program variables defined in Eqs.~(\ref{eq_GaugeProgramVar})-(\ref{eq_ProgramPotMultiScalar}), can be re-written as
[](){ #eq_ActionScalar }
```math
\begin{eqnarray}
\tilde S  = \left( \frac{\omega_*}{f_*}\right)^2 S = \int d^3\tilde x d \tilde\eta \left\{ \frac{1}{2} a^{3 - \alpha}\sum_i\left({\tilde\phi}_{i}\right)'^{\,2} - \frac{1}{2} a^{1 + \alpha} \sum_{i,k} (\tilde\partial_k \tilde\phi_{i})^2 - a^{3 + \alpha} \widetilde V(\lbrace \tilde\phi_{j} \rbrace) \right\} \, .
 \label{eq_ActionScalar}
\end{eqnarray}
```

While there is no unique way to obtain the discrete version of the EOM (see {\tt The Art\,I} for discussion on this), in this review we adopt a {\it hybrid} scheme, where at the level of the action only spatial derivatives are discretized\footnote{We demand  to recover the continuum limit at the level of the action at least to order $\mathcal{O}(dx^2)$}, while the temporal coordinate is treated as a continuous variable. Thus, the action for our reference example, using for example forward derivatives, reads
[](){ #eq_ActionScDiscHybrib }
```math
\begin{equation}
\widetilde S^{\rm L} = \int d\tilde\eta\sum_{\bn} \delta \tilde x^{\,3} \left\{\frac{ 1}{2} a^{3 - \alpha}\sum_i( \tilde\phi_{i}')^2 - \frac{1}{2} a^{1 + \alpha}   \sum_{i,k} (\widetilde\nabla_k^+ \tilde\phi_{i})^2 - a^{3 + \alpha} \widetilde V (\lbrace \tilde\phi_j \rbrace ) \right\} \, , \label{eq_ActionScDiscHybrib}
\end{equation}
```
and the scalar fields EOM 
[](){ #eq_EOMScalar-Discr_Hybrid }
[](){ #eq_EOMScaleFactor-Discr_Hybrid }
```math
\begin{eqnarray}
\label{eq_EOMScalar-Discr_Hybrid}
\left(a^{3 - \alpha} \tilde\phi_{i}' \right)' & = & a^{1 + \alpha} \sum_k \widetilde\nabla_k^- \widetilde\nabla_k^+ \tilde\phi_{i}  -  a^{3 + \alpha} \widetilde V_{,\tilde\phi_{i}}\,,~~~~ i = 1, 2, ..., N_s\, , \\
\label{eq_EOMScaleFactor-Discr_Hybrid}
a'' & = &  \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2 a^{1+2\alpha}\Big[ (\alpha - 2){\widetilde E}_{K}  + \alpha {{\widetilde E}_{G}} + (\alpha + 1 ) {{\widetilde E}_V} \Big] \,,
\end{eqnarray}
```
with 
[](){ #eq_EK_EG_EV_Discrete }
```math
\begin{eqnarray}\label{eq_EK_EG_EV_Discrete}
{\widetilde E}_K \equiv \frac{1}{2 a^{2\alpha}}\sum_{i}\left\langle (\tilde \phi_i')^2 \right\rangle\,,~~~ {\widetilde E}_G \equiv \frac{1}{2 a^2 }\sum_{i,k} \left\langle (\widetilde \nabla_k^+ \tilde \phi_{i})^2 \right\rangle\,, ~~~{\widetilde E}_V \equiv \left\langle \tilde{V}(\lbrace \tilde\phi_i\rbrace) \right\rangle\,.
\end{eqnarray}
```
From here, one can choose a suitable evolution algorithm to solve Eqs.~(\ref{eq_EOMScalar-Discr_Hybrid})-(\ref{eq_EOMScaleFactor-Discr_Hybrid}). 
As we will see, the hybrid prescription is specially suitable for the examples considered in this review, as it allows for a flexible choice of the time-integrator. 

In the following we present a collection of algorithms, 
divided into {\it symplectic} and {\it non-symplectic} integrators. Symplectic integrators include the {\it Leapfrog} and {\it Position-} and {\it Velocity-Verlet} methods, which are very stable numerical algorithms for canonical field theories, 
allowing for large-time evolution. They can also be extended 
to higher-order accuracy evolvers, know as {\it Yoshida} integrators, through recursive compositions of sub-steps. Non-symplectic integrators, on the other hand, are suitable for more general applications, including systems with non-canonical kinetic terms, dissipative dynamics, or interactions containing canonical momenta in the kernels. These algorithms include explicit {\it Runge–Kutta} schemes of various orders and multi-stage algorithms, which naturally allow for adaptive time-stepping and the use of auxiliary fields to handle intermediate sub-steps. Many of the complex models discussed in this review are non-canonical, and hence non-symplectic integrators are necessary to maintain accuracy and stability of the numerical solutions.

[](){ #subsubsec_SymplecticInt }
**Symplectic integrators**

Time integrators that are symplectic represent a class of algorithms tailored for the integration of Hamiltonian systems. The core principle of symplectic methods stems from {\it Liouville’s theorem}, which states that the phase-space volume must remain conserved throughout the system’s evolution. As a result, the field amplitudes and the corresponding conjugate momenta remain bounded, and they accurately preserve key constraints of the system, such as energy conservation. This property makes them particularly suitable for problems where a long-term dynamical behaviour is of primary importance.

One subtlety of these integrators lies in the importance of a wise choice of the conjugate momenta associated to the {\it dof}. 
An `improper' choice may lead to the loss of 
`symplecticity', resulting in a degradation of the 
desired 
numerical stability. This situation arises, for instance, when the choice of conjugate momentum $\pi_i$ associated to the $dof$ $f_i$, leads to a kernel that contains such momentum. 
In those cases, the application of a symplectic algorithm will not lead to an accurate (or even stable) solution. 

Following with the example of interacting scalar fields introduced in the previous subsection, a convenient choice for the conjugate momenta is the canonical choice (see {\tt The Art\,I} for a detailed discussion) 
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
\label{eq_EOMScalar-LatKernel}
\mathcal{K}^{\rm L}_{i}[a,\lbrace \tilde\phi_j \rbrace] & = & a^{1 + \alpha} \sum_k \widetilde\nabla_k^- \widetilde\nabla_k^+ \tilde\phi_{i}  -  a^{3 + \alpha} \widetilde V_{,\tilde\phi_{i}}\,,~~~~ i = 1, 2, ..., N_s\,, \\
\label{eq_EOMScaleFactor-LatKernel}
\mathcal{K}^{\rm L}_{a}[a,{\widetilde E}_K,{\widetilde E}_G,{\widetilde E}_V] & = & \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2 a^{1+2\alpha}\Big[ (\alpha - 2){\widetilde E}_{K}  + \alpha {{\widetilde E}_{G}} + (\alpha + 1 ) {{\widetilde E}_V} \Big] \,,
\end{eqnarray}
```

We review now representative cases of symplectic integrators (for en extensive discussion on these, see {\tt The Art-I}). To this end, we present their concrete implementation for the reference case of scalar interactive singlets.\\


\textbf{I) (Staggered) Leapfrog}. The {\it leapfrog} algorithm is one of the simplest methods for solving second order differential equations that ensures order $\mathcal{O}(\delta \eta^2)$. It requires that the field amplitudes and their conjugate momenta are displaced between each other by a half-time step $\delta\eta/2$. The same applies to the scale-factor and its derivative. In our case of reference, a convenient choice of the conjugate momenta is
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
&& \hspace*{2mm}IC  :  \lbrace \tilde\phi_i,a \rbrace {\rm ~at~} \tilde\eta_0, ~~~\lbrace \tilde\pi_{i,-{0}/2},b_{-{0}/2}\rbrace {\rm ~at~} \tilde\eta_0-0.5\delta\tilde\eta\, , \nonumber\\[1mm]
&& \left\lbrace
\begin{array}{rcl}
\tilde\pi_{i,+0/2} & = & \tilde\pi_{i,-0/2} + \delta\tilde\eta\mathcal{K}^{\rm L}_{i}[a,\lbrace \tilde\phi_j \rbrace]\, , \vspace*{0.15cm}\\
b_{+0/2} &=& b_{-0/2} + \delta\tilde\eta \mathcal{K}^{\rm L}_{a}[a,\overline{{\widetilde E}}_{K},{\widetilde E}_G,{\widetilde E}_V]\, , \vspace*{0.15cm}\\
a_{+0} &=&  a + \delta\tilde\eta\, b_{+0/2},\ ~~~~ \longrightarrow ~~~~ a_{+0/2} \equiv (a_{+0} + a)/2\,,\\
\tilde\phi_{i,+0} &=& \tilde\phi_a + \delta\tilde\eta\,\tilde\pi_{i,+0/2}a_{+0/2}^{-(3-\alpha)}\,,\vspace*{0.15cm}\\
\end{array}
\right. \\[1mm]
&& \hspace*{2mm}HC : b_{+0/2}^2 = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2a_{+0/2}^{2(\alpha+1)} \Big({{\widetilde E}_{K}} + \overline{{\widetilde E}}_{G} + \overline{{\widetilde E}}_{V} \,\Big)\,,\nonumber
\label{eq_HCschemeIII}
\end{eqnarray}
```
where $\overline{{\widetilde E}}_{K} \equiv \left({\widetilde E}_{K, -0/2} + {\widetilde E}_{K,+0/2} \right)/2$, $\overline{{\widetilde E}}_{G} \equiv \left({\widetilde E}_{G} + {\widetilde E}_{G,+0} \right)/2$ and $\overline{{\widetilde E}}_{V} \equiv \left({\widetilde E}_{V} + {\widetilde E}_{V,+0} \right)/2$. Above $IC$ represents the {\it initial conditions}, whereas $HC$ stands for {\it Hubble Constraint}.\\

\textbf{II) Velocity- and Position-Verlet}. Verlet methods eliminate the half–time-step offset in the leapfrog method between field amplitudes and conjugate momenta, by either applying the velocity part of the leapfrog algorithm at two successive half–time steps but with a single position update in between, or by applying the coordinate part of the leapfrog algorithm at two successive half–time steps with one velocity update in between. The former prescription is known as the {\it Velocity-Verlet} (VV) or “kick–drift–kick”  scheme, whereas the latter is known as the {\it Position-Verlet} (PV) or “drift-kick–drift” scheme. Through the intermediate steps both position and velocity can be obtained after the three steps at integer times, with an accuracy up to order $\mathcal{O}(\delta \eta^2)$. For our reference example of singlet fields, the Verlet iterative schemes read\\

\hspace{0.15cm}\textbf{II-1)} Velocity Verlet
\textbf{II-2)} Position Verlet
\hspace{0.45cm}$IC: \{\tilde{\phi}_i,\tilde{\pi}_i,a,b\}\ \text{at}\ \tilde{\eta}_0\, ,$
\hspace{0.25cm}$IC:  \{\tilde{\phi}_i,\tilde{\pi}_i,a,b\}\ \text{at}\ \tilde{\eta}_0\, ,$
[](){ #eq_auto_013 }
```math
\begin{equation}
\vcenter{\hbox{%
$\displaystyle
\left\lbrace
\begin{array}{@{}l}
b_{+0/2} = b +{\dfrac{\delta\tilde\eta}{2}} 
\mathcal{K}^{\rm L}_{a}[a,{\widetilde E}_{K},{\widetilde E}_G,{\widetilde E}_V]\,,\\\vspace{0.15cm}
\tilde\pi_{i,+0/2} = \tilde\pi^{(b)} + 
{\dfrac{\delta\tilde\eta}{2}} \mathcal{K}^{\rm L}_{i}[a,\{\tilde\phi_j\}]\,,~\\\vspace{0.15cm}
a_{+0} =  a +  {\delta\tilde\eta}b_{+0/2}\,,~a_{+0/2} = \dfrac{a_{+0}+a}{2}\,,\\\hspace{0.15cm}
\tilde\phi_{i,+0} = \tilde\phi_i + 
\delta\tilde\eta\,\tilde\pi_{i,+0/2}a_{+0/2}^{-(3-\alpha)}\,,\\\vspace{0.15cm}
\tilde\pi_{i,+0} = \tilde\pi_{i,+0/2} +
{\dfrac{\delta\tilde\eta}{2}} \mathcal{K}^{\rm L}_{i}[a,\{\tilde\phi_j\}]\big|_{+0}\,,\\\vspace{0.15cm}
b_{+0} = b_{+0/2} + {\dfrac{\delta\tilde\eta}{2}}  
\mathcal{K}^{\rm L}_{a}[a,{\widetilde E}_{K},{\widetilde E}_G,{\widetilde E}_V]\big|_{+0}\,,
\end{array}
\right.
$}}\nonumber
\label{eq_auto_013}
\end{equation}
```
[](){ #eq_auto_014 }
```math
\begin{equation}
\vcenter{\hbox{%
$\displaystyle
\left\lbrace
\begin{array}{@{}l}
a_{+0/2} =  a + {\dfrac{\delta\tilde\eta}{2}} b\,,\\\vspace{0.15cm}
\tilde\phi_{i,+0/2} = \tilde\phi_i + 
{\dfrac{\delta\tilde\eta}{2}}\,\tilde\pi_i a^{-(3-\alpha)}\,,\\\vspace{0.15cm}
\tilde\pi_{i,+0} = \tilde\pi_i +
{\delta\tilde\eta}\,\mathcal{K}^{\rm L}_{i}[a,\{\tilde\phi_j\}]\big|_{+0/2}\,,\\\vspace{0.15cm}
b_{+0} = b +{\delta\tilde\eta}\,
\mathcal{K}^{\rm L}_{a}[a,\overline{\widetilde{E}}_{K},{\widetilde E}_G,{\widetilde E}_V]\big|_{+0/2}\,,\\\hspace{0.15cm}
a_{+0} =  a_{+0/2} + {\dfrac{\delta\tilde\eta}{2}} b_{+0}\,,\\\vspace{0.15cm}
\tilde\phi_{i,+0} = \tilde\phi_{i,+0/2} + 
{\dfrac{\delta\tilde\eta}{2}}\,\tilde\pi_{i,+0}a_{+0}^{-(3-\alpha)}\,,
\end{array}
\right.
$}}
\label{eq_auto_014}
\end{equation}
```
\hspace{0.45cm}$HC: b^2 = \dfrac{1}{3}\!\left(\dfrac{ f_*}{m_p}\right)^2
a^{2(\alpha+1)} \big({\widetilde E}_{K} + {\widetilde E}_{G} + {\widetilde E}_{V}\big)\, ,$
\hspace{0.25cm}$HC: b^2 = \dfrac{1}{3}\!\left(\dfrac{ f_*}{m_p}\right)^2
a^{2(\alpha+1)} \big({\widetilde E}_{K} + {\widetilde E}_{G} + {\widetilde E}_{V}\big)\, ,$\\
with $\overline{{\widetilde E}}_{K} \equiv \left({\widetilde E}_{K} + {\widetilde E}_{K,+0} \right)/2$.

\textbf{III) Yoshida: Verlet Integration of $\mathcal{O}(\delta\eta^n)$}. The Verlet integration methods can be used recursively to construct higher–order (even) integrators with accuracy $\mathcal{O}(\delta \eta^n)$, with $n = 4, 6, 8, ...$. A single time step $\delta \eta$ is decomposed into $s$ sub-steps, $\delta \eta_p = w_p \delta \eta$, with $\sum_{p=1}^s w_p = 1$, and the corresponding Verlet algorithm is applied sequentially in each sub-step. For instance, the Velocity-Verlet version of this scheme can be written as
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
b^{(p)}_{1/2} &=& b^{(p-1)} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{ a}^{{\rm L},(p-1)}\, ,\vspace*{0.15cm}\\
\tilde\pi^{(p)}_{i,1/2} &=& \tilde\pi_i^{(p-1)} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{i}^{{\rm L},(p-1)}\, ,\vspace*{0.15cm}\\
a_{1/2}^{(p)} &=&  a^{(p-1)} + b_{1/2}^{(p)}\omega_p{\delta\tilde\eta\over2}\, ,\vspace*{0.15cm}\\
\tilde\phi^{(p)}_{i} &=& \tilde\phi^{(p-1)}_i + \omega_p\delta\tilde\eta\,\tilde\pi_{i,1/2}^{(p)}(a_{1/2}^{(p)})^{-(3-\alpha)}\, ,\vspace*{0.15cm}\\
a^{(p)} &=& a^{(p)}_{1/2} +  b^{(p)}_{1/2}\omega_p{\delta\tilde\eta\over2}\,,\vspace*{0.15cm}\\
\tilde\pi_{i}^{(p)} & = & \tilde\pi^{(p)}_{i,1/2} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{i}^{{\rm L}, (p)}\, ,
\vspace*{0.15cm}\\
b^{(p)} &=& b^{(p)}_{1/2} + \omega_p{\delta\tilde\eta\over 2}\mathcal{K}_{a}^{{\rm L}, (p)}\, ,
\end{array}
\right\rbrace_{p\,=\,1,\, ...,\, s} \hspace*{-1cm}
\Longrightarrow
\left\lbrace
\begin{array}{rcl}
\tilde\phi_{i,+0} &=&  \tilde\phi_i^{(s)}\, , \vspace*{0.15cm}\\
a_{+0} &=& a^{(s)}\, , \vspace*{0.15cm}\\
\tilde\pi_{i,+0} &=& \tilde\pi_i^{(s)}\, , \vspace*{0.15cm}\\
b_{+0} &=& b^{(s)}\, ,\vspace*{0.15cm} 
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
On the other hand, because these methods involve the execution of intermediate sub-steps, {\it auxiliary fields} are required to store the information at each stage. In the case of interacting scalar fields, a simple choice for the conjugate momenta,  
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
\tilde{\pi}'_i = \mathcal{K}^{\rm L}_{i}[a,\lbrace\tilde\phi_{j}\rbrace, b, \tilde{\pi}_{i}] \equiv -(3 - \alpha)\frac{a'}{a}\tilde{\pi}_{i} + a^{-2 (1  - \alpha )} \sum_i \tilde{\nabla}_i^-\tilde{\nabla}_i^+ \tilde{\phi}_i - \widetilde V_{,\tilde\phi_{i}} \; , ~~~~ i = 1, 2, ..., N_s\,.\\
\end{equation}
```
which we note it depends explicitly on $\tilde{\pi}_{i}$.\\ 

\textbf{I) Runge-Kutta 2nd order (RK2)}. These algorithms provide an evolution scheme accurate to $\mathcal{O}(\delta \eta^2)$ by introducing one intermediate step, whose information is stored in auxiliary fields, one per field \textit{dof}. While there exist several implementations, here we review one of the most common ones, known as the \textit{modified Euler} method,
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
\vspace{-0.75cm}
\left\lbrace
\begin{array}{rcl}
\tilde{\phi_i}^{(1)} = \tilde{\phi_i}\,, & \tilde{\phi_i}^{(2)} = \tilde{\phi_i}^{(1)} + \delta\tilde{\eta}\tilde{\pi}^{(1)}_{i}\,,\vspace*{0.15cm}\\
\tilde{\pi}^{(1)}_{i} = \tilde{\pi}_{i} & \tilde{\pi}^{(2)}_{i} = \tilde{\pi}^{(1)}_{i} + {\delta\tilde{\eta}}\mathcal{K}_{i}^{{\rm L},(1)} \,,
\vspace*{0.15cm}\\
a^{(1)} = a\,, & a^{(2)} = a^{(1)} + {\delta\tilde{\eta}}\tilde{\pi}^{(1)}_a\,,\vspace*{0.15cm}\\
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
where again we use $\mathcal{K}^{{\rm L},(l)}_{ i} = \mathcal{K}_{i}^{\rm L}[a^{(l)},\lbrace\tilde\phi_{j}^{(l)}\rbrace,b^{(l)},\tilde{\pi}^{(l)}_{i}]$ and $\mathcal{K}_{a}^{{\rm L}, (l)}=\mathcal{K}_{a}^{\rm L}[a^{(l)},{\widetilde E}_K^{(l)},{\widetilde E}_G^{(l)},{\widetilde E}_V^{(l)}]$.\\

\textbf{II) Runge-Kutta 4th order (RK4)}. The accuracy can be increased to $\mathcal{O}(\delta\eta^4)$ by adding a weighted average of four derivative stages in the previous Runge-Kutta algorithm of 2nd order. This leads to the renowned RK4 algorithm as
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
\hspace{7.5mm}
\begin{array}{c}
\left\lbrace
\begin{array}{llll}
\tilde{\phi}^{(1)}_i = \tilde{\phi}_i\,, & \tilde{\phi}^{(2)}_i = \tilde{\phi}^{(1)}_i + {{\delta\tilde{\eta}}\over2}\tilde{\pi}^{(1)}_{i}\,, & \tilde{\phi}^{(3)}_i = \tilde{\phi}^{(1)}_i + {{\delta\tilde{\eta}}\over2}\tilde{\pi}^{(2)}_{i}\,, & \tilde{\phi}^{(4)}_i = \tilde{\phi}^{(1)} + {\delta\tilde{\eta}}\tilde{\pi}^{(3)}_{i}\,,\vspace*{0.15cm}\\
\tilde{\pi}^{(1)}_{i} = \tilde{\pi}_{i} & \tilde{\pi}^{(2)}_{i} = \tilde{\pi}^{(1)}_{i} + {{\delta\tilde{\eta}}\over2}\mathcal{K}^{{\rm L}, (1)}_{i}\,, & \tilde{\pi}^{(3)}_{i} = \tilde{\pi}^{(1)}_{i} + {{\delta\tilde{\eta}}\over2}\mathcal{K}^{{\rm L}, (2)}_{i}\,, & \tilde{\pi}^{(4)}_{i} = \tilde{\pi}^{(1)}_{i} + {\delta\tilde{\eta}}\mathcal{K}^{{\rm L}, (3)}_{i}\,,\vspace*{0.15cm}\\
a^{(1)} = a\,, & a^{(2)} = a^{(1)} + {{\delta\tilde{\eta}}\over2}b^{(1)}\,, & a^{(3)} = a^{(1)} + {{\delta\tilde{\eta}}\over2}b^{(2)}\,, & a^{(4)} = a^{(1)} + {\delta\tilde{\eta}}b^{(3)}\,,\vspace*{0.15cm}\\
b^{(1)} = b\,, & b^{(2)} = b^{(1)} + {{\delta\tilde{\eta}}\over2}\mathcal{K}_{a}^{{\rm L}, (1)}\,, & b^{(3)} = b^{(1)} + {{\delta\tilde{\eta}}\over2}\mathcal{K}_{a}^{{\rm L},(2)}\,, & b^{(4)} = b^{(1)} + {\delta\tilde{\eta}}\mathcal{K}_{a}^{{\rm L}, (3)}\,, 
\end{array}
\right\rbrace\Longrightarrow\nonumber
\end{array}
\right.
\label{eq_auto_022}
\end{equation}
```
\\
[](){ #eq_RK4algorithm_1 }
```math
\begin{equation}
\Longrightarrow
\left\lbrace
\begin{array}{rcl}
\tilde{\phi}_{i,+0} &=& \tilde{\phi}^{(1)}_i + {1\over6}\delta\tilde{\eta}\left[\tilde{\pi}^{(1)}_{i}+2\tilde{\pi}^{(2)}_{i}+2\tilde{\pi}^{(3)}_{i}+\tilde{\pi}^{(4)}_{i}\right]\,,\vspace*{0.15cm}\\
a_{+0} &=& a^{(1)} + {1\over6}\delta\tilde{\eta}\left[b^{(1)}+2b^{(2)}+2b^{(3)}+b^{(4)}\right]\,,\vspace*{0.2cm}\\
\tilde{\pi}_{i,+0}&=&\tilde{\pi}^{(1)}_{i}+{1\over6}\delta\tilde{\eta}\left[\mathcal{K}^{{\rm L},(1)}_{i}+2\mathcal{K}^{{\rm L}, (2)}_{i}+2\mathcal{K}^{{\rm L}, (3)}_{i}+\mathcal{K}^{{\rm L}, (4)}_{i}\right]\,,\vspace*{0.15cm}\\
b_{+0}&=&b^{(1)}+{1\over6}\delta\tilde{\eta}\left[\mathcal{K}_{a}^{{\rm L}, (1)}+2\mathcal{K}_{a}^{{\rm L}, (2)}+2\mathcal{K}_{a}^{{\rm L},(3)}+\mathcal{K}_{a}^{{\rm L}, (4)}\right]\,,\vspace*{0.15cm}\\
\end{array}\right. \label{eq_RK4algorithm_1}
\end{equation}
```
\\
[](){ #eq_auto_023 }
```math
\begin{eqnarray}
HC : b^2 = \frac{1}{3} \left( \frac{ f_*}{m_p} \right)^2a^{2(\alpha+1)} \Big({{\widetilde E}_{K}} + {{\widetilde E}_{G}} + {{\widetilde E}_{V}} \Big)\,.\nonumber
\label{eq_auto_023}
\end{eqnarray}
```

\textbf{III) Low-storage Runge-Kutta}. These methods represent a refined version of the previous schemes, in which the number of auxiliary fields is reduced while maintaining the integration accuracy of $\mathcal{O}(\delta \eta^n)$ \cite{Carpenter1994Thirdorder2R,Carpenter1994Fourthorder2R,Bazavov:2021pik,Bazavov:2025dzo,Bazavov:2025exj}. This is achieved by introducing $s$ intermediate sub-stages, each with its corresponding weight coefficient.
[](){ #eq_auto_024 }
```math
\begin{equation}
IC  :  \lbrace \tilde \phi_i^{(0)},\tilde\pi_i^{(0)},a^{(0)},b^{(0)}\rbrace {\rm ~at~} \tilde\eta_0\,,\\ \nonumber
\label{eq_auto_024}
\end{equation}
```
\\
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
       \tilde\phi^{(p)}_i &=& \tilde\phi^{(p-1)}_i + B_p  \Delta\tilde\phi^{(p)}_i\, ,  \vspace*{0.15cm}\\
        \tilde\pi_{i}^{(p)} &=&\tilde\pi_{i}^{(p-1)}+  B_p \Delta\tilde\pi_{i}^{(p)}\, ,   \vspace*{0.15cm}\\
        a^{(p)} &=&a^{(p-1)} +B_p  \Delta a^{(p)}\, ,  \vspace*{0.15cm}\\
        b^{(p)} &=&b^{(p-1)} +B_p  \Delta b^{(p)}\, , \vspace*{0.15cm}
\end{array}
\right\rbrace_{p\,=\,1,\, ...,\, s} \Longrightarrow
\hspace*{-1cm}\nonumber
\label{eq_auto_025}
\end{equation}
```
[](){ #eq_RKLSalgorithm_1 }
```math
\begin{equation}
\Longrightarrow
\left\lbrace
\begin{array}{rcl}
\tilde\phi_{i,+0} &=&  \tilde \phi_i^{(s)}\, , \vspace*{0.15cm}\\
a_{+0} &=& a^{(s)}\,, \vspace*{0.15cm}\\
\tilde\pi_{i,+0} &=& \tilde\pi_i^{(s)}\, , \vspace*{0.15cm}\\
b_{+0} &=& b^{(s)}\, ,\\
\end{array}\label{eq_RKLSalgorithm_1}
\right.
\end{equation}
```
\\
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


