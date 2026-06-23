<!-- <div style="text-align: justify;"> -->

Here we explain how CosmoLattice implements the dynamics and measurement of gravitational waves (GWs) on a lattice. We first review the continuum description of tensor perturbations and their transverse-traceless (TT) projection. We then summarize the different sources that can contribute to the effective anisotropic stress tensor in the field theories supported by CosmoLattice. Finally, we describe the lattice implementation: the auxiliary tensor fields evolved by the code, the lattice TT projection, and the lattice definition of the GW energy density spectrum.

!!! note
    This section concerns the GW module itself. The expression for the effective anisotropic stress tensor below assumes canonical scalar and gauge sectors. Models with non-canonical interactions, such as non-minimal coupling to gravity or non-canonical kinetic terms, require the corresponding source term to be adapted.

### Gravitational waves in the continuum { #subsec_GWcont }

Gravitational waves are tensor perturbations $h_{ij}$ on top of a FLRW background. In $\alpha$-time, the perturbed line element is
[](){ #eq_GWmetric }
```math
\begin{align}
ds^2
=
-a^{2\alpha}(\eta)d\eta^2
+a^2(\eta)\left(\delta_{ij}+h_{ij}\right)dx^idx^j,
\hspace{0.6cm}
\partial_i h_{ij}=0,
\hspace{0.3cm}
h_{ii}=0 . \tag{114}
\end{align}
```

The GW equation of motion is linear and reads
[](){ #eq_GWEOMcontinuum }
```math
\begin{align}
h_{ij}''
+(3-\alpha){a'\over a}h_{ij}'
-a^{-2(1-\alpha)}\nabla^2h_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}\Pi_{ij}^{\rm TT} . \tag{115}
\end{align}
```
The source is the TT part of the anisotropic stress tensor. The full anisotropic stress tensor is defined from the spatial components of the energy-momentum tensor as
[](){ #eq_GWAnisotropicStress }
```math
\begin{align}
\Pi_{ij}
\equiv
T_{ij}-\bar p\,g_{ij}, \tag{116}
\end{align}
```
where $\bar p$ is the homogeneous background pressure.

In practice we use an effective anisotropic stress tensor $\Pi^{\rm eff}_{ij}$, defined so that it has the same TT projection as the full tensor, $(\Pi^{\rm eff}_{ij})^{\rm TT}=\Pi^{\rm TT}_{ij}$. The source terms used by CosmoLattice for each field sector are summarized in the subsections below.

The TT projection is non-local in real space, but it is algebraic in Fourier space:
[](){ #eq_GWTTsourceProjection }
```math
\begin{align}
\Pi_{ij}^{\rm TT}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})\Pi_{lm}^{\rm eff}({\bf k},\eta). \tag{117}
\end{align}
```
The continuum TT projector is
[](){ #eq_GWTTprojector }
```math
\begin{align}
\Lambda_{ij,lm}(\hat{\bf k})
&\equiv
P_{il}(\hat{\bf k})P_{jm}(\hat{\bf k})
-{1\over2}P_{ij}(\hat{\bf k})P_{lm}(\hat{\bf k}),
\\
P_{ij}(\hat{\bf k})
&\equiv
\delta_{ij}-\hat k_i\hat k_j,
\hspace{0.5cm}
\hat k_i\equiv {k_i\over k}. \tag{118}
\end{align}
```
This guarantees $k_i\Pi_{ij}^{\rm TT}=0$ and $\Pi_{ii}^{\rm TT}=0$.

Solving Eq. ([*115*][eq_GWEOMcontinuum]) directly would require a TT projection of the source at every time step. Instead, CosmoLattice evolves auxiliary tensor fields whose sources are local in configuration space. The first option is a symmetric tensor $u_{ij}$ with six independent components,
[](){ #eq_GWuToh }
```math
\begin{align}
h_{ij}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})u_{lm}({\bf k},\eta), \tag{119}
\end{align}
```
where $u_{ij}$ obeys
[](){ #eq_GWuEOM }
```math
\begin{align}
u_{ij}''
+(3-\alpha){a'\over a}u_{ij}'
-a^{-2(1-\alpha)}\nabla^2u_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}\Pi_{ij}^{\rm eff}. \tag{120}
\end{align}
```
The second option uses only five independent components by evolving a symmetric traceless tensor $v_{ij}$,
[](){ #eq_GWvTraceless }
```math
\begin{align}
v_{33}=-(v_{11}+v_{22}). \tag{121}
\end{align}
```
The $v$-fields obey
[](){ #eq_GWvEOM }
```math
\begin{align}
v_{ij}''
+(3-\alpha){a'\over a}v_{ij}'
-a^{-2(1-\alpha)}\nabla^2v_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}
\left[
\Pi_{ij}^{\rm eff}
-{1\over3}\delta_{ij}\Pi_{kk}^{\rm eff}
\right], \tag{122}
\end{align}
```
and the physical tensor perturbations are reconstructed as
[](){ #eq_GWvToh }
```math
\begin{align}
h_{ij}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})v_{lm}({\bf k},\eta). \tag{123}
\end{align}
```
Using $v_{ij}$ saves one sixth of the memory required by the $u_{ij}$ representation, at the price of computing the traceless source at every lattice site.

The energy density of a GW background is
[](){ #eq_GWrhoContinuum }
```math
\begin{align}
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
\int {d\rho_{\rm GW}\over d\log k}d\log k . \tag{124}
\end{align}
```
This gives the continuum spectral density
[](){ #eq_GWrhoSpectrumContinuum }
```math
\begin{align}
\left({d\rho_{\rm GW}\over d\log k}\right)(k,\eta)
=
{m_p^2 k^3\over 8\pi^2a^{2\alpha}V}
\int {d\Omega_k\over4\pi}\,
h'_{ij}(\hat{\bf k},k,\eta)h_{ij}^{\prime *}(\hat{\bf k},k,\eta). \tag{125}
\end{align}
```
For stochastic sources, one can define the power spectrum of $h'_{ij}$ as
[](){ #eq_GWhprimePS }
```math
\begin{align}
\left\langle
h'_{ij}({\bf k},\eta)h_{ij}^{\prime *}({\bf k}',\eta)
\right\rangle
=
(2\pi)^3P_{h'}(k,\eta)\delta^{(3)}({\bf k}-{\bf k}'). \tag{126}
\end{align}
```
The fractional GW energy density spectrum is then
[](){ #eq_GWOmegaContinuum }
```math
\begin{align}
\Omega_{\rm GW}(k,\eta)
\equiv
{1\over\rho_c}{d\rho_{\rm GW}\over d\log k}
=
{k^3\over 24\pi^2\mathcal H^2}P_{h'}(k,\eta)
=
{k^3\over 24\pi^2a^{2\alpha}H^2}P_{h'}(k,\eta). \tag{127}
\end{align}
```

### GWs from Singlet Scalars { #subsec_SingletGW }

For canonically normalized real singlet scalars $\{\phi_b\}$, the effective anisotropic stress contribution is
[](){ #eq_GWsourceSinglet }
```math
\begin{align}
\Pi^{\rm eff}_{ij}\big|_{\rm singlet}
=
\sum_b \partial_i\phi_b\,\partial_j\phi_b . \tag{128}
\end{align}
```
All pressure terms and terms proportional to $\delta_{ij}$ have been omitted because they vanish after TT projection.

In the code, this contribution is implemented with forward lattice derivatives of each real singlet scalar:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_singlet_source)

### GWs from Complex Scalars { #subsec_ComplexGW }

For $U(1)$-charged complex scalars $\{\varphi_b\}$, the effective source is
[](){ #eq_GWsourceComplex }
```math
\begin{align}
\Pi^{\rm eff}_{ij}\big|_{\rm complex}
=
2\sum_b
{\rm Re}
\left[
\left(D_i^A\varphi_b\right)^*
\left(D_j^A\varphi_b\right)
\right]. \tag{129}
\end{align}
```
Here $D_i^A$ is the $U(1)$ covariant derivative.

The corresponding source term is computed with the forward $U(1)$ covariant lattice derivative:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_complex_source)

### GWs from Doublet Scalars { #subsec_DoubletGW }

For charged doublet scalars $\{\Phi_b\}$, the effective source is
[](){ #eq_GWsourceDoublet }
```math
\begin{align}
\Pi^{\rm eff}_{ij}\big|_{\rm doublet}
=
2\sum_b
{\rm Re}
\left[
\left(D_i\Phi_b\right)^\dagger
\left(D_j\Phi_b\right)
\right]. \tag{130}
\end{align}
```
The covariant derivative $D_i$ contains the gauge fields under which the doublet is charged.

This is the continuum doublet contribution to the GW source. In the current implementation of `PITensor.h`, the explicit source terms exposed in this file are the singlet scalar, complex scalar, and Abelian gauge contributions shown in the surrounding subsections.

### GWs from Abelian Gauge Fields { #subsec_AbelianGaugeGW }

For Abelian gauge fields, the effective source is written in terms of the Abelian electric and magnetic fields,
[](){ #eq_GWsourceAbelian }
```math
\begin{align}
\Pi^{\rm eff}_{ij}\big|_{\rm Abelian}
=
-\left(
a^{-2\alpha}E_iE_j
+a^{-2}B_iB_j
\right). \tag{131}
\end{align}
```
This is the Abelian part of the gauge-field contribution to the anisotropic stress tensor.

In program variables, `PITensor.h` splits this source into electric and magnetic contributions. The electric part is written in terms of the conjugate momentum of the $U(1)$ gauge field, while the magnetic part is built from the Abelian field strength:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_u1_source)

The components of the Abelian magnetic field are obtained from the spatial field-strength tensor as follows:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_u1_magnetic_field)

### GWs from Non-Abelian Gauge Fields (TBA) { #subsec_NonAbelianGaugeGW }

### GWs from Fluids (TBA) { #subsec_FluidGW }

### How to activate GWs { #subsec_ActivateGW }

The GW module is activated from the input parameter file. The relevant parameters can be added to any model input file; for example, the scalar singlet file contains the following GW block:

@emgithub(models/parameter-files/lphi4.in:GW_settings)

To evolve gravitational waves in a simulation, set `withGWs = true`. This can be done by editing the input file, or by overriding the parameter directly from the command line, for example
```bash
./lphi4 input=../models/parameter-files/lphi4.in withGWs=true GWprojectorType=2 doLFforGWs=true
```
When this flag is enabled, CosmoLattice allocates the auxiliary GW tensor fields and their conjugate momenta, evolves them together with the matter sector, and measures the GW spectra from the projected tensor perturbations.

The parameter `GWprojectorType` selects which lattice TT projector is used when reconstructing $h_{ij}$ from the auxiliary fields in Fourier space:

```text
GWprojectorType = 1  # real projector, built from k^0_L
GWprojectorType = 2  # backward projector, built from k^-_L; default choice
GWprojectorType = 3  # forward projector, built from k^+_L
```

The corresponding lattice momenta are defined explicitly in Eqs. ([*137*][eq_GWLatticeMomenta])-([*139*][eq_GWComplexProjector]) below. If `GWprojectorType` is not specified, CosmoLattice uses the default value `2`. The GW module is implemented for three spatial dimensions, so simulations with a different number of spatial dimensions must keep `withGWs = false`.

### Evolvers used for GWs { #subsec_GWEvolvers }

The GW sector has its own evolver choice, controlled by `doLFforGWs`, which by default, `= true`, and the tensor sector is evolved with the leapfrog algorithm even if the matter fields use a different evolver. If `doLFforGWs = false`, the GW sector uses the same evolver as the matter fields, as specified by the usual `evolver` parameter. The top-level evolver dispatch applies the GW leapfrog kicks and drifts separately whenever `typeGW == LF`:

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_dispatch)

Before measurements, GWs are synchronized when they have been advanced by leapfrog:

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_sync)

There is one important restriction for models containing gauge fields. Since the GW source depends on both magnetic fields and electric fields, and the latter are conjugate momenta, the code rejects evolver combinations that do not synchronize fields and momenta consistently. In practice, for scalar-only models the default `doLFforGWs = true` is the standard choice. For models with $U(1)$ gauge fields and GWs, do not use `evolver = LF` for the matter sector; use a synchronized matter evolver such as `VVn`, `PVn`, or an RK family. If the matter sector uses a position-Verlet evolver in a $U(1)$ model, keep the GW sector on leapfrog.

### Gravitational waves on the lattice { #subsec_GWlattice }

On the lattice, CosmoLattice evolves the auxiliary $u$- or $v$-fields in configuration space and applies the TT projection only when reconstructing the physical GW field $h_{ij}$ for measurements. Below we describe the $v$-field implementation; the $u$-field case is obtained by replacing the traceless source by $\Pi_{ij}^{\rm eff}$.

The program variables for space and time follow the standard CosmoLattice definitions. The auxiliary tensor is also rescaled as
[](){ #eq_GWProgramVars }
```math
\begin{align}
\tilde v_{ij}
=
\left({m_p\over f_*}\right)^2v_{ij}. \tag{132}
\end{align}
```
The conjugate momenta are $(\tilde\pi_v)_{ij}=a^{3-\alpha}\tilde v'_{ij}$, and the lattice evolution system is
[](){ #eq_GWFirstOrderSystem }
```math
\begin{align}
\tilde v_{ij}'
&=
a^{\alpha-3}(\tilde\pi_v)_{ij},
\\
(\tilde\pi_v)'_{ij}
&=
a^{1+\alpha}\tilde\nabla^2\tilde v_{ij}
+2a^{1+\alpha}
\left[
\tilde\Pi_{ij}^{\rm eff}
-{1\over3}\delta_{ij}\tilde\Pi_{kk}^{\rm eff}
\right]. \tag{133}
\end{align}
```

The corresponding GW kernel contains the lattice Laplacian of the auxiliary tensor field and the effective anisotropic tensor source:

@emgithub(include/CosmoInterface/evolvers/kernels/gwskernels.h:gws_kernel_source)

The effective anisotropic stress tensor in program units is
[](){ #eq_GWProgramStress }
```math
\begin{align}
\tilde\Pi^{\rm eff}_{ij}
\equiv
{\Pi^{\rm eff}_{ij}\over \omega_*^2f_*^2}
&=
\sum_b\tilde\nabla_i\tilde\phi_b\,\tilde\nabla_j\tilde\phi_b
+2\sum_b{\rm Re}
\left[
\left(\tilde D_i^A\tilde\varphi_b\right)^*
\left(\tilde D_j^A\tilde\varphi_b\right)
\right]
\\
&\quad
+2\sum_b{\rm Re}
\left[
\left(\tilde D_i\tilde\Phi_b\right)^\dagger
\left(\tilde D_j\tilde\Phi_b\right)
\right]
-\left({\omega_*^2\over f_*^2}\right)
\left[
a^{-2\alpha}\tilde E_i\tilde E_j
+a^{-2}\tilde B_i\tilde B_j
\right]. \tag{134}
\end{align}
```
For scalar sources this amounts to replacing fields and derivatives by their program-variable counterparts. For Abelian gauge fields, the source contains the additional factor $(\omega_*^2/f_*^2)$ multiplying the products of program electric and magnetic fields.

The implemented source contributions are assembled in `PITensor.h`:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_total_source)

The physical GW field is reconstructed in Fourier space with a lattice TT projector,
[](){ #eq_GWhLattice }
```math
\begin{align}
h_{ij}(\tilde{\bf n},\eta)
=
\Lambda^{\rm L}_{ij,lm}(\tilde{\bf n})v_{lm}(\tilde{\bf n},\eta). \tag{135}
\end{align}
```
The lattice projector is
[](){ #eq_GWTTProjectorLattice }
```math
\begin{align}
\Lambda^{\rm L}_{ij,lm}(\tilde{\bf n})
&\equiv
P^{\rm L}_{il}(\tilde{\bf n})P^{{\rm L}*}_{jm}(\tilde{\bf n})
-{1\over2}P^{\rm L}_{ij}(\tilde{\bf n})P^{{\rm L}*}_{lm}(\tilde{\bf n}),
\\
P^{\rm L}_{ij}(\tilde{\bf n})
&\equiv
\delta_{ij}
-{k^*_{{\rm L},i}(\tilde{\bf n})k_{{\rm L},j}(\tilde{\bf n})
\over
|{\bf k}_{\rm L}(\tilde{\bf n})|^2}. \tag{136}
\end{align}
```
Here ${\bf k}_{\rm L}$ is the lattice momentum associated with the derivative operator used in the TT projection. Writing $\theta_i\equiv 2\pi\tilde n_i/N$, the three choices used by `GWprojectorType` are
[](){ #eq_GWLatticeMomenta }
```math
\begin{align}
k^0_{{\rm L},i}(\tilde{\bf n})
&=
\sin\theta_i,
\hspace{1cm}
[\texttt{GWprojectorType}=1],
\\
k^-_{{\rm L},i}(\tilde{\bf n})
&=
\sin\theta_i
-i\left(1-\cos\theta_i\right),
\hspace{1cm}
[\texttt{GWprojectorType}=2],
\\
k^+_{{\rm L},i}(\tilde{\bf n})
&=
\sin\theta_i
+i\left(1-\cos\theta_i\right),
\hspace{1cm}
[\texttt{GWprojectorType}=3].
\tag{137}
\end{align}
```
The overall factor $1/\delta\tilde x$ has been omitted in Eq. ([*137*][eq_GWLatticeMomenta]), because it cancels in the projector ratios. Equivalently, one may multiply every component by $1/\delta\tilde x$ when interpreting ${\bf k}_{\rm L}$ as a dimensionful lattice momentum. The neutral momentum ${\bf k}^0_{\rm L}$ is real, while the forward and backward momenta ${\bf k}^{\pm}_{\rm L}$ are complex.

For neutral derivatives, the lattice momentum is real and the projector can be written as
[](){ #eq_GWRealProjector }
```math
\begin{align}
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
|{\bf k}^0_{\rm L}|^2}. \tag{138}
\end{align}
```
For the forward or backward choices in Eq. ([*137*][eq_GWLatticeMomenta]), the lattice momentum is complex and the projector is
[](){ #eq_GWComplexProjector }
```math
\begin{align}
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
|{\bf k}^\pm_{\rm L}|^2}. \tag{139}
\end{align}
```
The lattice projector is transverse with respect to the lattice derivative used to define it. For the complex projector, it is also Hermitian and idempotent, properties that are essential for a consistent lattice TT projection.

After the TT projection, the GW energy density on the lattice follows from the discrete analogue of Eq. ([*124*][eq_GWrhoContinuum]):
[](){ #eq_GWrhoLattice }
```math
\begin{align}
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
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta). \tag{140}
\end{align}
```
Splitting the Fourier sum into spherical shells gives
[](){ #eq_GWrhoShells }
```math
\begin{align}
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
\Delta\log k . \tag{141}
\end{align}
```
The corresponding Type-II lattice spectrum is
[](){ #eq_GWDiscreteSpectrum }
```math
\begin{align}
\left({d\rho_{\rm GW}\over d\log k}\right)(l)
=
{m_p^2k^3(l)\over8\pi^2a^{2\alpha}L^3}
\left\langle
\left[\delta x^3h'_{ij}(|\tilde{\bf n}|,\eta)\right]
\left[\delta x^3h'_{ij}(|\tilde{\bf n}|,\eta)\right]^*
\right\rangle_{R(l)}. \tag{142}
\end{align}
```
Normalizing by the critical energy density gives
[](){ #eq_GWOmegaTypeII }
```math
\begin{align}
\Omega_{\rm GW}(\tilde{\bf n},\eta)
=
{1\over\rho_c}
{m_p^2k^3(l)\over8\pi^2a^{2\alpha}}
{\delta x^3\over N^3}
\left\langle
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta)
\right\rangle_{R(\tilde{\bf n})}
\hspace{0.5cm}
[{\rm Type~II}]. \tag{143}
\end{align}
```
The analogous Type-I lattice spectrum is
[](){ #eq_GWOmegaTypeI }
```math
\begin{align}
\Omega_{\rm GW}(\tilde{\bf n},\eta)
=
{1\over\rho_c}
{m_p^2k(l)\over8\pi a^{2\alpha}}
{\delta x\over N^5}
\#_l
\left\langle
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta)
\right\rangle_{R(l)}
\hspace{0.5cm}
[{\rm Type~I}]. \tag{144}
\end{align}
```

In the lattice code, the relevant bilinear $h'_{ij}h_{ij}^{\prime *}$ can be computed directly from the conjugate momenta of the auxiliary fields:
[](){ #eq_GWhprimeFromMomentum }
```math
\begin{align}
h'_{ij}
=
{\omega_*\over a^{3-\alpha}}
\Lambda^{\rm L}_{ij,lm}(\pi_v)_{lm}
=
{\omega_*\over a^{3-\alpha}}
\left({f_*\over m_p}\right)^2
\Lambda^{\rm L}_{ij,lm}(\tilde\pi_v)_{lm}. \tag{145}
\end{align}
```
This bilinear can be written compactly in terms of traces,
[](){ #eq_GWTraceFormula }
```math
\begin{align}
h'_{ij}h_{ij}^{\prime *}
=
{\rm Tr}({\tt P}{\tt v}'{\tt P}{\tt v}^{\prime *})
-{1\over2}
{\rm Tr}({\tt P}{\tt v}')
{\rm Tr}({\tt P}{\tt v}^{\prime *}). \tag{146}
\end{align}
```
Here ${\tt v}'$ and ${\tt P}$ are matrices with entries $({\tt v}')_{ij}=v'_{ij}$ and $({\tt P})_{ij}=P^{\rm L}_{ij}$.

CosmoLattice can also monitor whether the reconstructed $h_{ij}$ remains transverse and traceless with respect to the lattice projector. Useful diagnostics are
[](){ #eq_GWTTDiagnostics }
```math
\begin{align}
\delta(\eta)
&\equiv
{\left\langle \nabla_i^{\rm L}h_{ij}({\bf n},\eta)\right\rangle
\over
\left\langle S_i^{\rm L}h_{ij}({\bf n},\eta)\right\rangle},
\hspace{0.5cm}
[{\rm transversality}],
\\
\lambda(\eta)
&\equiv
{\left\langle \left|\sum_i h_{ii}({\bf n},\eta)\right|\right\rangle
\over
\left\langle \sum_i |h_{ii}({\bf n},\eta)|\right\rangle},
\hspace{0.5cm}
[{\rm tracelessness}]. \tag{147}
\end{align}
```
The operator $\nabla_i^{\rm L}$ is the lattice derivative associated with the momentum used in the TT projector, and
[](){ #eq_GWSoperators }
```math
\begin{align}
S_i^0h_{ij}
&\equiv
{h_{ij}({\bf n}+\hat{\imath},\eta)+h_{ij}({\bf n}-\hat{\imath},\eta)\over2\delta x},
\\
S_i^\pm h_{ij}
&\equiv
{h_{ij}({\bf n}\pm\hat{\imath},\eta)+h_{ij}({\bf n},\eta)\over\delta x}. \tag{148}
\end{align}
```
Both indicators are expected to be very small, typically at the level of machine precision, for a correctly projected GW field.

### GW output { #subsec_GWOutput }

When `withGWs = true`, CosmoLattice generates the standard output of the matter sector and, in addition, two GW-specific output files. These files are written inside the directory specified by `outputfile`.

-  `average_energies_gws.txt`: Volume-averaged GW energy quantities, printed at the same cadence as the GW spectrum measurements. The columns are

$\hspace{1cm}$ $\tilde{\eta}$, $\tilde{E}_{\rm GW}/\tilde{E}_{\rm matter}$, $\tilde{E}_{\rm GW}$.

Here $\tilde{E}_{\rm GW}/\tilde{E}_{\rm matter}$ is obtained by integrating the measured GW energy-density spectrum over the available momentum bins. The quantity $\tilde{E}_{\rm GW}$ is this fraction multiplied by the total matter energy density at the same time.

-  `spectra_energy_gws.txt`: GW fractional energy-density spectrum measured from the projected tensor perturbations. Its printing frequency is controlled by `tOutputInfreq`, together with the other spectra in the simulation. The file contains

$\hspace{1cm}$ $\tilde{k}$, $\Omega_{\rm GW}(\tilde{k})$, $\Delta n_{\rm bin}$,

up to the additional bin information requested by the usual spectrum-output parameters. The first column is the momentum scale associated with the bin, the second column is the GW spectrum computed with the projector selected by `GWprojectorType`, and the last column gives the bin multiplicity. For higher `spectraVerbosity`, CosmoLattice also prints extra bin statistics, analogously to the scalar spectra described in Section [*Outputs*](My first model of (singlet) scalar fields.md#outputs).

-  `average_spectra_times.txt`: List of times at which the GW spectrum and the other infrequent spectra are outputted.

If `hdf5Averages = true`, the averaged GW quantities are written into the common HDF5 averages file instead of `average_energies_gws.txt`. If `hdf5Spectra = true`, the GW spectrum is written into the common spectra HDF5 file instead of `spectra_energy_gws.txt`. If `saveUnbinnedSpectra = true`, the corresponding unbinned GW spectrum is saved through the standard unbinned-spectrum HDF5 output.
