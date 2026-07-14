
In Cosmology, the Universe is considered to be described by a homogeneous and isotropic spacetime background, characterized by the *Friedmann–Lemaître–Robertson–Walker* (FLRW) line element
[](){ #eq_FLRWmetric }
```math
\begin{equation}
d s^2 \equiv g_{\mu\nu}d x^\mu d x^\nu = - d t^2 + a^2(t) \delta_{ij} d x^i d x^j \ , \label{eq_FLRWmetric}
\end{equation}
```
where $t$ is the cosmic time, and $a(t)$ is the scale factor. It is useful to define a new time variable $\eta$ through the relation
[](){ #eq_auto_001 }
```math
\begin{equation} 
d \eta \equiv a^{-\alpha} (t) dt \ ,\label{eq_auto_001} 
\end{equation}
```
with $\alpha$ a real number conveniently chosen for each problem at hand. We refer to $\eta$ as the **$\alpha$-time**, and note that for $\alpha=1$, it represents the *conformal time*, whereas for $\alpha=0$, we recover cosmic time. In general, we will keep $\alpha$ as an unspecified real number, writing all the relevant equations in terms of $\alpha$-time. Only when a physical problem is chosen, then one needs to make a concrete choice for $\alpha$.

The evolution of the scale factor is dictated by the stress-energy tensor of the matter fields, which in order to be compatible with the statistical homogeneity and isotropy of the universe, it must take the form of a *perfect fluid* as
[](){ #eq_stresstensor }
```math
\begin{equation} {\bar T}_{\mu \nu} \equiv (\bar\rho + \bar p )u_{\mu} u_{\nu} + \bar p g_{\mu \nu}  \ , \hspace{0.4cm} g_{\mu \nu} u^{\mu} u^{\nu} = -1 \hspace{0.4cm} \Longrightarrow \hspace{0.4cm}   \begin{cases}
    \bar\rho \equiv a^{-2 \alpha}\,{\bar T}_{00} \ , \\[0.5em]
    \bar p \equiv {1\over 3a^2} \sum_j {\bar T}_{jj} \ ,
\end{cases} \label{eq_stresstensor} \end{equation}
```
where $\bar{p}$ and $\bar{\rho}$ are the background values of the total pressure and energy densities of the field sectors, and $u_{\mu} = (a^{\alpha},0,0,0)$
is the four-velocity of a fluid at rest. The evolution of the scale factor is determined by the first and second *Friedmann equations*,
[](){ #eq_Friedmann-full }
```math
\begin{eqnarray}
    \mathcal{H}^2 \equiv \left({a'\over a}\right)^2 = a^{2 \alpha} \frac{\bar {\rho}}{3 m_p^2} 
    \,,~~~~~~~
    {a''\over a} = \frac{a^{2 \alpha}}{6 m_p^2}\Big[ (2 \alpha - 1) \bar{\rho} - 3 \bar{ p} \Big]\,, \label{eq_Friedmann-full}
\end{eqnarray}
```
where ${f}' \equiv \text{d} f/ \text{d} \eta$ and $m_p \simeq 2.435\cdot10^{18}$ GeV is the reduced Planck mass. 

## Canonical Field Theory { #subsec_eomCont }

<!-- 
In Section [Lattice-Cosmology Reviews](MonographicReviews.md) we present the lattice formulation of different theories involving scalar and/or gauge fields, based on the techniques introduced in [Field Discretization Primer](Brief_Review_On_Lattice_Techniques.md). In order to introduce notation, 
 -->
We introduce now the equations of motion resulting in a scalar-gauge theory with three kinds of canonically-normalized scalar fields: a real scalar singlet $\phi$, a $U(1)$-charged complex scalar field $\varphi$, and a $[SU(N) \times U(1)]$-charged doublet scalar field $\Phi$. In the last two cases, there are also present, respectively, Abelian and non-Abelian gauge fields, $A_{\mu}$ and $C_{\mu} \equiv C_{\mu}^a T_a$, with $T_a$ the $N^2-1$ group generators, satisfying the $SU(N)$ Lie algebra properties $[T_a, T_b] = i f_{abc} T_c$ , ${\rm Tr}(T_a) = 0$ , ${\rm Tr}(T_a T_b) = \frac{1}{2} \delta_{ab}$, and $T^{\dagger}_a = T_a$, with $f_{abc}$ the totally anti-symmetric *structure constants* of the group, and $[\cdot,\cdot]$ the usual matrix commutator. In the particular case of $SU(2)$, $T_a \equiv \sigma_a / 2$, $a=1,2,3$, with $\sigma_a$ the *Pauli matrices*, and $f_{abc} = \epsilon_{abc}$ the total anti-symmetric tensor. The scalar fields can be explicitly written in terms of real components as follows,
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
            \varphi_1 +i\varphi_2 \\ \varphi_3 +i\varphi_3 \\ \vdots \\ \varphi_{2N -1} +i\varphi_{2N}
        \end{array}
        \right) \,.
    \end{array}
\end{eqnarray}
```

More specifically, for canonical scalar-gauge theories we consider the action $S = \int d^4x  \sqrt{-g}\, \mathcal{L}$, with $g \equiv {\rm det} (g_{\mu \nu})$, and the Lagrangian
[](){ #eq_lagrangian }
```math
\begin{align} 
-\mathcal{L} = \frac{1}{2}\partial_{\mu} \phi \partial ^{\mu}\phi + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} + V \ ,
\label{eq_lagrangian} 
\end{align}
```
with $V \equiv V(\phi,|\varphi|, |\Phi|)$ the potential describing the interactions between the scalar fields. The *covariant derivatives* and *field strength tensors* associated to the gauge fields, are defined as
[](){ #eq_AbCovDerivCont }
[](){ #eq_CovDerivCont }
```math
\begin{eqnarray}
D_{\mu}^{\rm A}  &\equiv &  \partial _{\mu} - i  g_A Q_AA_\mu \ , \hspace{4.5cm} F_{\mu \nu}\equiv  \partial_{\mu}  A_{\nu} - \partial_{\nu} A_{\mu} \ , \label{eq_AbCovDerivCont} \\
D_{\mu} & \equiv  &
\mathcal{I}D^{\rm A}_\mu
- i g_C Q_C C_{\mu}^a \,T_a
\ , \hspace{2.76cm}  G_{\mu \nu} \equiv \partial_{\mu} C_{\nu} - \partial_{\nu} C_{\mu} - i[C_\mu,C_\nu]\,,  \label{eq_CovDerivCont}  
\end{eqnarray}
```
with $g_{A}$ and $g_C$ the Abelian and non-Abelian gauge couplings, $Q_{A}$ and $Q_C$ the Abelian and non-Abelian charges of the scalar fields, $\mathcal{I}$ the $N\times N$ identity matrix. The gauge-invariant electric and magnetic fields associated to the Abelian and non-Abelian fields can be written as
[](){ #eq_ElectricMagneticDefs }
```math
\begin{equation}\label{eq_ElectricMagneticDefs}
E_i \equiv F_{0i} , \,\,\,\,\,\,\,\,  B_i \equiv \frac{1}{2} \epsilon_{i j k} F^{j k} , \,\,\,\,\,\,\,\,   E_i^a \equiv G_{0i}^a , \,\,\,\,\,\,\,\,  B_i^a \equiv \frac{1}{2} \epsilon_{i j k} G^{j k}_a \ , \end{equation}
```
where $\epsilon_{ijk}$ is the Levi-Civita symbol in three dimensions with normalization $\epsilon_{123}=+1$, and $G_{\mu \nu}^a \equiv {\rm Tr}(2G_{\mu \nu} T_a) = {\rm Tr}(G_{\mu \nu} \sigma_a)$.

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
    \phi'' - a^{-2(1 - \alpha)} {\vec\nabla}^{\,2} \hspace{-1mm}\phi + (3 - \alpha)\mathcal{H} {\phi'} &=& - a^{2 \alpha} V_{,\phi} \ , \label{eq_singlet-eom} \\
    \varphi'' - a^{-2(1 - \alpha)} {\vec D}_{\hspace{-0.5mm}A}^{\,2}\varphi + (3 - \alpha) \mathcal{H}  {\varphi'} &=& - \frac{a^{2 \alpha}V_{,|\varphi|} }{2} \frac{\varphi}{|\varphi |} \ , \label{eq_higgsU1-eom}\\
    \Phi'' - a^{-2(1 - \alpha)} {\vec D}^{\,2}\Phi + (3 - \alpha) \mathcal{H}  {\Phi'} &=& - \frac{a^{2 \alpha} V_{,|\Phi|}}{2} \frac{\Phi}{|\Phi |} \ , \label{eq_higgsSU2-eom}
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
where we have introduced the derivative operator $(\mathcal{D}_{\nu}O)_a = (\mathcal{D}_{\nu})_{a b}O_b \equiv ( \delta_{a b}  \partial_{\nu} - f_{abc} C_{\nu}^c ) O_b$, and defined the matter currents
[](){ #eq_AbelianCurrent }
[](){ #eq_NonAbelianCurrent }
```math
\begin{eqnarray}
    \label{eq_AbelianCurrent}
    J_A^\mu & \equiv & 2g_A Q_A^{(\varphi)} \mathcal{I}m [ \varphi^{*} ( D_A^{\mu} \varphi )] + 2g_A Q_A^{(\Phi)} \mathcal{I}m [ \Phi^\dagger (D^{\mu} \Phi  )]\,,\\
    \label{eq_NonAbelianCurrent}
    J_a^\mu & \equiv & 2g_C Q_C\mathcal{I}m [ \Phi^{\dagger} T_a( D^{\mu} \Phi )]\,.
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
This definition leads, using Eqs.~(\ref{eq_stresstensor}) and~(\ref{eq_lagrangian}), to *local* expressions for the field's energy and pressure densities, 
[](){ #eq_rhoLocal }
[](){ #eq_pLocal }
```math
\begin{eqnarray}
    \rho &=& {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} + {G}_{\phi} + {G}_{\varphi} + {G}_{\Phi} + {K}_{U(1)} + {G}_{U(1)} + {K}_{SU(2)} + {G}_{SU(2)} + {V},  \label{eq_rhoLocal}\\
    p &=& {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} -{1\over3}({G}_{\phi} + {G}_{\varphi} + {G}_{\Phi}) + {1\over3}({K}_{U(1)} + {G}_{U(1)}) + {1\over3}({K}_{SU(2)} + {G}_{SU(2)}) - {V}, \label{eq_pLocal}
\end{eqnarray}
```
with the different energy density contributions given by
Here is the same text with all \tilde, \widetilde, and \widetilde{...} removed, while leaving everything else unchanged.

[](){ #eq_energy-contributions }
```math
\begin{align}
\label{eq_energy-contributions}
\hspace{-1cm}\left\lbrace
\begin{array}{rcl}
{K}_{\phi} &=& \frac{1}{2 a^{2\alpha} } \phi'^2 \\
{K}_{\varphi} &=& \frac{1}{a^{2\alpha} } (D_0^A \varphi)^*(D_0^A \varphi)
\\
{K}_{\Phi} &=& \frac{1}{a^{2\alpha} } (D_0 \Phi )^\dagger(D_0 \Phi)
\\
\end{array}\right.
\hspace{0.1cm};\hspace{0.75cm}
\left\lbrace
\begin{array}{rcl}
{G}_{\phi} &=& \frac{1}{2 a^2} \sum_i (\partial_i \phi)^2
\\
{G}_{\varphi} &=& \frac{1}{a^2} \sum_i (D_i^A \varphi)^*(D_i^A \varphi)
\\
{G}_{\Phi} &=& \frac{1}{a^2} \sum_i (D_i\Phi)^\dagger(D_i \Phi)
\\
\end{array}\right.
\hspace{0.1cm};\hspace{0.75cm}
\left\lbrace
\begin{array}{rcl}
{K}_{U(1)} &=& \frac{1}{2 a^{2 + 2 \alpha}} \sum_{i} F_{0i}^2
\\
{K}_{SU(2)} &=& \frac{1}{2 a^{2 + 2 \alpha}}  \sum_{a,i} (G_{0i}^a)^2
\\
{G}_{U(1)} &=& \frac{1}{2 a^4}  \sum_{i,j < i} F_{ij}^2
\\
{G}_{SU(2)} &=& \frac{1}{2 a^4}  \sum_{a,i,j < i}  (G_{ij}^a)^2    . \\
\end{array}\right.
\\\nonumber\\
\text{(Kinetic-Scalar)} \hspace{5cm} \text{(Gradient-Scalar)} \hspace{6.5cm} \text{(Electric & Magnetic)} \hspace{3.0cm}\nonumber
\end{align}
```
If the fields dominate the energy budget of the Universe, the expansion rate can be determined through the Friedmann Eqs. \eqref{eq_Friedmann-full}, which in our case can be written as
[](){ #eq_FriedmannHub }
[](){ #eq_FriedmannD2a }
```math
\begin{align}
\label{eq_FriedmannHub}
\mathcal{H}^2    \equiv    \frac{a'^{ 2}}{a^2} &=  \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ E_K^{\phi} + E_K^{\varphi} + E_K^{\Phi} + E_G^{\phi} + E_G^{\varphi} + E_G^{\Phi} + E_K^A + E_K^B + E_G^A + E_G^B + E_V \right]  ,
\\
\label{eq_FriedmannD2a}
{a''\over a} &= \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ (\alpha-2)(E_K^{\phi} + E_K^{\varphi} + E_K^{\Phi}) + \alpha(E_G^{\phi} + E_G^{\varphi} + E_G^{\Phi}) + (\alpha + 1)E_V \right.\\
& \hspace{2cm} \left. +  (\alpha-1)(E_K^A + E_K^B + E_G^A + E_G^B) \right]  ,\nonumber
\end{align}
```
with $\langle \dots \rangle$ denoting an average over sufficiently large volumes that encompass all relevant wavelengths of the fields. In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we use Eq. (\ref{eq_FriedmannD2a}) to solve for the scale factor, while monitoring that the constraint equation (\ref{eq_FriedmannHub}) is verified throughout the evolution to some desired accuracy, see Section [*Evolution Algorithms*][subsec_Algorithms]. 

**Dynamics of gravitational waves**

Gravitational waves are transverse and traceless tensor perturbations, $h_{ij}$, of the background metric. Considering the FLRW solution as the background metric, the perturbed line element (in $\alpha$-time) is
[](){ #eq_GWmetric }
```math
\begin{align}
\label{eq_GWmetric}
ds^2
=
-a^{2\alpha}(\eta)d\eta^2
+a^2(\eta)\left(\delta_{ij}+h_{ij}\right)dx^idx^j\,,
\end{align}
```
which are transverse, $\partial_i h_{ij}=0$, and traceless, $h_{ii}=0$. In the linearized gravity regime, the equation of motion of GWs reads
[](){ #eq_GWEOMcontinuum }
```math
\begin{align}
\label{eq_GWEOMcontinuum}
h_{ij}''
+(3-\alpha){a'\over a}h_{ij}'
-a^{-2(1-\alpha)}\nabla^2h_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}\Pi_{ij}^{\rm TT} \,.
\end{align}
```
GWs are sourced by the transverse-traceless (TT) part of the anisotropic stress tensor, $\Pi_{ij}^{\rm TT}$. For a generic background fluid, this anisotropic stress tensor takes the form,
[](){ #eq_GWAnisotropicStress }
```math
\begin{align}
\label{eq_GWAnisotropicStress}
\Pi_{ij}
\equiv
T_{ij}-\bar p\,g_{ij},
\end{align}
```
In the practive, however, it is more convenient to define an effective anisotropic strees tensor containing only those contributions to $\Pi_{ij}$ that have a non-zero TT projection. For example, for a generic model consisting of canonically normalized scalars and Abelian fields, this takes the form,
[](){ #eq_GWEffectiveAnisotropicStress }
```math
\begin{align}
\label{eq_GWAnisotropicStress}
\Pi_{ij}^\mathrm{eff}=\sum_a \nabla_i\phi_a \nabla_j\phi_a+2\sum_b \text{Re}\left[\left(D_i^A\varphi_b)^*D_j^A\varphi_b\right]-a^{-2\alpha}E_iE_j-a^{-2}B_iB_j\,,
\equiv
\end{align}
```
from which the source of GWs is obtained after TT projection. This operation is non-local in real space, but corresponds to an algebraic relation in Fourier space,
[](){ #eq_GWTTsourceProjection }
```math
\begin{align}
\label{eq_GWTTsourceProjection}
\Pi_{ij}^{\rm TT}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})\Pi_{lm}^{\rm eff}({\bf k},\eta)\,,
\end{align}
```
where the Fourier-space TT projector takes the form,
[](){ #eq_GWTTprojector }
```math
\begin{align}
\label{eq_GWTTprojector}
\Lambda_{ij,lm}(\hat{\bf k})
&\equiv
P_{il}(\hat{\bf k})P_{jm}(\hat{\bf k})
-{1\over2}P_{ij}(\hat{\bf k})P_{lm}(\hat{\bf k})\,,\quad\quad \text{with}\quad\quad P_{ij}(\hat{\bf k})
&\equiv
\delta_{ij}-\hat k_i\hat k_j\,,
\end{align}
```
and $\hat k_i\equiv {k_i\over k}$. This guarantees the projected tensor is both transverse, $k_i\Pi_{ij}^{\rm TT}=0$, and traceless, $\Pi_{ii}^{\rm TT}=0$.

Numerically solving Eq.$~$\eqref{eq_GWEOMcontinuum} directly requires to determine the TT projected anisotropic stress tensor every time the GWs need to be evolved, which is a very costly operation. An alternative was proposed in Ref. [@GarciaBellido_2008], based on the use of auxiliary fields sourced by $\Pi_{ij}^\text{eff}$. While the original proposad used six unphysical degrees of freedom to simulate the GWs, it is possible to reduce this number to five, as discussed in $\mathtt{The~Art{\text -}II}$ [@BaezaBallesteros_2025tme],  by using a symmetric traceless tensor of auxiliary fields, $v_{ij}$, which obeys $v_{33}=-(v_{11}+v_{22})$. If these fields are evolved following
[](){ #eq_GWvEOM }
```math
\begin{align}
\label{eq_GWvEOM}
v_{ij}''
+(3-\alpha){a'\over a}v_{ij}'
-a^{-2(1-\alpha)}\nabla^2v_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}
\left[
\Pi_{ij}^{\rm eff}
-{1\over3}\delta_{ij}\Pi_{kk}^{\rm eff}\,.
\right],
\end{align}
```
then the physical degrees of freedom can be recovered at any time by TT projection,
[](){ #eq_GWvToh }
```math
\begin{align}
\label{eq_GWvToh}
h_{ij}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})v_{lm}({\bf k},\eta)\,.
\end{align}
```
This is the algorithm used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate the evolution of GWs.

Finally, it is worth mentioning about GW observables. The most relevant quantity related to GWs is the energy density of the GW background,
[](){ #eq_GWrhoContinuum }
```math
\begin{align}
\label{eq_GWrhoContinuum}
\rho_{\rm GW}(\eta)
&=
{m_p^2\over 4a^{2\alpha}V}
\int_V d^3{\bf x}\,
h'_{ij}({\bf x},\eta)h'_{ij}({\bf x},\eta)
\\
&\simeq
{m_p^2\over 4a^{2\alpha}V}
\int_V {d^3{\bf k}\over(2\pi)^3}
h'_{ij}({\bf k},\eta)h_{ij}^{\prime *}({\bf k},\eta)
\\
&\equiv
\int {d\rho_{\rm GW}\over d\log k}d\log k \,,
\end{align}
```
from which the spectral density can be defined as
[](){ #eq_GWrhoSpectrumContinuum }
```math
\begin{align}
\label{eq_GWrhoSpectrumContinuum}
\left({d\rho_{\rm GW}\over d\log k}\right)(k,\eta)
=
{m_p^2 k^3\over 8\pi^2a^{2\alpha}V}
\int {d\Omega_k\over4\pi}\,
h'_{ij}(\hat{\bf k},k,\eta)h_{ij}^{\prime *}(\hat{\bf k},k,\eta)\,.
\end{align}
```
For stochastic sources the volume average can be replaced by an ensemble average $\langle...\rangle$ over the independent realizations of the tensor fluctuations,
[](){ #eq_stochasticPS }
```math
\begin{align}
\rho_{\rm GW}(\eta)
&= \dfrac{m_p^2}{4a^{2\alpha}}
\left\langle h'_{ij}({\bf x},\eta) h_{ij}^{\prime *}({\bf x},\eta)\right\rangle
\nonumber \\
&= \dfrac{m_p^2}{4a^{2\alpha}}
\int \dfrac{\text{d}^3{\bf k}}{(2\pi)^3}
\dfrac{\text{d}^3{\bf k'}}{(2\pi)^3}
e^{-i {\bf x}\cdot({\bf k} - {\bf k'})}
\left\langle h'_{ij}({\bf k},\eta) h_{ij}^{\prime *}({\bf k'},\eta)\right\rangle
\nonumber \\
&\equiv
\dfrac{m_p^2}{8\pi^2a^{2\alpha}}
\int\dfrac{\text{d}k}{k} k^3 P_{h'}(k,\eta)\,,
\label{eq_stochasticPS}
\end{align}
```
where we have introduced the power spectrum of the time derivative of $h_{ij}$,
[](){ #eq_stochasticPS_2 }
```math
\begin{align}
\left\langle h'_{ij}({\bf k},\eta)h_{ij}^{\prime *}({\bf k'},\eta) \right\rangle
=
(2\pi)^3 P_{h'}(k,\eta)\delta^{(3)}({\bf k} - {\bf k'})\,.
\label{eq_stochasticPS_2}
\end{align}
```
In addition to the energy density, one usually also defines the fractional GW energy density power spectrum as
[](){ #eq_GWOmegaContinuum }
```math
\begin{align}
\label{eq_GWOmegaContinuum}
\Omega_{\rm GW}(k,\eta)
\equiv
{1\over\rho_\text{c}}{d\rho_{\rm GW}\over d\log k}
=
{k^3\over 24\pi^2\mathcal H^2}P_{h'}(k,\eta)
=
{k^3\over 24\pi^2a^{2\alpha}H^2}P_{h'}(k,\eta)\,.
\end{align}
```
where $\rho_\text{c}=3m_p^2 H^2$ is the critical energy density.

## Non-Canonical Field Theories { #subsec_eomNonCanonical }

To be added soon ...

**Scalar Non-Minimally Coupled to Gravity**

**Non-Minimal Kinetic Scalar Theories**

**Axion-Gauge interactions**

## Scalar-Gauge-Fluid Dynamics { #subsec_eomFluids }

To be added soon ...

**Fluid Dynamics**

**Scalar-Fluid Dynamics**

**Gauge-Fluid Dynamics**

<!-- **Scalar-Gauge-Fluid Dynamics** -->

