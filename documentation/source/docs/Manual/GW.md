<!-- <div style="text-align: justify;"> -->

Here we explain how CosmoLattice implements the dynamics (creation, evolution and measurement) of gravitational waves (GWs) on a lattice. We first review the continuum description of tensor perturbations and of a transverse-traceless (TT) projection. Following, we summarize the different fields that can contribute to the effective anisotropic stress tensor that sources GWs. We then describe the dynamics of GWs in CosmoLattice, which follows the proposal of (<span style="color:red;">**Ref. 2007 w/ Sastre paper**</span>) of evolving an auxiliary tensor field that is only TT-projected at spectral measurements, not at every time step. We finally discuss the lattice definition of the GW energy density spectrum in terms of this auxiliary tensor. 

!!! note
    This section concerns the GW module itself. The expression for the effective anisotropic stress tensor below assumes canonically normalized scalar and gauge sectors. Models with non-canonical interactions, such as non-minimally coupled to gravity scalars or non-canonical kinetic scalar theories, would require to adapt the GW source terms to their non-minimal nature. While we plan to make these additions in a near future, CosmoLattice 2.0 can only run GW dynamics for canonicaly-normalized field models. 

### **Gravitational waves in the continuum** { #subsec_GWcont }

Gravitational waves are tensor perturbations, $i.e.$ transverse-traceless (TT) degrees of freedom of metric perturbations over an isotropic background spacetime. Considering the FLRW solution as the background metric, the perturbed line element (in $\alpha$-time) is
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
where
```math
\begin{align}
\partial_i h_{ij}=0\,~~~\&~~~ h_{ii}=0 \,.
\label{eq_TT}
\end{align}
```

The GW equation of motion is linear and reads
[](){ #eq_GWEOMcontinuum }
```math
\begin{align}
\label{eq_GWEOMcontinuum}
h_{ij}''
+(3-\alpha){a'\over a}h_{ij}'
-a^{-2(1-\alpha)}\nabla^2h_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}\Pi_{ij}^{\rm TT} .
\end{align}
```
The source is the TT part of the anisotropic stress tensor. The full anisotropic stress tensor is defined from the spatial components of the energy-momentum tensor as
[](){ #eq_GWAnisotropicStress }
```math
\begin{align}
\label{eq_GWAnisotropicStress}
\Pi_{ij}
\equiv
T_{ij}-\bar p\,g_{ij},
\end{align}
```
where $\bar p$ is the homogeneous background pressure.

In practice we use an effective anisotropic stress tensor $\Pi^{\rm eff}_{ij}$, defined so that it has the same TT projection as the full tensor, $(\Pi^{\rm eff}_{ij})^{\rm TT}=\Pi^{\rm TT}_{ij}$. The source terms used by CosmoLattice for each field sector are summarized in the subsections below.

The TT projection is non-local in real space, but it is algebraic in Fourier space:
[](){ #eq_GWTTsourceProjection }
```math
\begin{align}
\label{eq_GWTTsourceProjection}
\Pi_{ij}^{\rm TT}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})\Pi_{lm}^{\rm eff}({\bf k},\eta).
\end{align}
```
The continuum TT projector is
[](){ #eq_GWTTprojector }
```math
\begin{align}
\label{eq_GWTTprojector}
\Lambda_{ij,lm}(\hat{\bf k})
&\equiv
P_{il}(\hat{\bf k})P_{jm}(\hat{\bf k})
-{1\over2}P_{ij}(\hat{\bf k})P_{lm}(\hat{\bf k}),
\\
P_{ij}(\hat{\bf k})
&\equiv
\delta_{ij}-\hat k_i\hat k_j,
\hspace{0.5cm}
\hat k_i\equiv {k_i\over k}.
\end{align}
```
This guarantees $k_i\Pi_{ij}^{\rm TT}=0$ and $\Pi_{ii}^{\rm TT}=0$.

Solving Eq.$~$\eqref{eq_GWEOMcontinuum} directly would require a TT projection of the source at every time step. Instead, CosmoLattice evolves auxiliary tensor fields whose sources are local in configuration space, following the idea introduced in Ref. (<span style="color:red;">**2007 w/ Sastre paper**</span>). One option is to choose a symmetric tensor $u_{ij}$ with six independent components,
[](){ #eq_GWuToh }
```math
\begin{align}
\label{eq_GWuToh}
h_{ij}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})u_{lm}({\bf k},\eta),
\end{align}
```
where $u_{ij}$ obeys
[](){ #eq_GWuEOM }
```math
\begin{align}
\label{eq_GWuEOM}
u_{ij}''
+(3-\alpha){a'\over a}u_{ij}'
-a^{-2(1-\alpha)}\nabla^2u_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}\Pi_{ij}^{\rm eff}.
\end{align}
```
A second option is to use only five independent components by evolving a symmetric traceless tensor $v_{ij}$,
[](){ #eq_GWvTraceless }
```math
\begin{align}
\label{eq_GWvTraceless}
v_{33}=-(v_{11}+v_{22}).
\end{align}
```
The $v$-fields obey
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
-{1\over3}\delta_{ij}\Pi_{kk}^{\rm eff}
\right],
\end{align}
```
and the physical tensor perturbations are reconstructed as
[](){ #eq_GWvToh }
```math
\begin{align}
\label{eq_GWvToh}
h_{ij}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})v_{lm}({\bf k},\eta).
\end{align}
```
Using $v_{ij}$ saves one sixth of the memory required by the $u_{ij}$ representation, at the price of computing the traceless part of source at every lattice site. The original idea from Ref.~(<span style="color:red;">**2007 w/ Sastre paper**</span>), based on the $u_{ij}$ 6-component representation, was implemented by default in CosmoLattice v1.1-v1.3. The $v_{ij}$ 5-component representation, proposed more recently in Ref.~(<span style="color:red;">**Art-II**</span>), is however the default representation in CosmoLattice v2.0 and above, (<span style="color:red;">**where one can switch in any case between either representation ?.**</span>)  

The energy density of a GW background is
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
\int {d\rho_{\rm GW}\over d\log k}d\log k .
\end{align}
```
This gives the continuum spectral density
[](){ #eq_GWrhoSpectrumContinuum }
```math
\begin{align}
\label{eq_GWrhoSpectrumContinuum}
\left({d\rho_{\rm GW}\over d\log k}\right)(k,\eta)
=
{m_p^2 k^3\over 8\pi^2a^{2\alpha}V}
\int {d\Omega_k\over4\pi}\,
h'_{ij}(\hat{\bf k},k,\eta)h_{ij}^{\prime *}(\hat{\bf k},k,\eta).
\end{align}
```
For stochastic sources, <span style="color:red;">**WRITE Eq. (2.99) from Art-II ?**</span>  one can define the power spectrum of $h'_{ij}$ as
[](){ #eq_GWhprimePS }
```math
\begin{align}
\label{eq_GWhprimePS}
\left\langle
h'_{ij}({\bf k},\eta)h_{ij}^{\prime *}({\bf k}',\eta)
\right\rangle
=
(2\pi)^3P_{h'}(k,\eta)\delta^{(3)}({\bf k}-{\bf k}').
\end{align}
```
The fractional GW energy density spectrum is then
[](){ #eq_GWOmegaContinuum }
```math
\begin{align}
\label{eq_GWOmegaContinuum}
\Omega_{\rm GW}(k,\eta)
\equiv
{1\over\rho_c}{d\rho_{\rm GW}\over d\log k}
=
{k^3\over 24\pi^2\mathcal H^2}P_{h'}(k,\eta)
=
{k^3\over 24\pi^2a^{2\alpha}H^2}P_{h'}(k,\eta).
\end{align}
```

(<span style="color:red;">**LIST below stop at Abelian-Higgs theories and don't highlight the TBA-cases, instead add a box**</span>)

### **GWs from singlet scalars** { #subsec_SingletGW }

For canonically normalized real singlet scalars $\{\phi_b\}$, the effective anisotropic stress contribution is
[](){ #eq_GWsourceSinglet }
```math
\begin{align}
\label{eq_GWsourceSinglet}
\Pi^{\rm eff}_{ij}\big|_{\rm singlet}
=
\sum_b \partial_i\phi_b\,\partial_j\phi_b .
\end{align}
```
Pressure and other terms proportional to $\delta_{ij}$, have been omitted because they vanish after TT projection.

In the code, this contribution is implemented with forward lattice derivatives of each real singlet scalar:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_singlet_source)

### **GWs from U(1)-charged complex scalars** { #subsec_ComplexGW }

For $U(1)$-charged complex scalars $\{\varphi_b\}$, the effective source is
[](){ #eq_GWsourceComplex }
```math
\begin{align}
\label{eq_GWsourceComplex}
\Pi^{\rm eff}_{ij}\big|_{\rm complex}
=
2\sum_b
{\rm Re}
\left[
\left(D_i^A\varphi_b\right)^*
\left(D_j^A\varphi_b\right)
\right],
\end{align}
```
with $D_i^A$ the $U(1)$ covariant derivative, which contains the corresponding Abelian gauge field(s).

The corresponding source term is computed with the forward $U(1)$ covariant lattice derivative:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_complex_source)

### **GWs from SU(2)-charged doublet scalars** { #subsec_DoubletGW }

For charged doublet scalars $\{\Phi_b\}$, the effective source is
[](){ #eq_GWsourceDoublet }
```math
\begin{align}
\label{eq_GWsourceDoublet}
\Pi^{\rm eff}_{ij}\big|_{\rm doublet}
=
2\sum_b
{\rm Re}
\left[
\left(D_i\Phi_b\right)^\dagger
\left(D_j\Phi_b\right)
\right].
\end{align}
```
The covariant derivative $D_i$ contains the non-Abelian gauge fields under which the doublet is charged.

This is the continuum doublet contribution to the GW source. In the current implementation of `PITensor.h`, the explicit source terms exposed in this file are the singlet scalar, complex scalar, and Abelian gauge contributions shown in the surrounding subsections.

### **GWs from Abelian gauge fields** { #subsec_AbelianGaugeGW }

For Abelian gauge fields, the effective source is written in terms of the Abelian electric and magnetic fields,
[](){ #eq_GWsourceAbelian }
```math
\begin{align}
\label{eq_GWsourceAbelian}
\Pi^{\rm eff}_{ij}\big|_{\rm Abelian}
=
-\left(
a^{-2\alpha}E_iE_j
+a^{-2}B_iB_j
\right).
\end{align}
```
This is the Abelian part of the gauge-field contribution to the anisotropic stress tensor.

In program variables, `PITensor.h` splits this source into electric and magnetic contributions. The electric part is written in terms of the conjugate momentum of the $U(1)$ gauge field, while the magnetic part is built from the Abelian field strength:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_u1_source)

The components of the Abelian magnetic field are obtained from the spatial field-strength tensor as follows:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_u1_magnetic_field)

### **GWs from Non-Abelian gauge fields (TBA)** { #subsec_NonAbelianGaugeGW }

### **GWs from fluids (TBA)** { #subsec_FluidGW }

### **How to activate GWs** { #subsec_ActivateGW }

The GW module is activated from the input parameter file. The relevant parameters can be added to any model input file; for example, the scalar singlet file contains the following GW block ((<span style="color:red;">**FILE ?**</span>)
):

@emgithub(models/parameter-files/lphi4.in:GW_settings)

To evolve gravitational waves in a simulation, set `withGWs = true`. This can be done by editing the input file, or by overriding the parameter directly from the command line, for example
```bash
./lphi4 input=../models/parameter-files/lphi4.in withGWs=true GWprojectorType=2 doLFforGWs=true
```
When the flag `withGWs` is enabled, CosmoLattice will create the auxiliary tensor fields (amplitudes and conjugate momenta), and these will be evolved simulataneously along the matter sectors. Such evolution is **passive**, in the sense that only the sourcing of GWs by the matter sectors is considered, but the backreaction of the GWs onto the matter sectors is not taken into account (as it is expected to be negligible in the majority of cases). When GWs are activated, CosmoLattice will measure the spectrum of the resulting GW background (at the same frequency as the matter sectors' spectra), using a TT-projection of the auxiliary tensor perturbations. 

On the lattice one can build different TT-projectors, see Ref.~<span style="color:red;">**2011 w/Rajantie**</span> for an explanation on this. The parameter `GWprojectorType` selects which lattice TT projector is used when reconstructing $h_{ij}$ from the auxiliary fields in Fourier space. The options in CosmoLattice are:

```text
GWprojectorType = 1  # real projector, built from k^0_L
GWprojectorType = 2  # backward projector, built from k^-_L; default choice
GWprojectorType = 3  # forward projector, built from k^+_L
```

The corresponding lattice momenta are defined explicitly in Eqs.$~$\eqref{eq_GWLatticeMomenta}-\eqref{eq_GWComplexProjector} below. If `GWprojectorType` is not specified, CosmoLattice uses the default the value `2`. **The GW module is implemented for three spatial dimensions, so simulations with a different number of spatial dimensions must keep `withGWs = false`, as the notion of GWs, and their mere existence, must be revised in those cases**.

### **Evolvers used for GWs** { #subsec_GWEvolvers }

The GW sector has its own evolver choice, controlled by `doLFforGWs`, which by default, `= true`, and the tensor sector is evolved with the leapfrog algorithm even if the matter fields use a different evolver. If `doLFforGWs = false`, the GW sector uses the same evolver as the matter fields, as specified by the usual `evolver` parameter. The top-level evolver dispatch applies the GW leapfrog kicks and drifts separately whenever `typeGW == LF`:

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_dispatch)

Before measurements, GWs are synchronized in case they are been evolved with leapfrog:

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_sync)

There is an important restriction for models containing gauge fields. Since the GW source depends on <!-- both magnetic fields and --> electric fields, and <!-- the latter --> these are conjugate momenta, the code rejects evolver combinations that do not synchronize fields and momenta consistently. In practice, for scalar-only models the default `doLFforGWs = true` is the standard choice, since there is no problem in this case. For models with $U(1)$ gauge fields and GWs, however, one cannot use `evolver = LF` for the matter sector; We need to use, instead, a synchronized matter evolver such as `VVn`, `PVn`, or the `RK` family. If the matter sector uses a position-Verlet evolver in a $U(1)$ model, one can still keep the evolution of the GW sector on leapfrog. (<span style="color:red;">**No queda clara la lógica de la casuistica aquí, i.e. el por qué. A table with combinations enable/disable would be desirable**</span>)


### **Gravitational waves on the lattice** { #subsec_GWlattice }

On the lattice, CosmoLattice evolves the auxiliary $u$- or $v$-fields in configuration space and applies the TT projection only when reconstructing the physical GW field $h_{ij}$ for measurements (<span style="color:red;">**SWITCH between u_ij and v_ij in CosmoLattice ?**</span>). Below we describe the $v$-field implementation; the $u$-field case is obtained by replacing the traceless source by $\Pi_{ij}^{\rm eff}$.

The program variables for space and time follow the standard CosmoLattice definitions. The auxiliary tensor is also rescaled as
[](){ #eq_GWProgramVars }
```math
\begin{align}
\label{eq_GWProgramVars}
\tilde v_{ij}
=
\left({m_p\over f_*}\right)^2v_{ij}.
\end{align}
```
The conjugate momenta are $(\tilde\pi_v)_{ij}=a^{3-\alpha}\tilde v'_{ij}$, and the lattice evolution system is
[](){ #eq_GWFirstOrderSystem }
```math
\begin{align}
\label{eq_GWFirstOrderSystem}
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
\right].
\end{align}
```

The corresponding GW kernel contains the lattice Laplacian of the auxiliary tensor field and the effective anisotropic tensor source:

@emgithub(include/CosmoInterface/evolvers/kernels/gwskernels.h:gws_kernel_source)

The effective anisotropic stress tensor in program units is
[](){ #eq_GWProgramStress }
```math
\begin{align}
\label{eq_GWProgramStress}
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
\right].
\end{align}
```
For scalar sources this amounts to replacing fields and derivatives by their program-variable counterparts. For Abelian gauge fields, the source contains the additional factor $(\omega_*^2/f_*^2)$ multiplying the products of program electric and magnetic fields.

The implemented source contributions are assembled in `PITensor.h`:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_total_source)

The physical GW field is reconstructed in Fourier space with a lattice TT projector,
[](){ #eq_GWhLattice }
```math
\begin{align}
\label{eq_GWhLattice}
h_{ij}(\tilde{\bf n},\eta)
=
\Lambda^{\rm L}_{ij,lm}(\tilde{\bf n})v_{lm}(\tilde{\bf n},\eta).
\end{align}
```
The lattice projector is
[](){ #eq_GWTTProjectorLattice }
```math
\begin{align}
\label{eq_GWTTProjectorLattice}
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
|{\bf k}_{\rm L}(\tilde{\bf n})|^2}.
\end{align}
```
Here ${\bf k}_{\rm L}$ is the lattice momentum associated with the derivative operator used in the TT projection. Writing $\theta_i\equiv 2\pi\tilde n_i/N$, the three choices used by `GWprojectorType` are
[](){ #eq_GWLatticeMomenta }
```math
\begin{align}
\label{eq_GWLatticeMomenta}
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
\end{align}
```
The overall factor $1/\delta\tilde x$ has been omitted in Eq.$~$\eqref{eq_GWLatticeMomenta}, because it cancels in the projector ratios (<span style="color:red;">**only Eq 31 ?**</span>)
. Equivalently, one may multiply every component by $1/\delta\tilde x$ when interpreting ${\bf k}_{\rm L}$ as a dimensionful lattice momentum. The neutral momentum ${\bf k}^0_{\rm L}$ is real, while the forward and backward momenta ${\bf k}^{\pm}_{\rm L}$ are complex.

For neutral derivatives, the lattice momentum is real and the projector can be written as
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
For the forward or backward choices in Eq.$~$\eqref{eq_GWLatticeMomenta} (<span style="color:red;">**other eqs**</span>), the lattice momentum is complex and the projector is
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
The lattice projector is transverse with respect to the lattice derivative used to define it. For the complex projector, it is also Hermitian and idempotent, properties that are essential for a consistent lattice TT projection.

After the TT projection, the GW energy density on the lattice follows from the discrete analogue of Eq.$~$\eqref{eq_GWrhoContinuum}:
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
Splitting the Fourier sum into spherical shells gives
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
\Delta\log k .
\end{align}
```
The corresponding Type-II lattice spectrum is
[](){ #eq_GWDiscreteSpectrum }
```math
\begin{align}
\label{eq_GWDiscreteSpectrum}
\left({d\rho_{\rm GW}\over d\log k}\right)(l)
=
{m_p^2k^3(l)\over8\pi^2a^{2\alpha}L^3}
\left\langle
\left[\delta x^3h'_{ij}(|\tilde{\bf n}|,\eta)\right]
\left[\delta x^3h'_{ij}(|\tilde{\bf n}|,\eta)\right]^*
\right\rangle_{R(l)}.
\end{align}
```
Normalizing by the critical energy density gives
[](){ #eq_GWOmegaTypeII }
```math
\begin{align}
\label{eq_GWOmegaTypeII}
\Omega_{\rm GW}(\tilde{\bf n},\eta)
=
{1\over\rho_c}
{m_p^2k^3(l)\over8\pi^2a^{2\alpha}}
{\delta x^3\over N^3}
\left\langle
h'_{ij}(\tilde{\bf n},\eta)h_{ij}^{\prime *}(\tilde{\bf n},\eta)
\right\rangle_{R(\tilde{\bf n})}
\hspace{0.5cm}
[{\rm Type~II}].
\end{align}
```
The analogous Type-I lattice spectrum is
[](){ #eq_GWOmegaTypeI }
```math
\begin{align}
\label{eq_GWOmegaTypeI}
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
[{\rm Type~I}].
\end{align}
```

In the lattice code, the relevant bilinear $h'_{ij}h_{ij}^{\prime *}$ can be computed directly from the conjugate momenta of the auxiliary fields:
[](){ #eq_GWhprimeFromMomentum }
```math
\begin{align}
\label{eq_GWhprimeFromMomentum}
h'_{ij}
=
{\omega_*\over a^{3-\alpha}}
\Lambda^{\rm L}_{ij,lm}(\pi_v)_{lm}
=
{\omega_*\over a^{3-\alpha}}
\left({f_*\over m_p}\right)^2
\Lambda^{\rm L}_{ij,lm}(\tilde\pi_v)_{lm}.
\end{align}
```
This bilinear can be written compactly in terms of traces,
[](){ #eq_GWTraceFormula }
```math
\begin{align}
\label{eq_GWTraceFormula}
h'_{ij}h_{ij}^{\prime *}
=
{\rm Tr}({\tt P}{\tt v}'{\tt P}{\tt v}^{\prime *})
-{1\over2}
{\rm Tr}({\tt P}{\tt v}')
{\rm Tr}({\tt P}{\tt v}^{\prime *}).
\end{align}
```
Here ${\tt v}'$ and ${\tt P}$ are matrices with entries $({\tt v}')_{ij}=v'_{ij}$ and $({\tt P})_{ij}=P^{\rm L}_{ij}$.

### **GW output** { #subsec_GWOutput }

When `withGWs = true`, CosmoLattice generates the standard output of the matter sector and, in addition, two GW-specific output files. These files are written inside the directory specified by `outputfile`.

-  `average_energies_gws.txt`: Volume-averaged GW energy quantities, printed at the same frequency as the GW spectrum measurements. The columns are

$\hspace{1cm}$ $\tilde{\eta}$, $\tilde{E}_{\rm GW}/\tilde{E}_{\rm matter}$, $\tilde{E}_{\rm GW}$.

Here $\tilde{E}_{\rm GW}/\tilde{E}_{\rm matter}$ is obtained by integrating the measured GW energy-density spectrum over the available momentum bins. The quantity $\tilde{E}_{\rm GW}$ is this fraction multiplied by the total matter energy density at the same time.

-  `spectra_energy_gws.txt`: GW fractional energy-density spectrum measured from the projected tensor perturbations. Its printing frequency is controlled by `tOutputInfreq`, together with the other spectra in the simulation. The file contains

$\hspace{1cm}$ $\tilde{k}$, $\Omega_{\rm GW}(\tilde{k})$, $\Delta n_{\rm bin}$,

up to the additional bin information requested by the usual spectrum-output parameters. The first column is the momentum scale associated with the bin, the second column is the GW spectrum computed with the projector selected by `GWprojectorType`, and the last column gives the bin multiplicity. For higher `spectraVerbosity`, CosmoLattice also prints extra bin statistics, analogously to the scalar spectra described in Section [*Outputs*](My first model of (singlet) scalar fields.md#outputs).

-  `average_spectra_times.txt`: List of times at which the GW spectrum and the other infrequent spectra are outputted.

If `hdf5Averages = true`, the averaged GW quantities are written into the common HDF5 averages file instead of `average_energies_gws.txt`. If `hdf5Spectra = true`, the GW spectrum is written into the common spectra HDF5 file instead of `spectra_energy_gws.txt`. If `saveUnbinnedSpectra = true`, the corresponding unbinned GW spectrum is saved through the standard unbinned-spectrum HDF5 output.
