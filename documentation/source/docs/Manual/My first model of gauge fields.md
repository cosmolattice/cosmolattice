$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is capable of simulating scalar-gauge field theories in an expanding universe. Note that we differentiate between *scalar-gauge interactions* and *ALP-gauge interactions*. While both contain gauge fields, the first case is based on charged scalars under gauge symmetries (as *e.g.* in scalar electrodynamics or the electroweak sector of the Standard Model). In the second case, a pseudo-scalar field which is not charged under a gauge symmetry, known as an *axion-like-particle* or ALP, is coupled to gauge fields via $\phi F\tilde F$ (Abelian) or $\phi G\tilde G$ (non-Abelian). In this section we explain how to simulate scalar-gauge theories, *i.e.* models containing charged scalar fields interacting with gauge fields, either Abelian or non-Abelian. For a description of ALP-gauge interactions in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, go to Section [Axion-Gauge Dynamics](ALP.md).

<!-- We start be reviewing briefly the framework of scalar-gauge theories in the continuum. -->

### **Program Variables** { #subsec_LatticeScalarGauge }

The action of a scalar-gauge theory with canonically normalized scalar fields, that can be simulated in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, is the following
[](){ #eq_Lagrangian }
```math
\begin{align}%\tag{50}
\label{eq_Lagrangian}
S & =- \int d^4 x \left\{\frac{1}{2}\partial_{\mu} \phi \partial ^{\mu}\phi + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} + V(\phi,|\varphi|, |\Phi|) \right\}  .  \\
\end{align}
```

This expression contains several types of scalar and gauge fields. In order to simplify notation, we have only added one copy of each field species, but $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can also handle multiple fields of the same kind, though see below the Note on the [*Number of fields in scalar-gauge theories*][NoteNumGaugeVariables]. The above action contains three types of scalar fields: a singlet $\phi$, a U(1)-charged scalar (complex field) $\varphi$, and a SU(2) doublet $\Phi$, where the latter two can be written in terms of real components $\varphi_j \in \mathcal{R}e$, as

<!-- [](){ #Table_Scalars } -->
```math
\begin{align} % \tag{3}
\begin{array}{c|c|c}
{\rm Singlet} & U(1){\rm-charged} & SU(2){\rm-charged Doublet}
\\\hline   &   &\\
\phi \in \mathcal{R}e &  \varphi \equiv {1\over\sqrt{2}}(\varphi_0 +i\varphi_1) & \Phi = \left(
\begin{array}{c}
\varphi^{(0)} \\ \varphi^{(1)}
\end{array}
\right) =
{1\over\sqrt{2}}
\left(
\begin{array}{c}
\varphi_0 +i\varphi_1 \\ \varphi_2 +i\varphi_3
\end{array}
\right)
\end{array}\label{Table_Scalars}
\end{align}
```

*c.f.* Eq. ([*3*][eq_ChargedScalars]) from [*Basic Field Equations in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*][subsec_BasicEOM] in Section [*Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*](Introduction to CosmoLattice.md). The complex field $\varphi$ can be charged under a U(1) gauge symmetry, while the doublet can be charged under both U(1) and SU(2). The scalar potential of this theory is $V = V(\phi, |\varphi|, |\Phi|)$, which depends on $\phi$, and/or on the moduli of the complex and doublet scalars, $|\varphi|, |\Phi|$. The corresponding covariant derivatives and field strengths in action $~$\eqref{eq_Lagrangian} are defined as
[](){ #eq_AbCovDerivCont }
[](){ #eq_CovDerivCont }
[](){ #eq_FmnAbelian }
[](){ #eq_GmnNonAb }
```math
\begin{align}\label{eq_AbCovDerivCont}
D_{\mu}^{\rm A} &=  \partial _{\mu} - i Q_A^{(\varphi)} g_{_A}A_\mu  ,%\tag{51}
\\
\label{eq_CovDerivCont}
D_{\mu} &=
\mathcal{I}D^{\rm A}_\mu
- i g_B Q_B B_{\mu}^a  T_a     =    \mathcal{I}\left( \partial _{\mu} - i Q_A^{(\Phi)} g_{_A}A_\mu \right)
- i g_B Q_B B_{\mu}^a  T_a  , %\tag{52}
\\
\label{eq_FmnAbelian}
F_{\mu \nu} &= \partial_{\mu}  A_{\nu} - \partial_{\nu} A_{\mu}  , %\tag{53}
\\
\label{eq_GmnNonAb}
G_{\mu \nu} &= \partial_{\mu} B_{\nu} - \partial_{\nu} B_{\mu} - i[B_\mu,B_\nu]  , 
\end{align}
```

where $Q_{A}^{(\varphi)}$ and $Q_{A}^{(\Phi)}$ are the Abelian charges of $\varphi$ and $\Phi$ respectively, $Q_B$ is the non-Abelian charge of $\Phi$, $g_A$ and $g_B$ are the corresponding gauge couplings, and $\mathcal{I}$ is the 2$\times$2 identity matrix. Using the properties of the $SU(N)$ generators $\lbrace T_a\rbrace$, $G_{\mu \nu}$ can be written as
[](){ #eq_Gmunu }
```math
\begin{align}
G_{\mu \nu} \equiv G_{\mu \nu}^a T_a   , \hspace{0.4cm} G_{\mu \nu}^a \equiv \partial_{\mu} B_{\nu}^a - \partial_{\nu} B_{\mu}^a + f^{a b c} B_{\mu}^b B_{\nu}^c  ,
\label{eq_Gmunu}
\end{align}
```

where $f_{abc}$ are the structure constants of the SU(N) group, determined by the relation $[T_a, T_b] = i f_{abc} T_c$. At the time of writing (July 2026), only the SU(2) non-Abelian group is implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, for which we simply have $T_a \equiv \sigma_a /2$, with $\sigma_a$ the Pauli matrices. In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we evolve the fields in the temporal gauge, so $A_{0} = B_0^a = 0$. Furthermore, we define the Abelian and non-Abelian electric and magnetic fields as follows,
[](){ #eq_ElectricMagneticDefs }
```math
\begin{equation}
\label{eq_ElectricMagneticDefs}
\mathcal{E}_i \equiv F_{0i},
\quad \mathcal{B}_i = \frac{1}{2} \epsilon_{i j k} F^{j k},
\quad \mathcal{E}_i^a \equiv G_{0i}^a,
\quad \mathcal{B}_i^a = \frac{1}{2} \epsilon_{i j k} G^{j k}_a,
\end{equation}
```

with $\epsilon_{ijk}$ the Levi-Civita symbol. **These expressions represent gauge-invariant physical quantites**.

[](){ #eq_GaugeProgramVar }
!!! note "**Program variables in Scalar-Gauge Theories**"
    As described in [*Program variables*][subsec_LatticeScalars] from Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md) on singlet-scalar theories, on the lattice we operate in a set of dimensionless spacetime and field variables called **program variables**. For scalar singlets, these were defined in <!-- Eq. [Eq. (29)](My first model of (singlet) scalar fields.md#eq_FieldSpaceTimeNaturalVariables) --> Eq. ([*2*][eq:FieldSpaceTimeNaturalVariables]) from Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md), in terms of the three constants $\{f_*,\omega_*,\alpha\}$, that must be judiciously chosen for each model. The same definitions hold for the scalar sector(s) of scalar-gauge theories, where we also introduce new dimensionless program variables for the gauge fields. Putting all program variables together, we have
    
    [](){ #eq_GaugeProgramVar }
    ```math
    
    \begin{align}
    d\tilde\eta \equiv a^{- \alpha} \omega_* dt , \hspace{0.4cm}
    d\tilde x^i \equiv \omega_* dx^i ,
    \hspace{0.4cm}
    \tilde\phi = \frac{\phi}{f_*}  , \hspace{0.4cm}
    \tilde\varphi = \frac{\varphi}{f_*}  , \hspace{0.4cm} \widetilde{\Phi} = \frac{\Phi}{f_*}  , \hspace{0.4cm}  \widetilde{A}_\mu=\frac{A_\mu }{\omega_*}  , \hspace{0.4cm} \widetilde B_{\mu}^a = \frac{B_{\mu}^a}{\omega_*}  . %\tag{56}
    \label{eq_GaugeProgramVar}
    \end{align}
    
    ```

[](){ #NoteNumGaugeVariables }
!!! note "Number of fields in scalar-gauge theories in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$"
    $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can run with an arbitrary number of scalar singlets, U(1) complex scalars, and SU(2) doublets. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ has been, however, only tested when considering a single U(1) gauge field. While, in principle, the code is prepared to work also with multiple U(1) gauge fields (coupled or not to scalars), this feature has not been thoroughly tested, so it is deactivated by default: the program crashes when a model is written with more than one U(1) gauge field. It can be re-activated at one own's risk by commenting out the following lines in  file $\texttt{include/CosmoInterface/abstractmodel.h}$

    @emgithub(include/CosmoInterface/abstractmodel.h:numb_U1_gauge_flds)

    **In the case of SU(2) gauge fields, however, only one of such fields can be considered at once in the code**. (<span style="color:red;">**Should we elimiate therefore the line "if constexpr (NSU2FLDS > 1)" in abstractmodel.h ?**</span>)
    

We can see that the U(1)- and SU(2)-charged scalars are re-scaled in the same way as singlet scalar fields. **The gauge fields, however, are instead re-scaled by the parameter $\omega_*$**. Correspondingly, we define program variables for the field strengths and covariant derivatives as follows:
[](){ #eq_FmnGmn }
```math
\begin{align}
\label{eq_FmnGmn}
\widetilde{F}_{\mu \nu} \equiv F_{\mu \nu} / \omega_*^2 , \hspace{0.4cm} \widetilde{G}_{\mu \nu}^a \equiv G_{\mu \nu}^a / \omega_*^2 , \hspace{0.4cm} \widetilde{D}_{\mu}^A \equiv D_{\mu}^A / \omega_* , \hspace{0.4cm} \widetilde{D}_{\mu} \equiv D_{\mu} / \omega_* .
\end{align}
```

The program potential is defined as before, like in the case of singlet scalars, as
[](){ #eq_ProgramPotMultiScalar }
```math
\begin{eqnarray}
%\tag{57}
\label{eq_ProgramPotMultiScalar}
\widetilde{V} (\tilde{\phi}, |\tilde{\varphi}|, |\widetilde{\Phi}|) \equiv \frac{1}{f_*^2 \omega_*^2} V(f_* \tilde \phi, f_* |\tilde \varphi|, f_* |\widetilde \Phi|  )  .
\end{eqnarray}
```

### **Equations of Motion**

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
\label{eq_singlet-eom}
\tilde \phi'' - a^{-2(1 - \alpha)} {\widetilde \nabla}^{ 2} \tilde \phi + (3 - \alpha)\frac{{a'}}{a} {\tilde  \phi'} &= - a^{2 \alpha} \widetilde V_{,\widetilde \phi}  , %\tag{58} \\
\\
\label{eq_higgsU1-eom}
\tilde \varphi'' - a^{-2(1 - \alpha)} {\widetilde{\vec D}}_{\hspace{-0.5mm}A}^{ 2}\tilde{\varphi} + (3 - \alpha)\frac{{a'}}{a}  {\tilde  \varphi'} &= - \frac{a^{2 \alpha}}{2} \widetilde V_{,|\widetilde \varphi|} \cdot \frac{\tilde  \varphi}{ |\tilde  \varphi |}   , %\tag{59}\\
\\
\label{eq_higgsSU2-eom}
\widetilde \Phi'' - a^{-2(1 - \alpha)} {\widetilde{\vec D}}^{ 2}\widetilde \Phi + (3 - \alpha)\frac{{a'}}{a}  {\widetilde \Phi'} &= - \frac{a^{2 \alpha}}{2} \widetilde V_{,|\widetilde \Phi|} \cdot \frac{\widetilde \Phi}{ |\widetilde \Phi |}  , %\tag{60}
\\
\label{eq_U1eom}
\tilde \partial_0 \widetilde F_{0i} - a^{-2(1 - \alpha )}\tilde  \partial_j \widetilde F_{ji} + (1 - \alpha) \frac{{a'}}{a} \widetilde F_{0i} &= \left( \frac{f_*}{\omega_*} \right)^2
a^{2 \alpha}\widetilde J^A_i  , %\tag{61}
\\
\label{eq_SU2eom}

(\widetilde{\mathcal{D}}_0 )_{a b} (\widetilde G_{0i})^b - a^{-2(1 - \alpha )} (  \widetilde{\mathcal{D}}_j )_{a b} (\widetilde{G}_{ji} )^b + (1 - \alpha) \frac{{a'}}{a} (\widetilde{G}_{0i} )^b &= \left( \frac{f_*}{\omega_*} \right)^2 a^{2 \alpha}(\widetilde{J}_i)_a  , %\tag{62}
\\
\label{eq_GaussU1-eom}
\tilde \partial_i \widetilde F_{0i} &= \left( \frac{f_*}{\omega_*} \right)^2 a^2 \widetilde J^A_0  , %\tag{63}\\
\\
\label{eq_GaussSU2-eom}
(\widetilde{\mathcal{D}}_i )_{a b} (\widetilde{G}_{0i})^b &= \left( \frac{f_*}{\omega_*} \right)^2 a^2(\widetilde{J}_0)_a  , %\tag{64}
\end{align}
```

where $(\widetilde{\mathcal{D}}_{\nu}O)_a = (\widetilde{\mathcal{D}}_{\nu})_{a b}O_b \equiv ( \delta_{a b}  \tilde \partial_{\nu} - f_{abc} \tilde B_{\nu}^c ) O_b$, and the scalar-charge currents are given by
[](){ #eq_AbelianCurrent }
[](){ #eq_NonAbelianCurrent }
```math
\begin{align}
%\tag{65}
\label{eq_AbelianCurrent}
\hspace{1.8cm} \widetilde J_A^\mu &= 2 g_AQ_A^{(\varphi)} \mathcal{I}m [ \tilde \varphi^{*} ( \widetilde{D}_A^{\mu} \tilde \varphi )] + 2 g_AQ_A^{(\Phi)} \mathcal{I}m [ \widetilde \Phi^\dagger (\widetilde D^{\mu} \widetilde \Phi  )] ,%\\\tag{66}
\\
\label{eq_NonAbelianCurrent}
\hspace{1.8cm} \widetilde J_a^\mu &= 2g_BQ_B\mathcal{I}m [ \widetilde \Phi^{\dagger} T_a( \widetilde{D}^{\mu} \widetilde \Phi )] .
\end{align}
```

Similarly, we define the *program pressure* and *energy density* as
[](){ #eq_pressureSG }
[](){ #eq_rhoSG }
```math
\begin{align}
\label{eq_pressureSG} 
\tilde{p} \equiv \frac{p}{f_*^2 \omega_*^2} \hspace{-0.2cm} &= \hspace{-0.2cm} \tilde{K}_{\phi} + \tilde{K}_{\varphi} + \tilde{K}_{\Phi} -{1\over3}(\tilde{G}_{\phi} + \tilde{G}_{\varphi} + \tilde{G}_{\Phi}) + {1\over3}  (\tilde{K}_{U(1)} + \tilde{G}_{U(1)} + \tilde{K}_{SU(2)} + \tilde{G}_{SU(2)} ) - \tilde{V}  ,
\\
\label{eq_rhoSG}
\tilde{\rho} \equiv \frac{\rho}{f_*^2 \omega_*^2}  \hspace{-0.2cm} &= \hspace{-0.2cm} \tilde{K}_{\phi} + \tilde{K}_{\varphi} + \tilde{K}_{\Phi} + \tilde{G}_{\phi} + \tilde{G}_{\varphi} + \tilde{G}_{\Phi} +  \tilde{K}_{U(1)} + \tilde{G}_{U(1)} + \tilde{K}_{SU(2)} + \tilde{G}_{SU(2)} + \tilde{V}  , 
\end{align}
```

where each of the individual kinetic, gradient, and potential energy contributions are
[](){ #eq_energy-contrib }
```math
\begin{align}
\label{eq_energy-contrib}
\hspace{-1cm}\left\lbrace
\begin{array}{rcl} %\tag{67}
\widetilde {K}_{\phi} &=& \frac{1}{2 a^{2\alpha} } \tilde \phi'^2 \\
\widetilde {K}_{\varphi} &=& \frac{1}{a^{2\alpha} } (\widetilde D_0^A \widetilde  \varphi)^*(\widetilde D_0^A \widetilde \varphi)
\\
\widetilde {K}_{\Phi} &=& \frac{1}{a^{2\alpha} } (\widetilde D_0 \widetilde \Phi )^\dagger(\widetilde D_0 \widetilde \Phi)
\\
\end{array}\right.
\hspace{0.1cm};\hspace{0.75cm}
\left\lbrace
\begin{array}{rcl}
\widetilde {G}_{\phi} &=& \frac{1}{2 a^2} \sum_i (\tilde \partial_i \tilde \phi)^2
\\
\widetilde {G}_{\varphi} &=& \frac{1}{a^2} \sum_i (\widetilde D_i^A \varphi)^*(\widetilde D_i^A \widetilde \varphi)
\\
\widetilde {G}_{\Phi} &=& \frac{1}{a^2} \sum_i (\widetilde  D_i\widetilde  \Phi)^\dagger(\widetilde D_i \widetilde \Phi)
\\
\end{array}\right.
\hspace{0.1cm};\hspace{0.75cm}
\left\lbrace
\begin{array}{rcl}
\widetilde {K}_{U(1)} &=& \frac{1}{2 a^{2 + 2 \alpha}} ( {\omega_* \over f_*} )^2  \sum_{i} \widetilde  F_{0i}^2
\\
\widetilde {K}_{SU(2)} &=& \frac{1}{2 a^{2 + 2 \alpha}} ( {\omega_* \over f_*} )^2 \sum_{a,i} (\widetilde G_{0i}^a)^2
\\
\widetilde {G}_{U(1)} &=& \frac{1}{2 a^4} ( {\omega_* \over f_*} )^2 \sum_{i,j < i} \widetilde F_{ij}^2
\\
\widetilde {G}_{SU(2)} &=& \frac{1}{2 a^4} ( {\omega_* \over f_*} )^2 \sum_{a,i,j < i}  (\widetilde G_{ij}^a)^2    . \\
\end{array}\right.
\\\nonumber\\
\text{(Kinetic-Scalar)} \hspace{5cm} \text{(Gradient-Scalar)} \hspace{6.5cm} \text{(Electric & Magnetic)} \hspace{3.0cm}\nonumber
\end{align}
```

If the expansion of the Universe is self-consistent, i.e. it is sourced by the homogeneous (volume averaged) pressure and energy densities of the simulated fields, the scale factor evolution is dictated by the Friedmann equations
[](){ #eq_FriedmannHubble }
[](){ #eq_FriedmannDDa }
```math
\begin{align}%\tag{68}
\label{eq_FriedmannHubble}
\mathcal{H}^2    \equiv    \frac{a'^{ 2}}{a^2} &=  \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ \widetilde E_K^{\phi} + \widetilde E_K^{\varphi} + \widetilde E_K^{\Phi} + \widetilde E_G^{\phi} + \widetilde E_G^{\varphi} + \widetilde E_G^{\Phi} + \widetilde E_K^A + \widetilde E_K^B + \widetilde E_G^A + \widetilde E_G^B + \widetilde E_V \right]  ,
\\
%\tag{69}
\label{eq_FriedmannDDa}
{a''\over a} &= \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ (\alpha-2)(\widetilde E_K^{\phi} + \widetilde E_K^{\varphi} + \widetilde E_K^{\Phi}) + \alpha(\widetilde E_G^{\phi} + \widetilde E_G^{\varphi} + \widetilde E_G^{\Phi}) + (\alpha + 1)\widetilde E_V \right.\\
& \hspace{8.1cm}\left. +  (\alpha-1)(\widetilde E_K^A + \widetilde E_K^B + \widetilde E_G^A + \widetilde E_G^B) \right]  ,\nonumber
\end{align}
```

where we have defined the volume-averaged energy contributions as: $E_{K}^{f} = \langle \widetilde{K}_{f} \rangle$ and $E_{G}^{f} = \langle \widetilde{G}_{f} \rangle$ for the scalar fields $f=\phi,\varphi,\Phi$; $\widetilde E_{K}^{A} = \langle \widetilde{K}_{U(1)} \rangle$, $\widetilde E_{G}^{A} = \langle \tilde{G}_{U(1)} \rangle$, $\widetilde E_{K}^{B} = \langle \widetilde{K}_{SU(2)} \rangle$, and $\widetilde E_{G}^{B} = \langle \widetilde{G}_{SU(2)} \rangle$ for the gauge fields, and $\widetilde{E}_V = \langle \widetilde{V} \rangle$ for the potential energy. Instead, if the expansion is sourced by an external energetically-dominant fluid with constant equation of state $w$, $a(\eta)$ is given by the power-law function, *c.f.* Eq. ([*11*][eq_ScaleFactorPowerLaw]) from Section [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md).

[](){ #NoteNumGaugeVariables }
!!! note "Important Note"
    The evolution algorithms implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ use a discretized versions of Eq.$~$\eqref{eq_singlet-eom}-\eqref{eq_SU2eom} to solve for the field dynamics, and a lattice version of Eq.$~$\eqref{eq_FriedmannDDa} to solve for the scale factor. Eq.$~$\eqref{eq_GaussU1-eom} and Eq.$~$\eqref{eq_GaussSU2-eom} are the Gauss constraints of the U(1) and SU(2) gauge sectors respectively, which must be satisfied (by their lattice version counterparts) during all times during the simulation. Analogously, Eq.$~$\eqref{eq_FriedmannHubble}, which represents the Hubble constraint, must also be satisfied (again by its lattice analogue) all throughout the simulation. The degree of satisfaction of Eq.$~$\eqref{eq_FriedmannHubble} is an indicator of the ability of a given integrator to conserve energy. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ monitors the degree of conservation of both Gauss and Hubble constraints, providing in this manner a quantitative validation of the numerical integration of the EOM. Our discretization techniques guarantee that the Gauss constraints are obeyed up to machine precision, see $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl] for details. On the other hand, the Hubble constraint Eq.$~$\eqref{eq_FriedmannHubble} holds numerically only to a certain degree of approximation, possibly reaching down to machine precision (depending on the model) only in the case of the highest order integrators like `VV10` or `PV10`.



### **Model and input files** { #subsec_ScalarGaugeInput }

We explain now how to implement a model with Abelian and non-Abelian gauge symmetries in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. Two gauge models are already implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$: the model $\texttt{lphi4U1}$, which includes a complex scalar charged under a U(1) gauge symmetry and one Abelian gauge field, *i.e.* it describes **scalar-electrodynamics**; the model $\texttt{lphi4SU2U1}$, which contains a scalar field charged under $U(1)\times SU(2)$, one Abelian gauge field, and one non-Abelian SU(2) gauge field, *i.e.* it describes **the electroweak (bosonic) sector of the Standard Model**. These are models ready-to-use as templates for your own models. You cna find their $\texttt{.h}$ implementations in the folder $\texttt{./models}$. In this manual, we will use as a reference example the model $\texttt{lphi4SU2U1}$, as it contains all possible field species and interactions in scalar-gauge theories that can be currently simulated with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

The model $\texttt{lphi4SU2U1}$ consists of a doublet $\Phi$ charged under a $SU(2)\times U(1)$ gauge group, coupled to one Abelian gauge field $A_{\mu}$ and one non-Abelian one $B_{\mu}^a$, via the previously defined covariant derivative. We also couple $\Phi$ to a scalar singlet $\phi$ and to a U(1)-charged scalar field $\varphi$ via quadratic interactions in the scalar potential. We consider a scenario in which the doublet $\Phi$ acts as the dominant mother field, and we simulate its non-perturbative decay into gauge fields and other scalars, as induced through parametric resonance effects due to the coherent oscillations of $\Phi$. One could think of this situation as a preheating scenario where $\Phi$ plays the role of the inflaton field, coupled to the daughter fields $\phi, \varphi, A_\mu$ and $B_\mu = T_aB_\mu^a$. In particular, we will implement the theory described by action Eq.$~$\eqref{eq_Lagrangian} with the following scalar potential,
[](){ #eq_PotSingletComplexDoublet }
```math
\begin{eqnarray}
\label{eq_PotSingletComplexDoublet}
V(\phi,|\varphi|,|\Phi |) = \lambda |\Phi|^4  + g^2 |\Phi |^2 \phi^2 + 2 h^2 |\Phi |^2 |\varphi|^2  ,
 %\tag{70}
 \end{eqnarray}
```

where $\lambda$, $g$, and $h$ are dimensionless coupling constants. The first term is the inflaton potential, and the second and third terms are quadratic interactions between the inflaton and $\phi$ and $\varphi$, respectively. As said, the theory contains one field of each kind. We will assume that at the onset of the simulation (say at the end of slow-roll inflation), the inflaton amplitude has an initial non-zero homogeneous component with modulus $|\Phi| = |\bar{\Phi}_*|$, while the homogeneous components of the rest of scalar and gauge fields are set to zero (this is natural as they are massive during inflation, with their mass induced by the large amplitude of $\Phi$, and hence, initially, they are not excited). The post-inflationary oscillations of $\Phi$ will trigger a resonant growth of $\phi$ and $\varphi$ due to resonant effects via the second and third terms of the potential, and also a parametric excitation of the gauge fields $A_{\mu}$ and $B_{\mu}^a$ due to their coupling to $\Phi$ encoded in the covariant derivative $g_A^2 |\Phi|^2 A_{\mu}A^{\mu}$, $g_B^2 |\Phi|^2 B_{\mu}^{a}B^{\mu}_{a} \in (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi)$.

This scenario is implemented in the model file $\texttt{models/lphi4SU2U1.h}$, and the corresponding input parameter file is in $\texttt{models/parameter-files/lphi4SU2U1.in}$. Most of the parameters defined in the input file are the same as for singlet scalar fields, see [*Running the program with an input parameter file*][subsec_Input-Scalars] in Section [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md). However, there are several extra parameters that need to be set now in $\texttt{models/parameter-files/lphi4SU2U1.in}$. First, let us specify the initial homogeneous components of all scalar fields, as follows:

@emgithub(models/parameter-files/lphi4SU2U1.in:initial_conditions)

Above, `initial_amplitudes` and `initial_momenta` represent the initial homogenous amplitudes of the scalar singlet, $\phi_*$ and $\dot{\phi}_*$. If there was more than one singlet, their initial conditions would be specified in a vector form, as explained in [*Running the program with an input parameter file*][subsec_Input-Scalars] in the [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md) Section. Parameters `cmplx_field_initial_norm` and `cmplx_momentum_initial_norm` represent the **initial absolute values** of the complex field amplitude and its time-derivative, *i.e.* $|\varphi_*| \equiv \sqrt{(\varphi_{0*}^2 + \varphi_{1*}^2)/2}$ and $|\dot{\varphi}_*| \equiv \sqrt{(\dot{\varphi}_{0*}^2 + \dot{\varphi}_{1*}^2)/2}$, respectively (set to zero in this example). Similarly, the parameters `SU2Doublet_initial_norm`,  `SU2Doublet_initial_momenta_norm` specify $|\Phi_*| \equiv \sqrt{ \sum_{n=0}^3 \varphi_{n*}^2 / 2}$, and $|\dot{\Phi}_*| \equiv \sqrt{ \sum_{n=0}^3 \dot{\varphi}_{n*}^2 / 2 }$, respectively. **As before, initial amplitudes must be introduced in $\text{GeV}$, and initial derivatives in $\text{GeV}^2$**.


We also need to specify the parameters that deal with the gauge couplings and charges in the covariant derivatives. This is done as follows:

@emgithub(models/parameter-files/lphi4SU2U1.in:gauge_couplings)

Here, `gU1s` fixes $g_A$, `gSU2s` fixes $g_B$, `CSU1Charges` fixes $Q_A^{(\varphi)}$, `SU2DoubletU1Charges` fixes $Q_A^{(\Phi)}$, and `SU2DoubletSU2Charges` fixes $Q_B$. Although in our example we only consider one field for each species, multiple couplings and charges can also be specified in vector form.

Finally, we also want to specify the model parameters that appear in the potential Eq.$~$\eqref{eq_PotSingletComplexDoublet}. This is done as follows,

@emgithub(models/parameter-files/lphi4SU2U1.in:model_parameters)
where we have defined three parameters: `lambda` (which represents $\lambda$), and `qG` and `qH` (representing $q_G \equiv g^2 / \lambda$ and $q_H \equiv h^2 / \lambda$, respectively).

Let us now analyze the model file $\texttt{models/lphi4-SU2U1.h}$, which can be used as a template to simulate different gauge field theories. We start by specifying the field content of our theory:
@emgithub(models/lphi4SU2U1.h:model_pars)

In first five lines inside the structure `ModelPars`, <!-- below `struct ModelPars : public TempLat::DefaultModelPars {`,  --> we have specified the number of fields of each species: `NScalars` refers to $\phi$, `NCScalars` refer to $\varphi$, `NU1Flds` refer to $A_{\mu}$, `NSU2Doublet` refers to $B_{\mu}^a$, and `NSU2Flds` refers to $\Phi$. In the following line we specify the number of terms `NPotTerms` in the potential, which is $3$ in our case. Finally, in lines starting by `typedef ...`, we define three types of *coupling managers*, which deal with the couplings between the scalar and gauge fields in the covariant derivatives: `U1CsCouplings` must be defined if $\varphi$ couples to $A_{\mu}$, `U1SU2DoubletCouplings` must be defined if $\Phi$ couples to $A_{\mu}$, and `SU2SU2DoubletCouplings` must be defined if $\Phi$ couples to $B_{\mu}^a$. Finally, in the last line, we specify the name of our model as `lphi4SU2U1`, in agreement with the name of the header file.

After that, inside the template model, we declare several model parameters (`g`, `h`, `lambda`, `qG`, `qH`) as private variables:
@emgithub(models/lphi4SU2U1.h:class_declaration)

We then use the parser to read the initial homogeneous components of the scalar field amplitudes and derivatives as follows:
@emgithub(models/lphi4SU2U1.h:constructor_initial_conditions)

We see that for the scalar singlet $\phi$, the initial amplitude and derivative are read by the parser from the input file in the lines that begin as `fldS0(0_c) = ...` and `piS0(0_c) = ...`, respectively, as described in Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md). As explained there in [*Running the program with an input parameter file*][subsec_Input-Scalars], the singlet initial values values must be stored in the variables `fldS0(0_c)` and `piS0(0_c)`, respectively, where `0_c` denotes the label of the field (since there is only one singlet in our example, then only the amplitudes of `0_c` are initiallized). Regarding $\varphi$, in lines beginning by ` double normCmplx0 = ...` and `double normPiCmplx0 = ...`, we read the initial values of $|\varphi_*|$ and $|\dot{\varphi}_*|$. For convenience, we stored them in the local variables `normCmplx0` and `normPiCmplx0` respectively. Then, in lines beggining by `fldCS0(0_c) = ...` and `piCS0(0_c) = ...` we specify how these amplitudes are distributed between the two components of $\varphi$, *i.e.* its real and imaginary parts, $\varphi_0$ and $\varphi_1$ [*c.f.* Eq. ([*3*][eq_ChargedScalars]) for notation]. In our example, this is done with the `Complexify` function, which creates a two-component vector storing the real and imaginary parts of a complex number. For consistency, one must always guarantee that the initial components satisfy $\sqrt{(\varphi_{0*}^2 + \varphi_{1*}^2) /2} = |\varphi_*|$, as well as $\sqrt{(\dot{\varphi}_{0*}^2 + \dot{\varphi}_{1*}^2) /2} = |\dot{\varphi}_*|$. In our example, we have decided to give the same initial amplitude to all components, so that $\varphi_{0*} = |\varphi_*|$ and $\varphi_{1*} = |\varphi_*|$, whereas $\dot{\varphi}_{0*} = |\dot{\varphi}_*|$ and $\dot{\varphi}_{1*} = |\dot{\varphi}_*|$. In any case, the created vectors must be stored in the model variables `fldCS0(0_c)` (for the amplitude) and `piCS0(0_c)` (for the time-derivative), where again `0_c` is the field label.

We use a similar technique in the remaining lines above, where we specify the initial conditions of $\Phi_*$ and $\dot{\Phi}_*$. First, in the lines beginning by `double normDoublet0 = ...` and `double normPiDoublet0 = ...`, we store the values of $|\Phi_*|$ and $|\dot{\Phi}_*|$, specified in the input file in the local variables `normDoublet0` and `normPiDoublet0`, respectively. We then need to indicate how these are distributed between the four components of the doublets $\Phi_*$ and $\dot{\Phi}_*$, i.e. $\varphi_{n*}$ and $\dot{\varphi}_{n*}$ for $n=0,1,2,3$ [see Eq. ([*3*][eq_ChargedScalars])]. For consistency, we must always ensure that $\sqrt{ \sum_{n=0}^3 \varphi_{n*}^2 / 2} = |\Phi_*|$ and $\sqrt{ \sum_{n=0}^3 \dot{\varphi}_{n*}^2 / 2 } = |\dot{\Phi}_*|$. In our example, this is done in the lines beginning by `fldSU2Doublet0(0_c) = ...` and `piSU2Doublet0(0_c) = ...`, with the `MakeSU2Doublet` function creating a SU(2) doublet with equal amplitude for all the components (in this case, $\varphi_{n*} = |{\Phi}_*| /\sqrt{2}$ and $\dot{\varphi}_{n*} = |\dot{\Phi}_*| / \sqrt{2}$. Finally, the corresponding initial SU(2) doublets must be stored in the variables `fldSU2Doublet0(0_c)` and `piSU2Doublet0(0_c)`, with `0_c` again denoting the field label.

We now proceed to read the model parameters `qG`, `qH`, and `lambda` from the input file in the usual way, and to compute new parameters `g` and `h`, as follows
@emgithub(models/lphi4SU2U1.h:model_parameters)

The next step is to define appropriate program variables for the model, as well as setting the initial masses. The potential of the dominating oscillatory field is quartic, similar to the case considered in the section about singlet scalars, see Eq. ([*12*][eq_lphi4-ProgVar]) from Section [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md). Ee thus choose them as
[](){ #eq_ProgVariabScalarGauge }
```math
\begin{align}
\label{eq_ProgVariabScalarGauge}
f_*=|\overline{\Phi}_{*} | ,~~  \omega_*=\sqrt{\lambda} | \overline{\Phi}_* |,~~  \alpha=1  .
\end{align}
```

This is done in the code as follows,
@emgithub(models/lphi4SU2U1.h:rescaling)

Finally, we call the generic function responsible to set the masses of the matter fields together with the initial potential
@emgithub(models/lphi4SU2U1.h:masses_setup)

We now need to specify the scalar potential of our field theory. As for scalar singlet interactions, a scalar-gauge theory in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ requires a *program potential*, defined in Eq.$~$\eqref{eq_ProgramPotMultiScalar}. In our example, it is given by
<!-- [](){ #eq_Pot_exampleSU2U1 } -->
```math
\begin{align}
\widetilde V( \tilde\phi, |\tilde\varphi|, |\tilde\Phi| ) \equiv \frac{1}{f_*^2 \omega_*^2}V(f_*\tilde \phi,f_*|\tilde \varphi |, f_*|\tilde \Phi |) = |\tilde\Phi|^4 + \frac{g^2}{\lambda}|\tilde\Phi|^2\tilde\phi^2 + 2 \frac{h^2}{\lambda}|\tilde\Phi|^2|\tilde\varphi|^2   .
\label{eq_Pot_exampleSU2U1}
\end{align}
```

The potential above is composed of three different terms: the quartic potential of the inflaton, the quadratic coupling between the inflaton $\Phi$ and $\phi$, and the quadratic coupling between the inflaton and $\varphi$. We label them as terms 0, 1, and 2, respectively. The different terms are implemented in the model file with the `potentialTerms` function, similarly as we described it in the Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md). Singlet scalars are denoted by the variable `fldS(X_c)` as before, with `X_c` the field label. Charged scalars $\varphi$ and $\Phi$ are set, instead, by the variables `fldCS(X_c)` and `fldSU2Doublet(X_c)`, respectively. Due to gauge invariance, the potential only depends on the moduli of these fields, which we obtain with the `norm` function as `norm(fldCS(X_c))` and `norm(fldSU2Doublet(X_c))`, respectively. The three terms of the potential are then specified as follows:
@emgithub(models/lphi4SU2U1.h:potential_terms)

We now need to specify the first derivatives of $\tilde{V}$ with respect $\tilde\phi$, $|\tilde\varphi|$, and $|\tilde\Phi|$. These must be specified in the functions `potDeriv(Tag<0>)`, `potDerivNormCS(Tag<0>)` and `potDerivNormSU2Doublet(Tag<0>)`, respectively, with `Tag<X>` indicating the corresponding field label (in our example there is only one copy for each species, so it is `Tag<0>` in the three cases). This is done as follows:
@emgithub(models/lphi4SU2U1.h:potential_derivs)

Finally, we need to specify the second derivatives of $\tilde{V}$ with respect to $\tilde{\phi}$, $\tilde{\varphi}$, and $\tilde{\Phi}$. These are implemented in the functions `potDeriv2(Tag<X>)`, `potDeriv2NormCS(Tag<X>)` and `potDeriv2NormSU2Doublet(Tag<X>)`, as follows:
@emgithub(models/lphi4SU2U1.h:potential_second_derivs)

### **Output files**

The code generates two different kinds of output files, classified according to the information they contain:

<div class="grid cards cl-outkinds" markdown>

-   **Averages**

    Volume-averages of field quantities (e.g. mean amplitude, variance), or other quantities that are independent of the lattice site (e.g. scale factor). Their printing frequency is controlled by the parameter `tOutputFreq`.

-   **Spectra**

    Binned spectra of fields and other quantities in momentum space. Their printing frequency is controlled by the parameter `tOutputInfreq`. Their computation is generally more time-consuming than averages, as they imply Fourier transforming the whole lattice forth and back.

</div>

We indicate here the different output files generated by the simulation and their column content:

<div class="grid cards cl-files" markdown>

-   `average_scalar_[nfld].txt`{ .cl-fname }

    $\tilde{ \eta}$, $\langle \tilde{\phi} \rangle$, $\langle \tilde{\phi}' \rangle$, $\langle \tilde{\phi}^2 \rangle$, $\langle \tilde{\phi}^{'2} \rangle$, $\text{rms} (\tilde{\phi})$, $\text{rms} (\tilde{\phi}')$
    {: .cl-schema }

-   `average_norm_cmplx_scalar_[nfld].txt`{ .cl-fname }

    $\tilde{ \eta}$, $\langle |\tilde{\varphi} |\rangle$, $\langle | \tilde{\varphi}' |\rangle$, $\langle |\tilde{\varphi} |^2 \rangle$, $\langle |\tilde{\varphi}'|^{2} \rangle$, $\text{rms} (|\tilde{\varphi}|)$, $\text{rms} (|\tilde{\varphi}'|)$
    {: .cl-schema }

-   `average_[Re/Im]_cmplx_scalar_[nfld].txt`{ .cl-fname }

    $\tilde{ \eta}$, $\langle \tilde{\varphi}_n \rangle$, $\langle \tilde{\varphi}'_n \rangle$, $\langle \tilde{\varphi}_n^2 \rangle$, $\langle \tilde{\varphi}^{'2}_n \rangle$, $\text{rms} (\tilde{\varphi}_n)$, $\text{rms} (\tilde{\varphi}'_n)$
    {: .cl-schema }

-   `average_norm_SU2Doublet_[nfld].txt`{ .cl-fname }

    $\tilde{ \eta}$, $\langle |\tilde{\Phi} |\rangle$, $\langle | \tilde{\Phi}' |\rangle$, $\langle |\tilde{\Phi} |^2 \rangle$, $\langle |\tilde{\Phi}'|^{2} \rangle$, $\text{rms} (|\tilde{\Phi}|)$, $\text{rms} (|\tilde{\Phi}'|)$
    {: .cl-schema }

-   `average_SU2Doublet_[nfld]_[n].txt`{ .cl-fname }

    $\tilde{ \eta}$, $\langle \tilde{\varphi}_n \rangle$, $\langle \tilde{\varphi}'_n \rangle$, $\langle \tilde{\varphi}_n^2 \rangle$, $\langle \tilde{\varphi}^{'2}_n \rangle$, $\text{rms} (\tilde{\varphi}_n)$, $\text{rms} (\tilde{\varphi}'_n)$
    {: .cl-schema }

-   `average_norm_U1_[nfld].txt`{ .cl-fname }

    $\tilde{ \eta}$, $\langle {|\vec{\widetilde{ \mathcal E}}|} \rangle$, $\langle {|\vec{\widetilde{\mathcal B}}|} \rangle$, $\langle {|\vec{\widetilde{ \mathcal E}}|^2} \rangle$, $\langle {|\vec{\widetilde{\mathcal B}}|^2} \rangle$, $\text{rms} (|\vec{\widetilde{ \mathcal E}}|)$, $\text{rms} (|\vec{\widetilde{\mathcal B}}|)$
    {: .cl-schema }

-   `average_norm_SU2_[nfld].txt`{ .cl-fname }

    $\tilde{ \eta}$, $\sum_a \langle {|\vec{\widetilde{ \mathcal E^a}}|} \rangle$, $\sum_a \langle {|\vec{\widetilde{\mathcal B^a}}|} \rangle$, $\sum_a \langle {|\vec{\widetilde{ \mathcal E^a}}|^2} \rangle$, $\sum_a \langle {|\vec{\widetilde{\mathcal B^a}}|^2} \rangle$, $\sum_a \text{rms} (|\vec{\widetilde{ \mathcal E^a}}|)$, $\sum_a \text{rms} (|\vec{\widetilde{\mathcal B^a}}|)$
    {: .cl-schema }

-   `average_energies.txt`{ .cl-fname }

    $\tilde{\eta}$, $\tilde{E}_K^{(\phi, 0)}$, $\tilde{E}_G^{(\phi, 0)}$, $\ldots$, $\tilde{E}_K^{(\phi, N_s-1)}$, $\tilde{E}_G^{(\phi, N_s-1)}$, $\tilde{E}_K^{(\varphi, 0)}$, $\tilde{E}_G^{(\varphi, 0)}$, $\ldots$, $\tilde{E}_K^{(\varphi, N_c-1)}$, $\tilde{E}_G^{(\varphi, N_c-1)}$, $\tilde{E}_K^{(\Phi, 0)}$, $\tilde{E}_G^{(\Phi, 0)}$, $\ldots$, $\tilde{E}_K^{(\Phi, N_d-1)}$, $\tilde{E}_G^{(\Phi, N_d-1)}$, $\tilde{E}_K^{(A, 0)}$, $\tilde{E}_G^{(A, 0)}$, $\ldots$, $\tilde{E}_K^{(A, N_{u1}-1)}$, $\tilde{E}_G^{(A, N_{u1}-1)}$, $\tilde{E}_K^{(B, 0)}$, $\tilde{E}_G^{(B, 0)}$, $\ldots$, $\tilde{E}_K^{(B, N_{s2}-1)}$, $\tilde{E}_G^{(B, N_{s2}-1)}$, $\tilde{E}_V^{(0)}$, $\ldots$, $\tilde{E}_V^{(N_p-1)}$, $\langle \tilde{\rho} \rangle$
    {: .cl-schema }

-   `average_energy_conservation.txt`{ .cl-fname }

    If no expansion:

    $\tilde{\eta}$, $1 - \frac{\langle \tilde{\rho} (\tilde{\eta} ) \rangle}{\langle \tilde{\rho} (\tilde{\eta}_*  ) \rangle}$
    {: .cl-schema }

    If self-consistent expansion:

    $\tilde{\eta}$, $\frac{\langle\text{LHS} - \text{RHS}\rangle}{\langle \text{LHS} + \text{RHS}\rangle}$, $\langle  \text{LHS} \rangle$, $\langle \text{RHS} \rangle$
    {: .cl-schema }

    where LHS and RHS are the left- and right-hand sides of Eq.$~$\eqref{eq_FriedmannHubble}.

-   `average_gauss_[U1/SU2]_[nfld].txt`{ .cl-fname }

    $\tilde{\eta}$, $\frac{\langle \sqrt{(\text{LHS} - \text{RHS})^2} \rangle}{\langle \sqrt{(\text{LHS} + \text{RHS})^2} \rangle}$, $\langle \sqrt{(\text{LHS} - \text{RHS})^2} \rangle$, $\langle \sqrt{(\text{LHS} + \text{RHS})^2} \rangle$
    {: .cl-schema }

    where LHS and RHS are the left- and right-hand sides of Eq.$~$\eqref{eq_GaussU1-eom} [for the U(1) sector] and Eq.$~$\eqref{eq_GaussSU2-eom} [for the SU(2) sector].

-   `average_scale_factor.txt`{ .cl-fname }

    $\tilde \eta$, $a$, $a'$, $a' \over a$
    {: .cl-schema }

-   `spectra_scalar_[nfld].txt`{ .cl-fname }

    $\tilde{k}$, $\widetilde{\Delta}_{\tilde \phi} (\tilde k)$, $\widetilde{\Delta}_{\tilde \phi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{bin}$
    {: .cl-schema }

-   `spectra_norm_cmplx_scalar_[nfld].txt`{ .cl-fname }

    $\tilde{k}$, $\widetilde{\Delta}_{\widetilde\varphi} (\tilde k)$, $\widetilde{\Delta}_{\widetilde\varphi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{bin}$
    {: .cl-schema }

-   `spectra_norm_SU2Doublet_scalar_[nfld].txt`{ .cl-fname }

    $\tilde{k}$, $\widetilde{\Delta}_{\widetilde\Phi} (\tilde k)$, $\widetilde{\Delta}_{\widetilde\Phi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{bin}$
    {: .cl-schema }

-   `spectra_norm_[U1/SU2]_[nfld].txt`{ .cl-fname }

    $\tilde{k}$, $\widetilde{\Delta}_{\widetilde{\mathcal{E}}}(\tilde k)$, $\widetilde{\Delta}_{\widetilde{\mathcal{B}}}(\tilde k)$, $\Delta n_{bin}$
    {: .cl-schema }

</div>

### **Scalar-gauge physics inside $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$**

<!-- #### Initial conditions { #subsubsec_initialConditionsNonAb } -->
[](){ #subsubsec_initialConditionsNonAb }
**Initial conditions**

In the tab [*Initialization of fluctuations*][sec_InitScalar] of Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md), we have presented how the initial conditions for singlet scalars are typically imposed in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. Here, we explain instead how to set the initial conditions for complex and SU(2) doublet scalars, as well as for the associated Abelian and non-Abelian gauge fields, see also Section [*Initial Conditions*](IC.md). We denote the time at which the initial conditions are imposed (typically the initial time of the simulation) as $\eta_*$, so that all quantities with a $*$ subindex must be understood to be evaluated at that time: for example, $\varphi_* \equiv \varphi ({\bf x}, \eta_* )$ for complex scalars, and $\dot{\varphi}_* \equiv \dot{\varphi} ({\bf x}, \eta_* )$ for the doublets.

The initialization of the complex and SU(2) doublet scalars is very similar to the singlet scalars: they consist in a homogeneous amplitude chosen by the user, over which a set of fluctuations is superimposed. However, we must take into account that these fields have multiple components. As described above, see [*The model and input files for scalar-gauge theories*][subsec_ScalarGaugeInput], in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ the user can specify the initial absolute values $|\varphi_*|$ and $|\Phi_*|$ in the input file Then one could decide how to distribute this power between the different components in the model file, but as the scalar potential only depends on $|\varphi|$ and $|\Phi|$, we can always *rotate* the field configuration so that all components have equal initial homogeneous amplitude. Thus, for the complex scalars we impose

[](){ #eq_ICcomplex }
[](){ #eq_ICcomplexDeriv }
```math
\begin{align} 
\label{eq_ICcomplex}
\varphi_* &= \frac{1}{\sqrt{2}} (\varphi_{0*} + i \varphi_{1*} ) \hspace{0.4cm} \Longrightarrow \hspace{0.4cm}
\varphi_{n*} \equiv |\varphi_*|  +  \delta \varphi_{n*} ({ \bf x})   , \hspace{0.4cm} [n=0,1]  ,  \\
\label{eq_ICcomplexDeriv}
\dot{\varphi}_{*} &= \frac{1}{\sqrt{2}} (\dot{\varphi}_{0*} + i \dot{\varphi}_{1*} ) \hspace{0.4cm} \Longrightarrow \hspace{0.4cm}
\dot{\varphi}_{n*}  \equiv |\dot{\varphi}_*|  +  \delta \dot{\varphi}_{n*} ({ \bf x})   , \hspace{0.4cm} [n=0,1]  ,
\end{align}
```

where $\delta \phi_{n*} (\vec{x})$ and $\delta \dot{\phi}_{n*} (\vec{x})$ will be tyipcally set by a spectrum of fluctuations of the field amplitude and time-derivative components, specified in Fourier space. Similarly, for the doublets, we impose

[](){ #eq_ICdoublet }
[](){ #eq_ICdoubletDeriv }
```math
\begin{align} 
\label{eq_ICdoublet}
\Phi_*  &=
{1\over\sqrt{2}}
\left(
\begin{array}{c}
\varphi_{0*} +i\varphi_{1*} \\ \varphi_{2*} +i\varphi_{3*}
\end{array}
\right)  \hspace{0.4cm}  \Longrightarrow  \hspace{0.4cm}
\varphi_n ({\bf x}, t_* ) \equiv \frac{|\Phi_*|}{\sqrt{2}}  +  \delta \varphi_{n*} ({ \bf x})    , \hspace{0.4cm} [n=0,1,2,3]  , \\
\label{eq_ICdoubletDeriv}
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

On the other hand, for the Abelian and non-Abelian gauge fields, we impose
[](){ #eq_Inflc1 }
[](){ #eq_Inflc2 }
[](){ #eq_Inflc3 }
[](){ #eq_Inflc4 }
```math
\begin{align}
A_i ({\bf x}, t_* ) &= 0  , \label{eq_Inflc1}\\
B_i^a ({\bf x}, t_* ) &= 0  ,  \label{eq_Inflc2} \\
\dot{A}_i ({\bf x}, t_* ) &= \delta \dot{A}_{i*} ({\bf x})  ,  \label{eq_Inflc3} \\
\dot{B}_i^a ({\bf x}, t_* ) &= \delta \dot{B}_{i*}^a ({\bf x})  ,  \label{eq_Inflc4}
\end{align}
```

i.e. the initial amplitude of the gauge fields is set *exactly* to zero at all lattice points, while we only impose an initial spectrum of fluctuations to their time-derivatives (over vanishing homogeneous values). Due to this, the initial magnetic energy will be exactly zero, while a small amount of electric energy will be initially present due to the fluctuations of the time-derivatives of the gauge fields. While this is just a choice, the user is welcome to modify this prescription, but we anticipate that this might become a non-trivial task, due to the necessity to respect the Gauss-law(s), namely Eq.$~$\eqref{eq_GaussU1-eom} [for the U(1) sector] and Eq.$~$\eqref{eq_GaussSU2-eom} [for the SU(2) sector]. We discuss this next, for our chosen configuration above.

The initial fluctuations of both charged scalars and gauge fields must be imposed so that the Gauss constraint is verified initially. As long as this is true, the Gauss constraint will remain preserved during the entire dynamical evolution of the system, thanks to the fact that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ uses lattice gauge-invariant techniques. In order to achieve the said goal, let us first Fourier transform the Gauss constraints Eq.$~$\eqref{eq_GaussU1-eom} and Eq.$~$\eqref{eq_GaussSU2-eom} at the initial time $\eta_*$. We get
[](){ #eq_kAi1 }
```math
\begin{eqnarray}
{k}^i \widetilde{A}'_i ({\bf k}) = \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^A ({\bf k})  , \hspace{0.4cm} {k}^i \widetilde{B}_i^{a'} ({\bf k}) = \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^a ({\bf k})  , \label{eq_kAi1}
\end{eqnarray}
```

where $J_0^A ({\bf k})$ and $J_0^a ({\bf k})$ are the Fourier transforms of each current. A solution of these equations, for $\bf k \neq \bf 0$, is trivially found as
[](){ #eq_GaugeCurrentFluc }
```math
\begin{eqnarray}
\widetilde{A}'_i ({\bf k}) = i \frac{{k}_i}{{k}^2} \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^A ({\bf k})  , \hspace{0.4cm} \widetilde{B}^{a'}_i ({\bf k}) = i \frac{{k}_i}{{k}^2} \frac{f_*^2}{\omega_*^2} \widetilde{J}_0^a ({\bf k}) . \label{eq_GaugeCurrentFluc}
\end{eqnarray}
```

The way in which we proceed to set fluctuations is the following. First,
we impose on the lattice the following fluctuations to the components of the charged scalars (in program units), following the same prescription for the fluctuations of the scalar singlets, *c.f.* Eqs. ([*24*][eq_fpr_influct])-([*25*][eq_fpr_influct2]) in Section [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md):
[](){ #eq_fpr_influct3 }
[](){ #eq_fpr_influct4 }
```math
\begin{align}
\delta \tilde{\varphi}_{n*}({  \bf \tilde n}) &= \frac{1}{\sqrt{2}} \left(|\delta \tilde{\varphi}_{n1} ({  \bf \tilde n})|  e^{i \theta_{n1} ({  \bf \tilde n}) } + |\delta \tilde{\varphi}_{n2} ({  \bf \tilde n})| e^{i \theta_{n2} ({  \bf \tilde n}) }   \right) \label{eq_fpr_influct3}  , \\
\delta \tilde{\varphi}'_{n*} ({  \bf \tilde n}) &= a^{1-\alpha}\left[\frac{1}{\sqrt{2}} i \tilde{\omega}_{k,n} \left(|\delta \tilde{\varphi}_{n1} ({  \bf \tilde n})| e^{i \theta_{n1} ({  \bf \tilde n}) } - |\delta \tilde{\varphi}_{n2}  ({  \bf \tilde n})| e^{i \theta_{n2} ({  \bf \tilde n}) }  \right)\right]  - \tilde{\mathcal{H}} \delta \tilde{\varphi}_{n} ({  \bf \tilde n}) ,  \label{eq_fpr_influct4}
\end{align}
```

where $\tilde{\omega}_{k,n}  \equiv \omega_{k,n} /\omega_* =  \sqrt{\tilde{k}^2 + a^2 (\partial^2 \tilde{V} / \partial \tilde{\varphi}_n^2)}$ is the initial effective frequency of the modes of each field component in program units. Once created, these fluctuations generate fluctuations on the currents $\widetilde{J}_0^A ({\bf x})$, and $\widetilde{J}_0^a ({\bf x})$, which can then be used to compute the corresponding fluctuations of the gauge fields in momentum space, via Eq.$~$\eqref{eq_GaugeCurrentFluc}. Finally, transforming back to position space, we obtain $\delta \widetilde{A'}_{i*} ({\bf x})$, $\delta \widetilde{B}_{i*}^{a'} ({\bf x})$.

In order for this procedure to work consistenly, we need to slightly modify the initialization of the charged field components Eq.$~$\eqref{eq_fpr_influct3}-Eq.$~$\eqref{eq_fpr_influct4} with respect to the prescription for scalar singlets. In the case of scalar singlets, we would have $\theta_1 ({\bf \tilde{n}})$ and $\theta_2 ({\bf \tilde{n}})$ as two random independent phases which vary uniformly in the range $[0, 2\pi)$ from point to point, while $|\delta \tilde{\phi}_1 ({\bf \tilde{n}})|$ and $|\delta \tilde{\phi}_2 ({\bf \tilde{n}})|$ would be two amplitudes that vary from point to point according to a *Rayleigh* distribution with expected square amplitude given by Eq. ([*26*][eq_QuantumFlucts2]) from Section [*Scalar-Scalar Interactions*](My first model of (singlet) scalar fields.md). However, in the case of charged fields, we need to set the homogeneous mode of the currents to zero, *i.e.* $J_0^A ({\bf k} ={\bf 0} ) = J_0^a ({\bf k} ={\bf 0} ) = 0$, so that there is a vanishing net-charge in the lattice. As shown in $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl], this can be ensured by the choice
[](){ #InConstr_1 }
[](){ #InConstr_2 }
```math
\begin{eqnarray}
\begin{array}{rcll} |\delta \varphi_{n1} ({\bf k})| &=& |\delta \varphi_{n2}  ({\bf k})|  & , ~n=0,1(,2,3)  , \\
\theta_{n2} ({\bf k}) &=& \theta_{02} ({\bf k}) + \theta_{n1} ({\bf k}) - \theta_{01} ({\bf k})  & , ~n=1(,2,3)  . 
\end{array} \label{InConstr_2}
\end{eqnarray}
```

In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, we thus generate randomly only $\theta_{01}$, $\theta_{02}$, as well as $\theta_{n1}$, $\varphi_{n1}$ for $n=1(,2,3)$, and let the other functions be initialized through out the constraints given in Eq.$~$\eqref{InConstr_2}.

<!-- #### Evolution equations -->
[](){ #subsubsec_EvolEqsScalarGauge }
**Evolution equations**

In the tab [*Evolution of the system*][eq_evolution-sc] of Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md), we wrote a Hamiltonian scheme for the equations of motion (EOM) of a system of singlet scalars in an expanding universe. As explained there, we conveniently defined a set of conjugate momenta $\{\pi_{\phi},b\}$ for the scalar field(s) and the scale factor $\{\phi, a\}$, in such a way that the fields' EOM and Friedmann equations become a set of four first-order differential equations. Thanks to our definitions of conjugate momentum, the kernels in the EOM do not depend on the time-derivatives of the corresponding fields, allowing to solving the EOM (their discretized version) via symplectic algorithms, such as staggered leapfrog or verlet integration methods.

The same idea can be applied in the case of a scalar-gauge theory, which bsides scalar fields it also contains gauge fields. In particular, we define the following conjugate momentum for each of the five field species $\{\phi,\varphi,\Phi,A_i,B_i^a\}$, as well as for the scale factor $a(\eta)$, as follows
[](){ #eq_momSinglet }
[](){ #eq_momU1complex }
[](){ #eq_momSU2doublet }
[](){ #eq_momU1vec }
[](){ #eq_momSU2vec }
```math
\begin{align}
\tilde\pi_\phi &= a^{3-\alpha}\tilde\phi'   ,  \label{eq_momSinglet}\\
\tilde\pi_\varphi&=  a^{3-\alpha}\tilde\varphi'   ,   \label{eq_momU1complex}\\
\widetilde\pi_\Phi&= a^{3-\alpha}\widetilde\Phi'   ,    \label{eq_momSU2doublet}\\
\left(\tilde\pi_A\right)_i &= a^{1-\alpha}\widetilde F_{0i}   , \label{eq_momU1vec}  \\
\left(\tilde\pi_B\right)^{a}_i &= a^{1-\alpha}\widetilde G^a_{0i}   , \label{eq_momSU2vec} \\
b &= a'   .
\end{align}
```

With these definitions the equations of motion can be then written as
[](){ #eq_kernelsingletscalar }
[](){ #eq_kernelcomplexscalar }
[](){ #eq_kerneldoubletscalar }
[](){ #eq_kernelsU1 }
[](){ #eq_kernelsSU2 }
```math
\begin{align}
(\tilde\pi_\phi)'    &=   \mathcal{K}_{\phi}[a,\tilde\phi,|\tilde{\varphi}|,|\widetilde{\Phi}|] & &   \equiv     - a^{3 + \alpha} \widetilde V_{,\tilde\phi}  + a^{1 + \alpha} {\widetilde \nabla}^{2} \tilde\phi    ,\label{eq_kernelsingletscalar}\\
(\tilde\pi_\varphi)'    &=   \mathcal{K}_{\varphi}[a,\tilde\phi,\tilde\varphi,|\widetilde{\Phi}|,\widetilde A_j]  & &   \equiv     a^{3 + \alpha} \widetilde V_{,|\tilde\varphi|} \frac{1}{2} \frac{\tilde\varphi}{|\tilde\varphi |} + a^{1 + \alpha} {\vec{\widetilde D}}_{\hspace{-0.5mm}A}^{ 2}\tilde\varphi    , \label{eq_kernelcomplexscalar} \\
(\widetilde\pi_\Phi)'    &=   \mathcal{K}_{\Phi}[a,\tilde\phi,|\tilde\varphi|,\widetilde\Phi,\widetilde B_j^a] & &    \equiv     - a^{3 + \alpha} \widetilde V_{,|\widetilde\Phi|} \frac{1}{2} \frac{\widetilde\Phi}{|\widetilde\Phi |} + a^{1 + \alpha} {\vec{\widetilde{D}}}_{\hspace{-0.5mm}A}^{ 2}\widetilde\Phi  \label{eq_kerneldoubletscalar}    ,\\
\left(\tilde\pi_A\right)'_i    &=   \mathcal{K}_{A_i}[a,\tilde\varphi,\widetilde \Phi,\widetilde A_j] & &   \equiv     a^{1+ \alpha}\widetilde J^A_i + a^{\alpha - 1}\tilde{\partial}_j \widetilde F_{ji}     ,   \label{eq_kernelU1}\\
\left(\left(\tilde\pi_B\right)_i^a\right)'   &=   \mathcal{K}_{B_i}[a,\widetilde\Phi,\widetilde A_j,\widetilde B_j^a] & &    \equiv     a^{1+ \alpha}\widetilde J^a_i + a^{\alpha - 1}( \mathcal{\widetilde D}_j )_{a b} (\widetilde G_{ji} )^b      . \label{eq_kernelSU2}
\end{align}
```

On the other hand, the equation for the evolution of the scale factor can be written as
[](){ #eq_kernelscalefactor }
```math
\begin{align}
\label{eq_kernelscalefactor}
\hspace{-1.5cm}
b' &= \mathcal{K}_a\hspace{-1mm}\left[a,{\widetilde E}_K^\phi,{\widetilde E}_K^\varphi,{\widetilde E}_K^\Phi,{\widetilde E}_G^\phi,{\widetilde E}_G^\varphi,{\widetilde E}_G^\Phi,{\widetilde E}_K^A,{\widetilde E}_G^A,{\widetilde E}_K^B,{\widetilde E}_G^B,{\widetilde E}_V\right]  \\
&\equiv \frac{a^{2\alpha+1}}{3}{f_*^2\over m_p^2}\left[ (\alpha-2)({\widetilde E}_K^\phi  + {\widetilde E}_K^\varphi + {\widetilde E}_K^\Phi )  +
\alpha ({\widetilde E}_G^\phi  + {\widetilde E}_G^\varphi + {\widetilde E}_G^\Phi )+ (\alpha-1)
({\widetilde E}_K^A+{\widetilde E}_G^A + {\widetilde E}_K^B+{\widetilde E}_G^B) + (\alpha+1) {\widetilde E}_V  \right]  .\nonumber
\end{align}
```

The functions $\mathcal{K}_f$ above, represent the kernels of the different matter field species, $f=\phi,\varphi,\Phi,A_i,B_i^a$, whereas $\mathcal{K}_a$ represents the kernel of the scale factor. On a lattice, the different kernels can be discretized using lattice gauge-invariant techniques, as described *e.g.* in  Section 3 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl], or in this website's tab [*Lattice gauge invariant techniques*][subsec_LGT]. As for singlet scalar theories, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ provides already implemented different evolution algorithms to solve the EOM of scalar-gauge theories, namely staggered *leapfrog* and *Verlet* evolvers. While the former has a fixed accuray in time-evolution of $\mathcal{O}(d\eta^2)$, the latter methods (both position- and velocity-Verlet), which by default have also $\mathcal{O}(d\eta^2)$ accuracy, can however be improved via recursive variants (using the Yoshida method), leading to algorithms of successive increasing accuracies, namely of $\mathcal{O}(d\eta^4)$, $\mathcal{O}(d\eta^6)$, $\mathcal{O}(d\eta^8)$, and $\mathcal{O}(d\eta^{10})$. The details of the lattice version of the EOM specialized for scalar-gauge theories, and of how the aforementioned evolution algorithms work, can be found in Sections 5 (for Abelian gauge theories) and 6 (non-Abelian gauge theories) of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl]. 

