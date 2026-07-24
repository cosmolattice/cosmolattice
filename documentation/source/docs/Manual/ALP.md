<!-- <div style="text-align: justify;"> -->
Since version 2.0, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ includes a module to evolve pseudoscalar fields coupled to $\mathrm{U}(1)$ gauge fields via an interaction of the form $\phi F\tilde F$. These pseudoscalar fields are commonly known as *axion-like-particles* (ALPs). As opposed to charged complex scalars described in [Scalar-Gauge Interactions](My first model of gauge fields.md), the pseudoscalar fields we consider here are not charged under a gauge symmetry. Note that while ALPs can also couple to non-Abelian gauge fields in nature (via $\phi G\tilde G$), currently only the Abelian $\mathrm{U}(1)$ interactions are implemented in the code. These models can be considered as an extension of the standard scalar and Abelian gauge dynamics: fields, program variables, input-file syntax, potential functions, and standard outputs follow the same conventions to a large extent, and we recommend the reader to be familiar with the [Scalar-Gauge Interactions](My first model of gauge fields.md) section of this manual first. In this section, we emphasize the additional details that arise from introducing this type of non-canonical coupling.

This section is structured as follows. We first introduce in [*Axion-gauge continuum dynamics*][subsec_ALPcontinuum] the continuum equations of motion for the axion and the $\mathrm{U}(1)$ gauge-field, as well as for the self-consistent expansion of the scale factor. In [*Model and input files for axion-$\mathrm{U}(1)$ theories*][subsec_ALPInput] we describe an example model and input parameters, and also explain how it can be modified to create a different model of axion-$\mathrm{U}(1)$ interactions. Finally, in [*The axion-$\mathrm{U}(1)$ physics inside $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*][sec_WhatHappensAutoinALP] we detail the axion-specific initialization and evolution algorithms.


### **Axion-gauge dynamics in the continuum** { #subsec_ALPcontinuum }

The action of an axion-$\mathrm{U}(1)$ gauge theory that can be simulated in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is the following:
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
where $m_\text{p}$ is the reduced Planck mass and $\alpha_{\Lambda}=m_p/\Lambda$ is the dimensionless axion-$\mathrm{U}(1)$ coupling strength, with $\Lambda$ representing an energy scale. Note that $F_{\mu\nu}$ is the standard field strength tensor of the $\mathrm{U}(1)$ gauge field $A_{\mu}$ defined in Eq. ([*5*](My first model of gauge fields.md#eq_FmnAbelian)) of [Scalar-Gauge Interactions](My first model of gauge fields.md), and that $\tilde{F}_{\mu\nu} = \frac{1}{2}\epsilon_{\mu\nu\rho\sigma}F^{\rho\sigma}$ is its dual counterpart, with $\epsilon_{\mu\nu\rho\sigma}$ being the completely antisymmetric Levi-Civita tensor, which in a curved spacetime takes the normalization $\epsilon_{0123} = 1/\sqrt{-g}$.


We use the same spatially-flat FLRW background metric introduced in Eq. ([*1*](My first model of (singlet) scalar fields.md#eq_FLRWmetric)) of [Scalar-Scalar interactions](My first model of (singlet) scalar fields.md). The variation of the action yields the continuum equations of motion for the axion and the gauge fields. Using cosmic time and working in the temporal gauge, $A_0=0$, they take the form
[](){ #eqn_eom_axion }
```math
\begin{align}
\label{eqn:eom1}
\ddot{\phi} &= -3H\dot{\phi}+\frac{1}{a^2}\nabla^2\phi-V(\phi)_{,\phi}+\frac{\alpha_\Lambda}{ m_pa^{3}}E_iB_i \, , \\
\label{eqn:eom2}
\dot{E}_i &= -HE_i-\frac{1}{a^2}\epsilon_{ijk}\partial_jB_k-\frac{\alpha_\Lambda}{am_p}\left(\dot{\phi}B_i-\epsilon_{ijk}\partial_j\phi E_k\right) \, ,
\end{align}
```
with the standard definitions of the electric and magnetic field from Eq. ([*8*](My first model of gauge fields.md#eq_ElectricMagneticDefs)) of [Scalar-Gauge Interactions](My first model of gauge fields.md). Additionally, the variation of the action with respect to the temporal component of the gauge field leads to the Gauss' law, which for this type of interactions takes the form
[](){ #eqn_Gauss }
```math
\begin{align}
\label{eqn:Gauss}
\partial_iE_i &= -\frac{\alpha_{\Lambda}}{m_pa}B_i\partial_i\phi \, . \quad\quad\quad {\rm [Gauss~Constraint]}
\end{align}
```

If we consider that the background expansion is driven by both the ALP and the gauge field, the Friedmann equations can be written as,
[](){ #eqn_Friedmann }
```math
\begin{align}
\ddot{a} &= -\frac{a}{3m_p^2}\big( E_{K} - E_{V} + E_{EM} \big)\,, \label{eqn:ddaAxion} \\
H^2 &= \frac{1}{3m_p^2}\big(E_{K} + E_{G} + E_{V} + E_{EM}\big)\,, \quad\quad\quad \text{[Hubble Constraint]} \label{eqn:HubbleAxion}
\end{align}
```
where, the various homogeneous components of the energy density are defined as in [Scalar-Gauge Interactions](My first model of gauge fields.md) for a non-charged scalar and an Abelian gauge field,
[](){ #eqn_energies }
```math
\begin{align}
E_{K} \equiv \frac{1}{2}\langle\dot{\phi}^2\rangle\;, \quad E_G \equiv \frac{1}{2a^2}\langle(\vec\nabla\phi)^2\rangle\;, \quad E_{V} \equiv \langle V(\phi) \rangle\;, \quad E_{EM} \equiv \frac{1}{2a^4}\langle a^2\vec E^2+\vec B^2\rangle\;. \label{eqn:energyDensityTerms}
\end{align}
```
Note that $E_K$, $E_G$, and $E_V$ refer to the kinetic, gradient, and potential energy densities of the axion, respectively, while $E_{EM}$ denotes the electromagnetic energy density associated with the Abelian gauge field $A_\mu$.


Regarding $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, we define program variables and $\alpha$-time exactly as in [Scalar-Gauge Interactions](My first model of gauge fields.md) and [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md). Thus, the matter dynamics and constraints to be simulated read as follows,
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
Furthermore, the self-consistent expansion is governed by the program-units Friedmann equations, given in Eqs.$~$([*24*](My first model of gauge fields.md#eq_FriedmannHubble)) and ([*25*](My first model of gauge fields.md#eq:#eq_FriedmannDDa)) of [Scalar-Gauge Interactions](My first model of gauge fields.md), which in turn contain the definitions given in Eq.$~$([*23*](My first model of gauge fields.md#eq:#eq_energy-contrib)) from the same section for the various energy components present.



### **Example model and input files for axion-$\mathrm{U}(1)$ theories** { #subsec_ALPInput }

An example model to simulate an ALP coupled to a $\mathrm{U}(1)$ gauge field is given by the $\texttt{axionU1_m2phi.h}$ model file. This contains one scalar singlet field, $N_\mathrm{s}=1$, which represents the axion-like particle, and one $\mathrm{U}(1)$ gauge field, $N_{\mathrm{U}(1)}=1$. The potential of this model is chosen to consist of a single term, $N_{\rm pot}=1$, given by
[](){ #eq_AxionExamplePotential }
```math
\begin{align}
\label{eq_AxionExamplePotential}
V(\phi)
&=
\frac{1}{2}m^2\phi^2 ,
\end{align}
```
where $m$ is the mass of the axion-like particle. In addition, the following program variables are chosen,
[](){ #eq_AxionExampleProgramVars }
```math
\begin{align}
\label{eq_AxionExampleProgramVars}
f_*=\phi_*~,\hspace{0.4cm}
\omega_*=m~,\hspace{0.4cm}
\alpha=0 .
\end{align}
```
with $\phi_*$ the initial homogeneous amplitude of the scalar field. The program potential then reads
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

To simulate this model, an example parameter file is provided, $\texttt{models/parameter-files/axionU1_m2phi2.in}$, which reads

@emgithub(models/parameter-files/axionU1_m2phi2.in)

The parameter syntax for lattice parameters, output frequencies and scalar initial conditions are the same as in Section [*Running the program with an input parameter file*](My first model of (singlet) scalar fields.md#subsec_Input-Scalars). The axion-gauge-specific additions and new restrictions are:

| **Parameters** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `alphaLambda_AxionU1` | Dimensionless coupling constant $\alpha_{\Lambda}$ between the axion-like particle and the $\mathrm{U}(1)$ gauge field. It defaults to $\alpha_\Lambda=1$ if unspecified. |
| `evolver` | Axion-gauge models  can only  be used with one of the Runge-Kutta evolvers, such as `RK2`, `RK3_3`, `RK3_4`, or `RK4_5`. |
| `tNonLinearAxionU1` | If a value between `t0` and `tmax` is specified for this variable, the evolution between the start of the simulation and this time will take place in the *linear regime*. Further details are provided in Section [*Linear Regime*][sec_evolution-axionU1_linear_regime]. |
| `flagChiralPS` | Setting this parameter to `true` triggers the generation of the $\texttt{spectra_chiral_U1_0.txt}$ and $\texttt{spectra_chiral_Elec_U1_0.txt}$ files, which contain the chiral spectra $A^{\pm}$ and $E^{\pm}$, respectively. More details are provided in the [*Output*][subsubsec_output] part of this section. |

In addition, this particular model requires the keyword `mass` that specifies the mass of the axion in GeV. In addition, initial conditions are provided via `initial_amplitudes` and `initial_momenta`, both of which contain only one entry corresponding to the axion field $\phi$.

#### Fixed background expansion

Axion-gauge models in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can also be run in a fixed-expanding background. This is activated by indicating

```text
expansion = true
fixedBackground = true
omegaEoS = ...
H0 = ...
```
in the parameter file. In such case, the scale factor is not sourced by the lattice fields, but by an external homogeneous component with constant equation of state $\omega_{\rm EoS}$, see Eq. ([*11*](My first model of (singlet) scalar fields.md#eq_ScaleFactorPowerLaw)) of [Scalar-Scalar interactions](My first model of (singlet) scalar fields.md).

#### Output files { #subsubsec_output }

An axion-$\mathrm{U}(1)$ simulation generates the standard output files for both the scalar-singlet (the axion) and the $\mathrm{U}(1)$ gauge field, following the default conventions described in [Scalar-Gauge Interactions](My first model of gauge fields.md), as well as two additional files related to the power spectra. The specific novelties and additional files for this module are:

*   **Gauss Constraint**: The Gauss constraint evaluation is adapted to the specific axion-$\mathrm{U}(1)$ expression given in Eq.$~$\eqref{eqn:Gauss_pralpha}. The file output format remains identical to the standard gauge setup.
*   **Energy Conservation**: When evolving within the *linear regime* (see Section [*Linear regime*][sec_evolution-axionU1_linear_regime]), the total energy density calculation used for the energy conservation check does not include the contribution from the gauge sector.
*   **Chiral Spectra**: If `flagChiralPS = true`, the module generates additional output files containing the chiral spectra of the gauge field and conjugate momentum. We now comment on how the chiral spectra are determined.

The procedure for computing the chiral power spectra of the gauge field and its conjugate momenta follows the standard methodology of computing power spectra in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, followed by an additional step consisting of the projection from the Cartesian basis, $(A_i, E_i)$, into the chiral basis, $(A^{\pm}, E^{\pm})$. This transformation is performed using a chiral projector, which is implicitly constructed in $\texttt{definitions/chiralpowerspectrum.h}$. The reader is referred to chapter 4 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{II}$ [@BaezaBallesteros_2025tme] for a detailed definition and mathematical construction of this chiral projector in both the continuum and the discrete lattice formulation.

The outputs files related to the chiral power spectra are:

<div class="grid cards cl-files" markdown>

-   `spectra_chiral_U1_0.txt`{ .cl-fname }

    Chiral power spectra of the $\mathrm{U}(1)$ gauge field, containing the columns:

    $\tilde{k}$, $\widetilde{\Delta}_{\widetilde{A}^{+}} (\tilde k)$, $\widetilde{\Delta}_{\widetilde{A}^{-}} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{\rm bin}$
    {: .cl-schema }

-   `spectra_chiral_Elec_U1_0.txt`{ .cl-fname }

    Chiral power spectra of the $\mathrm{U}(1)$ electric field, containing the columns:

    $\tilde{k}$, $\widetilde{\Delta}_{\widetilde{E}^{+}} (\tilde k)$, $\widetilde{\Delta}_{\widetilde{E}^{-}} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{\rm bin}$
    {: .cl-schema }

</div>


### **The axion-$\mathrm{U}(1)$ model file** { #sec_TheALPModelFile }

To define a new axion-$\mathrm{U}(1)$ model, one can use as a starting point the example model file $\texttt{models/axionU1_m2phi2.h}$. As of July 2026, the axion-gauge module of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ only allows for the simulation of a single scalar species coupled to a single Abelian gauge field, although this may change in the future. The field number is specified in the `ModelPars` structure within the parameter file, the interaction is also activated using the `CouplingsManager` class

@emgithub(models/axionU1_m2phi2.h:model_pars)

The definition of an `ScalarU1AxionCouplings` alias activates the axion-$\mathrm{U}(1)$ interactions. Its first template argument is the total number of scalar fields (`NScalars`), its second is the total number of $\mathrm{U}(1)$ gauge fields (`NU1Flds`), and the final boolean flag, if `true`, activates the axion-like interaction between them. Once this is set, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ will automatically simulate axion-gauge interactions as given in Eq.$~$\eqref{eq_AxionAction}.

!!! warning "Current scope of the axion-gauge module"
    As of version 2.0 of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, it is only possible to run axion-gauge simulations with a single scalar field. This will be extended in the future to make it possible to simulate multiple ALPs and also scalar species not coupled to the topological term. <!--While couplings to more scalar fields are It is possible to define a model with more than one scalar field (`NScalars > 1`); and by default, all included scalars will be coupled to the $\mathrm{U}(1)$ gauge field. Please be aware that the module has not been formally tested with more than one coupled scalar. If you wish to add an additional scalar field that is *not* coupled to the gauge sector, you must set the coupling flag to `false` for that specific field index. Please note that this configuration is also untested.-->

#### **The potential and its derivatives** { #sec_PotDerivsNMC }

In addition to specifying the number of fields and setting on the axion-gauge coupling, you also need to specify the model potential. For the provided example, the model only contains one term, Eq.$~$\eqref{eq_AxionExampleProgramPotential}, which is the inflationary potential:

@emgithub(models/axionU1_m2phi2.h:potential_terms)

Its first derivative is

@emgithub(models/axionU1_m2phi2.h:potential_derivs)

corresponding to
[](){ #eq_AxionExampleProgramPotentialDerivative }
```math
\begin{align}
\label{eq_AxionExampleProgramPotentialDerivative}
\widetilde V_{,\tilde\phi}=\tilde\phi,
\end{align}
```
and the second derivative reads

@emgithub(models/axionU1_m2phi2.h:potential_second_derivs)

where `OneType()` is a default class that returns one adapted to the particular context of its evaluation.

<!--The numbering of these functions must be consistent with the scalar-field numbering. In this example we just have `Tag<0>` of the inflaton/axion $\phi$.

This is a particular example of a potential, any other option can be implemented by changing the three entries above.-->



### **Axion-$\mathrm{U}(1)$ physics inside $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$** { #sec_WhatHappensAutoinALP }

#### Initial conditions { #sec_InitAxionU1 }

By default, simulations of ALPs initialize each chirality of the $\mathrm{U}(1)$ gauge field and its conjugate momenta (the electric field) using the Bunch-Davies (BD) solution, while the longitudinal components of both fields are set to zero. Consequently, the user does not need to specify any initialization parameters in the input parameter file for the Abelian gauge sector, as the BD initial conditions are automatically applied across the momentum range $[\tilde{k}_{\rm IR}, \tilde{k}_{\rm UV}]$.

To satisfy the initial Gauss constraint—which, under BD initial conditions for the gauge fields implies that the divergence of the electric field vanishes—the scalar field and its time derivative are set to be initially homogeneous. Their initial values are determined entirely by the standard parameters provided in the input file using the `initial_amplitudes` and `initial_momenta` keywords, for the amplitude of the field and its derivative, respectively. For a more detailed discussion on the theoretical background and the current implementation of these initial conditions, see [*Bunch-Davies transverse Abelian fields*](IC.md#subsubsec_BunchDaviesTransverseU1IC) in the [*Initial Conditions*](IC.md) section.

Following the initialization in the BD vacuum, it is recommended to evolve the fields in the linear regime to prevent the appearance of discretization effects related to UV modes of the BD spectrum. More details on this regime and how to control it are provided in [*Linear Regime*][sec_evolution-axionU1_linear_regime].

#### Evolution equations { #sec_evolution-axionU1 }

For self-consistent expansion, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ evolves the fields by solving a set of first-order differential equations. Defining the conjugate momenta
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
the discrete equations of motion are
[](){ #eq_coupledFirstDiffEqAxion }
```math
\begin{align}
\label{eq:coupledFirstDiffEqAxion}
\tilde{\phi}' &= a^{\alpha-3}\tilde{\pi}_\phi, & \tilde{\pi}_\phi' &= \mathcal{K}^{\rm L}_{\phi}[a,\tilde{\phi},\tilde{A}_j,a',\tilde{\pi}_{\phi},\left(\tilde{\pi}_{A}\right)_{j}]\,, \\
\tilde{A}_i' &= a^{\alpha-1}\left(\tilde{\pi}_{A}\right)_{i}, & \left(\tilde{\pi}_{A}\right)'_{i} &= \mathcal{K}^{\rm L}_{A_i}[a,\tilde{\phi},\tilde{A}_j,a',\tilde{\pi}_{\phi},\left(\tilde{\pi}_{A}\right)_{j}]\,, \\
a' &= b, & b' &= \mathcal{K}^{\rm L}_{a}[a,\tilde{E}^{\phi}_{K},\tilde{E}^{\phi}_{G},\tilde{E}^{\phi}_{V},\tilde{E}^{A}_{K},\tilde{E}^{A}_{G}]\,.
\end{align}
```
where the kernels for the fields are given by
[](){ #eq_explicitEOMKernels }
```math
\begin{align}
\mathcal{K}^{\rm L}_{\phi}={}&a^{1+\alpha} \sum_i \tilde{\nabla}_i^-\tilde{\nabla}_i^+ \tilde{\phi} - a^{\alpha+3}\widetilde V_{,\tilde\phi} + \left(\frac{\omega^2_{*}}{f_{*}m_p}\right)\alpha_\Lambda a^{\alpha-1}\sum_i \left(\tilde{\pi}_{A}\right)_{i}^{(2)}\tilde{B}_i^{(4)} \;, \label{eq:explicitEOMscalarKernelAlpha} \\
\mathcal{K}^{\rm L}_{A_i} ={}& - a^{\alpha-1} \sum_{j,k} \epsilon_{ijk} \tilde{\nabla}_j^- \tilde{B}_k - \left(\frac{f_{*}}{m_p}\right) \frac{\alpha_\Lambda a^{\alpha-3}}{2}\left(\tilde{\pi}_{\phi} \tilde{B}_i^{(4)} + \tilde{\pi}_{\phi,+i}\tilde{B}^{(4)}_{i,+i} \right) \nonumber \\
& + \left(\frac{f_{*}}{m_p}\right)\frac{\alpha_\Lambda a^{\alpha-1}}{4} \sum_\pm \sum_{j,k} \epsilon_{ijk}  \left\{ \left[ (\tilde{\nabla}_j^\pm \tilde{\phi}) \left(\tilde{\pi}_{A}\right)_{k,\pm j}^{(2)} \right]_{+i} +  \left[ (\tilde{\nabla}_j^\pm \tilde{\phi}) \left(\tilde{\pi}_A\right)_{k,\pm j}^{(2)}   \right] \right\} \;, \label{eq:explicitEOMgaugeKernelAlpha}
\end{align}
```

and the scale-factor kernel reads:
[](){ #eq_explicitScaleFactorKernelAlpha }
```math
\begin{align}
\label{eq:explicitScaleFactorKernelAlpha}
\mathcal{K}_{a}^{\rm L}[a,\tilde{E}^{\phi}_{K},\tilde{E}^{\phi}_{G},\tilde{E}^{\phi}_{V},\tilde{E}^{A}_{K},\tilde{E}^{A}_{G}] = \left(\frac{f_{*}}{m_{\text{p}}}\right)^2\frac{a^{2\alpha+1}}{3}\left[(\alpha-2)\tilde{E}^{\phi}_{K}+\alpha\tilde{E}^{\phi}_{G}+(\alpha+1)\tilde{E}^{\phi}_{V}+(\alpha-1)\tilde{E}^{A}_{K}+(\alpha-1)\tilde{E}^{A}_{G}\right]\,.
\end{align}
```

The matter kernels $\mathcal{K}_{\phi}^{\rm L}$ and $\mathcal{K}_{A_i}^{\rm L}$ represent the discrete lattice formulations that govern the dynamical evolution of the system. All terms not coupled to the axion-like parameter $\alpha_\Lambda$ follow the standard spatial discretization that strictly respects lattice gauge invariance. This default procedure can be reviewed in Section [Scalar-Gauge Interactions](My first model of gauge fields.md) or in $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl].

However, due to the chiral coupling between the axion-like particle and the Abelian gauge field, the lattice formulation must additionally capture the topological nature of the $\phi F\tilde{F}$ term while preserving the aforementioned gauge symmetries. To achieve this, we make use of the redefined discrete electric and magnetic fields, $\left(\tilde{\pi}_{A}\right)^{(2)}_i$ and $\tilde{B}^{(4)}_i$, which are defined at each lattice site $\mathbf{n}$ as:
[](){ #eq_DiscreteAveragedFields }
```math
\begin{align}
\label{eq_DiscreteAveragedFields}
\left(\tilde{\pi}_A\right)^{(2)}_i(\tilde{\eta}, \mathbf{n}) &\equiv \frac{1}{2}\left[ \left(\tilde{\pi}_A\right)_i(\tilde{\eta}, \mathbf{n}) + \left(\tilde{\pi}_A\right)_{i,-i}(\tilde{\eta}, \mathbf{n}) \right] \,, \\
\tilde{B}^{(4)}_i(\tilde{\eta}, \mathbf{n}) &\equiv \frac{1}{4}\left[ \tilde{B}_i(\tilde{\eta}, \mathbf{n}) + \tilde{B}_{i,-j}(\tilde{\eta}, \mathbf{n}) + \tilde{B}_{i,-k}(\tilde{\eta}, \mathbf{n}) + \tilde{B}_{i,-j-k}(\tilde{\eta}, \mathbf{n}) \right] \,,
\end{align}
```
where the negative subscripts indicate spatial shifts in the corresponding backward directions, and $j, k$ are the two spatial directions transverse to the component $i$ (such that $i, j, k$ are mutually orthogonal).

The use of these specifically averaged operators guarantees that:

*   The continuous topological properties and gauge invariances are preserved at the discrete level.
*   The standard second-order spatial accuracy, $\mathcal{O}(dx^2)$, is maintained.
*   Discrete Bianchi identities are satisfied.

For a comprehensive technical discussion of this lattice gauge-invariant implementation, the reader is referred to chapter 4 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{II}$ [@BaezaBallesteros_2025tme] or to the original paper where this lattice formulation was first introduced,  Ref$~$[@Figueroa_2017qmv] . Moreover, the user can find the implementation of the axion-gauge couplings used in the evolution kernels in $\texttt{definitions/axioncouplings.h}$, and the functions of the redefined electric and magnetic field in $\texttt{TempLat/lattice/algebra/axionalgebra/electricfield2.h}$ and $\texttt{TempLat/lattice/algebra/axionalgebra/magneticfield4.h}$ within  [`TempLat`](https://github.com/cosmolattice/templat).



As a final comment, we note that axion-$\mathrm{U}(1)$ interactions introduce an inherent dependence on the conjugate momenta $\left(\tilde{\pi}_{A}\right)_j$ directly inside the gauge-field kernel $\mathcal{K}_{A_i}^{\rm L}$. Because the evaluation of these kernels requires simultaneous knowledge of the fields and their respective conjugate momenta, standard symplectic algorithms such as `LF`, `VV`, or `PV` cannot be employed, and simulations will fail if they are indicated in the parameter file. Non-symplectic algorithms, such as the Runge-Kutta family, must be used instead, for example:
```text
evolver = RK2
```
Higher-order RK algorithms can be used if better time-integration accuracy is needed. <!--During the run, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ always monitors the Friedmann constraint using the total energy density, while the dynamical update of the scale factor is performed through the kernel equation defined above.-->


#### Linear Regime { #sec_evolution-axionU1_linear_regime }

The linear regime represents a specific simplified case of the full axion-gauge dynamics where the backreaction from the gauge sector onto the axion field is switched off. Operationally, this means the topological backreaction term proportional to $\sum_i \left(\tilde{\pi}_A\right)_i^{(2)}\tilde{B}_i^{(4)}$ is explicitly removed from the axion equation of motion. As a direct consequence of neglecting this backreaction, no spatial gradients are generated for the axion field, which is initially homogeneous. This causes the spatial Laplacian to vanish ($\sum_i \tilde{\nabla}_i^-\tilde{\nabla}_i^+ \tilde{\phi} = 0$) and, as a consequence, the discrete terms containing spatial derivatives of the axion field in the gauge field kernel become strictly zero
```math
\begin{align}
\sum_\pm \sum_{j,k} \epsilon_{ijk}  \left\{ \left[ (\tilde{\nabla}_j^\pm \tilde{\phi}) \left(\tilde{\pi}_{A}\right)_{k,\pm j}^{(2)} \right]_{+i} +  \left[ (\tilde{\nabla}_j^\pm \tilde{\phi}) \left(\tilde{\pi}_A\right)_{k,\pm j}^{(2)}   \right]\right\}=0\,.
\end{align}
```
Accounting for these simplifications, the discrete lattice kernels in the linear regime reduce to:
[](){ #eq_linearEOMKernels }
```math
\begin{align}
\mathcal{K}^{\rm L}_{\phi}={}& - a^{\alpha+3}\widetilde V_{,\tilde\phi} \;, \label{eq:linearEOMscalarKernel} \\
\mathcal{K}^{\rm L}_{A_i} ={}&  - a^{\alpha-1)} \sum_{j,k} \epsilon_{ijk} \tilde{\nabla}_j^- \tilde{B}_k - \left(\frac{f_{*}}{m_p}\right) \frac{\alpha_\Lambda a^{\alpha-3}}{2}\left(\tilde{\pi}_{\phi} \tilde{B}_i^{(4)} + \tilde{\pi}_{\phi,+i}\tilde{B}^{(4)}_{i,+i} \right) \;. \label{eq:linearEOMgaugeKernel}
\end{align}
```

Additionally, the contribution of the gauge sector to the cosmic expansion is completely neglected in the linear regime. The energy density of the gauge fields is omitted from both the dynamical evolution of the scale factor and the Hubble constraint. Since the axion field remains homogeneous (setting the axion gradient energy to zero, $\tilde{E}^{\phi}_{G} = 0$), the scale factor kernel simplifies to:
[](){ #eq_linearScaleFactorKernel }
```math
\begin{align}
\label{eq:linearScaleFactorKernel}
\mathcal{K}_{a}^{\rm L} = \left(\frac{f_{*}}{m_{\text{p}}}\right)^2\frac{a^{2\alpha+1}}{3}\left[(\alpha-2)\tilde{E}^{\phi}_{K}+(\alpha+1)\tilde{E}^{\phi}_{V}\right]\,.
\end{align}
```

The motivation for simulating linearly from an initial time `t0` up to `tNonLinearAxionU1`, and only then switching to the full non-linear dynamics until `tmax`, stems from the physical behavior of the initial state. In certain scenarios, the UV tail of the initial Bunch-Davies vacuum solution can unphysically dominate the early-time dynamics rather than remaining subdominant. Through this multi-stage procedure, instead, we allow the gauge modes to first undergo chiral excitation within the homogeneous scalar field background. Once this physical excitation grows to dominate over the unphysical UV tail, the full non-linear dynamics can be safely activated. Crucially, this transition must be triggered before the backreaction of the gauge sector becomes non-negligible.

Note that any preliminary analysis—such as determining the optimal time thresholds, assessing the impact of the UV tail for the specific range of momentum modes simulated on the lattice, etc.—is not performed automatically by $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. It is the user's responsibility to fix the `tNonLinearAxionU1` parameter and carry out these checks beforehand to ensure the correct and physical application of this hybrid evolution scheme.


