<!-- <div style="text-align: justify;"> -->

Here we discuss scalar field dynamics with a non-minimal coupling (NMC) to gravity. We first write the continuum equations for a single real scalar field coupled to the Ricci scalar through a term proportional to $\xi R\phi^2$, where $\xi$ is a dimensionless coupling constant and $R$ is the Ricci scalar. In the explicit example model, this NMC scalar is denoted by $\chi$, while $\phi$ is kept as a minimally coupled inflaton.

This section is structured as follows. In Section [*The continuum theory*][sec_NMCcontinuum], we first introduce the equations of motion in the continuum for both the matter sector and the scale factor, and recall the corresponding *program variables*. In Section [*NMC model*][sec_NMCModel] we present an example model and define its corresponding program variables and potential specific to it. We then explain in Section [*My first NMC run*][sec_MyFirstNMCRun] how to compile and run the code. After that, in Section [*The model file*][sec_TheNMCModelFile] we walk the user through the *model file*, where the model details are actually implemented. Finally, in Section [*The NMC physics implemented in CosmoLattice*][sec_WhatHappensAutoinNMC] we provide a summarized picture of what happens 'under the hood', noting the external-spectrum initialization of the NMC field and the Runge-Kutta evolution used by this module.

### The continuum theory { #sec_NMCcontinuum }

In this section we work directly in the Jordan frame, keeping the non-minimal coupling explicit. We consider a scalar field $\phi$ non-minimally coupled to gravity, together with a generic matter sector denoted by $\{\varphi_{\rm m}\}$. The action is
[](){ #eq_NMCaction }
```math
\begin{align}
S =
\int d^{4}x \sqrt{-g} \left[
\frac{1}{2}m_p^2R
- \frac{1}{2}\xi R \phi^{2}
- \frac{1}{2} g^{\mu\nu}\partial_{\mu}\phi\partial_{\nu}\phi
- V(\phi,\{\varphi_{\rm m}\})
+ \mathcal{L}_{\rm m}
\right] . \tag{87}
\end{align}
```

Here $m_p$ is the reduced Planck mass, $V(\phi,\{\varphi_{\rm m}\})$ is the scalar potential, and $\mathcal{L}_{\rm m}$ contains the remaining matter fields. The parameter $\xi$ controls the strength of the non-minimal coupling. We neglect gravitational perturbations and take the background metric to be the spatially flat FLRW metric written in $\alpha$-time,
[](){ #eq_NMCFLRWmetric }
```math
\begin{align}
ds^2 = -a(\eta)^{2\alpha}d\eta^2 + a(\eta)^2\delta_{ij}dx^idx^j . \tag{88}
\end{align}
```

!!! note
    As in the rest of the manual, primes denote derivatives with respect to the $\alpha$-time variable $\eta$. The choices $\alpha=0$ and $\alpha=1$ correspond to cosmic time and conformal time, respectively.

In this background, the equation of motion of the NMC scalar field reads
[](){ #eq_NMCeom }
```math
\begin{align}
\phi'' +(3-\alpha) \frac{a'}{a}\phi'
- a^{-2(1-\alpha)}\nabla^2\phi
+ a^{2\alpha}\left(\xi \bar R \phi + \frac{\partial V}{\partial \phi}\right) =0 , \tag{89}
\end{align}
```
where $\bar R$ is the background Ricci scalar,
[](){ #eq_NMCricciMetric }
```math
\begin{align}
\bar R =
\frac{6}{a^{2\alpha}}
\left[
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
\right] . \tag{90}
\end{align}
```
In practice, it is convenient to obtain the background Ricci scalar from the trace of the Einstein equations. The trace of the NMC scalar energy-momentum tensor is
[](){ #eq_NMCtraceScalar }
```math
\begin{align}
T_{\phi}
=
\left(6\xi -1\right)
\left(
\partial^\mu\phi\partial_\mu\phi
+\xi R\phi^2
\right)
+6\xi\phi\frac{\partial V}{\partial\phi}
-4V . \tag{91}
\end{align}
```

The traced Einstein equations give
[](){ #eq_NMCtraceEinstein }
```math
\begin{align}
R
=
-\frac{1}{m_p^2}
g^{\mu\nu}\left(T^\phi_{\mu\nu}+T^{\rm m}_{\mu\nu}\right)
=
-\frac{1}{m_p^2}\left(T_\phi+T_{\rm m}\right). \tag{92}
\end{align}
```
After volume averaging and solving for $\bar R$, we obtain
[](){ #eq_NMCricciTrace }
```math
\begin{align}
\bar R
=
\frac{F(\phi)}{m_p^2}
\left[
\left(1-6\xi\right)
\langle\partial^\mu\phi\partial_\mu\phi\rangle
+4\langle V\rangle
-6\xi\langle\phi V_{,\phi}\rangle
-\langle T_{\rm m}\rangle
\right], \tag{93}
\end{align}
```
with
[](){ #eq_NMCFfactor }
```math
\begin{align}
F(\phi)
\equiv
\frac{1}{
1+\left(6\xi-1\right)\xi\langle\phi^2\rangle/m_p^2
}. \tag{94}
\end{align}
```

Combining this result with Eq. ([*90*][eq_NMCricciMetric]) gives a differential equation for the scale factor,
[](){ #eq_NMCscaleFactorSecondOrder }
```math
\begin{align}
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
=
\frac{a^{2\alpha}F(\phi)}{6m_p^2}
\left[
\left(1-6\xi\right)
\langle\partial^\mu\phi\partial_\mu\phi\rangle
+4\langle V\rangle
-6\xi\langle\phi V_{,\phi}\rangle
-\langle T_{\rm m}\rangle
\right]. \tag{95}
\end{align}
```
This equation is solved simultaneously with the NMC scalar equation of motion and the equations of motion of the remaining matter fields.

!!! note "Program Variables"
    <!-- **Important:** --> On the lattice, we operate with a particular set of dimensionless field and spacetime variables, $\{\tilde{\phi},\tilde{\eta},\tilde{x}^i\}$, which we call **program variables**. The transformation from 'physical' to program variables, $t \rightarrow \tilde{\eta}$, $x^i \rightarrow \tilde{x}^i$, and $\phi \rightarrow \tilde{\phi}$, is given by the following relations
    [](){ #eq_NMCFieldSpaceTimeNaturalVariables }

    ```math

    \begin{align}
    \tilde\phi \equiv {\frac{\phi}{f_*}} ,  d\tilde\eta \equiv a^{- \alpha}  \omega_* dt ,  d\tilde x^i \equiv \omega_* dx^i . \tag{96}
    \end{align}

    ```

    where $f_*$ and $\omega_*$ are two constants with dimensions of energy. Program variables, and any quantity defined in terms of them, will be tagged in this manual with the *diacritic* symbol '$\sim$'. The program variable re-scalings for space and time variables, $d\tilde\eta, d\tilde x^i$, will be universal in CosmoLattice, independently of the matter field content. Program variable re-scalings for matter fields other than singlet scalar fields, will be introduced later on, when dealing *e.g.* with gauge fields or fluid dynamics.


### NMC model { #sec_NMCModel }

We consider an illustrative two-field model, closely related to the scalar singlet example discussed in Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md). The field $\phi$ is a minimally-coupled inflaton with quartic potential, while the second scalar $\chi$ is non-minimally coupled to gravity. There is no direct potential coupling between $\phi$ and $\chi$; the field $\chi$ feels the background dynamics through the curvature-induced term $\xi R\chi^2$. The action is
[](){ #eq_NMCExampleAction }
[](){ #eq_NMCExamplePotential }
```math
\begin{align}
S &=
\int d^4x\sqrt{-g}
\left[
\frac{1}{2}m_p^2R
-\frac{1}{2}\xi R\chi^2
-\frac{1}{2}\partial^\mu\phi\partial_\mu\phi
-\frac{1}{2}\partial^\mu\chi\partial_\mu\chi
-V(\phi,\chi)
\right] , \tag{97}
\\
V(\phi,\chi)
&=
\frac{\lambda}{4}\phi^4 . \tag{98}
\end{align}
```

In this example there are $N_s=2$ scalar fields. We identify $\phi_0\equiv\phi$ and $\phi_1\equiv\chi$. The only non-zero potential term is the quartic potential of the inflaton. The NMC coupling affects only the second scalar field $\chi$, through the term $-\frac12\xi R\chi^2$.

The field equations in $\alpha$-time are
[](){ #eq_NMCExampleEOM }
```math
\begin{align}
\phi''
-a^{-2(1-\alpha)}\nabla^2\phi
+(3-\alpha)\frac{a'}{a}\phi'
&=
-a^{2\alpha}
\lambda\phi^3, \tag{99}
\\
\chi''
-a^{-2(1-\alpha)}\nabla^2\chi
+(3-\alpha)\frac{a'}{a}\chi'
&=
-a^{2\alpha}
\xi\bar R\chi . \tag{100}
\end{align}
```
The equation for $\chi$ is sourced only by the curvature-induced mass term $\xi\bar R\chi$. The scale factor is evolved with the trace-based Ricci scalar equation described in Section [*The continuum theory*][sec_NMCcontinuum], with $\chi$ playing the role of the non-minimally coupled field.

For this example we use the same program-variable normalization as in the canonical $\lambda\phi^4$ model,
[](){ #eq_NMCExampleProgramVars }
```math
\begin{align}
f_*=\overline{\phi}_*,
\hspace{0.5cm}
\omega_*=\sqrt{\lambda}\,\overline{\phi}_*,
\hspace{0.5cm}
\alpha=1 . \tag{101}
\end{align}
```
Here $\overline{\phi}_*$ is the initial homogeneous amplitude of the inflaton. With this choice, the program potential is
[](){ #eq_NMCExampleProgramPotential }
```math
\begin{align}
\widetilde V(\tilde\phi,\tilde\chi)
\equiv
\frac{1}{f_*^2\omega_*^2}
V(f_*\tilde\phi,f_*\tilde\chi)
=
\frac{1}{4}\tilde\phi^4 . \tag{102}
\end{align}
```
The model file implements this expression directly in program units.

### My first NMC run { #sec_MyFirstNMCRun }

CosmoLattice comes with a ready-to-run implementation of the model above in the file $\texttt{models/lphi4NonMinimal.h}$. We now show how to compile and run it, and how to pass the required NMC parameters to the simulation.

#### Compilation

First choose a compilation directory. For example, starting from the root folder of CosmoLattice,
```bash
cd cosmolattice
mkdir build
cd build
```

The model $\texttt{lphi4NonMinimal.h}$ is compiled by passing its name to CMake,
```bash
cmake -DMODEL=lphi4NonMinimal ../
make cosmolattice
```

The last argument of the `cmake` command is the path to the CMake configuration file, here `../`. The option `-DMODEL=lphi4NonMinimal` tells CMake to compile the model file $\texttt{models/lphi4NonMinimal.h}$. If the compilation succeeds, the executable is named $\texttt{lphi4NonMinimal}$.

!!! note "**Important Note**"
    Every time you call CMake with a different model, it is a good practice to first remove the previously generated $\texttt{CMakeCache.txt}$ file.

#### Running the program with an input parameter file { #subsec_Input-NMC }

The corresponding input file is $\texttt{models/parameter-files/lphi4NonMinimal.in}$. A first run can be launched as
```bash
./lphi4NonMinimal input=../models/parameter-files/lphi4NonMinimal.in
```

This launches $\texttt{lphi4NonMinimal}$ with the parameters specified in the input file. Let us display the full file:

$\texttt{models/parameter-files/lphi4NonMinimal.in}$

@emgithub(models/parameter-files/lphi4NonMinimal.in)

The structure of the input file is the same as in the scalar singlet example. Single-value parameters are written as `parameterName = value`, while parameters that take several values are written as
```text
parameterName = value1 value2 value3 ...
```
The character `#` starts a comment. Parameters passed directly through the command line override those specified in the input file. For example,
```bash
./lphi4NonMinimal input=../models/parameter-files/lphi4NonMinimal.in N=64 xis=50
```
runs the same model with a $64^3$ lattice and with the NMC coupling changed to $\xi=50$.

The NMC-specific parameter is `xis`, which stores the non-minimal gravitational couplings of the non-minimally coupled scalar fields. In this example the model declares only one NMC scalar field, $\chi$, so the file contains a single value,
```text
xis = 100.0
```
If a model declared several non-minimally coupled fields, `xis` would contain one value per NMC field, separated by spaces.

For this example, the most relevant parameters are:

| **Parameter** | **Explanation** |
| --- | --- |
| `N` | Number of lattice points per dimension. |
| `kIR` | Infrared cutoff of the lattice in program units, $\tilde k_{\rm IR}=k_{\rm IR}/\omega_*$. |
| `dt` | Time step in program units, $\delta\tilde\eta$. |
| `tMax` | Final simulation time in program units. |
| `tOutputFreq` | Time interval between frequent outputs. |
| `tOutputInfreq` | Time interval between infrequent outputs, such as spectra. |
| `expansion` | If `true`, the scale factor evolves self-consistently. |
| `fixedBackground` | If `true`, the scale factor is not evolved self-consistently, but fixed by an external power-law background. |
| `omegaEoS` | Barotropic equation-of-state parameter $\omega_{\rm EoS}=p/\rho$ of the external component sourcing the fixed background expansion. |
| `H0` | Initial Hubble rate in GeV used for fixed background expansion. |
| `evolver` | Evolution algorithm. For self-consistent NMC expansion, the NMC kernels depend on momenta through $\tilde{\bar R}$, so the simulation must be evolved with the Runge-Kutta family implemented in CosmoLattice, e.g. `RK2`, `RK3_3`, `RK3_4`, or `RK4_5`. |
| `xis` | Non-minimal coupling $\xi$ of the NMC scalar field $\chi$. |
| `initial_amplitudes` | Initial homogeneous field amplitudes for $\phi$ and $\chi$, in GeV. |
| `initial_momenta` | Initial homogeneous field momenta for $\phi$ and $\chi$, in GeV$^2$. |
| `lambda` | Inflaton quartic self-coupling $\lambda$. |
| `ext_PS0`, `ext_PS1` | Optional external initial power spectra for fields 0 and 1. In this NMC example, `ext_PS1` initializes the non-minimally coupled field $\chi$; `ext_PS0 = none` leaves the inflaton initialized with the default scalar prescription. |
| `PS_type` | Power-spectrum convention, called `PSType` internally; see Section [*External power spectrum for scalar singlet initialization*](IC.md#subsubsec_ExternalPSSingletIC). |

!!! note
    In this model, `initial_amplitudes` and `initial_momenta` must contain two entries. The first entry corresponds to the minimally coupled inflaton $\phi$, and the second one to the non-minimally coupled field $\chi$.

!!! note
    The line `ext_PS1 = none` in the example input file is a placeholder. To initialize the NMC field with an external power spectrum, replace `none` by the path to the spectrum file. The corresponding inflaton parameter is `ext_PS0`; it can remain `none` if the inflaton fluctuations are initialized with the default prescription.

#### Fixed background expansion

NMC models can also be run in a fixed background expansion, as in the scalar singlet case discussed around Eq. ([*37*][eq_ScaleFactorPowerLaw]). In this setup, the scale factor is prescribed by an external homogeneous component with constant equation of state $\omega_{\rm EoS}$, rather than being sourced self-consistently by the lattice fields. This is activated with
```text
expansion = true
fixedBackground = true
omegaEoS = ...
H0 = ...
```
where `H0` is the initial Hubble rate in GeV. CosmoLattice converts it to program units as $\tilde{\mathcal H}_* \equiv H_0/\omega_*$.

The fixed background scale factor is
[](){ #eq_NMCFixedBackgroundScaleFactor }
```math
\begin{align}
a(\tilde\eta)
=
\left[
1+
{\tilde{\mathcal H}_*\over p}
\left(\tilde\eta-\tilde\eta_*\right)
\right]^p,
\hspace{0.6cm}
{\mathcal H}(\tilde\eta)
\equiv
{a'\over a}
=
{\tilde{\mathcal H}_*\over
1+
{\tilde{\mathcal H}_*\over p}
\left(\tilde\eta-\tilde\eta_*\right)},
\hspace{0.6cm}
p =
{2\over 3(1+\omega_{\rm EoS})-2\alpha}.
\end{align}
```
For NMC evolution, CosmoLattice also needs the background Ricci scalar appearing in the curvature-induced mass term. In a fixed background run it is not computed from the trace of the lattice energy-momentum tensor, but directly from the prescribed scale factor:
[](){ #eq_NMCFixedBackgroundRicci }
```math
\begin{align}
\tilde{\bar R}_{\rm fixed}(\tilde\eta)
=
\left[
-6\tilde{\mathcal H}_*^2 p
\left[
1+
{\tilde{\mathcal H}_*\over p}
\left(\tilde\eta-\tilde\eta_*\right)
\right]^{-2\alpha p}
\left[1+p(\alpha-2)\right]
\right]
\left[
p+\tilde{\mathcal H}_*
\left(\tilde\eta-\tilde\eta_*\right)
\right]^{-2}.
\end{align}
```
At the initial time this reduces to
```math
\begin{align}
\tilde{\bar R}_{\rm fixed}(\tilde\eta_*)
=
-9\tilde{\mathcal H}_*^2
\left(\omega_{\rm EoS}-{1\over3}\right),
\end{align}
```
so the fixed-background Ricci scalar vanishes for a radiation-dominated background, $\omega_{\rm EoS}=1/3$, as expected.

#### Outputs

An NMC run generates the same standard output files as a scalar singlet simulation, described in Section [*Outputs*](My first model of (singlet) scalar fields.md#outputs). In addition, when the model declares non-minimally coupled scalar fields, the following NMC-specific quantities are added to the standard averaged output:

-  `average_energies.txt`: The energy-density file contains the usual scalar kinetic, scalar gradient, and potential-energy columns, followed by the NMC contribution before the total energy. For a model with $N_s$ scalar singlets and $N_p$ potential terms, the columns are

$\hspace{1cm}$ $\tilde{\eta}$, $\tilde{E}_K^{(0)}$, $\tilde{E}_G^{(0)}$, ... , $\tilde{E}_K^{(N_s-1)}$, $\tilde{E}_G^{(N_s-1)}$, $\tilde{E}_V^{(0)}$, ... , $\tilde{E}_V^{(N_p-1)}$, $\tilde{\rho}_{\rm NMC}^{(1)}$, $\tilde{\rho}_{\rm NMC}^{(2)}$, $\tilde{\rho}_{\rm NMC}$, $\langle \tilde{\rho} \rangle$.

Here $\tilde{\rho}_{\rm NMC}^{(1)}$ and $\tilde{\rho}_{\rm NMC}^{(2)}$ are the two volume-averaged non-minimal energy-density contributions generated by the $\xi R\phi^2$ coupling, and $\tilde{\rho}_{\rm NMC}$ is their sum. The total energy density $\langle \tilde{\rho} \rangle$ includes this NMC contribution.

-  `average_scale_factor.txt`: In an expanding NMC simulation, the scale-factor file contains one additional column after the standard scale-factor quantities:

$\hspace{1cm}$ $\tilde{\eta}$, $a$, $a'$, $a'/a$, $\tilde{\bar R}$.

The last column is the volume-averaged background Ricci scalar in program units. This is the same quantity entering the curvature-induced mass term of the NMC scalar field.

-  `average_energy_conservation.txt`: No separate NMC conservation file is produced. The usual Friedmann-constraint output is still used, but its right-hand side is computed with the total energy density including $\tilde{\rho}_{\rm NMC}$.

### The model file { #sec_TheNMCModelFile }

To define a new NMC model, the main file to modify is the model file specified through the `-DMODEL=...` CMake argument. In the example above this is $\texttt{models/lphi4NonMinimal.h}$. We now walk through the relevant parts of this file.

#### Definition and declaration of the model { #subsubsec_DefAndDeclModelNMC }

The first step is to specify the matter content of the theory. In this example, the model has two scalar fields. It also declares that only one scalar field is non-minimally coupled to gravity:
@emgithub(models/lphi4NonMinimal.h:model_pars)

The `NonMinimalCouplings` typedef in the snippet above is the part that activates the NMC module. The first template argument is the total number of scalar fields, while the second one is the number of non-minimally coupled scalar fields. In this model, the declared NMC scalar is field index `1`, corresponding to $\chi$, while the inflaton field, index `0`, remains minimally coupled.

The model name is then defined as
@emgithub(models/lphi4NonMinimal.h:model_name)

As usual, **the model name must match the file name without the `.h` extension**. The generic model skeleton is generated with
@emgithub(models/lphi4NonMinimal.h:make_model)

and the customized model class derives from it:
@emgithub(models/lphi4NonMinimal.h:class_declaration)

#### Setting up the model { #sec_SettingNMCModel }

The model-specific parameters are declared as private members,
@emgithub(models/lphi4NonMinimal.h:private_members)

Here $\lambda$ is the inflaton self-coupling.

The constructor reads the model parameters from the input file or from the command line:
@emgithub(models/lphi4NonMinimal.h:constructor_params)

Calling `parser.get<double>("lambda")` declares `lambda` as a required input parameter of type `double`.

The homogeneous initial conditions are read as
@emgithub(models/lphi4NonMinimal.h:initial_conditions)

The second template argument, `2`, indicates that two values are expected, one for each scalar field. The parameter `initial_amplitudes` is mandatory, while `initial_momenta` is optional and defaults to `{0, 0}`.

The program-variable normalization is fixed by
@emgithub(models/lphi4NonMinimal.h:rescaling)

This implements Eq. ([*101*][eq_NMCExampleProgramVars]). The code uses the initial inflaton amplitude as $f_*$ and the characteristic quartic oscillation frequency as $\omega_*$.

Finally, the model initializes the potential energy and effective masses from the potential derivatives:
@emgithub(models/lphi4NonMinimal.h:masses_setup)

This evaluates the potential and its second derivatives on the homogeneous initial configuration. These quantities are used, among other things, to initialize the expansion rate and the field fluctuation spectra.

#### The potential and its derivatives { #sec_PotDerivsNMC }

The only non-zero program potential term is the inflaton quartic potential,
@emgithub(models/lphi4NonMinimal.h:potential_terms)

This implements Eq. ([*102*][eq_NMCExampleProgramPotential]). The object `fldS` contains the scalar fields. The expression `fldS(0_c)` refers to the inflaton $\tilde\phi$, while `fldS(1_c)` would refer to the NMC field $\tilde\chi$. The function `pow<n>` computes an integer power at each lattice site.

The first derivative of the potential is
@emgithub(models/lphi4NonMinimal.h:potential_derivs)

which corresponds to
```math
\begin{align}
\widetilde V_{,\tilde\phi}
=
\tilde\phi^3 .
\end{align}
```
The potential derivative with respect to $\chi$ vanishes in this model, because $\chi$ has no direct potential interaction. The NMC contribution $\xi\tilde{\bar R}\tilde\chi$ is supplied automatically by the NMC evolution kernel once the model declares `NonMinimalCouplings`.

Finally, the second derivative used for the initial effective mass of the inflaton is
@emgithub(models/lphi4NonMinimal.h:potential_second_derivs)

The numbering of these functions must remain consistent with the field numbering. The functions tagged with `Tag<0>` refer to derivatives with respect to field `0`, the inflaton $\phi$. The non-minimally coupled scalar $\chi$ is field `1`; in the current model file it has no direct potential derivative, and its curvature contribution is handled by the NMC machinery.

With these ingredients, the file $\texttt{lphi4NonMinimal.h}$ fully specifies the matter content, the NMC assignment, the program-variable normalization, the potential, and the potential derivatives needed by CosmoLattice.

### The NMC physics implemented in CosmoLattice { #sec_WhatHappensAutoinNMC }

We now summarize what CosmoLattice does internally when running the NMC model described above with self-consistent expansion. The structure is similar to the scalar singlet case, but two differences are essential. First, the NMC field $\chi$ is initialized from an external power spectrum. Second, the self-consistent evolution must be performed with the Runge-Kutta family of algorithms, because the NMC kernels depend on volume averages involving the fields and their conjugate momenta.

#### Initialization of fluctuations { #sec_InitNMC }

As in the scalar singlet case, the simulation starts from homogeneous field values plus fluctuations. The homogeneous values are read from `initial_amplitudes` and `initial_momenta`: the first entries correspond to the minimally coupled inflaton $\phi$, and the second entries correspond to the NMC field $\chi$.

The important NMC-specific ingredient is the initialization of the inhomogeneous $\chi$ fluctuations. The external power-spectrum prescription for scalar singlets is described in Section [*External power spectrum for scalar singlet initialization*](IC.md#subsubsec_ExternalPSSingletIC). In this model $\chi$ is scalar field `1`, so it can be initialized from an external spectrum with
```text
ext_PS1 = path/to/spectrum.dat
```
The corresponding parameter for the inflaton is `ext_PS0`; it can remain `none` if the inflaton fluctuations are initialized with the default scalar prescription. The `PS_type` value must match the convention used to produce the external file, and the file momenta must be measured in the same units as $\omega_*$. This setup is useful when the NMC spectator fluctuations have first been evolved in the linear regime as functions of momentum, and the resulting spectrum is then used as the initial condition for the fully nonlinear lattice stage.

#### Evolution of the system { #eq_evolution-NMC }

Once the fields are initialized, CosmoLattice evolves the two-field NMC model in first-order form. For the minimally coupled inflaton $\phi$ and the NMC field $\chi$, we define the program conjugate momenta
[](){ #eq_NMCmomentumChi }
```math
\begin{align}
b=a^{1-\alpha} a'\,
\hspace{0.6cm}
\tilde\pi_\phi = a^{3-\alpha}\tilde\phi',
\hspace{0.6cm}
\tilde\pi_\chi = a^{3-\alpha}\tilde\chi' . \tag{103}
\end{align}
```
The schematic first-order equations for the model introduced above are
[](){ #eq_HamiltonEOMNMC }
```math
\begin{align}
\tilde\phi'
&=
a^{\alpha-3}\tilde\pi_\phi,
\\
\tilde\pi_\phi'
&=
\mathcal{K}^{\rm L}_\phi[a,\tilde\phi],
\\
\tilde\chi'
&=
a^{\alpha-3}\tilde\pi_\chi,
\\
\tilde\pi_\chi'
&=
\mathcal{K}^{\rm L}_\chi[a,\tilde\chi,\tilde{\bar R}],
\\
a'
&=
a^{\alpha-1}b,
\\
b'
&=
\mathcal{K}^{\rm L}_a[a,\tilde{\bar R}] . \tag{104}
\end{align}
```
For the present example, the inflaton has the quartic potential derivative $\widetilde V_{,\tilde\phi}=\tilde\phi^3$, while $\chi$ has no direct potential interaction. The corresponding lattice kernels are
[](){ #eq_NMClatticeKernelChi }
```math
\begin{align}
\mathcal{K}^{\rm L}_\phi
&=
a^{1+\alpha}\sum_i\tilde\nabla_i^-\tilde\nabla_i^+\tilde\phi
-a^{3+\alpha}\tilde\phi^3,
\\
\mathcal{K}^{\rm L}_\chi
&=
a^{1+\alpha}\sum_i\tilde\nabla_i^-\tilde\nabla_i^+\tilde\chi
-a^{3+\alpha}\xi\tilde{\bar R}\tilde\chi,
\\
\mathcal{K}^{\rm L}_a
&=
\frac{a^{2+\alpha}}{6}\tilde{\bar R} , \tag{105}
\end{align}
```
where $\tilde{\bar R}$ is computed from volume averages of the full system, including both $\phi$ and $\chi$, in the following manner:
```math
\begin{align}
\tilde{\bar R} &=\frac{f_*^2}{m_p^2} \left[\frac{2\left(1-6\xi \right) \big\langle\tilde  G^{\phi} -\tilde K^{\phi}\big\rangle  + 4\langle \tilde V\rangle- 6\xi\langle \tilde\phi \,\tilde V_{,\tilde\phi}\rangle+({\tilde{\bar\rho}}_{\rm  m}-3{\tilde{\bar p}}_{\rm  m})}{1 + \left(6\xi -1\right)\xi \langle\tilde\phi^2\rangle (f_*^2/m_p^2)}\right] . \tag{106}
\end{align}
```
with
```math
\begin{align}
    \tilde K^{\phi} \equiv \frac{1}{2a^{2\alpha}} \tilde\phi'^2\, , \quad \tilde G^{\phi} \equiv \frac{1}{2a^{2}} (\tilde\nabla
     \tilde\phi)^2 . \tag{107} 
\end{align}
```
The spatial discretization follows the standard scalar singlet prescription. Gradients are discretized with forward derivatives, while the Laplacian is implemented symmetrically,
[](){ #eq_NMCderivativeDiscretization }
```math
\begin{align}
\partial_i f\partial_i f
&\longrightarrow
\sum_i \nabla_i^+f\,\nabla_i^+f ,
\\
\vec\nabla^2f
&\longrightarrow
\sum_i \nabla_i^-\nabla_i^+f,
\hspace{0.5cm}
f\in\{\phi,\chi\}. \tag{108}
\end{align}
```

!!! note
    In self-consistent NMC expansion, the background Ricci scalar $\tilde{\bar R}$ depends on volume averages of both fields and their conjugate momenta. Consequently, the NMC field kernel depends indirectly on $\tilde\pi_\phi$ and $\tilde\pi_\chi$ through $\tilde{\bar R}$. This momentum dependence means that symplectic algorithms such as LF, VV, or PV should not be used for self-consistent NMC evolution.

CosmoLattice therefore evolves self-consistent NMC models with explicit Runge-Kutta algorithms. The input parameter `evolver` should be chosen from the RK family implemented in CosmoLattice, such as `RK2`, `RK3_3`, `RK3_4`, or `RK4_5`. The example input file uses
```text
evolver = RK2
```
which is the second-order Runge-Kutta option. Higher-order RK algorithms can be used analogously when higher time-integration accuracy is required.

During the simulation, CosmoLattice still monitors the Friedmann constraint using volume-averaged energy densities, but the dynamical update of the scale factor is performed through the Ricci-scalar equation described above. This is the main structural difference with respect to the canonical scalar singlet case, where symplectic algorithms can be used and the scale factor kernel does not depend on the fields' momenta through an NMC Ricci scalar.
