In this section we explain how to simulate in CosmoLattice a model containing both charged scalar fields and gauge fields (either Abelian or non-Abelian).

### Scalar-gauge field dynamics: program variables

CosmoLattice is capable of simulating scalar-gauge field theories in an expanding universe. The action of a generic theory (with canonically normalized scalar fields) that can be simulated by CosmoLattice is the following
[](){ #eq_Lagrangian }
```math
\begin{align}
S & =- \int d^4 x \left\{\frac{1}{2}\partial_{\mu} \phi \partial ^{\mu}\phi + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} + V(\phi,|\varphi|, |\Phi|) \right\}  ,  \\
\tag{50}
\end{align}
```

which contains several types of scalar and gauge fields. In order to simplify notation, we have only added one copy of each field species, but CosmoLattice can also handle multiple fields of the same kind. This theory contains three types of scalar fields: a singlet $\phi$, a $U(1)$-charged scalar (complex field) $\varphi$, and a $SU(2)$ doublet $\Phi$. The latter two can be written in terms of real components as in Eq. ([*3*][eq_ChargedScalars]). The complex field $\varphi$ can be charged under a $U(1)$ gauge symmetry, while the doublet can be charged under both $U(1)$ and $SU(2)$. The scalar potential of the theory is $V = V(\phi, |\varphi|, |\Phi|)$, which depends on $\phi$, as well as on the modulus of the complex and doublet scalars, $|\varphi|$ and $|\Phi|$. The corresponding covariant derivatives and field strengths in action ([*50*][eq_Lagrangian]) are defined as
[](){ #eq_AbCovDerivCont }
[](){ #eq_CovDerivCont }
[](){ #eq_FmnAbelian }
[](){ #eq_GmnNonAb }
```math
\begin{align}
D_{\mu}^{\rm A} &=  \partial _{\mu} - i Q_A^{(\varphi)} g_{_A}A_\mu  ,
\tag{51}\\
D_{\mu} &=
\mathcal{I}D^{\rm A}_\mu
- i g_B Q_B B_{\mu}^a  T_a     =    \mathcal{I}\left( \partial _{\mu} - i Q_A^{(\Phi)} g_{_A}A_\mu \right)
- i g_B Q_B B_{\mu}^a  T_a  , \tag{52}\\
F_{\mu \nu} &= \partial_{\mu}  A_{\nu} - \partial_{\nu} A_{\mu}  , \tag{53}\\
G_{\mu \nu} &= \partial_{\mu} B_{\nu} - \partial_{\nu} B_{\mu} - i[B_\mu,B_\nu]  , \tag{54}
\end{align}
```

where $Q_{A}^{(\varphi)}$ and $Q_{A}^{(\Phi)}$ are the Abelian charges of $\varphi$ and $\Phi$ respectively, $Q_B$ is the non-Abelian charge of $\Phi$, $g_A$ and $g_B$ are the corresponding gauge couplings, and $\mathcal{I}$ is the 2$\times$2 identity matrix. Note that, using the properties of the $SU(N)$ generators, $G_{\mu \nu}$ can be written as
```math
\begin{align}
G_{\mu \nu} \equiv G_{\mu \nu}^a T_a   , \hspace{0.4cm} G_{\mu \nu}^a \equiv \partial_{\mu} B_{\nu}^a - \partial_{\nu} B_{\mu}^a + f^{a b c} B_{\mu}^b B_{\nu}^c  ,
\end{align}
```

where $f_{abc}$ are the structure constants of the SU(N) group, determined by the relation $[T_a, T_b] = i f_{abc} T_c$. CosmoLattice is only implemented (at least for the time being) for $SU(2)$, for which we simply have $T_a \equiv \sigma_a /2$, with $\sigma_a$ the Pauli matrices. In CosmoLattice we evolve the fields in the temporal gauge, so $A_{0} = B_0^a = 0$. Furthermore, we define the Abelian and non-Abelian electric and magnetic fields as follows,
[](){ #eq_ElectricMagneticDefs }
```math
\tag{55}
\mathcal{E}_i \equiv F_{0i},
\quad \mathcal{B}_i = \frac{1}{2} \epsilon_{i j k} F^{j k},
\quad \mathcal{E}_i^a \equiv G_{0i}^a,
\quad \mathcal{B}_i^a = \frac{1}{2} \epsilon_{i j k} G^{j k}_a,
```

with $\epsilon_{ijk}$ the Levi-Civita symbol. These expressions represent gauge-invariant physical quantites.

!!! note
    **Important Note:** As described in Section [*Program variables*][subsec_LatticeScalars] in the context of scalar theories, on the lattice we operate in a set of dimensionless spacetime and field variables called **program variables**. For scalar theories, these were defined in Eq. ([*29*][eq_FieldSpaceTimeNaturalVariables]) in terms of the three constants $\{f_*,\omega_*,\alpha\}$, that must be judiciously chosen for each specific model. The same definitions hold for the scalar sector(s) of scalar-gauge theories, where we also introduce new dimensionless program variables for the gauge fields. Putting all program variables together, we have
    [](){ #eq_GaugeProgramVar }
    
    ```math
    
    \begin{align}
    d\tilde\eta \equiv a^{- \alpha} \omega_* dt , \hspace{0.4cm}
    d\tilde x^i \equiv \omega_* dx^i ,
    \hspace{0.4cm}
    \tilde\phi = \frac{\phi}{f_*}  , \hspace{0.4cm}
    \tilde\varphi = \frac{\varphi}{f_*}  , \hspace{0.4cm} \widetilde{\Phi} = \frac{\Phi}{f_*}  , \hspace{0.4cm}  \widetilde{A}_\mu=\frac{A_\mu }{\omega_*}  , \hspace{0.4cm} \widetilde B_{\mu}^a = \frac{B_{\mu}^a}{\omega_*}  . \tag{56}
    \end{align}
    
    ```

!!! note
    **Important Note:** CosmoLattice can run with an arbitrary number of scalar singlets, $U(1)$ complex scalars and $SU(2)$ doublets. CosmoLattice has been only tested however when considering a single $U(1)$ gauge field and a single $SU(2)$ gauge field. In principle, the code also works with multiple $U(1)$ gauge fields (coupled or not to scalars), but this feature has not thoroughly tested, so it is deactivated by default: the program crashes when a model is written with more than one $U(1)$ gauge field. It can be re-activated at one own's risk by commenting out line `216` in the file  `src/include/CosmoInterface/abstractmodel.h`. In the case of $SU(2)$ gauge fields, only one of such fields can be considered at once.

We can see that the U(1)- and SU(2)-charged scalars are re-scaled in the same way as the singlet scalar fields. The gauge fields, however, are instead re-scaled by the parameter $\omega_*$. Similarly, we define program variables for the field strengths and covariant derivatives as follows:
```math
\begin{align}
\widetilde{F}_{\mu \nu} \equiv F_{\mu \nu} / \omega_*^2 , \hspace{0.4cm} \widetilde{G}_{\mu \nu}^a \equiv G_{\mu \nu}^a / \omega_*^2 , \hspace{0.4cm} \widetilde{D}_{\mu}^A \equiv D_{\mu}^A / \omega_* , \hspace{0.4cm} \widetilde{D}_{\mu} \equiv D_{\mu} / \omega_* .
\end{align}
```

The program potential is defined, as before, as
[](){ #eq_ProgramPotMultiScalar }
```math
\tag{57}
\tilde{V} (\tilde{\phi}, |\tilde{\varphi}|, |\widetilde{\Phi}|) \equiv \frac{1}{f_*^2 \omega_*^2} V(f_* \tilde \phi, f_* |\tilde \varphi|, f_* |\widetilde \Phi|  )  .
```

### Scalar-gauge field dynamics: equations of motion

In terms of the program variables, the field equations can be written as
[](){ #eq_singlet-eom }
[](){ #eq_higgsU1-eom }
[](){ #eq_higgsSU2-eom }
[](){ #eq_U1eom }
[](){ #eq_SU2eom }
[](){ #eq_GaussU1-eom }
[](){ #eq_GaussSU2-eom }
```math
\begin{align}
\tilde \phi'' - a^{-2(1 - \alpha)} {\widetilde \nabla}^{ 2} \tilde \phi + (3 - \alpha)\frac{{a'}}{a} {\tilde  \phi'} &= - a^{2 \alpha} \widetilde V_{,\widetilde \phi}  , \tag{58} \\
\tilde \varphi'' - a^{-2(1 - \alpha)} {\widetilde{\vec D}}_{\hspace{-0.5mm}A}^{ 2}\tilde{\varphi} + (3 - \alpha)\frac{{a'}}{a}  {\tilde  \varphi'} &= - \frac{a^{2 \alpha}}{2} \widetilde V_{,|\widetilde \varphi|} \cdot \frac{\tilde  \varphi}{ |\tilde  \varphi |}   , \tag{59}\\
\widetilde \Phi'' - a^{-2(1 - \alpha)} {\widetilde{\vec D}}^{ 2}\widetilde \Phi + (3 - \alpha)\frac{{a'}}{a}  {\widetilde \Phi'} &= - \frac{a^{2 \alpha}}{2} \widetilde V_{,|\widetilde \Phi|} \cdot \frac{\widetilde \Phi}{ |\widetilde \Phi |}  , \tag{60}
\\
\tilde \partial_0 \widetilde F_{0i} - a^{-2(1 - \alpha )}\tilde  \partial_j \widetilde F_{ji} + (1 - \alpha) \frac{{a'}}{a} \widetilde F_{0i} &= \left( \frac{f_*}{\omega_*} \right)^2
a^{2 \alpha}\widetilde J^A_i  , \tag{61}
\\
(\widetilde{\mathcal{D}}_0 )_{a b} (\widetilde G_{0i})^b - a^{-2(1 - \alpha )} (  \widetilde{\mathcal{D}}_j )_{a b} (\widetilde{G}_{ji} )^b + (1 - \alpha) \frac{{a'}}{a} (\widetilde{G}_{0i} )^b &= \left( \frac{f_*}{\omega_*} \right)^2 a^{2 \alpha}(\widetilde{J}_i)_a  , \tag{62}
\\
\tilde \partial_i \widetilde F_{0i} &= \left( \frac{f_*}{\omega_*} \right)^2 a^2 \widetilde J^A_0  , \tag{63}\\
(\widetilde{\mathcal{D}}_i )_{a b} (\widetilde{G}_{0i})^b &= \left( \frac{f_*}{\omega_*} \right)^2 a^2(\widetilde{J}_0)_a  , \tag{64}
\end{align}
```

where $(\widetilde{\mathcal{D}}_{\nu}O)_a = (\widetilde{\mathcal{D}}_{\nu})_{a b}O_b \equiv ( \delta_{a b}  \tilde \partial_{\nu} - f_{abc} \tilde B_{\nu}^c ) O_b$, and the currents are given by
[](){ #eq_AbelianCurrent }
[](){ #eq_NonAbelianCurrent }
```math
\begin{align}
\tag{65}
\hspace{1.8cm} \widetilde J_A^\mu &= 2 g_AQ_A^{(\varphi)} \mathcal{I}m [ \tilde \varphi^{*} ( \widetilde{D}_A^{\mu} \tilde \varphi )] + 2 g_AQ_A^{(\Phi)} \mathcal{I}m [ \widetilde \Phi^\dagger (\widetilde D^{\mu} \widetilde \Phi  )] ,\\
\tag{66}
\hspace{1.8cm} \widetilde J_a^\mu &= 2g_BQ_B\mathcal{I}m [ \widetilde \Phi^{\dagger} T_a( \widetilde{D}^{\mu} \widetilde \Phi )] .
\end{align}
```

Similarly, we define the *program energy density* and *program pressure density* as
```math
\begin{align} \tilde{\rho} \equiv \frac{\rho}{f_*^2 \omega_*^2}  \hspace{-0.2cm} &= \hspace{-0.2cm} \tilde{K}_{\phi} + \tilde{K}_{\varphi} + \tilde{K}_{\Phi} + \tilde{G}_{\phi} + \tilde{G}_{\varphi} + \tilde{G}_{\Phi} +  \tilde{K}_{U(1)} + \tilde{G}_{U(1)} + \tilde{K}_{SU(2)} + \tilde{G}_{SU(2)} + \tilde{V}  , \\
\tilde{p} \equiv \frac{p}{f_*^2 \omega_*^2} \hspace{-0.2cm} &= \hspace{-0.2cm} \tilde{K}_{\phi} + \tilde{K}_{\varphi} + \tilde{K}_{\Phi} -{1\over3}(\tilde{G}_{\phi} + \tilde{G}_{\varphi} + \tilde{G}_{\Phi}) + {1\over3}  (\tilde{K}_{U(1)} + \tilde{G}_{U(1)} + \tilde{K}_{SU(2)} + \tilde{G}_{SU(2)} ) - \tilde{V}  ,
\end{align}
```

where each of the individual kinetic, gradient, and potential energy contributions are
[](){ #eq_energy-contrib }
```math
\begin{align}
\begin{array}{lcl} \tag{67}
\widetilde {K}_{\phi} &= \frac{1}{2 a^{2\alpha} } \tilde \phi'^2 \\
\widetilde {K}_{\varphi} &= \frac{1}{a^{2\alpha} } (\widetilde D_0^A \widetilde  \varphi)^*(\widetilde D_0^A \widetilde \varphi)
\\
\widetilde {K}_{\Phi} &= \frac{1}{a^{2\alpha} } (\widetilde D_0 \widetilde \Phi )^\dagger(\widetilde D_0 \widetilde \Phi)
\\
\end{array}
\hspace{0.1cm};\hspace{0.3cm}
\begin{array}{lcl}
\widetilde {G}_{\phi} &= \frac{1}{2 a^2} \sum_i (\tilde \partial_i \tilde \phi)^2
\\
\widetilde {G}_{\varphi} &= \frac{1}{a^2} \sum_i (\widetilde D_i^A \varphi)^*(\widetilde D_i^A \widetilde \varphi)
\\
\widetilde {G}_{\Phi} &= \frac{1}{a^2} \sum_i (\widetilde  D_i\widetilde  \Phi)^\dagger(\widetilde D_i \widetilde \Phi)
\\
\end{array}
\hspace{0.1cm};\hspace{0.3cm}
\begin{array}{lcl}
\widetilde {K}_{U(1)} &= \frac{1}{2 a^{2 + 2 \alpha}} ( {\omega_* \over f_*} )^2  \sum_{i} \widetilde  F_{0i}^2
\\
\widetilde {K}_{SU(2)} &= \frac{1}{2 a^{2 + 2 \alpha}} ( {\omega_* \over f_*} )^2 \sum_{a,i} (\widetilde G_{0i}^a)^2
\\
\widetilde {G}_{U(1)} &= \frac{1}{2 a^4} ( {\omega_* \over f_*} )^2 \sum_{i,j < i} \widetilde F_{ij}^2
\\
\widetilde {G}_{SU(2)} &= \frac{1}{2 a^4} ( {\omega_* \over f_*} )^2 \sum_{a,i,j < i}  (\widetilde G_{ij}^a)^2    . \\
\end{array}
\\
\text{(Kinetic-Scalar)} \hspace{2.5cm} \text{(Gradient-Scalar)} \hspace{2.75cm} \text{(Electric & Magnetic)} \hspace{1cm}
\end{align}
```

If the expansion of the Universe is self-consistent, i.e. it is sourced by volume averages of the energy and pressure densities of the simulated fields, the scale factor evolution can be obtained from the Friedmann equations
[](){ #eq_FriedmannHubble }
[](){ #eq_FriedmannDDa }
```math
\begin{align}\tag{68}
\mathcal{H}^2    \equiv    \frac{a'^{ 2}}{a^2} &=  \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ \widetilde E_K^{\phi} + \widetilde E_K^{\varphi} + \widetilde E_K^{\Phi} + \widetilde E_G^{\phi} + \widetilde E_G^{\varphi} + \widetilde E_G^{\Phi} + \widetilde E_K^A + \widetilde E_K^B + \widetilde E_G^A + \widetilde E_G^B + \widetilde E_V \right]  ,
\\
\tag{69}
{a''\over a} &= \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ (\alpha-2)(\widetilde E_K^{\phi} + \widetilde E_K^{\varphi} + \widetilde E_K^{\Phi}) + \alpha(\widetilde E_G^{\phi} + \widetilde E_G^{\varphi} + \widetilde E_G^{\Phi}) + (\alpha + 1)\widetilde E_V \right.\\
& \hspace{5.1cm}\left. +  (\alpha-1)(\widetilde E_K^A + \widetilde E_K^B + \widetilde E_G^A + \widetilde E_G^B) \right]  ,
\end{align}
```

where we have defined the following volume-average energy contributions: $E_{K}^{f} = \langle \widetilde{K}_{f} \rangle$ and $E_{G}^{f} = \langle \widetilde{G}_{f} \rangle$ for the scalar fields $f=\phi,\varphi,\Phi$; $\widetilde E_{K}^{A} = \langle \widetilde{K}_{U(1)} \rangle$, $\widetilde E_{G}^{A} = \langle \tilde{G}_{U(1)} \rangle$, $\widetilde E_{K}^{B} = \langle \widetilde{K}_{SU(2)} \rangle$, and $\widetilde E_{G}^{B} = \langle \widetilde{G}_{SU(2)} \rangle$ for the gauge fields, and $\widetilde{E}_V = \langle \widetilde{V} \rangle$ for the potential energy. Instead, if the expansion is sourced by an external energetically-dominant fluid with constant equation of state $w$, $a(\eta)$ is given by the power-law function ([*36*][eq_ScaleFactorPowerLaw]).

The evolution algorithms implemented in CosmoLattice use a discretized versions of Eqs. ([*58*][eq_singlet-eom])-([*62*][eq_SU2eom]) to solve for the field dynamics, and a lattice version of Eq. ([*69*][eq_FriedmannDDa]) to solve for the scale factor. Eqs. ([*63*][eq_GaussU1-eom]) and ([*64*][eq_GaussSU2-eom]) are the Gauss constraints of the U(1) and SU(2) gauge sectors respectively, which must be satisfied (by their lattice version counterparts) during all times during the simulation. Analogously, Eq. ([*68*][eq_FriedmannHubble]), which represents the Hubble constraint, must also be satisfied (again by its lattice analogue) all throughout the simulation. In a sense, this checks the ability of a given integrator to conserve energy). CosmoLattice monitors in particular the degree of conservation of both the Gauss and Hubble constraints, providing in this way a procedure for checking the validity of the numerical integration of the EOM. Our discretization techniques guarantee that the Gauss constraints are obeyed up to machine precision, see Ref. [@Figueroa_2020rrl] for details. On the other hand, the Hubble constraint ([*68*][eq_FriedmannHubble])  holds numerically only to a certain degree of approximation, possibly reaching down to machine precision (depending on the model) only in the case of the highest order integrators like `VV10`.

### The model and input files for gauge field theories

Let us now explain how to implement a model with Abelian and non-Abelian gauge fields in CosmoLattice. Two gauge models are already implemented in CosmoLattice: `lphi4U1`, which includes a complex scalar charged under a $U(1)$ gauge symmetry and one Abelian gauge field (like in scalar-electrodynamics), and `lphi4SU2U1`, which contains a scalar field charged under $U(1)\times SU(2)$, and hence with both one Abelian and one non-Abelian gauge field (similar to the electroweak sector of the Standard Model). These are models ready-to-use as templates for your own models. In this manual, we will explain the implementation of the second model as an example, as it contains all possible field species and interactions that can be currently simulated with CosmoLattice.

The model `lphi4SU2U1` consists of a doublet $\Phi$ charged under a $SU(2)\times U(1)$ gauge group, coupled to one Abelian gauge field $A_{\mu}$ and one non-Abelian one $B_{\mu}^a$ via the previously defined covariant derivative. We also couple $\Phi$ to a scalar singlet $\phi$ and to a $U(1)$-charged scalar field $\varphi$ via quadratic interactions. We will consider a scenario in which the doublet $\Phi$ acts as the dominant mother field, and simulate its non-perturbative decay into the gauge fields and the other scalars, which is induced via parametric field excitations of the other fields due to the coherent oscillations of $\Phi$. For the sake of simplicity, we will consider like if this was a preheating scenario where $\Phi$ plays the role of the inflaton field, coupled to the daughter fields $\phi, \varphi, A_\mu$ and $B_\mu = T_aB_\mu^a$. In particular, we will implement the theory described by action ([*50*][eq_Lagrangian]) with the following scalar potential,
[](){ #eq_PotGauge }
```math
V(\phi,|\varphi|,|\Phi |) = \lambda |\Phi|^4  + g^2 |\Phi |^2 \phi^2 + 2 h^2 |\Phi |^2 |\varphi|^2  , \tag{70}
```

where $\lambda$, $g$, and $h$ are dimensionless coupling constants. The first term is the inflaton potential, and the second and third terms are quadratic interactions between the inflaton and $\phi$ and $\varphi$ respectively. As said, the theory contains one field of each kind. We will assume that at the onset of the simulation (say at the end of slow-roll inflation), the inflaton amplitude has an initial non-zero homogeneous component with modulus $|\Phi| = |\bar{\Phi}_*|$, while the homogeneous components of the rest of scalar and gauge fields are set to zero (this is natural as they are massive during inflation, with their mass induced by the large amplitude of $\Phi$, and hence they are not excited initially). The post-inflationary oscillations of $\Phi$ trigger a resonant growth of $\phi$ and $\varphi$ due to resonant effects via the second and third terms of the potential, and also a parametric excitation of the gauge fields $A_{\mu}$ and $B_{\mu}^a$ due to their coupling to $\Phi$ via the covariant derivative $(D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) \in g_A^2 |\Phi|^2 A_{\mu}^2$, $g_B^2 |\Phi|^2 {B_{\mu}^{a}}^2$.

This scenario is implemented in the model file `src/models/lphi4SU2U1.h`, and the corresponding input parameter file is in `src/models/parameter-files/lphi4SU2U1.in`. Most of the parameters defined in the input file are the same as for singlet scalar fields, see Section [*Running the program with an input parameter file*][subsec_Input-Scalars]. However, there are several extra parameters that need to be set now. First, let us specify the initial homogeneous components of all scalar fields as follows:

@emgithub(models/parameter-files/lphi4SU2U1.in:initial_conditions)

Above `initial_amplitudes` and `initial_momenta` contain the initial homogenous amplitudes of the scalar singlet, $\phi_*$ and $\dot{\phi}_*$. If we had more than one singlet, their initial conditions would be specified in a vector form, as explained in Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md). Parameters `cmplx_field_initial_norm` and `cmplx_momentum_initial_norm` contain the initial **absolute values** of the complex field amplitude and its time-derivative , i.e. $|\varphi_*| \equiv \sqrt{(\varphi_{0*}^2 + \varphi_{1*}^2)/2}$ and $|\dot{\varphi}_*| \equiv \sqrt{(\dot{\varphi}_{0*}^2 + \dot{\varphi}_{1*}^2)/2}$ respectively (although in this example we set these to zero). Similarly, parameters `SU2Doublet_initial_norm` and `SU2Doublet_initial_momenta_norm` contain $|\Phi_*| \equiv \sqrt{ \sum_{n=0}^3 \varphi_{n*}^2 / 2}$ and $|\dot{\Phi}_*| \equiv \sqrt{ \sum_{n=0}^3 \dot{\varphi}_{n*}^2 / 2 }$ respectively. **As before, initial amplitudes must be introduced in $\text{GeV}$, and initial derivatives in $\text{GeV}^2$**.

We also need to specify the parameters that deal with the gauge couplings and charges in the covariant derivatives. This is done as follows:

@emgithub(models/parameter-files/lphi4SU2U1.in:gauge_couplings)

Here, `gU1s` fixes $g_A$, `gSU2s` fixes $g_B$, `CSU1Charges` fixes $Q_A^{(\varphi)}$, `SU2DoubletU1Charges` fixes $Q_A^{(\Phi)}$, and `SU2DoubletSU2Charges` fixes $Q_B$. Although in our example we only consider one field for each species, multiple couplings and charges can also be specified in vector form.

Finally, we also want to specify the model parameters that appear in the potential ([*70*][eq_PotGauge]). This is done as follows,

@emgithub(models/parameter-files/lphi4SU2U1.in:model_parameters)
where we have defined three parameters: `lambda` (which represents $\lambda$), `qG` (which contains $q_G \equiv g^2 / \lambda$, and `qH` (which contains $q_H \equiv h^2 / \lambda$).

Let us now analyze the model file `src/models/lphi4-SU2U1.h`, which can be used as a template to simulate different gauge field theories. We start by specifying the field content of our theory:
@emgithub(models/lphi4SU2U1.h:model_pars)

In lines `24`-`28`, we have specified the number of fields of each species: `NScalars` refers to $\phi$, `NCScalars` refer to $\varphi$, `NU1Flds` refer to $A_{\mu}$, `NSU2Doublet` refers to $B_{\mu}^a$, and `NSU2Flds` refers to $\Phi$. In line `29` we specify the number of terms in the potential, which is $3$ in our case. Finally, in lines `34`-`38` we defined three types of *coupling managers*, which deal with the couplings between the scalar and gauge fields in the covariant derivatives. `U1CsCouplings` must be defined if $\varphi$ couples to $A_{\mu}$, `U1SU2DoubletCouplings` must be defined if $\Phi$ couples to $A_{\mu}$, and `SU2SU2DoubletCouplings` must be defined if $\Phi$ couples to $B_{\mu}^a$. Finally, in line `42`, we have specified that the name of our model is `lphi4SU2U1`, in agreement with the name of the header file.

After that, inside the template model, we declare several model parameters (`g`, `h`, `lambda`, `qG`, `qH`) as private variables:
@emgithub(models/lphi4SU2U1.h:class_declaration)

We then use the parser to read the initial homogeneous components of the scalar field amplitudes and derivatives as follows:
@emgithub(models/lphi4SU2U1.h:constructor_initial_conditions)

We can see that for the scalar singlet $\phi$, the initial amplitude and derivative are read by the parser from the input file in lines `72`-`73`, in the same way as described in Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md). As explained, these values must be stored in the variables `fldS0(0_c)` and `piS0(0_c)` respectively, where `0` denotes the label of the field. Regarding $\varphi$, in lines `76`-`77` we use the same technique to read the initial values of $|\varphi_*|$ and $|\dot{\varphi}_*|$. For convenience, we stored them in the local variables `normCmplx0` and `normPiCmplx0` respectively. Then, in lines `81`-`82` we specify how these amplitudes are distributed between the two components of $\varphi$, i.e. $\varphi_0$ and $\varphi_1$ [see Eq. ([*3*][eq_ChargedScalars])]. In the example, this is done with the `Complexify` function, which creates a two-component vector storing the real and imaginary parts of a complex number. For consistency, one must always guarantee that the initial components satisfy $\sqrt{(\varphi_{0*}^2 + \varphi_{1*}^2) /2} = |\varphi_*|$ and $\sqrt{(\dot{\varphi}_{0*}^2 + \dot{\varphi}_{1*}^2) /2} = |\dot{\varphi}_*|$. In the example, we have decided to set the same initial power to all components, so that $\varphi_{0*} = |\varphi_*| $and $\varphi_{1*} = |\varphi_*|$, as well as $\dot{\varphi}_{0*} = |\dot{\varphi}_*|$ and $\dot{\varphi}_{1*} = |\dot{\varphi}_*| $. In any case, the created vectors must be stored in the model variables `fldCS0(0_c)` (for the amplitude) and `piCS0(0_c)` (for the time-derivative), where `0` is the field label.

We use a similar technique in lines `85`-`91` to specify the initial conditions of $\Phi_*$ and $\dot{\Phi}_*$. First, in lines 53-54 we store the values of $|\Phi_*|$ and $|\dot{\Phi}_*|$ specified in the input file in the local variables `normDoublet0` and `normPiDoublet0` respectively. We then need to indicate how these are distributed between the four components of the doublets $\Phi_*$ and $\dot{\Phi}_*$, i.e. $\varphi_{n*}$ and $\dot{\varphi}_{n*}$ for $n=0,1,2,3$ [see Eq. ([*3*][eq_ChargedScalars])]. For consistency, we must always ensure that $\sqrt{ \sum_{n=0}^3 \varphi_{n*}^2 / 2} = |\Phi_*|$ and $\sqrt{ \sum_{n=0}^3 \dot{\varphi}_{n*}^2 / 2 } = |\dot{\Phi}_*|$. In the example, this is done in lines `90`-`91` with the `MakeSU2Doublet` function, which creates a SU(2) doublet with the same amplitude for all the components (in this case, $\varphi_{n*} = |{\Phi}_*| /\sqrt{2}$ and $\dot{\varphi}_{n*} = |\dot{\Phi}_*| / \sqrt{2}$. Finally, the corresponding initial SU(2) doublets must be stored in the variables `fldSU2Doublet0(0_c)` and `piSU2Doublet0(0_c)`, with `0` again denoting the field label.

We now proceed to read model parameters `qG`, `qH`, and `lambda` from the input file in the usual way, as well as to compute new parameters `g` and `h` as follows,
@emgithub(models/lphi4SU2U1.h:model_parameters)

The next step is to define appropriate program variables for the model, as well as set the initial masses. The potential of the dominating oscillatory field is quartic, similar to the scalar case considered in Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md), so mimicking Eq. ([*37*][eq_lphi4-ProgVar]), we choose them as
```math
\begin{align}
f_*=|\overline{\Phi}_{*} | ,  \omega_*=\sqrt{\lambda} | \overline{\Phi}_* |,  \alpha=1  .
\end{align}
```

This is done in the code as follows,
@emgithub(models/lphi4SU2U1.h:rescaling)

Finally we call the generic function responsible to set the masses of the matter fields together with the initial potential
@emgithub(models/lphi4SU2U1.h:masses_setup)

We now need to specify the scalar potential of our field theory. As for scalar singlet theories, any gauge field theory in CosmoLattice is implemented by means of the *program potential*, defined in Eq. ([*57*][eq_ProgramPotMultiScalar]). In our example, it is given by
```math
\begin{align}
\widetilde V( \tilde\phi, |\tilde\varphi|, |\tilde\Phi| ) \equiv \frac{1}{f_*^2 \omega_*^2}V(f_*\tilde \phi,f_*|\tilde \varphi |, f_*|\tilde \Phi |) = |\tilde\Phi|^4 + \frac{g^2}{\lambda}|\tilde\Phi|^2\tilde\phi^2 + 2 \frac{h^2}{\lambda}|\tilde\Phi|^2|\tilde\varphi|^2   .
\end{align}
```

The potential is composed of three different terms: the quartic potential of the inflaton, the quartic coupling between the inflaton and $\phi$, and the quartic coupling between the inflaton and $\varphi$. We label them as terms 0, 1, and 2 respectively. The different terms are implemented in the model file with the `potentialTerms` function as described in Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md). Scalars are given by the variable `fldS(X_c)` as before, with `X` the field label. Fields $\varphi$ and $\Phi$ are given instead by the variables `fldCS(X_c)` and `fldSU2Doublet(X_c)` respectively. Of course, the potential only depends on the moduli of these fields, which we can obtain with the `norm` function as `norm(fldCS(X_c))` and `norm(fldSU2Doublet(X_c))` respectively. The three terms of the potential are then specified as follows:
@emgithub(models/lphi4SU2U1.h:potential_terms)

We now need to specify the first derivatives of $\tilde{V}$ with respect $\tilde\phi$, $|\tilde\varphi|$, and $|\tilde\Phi|$. These must be specified in the functions `potDeriv(Tag<0>)`, `potDerivNormCS(Tag<0>)` and `potDerivNormSU2Doublet(Tag<0>)` respectively, with `Tag<X>` indicating the corresponding field label (there is only one copy for each species, so it is `Tag<0>` in the three cases). This is done as follows:
@emgithub(models/lphi4SU2U1.h:potential_derivs)

Finally, we need to specify the second derivatives of $\tilde{V}$ with respect $\tilde{\phi}$, $\tilde{\varphi}$, and $\tilde{\Phi}$. These are implemented in the functions `potDeriv2(Tag<X>)`, `potDeriv2NormCS(Tag<X>)` and `potDeriv2NormSU2Doublet(Tag<X>)` as follows:
@emgithub(models/lphi4SU2U1.h:potential_second_derivs)

### Output files

We indicate here the different output files:

-  `average_scalar_[nfld].txt`: $\tilde{ \eta}$, $\langle \tilde{\phi} \rangle$, $\langle \tilde{\phi}' \rangle$, $\langle \tilde{\phi}^2 \rangle$, $\langle \tilde{\phi}^{'2} \rangle$, $\text{rms} (\tilde{\phi})$, $\text{rms} (\tilde{\phi}')$

-  `average_norm_cmplx_scalar_[nfld].txt`:  $\tilde{ \eta}$, $\langle |\tilde{\varphi} |\rangle$, $\langle | \tilde{\varphi}' |\rangle$, $\langle |\tilde{\varphi} |^2 \rangle$, $\langle |\tilde{\varphi}'|^{2} \rangle$, $\text{rms} (|\tilde{\varphi}|)$, $\text{rms} (|\tilde{\varphi}'|)$

-  `average_[Re/Im]_cmplx_scalar_[nfld].txt`: $\tilde{ \eta}$, $\langle \tilde{\varphi}_n \rangle$, $\langle \tilde{\varphi}'_n \rangle$, $\langle \tilde{\varphi}_n^2 \rangle$, $\langle \tilde{\varphi}^{'2}_n \rangle$, $\text{rms} (\tilde{\varphi}_n)$, $\text{rms} (\tilde{\varphi}'_n)$

-  `average_norm_SU2Doublet_[nfld]\.txt`:  $\tilde{ \eta}$, $\langle |\tilde{\Phi} |\rangle$, $\langle | \tilde{\Phi}' |\rangle$, $\langle |\tilde{\Phi} |^2 \rangle$, $\langle |\tilde{\Phi}'|^{2} \rangle$, $\text{rms} (|\tilde{\Phi}|)$, $\text{rms} (|\tilde{\Phi}'|)$

-  `average_SU2Doublet_[nfld]_[n].txt`: $\tilde{ \eta}$, $\langle \tilde{\varphi}_n \rangle$, $\langle \tilde{\varphi}'_n \rangle$, $\langle \tilde{\varphi}_n^2 \rangle$, $\langle \tilde{\varphi}^{'2}_n \rangle$, $\text{rms} (\tilde{\varphi}_n)$, $\text{rms} (\tilde{\varphi}'_n)$

-  `average_norm_[U1]_[nfld].txt`: $\tilde{ \eta}$, $\langle {|\vec{\widetilde{ \mathcal E}}|} \rangle$, $\langle {|\vec{\widetilde{\mathcal B}}|} \rangle$, $\langle {|\vec{\widetilde{ \mathcal E}}|^2} \rangle$, $\langle {|\vec{\widetilde{\mathcal B}}|^2} \rangle$, $\text{rms} (|\vec{\widetilde{ \mathcal E}}|)$, $\text{rms} (|\vec{\widetilde{\mathcal B}}|)$

-  `average_norm_[SU2]_[nfld].txt`:

$\tilde{ \eta}$, $\sum_a \langle {|\vec{\widetilde{ \mathcal E^a}}|} \rangle$, $\sum_a \langle {|\vec{\widetilde{\mathcal B^a}}|} \rangle$, $\sum_a \langle {|\vec{\widetilde{ \mathcal E^a}}|^2} \rangle$, $\sum_a \langle {|\vec{\widetilde{\mathcal B^a}}|^2} \rangle$, $\sum_a \text{rms} (|\vec{\widetilde{ \mathcal E^a}}|)$, $\sum_a \text{rms} (|\vec{\widetilde{\mathcal B^a}}|)$

-  `average_energies.txt`:

$\tilde{\eta}$,
$\tilde{E}_K^{(\phi, 0)}$,
$\tilde{E}_G^{(\phi, 0)}$, ... ,
$\tilde{E}_K^{(\phi, N_s-1)}$,
$\tilde{E}_G^{(\phi, N_s-1)}$,
$\tilde{E}_K^{(\varphi, 0)}$,
$\tilde{E}_G^{(\varphi, 0)}$, ... ,
$\tilde{E}_K^{(\varphi, N_c-1)}$,
$\tilde{E}_G^{(\varphi, N_c-1)}$,

$\tilde{E}_K^{(\Phi, 0)}$,
$\tilde{E}_G^{(\Phi, 0)}$, ... ,
$\tilde{E}_K^{(\Phi, N_d-1)}$,
$\tilde{E}_G^{(\Phi, N_d-1)}$,
$\tilde{E}_K^{(A, 0)}$,
$\tilde{E}_G^{(A, 0)}$, ... ,
$\tilde{E}_K^{(A, N_{u1}-1)}$,
$\tilde{E}_G^{(A, N_{u1}-1)}$,

$\tilde{E}_K^{(B, 0)}$,
$\tilde{E}_G^{(B, 0)}$, ...,
$\tilde{E}_K^{(B, N_{s2}-1)}$,
$\tilde{E}_G^{(B, N_{s2}-1)}$,
$\tilde{E}_V^{(0)}$, ...,
$\tilde{E}_V^{(N_p-1)}$,
$\langle \tilde{\rho} \rangle$

-  `average_energy_conservation.txt`:

-  If no expansion: $\tilde{\eta}$, $1 - \frac{\langle \tilde{\rho} (\tilde{\eta} ) \rangle}{\langle \tilde{\rho} (\tilde{\eta}_*  ) \rangle}$

-  If self-consistent expansion: $\tilde{\eta}$, $\frac{\langle\text{LHS} - \text{RHS}\rangle}{\langle \text{LHS} + \text{RHS}\rangle}$, $\langle  \text{LHS} \rangle$, $\langle \text{RHS} \rangle$,

where LHS and RHS are the left and hand sides of Eq. ([*68*][eq_FriedmannHubble]).

-  `average_gauss_[U1/SU2]_[nfld].txt`: $\tilde{\eta}$, $\frac{\langle \sqrt{(\text{LHS} - \text{RHS})^2} \rangle}{\langle \sqrt{(\text{LHS} + \text{RHS})^2} \rangle}$,  $\langle \sqrt{(\text{LHS} - \text{RHS})^2} \rangle$ , $\langle \sqrt{(\text{LHS} + \text{RHS})^2} \rangle$.

where LHS and RHS are the left and hand sides of Eqs. ([*63*][eq_GaussU1-eom]) (for the U(1) sector) and Eqs. ([*64*][eq_GaussSU2-eom]) (for the SU(2) sector).

-  `average_scale_factor.txt`: $\tilde \eta$, $a$, $a'$, $a' \over a$

-  `spectra_scalar_[nfld].txt`:  $\tilde{k}$,  $\widetilde{\Delta}_{\tilde \phi} (\tilde k)$, $\widetilde{\Delta}_{\tilde \phi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{bin}$
-  `spectra_norm_cmplx_scalar_[nfld].txt`:
$\tilde{k}$,  $\widetilde{\Delta}_{\widetilde\varphi} (\tilde k)$, $\widetilde{\Delta}_{\widetilde\varphi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{bin}$
-  `spectra_norm_SU2Doublet_scalar_[nfld].txt`: $\tilde{k}$, $\widetilde{\Delta}_{\widetilde\Phi} (\tilde k)$, $\widetilde{\Delta}_{\widetilde\Phi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{bin}$
-  `spectra_norm_[U1/SU2]_[nfld].txt`:
$\tilde{k}$, $\widetilde{\Delta}_{\widetilde{\mathcal{E}}}(\tilde k)$
$\widetilde{\Delta}_{\widetilde{\mathcal{B}}}(\tilde k)$, $\Delta n_{bin}$

### The physics implemented in CosmoLattice

#### Initial conditions { #subsubsec_initialConditionsNonAb }

In Section [*Initialization of fluctuations*][sec_InitScalar] we summarized how the initial conditions for scalar singlets are imposed.
Here we explain now how we set the initial conditions to complex scalars and SU(2) doublets, as well as to the Abelian and non-Abelian gauge fields. We denote the time at which the initial conditions are imposed (i.e. the initial time of the simulation) as $t_*$, and all quantities with a $*$ subindex must be understood to be evaluated at that time: for example, $\varphi_* \equiv \varphi ({\bf x}, t_* )$ for complex scalars, and $\dot{\varphi}_* \equiv \dot{\varphi} ({\bf x}, t_* )$ for the doublets.

The initialization of the complex scalars and $SU(2)$ doublets is very similar to the scalar singlets: they consist in a homogeneous amplitude chosen by the user, over which a set of fluctuations is imposed. However, we must take into account that these fields have multiple components. As described above, in CosmoLattice the user can specify the initial absolute values $|\varphi_*|$ and $|\Phi_*|$ in the input file, and then decide how to distribute this power between the different components in the model file. However, as the scalar potential only depends on $|\varphi|$ and $|\Phi|$, we can always rotate the system so that all components have the same initial homogeneous amplitudes. Therefore, for the complex scalars we can impose
```math
\begin{align} \varphi_* &= \frac{1}{\sqrt{2}} (\varphi_{0*} + i \varphi_{1*} ) \hspace{0.4cm} \Longrightarrow \hspace{0.4cm}
\varphi_{n*} \equiv |\varphi_*|  +  \delta \varphi_{n*} ({ \bf x})   , \hspace{0.4cm} [n=0,1]  ,  \\
\dot{\varphi}_{*} &= \frac{1}{\sqrt{2}} (\dot{\varphi}_{0*} + i \dot{\varphi}_{1*} ) \hspace{0.4cm} \Longrightarrow \hspace{0.4cm}
\dot{\varphi}_{n*}  \equiv |\dot{\varphi}_*|  +  \delta \dot{\varphi}_{n*} ({ \bf x})   , \hspace{0.4cm} [n=0,1]  ,
\end{align}
```

where $\delta \phi_{n*} (\vec{x})$ and $\delta \dot{\phi}_{n*} (\vec{x})$ are the initial spectrum of fluctuations of the field components and their corresponding time-derivatives (we present the functional form of these functions below). Similarly, for the complex doublets we impose
```math
\begin{align} \Phi_*  &=
{1\over\sqrt{2}}
\left(
\begin{array}{c}
\varphi_{0*} +i\varphi_{1*} \\ \varphi_{2*} +i\varphi_{3*}
\end{array}
\right)  \hspace{0.4cm}  \Longrightarrow  \hspace{0.4cm}
\varphi_n ({\bf x}, t_* ) \equiv \frac{|\Phi_*|}{\sqrt{2}}  +  \delta \varphi_{n*} ({ \bf x})    , \hspace{0.4cm} [n=0,1,2,3]  , \\
\dot{\Phi}_*  &=
{1\over\sqrt{2}}
\left(
\begin{array}{c}
\dot \varphi_{0*} +i \dot \varphi_{1*} \\ \dot \varphi_{2*} +i \dot \varphi_{3*}
\end{array}
\right)  \hspace{0.4cm} \Longrightarrow  \hspace{0.4cm}
\dot \varphi_n ({\bf x}, t_* ) \equiv \frac{|\dot{\Phi}_*|}{\sqrt{2}}  +  \delta \dot{\varphi}_{n*} ({ \bf x})     , \hspace{0.4cm} [n=0,1,2,3]  .
\end{align}
```

On the other hand, for the Abelian and non-Abelian gauge field modes we impose
[](){ #eq_Inflc1 }
[](){ #eq_Inflc2 }
[](){ #eq_Inflc3 }
[](){ #eq_Inflc4 }
```math
\begin{align}
A_i ({\bf x}, t_* ) &= 0  , \tag{71}\\
B_i^a ({\bf x}, t_* ) &= 0  ,  \tag{72} \\
\dot{A}_i ({\bf x}, t_* ) &= \delta \dot{A}_{i*} ({\bf x})  ,  \tag{73} \\
\dot{B}_i^a ({\bf x}, t_* ) &= \delta \dot{B}_{i*}^a ({\bf x})  ,  \tag{74}
\end{align}
```

i.e. the initial amplitude of the gauge fields is set *exactly* to zero at all lattice points, while we only impose an initial spectrum of fluctuations to their time-derivatives (over vanishing homogeneous values). Due to this, the initial magnetic energy will be exactly zero, while a small amount of electric energy will be initially present due to the fluctuations of the time-derivatives of the gauge fields.

The initial spectrum of fluctuations for both the charged scalars and gauge fields must be imposed so that the Gauss constraint is verified initially. As long as this is true, the Gauss constraint will remain preserved during the entire dynamical evolution of the system. More specifically, let us Fourier transform the Gauss constraints ([*63*][eq_GaussU1-eom]) and ([*64*][eq_GaussSU2-eom]) at the initial time $t=t_*$. We get
[](){ #eq_kAi1 }
```math
{k}^i \widetilde{A}'_i ({\bf k}) = \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^A ({\bf k})  , \hspace{0.4cm} {k}^i \widetilde{B}_i^{a'} ({\bf k}) = \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^a ({\bf k})  , \tag{75}
```

where $J_0^A ({\bf k})$ and $J_0^a ({\bf k})$ are the Fourier transforms of each current. A solution of these equations is, for $\bf k \neq \bf 0$,
[](){ #eq_GaugeCurrentFluc }
```math
\widetilde{A}'_i ({\bf k}) = i \frac{{k}_i}{{k}^2} \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^A ({\bf k})  , \hspace{0.4cm} \widetilde{B}^{a'}_i ({\bf k}) = i \frac{{k}_i}{{k}^2} \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^a ({\bf k}) \tag{76}  .
```

The way in which we proceed to set fluctuations is the following. First,
we impose on the lattice the following fluctuations to the components of the charged scalars (in program units), mimicking the spectrum of fluctuations of the scalar singlets ([*47*][eq_fpr_influct])-([*48*][eq_fpr_influct2]):
[](){ #eq_fpr_influct3 }
[](){ #eq_fpr_influct4 }
```math
\begin{align}
\delta \tilde{\varphi}_{n*}({  \bf \tilde n}) &= \frac{1}{\sqrt{2}} \left(|\delta \tilde{\varphi}_{n1} ({  \bf \tilde n})|  e^{i \theta_{n1} ({  \bf \tilde n}) } + |\delta \tilde{\varphi}_{n2} ({  \bf \tilde n})| e^{i \theta_{n2} ({  \bf \tilde n}) }   \right) \tag{77}  , \\
\delta \tilde{\varphi}'_{n*} ({  \bf \tilde n}) &= a^{1-\alpha}\left[\frac{1}{\sqrt{2}} i \tilde{\omega}_{k,n} \left(|\delta \tilde{\varphi}_{n1} ({  \bf \tilde n})| e^{i \theta_{n1} ({  \bf \tilde n}) } - |\delta \tilde{\varphi}_{n2}  ({  \bf \tilde n})| e^{i \theta_{n2} ({  \bf \tilde n}) }  \right)\right]  - \tilde{\mathcal{H}} \delta \tilde{\varphi}_{n} ({  \bf \tilde n}) ,  \tag{78}
\end{align}
```

where $\tilde{\omega}_{k,n}  \equiv \omega_{k,n} /\omega_* =  \sqrt{\tilde{k}^2 + a^2 (\partial^2 \tilde{V} / \partial \tilde{\varphi}_n^2)}$ is the initial effective frequency of the modes of each field component in program units. Then these fluctuations generate fluctuations on the currents $\widetilde{J}_0^A ({\bf x})$, and $\widetilde{J}_0^a ({\bf x})$, which can be used to compute the corresponding fluctuations to the gauge fields in momentum space via Eqs. ([*76*][eq_GaugeCurrentFluc]). Finally, transforming back to position space we obtain $\delta \widetilde{A'}_{i*} ({\bf x})$, $\delta \widetilde{B}_{i*}^{a'} ({\bf x}) $.

However, in order for this procedure to work, we need to slightly modify the initialization of the charged field components ([*77*][eq_fpr_influct3])-([*78*][eq_fpr_influct4]) with respect to the prescription for scalar singlets. In the case of scalar singlets, we would have $\theta_1 ({\bf \tilde{n}})$ and $\theta_2 ({\bf \tilde{n}})$ as two random independent phases which vary uniformly in the range $[0, 2\pi)$ from point to point, while $|\delta \tilde{\phi}_1 ({\bf \tilde{n}})|$ and $|\delta \tilde{\phi}_2 ({\bf \tilde{n}})|$ would be two amplitudes that vary from point to point according to a *Rayleigh* distribution with expected square amplitude given by Eq. ([*49*][eq_QuantumFlucts2]). However, in the case of charged fields we need to set the homogeneous mode of the currents to zero, i.e. $J_0^A ({\bf k} ={\bf 0} ) = J_0^a ({\bf k} ={\bf 0} ) = 0$. As shown in Ref. [@Figueroa_2020rrl], this can be ensured if
[](){ #InConstr_1 }
[](){ #InConstr_2 }
```math
\begin{align}  |\delta \varphi_{n1} ({\bf k})| &= |\delta \varphi_{n2}  ({\bf k})|  , \hspace{3.5cm} n=0,1(,2,3)  , \tag{79} \\
\theta_{n2} ({\bf k}) &= \theta_{02} ({\bf k}) + \theta_{n1} ({\bf k}) - \theta_{01} ({\bf k})  , \hspace{0.85cm} n=1(,2,3)  . \tag{80}
\end{align}
```

In CosmoLattice, we then generate randomly only $\theta_{01}$, $\theta_{02}$, as well as $\theta_{n1}$, $\varphi_{n1}$ for $n=1(,2,3)$, and let the other functions be imposed via constraints ([*79*][InConstr_1])-([*80*][InConstr_2]).

#### Evolution equations

In Section [*Evolution of the system*][eq_evolution-sc] we wrote a Hamiltonian scheme for the equations of motion of a system of scalar singlets in an expanding universe. As explained, we conveniently defined a set of conjugate momenta $\{\pi_{\phi},b\}$ for the scalar field(s) $\phi$ and the scale factor $a$, in a manner that allowed us to write the field and Friedmann equations of motion as a set of four first-order differential equations. Thanks to our definitions, the kernels of the conjugate momenta do now depend on the time-derivatives of the corresponding fields, which then allow for solving the equations of motion (their discretized version) using algorithms such as staggered leapfrog or velocity verlet.

The same idea can be applied in our present case of a scalar-gauge theory that contains both scalar and gauge fields. In particular, we can define the following momenta for each of the five field species $\{\phi,\varphi,\Phi,A_i,B_i^a\}$ and for the scale factor $a(\eta)$, as
[](){ #eq_momU1singlet }
[](){ #eq_momSU2doublet }
[](){ #eq_momU1vec }
[](){ #eq_momSU2vec }
```math
\begin{align}
\tilde\pi_\phi &= a^{3-\alpha}\tilde\phi'   ,  \\
\tilde\pi_\varphi&=  a^{3-\alpha}\tilde\varphi'   ,   \tag{81}\\
\widetilde\pi_\Phi&= a^{3-\alpha}\widetilde\Phi'   ,    \tag{82}\\
\left(\tilde\pi_A\right)_i &= a^{1-\alpha}\widetilde F_{0i}   , \tag{83}  \\
\left(\tilde\pi_B\right)^{a}_i &= a^{1-\alpha}\widetilde G^a_{0i}   , \tag{84} \\
b &= a'   .
\end{align}
```

With these definitions the equations of motion can be then written as
[](){ #eq_kernelcomplexscalar }
[](){ #eq_kernelsSU21 }
```math
\begin{align}
(\tilde\pi_\phi)'    &=   \mathcal{K}_{\phi}[a,\tilde\phi,|\tilde{\varphi}|,|\widetilde{\Phi}|] & &   \equiv     - a^{3 + \alpha} \widetilde V_{,\tilde\phi}  + a^{1 + \alpha} {\widetilde \nabla}^{2} \tilde\phi    ,\\
(\tilde\pi_\varphi)'    &=   \mathcal{K}_{\varphi}[a,\tilde\phi,\tilde\varphi,|\widetilde{\Phi}|,\widetilde A_j]  & &   \equiv     a^{3 + \alpha} \widetilde V_{,|\tilde\varphi|} \frac{1}{2} \frac{\tilde\varphi}{|\tilde\varphi |} + a^{1 + \alpha} {\vec{\widetilde D}}_{\hspace{-0.5mm}A}^{ 2}\tilde\varphi    , \tag{85} \\
(\widetilde\pi_\Phi)'    &=   \mathcal{K}_{\Phi}[a,\tilde\phi,|\tilde\varphi|,\widetilde\Phi,\widetilde B_j^a] & &    \equiv     - a^{3 + \alpha} \widetilde V_{,|\widetilde\Phi|} \frac{1}{2} \frac{\widetilde\Phi}{|\widetilde\Phi |} + a^{1 + \alpha} {\vec{\widetilde{D}}}_{\hspace{-0.5mm}A}^{ 2}\widetilde\Phi  \tag{86}    ,\\
\left(\tilde\pi_A\right)'_i    &=   \mathcal{K}_{A_i}[a,\tilde\varphi,\widetilde \Phi,\widetilde A_j] & &   \equiv     a^{1+ \alpha}\widetilde J^A_i + a^{\alpha - 1}\tilde{\partial}_j \widetilde F_{ji}     ,   \\
\left(\left(\tilde\pi_B\right)_i^a\right)'   &=   \mathcal{K}_{B_i}Comp[a,\widetilde\Phi,\widetilde A_j,\widetilde B_j^a] & &    \equiv     a^{1+ \alpha}\widetilde J^a_i + a^{\alpha - 1}( \mathcal{\widetilde D}_j )_{a b} (\widetilde G_{ji} )^b      .
\end{align}
```

On the other hand, the equation for the evolution of the scale factor can be written as
```math
\begin{align}
\hspace{-1cm}
b' &= \mathcal{K}_a\hspace{-1mm}\left[a,{\widetilde E}_K^\phi,{\widetilde E}_K^\varphi,{\widetilde E}_K^\Phi,{\widetilde E}_G^\phi,{\widetilde E}_G^\varphi,{\widetilde E}_G^\Phi,{\widetilde E}_K^A,{\widetilde E}_G^A,{\widetilde E}_K^B,{\widetilde E}_G^B,{\widetilde E}_V\right]  \\
&= \frac{a^{2\alpha+1}}{3}{f_*^2\over m_p^2}\left[ (\alpha-2)({\widetilde E}_K^\phi  + {\widetilde E}_K^\varphi + {\widetilde E}_K^\Phi )  +
\alpha ({\widetilde E}_G^\phi  + {\widetilde E}_G^\varphi + {\widetilde E}_G^\Phi )+ (\alpha-1)
({\widetilde E}_K^A+{\widetilde E}_G^A + {\widetilde E}_K^B+{\widetilde E}_G^B) + (\alpha+1) {\widetilde E}_V  \right]  ,
\end{align}
```

where $\mathcal{K}_f$ with $f=\phi,\varphi,\Phi,A_i,B_i^a$ are the kernels for the different field species. As for singlet scalar theories, CosmoLattice provides already implemented two different evolution algorithms to solve these equations: staggered leapfrog (of accuracy 2) and velocity verlet (with accuracy of order 2, 4, 6, 8, and even 10). The details of how these algorithms work, specialized for Abelian and non-Abelian gauge theories, can be found in Sections 5 and 6 of Ref. [@Figueroa_2020rrl], respectively. On the lattice, the different kernels can be discretized using the toolkits of Section [*Lattice gauge invariant techniques*][subsec_LGT], or Section 3 of Ref. [@Figueroa_2020rrl]. The details of the lattice version EOM can they all found in Sections 5 (for Abelian gauge theories) and 6 (non-Abelian gauge theories) of Ref. [@Figueroa_2020rrl].

