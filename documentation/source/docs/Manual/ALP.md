<!-- <div style="text-align: justify;"> -->
CosmoLattice can evolve pseudo-scalar fields, known as *axion-like-particles* (ALPs), that are coupled to $U(1)$ gauge fields via an interaction of the form $\phi F\tilde F$. As opposed to the charged scalars described in Section [Scalar-Gauge Interactions](My first model of gauge fields.md), these pseudo-scalar fields are not charged under a gauge symmetry. Note that while ALPs can also couple to non-Abelian gauge fields in nature (via $\phi G\tilde G$), currently only the Abelian $U(1)$ interactions are implemented in the code. This module can be considered as an extension of the standard scalar and Abelian gauge dynamics: the fields, program variables, input-file syntax, potential functions, and standard outputs follow the same conventions to a large extent. In this section, we emphasize the additional technical details that arise from introducing this type of non-canonical coupling.

This section is structured as follows. In Section [*Axion-gauge continuum dynamics*][subsec_ALPcontinuum] we introduce the continuum equations of motion for the axion and the $U(1)$ gauge-field solved by the code, as well as the self-consistent exapansion of the scale factor. In Section [*Model and input files for axion-U(1) theories*][subsec_ALPInput] we describe an example model and axion-U(1) input parameters. In Section [*The axion-U(1) model file*][sec_TheALPModelFile] we show the model-file additions that activate the module. Finally, in Section [*The axion-U(1) physics inside CosmoLattice*][sec_WhatHappensAutoinALP] we summarize the axion-specific initialization and evolution logic.




### **Axion-gauge continuum dynamics** { #subsec_ALPcontinuum }

The action of an axion-$U(1)$ gauge theory that can be simulated in CosmoLattice is the following:
[](){ #eq_AxionAction }
```math
\begin{align}
\label{eq_AxionAction}
S =
\int d^{4}x \sqrt{-g} \left[
\frac{1}{2}m_p^2R
- \frac{1}{2} g^{\mu\nu}\partial_{\mu}\phi\partial_{\nu}\phi
- V(\phi)
- \frac{1}{4}F_{\mu\nu}F^{\mu\nu}
+ \frac{\alpha_{\Lambda}}{4}\frac{\phi}{m_p} F_{\mu\nu}\tilde{F}^{\mu\nu}
\right]  ,
\end{align}
```

where $m_p$ is the reduced Planck mass and $\alpha_{\Lambda}=m_p/\Lambda$ is the dimensionless coupling strength of axion-U(1) coupling with with $\Lambda$ representing an energy scale. Note that $F_{\mu\nu}$ is the standard field strength tensor of the U(1) gauge field $A_{\mu}$ defined in Eq. ([*5*](My first model of gauge fields.md#eq:eq_FmnAbelian)) of [Scalar-Gauge Interactions](My first model of gauge fields.md), and that $\tilde{F}_{\mu\nu} = \frac{1}{2}\epsilon_{\mu\nu\rho\sigma}F^{\rho\sigma}$ is its dual counterpart, with $\epsilon_{\mu\nu\rho\sigma}$ being the completely antisymmetric Levi-Civita tensor, which in a curved spacetime takes the form $\epsilon_{0123} = 1/\sqrt{-g}$.


We use the same spatially-flat FLRW background metric introduced in Eq. ([*1*](My first model of (singlet) scalar fields.md#eq_FLRWmetric)) from Section [Scalar-singlet interactions](My first model of (singlet) scalar fields.md). The variation of the action yields the following continuum equations of motion for the axion and the gauge fields (in cosmic time and temporal gauge $A_0$):

[](){ #eqn_eom_axion }
```math
\begin{align}
\label{eqn:eom1}
\ddot{\phi} &= -3H\dot{\phi}+\frac{1}{a^2}\nabla^2\phi-V(\phi)_{,\phi}+\frac{\alpha_\Lambda}{ m_pa^{3}}E_iB_i \, , \\
\label{eqn:eom2}
\dot{E}_i &= -HE_i-\frac{1}{a^2}\epsilon_{ijk}\partial_jB_k-\frac{\alpha_\Lambda}{am_p}\left(\dot{\phi}B_i-\epsilon_{ijk}\partial_j\phi E_k\right) \, ,
\end{align}
```
with the standard definitions of the electric and magnetic field from Eq. ([*8*](My first model of gauge fields.md#eq:eq_FmnAbelian)) of [Scalar-Gauge Interactions](My first model of gauge fields.md) but using standard letters $E_i$ and $B_i$.

Additionally, the variation with respect to the temporal component of the gauge field leads to the following constraint equation:

[](){ #eqn_Gauss }
```math
\begin{align}
\label{eqn:Gauss}
\partial_iE_i &= -\frac{\alpha_{\Lambda}}{m_pa}\partial_i\phi B_i \, . \quad\quad\quad {\rm [Gauss~Constraint]}
\end{align}
```

If we consider that the background expansion is driven by both the axion-like particle and the gauge field, the Friedmann equations can be written as:

[](){ #eqn_Friedmann }
```math 
\begin{align}
\ddot{a} &= -\frac{a}{3m_p^2}\big( 2E_{K} - E_{V} + E_{EM} \big)\,, \label{eqn:ddaAxion} \\ 
H^2 &= \frac{1}{3m_p^2}\big(E_{K} + E_{G} + E_{V} + E_{EM}\big)\,. \quad\quad\quad \text{[Hubble Constraint]} \label{eqn:HubbleAxion}
\end{align}
```

In this context, the various homogeneous components of the energy density are defined as in [Scalar-Gauge Interactions](My first model of gauge fields.md) for a non-charged scalar and an abelian gauge field:

[](){ #eqn_energies }
```math 
\begin{align}
E_{K} \equiv \frac{1}{2}\langle\dot{\phi}^2\rangle\;, \quad E_{G} \equiv \frac{1}{2a^2}\langle(\vec\nabla\phi)^2\rangle\;, \quad E_{V} \equiv \langle V(\phi) \rangle\;, \quad E_{EM} \equiv \frac{1}{2a^4}\langle a^2\vec E^2+\vec B^2\rangle\;. \label{eqn:energyDensityTerms}
\end{align}
```

Note that $E_K$, $E_G$, and $E_V$ refer to the kinetic, gradient, and potential energy densities of the axion, respectively, while $E_{EM}$ denotes the electromagnetic energy density associated with the abelian gauge field $A_\mu$.


Regarding CosmoLattice, we treat the continuum dynamics of the aforementioned model using the program variables and $\alpha$-time exactly as in [Scalar-Gauge Interactions](My first model of gauge fields.md) and [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md). Thus, the matter dynamics and constraints read as follows:

[](){ #eqn_eom_axion_pralpha }
```math
\begin{align}
\label{eqn:eom1_pralpha}
\tilde{\phi}'' &= -(3-\alpha)\frac{a'}{a}\tilde{\phi}'+a^{2(\alpha-1)}\tilde{\nabla}^2\tilde{\phi}-a^{2\alpha}\tilde{V}(\tilde{\phi})_{,\tilde{\phi}}+\left(\frac{\omega^2_{*}}{f_{*}m_p}\right)\alpha_\Lambda a^{\alpha-3}\tilde{E}_i\tilde{B}_i \, , \\
\label{eqn:eom2_pralpha}
\tilde{E}'_i &= (\alpha-1)\frac{a'}{a}\tilde{E}_i-a^{2(\alpha-1)}\epsilon_{ijk}\tilde{\partial}_j\tilde{B}_k-\left(\frac{f_{*}}{m_p}\right) \alpha_\Lambda a^{\alpha-1}\left(\tilde{\phi}'\tilde{B}_i-\epsilon_{ijk}\tilde{\partial}_j\tilde{\phi} \tilde{E}_k\right) \, , \\
\label{eqn:Gauss_pralpha}
\tilde{\partial}_i \tilde{E}_i &= -\left(\frac{f_{*}}{m_p}\right)\alpha_\Lambda a^{\alpha-1}\tilde{\partial}_i\tilde{\phi} \tilde{B}_i \, . \quad\quad\quad {\rm [Gauss~Constraint]}
\end{align}
```

Furthermore, the self-consistent expansion is governed by the Friedmann equations ([*24*](My first model of gauge fields.md#eq_FriedmannHubble)) and ([*25*](My first model of gauge fields.md#eq:#eq_FriedmannDDa)) of [Scalar-Gauge Interactions](My first model of gauge fields.md), which in turn contain the definitions ([*23*](My first model of gauge fields.md#eq:#eq_energy-contrib)) from the same section for the various energy components present.



### **Model and input files for axion-U(1) theories** { #subsec_ALPInput }

The example model $\texttt{m2phi_axionU1.h}$ contains one scalar singlet, $N_s=1$, which represents the axion-like particle, and one $U(1)$ gauge field, $N_{U1}=1$. The potential consists of a single term, $N_{\rm pot}=1$, given by

[](){ #eq_AxionExamplePotential }
```math
\begin{align}
\label{eq_AxionExamplePotential}
V(\phi)
&=
\frac{1}{2}m^2\phi^2 ,
\end{align}
```
where $m$ is the mass of the axion-like particle.

For this model, we choose the following program variables:

[](){ #eq_AxionExampleProgramVars }
```math
\begin{align}
\label{eq_AxionExampleProgramVars}
f_*=\phi_*~,\hspace{0.4cm}
\omega_*=m~,\hspace{0.4cm}
\alpha=0 .
\end{align}
```

The program potential therefore contains only one term,

[](){ #eq_AxionExampleProgramPotential }
```math
\begin{align}
\label{eq_AxionExampleProgramPotential}
\widetilde V(\tilde\phi)
\equiv
\frac{1}{f_*^2\omega_*^2}
V(f_*\tilde\phi)
=
\frac{1}{2}\tilde\phi^2 .
\end{align}
```


The corresponding input file is $\texttt{models/parameter-files/axionU1_m2phi2.in}$:

@emgithub(models/parameter-files/axionU1_m2phi2.in)

The parameter syntax, <!-- command-line overrides,  --> lattice parameters, output frequencies, and scalar initial conditions are the same as in Section [*Running the program with an input parameter file*](My first model of (singlet) scalar fields.md#subsec_Input-Scalars). The Axion-Guage specific additions are:

| **Parameters** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `alphaLambda_AxionU1` | Dimensionless coupling constant $\alpha_{\Lambda}$ between the axion-like particle and the $U(1)$ gauge field. In this model there is one ALP, so one value is expected. |
| `mass` | Mass $m$ of the axion-like particle in GeV. In this model there is one ALP, so one value is expected. |
| `evolver` | Axion-gauge models  can only  be used with of the Runge-Kutta evolvers, such as `RK2`, `RK3_3`, `RK3_4`, or `RK4_5`. |
| `tNonLinearAxionU1` | If a value between `t0` and `max` is specified for this variable, the evolution up to this value will take place in what is known as the *linear regime*. Further details are provided in Section [B]. |
| `flagChiralPS` | Setting this parameter to `true` triggers the generation of the $\texttt{spectra_chiral_U1_0.txt}$ and $\texttt{spectra_chiral_Elec_U1_0.txt}$ files, which contain the chiral spectra $A^{\pm}$ and $E^{\pm}$, respectively. More details are provided in Section [*External power spectrum for scalar singlet initialization*](IC.md#subsubsec_ExternalPSSingletIC). |

Standard scalar-singlet parameters still apply. In particular, `initial_amplitudes` and `initial_momenta` contain only one entry, corresponding to the axion field $\phi$. A typical command-line override is
```bash
./m2phi_axionU1 input=../models/parameter-files/m2phi_axionU1.in N=64 alphaLambda_AxionU1=13 mass=1.5e13
```
which runs the model on a $64^3$ lattice and sets $\alpha_{\Lambda}=13$ and $m=1.5\cdot 10^{13} \text{GeV}$.

#### Fixed background expansion

This model can also be run in a fixed expanding background, analogously to the scalar-singlet setup around Eq. ([*11*](My first model of (singlet) scalar fields.md#eq_ScaleFactorPowerLaw)). This is activated with

```text
expansion = true
fixedBackground = true
omegaEoS = ...
H0 = ...
```

In that case, the scale factor is not sourced by the lattice fields, but by an external homogeneous component with constant equation of state $\omega_{\rm EoS}$.

#### Output files

An axion-$U(1)$ simulation generates the standard output files for both the scalar-singlet (the axion) and the $U(1)$ gauge field sectors, following the default conventions described in Sections [A] and [B]. The specific features and additional files for this module are:

*   **Gauss Constraint**: The Gauss constraint evaluation is adapted to the specific axion-$U(1)$ expression given in Eq. [C], but the file output format remains identical to the standard gauge setup.
*   **Energy Conservation**: When evolving within the *linear regime* (see Section [B]), the total energy density calculation used for the energy conservation check does not include the contribution from the gauge sector.
*   **Chiral Spectra**: If `flagChiral = true`, the module generates additional output files containing the chiral spectra:
    *   $\texttt{spectra\_chiral\_U1\_0.txt}$: Chiral power spectra of the $U(1)$ gauge field, containing the columns:  
        $\hspace{1cm}$ $\tilde{k}$, $\widetilde{\Delta}_{\widetilde{A}^{+}} (\tilde k)$, $\widetilde{\Delta}_{\widetilde{A}^{-}} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{\rm bin}$
    *   $\texttt{spectra\_chiral\_Elec\_U1\_0.txt}$: Chiral power spectra of the $U(1)$ electric field, containing the columns:  
        $\hspace{1cm}$ $\tilde{k}$, $\widetilde{\Delta}_{\widetilde{E}^{+}} (\tilde k)$, $\widetilde{\Delta}_{\widetilde{E}^{-}} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{\rm bin}$

### **The axion-U(1) model file** { #sec_TheALPModelFile }

To define a new axion-$U(1)$ model, start from the example model file $\texttt{models/m2phi2_axionU1.h}$. You simply define the total number of scalar fields and gauge fields in the `ModelPars` structure, and activate the interaction via the coupling manager:

@emgithub(models/axionU1_m2phi2.h:model_pars)

The `ScalarU1AxionCouplings` typedef activates the axion-$U(1)$ module. Its first template argument is the total number of scalar fields (`NScalars`), its second is the total number of $U(1)$ gauge fields (`NU1Flds`), and the final boolean flag (`true`) activates the axion-like interaction between them.

!!! note "Important Note"
    It is possible to define a model with more than one scalar field (`NScalars > 1`); and by default, all included scalars will be coupled to the $U(1)$ gauge field. Please be aware that the module has not been formally tested with more than one coupled scalar. If you wish to add an additional scalar field that is *not* coupled to the gauge sector, you must set the coupling flag to `false` for that specific field index. Please note that this configuration is also untested.

#### The potential and its derivatives { #sec_PotDerivsNMC }

The model has one non-zero program-potential term, Eq.$~$\eqref{eq_AxionExampleProgramPotential}, which is the inflationary potential:

@emgithub(models/axionU1_m2phi2.h:potential_terms)

where its first derivative is

@emgithub(models/axionU1_m2phi2.h:potential_derivs)

corresponding to
[](){ #eq_AxionExampleProgramPotentialDerivative }
```math
\begin{align}
\label{eq_AxionExampleProgramPotentialDerivative}
\widetilde V_{,\tilde\phi}=\tilde\phi,
\end{align}
```

and second derivative

@emgithub(models/axionU1_m2phi2.h:potential_second_derivs)

The numbering of these functions must be consistent with the scalar-field numbering. In this example we just have `Tag<0>` of the inflaton/axion $\phi$. 

This is a particular example of a potential, any other option can be tested by changing the three entries above.



### **The axion-U(1) physics inside CosmoLattice** { #sec_WhatHappensAutoinALP }

#### Initialization of the fields present { #sec_InitAxionU1 }

By default, the simulation initializes each chirality of the $U(1)$ gauge field and the electric field using the Bunch-Davies solution, while the longitudinal components of both fields are set to zero. Consequently, the user does not need to specify any additional initialization parameters in the input file, as these initial conditions are automatically applied across the momentum range $[k_{\rm IR}, k_{\rm UV}]$.

Furthermore, to satisfy the initial Gauss constraint—which, under these initial conditions, implies that the divergence of the electric field vanishes—the scalar field and its time derivative are set to be initially homogeneous. Their initial values are determined entirely by the standard parameters provided in the input file. For a more detailed discussion on the theoretical background and the current implementation of this procedure, see [*Bunch-Davies transverse Abelian fields*](IC.md#subsubsec_BunchDaviesTransverseU1IC).




#### Evolution equations { #eq_evolution-axionU1 }

For self-consistent expansion, CosmoLattice evolves the fields by solving a set of first-order differential equations. Defining the conjugate momenta
[](){ #eq_AxionU1momentum }
```math
\begin{align}
\label{eq_AxionU1momentum}
\tilde\pi_\phi = a^{3-\alpha}\tilde\phi',
\hspace{0.6cm}
\left(\tilde{\pi}_{A}\right)_{i} = a^{1-\alpha}\tilde{A}_i' \equiv a^{1-\alpha}\tilde{E}_i,
\hspace{0.6cm}
b=a^{1-\alpha}a',
\end{align}
```

the schematic equations of motion are
[](){ #eq_coupledFirstDiffEqAxion }
```math
\begin{align}
\label{eq:coupledFirstDiffEqAxion}
\tilde{\phi}' &= a^{\alpha-3}\tilde{\pi}_\phi, & \tilde{\pi}_\phi' &= \mathcal{K}^{\rm L}_{\phi}[a,\tilde{\phi},\tilde{A}_j,a',\tilde{\pi}_{\phi},\left(\tilde{\pi}_{A}\right)_{j}]\,, \\
\tilde{A}_i' &= a^{\alpha-1}\left(\tilde{\pi}_{A}\right)_{i}, & \left(\tilde{\pi}_{A}\right)'_{i} &= \mathcal{K}^{\rm L}_{A_i}[a,\tilde{\phi},\tilde{A}_j,a',\tilde{\pi}_{\phi},\left(\tilde{\pi}_{A}\right)_{j}]\,, \\
a' &= b, & b' &= \mathcal{K}^{\rm L}_{a}[a,\tilde{E}^{\phi}_{K},\tilde{E}^{\phi}_{G},\tilde{E}^{\phi}_{V},\tilde{E}^{A}_{K},\tilde{E}^{A}_{G}]\,.
\end{align}
```

where the lattice kernels for the fields are given by:
[](){ #eq_explicitEOMKernels }
```math
\begin{align}
\mathcal{K}^{\rm L}_{\phi}={}&a^{1+\alpha} \sum_i \tilde{\nabla}_i^-\tilde{\nabla}_i^+ \tilde{\phi} - a^{\alpha+3}\frac{d\tilde{V}(\tilde{\phi})}{d\tilde{\phi}} + \left(\frac{\omega^2_{*}}{f_{*}m_p}\right)\alpha_\Lambda a^{\alpha-1}\sum_i \left(\tilde{\pi}_{A}\right)_{i}^{(2)}\tilde{B}_i^{(4)} \;, \label{eq:explicitEOMscalarKernelAlpha} \\
\mathcal{K}^{\rm L}_{A_i} ={}& - a^{1+\alpha} \sum_{j,k} \epsilon_{ijk} \tilde{\nabla}_j^- \tilde{B}_k- \left(\frac{f_{*}}{m_p}\right) \frac{\alpha_\Lambda a^{\alpha-3}}{2}\left(\tilde{\pi}_{\phi} \tilde{B}_i^{(4)} + \tilde{\pi}_{\phi,+i}\tilde{B}^{(4)}_{i,+i} \right) + \left(\frac{f_{*}}{m_p}\right)\frac{\alpha_\Lambda a^{\alpha-1}}{4} \sum_\pm \sum_{j,k} \epsilon_{ijk}  \left\{ \left[ (\tilde{\nabla}_j^\pm \tilde{\phi}) \left(\tilde{\pi}_{A}\right)_{k,\pm j}^{(2)} \right]_{+i} +  \left[ (\tilde{\nabla}_j^\pm \tilde{\phi}) \left(\tilde{\pi}_A\right)_{k,\pm j}^{(2)}   \right] \right\} \;, \label{eq:explicitEOMgaugeKernelAlpha}
\end{align}
```

and the scale factor kernel reads:
[](){ #eq_explicitScaleFactorKernelAlpha }
```math
\begin{align}
\label{eq:explicitScaleFactorKernelAlpha}
\mathcal{K}_{a}^{\rm L}[a,\tilde{E}^{\phi}_{K},\tilde{E}^{\phi}_{G},\tilde{E}^{\phi}_{V},\tilde{E}^{A}_{K},\tilde{E}^{A}_{G}] = \left(\frac{f_{*}}{m_{\text{p}}}\right)^2\frac{a^{2\alpha+1}}{3}\left[(\alpha-2)\tilde{E}^{\phi}_{K}+\alpha\tilde{E}^{\phi}_{G}+(\alpha+1)\tilde{E}^{\phi}_{V}+(\alpha-1)\tilde{E}^{A}_{K}+(\alpha-1)\tilde{E}^{A}_{G}\right]\,.
\end{align}
```

The matter kernels $\mathcal{K}_{\phi}^{\rm L}$ and $\mathcal{K}_{A_i}^{\rm L}$ represent the discrete lattice formulations that govern the dynamical evolution of the system. All terms not coupled to the axion-like parameter $\alpha_\Lambda$ follow the standard spatial discretization that strictly respects lattice gauge invariance. This default procedure can be reviewed in Section [A/B] or in [*The Art of CosmoLattice I*](https://arxiv.org/abs/2006.15122).

However, due to the chiral coupling between the axion-like particle (ALP) and the Abelian gauge field, the lattice formulation must additionally capture the topological nature of the $\phi F\tilde{F}$ term while preserving the aforementioned gauge symmetries. To achieve this, we make use of the redefined discrete electric and magnetic fields, $\left(\tilde{\pi}_{A}\right)^{(2)}_i$ and $\tilde{B}^{(4)}_i$, which are defined at each lattice site $\mathbf{n}$ as:
[](){ #eq_DiscreteAveragedFields }
```math
\begin{align}
\label{eq_DiscreteAveragedFields}
\left(\tilde{\pi}_A\right)^{(2)}_i(\tilde{\eta}, \mathbf{n}) &\equiv \frac{1}{2}\left( \left(\tilde{\pi}_A\right)_i(\tilde{\eta}, \mathbf{n}) + \left(\tilde{\pi}_A\right)_{i,-i}(\tilde{\eta}, \mathbf{n}) \right) \,, \\
\tilde{B}^{(4)}_i(\tilde{\eta}, \mathbf{n}) &\equiv \frac{1}{4}\left( \tilde{B}_i(\tilde{\eta}, \mathbf{n}) + \tilde{B}_{i,-j}(\tilde{\eta}, \mathbf{n}) + \tilde{B}_{i,-k}(\tilde{\eta}, \mathbf{n}) + \tilde{B}_{i,-j-k}(\tilde{\eta}, \mathbf{n}) \right) \,,
\end{align}
```
where the negative subscripts indicate spatial shifts in the corresponding backward directions (i.e., $f_{-a}(\mathbf{n}) \equiv f(\mathbf{n} - \hat{a})$), and $j, k$ are the two spatial directions transverse to the component $i$ (such that $i, j, k$ are mutually orthogonal).

The use of these specifically averaged operators guarantees that we:

*   Preserve the continuous topological properties and gauge invariances at the discrete level.
*   Maintain the standard second-order spatial accuracy, $\mathcal{O}(dx^2)$.
*   Satisfy the discrete Bianchi identities structurally.

For a comprehensive technical discussion of this lattice gauge-invariant implementation, the reader is referred to [The Art of CosmoLattice II](https://arxiv.org/abs/2108.01086) or to the original papers where this topological lattice formulation was first introduced (see e.g. [Cuissa & Figueroa (2020)](https://arxiv.org/abs/2002.04656)).



!!! note "Important Note"
    In this module, the axion-$U(1)$ interaction introduces an inherent dependence on the conjugate momenta $\left(\tilde{\pi}_{A}\right)_j$ directly inside the field kernel $\mathcal{K}_{A_i}^{\rm L}$. Because the evaluation of these kernels requires simultaneous knowledge of the fields and their respective conjugate momenta, standard symplectic algorithms such as `LF`, `VV`, or `PV` cannot be employed. Non-symplectic algorithms, such as the Runge-Kutta family, must be used instead.

The axion-$U(1)$ module can be evolved with the Runge-Kutta family of algorithms implemented in CosmoLattice, for example:
```text
evolver = RK2
```

Higher-order RK algorithms can be used if better time-integration accuracy is needed. During the run, CosmoLattice always monitors the Friedmann constraint using the total energy density, while the dynamical update of the scale factor is performed through the kernel equation defined above.

