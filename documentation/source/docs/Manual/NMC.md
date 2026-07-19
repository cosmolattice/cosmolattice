<!-- <div style="text-align: justify;"> -->

CosmoLattice can evolve singlet scalar fields that are non-minimally coupled (NMC) to gravity, via an interaction of the form $\propto \phi^2R$ in the Jordan frame. This module can be considered as an extension of the scalar-singlet dynamics described in Section [Scalar-singlet interactions](My first model of (singlet) scalar fields.md): the scalar fields, program variables, input-file syntax, potential functions, and standard outputs follow the same conventions. In this section we go through what changes when one or more scalar singlets are non-minimally coupled to gravity.

This section is structured as follows. In Section [*Non-minimally coupled scalar dynamics*][subsec_NMCcontinuum] we introduce the continuum equations and the Ricci-scalar equation solved by the code. In Section [*Model and input files for NMC theories*][subsec_NMCInput] we describe an example model and NMC-specific input parameters. In Section [*The NMC model file*][sec_TheNMCModelFile] we show the model-file additions that activate the module. Finally, in Section [*The NMC physics inside CosmoLattice*][sec_WhatHappensAutoinNMC] we summarize the NMC-specific initialization and evolution logic.

### **Non-minimally coupled scalar dynamics** { #subsec_NMCcontinuum }

We work in the Jordan frame and keep the non-minimal coupling $\propto \phi^2R$ explicit. We keep the presentation compact, so for details on theoretical derivations we point the reader to Section II of Ref [@Figueroa_2021iwm]. For one non-minimally coupled scalar $\chi$, together with any additional minimally coupled matter sector, the relevant part of the action is
[](){ #eq_NMCaction }
```math
\begin{align}
\label{eq_NMCaction}
S =
\int d^{4}x \sqrt{-g} \left[
\frac{1}{2}m_p^2R
- \frac{1}{2}\xi R \chi^{2}
- \frac{1}{2} g^{\mu\nu}\partial_{\mu}\chi\partial_{\nu}\chi
- V(\chi,\{\varphi_{\rm m}\})
+ \mathcal{L}_{\rm m}
\right]  ,
\end{align}
```

where $m_p$ is the reduced Planck mass, $\xi$ is the dimensionless coupling strength of the non-minimal coupling, and $\mathcal{L}_{\rm m}$ describes kinetic terms of the remaining minimally-coupled scalar fields $\{\varphi_{\rm m}\}$ (which could be either singlet or charged scalars). We use the same spatially-flat FLRW background metric and $\alpha$-time convention introduced in Eq. ([*1*](My first model of (singlet) scalar fields.md#eq_FLRWmetric)) from Section [Scalar-singlet interactions](My first model of (singlet) scalar fields.md). Neglecting gravitational perturbations, the equation of motion of the non-minimally coupled field, is 
[](){ #eq_NMCeom }
```math
\begin{align}
\label{eq_NMCeom}
\chi''
-a^{-2(1-\alpha)}\nabla^2\chi
+(3-\alpha) \frac{a'}{a}\chi'
+ a^{2\alpha}\left(\xi \bar R \chi + V_{,\chi}\right) =0 ,
\end{align}
```

where $\bar R$ is the homogeneous Ricci scalar,
[](){ #eq_NMCricciMetric }
```math
\begin{align}
\label{eq_NMCricciMetric}
\bar R =
\frac{6}{a^{2\alpha}}
\left[
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
\right] .
\end{align}
```

The new ingredient with respect to the scalar-singlet system is that $\bar R$ must be evolved consistently along with the non-minimally and other minimally coupled species. In CosmoLattice this is done through the trace of the Einstein equations. The trace of the NMC scalar energy-momentum tensor is
[](){ #eq_NMCtraceScalar }
```math
\begin{align}
\label{eq_NMCtraceScalar}
T_{\chi}
=
\left(6\xi -1\right)
\left(
\partial^\mu\chi\partial_\mu\chi
+\xi R\chi^2
\right)
+6\xi\chi V_{,\chi}
-4V .
\end{align}
```

The traced Einstein equations give
[](){ #eq_NMCtraceEinstein }
```math
\begin{align}
\label{eq_NMCtraceEinstein}
R
=
-\frac{1}{m_p^2}
g^{\mu\nu}\left(T^\chi_{\mu\nu}+T^{\rm m}_{\mu\nu}\right)
=
-\frac{1}{m_p^2}\left(T_\chi+T_{\rm m}\right).
\end{align}
```

After volume averaging and solving for $\bar R$, one obtains
[](){ #eq_NMCricciTrace }
[](){ #eq_NMCFfactor }
```math
\begin{align}
\label{eq_NMCricciTrace}
\bar R
&=
\frac{F(\chi)}{m_p^2}
\left[
\left(1-6\xi\right)
\langle\partial^\mu\chi\partial_\mu\chi\rangle
+4\langle V\rangle
-6\xi\langle\chi V_{,\chi}\rangle
-\langle T_{\rm m}\rangle
\right],
\\
\label{eq_NMCFfactor}
F(\chi)
&\equiv
\frac{1}{
1+\left(6\xi-1\right)\xi\langle\chi^2\rangle/m_p^2
}.
\end{align}
```

Combining Eq.$~$\eqref{eq_NMCricciTrace} with Eq.$~$\eqref{eq_NMCricciMetric} gives the scale-factor evolution in the presence of NMC scalar-singlets,
[](){ #eq_NMCscaleFactorSecondOrder }
```math
\begin{align}
\label{eq_NMCscaleFactorSecondOrder}
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
=
\frac{a^{2\alpha}F(\chi)}{6m_p^2}
\left[
\left(1-6\xi\right)
\langle\partial^\mu\chi\partial_\mu\chi\rangle
+4\langle V\rangle
-6\xi\langle\chi V_{,\chi}\rangle
-\langle T_{\rm m}\rangle
\right] .
\end{align}
```
The homogeneous energy density of a single NMC scalar is also modified, thereofre new terms will arise due to this coupling which will have to be accounted in the total energy density
[](){ #eq_NMCEnergyDensityContinuum }
```math
\begin{align}
\label{eq_NMCEnergyDensityContinuum}
\bar\rho_{\chi}(\eta)
&=
\frac{1}{2a^{2\alpha}}\left\langle \chi'^{\,2}\right\rangle
+\frac{1}{2a^2}\left\langle \left(\nabla\chi\right)^2\right\rangle
+\left\langle V(\chi)\right\rangle
+\frac{3\xi}{a^{2\alpha}}\mathcal{H}^{2}
\left\langle \chi^2\right\rangle
+\frac{6\xi}{a^{2\alpha}}\mathcal{H}
\left\langle \chi\chi'\right\rangle
-\frac{\xi}{a^2}
\left\langle \nabla^2\chi^2\right\rangle ,
\hspace{0.7cm}
\mathcal{H}\equiv\frac{a'}{a}.
\end{align}
```
This expression fixes the NMC contribution to the averaged total energy density, which is used in the Friedmann constraint and in the energy-output columns described below. The final term is a total spatial derivative and drops out of volume averages for periodic boundary conditions.

The program variables are the scalar-singlet ones of Eq. ([*2*](My first model of (singlet) scalar fields.md#eq:FieldSpaceTimeNaturalVariables)), from Section [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md). No new spacetime or field rescalings are introduced by the NMC module; the only additional program quantity required is the dimensionless Ricci scalar
[](){ #eq_NMCRicciProgram }
```math
\begin{align}
\label{eq_NMCRicciProgram}
\widetilde{\bar R} \equiv \frac{\bar R}{\omega_*^2}.
\end{align}
```

### **Model and input files for NMC theories** { #subsec_NMCInput }

The example model $\texttt{lphi4NonMinimal.h}$ contains two scalar singlets, $N_s=2$. We identify $\phi_0\equiv\phi$ with the --  minimally coupled -- inflaton, and $\phi_1\equiv\chi$ with a non-minimally coupled scalar. The theory is
[](){ #eq_NMCExampleAction }
[](){ #eq_NMCExamplePotential }
```math
\begin{align}
\label{eq_NMCExampleAction}
S &=
\int d^4x\sqrt{-g}
\left[
\frac{1}{2}m_p^2R
-\frac{1}{2}\xi R\chi^2
-\frac{1}{2}\partial^\mu\phi\partial_\mu\phi
-\frac{1}{2}\partial^\mu\chi\partial_\mu\chi
-V(\phi,\chi)
\right] ,
\\
\label{eq_NMCExamplePotential}
V(\phi,\chi)
&=
\frac{\lambda}{4}\phi^4 .
\end{align}
```

Relative to the scalar-singlet example in Eq. ([*5*](My first model of (singlet) scalar fields.md#eq_potentialExampleI)), the direct interaction term $\frac{1}{2}g^2\phi^2\chi^2$ is absent. The $\chi$ field is sourced only by the curvature-induced term $\xi \bar R\chi$. The field equations in $\alpha$-time are
[](){ #eq_NMCExampleEOMphi }
[](){ #eq_NMCExampleEOMchi }
```math
\begin{align}
\label{eq_NMCExampleEOMphi}
\phi''
-a^{-2(1-\alpha)}\nabla^2\phi
+(3-\alpha)\frac{a'}{a}\phi'
&=
-a^{2\alpha}
\lambda\phi^3 ,
\\
\label{eq_NMCExampleEOMchi}
\chi''
-a^{-2(1-\alpha)}\nabla^2\chi
+(3-\alpha)\frac{a'}{a}\chi'
&=
-a^{2\alpha}
\xi\bar R\chi .
\end{align}
```

As in the quartic scalar-singlet model, we choose
[](){ #eq_NMCExampleProgramVars }
```math
\begin{align}
\label{eq_NMCExampleProgramVars}
f_*=\overline{\phi}_{*}~,\hspace{0.4cm}
\omega_*=\sqrt{\lambda}\,\overline{\phi}_{*}~,\hspace{0.4cm}
\alpha=1 .
\end{align}
```

The program potential therefore contains only one term,
[](){ #eq_NMCExampleProgramPotential }
```math
\begin{align}
\label{eq_NMCExampleProgramPotential}
\widetilde V(\tilde\phi,\tilde\chi)
\equiv
\frac{1}{f_*^2\omega_*^2}
V(f_*\tilde\phi,f_*\tilde\chi)
=
\frac{1}{4}\tilde\phi^4 .
\end{align}
```

The corresponding input file is $\texttt{models/parameter-files/lphi4NonMinimal.in}$:

@emgithub(models/parameter-files/lphi4NonMinimal.in)

The parameter syntax, <!-- command-line overrides,  --> lattice parameters, output frequencies, and scalar initial conditions are the same as in Section [*Running the program with an input parameter file*](My first model of (singlet) scalar fields.md#subsec_Input-Scalars). The NMC-specific additions are:

| **Parameters** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `xis` | Non-minimal coupling $\xi$ of each non-minimally coupled scalar field. In this model there is one NMC field, so one value is expected. |
| `evolver` | For self-consistent NMC expansion this must be one of the Runge-Kutta evolvers, such as `RK2`, `RK3_3`, `RK3_4`, or `RK4_5`. |
| `ext_PS1` | Optional external power spectrum for field `1`, the NMC scalar $\chi$. |
| `PS_type` | Convention used by the external power-spectrum file; see Section [*External power spectrum for scalar singlet initialization*](IC.md#subsubsec_ExternalPSSingletIC). |

Standard scalar-singlet parameters still apply. In particular, `initial_amplitudes` and `initial_momenta` contain two entries: the first one for $\phi$ and the second one for $\chi$. A typical command-line override is
```bash
./lphi4NonMinimal input=../models/parameter-files/lphi4NonMinimal.in N=64 xis=50
```

which runs the model on a $64^3$ lattice and sets $\xi=50$.

#### Fixed background expansion

NMC models can also be run in a fixed expanding background, analogously to the scalar-singlet setup around Eq. ([*11*](My first model of (singlet) scalar fields.md#eq_ScaleFactorPowerLaw)). This is activated with
```text
expansion = true
fixedBackground = true
omegaEoS = ...
H0 = ...
```

In that case, the scale factor is not sourced by the lattice fields, but by an external homogeneous component with constant equation of state $\omega_{\rm EoS}$. The NMC term still needs the background Ricci scalar, which CosmoLattice computes directly from the prescribed scale factor. The first line below gives the time-dependent Ricci scalar, while the second line is its value at the initial time $\tilde\eta_*$:
[](){ #eq_NMCFixedBackgroundRicci }
```math
\begin{align}
\label{eq_NMCFixedBackgroundRicci}
\tilde{\bar R}_{\rm fixed}(\tilde\eta)
&=
-6\tilde{\mathcal H}_*^2 p
\left[
1+
{\tilde{\mathcal H}_*\over p}
\left(\tilde\eta-\tilde\eta_*\right)
\right]^{-2\alpha p}
\left[1+p(\alpha-2)\right]
\left[
p+\tilde{\mathcal H}_*
\left(\tilde\eta-\tilde\eta_*\right)
\right]^{-2},
\\
\tilde{\bar R}_{\rm fixed,*}
&\equiv \tilde{\bar R}_{\rm fixed,*}(\tilde\eta = \tilde\eta_*) =  
-9\tilde{\mathcal H}_*^2
\left(\omega_{\rm EoS}-{1\over3}\right),
\end{align}
```

where $\tilde{\mathcal H}_* \equiv H_0/\omega_*$ and $p=2/[3(1+\omega_{\rm EoS})-2\alpha]$. The fixed-background Ricci scalar vanishes for radiation domination, $\omega_{\rm EoS}=1/3$, as expected.

#### Output files

An NMC run generates the usual scalar-singlet output files listed in Section [*Outputs*](My first model of (singlet) scalar fields.md#outputs). The NMC module adds the following information:

<div class="grid cards cl-files" markdown>

-   `average_energies.txt`{ .cl-fname }

    After the usual scalar kinetic, scalar gradient, and potential-energy columns, the file contains the two NMC energy-density contributions and their sum,

    $\tilde{\eta}$, $\tilde{E}_K^{(0)}$, $\tilde{E}_G^{(0)}$, ... , $\tilde{E}_K^{(N_s-1)}$, $\tilde{E}_G^{(N_s-1)}$, $\tilde{E}_V^{(0)}$, ... , $\tilde{E}_V^{(N_p-1)}$, $\tilde{E}_{\rm NMC}^{(1)}$, $\tilde{E}_{\rm NMC}^{(2)}$, $\tilde{E}_{\rm NMC}$, $\langle \tilde{\rho} \rangle$
    {: .cl-schema }

</div>

The two NMC columns are the averaged contributions
[](){ #eq_NMCOutputEnergyContributions }
```math
\begin{align}
\label{eq_NMCOutputEnergyContributions}
\tilde{E}_{\rm NMC}^{(1)}
&=
\sum_{I\in{\rm NMC}}
\frac{3\xi_I}{a^{2\alpha}}
\left(\frac{a'}{a}\right)^2
\left\langle \tilde\phi_I^2\right\rangle ,
\\
\tilde{E}_{\rm NMC}^{(2)}
&=
\sum_{I\in{\rm NMC}}
\frac{6\xi_I}{a^{2\alpha}}
\left(\frac{a'}{a}\right)
\left\langle \tilde\phi_I\tilde\phi_I'\right\rangle ,
\\
\tilde{E}_{\rm NMC}
&=
\tilde{E}_{\rm NMC}^{(1)}
+\tilde{E}_{\rm NMC}^{(2)} .
\end{align}
```

For the example model, the sum contains only $I=1$, with $\tilde\phi_1=\tilde\chi$. The last term in Eq.$~$\eqref{eq_NMCEnergyDensityContinuum} is a total spatial derivative and therefore vanishes in the volume average for periodic lattice boundary conditions; it is not printed as a separate averaged-energy column.

<div class="grid cards cl-files" markdown>

-   `average_scale_factor.txt`{ .cl-fname }

    In an expanding NMC simulation, the file contains one additional final column,

    $\tilde{\eta}$, $a$, $a'$, $a'/a$, $\widetilde{\bar R}$
    {: .cl-schema }

-   `average_energy_conservation.txt`{ .cl-fname }

    No separate NMC conservation file is produced. The usual Friedmann-constraint output is used, with the total energy density including the NMC contribution.

</div>

### **The NMC model file** { #sec_TheNMCModelFile }

To define a new NMC model, start from a scalar-singlet model file and add the NMC field assignment in the `ModelPars` structure. In $\texttt{models/lphi4NonMinimal.h}$ this declares that the second scalar, field index `1`, couples non-minimally to gravity:

@emgithub(models/lphi4NonMinimal.h:model_pars)

The `NonMinimalCouplings` typedef activates the NMC module. Its first template argument is the total number of scalar singlets, while the second one is the number of non-minimally coupled scalar fields. In this model, field index `1` is the NMC scalar $\chi$, while field index `0` is the minimally coupled inflaton $\phi$.

#### The potential and its derivatives { #sec_PotDerivsNMC }

The model has one non-zero program-potential term, Eq.$~$\eqref{eq_NMCExampleProgramPotential}, which is the inflationary potential:

@emgithub(models/lphi4NonMinimal.h:potential_terms)

where its first derivative is

@emgithub(models/lphi4NonMinimal.h:potential_derivs)

corresponding to
[](){ #eq_NMCProgramPotentialDerivative }
```math
\begin{align}
\label{eq_NMCProgramPotentialDerivative}
\widetilde V_{,\tilde\phi}=\tilde\phi^3,
\hspace{0.7cm}
\widetilde V_{,\tilde\chi}=0 .
\end{align}
```

**The curvature term $\xi\widetilde{\bar R}\tilde\chi$ is not entered as part of the potential. It is supplied automatically by the NMC evolution kernel once the model declares `NonMinimalCouplings`**.

The second derivative needed for the initial inflaton effective mass is

@emgithub(models/lphi4NonMinimal.h:potential_second_derivs)

The numbering of these functions must be consistent with the scalar-field numbering. `Tag<0>` refers to the inflaton $\phi$. The NMC scalar $\chi$ is field `1`, which in our example model has no potential term.

### **The NMC physics inside CosmoLattice** { #sec_WhatHappensAutoinNMC }

#### Initialization of fluctuations { #sec_InitNMC }

Any scalar singlet can be initialized from an external spectrum, following the prescription described in Section [*External power spectrum for scalar singlet initialization*](IC.md#subsubsec_ExternalPSSingletIC). The NMC-specific use case is to apply this mechanism to the non-minimally coupled scalar. For the present model this is done with
```text
ext_PS1 = path/to/spectrum.dat
```

because in $\texttt{lphi4NonMinimal.h}$ field `1` is $\chi$. The corresponding inflaton parameter is `ext_PS0`; it can remain `none` if the inflaton fluctuations use the default scalar prescription. The `PS_type` value must match the convention used to produce the external file, and the momenta in that file must be expressed in units compatible with $\omega_*$. This is useful when the NMC spectator fluctuations have first been evolved in the linear regime and are then passed to the nonlinear lattice simulation.

#### Evolution equations { #eq_evolution-NMC }

For self-consistent expansion, CosmoLattice evolves the fields by solving a set of first-order differential equations. Defining the conjugate momenta
[](){ #eq_NMCmomentumChi }
```math
\begin{align}
\label{eq_NMCmomentumChi}
\tilde\pi_\phi = a^{3-\alpha}\tilde\phi',
\hspace{0.6cm}
\tilde\pi_\chi = a^{3-\alpha}\tilde\chi',
\hspace{0.6cm}
b=a^{1-\alpha}a',
\end{align}
```

the schematic equations of motion are
[](){ #eq_HamiltonEOMNMC }
```math
\begin{align}
\label{eq_HamiltonEOMNMC}
\tilde\phi'
&=
a^{\alpha-3}\tilde\pi_\phi,
&
\tilde\pi_\phi'
&=
\mathcal{K}^{\rm L}_\phi[a,\tilde\phi],
\\
\tilde\chi'
&=
a^{\alpha-3}\tilde\pi_\chi,
&
\tilde\pi_\chi'
&=
\mathcal{K}^{\rm L}_\chi[a,\tilde\chi,\widetilde{\bar R}],
\\
a'
&=
a^{\alpha-1}b,
&
b'
&=
\mathcal{K}^{\rm L}_a[a,\widetilde{\bar R}] .
\end{align}
```

For the example model the lattice kernels are
[](){ #eq_NMClatticeKernelChi }
```math
\begin{align}
\label{eq_NMClatticeKernelChi}
\mathcal{K}^{\rm L}_\phi
&=
a^{1+\alpha}\sum_i\widetilde\nabla_i^-\widetilde\nabla_i^+\tilde\phi
-a^{3+\alpha}\tilde\phi^3,
\\
\mathcal{K}^{\rm L}_\chi
&=
a^{1+\alpha}\sum_i\widetilde\nabla_i^-\widetilde\nabla_i^+\tilde\chi
-a^{3+\alpha}\xi\widetilde{\bar R}\tilde\chi,
\\
\mathcal{K}^{\rm L}_a
&=
\frac{a^{2+\alpha}}{6}\widetilde{\bar R}.
\end{align}
```

As in the case we are considering, $c.f.$ Eq.$~$\eqref{eq_NMCExampleAction}, only $\chi$ is non-minimally coupled and $\widetilde V=\tilde\phi^4/4$, the Ricci scalar entering the above kernels is
[](){ #eq_NMCRicciProgramSpecificModel }
```math
\begin{align}
\label{eq_NMCRicciProgramSpecificModel}
\widetilde{\bar R}
=
\left(\frac{f_*}{m_p}\right)^2
\frac{
2(1-6\xi)\left\langle \widetilde G_\chi-\widetilde K_\chi\right\rangle
+2\left\langle \widetilde G_\phi-\widetilde K_\phi\right\rangle
+4\left\langle \widetilde V\right\rangle
}{
1+\left(6\xi-1\right)\xi
\left(\frac{f_*}{m_p}\right)^2
\left\langle \tilde\chi^2\right\rangle
},
\end{align}
```

where
[](){ #eq_NMCRicciProgramSpecificEnergies }
```math
\begin{align}
\label{eq_NMCRicciProgramSpecificEnergies}
\widetilde K_f
=
\frac{1}{2a^{2\alpha}}\tilde f'^{\,2},
\hspace{0.6cm}
\widetilde G_f
=
\frac{1}{2a^2}\sum_i(\widetilde\nabla_i\tilde f)^2,
\hspace{0.6cm}
f\in\{\phi,\chi\}.
\end{align}
```

!!! note "Important Note"
    In self-consistent NMC expansion, $\widetilde{\bar R}$ depends on volume averages involving the fields and their conjugate momenta. Consequently, the NMC kernels depend on conjugate momenta through $\widetilde{\bar R}$. Symplectic algorithms such as `LF`, `VV`, or `PV`, should therefore not be used for self-consistent NMC evolution. Non-symplectic algorithms, such as Runge-Kutta, should be used instead.

The self-consistent NMC module can be evolved with the Runge-Kutta family of algorithms implemented in CosmoLattice, for example
```text
evolver = RK2
```

Higher-order RK algorithms can be used if better time-integration accuracy is needed. During the run, CosmoLattice always monitors the Friedmann constraint using the total energy density, including the NMC energy contribution, but the dynamical update of the scale factor is performed through the Ricci-scalar equation above.
