
In Cosmology, the Universe is considered to be described by a homogeneous and isotropic spacetime background, characterized by the *Friedmann–Lemaître–Robertson–Walker* (FLRW) line element
[](){ #eq_FLRWlineElem }
```math
\begin{equation}
d s^2 \equiv g_{\mu\nu}d x^\mu d x^\nu = - d t^2 + a^2(t) \delta_{ij} d x^i d x^j \ , 
\label{eq_FLRWlineElem}
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

The evolution of the scale factor is dictated by the stress-energy tensor of the matter fields, which in order to be compatible with the statistical homogeneity and isotropy of the universe, must take the form of a *perfect fluid* as
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
We introduce now the equations of motion resulting in a scalar-gauge theory with three kinds of canonically-normalized scalar fields: a real scalar singlet $\phi$, a U(1)-charged complex scalar field $\varphi$, and a $[SU(N) \times U(1)]$-charged doublet scalar field $\Phi$. In the last two cases, there are also present, respectively, Abelian and non-Abelian gauge fields, $A_{\mu}$ and $C_{\mu} \equiv C_{\mu}^a T_a$, with $T_a$ the $N^2-1$ group generators, satisfying the $SU(N)$ Lie algebra properties $[T_a, T_b] = i f_{abc} T_c$ , ${\rm Tr}(T_a) = 0$ , ${\rm Tr}(T_a T_b) = \frac{1}{2} \delta_{ab}$, and $T^{\dagger}_a = T_a$, with $f_{abc}$ the totally anti-symmetric *structure constants* of the group, and $[\cdot,\cdot]$ the usual matrix commutator. In the particular case of SU(2), $T_a \equiv \sigma_a / 2$, $a=1,2,3$, with $\sigma_a$ the *Pauli matrices*, and $f_{abc} = \epsilon_{abc}$ the totally anti-symmetric tensor. The scalar fields can be explicitly written in terms of real components as follows,
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
where $\epsilon_{ijk}$ is the Levi-Civita symbol in three dimensions with normalization $\epsilon_{123}=+1$, and $G_{\mu \nu}^a \equiv {\rm Tr}(2G_{\mu \nu} T_a) = {\rm Tr}(G_{\mu \nu} \sigma_a)$. **We note that the electric field definitions depend on the $\alpha$-time $\eta$, as $F_{0i}$ and $G_{0i}$ are defined with respect to $\eta$, not $t$.**

The equations of motion for the matter fields and the scale factor have been derived in more detail in $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]). Here we simply quote their resulting form, which read
[](){ #eq_singlet-eomCONT }
[](){ #eq_higgsU1-eom }
[](){ #eq_higgsSU2-eom }
[](){ #eq_U1eom }
[](){ #eq_SU2eom }
[](){ #eq_GaussU1-eom }
[](){ #eq_GaussSU2-eom }
```math
\begin{eqnarray}
    \phi'' - a^{-2(1 - \alpha)} {\vec\nabla}^{\,2} \hspace{-1mm}\phi + (3 - \alpha)\mathcal{H} {\phi'} &=& - a^{2 \alpha} V_{,\phi} \ , \label{eq_singlet-eomCONT} \\
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
Note that Eqs. \eqref{eq_GaussU1-eom} and \eqref{eq_GaussSU2-eom} are the Gauss constraint of the Abelian and non-Abelian sectors, respectively, which must be preserved at all times during the evolution.

The energy-momentum tensor of a system characterized by a lagrangian $\mathcal{L}$, is given by 
[](){ #eq_auto_002 }
```math
\begin{equation}
T_{\mu \nu} \equiv -\frac{2}{\sqrt{g}}\frac{\delta(\sqrt{g} \mathcal{L})}{\delta g^{\mu \nu}}\,.
\label{eq_auto_002}
\end{equation}
```
This definition leads, using Eqs. (\ref{eq_stresstensor}) and (\ref{eq_lagrangian}), to *local* expressions for the field's energy and pressure densities,
[](){ #eq_rhoLocal }
[](){ #eq_pLocal }
```math
\begin{eqnarray}
    \rho &=& {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} + {G}_{\phi} + {G}_{\varphi} + {G}_{\Phi} + {K}_{U(1)} + {G}_{U(1)} + {K}_{SU(2)} + {G}_{SU(2)} + {V},  \label{eq_rhoLocal}\\
    p &=& {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} -{1\over3}({G}_{\phi} + {G}_{\varphi} + {G}_{\Phi}) + {1\over3}({K}_{U(1)} + {G}_{U(1)}) + {1\over3}({K}_{SU(2)} + {G}_{SU(2)}) - {V}, \label{eq_pLocal}
\end{eqnarray}
```
with the different energy density contributions given by
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
\text{(Kinetic-Scalar)} \hspace{5cm} \text{(Gradient-Scalar)} \hspace{6.5cm} \text{(Electric & Magnetic)} \hspace{3.0cm}\nonumber\\
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
where we have defined the volume-averaged energy contributions as $E_{K}^{f} = \langle K_{f} \rangle$ and $E_{G}^{f} = \langle G_{f} \rangle$ for the scalar fields $f=\phi,\varphi,\Phi$, $E_{K}^{A} = \langle K_{U(1)} \rangle$, $E_{G}^{A} = \langle G_{U(1)} \rangle$, $E_{K}^{B} = \langle K_{SU(2)} \rangle$, and $E_{G}^{B} = \langle G_{SU(2)} \rangle$ for the gauge fields, and ${E}_V = \langle {V} \rangle$ for the potential energy, with $\langle \dots \rangle$ denoting an average over sufficiently large volumes that encompass all relevant wavelengths of the fields. In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we use Eq. (\ref{eq_FriedmannD2a}) to solve for the scale factor, while monitoring that the constraint equation (\ref{eq_FriedmannHub}) is verified throughout the evolution to some desired accuracy, see Section [*Evolution Algorithms*][subsec_Algorithms]. 

## Non-Canonical Field Theories { #subsec_eomNonCanonical }

There are many possibilities to consider when dealing when field theories with non-canonical interactions. Below we consider few representative examples, such as scalar fields non-minimally coupled (NMC) to gravity through a term of the form $\propto \phi^{2}R$, with $R$ the Ricci scalar; scalar fields with non-minimal kinetic (NMK) terms of the form $f(\phi)X$, with $X \equiv \partial^{\mu}\phi\partial_{\mu}\phi$; and axion-like-particles (ALPs) interacting with gauge fields through a coupling of the form $\phi F\tilde{F}$.

<div style="height: 20px;"></div>

[](){ #NMC_th }
**Scalar Non-Minimally Coupled to Gravity**

<!-- The starting point here is the Jordan-frame dynamics of a scalar field $\phi$ non-minimally coupled to gravity.  -->
We begin with an action containing a curvature interaction proportional to $\xi R\phi^2$ and a generic potential $V(\phi,\{\varphi_{\rm m}\})$ for $\phi$ and the remaining matter fields $\{\varphi_{\rm m}\}$, represented by $\mathcal{L}_{\rm m}$. In the Jordan-frame, this reads

[](){ #eq_action }

```math
S =
\int d^{4}x\,\sqrt{-g}
\left[
\frac{1}{2}m_p^2R
-\frac{1}{2}\xi R\phi^{2}
-\frac{1}{2}g^{\mu\nu}\partial_{\mu}\phi\partial_{\nu}\phi
-V(\phi,\{\varphi_{\rm m}\})
+\mathcal{L}_{\rm m}
\right]\,,
\label{eq_action}
```
where $R$ is the Ricci scalar and $\xi$ the non-minimal coupling parameter. Neglecting gravitational perturbations and restricting the metric to a spatially flat FLRW background, the equation of motion for $\phi$ becomes

[](){ #eq_eom }
```math
\begin{eqnarray}
\phi''
+(3-\alpha)\frac{a'}{a}\phi'
-a^{-2(1-\alpha)}\nabla^2\phi
+a^{2\alpha}
\left(
\xi\bar R\phi+\frac{\partial V}{\partial\phi}
\right)
=0\,,
\label{eq_eom}\\
{\rm where} ~~~~~~\bar R =
\frac{6}{a^{2\alpha}}
\left[
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
\right]\,.~~~~~~~~~~~~~~
\label{eq_cosmic_R}
\end{eqnarray}
```
<!-- 
where the background Ricci scalar is given by

[](){ #eq_cosmic_R }
```math
\bar R =
\frac{6}{a^{2\alpha}}
\left[
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
\right]\,.
\label{eq_cosmic_R}
```
 -->

Assuming homogeneity and isotropy on large scales, the energy-momentum tensor takes the perfect-fluid form $T^\mu{}_\nu=\operatorname{diag}\{-\bar\rho(\eta),\bar p(\eta),\bar p(\eta),\bar p(\eta)\}$. The background pressure and energy density are decomposed into contributions from the non-minimally coupled scalar and the remaining matter sectors, $\bar p=\bar p_\phi+\bar p_{\rm m}$ and $\bar\rho=\bar\rho_\phi+\bar\rho_{\rm m}$. The Einstein equations then reduce to the Friedmann equations in $\alpha$-time, 

[](){ #eq_Hu }
[](){ #eq_2FE }
```math
\begin{eqnarray}
\mathcal{H}^{2}
\equiv
\left(\frac{a'}{a}\right)^2
=
\frac{a^{2\alpha}}{3m_p^2}
\left(\bar\rho_\phi+\bar\rho_{\rm m}\right)
\label{eq_Hu}\,~~;~~~~~~~~~~
\frac{a''}{a}
=
-\frac{a^{2\alpha}}{6m_p^2}
\left[
(1-2\alpha)
\left(\bar\rho_\phi+\bar\rho_{\rm m}\right)
+3\left(\bar p_\phi+\bar p_{\rm m}\right)
\right]\,,
\end{eqnarray}
```

where the energy density and pressure of the non-minimally coupled scalar field are [@Figueroa:2021iwm]

[](){ #eq_nmcrho }
```math
\bar\rho_\phi(\eta)
=
\frac{1}{2a^{2\alpha}}\left\langle\phi'^2\right\rangle
+\frac{1}{2a^2}\left\langle(\nabla\phi)^2\right\rangle
+\left\langle V(\phi)\right\rangle
+\frac{3\xi}{a^{2\alpha}}\mathcal{H}^2
 \left\langle\phi^2\right\rangle
+\frac{6\xi}{a^{2\alpha}}\mathcal{H}
 \left\langle\phi\phi'\right\rangle
-\frac{\xi}{a^2}\left\langle\nabla^2\phi^2\right\rangle\,.
\label{eq_nmcrho}\\
```
[](){ #eq_nmcp }
```math
\begin{aligned}
\bar p_\phi(\eta)
={}&
\frac{1-4\xi}{2a^{2\alpha}}\left\langle\phi'^2\right\rangle
-\frac{1-12\xi}{6a^2}\left\langle(\nabla\phi)^2\right\rangle
-\left\langle V(\phi)\right\rangle
+\frac{2\xi}{a^{2\alpha}}\mathcal{H}
 \left\langle\phi\phi'\right\rangle
-\frac{\xi}{3a^2}\left\langle\nabla^2\phi^2\right\rangle
\\
&+
2\xi\left\langle\phi V_{,\phi}\right\rangle
+\frac{\xi}{a^{2\alpha}}
\left[
\mathcal{H}^2
+12\left(\xi-\frac{1}{6}\right)
\left(
\frac{a''}{a}
+(1-\alpha)\mathcal{H}^2
\right)
\right]
\left\langle\phi^2\right\rangle\,,
\end{aligned}
\label{eq_nmcp}
```
with $V_{,\phi}\equiv\partial V/\partial\phi$, and $\langle \dots \rangle$ denoting volume-averaging over sufficiently large scales that encompass all relevant wavelengths of the fields. The scale factor may, in principle, be evolved using the Friedmann equations in $~$\eqref{eq_Hu}. We note, however, that contrary to canonical scenarios, the *r.h.s.* of the Friedmann equations depend on time derivatives of $a(\eta)$, given the expression of $\bar\rho_\phi(\eta), \bar p_\phi(\eta)$.

As proposed in Ref. [@Figueroa_2021iwm], the scale factor evolution can be obtained alternatively from the trace of the energy-momentum tensor of the non-minimally coupled field, 

[](){ #eq_4dtrT }
```math
T_\phi
=
(6\xi-1)
\left(
\partial^\mu\phi\,\partial_\mu\phi
+\xi R\phi^2
\right)
+6\xi\phi\frac{\partial V}{\partial\phi}
-4V\,.
\label{eq_4dtrT}
```

Given the traced Einstein equations $R = -\frac{1}{m_p^2}g^{\mu\nu}\left(T^\phi_{\mu\nu}+T^{\rm m}_{\mu\nu}\right)$ = $-\frac{1}{m_p^2}\left(T_\phi+T_{\rm m}\right)$, it follows that the background curvature satisfies

[](){ #eq_EFEtrBack }
```math
m_p^2\bar R =
(1-6\xi)
\left[
\left\langle
\partial^\mu\phi\,\partial_\mu\phi
\right\rangle
+\xi\bar R\left\langle\phi^2\right\rangle
\right]
-6\xi\left\langle\phi V_{,\phi}\right\rangle
+4\left\langle V\right\rangle
-\left\langle T_{\rm m}\right\rangle\,,
\label{eq_EFEtrBack}
```
where $\langle\cdots\rangle$ denotes again volume-averaging of the corresponding local spatial inhomogeneities, over length scales much larger than the inverse gradient-scales of the problem. Solving the above expression for $\bar R$, then gives

[](){ #eq_eomR }
[](){ #eq_Fphi }
```math
\begin{eqnarray}
\bar R
=
\frac{F(\phi)}{m_p^2}
\left[
(1-6\xi)
\left\langle
\partial^\mu\phi\,\partial_\mu\phi
\right\rangle
+4\left\langle V\right\rangle
-6\xi\left\langle\phi V_{,\phi}\right\rangle
-\left\langle T_{\rm m}\right\rangle
\right]\,.
\label{eq_eomR}\\
~~~~{\rm where}~~~
F(\phi)
\equiv
\frac{1}{
1+(6\xi-1)\xi\left\langle\phi^2\right\rangle/m_p^2
}\,.~~~~~~~~~~~~~~~
\label{eq_Fphi}
\end{eqnarray}
```


Using Eq.$~$\eqref{eq_cosmic_R}, we then obtain the following differential equation for the scale factor,

[](){ #eq_piadot }
```math
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
=
\frac{a^{2\alpha}F(\phi)}{6m_p^2}
\left[
(1-6\xi)
\left\langle
\partial^\mu\phi\,\partial_\mu\phi
\right\rangle
+4\left\langle V\right\rangle
-6\xi\left\langle\phi V_{,\phi}\right\rangle
-\left\langle T_{\rm m}\right\rangle
\right]\,.
\label{eq_piadot}
```

This equation can be evolved simultaneously with the equations of motion of the non-minimally coupled scalar and the remaining matter fields. In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we actually follow this method.

<div style="height: 20px;"></div>

[](){ #NMK_th }
**Non-Minimal Kinetic Scalar Theories**

Another example of non-canonical interactions is given by models with an internal field-space metric (that may, in principle, depend on both the field amplitudes and their conjugate momenta), $\mathcal{G}_{ab}\equiv\mathcal{G}_{ab}[\{\phi_a\},\{\pi_{\phi_a}\}]$. These theories are characterized by an action of the form

[](){ #eq_ScalarActionNonCanonicalCont }
```math
S_{\rm NMK}
=
-\int d^4x\,\sqrt{-g}
\left[
\frac{1}{2}g^{\mu\nu}\mathcal{G}_{ab}
\partial_\mu\phi_a\partial_\nu\phi_b
+V(\{\phi\})
\right]\,,
\label{eq_ScalarActionNonCanonicalCont}
```
where the field-space metric is symmetric, $\mathcal{G}_{ab}=\mathcal{G}_{ba}$. The case of canonically normalized scalar fields is trivially recovered by $\mathcal{G}_{ab}=\delta_{ab}$. Likewise, if $\mathcal{G}_{ab}=\beta\delta_{ab}$, with $\beta>0$ constant, the kinetic term can be brought to canonical form through the field redefinition $\phi_a\rightarrow\sqrt{\beta}\phi_a$. *Non-canonically normalized* fields correspond therefore only to $\mathcal{G}_{ab}$ depending explicitly on the field amplitudes and/or their derivatives. In such a case, one further assumes $\det(\mathcal{G}_{ab})\neq 0$, so that an inverse metric $\mathcal{G}^{-1}_{ab}$ exists and satisfies $\mathcal{G}_{ac}\mathcal{G}^{-1}_{cb}=\delta_{ab}$. For simplicity, we restrict below to field-space metrics that depend only on the field amplitudes, $\mathcal{G}_{ab}=\mathcal{G}_{ab}(\{\phi_a\})$. Varying the above action and considering the metric given by the FLRW background, we obtain field equations of motion as
<!-- 
[](){ #eq_EOMflatScalarFlds }
```math
\frac{1}{\sqrt{-g}}
\partial_\mu
\left(
\sqrt{-g}\,g^{\mu\nu}\mathcal{G}_{ab}\partial_\nu\phi_b
\right)
-\frac{1}{2}g^{\mu\nu}\mathcal{G}_{bc,a}
\partial_\mu\phi_b\partial_\nu\phi_c
-\frac{\partial V}{\partial\phi_a}
=0\,,
\label{eq_EOMflatScalarFlds}
```
 -->
[](){ #eqn_FLRWeqnforNMK }
[](){ #eq_gammaNMK }
```math
\begin{eqnarray}
\mathcal{G}_{ab}\phi_b''
+
(3-\alpha)\frac{a'}{a}\mathcal{G}_{ab}\phi_b'
-a^{-2(1-\alpha)}
\mathcal{G}_{ab}\nabla^2\phi_b
+ \gamma_{abc}
\left(
\phi_b'\phi_c'
-a^{-2(1-\alpha)}
\vec{\nabla}\phi_b\cdot\vec{\nabla}\phi_c
\right)
+a^{2\alpha}\frac{\partial V}{\partial\phi_a}
=0\,,
\label{eqn_FLRWeqnforNMK}\\
{\rm where}~~~~~~~~ \gamma_{abc} \equiv \mathcal{G}_{ab,c}
-\frac{1}{2}\mathcal{G}_{bc,a}\,,~~~~{\rm and}~~~~ \mathcal{G}_{bc,a}\equiv\partial\mathcal{G}_{bc}/\partial\phi_a\,. ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
\label{eq_gammaNMK}
\end{eqnarray}
```

Introducing the conjugate momenta $\pi_a\equiv\pi_{\phi_a}$, Eq.$~$\eqref{eqn_FLRWeqnforNMK} can be recast as the first-order system

[](){ #eqn_NMKcontinuum }
[](){ #eq_FaNMK }
```math
\begin{eqnarray}
\left\lbrace
\begin{array}{l}
\phi_a'
\equiv
\mathcal{G}^{-1}_{ab}\pi_b\,,
\\[3mm]
\pi_a'
+(3-\alpha)\frac{a'}{a}\pi_a
= (\mathcal{F}_a)_{bc}\pi_b\pi_c + 
a^{-2(1-\alpha)}
\left[
\mathcal{G}_{ab}\nabla^2\phi_b
+\gamma_{abc}
\vec{\nabla}\phi_b\cdot\vec{\nabla}\phi_c
\right]
-a^{2\alpha}\frac{\partial V}{\partial\phi_a}\,,
\end{array}\right.
\label{eqn_NMKcontinuum}\\
{\rm where}~~~~~~~~ (\mathcal{F}_a)_{bc} \equiv \mathcal{G}_{ae}\mathcal{G}^{-1}_{ec,d}\mathcal{G}^{-1}_{db} + \left(\mathcal{G}_{ae,d}-\frac{1}{2}\mathcal{G}_{ed,a}\right)\mathcal{G}^{-1}_{ec}\mathcal{G}^{-1}_{db}\,.~~~~~~~~~~~~
\label{eq_FaNMK}
\end{eqnarray}
```

To solve these equations, one must also specify the equation of motion for the scale factor, whose evolution is governed by Eq.$~$\eqref{eq_FriedmannD2a}. For scalar fields with non-canonical kinetic terms, the kinetic and gradient energy-density contributions, compared with Eq.$~$\eqref{eq_energy-contributions}, become

[](){ #eq_energy-contrib-NMK }
```math
K_{\rm NMK}
=
\frac{1}{2a^{2\alpha}}
\mathcal{G}_{ab}\phi_a'\phi_b'\,,
\qquad
G_{\rm NMK}
=
\frac{1}{2a^2}
\mathcal{G}_{ab}
\vec{\nabla}\phi_a\cdot\vec{\nabla}\phi_b\,.
\label{eq_energy-contrib-NMK}
```

The corresponding Friedmann equations are

[](){ #eq_FriedmannHubble-NMK }
[](){ #eq_FriedmannDDa-NMK }
```math
\begin{eqnarray}
\left(\frac{a'}{a}\right)^2
&=&
\frac{a^{2\alpha}}{3m_p^2}
\left\langle
K_{\rm NMK}+G_{\rm NMK}+V
\right\rangle\,,
\label{eq_FriedmannHubble-NMK}\\
{\rm and} ~~~~~~~~~~~~~~~~~~~~~~~~~~~ && \nonumber \\
\frac{a''}{a}
&=&
\frac{a^{2\alpha}}{3m_p^2}
\left\langle
(\alpha-2)K_{\rm NMK}
+\alpha G_{\rm NMK}
+(\alpha+1)V
\right\rangle\,.
\label{eq_FriedmannDDa-NMK}
\end{eqnarray}
```
where, as usual, $\langle\cdots\rangle$ denotes volume averaging over regions sufficiently large to encompass all relevant field wavelengths.

<div style="height: 20px;"></div>

[](){ #ALP_th }
**Axion-Gauge interactions**

Axion-like particles (ALPs) enjoy a shift-symmetry $\phi=\phi+C$ that allows them to couple derivatively to gauge fields through Chern–Simons terms, as $\phi F\tilde{F}$ (Abelian) or $\phi G\tilde{G}$ (non-Abelian). Here we consider an axion–$U(1)$ sector described by the action

[](){ #eq_AxionAction }

```math
S =
\int d^4x\,\sqrt{-g}
\left[
\frac{1}{2}m_p^2R
-\frac{1}{2}\partial_\mu\phi\,\partial^\mu\phi
-V(\phi)
-\frac{1}{4}F_{\mu\nu}F^{\mu\nu}
+\frac{1}{4}\frac{\phi}{\Lamda}
F_{\mu\nu}\tilde F^{\mu\nu}
\right]\,.
\label{eq_AxionAction}
```
where $\phi$ is a real pseudo-scalar field, the axion-like particle (ALP), and $V(\phi)$ its potential (which might break the shift symmetry explicitly in some scenarios). The field-strength tensor of the $U(1)$ gauge field $A_\mu$ is defined as in standard canonical theories, $F_{\mu\nu}\equiv\partial_\mu A_\nu-\partial_\nu A_\mu$. The parameter $\alpha_\Lambda\equiv m_p/\Lambda$ characterizes the dimensionless ALP-gauge coupling strength associated with the scale $\Lambda$. The dual field-strength tensor is defined as $\tilde F_{\mu\nu}\equiv\frac{1}{2}\epsilon_{\mu\nu\rho\sigma}F^{\rho\sigma}$, where $\epsilon_{\mu\nu\rho\sigma}$ is the four-dimensional Levi-Civita pseudotensor in curved spacetime, normalized as $\epsilon_{0123}=1/\sqrt{-g}$. Varying the action with respect to $\phi$ and $A_i$ in an FLRW background, and considering the temporal gauge, $A_0=0$, yields the equations of motion in $\alpha$-time as

[](){ #eq_axion_eom }

[](){ #eq_axion_gauge_eom }

```math
\begin{eqnarray}
\phi''
+(3-\alpha)\mathcal{H}\phi'
-a^{2(\alpha-1)}\vec{\nabla}^{\,2}\phi
+a^{2\alpha}V_{,\phi}
&=&
\frac{\alpha_\Lambda}{m_p}
a^{\alpha-3}\vec E\cdot\vec B\,,
\label{eq_axion_eom}
\\
E_i'
+(1-\alpha)\mathcal{H}E_i
-a^{2(\alpha-1)}\epsilon_{ijk}\partial_jB_k
&=&
\frac{\alpha_\Lambda}{m_p}a^{\alpha-1}
\left(
\phi'B_i+\epsilon_{ijk}\partial_j\phi\,E_k
\right)\,.
\label{eq_axion_gauge_eom}
\end{eqnarray}
```
where primes denote derivatives with respect to $\alpha$-time, $\eta$, and $E_i \equiv F_{0i} = A_i'$. The system is subject to the Gauss constraint, which, in the absence of external charged currents, reduces to

[](){ #eq_axion_gauss }

```math
\partial_iE_i
=
-\frac{\alpha_\Lambda}{m_p}
a^{\alpha-1}\partial_i\phi\,B_i\,.
\label{eq_axion_gauss}
```

The local energy density and pressure of the combined axion–gauge sector are

[](){ #eq_axion_rho }

[](){ #eq_axion_p }

```math
\begin{eqnarray}
\rho
&=&
K_\phi+G_\phi+V(\phi)+K_{U(1)}+G_{U(1)}\,,
\label{eq_axion_rho}
\\
p
&=&
K_\phi-\frac{1}{3}G_\phi-V(\phi)
+\frac{1}{3}
\left[
K_{U(1)}+G_{U(1)}
\right]\,.
\label{eq_axion_p}
\end{eqnarray}
```

The individual kinetic and gradient contributions of the scalar and vector fields coincide with those defined in Eq.$~$\eqref{eq_energy-contributions}. If the axion–$U(1)$ gauge sector dominates the energy budget of the Universe, the expansion is governed by the Friedmann equations, Eqs.$~$\eqref{eq_FriedmannHub} and $\eqref{eq_FriedmannD2a}, retaining only the contributions from this sector.

## Scalar-Gauge-Fluid Dynamics { #subsec_eomFluids }

Coming soon ...

<!-- 
**Fluid Dynamics**

**Scalar-Fluid Dynamics**

**Gauge-Fluid Dynamics**
 -->

<!-- **Scalar-Gauge-Fluid Dynamics** -->

## Gravitational Waves { #subsec_eomGWs }

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
In practice, however, it is more convenient to define an effective anisotropic stress tensor containing only those contributions to $\Pi_{ij}$ that have a non-zero TT projection. For example, for a generic model consisting of canonically normalized scalars and Abelian gauge fields, this takes the form,
[](){ #eq_GWEffectiveAnisotropicStress }
```math
\begin{align}
\label{eq_GWEffectiveAnisotropicStress}
\Pi_{ij}^\mathrm{eff}=\sum_a \nabla_i\phi_a \nabla_j\phi_a+2\sum_b \text{Re}\left[ \left(D_i^A\varphi_b\right)^*D_j^A\varphi_b \right]-a^{-2\alpha}E_iE_j-a^{-2}B_i B_j \,,
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
\equiv
P_{il}(\hat{\bf k})P_{jm}(\hat{\bf k})
-{1\over2}P_{ij}(\hat{\bf k})P_{lm}(\hat{\bf k})\,,\quad\quad \text{with}\quad\quad P_{ij}(\hat{\bf k})
\equiv
\delta_{ij}-\hat k_i\hat k_j\,,
\end{align}
```
and $\hat k_i\equiv {k_i\over k}$. This guarantees the projected tensor is both transverse, $k_i\Pi_{ij}^{\rm TT}=0$, and traceless, $\Pi_{ii}^{\rm TT}=0$.

<!--Numerically solving Eq.$~$\eqref{eq_GWEOMcontinuum} directly requires to determine the TT projected anisotropic stress tensor every time the GWs need to be evolved, which is a very costly operation. An alternative was proposed in Ref. [GarciaBellido_2008], based on the use of auxiliary fields sourced by $\Pi_{ij}^\text{eff}$. While the original proposal used six unphysical degrees of freedom to simulate the GWs, it is possible to reduce this number to five, as discussed in $\mathtt{The~Art{\text -}II}$ [BaezaBallesteros_2025tme],  by using a symmetric traceless tensor of auxiliary fields, $v_{ij}$, which obeys $v_{33}=-(v_{11}+v_{22})$. If these fields are evolved following
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
This is the algorithm used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate the evolution of GWs.-->

Finally, it is worth mentioning GW observables. The most relevant quantity related to GWs is the energy density of the GW background,
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
