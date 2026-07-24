<!-- <div style="text-align: justify;"> -->

This section covers how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ simulates the production and evolution of gravitational waves (GWs). We first explain in [*Gravitational waves on the lattice*][subsec_GWinCL] how the dynamics of GWs are simulated on the lattice, and describe how they are implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. We make emphasis on the form of the anisotropic stress tensor and the form of the lattice transverse-traceless projector. We also describe how the simulation of GWs can be controlled from the parameter file and some of the restrictions built in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ since version 2.0. We later describe in [*GW observables*][subsec_GWobservables] the most relevant GW-related observables and how they can be measured with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. A brief review of the dynamics of GWs in the continuum can be found in [*Canonical Field Theory*][subsec_eomCont].

!!! "Scope of the GW module"
    As of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 2.0, the simulation of GWs is limited to models of canonically normalized scalar (real and complex) and Abelian gauge fields. Models with non-canonical interactions, such as scalars non-minimally coupled to gravity or non-canonical kinetic scalar theories, would require adapting the GW source terms to their non-minimal nature. The production of GWs from models involving $\mathrm{SU}(2)$ doublets or gauge fields is also not implemented in the current version. Both these features are planned to be released in the future.

!!! "Linearized gravity regime"
    GWs in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ are simulated in the linearized gravity regime, this is, backreaction of the GWs into the matter fields is neglected. We usually say that GWs are thus **passive**.




### Gravitational waves in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ { #subsec_GWlatt }

The production and evolution of gravitational waves can be easily simulated with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, simply by setting `withGWs = true` in the parameter file. At the moment, the emission of GWs is implemented for canonically normalized scalars (both real and complex) and Abelian $\mathrm{U}(1)$ fields. We now review how GWs are simulated in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and explain in detail the different options that can be chosen in the parameter file.

**Gravitational waves on the lattice**
 Directly simulating the physical degrees of freedom, sourced by the transverse-traceless part of the anisotropic stress tensor, would require the computation of TT projections on every time step, which involves back-and-forth transformations to Fourier space. This would be extremely expensive in terms of computational cost, scaling as $\mathcal{O}(N^3\log N)$ with the number of points per dimension of the lattice. An alternative algorithm that overcomes this limitation by using unphysical degrees of freedom that are sourced by an effective stress anisotropic tensor was first proposed in [@GarciaBellido_2008]. The original algorithm, that employs six unphysical degrees of freedom, was used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ v1.1 to v1.3.

Since $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ $\tt v2.0$, the simulation of GWs follows an alternative algorithm first presented in $\,\texttt{The}\,\texttt{Art}$-$\texttt{II}$ [@BaezaBallesteros_2025tme]. This makes use of five auxiliary degrees of freedom, which form a symmetric and traceless tensor, $v_{ij}$ with $v_{33}=-v_{11}-v_{22}$ that is evolved following a discretized version of
[](){ #eq_GWvEOM2 }
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
-{1\over3}\delta_{ij}\Pi_{kk}^{\rm eff}
\right]\,,
\end{align}
```
where $\Pi_{ij}^\text{eff}$ is the effective anisotropic stress tensor, that we describe below. In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, this is implemented by defining conjugate momenta variables $(\tilde\pi_v)_{ij}=a^{3-\alpha}\tilde v'_{ij}$, and rewriting the equation of motion as
[](){ #eq_GWFirstOrderSystem }
```math
\begin{align}
\label{eq_GWFirstOrderSystem}
\tilde v_{ij}'
&=
a^{\alpha-3}(\tilde\pi_v)_{ij}\,,
\\
(\tilde\pi_v)'_{ij}
&=
a^{1+\alpha}\tilde\nabla^2\tilde v_{ij}
+2a^{1+\alpha}
\left[
\tilde\Pi_{ij}^{\rm eff}
-{1\over3}\delta_{ij}\tilde\Pi_{kk}^{\rm eff}
\right].
\end{align}
```
where the auxiliary tensor, although dimensionless, is rescaled as
```math
\begin{align}
\label{eq_GWProgramVars}
\tilde v_{ij}
=
\left({m_p\over f_*}\right)^2v_{ij}.
\end{align}
```
The GW kernel, corresponding to the right hand side of the second equation in Eq.$~$\eqref{eq_GWFirstOrderSystem}, is implemented in `evolvers/kernels/gwskernels.h`. It contains the lattice Laplacian of the auxiliary tensor field and the effective anisotropic tensor source, which we discuss below,
@emgithub(include/CosmoInterface/evolvers/kernels/gwskernels.h:gws_kernel_source)

!!! "Note for advanced users"
    The auxiliary fields used to simulate the evolution of GWs are implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ using smart pointers from C++, instead of static objects as is the case for matter fields. This makes it possible to turn the GWs on or off from the parameter file without the need to recompile the model, but it also means that some routines are implemented in a slightly different manner, compared to matter fields.

**Effective anisotropic stress tensor**

For a generic model with scalar and Abelian gauge fields, the effective anisotropic stress tensor takes the form
[](){ #eq_GWProgramStress }
```math
\begin{align}
\label{eq_GWProgramStress}
\begin{array}{rclcl}
\tilde\Pi^{\rm eff}_{ij}
&\equiv&\displaystyle
{\Pi^{\rm eff}_{ij}\over \omega_*^2f_*^2}
&=&\displaystyle
\sum_b\tilde\nabla_i^+\tilde\phi_b\,\tilde\nabla_j^+\tilde\phi_b
+2\sum_b{\rm Re}
\left[
\left(\tilde D_i^{A,+}\tilde\varphi_b\right)^*
\left(\tilde D_j^{A,+}\tilde\varphi_b\right)
\right]
\\[7pt]
& & & & \displaystyle-\left({\omega_*^2\over f_*^2}\right)
\left[
a^{-2\alpha}\tilde E_i\tilde E_j
+a^{-2}\tilde B_i\tilde B_j
\right]\,.
\end{array}
\end{align}
```
where we highlight the presence of the $(\omega_*^2/f_*^2)$ multiplying the products of electric and magnetic fields. This is related to the particular choice of program variables made in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. In the code, the effective anisotropic stress tensor is defined in $\texttt{definitions/PItensor.h}$, where the contributions from the different types of fields are implemented separately:

-   Scalar singlets:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_singlet_source)

-   Complex scalars:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_complex_source)

-   Abelian $\mathrm{U}(1)$ fields:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_u1_source)

Here, the electric field is written in terms of the conjugate momentum of the $\mathrm{U}(1)$ gauge field, while the magnetic field is built from the Abelian field strength:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_u1_magnetic_field)

The effective anisotropic stress tensor is finally the sum of all the separate contributions

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_total_source)

**Transverse-traceless projection**

As the auxiliary degrees of freedom are evolved, they can be used to reconstruct the physical GW fields which are needed to determine the energy density of GWs, as we discuss later. Such physical degrees of freedom are obtained in Fourier space after a lattice TT projection
[](){ #eq_GWhLattice }
```math
\begin{align}
\label{eq_GWhLattice}
h_{ij}(\tilde{\bf n},\eta)
=
\Lambda^{\rm L}_{ij,lm}(\tilde{\bf n})v_{lm}(\tilde{\bf n},\eta)\,,
\end{align}
```
where  the lattice projector is defined analogously to its continuous counterpart,
[](){ #eq_GWTTProjectorLattice }
```math
\begin{align}
\label{eq_GWTTProjectorLattice}
\Lambda^{\rm L}_{ij,lm}(\tilde{\bf n})
&\equiv
P^{\rm L}_{il}(\tilde{\bf n})P^{{\rm L}*}_{jm}(\tilde{\bf n})
-{1\over2}P^{\rm L}_{ij}(\tilde{\bf n})P^{{\rm L}*}_{lm}(\tilde{\bf n}),
\end{align}
```
with
[](){ #eq_GWTTProjectorLattice2 }
```math
\begin{align}
P^{\rm L}_{ij}(\tilde{\bf n})
&\equiv
\delta_{ij}
-{k^*_{{\rm L},i}(\tilde{\bf n})k_{{\rm L},j}(\tilde{\bf n})
\over
|{\bf k}_{\rm L}(\tilde{\bf n})|^2}.
\end{align}
```
Here ${\bf k}_{\rm L}$ is the  lattice momentum associated with the derivative operator that characterizes the form of the projector. It determines with respect to which lattice derivative operators are GWs transverse, this is, with respect to which operator $\nabla^\text{L}$ the projected fields obey $\nabla^\text{L}_i h_{ij}=0$.   In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ three different choices are implemented,
[](){ #eq_GWLatticeMomenta}
```math
\begin{align}
\label{eq_GWLatticeMomenta_1}
\begin{array}{rcll}
k^0_{{\rm L},j}(\tilde{\bf n})
&=&\displaystyle
\frac{1}{\delta \tilde{x}}\sin\theta_j\,,
&
[\texttt{Neutral derivative}]\,,\\[10pt]
k^-_{{\rm L},j}(\tilde{\bf n})
&=&\displaystyle
\frac{1}{\delta \tilde{x}}\left[\sin\theta_j
-i\left(1-\cos\theta_j\right)\right]\,,&
\hspace{1cm}
[\texttt{Forward derivative}],
\\[10pt]
k^+_{{\rm L},j}(\tilde{\bf n})
&=&\displaystyle
\frac{1}{\delta \tilde{x}}\left[\sin\theta_j
+i\left(1-\cos\theta_j\right)\right]\,,\quad\quad\quad&
[\texttt{Backward derivative}]\,.
\end{array}
\end{align}
```
where $\theta_j\equiv 2\pi\tilde n_j/N$. The neutral momentum, ${\bf k}^0_{\rm L}$, is real, while the forward and backward momenta, ${\bf k}^{\pm}_{\rm L}$, are complex.

In the case of TT projection with respect to neutral derivatives, as the associated lattice momentum is real, the projector can be written as
[](){ #eq_GWRealProjector }
```math
\begin{align}
\label{eq_GWRealProjector}
\Lambda_{ij,lm}^{{\rm L},0}
&=
P^{{\rm L},0}_{il}P^{{\rm L},0}_{jm}
-{1\over2}P^{{\rm L},0}_{ij}P^{{\rm L},0}_{lm},
\\
P_{ij}^{{\rm L},0}
&=
\delta_{ij}
-{k^0_{{\rm L},i}k^0_{{\rm L},j}
\over
|{\bf k}^0_{\rm L}|^2}.
\end{align}
```
On the other hand, for forward or backward derivatives, the lattice momentum is complex and the projector becomes
[](){ #eq_GWComplexProjector }
```math
\begin{align}
\label{eq_GWComplexProjector}
\Lambda_{ij,lm}^{{\rm L},\pm}
&=
P^{{\rm L},\pm}_{il}P^{{\rm L},\pm *}_{jm}
-{1\over2}P^{{\rm L},\pm}_{ij}P^{{\rm L},\pm *}_{lm},
\\
P_{ij}^{{\rm L},\pm}
&=
\delta_{ij}
-{(k^\pm_{{\rm L},i})^*k^\pm_{{\rm L},j}
\over
|{\bf k}^\pm_{\rm L}|^2}.
\end{align}
```
For the complex projector, it is also Hermitian and idempotent, properties that are essential for a consistent lattice TT projection. For a complete list of the projector properties see, for example, $\,\texttt{The}\,\texttt{Art}$-$\texttt{II}$ [@BaezaBallesteros_2025tme].

### **Running a simulation with GWs** { #subsec_ActivateGW }

Simulating the production of GWs in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is controlled with the `withGWs` keyword in the input parameter file. For any model with the valid matter content, setting this to `withGWs = true` will include the production of GWs in the simulation. For example, $\texttt{models/parameter-files/lphi4.in}$ contains the following GW block:

@emgithub(models/parameter-files/lphi4.in:GW_settings)

Thus, a simulation of the $\texttt{lphi4}$ model with GWs could be performed by simply changing this flag to `withGWs = true`. Alternatively, the parameter can be override directly from the command line when executing the model, for example
```bash
./lphi4 input=../models/parameter-files/lphi4.in withGWs=true GWprojectorType=2 doLFforGWs=true
```
where we explain later the remaining parameters.

When the flag `withGWs` is enabled, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ includes the auxiliary tensor fields (amplitudes and conjugate momenta), which are evolved simultaneously along the matter sectors. As already mentioned, such evolution is **passive**, in the sense that only the sourcing of GWs by the matter sectors is considered, while the backreaction of the GWs onto the matter sectors is not taken into account (as it is expected to be negligible in the majority of cases).

!!! warning "Linearized gravity regime"
    GWs are only defined for `NDim = 3` spatial dimensions. Simulations with other number of dimensions will fail when trying to run with GWs, and so must keep `withGWs = false`.


Another option that can be controlled by the user is related to the evolver used for the GWs. Since $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 2.0, the GW sector has its own evolver choice, controlled by the `doLFforGWs` keyword. By default `doLFforGWs = true`, and the tensor sector is evolved with the leapfrog algorithm even if the matter fields use a different evolver. This aims at reducing the numerical cost of the simulation when using higher-order or non-symplectic algorithms for the matter sector. We note that, while it is possible to run a higher-order evolver for the matter sector compared to GWs, the presence of discretization effects on the simulation for GWs needs to be studied in a case-by-case basis.

Alternatively, one can set `doLFforGWs = false` so that the GW sector uses the same evolver as the matter fields, as specified by the usual `evolver` parameter. This case, however, has some restrictions for models containing gauge fields. As the Abelian GW source depends on the electric field in addition to the magnetic and scalar fields, it needs to be evaluated at a time where fields and conjugate momenta are synchronized. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ therefore rejects evolver combinations that cannot provide a consistently synchronized matter source for the GW update. The supported combinations are:

| Matter content | Matter evolver | GW evolver | Supported |
| -------------- | -------------- | ---------- | --------- |
| Scalars | `LF` | `LF` | ✓ |
| Scalars | `VVn` | `LF` | ✓ |
| Scalars | `VVn` | `VVn` | ✓ |
| Scalars | `PVn` | `LF` | ✓ |
| Scalars | `PVn` | `PVn` | ✓ |
| Scalars | `RKn` | `LF` | ✓ |
| Scalars | `RKn` | `RKn` | Not implemented |
| Scalars + U(1) gauge fields | `LF` | `LF` | ✗ |
| Scalars + U(1) gauge fields | `VVn` | `LF` | ✓ |
| Scalars + U(1) gauge fields | `VVn` | `VVn` | ✓ |
| Scalars + U(1) gauge fields | `PVn` | `LF` | ✓ |
| Scalars + U(1) gauge fields | `PVn` | `PVn` | ✗ |
| Scalars + U(1) gauge fields | `RKn` | `LF` | ✓ |
| Scalars + U(1) gauge fields | `RKn` | `RKn` | Not implemented |

Rows with GW evolver `LF` correspond to `doLFforGWs = true`, and rows where the GW evolver matches the matter evolver correspond to `doLFforGWs = false`. The unsupported $\mathrm{U}(1)$ rows reflect the staggered-time mismatch of the gauge electric field and gauge links when the matter sector itself is evolved with `LF` and `PVn`. Evolvers `RKn` are not implemented for GWs evolution as of version 2.0. Note this is not the case for the velocity Verlet algorithm, as the kernel of GWs only needs to be evaluated before any matter is evolved or after the end of the matter evolution.

Finally, we briefly comment on the implementation of the GW leapfrog evolver. This applies kicks and drifts to the auxiliary GW fields whenever `typeGW == LF`, and is implemented within the $\texttt{evolvers/evolver.h}$ header file,

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_dispatch)

In addition, GWs are synchronized before measurement, similarly to matter fields when using the LF algorithm,

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_sync)


## **GW observables** { #subsec_GWobservables }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ makes it possible to keep track of the evolution of the energy density of GWs, both its total value and its spectral density. Before explaining how they are measured in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, we briefly review the relevant lattice definitions.

[](){ #subsec_GWobservablesLattice }
**GW observables on the lattice**

On the lattice, the GW energy density is defined as
[](){ #eq_GWrhoLattice }
```math
\begin{align}
\label{eq_GWrhoLattice}
\rho_{\rm GW}(\eta)
&=
{m_p^2\over4a^{2\alpha}N^3}
\sum_{\bf n}
h'_{ij}({\bf n},\eta)h'_{ij}({\bf n},\eta)
\\
&=
{m_p^2\over4a^{2\alpha}}
{1\over N^6}
\sum_{\tilde{\bf n}}
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta).
\end{align}
```
As for any other power spectrum, this sum can be separated as a radial sum over radial bins, and an average over spherical shells, $R(l)$,
[](){ #eq_GWrhoShells }
```math
\begin{align}
\label{eq_GWrhoShells}
\rho_{\rm GW}(\eta)
=
\sum_l
\left\{
{m_p^2\delta x^6\over 8\pi^2a^{2\alpha}L^3}
k^3(l)
\left\langle
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta)
\right\rangle_{R(l)}
\right\}
\Delta\log k \,,
\end{align}
```
from which the associated lattice power spectrum and the energy density power spectrum can be defined. For example, in CosmoLattice the Type-I version 1 fractional energy density power spectrum is defined as
[](){ #eq_GWOmegaTypeI }
```math
\begin{align}
\label{eq_GWOmegaTypeI}
\Omega_{\rm GW}(\tilde{\bf n},\eta)
=
{1\over\rho_\text{mat}}
{m_p^2k(l)\over8\pi a^{2\alpha}}
{\delta x\over N^5}
\#_l
\left\langle
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta)
\right\rangle_{R(l)}
\hspace{0.5cm}
[{\rm Type~I, version~1}].
\end{align}
```
with $\#_l$ the multiplicity of bin $l$. Alternatively, the Type II version 1 power spectrum takes the form
[](){ #eq_GWOmegaTypeII }
```math
\begin{align}
\label{eq_GWOmegaTypeII}
\Omega_{\rm GW}(\tilde{\bf n},\eta)
=
{1\over\rho_\text{mat}}
{m_p^2k^3(l)\over8\pi^2a^{2\alpha}}
{\delta x^3\over N^3}
\left\langle
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta)
\right\rangle_{R(\tilde{\bf n})}
\hspace{0.5cm}
[{\rm Type~II, version~1}]\,.
\end{align}
```
Other versions are also defined analogously to the power spectra of matter fields.

Note that the fractional energy density in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is defined with respect to the total energy density of the matter sector. This corresponds to the critical density for self consistent expansion, but it is different for fixed expansion or flat background. If in the case of fixed expansion you want the results normalized with the critical energy density, you shall use the results for the total energy density and the Hubble constant stored in $\texttt{average_energies.txt}$ and $\texttt{average_scale_factor.txt}$, respectively.

Finally, it is worth noting that, on the lattice, the relevant bilinear $h'_{ij}h_{ij}^{\prime *}$ needed to evaluate the energy density of GWs can be computed directly from the conjugate momenta of the auxiliary fields in terms of traces
[](){ #eq_GWTraceFormula }
```math
\begin{align}
\label{eq_GWTraceFormula}
h'_{ij}h_{ij}^{\prime *}
=
{\rm Tr}({\tt P}{\tt v}'{\tt P}{\tt v}^{\prime *})
-{1\over2}
{\rm Tr}({\tt P}{\tt v}')
{\rm Tr}({\tt P}{\tt v}^{\prime *})\,,
\end{align}
```
where ${\tt v}'$ and ${\tt P}$ are matrices with entries $({\tt v}')_{ij}=v'_{ij}$ and $({\tt P})_{ij} = P^{\rm L}_{ij}$. This expression slightly simplifies in the case of a real TT projector.

[](){ #subsec_GWobservablesCL }
**GW observables in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$**

Measurements of the GW energy density are automatically performed for simulations with GWs with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. If the keyword `withGWs = true` is specified in the parameter file, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ will generate, in addition to the measurements related to the matter sector, two GW-related output files, saved inside the same directory as specified by `outputfile`:

<div class="grid cards cl-files" markdown>

-   `spectra_energy_gws.txt`{ .cl-fname }

    GW fractional energy-density spectrum measured from the projected tensor perturbations. As for matter spectra, the frequency of the measurements is controlled by the `tOutputInfreq` keyword. By default, the file contains

    [](){ #eq_GWPSmeasurements }
    $\tilde{k}$, $\Omega_{\rm GW}(\tilde{k})$, $\#_{\rm bin}$
    {: .cl-schema }

    The first column is the momentum scale associated with each bin, the second is the GW spectrum computed with the projector selected by `GWprojectorType` as discussed below, and the last one gives the bin multiplicity.  The specific output can be controlled with the same parameters as for other power spectra, see [**Output Observables**](Observables.md) for a detailed explanation.  The times at which the spectra are measured are saved sequentially in the $\texttt{average_spectra_times.txt}$ file. Finally, we recall that the fractional energy density is normalized with the total energy density of the matter fields, rather than the critical energy density.

-   `average_energies_gws.txt`{ .cl-fname }

    Volume-averaged GW energy density, determined from summing over the contributions of all bins in the power spectrum, as indicated in Eq.$~$\eqref{eq_GWrhoShells}. The columns of this file correspond to

    [](){ #eq_GWenergymeasurements }
    $\tilde{\eta}$, $\tilde{\rho}_{\rm GW}/\tilde{\rho}_{\rm matter}$, $\tilde{\rho}_{\rm GW}$
    {: .cl-schema }

    Here $\tilde{\rho}_{\rm GW}/\tilde{\rho}_{\rm matter}$ is obtained by integrating the measured GW energy-density spectrum over the available momentum bins. The quantity $\tilde{\rho}_{\rm GW}$ is this fraction multiplied by the total matter energy density at the same time.

</div>

For both outputs, the observables are determined after TT projecting the auxiliary fields used by $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, using Eq.$~$\eqref{eq_GWTraceFormula}. The type of projector used is controlled using the `GWprojectorType` keyword in the parameter file. This can take three different values, corresponding to the three choices of lattice momenta in Eq.$~$\eqref{eq_GWRealProjector}:


| **Value** <div style="width:150px"> | **Projector type** |
| ---------------------------------------- | --------------- |
| `GWprojectorType = 1` | Real projector, built from $k^0_\mathrm{L}$ |
| `GWprojectorType = 2` | Backward projector, built from $k^-_\mathrm{L}$. Default choice. |
| `GWprojectorType = 3` | Forward projector, built from $k^+_\mathrm{L}$. |

Finally, we note that the results of the total energy density of GWs, as it is computed from integrating the power spectrum, may depend on the particular type and version of power spectrum used, as well as on the width of the binning. We note, however, that for `PS_type = 1` and `PS_version = 1`, the result is independent of the binning.
