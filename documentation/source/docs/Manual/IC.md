<!-- <div style="text-align: justify;"> -->

Here we explain how to implement the initial conditions for the different matter sectors.

### **Singlet scalars** { #subsec_SingletIC }

#### Default scalar singlet initialization { #subsubsec_DefaultSingletIC }

The default initialization of scalar singlet fluctuations follows the prescription described in Section [*Initialization of fluctuations*](My first model of (singlet) scalar fields.md#sec_InitScalar) of the scalar-singlet example. If no external spectrum is provided for scalar field `i`, meaning that `ext_PSi` is absent or set to `none`, CosmoLattice samples the field and its conjugate momentum from the default Gaussian vacuum prescription.

For a scalar singlet $f$, in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we introduce by default a continuum spectrum mimics quantum vaccum fluctuations as
[](){ #eq_ICDefaultScalarSpectrum }
```math
\begin{align}
\label{eq_ICDefaultScalarSpectrum}
\Delta_{\delta f}(k)
&\equiv
{k^3\over 2\pi^2}\mathcal{P}_{\delta f}(k),
\hspace{0.6cm}
\mathcal{P}_{\delta f}(k)
\equiv
{1\over 2a^2\omega_{k,f}},
\\
\omega_{k,f}
&\equiv
\sqrt{k^2+a^2m_f^2},
\hspace{0.6cm}
m_f^2
\equiv
\frac{\partial^2V}{\partial f^2}
\bigg|_{f=\bar f_*}.
\end{align}
```

In program variables, the initial Fourier modes are built from two independent fields (represeting *left-* and *right-moving* waves)
[](){ #eq_ICDefaultScalarModes }
```math
\begin{align}
\label{eq_ICDefaultScalarModes}
\delta\tilde f(\tilde{\bf n})
&=
{1\over\sqrt{2}}
\left[
\delta\tilde f_1(\tilde{\bf n})
+\delta\tilde f_2(\tilde{\bf n})
\right],
\\
\delta\tilde f'(\tilde{\bf n})
&=
{i\tilde\omega_k\over\sqrt{2}}
\left[
\delta\tilde f_1(\tilde{\bf n})
-\delta\tilde f_2(\tilde{\bf n})
\right]
-\tilde{\mathcal H}\delta\tilde f(\tilde{\bf n}) .
\end{align}
```

In these expressions, $\delta \tilde f_{1,2}$ are both complex fields, each with their real and imaginary parts, say $\delta \tilde f_{\rm R} + i\delta \tilde f_{\rm I}$. At each point $\tilde{\bf n}$ of the reciprocal lattice, the code draws independent random realizations of the real ($\delta \tilde f_{\rm R}$) and imaginary ($\delta \tilde f_{\rm I}$) parts of each of the fields $\delta \tilde f_{1,2}$, from a Gaussian distribution with vanishing mean and variance
[](){ #eq_ICDefaultScalarVariance }
```math
\begin{align}
\label{eq_ICDefaultScalarVariance}
\sigma_{\delta f}(\tilde{\bf n})
=
H(\tilde k_{\rm cut}-\tilde k)
\left({\omega_*\over f_*}\right)
\left({N\over \delta \tilde{x}}\right)^{3/2}
{1\over\sqrt{2}}
{1\over \sqrt{2\tilde\omega_k}},
\hspace{0.6cm}
\tilde\omega_k=\sqrt{\tilde k^2+\tilde m_f^2}\,,
\end{align}
```
where we have fixed the scale factor to $a = 1$. The different factors in the above formula, are written because of various of reasons: $\left({\omega_*\over f_*}\right)$ is due to the use of program variables, $\left({N\over \delta \tilde{x}}\right)^{3/2}$ is due to lattice discretization effects (see *e.g.* [Technical Note I][note_I] in [*Technical Notes*](../th_framework/TechnicalNotes.md) of the [*Theory Framework*](../th_framework/TF_readme.md) Tab), and the extra factor ${1\over\sqrt{2}}$ multiplying $\mathcal{P}_{\delta f} (\tilde{\bf n})$ is choosen to guarantee that $\langle \delta \tilde f_1^2 \rangle $ = $\langle \delta \tilde f_1^2 \rangle \equiv$ $\left({\omega_*/f_*}\right)^2\left({N/\delta \tilde{x}}\right)^3\times{1\over2}\mathcal{P}_{\delta f} (\tilde{\bf n})$, and hence $\langle |\delta \tilde f ({\bf\tilde{n}})|^2 \rangle = \left({\omega_*/ f_*}\right)^2\left({N/\delta \tilde{x}}\right)^3\times\widetilde{\mathcal{P}}_{\delta \phi} (\tilde{\bf n})$ in program units, or equivalently $\langle |\delta f ({  \bf \tilde{n}})|^2 \rangle = \mathcal{P}_{\delta f} (k(\tilde{\bf n}))$ in physical units, as desired, with $\mathcal{P}_{\delta f}$ given above in Eq.$~$\eqref{eq_ICDefaultScalarSpectrum}.

**Note -.** Eq.$~$\eqref{eq_ICDefaultScalarVariance} is the expression implemented in the code for simulations in 3-spatial dimensions. In 1- and 2-dimensional spatial simulations, the prefactors are modified as shown in the same function; See Section 7.1 of $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]). 

The scalar initializer does the above oeprations through

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:default_scalar_dispatch)

The mode frequency and the optional UV cutoff entering the fluctuation amplitude are built as

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_frequency_cutoff)

For three-dimensional simulations, the normalization of each real Gaussian component of the Fourier mode is

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_norm_3d)

This normalization multiplies a random Gaussian field,

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_random_gaussian_field)

and the sampled left- and right-moving modes are combined into the field fluctuation as

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_field_modes)

The conjugate momentum fluctuation uses the same two sampled modes, with the relative phase and Hubble term shown in the scalar-singlet prescription:

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_momentum_modes)

After the fluctuations are set, the homogeneous modes supplied by `initial_amplitudes` and `initial_momenta` are added in program variables:

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:default_scalar_homogeneous_modes)

#### External power spectrum for scalar singlet initialization { #subsubsec_ExternalPSSingletIC }

Scalar singlet fields can be initialized from a user-supplied external power spectrum. For the scalar field with index `i`, the corresponding input parameter is
```text
ext_PSi = path/to/spectrum.dat
```
For example, `ext_PS0` refers to scalar field `0`, `ext_PS1` to scalar field `1`, and so on. If the value is `none`, CosmoLattice initializes that field with the default scalar prescription. If a file path is supplied, the external spectrum fixes the variance of the initial Fourier modes of that scalar field.

For a scalar singlet field $f$, the continuum spectrum may be written as
[](){ #eq_ICExternalSpectrum }
```math
\begin{align}
\label{eq_ICExternalSpectrum}
\left\langle \delta f^2 \right\rangle
= \int d\log k~ \Delta_{\delta f}(k),\hspace{0.6cm}
\Delta_{\delta f}(k) \equiv {k^3\over 2\pi^2}\mathcal{P}_{\delta f}(k),\hspace{0.6cm}
\left\langle \delta f_{\bf k}\delta f^*_{{\bf k}'} \right\rangle
= (2\pi)^3\mathcal{P}_{\delta f}(k)\delta({\bf k}-{\bf k}') .
\end{align}
```
The spectrum $\Delta_{\delta f}$ carries the dimensions of $\delta f^2$ and is invariant under a constant re-normalization of the scale factor. Equivalently, the Fourier-space spectrum $\mathcal{P}_{\delta f}$ scales as $a^{-3}$ when written as a function of the physical momentum $p=k/a$. This is the normalization convention assumed when the external spectrum is converted into program units and used on the lattice.

On the lattice, the external spectrum is interpreted through the discrete counterpart of this continuum definition,
[](){ #eq_ICExternalSpectrumLattice }
```math
\begin{align}
\label{eq_ICExternalSpectrumLattice}
\Delta_f(k(|{\bf \tilde n}|))
&\equiv
{k(\tilde{\bf n})\over 2\pi}
{\delta x\over N^5}
\#_{R(\tilde{\bf n})}
\left\langle
\left| f(\tilde{\bf n})\right|^2
\right\rangle_{R(\tilde{\bf n})}
\\
&=
{k^3(\tilde{\bf n})\over 2\pi^2}
\Upsilon_{|\tilde{\bf n}|}
\left({\delta x\over N}\right)^3
\left\langle
\left| f(\tilde{\bf n})\right|^2
\right\rangle_{R(\tilde{\bf n})} .
\end{align}
```
Here $R(\tilde{\bf n})$ denotes the spherical shell of lattice momenta around $|\tilde{\bf n}|$, $\#_{R(\tilde{\bf n})}$ is the number of Fourier sites in that shell, and
[](){ #eq_ICExternalSpectrumUpsilon }
```math
\begin{align}
\label{eq_ICExternalSpectrumUpsilon}
\Upsilon_{|\tilde{\bf n}|}
\equiv
{\#_{R(\tilde{\bf n})}\over 4\pi|\tilde{\bf n}|^2} .
\end{align}
```
The input flag `PS_type`, referred to as `PSType` internally, selects how this multiplicity factor is treated. `PSType = 1` selects Type-I spectra, which use the exact lattice multiplicity $\#_{R(\tilde{\bf n})}$ and keep $\Upsilon_{|\tilde{\bf n}|}$ explicitly. `PSType = 2` selects Type-II spectra, which use the continuum shell multiplicity $\#_{R(\tilde{\bf n})}=4\pi|\tilde{\bf n}|^2$, so $\Upsilon_{|\tilde{\bf n}|}=1$.

In the scalar initializer, a non-empty `ext_PSi` entry dispatches the corresponding scalar field and its conjugate momentum to the external-spectrum initializer:

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:externalps_scalar_dispatch)

Identifying the shell average with the statistical average of the Gaussian realization gives the variances used to initialize each Fourier mode from the external spectrum. In program units,
[](){ #eq_ICExternalSpectrumVariance }
```math
\begin{align}
\label{eq_ICExternalSpectrumVariance}
\left\langle
\left|\tilde f(\tilde{\bf n})\right|^2
\right\rangle_{\rm stat}
&=
{\mathcal C_*\over \Upsilon_{|\tilde{\bf n}|}}
\left({N\over \delta\tilde x}\right)^3
{\widetilde{\mathcal F}(\kappa/a)\over a^3},
\\
\left\langle
\left|\tilde f'(\tilde{\bf n})\right|^2
\right\rangle_{\rm stat}
&=
{\mathcal D_*\over \Upsilon_{|\tilde{\bf n}|}}
\left({N\over \delta\tilde x}\right)^3
{\widetilde{\mathcal G}(\kappa/a)\over a^{3-2\alpha}} .
\end{align}
```
The normalization constants are
[](){ #eq_ICExternalSpectrumNorms }
```math
\begin{align}
\label{eq_ICExternalSpectrumNorms}
\mathcal C_*
&\equiv
\left\{
\begin{array}{cl}
\omega_*^3/(f_*^2E_*) & [{\rm general}],\\
(\omega_*/f_*)^2 & [E_*=\omega_*],
\end{array}
\right.
\\
\mathcal D_*
&\equiv
\left\{
\begin{array}{cl}
\omega_*M_*/f_*^2 & [{\rm general}],\\
(\omega_*/f_*)^2 & [M_*=\omega_*].
\end{array}
\right.
\end{align}
```
In the code, the external file is interpolated on the lattice momenta before the variances are built:

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_interpolators)

The two choices of `PS_type` are implemented by the following normalization factors. For Type-I spectra, the exact lattice multiplicity enters through `MomentumMultiplicity`:

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_typeI_norm)

For Type-II spectra, the continuum shell multiplicity has already been assumed:

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_typeII_norm)

CosmoLattice then draws the real and imaginary parts of $\tilde f(\tilde{\bf n})$ and $\tilde f'(\tilde{\bf n})$ from Gaussian distributions with zero mean and variance equal to one half of the expressions above. Equivalently, one may view the same initialization as drawing random phases and Rayleigh-distributed amplitudes whose expected square amplitudes are fixed by the supplied spectrum. In the code this is the product of the normalization factor with a random Gaussian field:

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_random_gaussian_field)

Finally, the sampled left- and right-moving Gaussian modes are combined to build the Fourier-space initial field configuration,

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_sample_field_modes)

and the corresponding conjugate momentum configuration:

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_sample_momentum_modes)

The same power-spectrum normalization also applies to any supplied initial spectrum for $\delta f'$. In that case the spectrum fixes $\left\langle \delta f'^2 \right\rangle$, with the additional $a^{2\alpha}$ scaling associated with the $\alpha$-time derivative used by CosmoLattice. The convention used to produce the external file should match the value of `PS_type` in the run that reads it, and momenta in the external file must be expressed in the units expected by the initializer, namely the same units as $\omega_*$.

!!! note
    Type-I external-spectrum initialization is supported for three-dimensional lattices. For lower-dimensional runs, adapt $\Upsilon_{|\tilde{\bf n}|}$ according to the dimension $d$.

### **Complex scalars** { #subsec_ComplexIC }

Complex scalars are initialized by the Abelian-sector initializer, even in models with complex scalars but no dynamical U(1) gauge field. There is no independent input flag `ICtype_CS`. Instead, the complex scalar initialization is selected when the U(1) initial-condition type is `RandomWithMatter`, or when `ICtype_U1 = Default` and the model contains at least one complex scalar.

The initializer first samples fluctuations for each complex scalar and then adds the homogeneous values supplied by the model. In input-parameter language these homogeneous values are usually set by the model through `initial_amplitudes` and `initial_momenta`, or by model-specific aliases for the complex scalar initial norm and initial momentum norm.

The purpose of this special initialization is to generate charged matter fluctuations without introducing a homogeneous charge density. As in the singlet case, each real component is written as a sum of left- and right-moving waves. For charged fields, however, the left- and right-moving amplitudes of each real component are taken equal, and some phases are constrained. This is the minimal condition used by the code to make the zero mode of the electric charge density vanish before the gauge momenta are solved from Gauss' law.

For a complex scalar written as two real components $\varphi_0$ and $\varphi_1$, the Fourier-space fluctuation is built as
[](){ #eq_ICComplexScalarModes }
```math
\begin{align}
\label{eq_ICComplexScalarModes}
\delta\tilde\varphi_n(\tilde{\bf n})
&=
{1\over\sqrt{2}}
\left[
|\delta\tilde\varphi^{(l)}_n(\tilde{\bf n})|
e^{i\theta^{(l)}_n(\tilde{\bf n})}
+
|\delta\tilde\varphi^{(r)}_n(\tilde{\bf n})|
e^{i\theta^{(r)}_n(\tilde{\bf n})}
\right],
\hspace{0.6cm}
n=0,1 .
\end{align}
```
The amplitudes are Rayleigh-distributed and use the same vacuum fluctuation normalization as scalar singlets, with the corresponding complex-scalar mass matrix entries. In the code an additional factor $1/\sqrt{2}$ is applied for the conversion between a complex field and its two real components.

The zero-mode electric charge condition is enforced mode by mode through
[](){ #eq_ICComplexScalarZeroCharge }
```math
\begin{align}
\label{eq_ICComplexScalarZeroCharge}
\mathrm{Re}
\left[
\tilde\varphi'_1(\tilde{\bf n})
\tilde\varphi^*_0(\tilde{\bf n})
-
\tilde\varphi'_0(\tilde{\bf n})
\tilde\varphi^*_1(\tilde{\bf n})
\right]
=
0 .
\end{align}
```
In the notation of Eq.$~$\eqref{eq_ICComplexScalarModes}, CosmoLattice realizes this by imposing
[](){ #eq_ICComplexScalarPhaseConstraint }
```math
\begin{align}
\label{eq_ICComplexScalarPhaseConstraint}
|\delta\tilde\varphi^{(l)}_0|
&=
|\delta\tilde\varphi^{(r)}_0|,
\hspace{0.6cm}
|\delta\tilde\varphi^{(l)}_1|
=
|\delta\tilde\varphi^{(r)}_1|,
\\
\theta^{(r)}_1
&=
\theta^{(r)}_0
+
\theta^{(l)}_1
-
\theta^{(l)}_0 .
\end{align}
```
The code draws two independent amplitudes and three independent phases, uses the same amplitude for the left- and right-moving waves of each component, and fixes the remaining phase with Eq.$~$\eqref{eq_ICComplexScalarPhaseConstraint}. Because of this constraint, the complex-scalar fluctuations are not a completely independent Gaussian draw component by component, although their amplitudes follow the same fluctuation spectrum as the default scalar prescription.

The conjugate momenta are built from the difference of the two waves and include the Hubble-drag term, in direct analogy with the scalar singlet case,
[](){ #eq_ICComplexScalarMomenta }
```math
\begin{align}
\label{eq_ICComplexScalarMomenta}
\delta\tilde\varphi'_n(\tilde{\bf n})
=
-{i\tilde\omega_{k,n}\over\sqrt{2}}
\left[
|\delta\tilde\varphi^{(l)}_n|
e^{i\theta^{(l)}_n}
-
|\delta\tilde\varphi^{(r)}_n|
e^{i\theta^{(r)}_n}
\right]
-\tilde{\mathcal H}\delta\tilde\varphi_n(\tilde{\bf n}) .
\end{align}
```
The zero mode of the fluctuation is set to zero. The homogeneous complex scalar field and momentum are then added to the zero mode in program units.

The corresponding code path is `initializeCScalar` and `addFluctuationsCScalarFromPhases` in `include/CosmoInterface/initializers/u1initializer.h`. The constrained phase choice is implemented as

@emgithub(include/CosmoInterface/initializers/u1initializer.h:complex_scalar_constrained_phases)

and the field and momentum modes are then assigned through

@emgithub(include/CosmoInterface/initializers/u1initializer.h:complex_scalar_modes)

### **Doublet scalars** { #subsec_DoubletIC }

SU(2) doublet scalars are initialized by the non-Abelian-sector initializer. They do not have an independent run-parameter flag analogous to `ICtype_S`. When a model contains SU(2) doublets, CosmoLattice initializes the doublet fields first and then uses their charge density to initialize the non-Abelian gauge momenta.

The structure is the same as for the complex scalar initialization, but now the doublet is represented by four real components $h_a$, with $a=0,1,2,3$. The homogeneous doublet norm supplied by the model is distributed among the real components by the model definition. Fluctuations are then added component by component,
[](){ #eq_ICDoubletScalarModes }
```math
\begin{align}
\label{eq_ICDoubletScalarModes}
\delta\tilde h_a(\tilde{\bf n})
=
{1\over\sqrt{2}}
\left[
|\delta\tilde h^{(l)}_a(\tilde{\bf n})|
e^{i\theta^{(l)}_a(\tilde{\bf n})}
+
|\delta\tilde h^{(r)}_a(\tilde{\bf n})|
e^{i\theta^{(r)}_a(\tilde{\bf n})}
\right],
\hspace{0.6cm}
a=0,1,2,3 .
\end{align}
```
The zero-mode charge constraints can be written as
[](){ #eq_ICDoubletScalarZeroCharge }
```math
\begin{align}
\label{eq_ICDoubletScalarZeroCharge}
\mathrm{Re}
\left[
\tilde h'_m(\tilde{\bf n})
\tilde h^*_0(\tilde{\bf n})
-
\tilde h'_0(\tilde{\bf n})
\tilde h^*_m(\tilde{\bf n})
\right]
=
0,
\hspace{0.6cm}
m=1,2,3 .
\end{align}
```
CosmoLattice imposes these conditions with the same choice used in `IC.txt`,
[](){ #eq_ICDoubletScalarPhaseConstraint }
```math
\begin{align}
\label{eq_ICDoubletScalarPhaseConstraint}
|\delta\tilde h^{(l)}_a|
&=
|\delta\tilde h^{(r)}_a|,
\hspace{0.6cm}
a=0,1,2,3,
\\
\theta^{(r)}_m
&=
\theta^{(r)}_0
+
\theta^{(l)}_m
-
\theta^{(l)}_0,
\hspace{0.6cm}
m=1,2,3 .
\end{align}
```
The left-moving phases are sampled independently. The right-moving phase of the first component is also sampled independently, while the other right-moving phases are fixed by Eq.$~$\eqref{eq_ICDoubletScalarPhaseConstraint}. The equal left/right moduli and the three phase constraints ensure that the homogeneous modes of the SU(2) charge densities vanish initially. The gauge momenta can then be obtained from the lattice Gauss constraints without introducing a net charge.

The momenta are initialized as
[](){ #eq_ICDoubletScalarMomenta }
```math
\begin{align}
\label{eq_ICDoubletScalarMomenta}
\delta\tilde h'_a(\tilde{\bf n})
=
-{i\tilde\omega_{k,a}\over\sqrt{2}}
\left[
|\delta\tilde h^{(l)}_a|
e^{i\theta^{(l)}_a}
-
|\delta\tilde h^{(r)}_a|
e^{i\theta^{(r)}_a}
\right]
-{\tilde{\mathcal H}\over\sqrt{2}}\delta\tilde h_a(\tilde{\bf n}) .
\end{align}
```
After the fluctuation zero mode is set to zero, the model-supplied homogeneous doublet field and momentum are added.

The corresponding code path is `initializeSU2Doublet` and `addFluctuationsSU2DoubletFromPhases` in `include/CosmoInterface/initializers/su2initializer.h`. The constrained right-moving phases are set by

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2_doublet_constrained_phases)

and the doublet field and momentum modes are assigned through

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2_doublet_modes)

### **Abelian gauge fields** { #subsec_AbelianGaugeIC }

The Abelian sector is controlled by the input parameter `ICtype_U1`. If `ICtype_U1 = Default`, the model chooses the initialization according to its matter content:

| Matter content | Default Abelian IC |
|---|---|
| Complex scalars present | `RandomWithMatter` |
| No complex scalars, but scalar-U(1) axion couplings present | `BunchDavisTransverseU1` |
| Otherwise | `PlaneWavesZeroB` |

The U(1) initializer implements three choices: `RandomWithMatter`, `PlaneWavesZeroB`, and `BunchDavisTransverseU1`. Other names may be accepted by the parser, such as `PlaneWaves`, `DefectsNetwork`, and `DefectsWhiteNoise`, but they are not dispatched by the U(1) initializer and lead to an error.

#### Random matter with constrained Abelian gauge momenta { #subsubsec_RandomWithMatterU1IC }

For `ICtype_U1 = RandomWithMatter`, CosmoLattice first initializes the complex scalar fields as described in Section [*Complex scalars*](#subsec_ComplexIC). If the model also contains U(1) gauge fields, their initial vector potentials are set to zero and their momenta are determined by the lattice Gauss constraint sourced by the matter charge density.

This is the lattice version of the continuum prescription $A_i(\mathbf{x},\eta_*)=0$, with only the electric field initialized. Since the vector potential vanishes initially, the magnetic energy is initially zero. The electric field is then fixed by
[](){ #eq_ICU1ContinuumGauss }
```math
\begin{align}
\label{eq_ICU1ContinuumGauss}
\partial_i A'_i(\mathbf{x})
=
J^A_0(\mathbf{x}),
\hspace{0.6cm}
A'_i(\mathbf{k})
=
i{k_i\over k^2}J^A_0(\mathbf{k}),
\hspace{0.6cm}
\mathbf{k}\neq \mathbf{0}.
\end{align}
```
The zero mode of $J^A_0$ must vanish for this equation to be solvable at $\mathbf{k}=0$; this is the reason for the constrained charged-scalar phases above.

In Fourier space, the code solves
[](){ #eq_ICU1GaussSolve }
```math
\begin{align}
\label{eq_ICU1GaussSolve}
\tilde\Pi^{(a)}_{A_i}(\tilde{\bf n})
=
{(k^-_{L,i})^*\over |{\bf k}^-_L|^2}
\tilde j^{(a)}_0(\tilde{\bf n}),
\hspace{0.6cm}
\tilde j^{(a)}_0
=
-\delta\tilde x^2\,
\widetilde{J^{(a)}_0},
\hspace{0.6cm}
\tilde A^{(a)}_i=0 .
\end{align}
```
Here $a$ labels the Abelian gauge field and $k^-_{L,i}=1-\exp(-2\pi i n_i/N)$ is the backward lattice momentum used by the implementation. The zero mode of the gauge momentum is set to zero.

The corresponding code path is `initializeRandomWithMatterU1` in `include/CosmoInterface/initializers/u1initializer.h`:

@emgithub(include/CosmoInterface/initializers/u1initializer.h:u1_gauss_solve)

#### Transverse plane waves with zero magnetic field { #subsubsec_PlaneWavesZeroBU1IC }
 
For `ICtype_U1 = PlaneWavesZeroB`, CosmoLattice generates transverse plane-wave fluctuations for the gauge momentum using two polarizations. The gauge field itself is then set to zero,
[](){ #eq_ICU1PlaneWavesZeroB }
```math
\begin{align}
\label{eq_ICU1PlaneWavesZeroB}
\tilde A_i^{(a)}(\tilde{\bf n})=0,
\hspace{0.6cm}
k^-_{L,i}\tilde\Pi^{(a)}_{A_i}(\tilde{\bf n})=0 .
\end{align}
```
Thus the initial magnetic field vanishes, while the electric field is a transverse random plane-wave configuration. The zero modes are set to zero.

The corresponding code path is `initializePlaneWavesZeroBU1` in `include/CosmoInterface/initializers/u1initializer.h`.

#### Bunch-Davies transverse Abelian fields { #subsubsec_BunchDaviesTransverseU1IC }

When `ICtype_U1 = BunchDavisTransverseU1` is selected, CosmoLattice initializes the gauge and electric fields using the Bunch-Davies (BD) vacuum solution. The BD solutions for the chiral modes are given by:
```math
A^{\pm}_k = \frac{1}{\sqrt{2k}}e^{ik/aH} \;, \quad E^{\pm}_k = -\frac{i}{a} \sqrt{\frac{k}{2}}e^{ik/aH}  \;.
```
This initialization implies that both fields are strictly transverse, meaning their longitudinal components are exactly zero. 

To achieve this, the initialization routine calls `BunchDavisTransverseU1` (located in $\texttt{CosmoInterface/initializers/externalpowerspectruminitializer.h}$), which executes the following sequence of steps:

1. **RGF Initialization**: First, it generates Random Gaussian Field fluctuations with a unit variance for both the gauge and electric fields. This is performed in the exact same way as described in [*External power spectrum for scalar singlet initialization*][subsubsec_ExternalPSSingletIC], but applied independently to each spatial component of the vector field.

2. **Transverse Projection**: A transverse projector is constructed and applied to the generated fluctuations. The discrete lattice transverse projector in momentum space is defined as:
    ```math
    \Pi^{L}_{ij}(\tilde{\mathbf{n}}) = \delta_{ij} - \frac{k_{\text{L},i} k_{\text{L},j}}{k_{\text{L}}^2} \;.
    ```
    
    Physically, the BD vacuum solutions correspond to the chiral basis. To ensure the absence of a longitudinal part in the Cartesian basis (i.e., enforcing $\nabla \cdot \mathbf{E} = 0$), we must project out the longitudinal modes. Since the BD amplitudes for both the plus ($+$) and minus ($-$) chiral polarizations are identical, applying this standard symmetric transverse projector is entirely sufficient. This avoids the need for the chiral projector utilized in [*Output*][subsubsec_output] of [*Axion-Gauge Interactions*](ALP.md).

3. **Phase Shift Application**: The correct relative phase shift between the gauge field and the electric field is properly established. In complex space, the BD vacuum solution dictates a strict $\pi/2$ phase difference between the field and its conjugate momentum. Assigning this phase accurately is crucial to eliminate unphysical fluctuations during the subsequent temporal evolution.

4. **Amplitude Assignment**: Finally, the physical BD amplitudes are assigned accordingly.

In the specific case of the axion-gauge coupling, combining these transverse initial conditions for the Abelian field with a strictly homogeneous initial scalar (axion) field guarantees that the Gauss constraint is satisfied to machine precision at the beginning of the simulation.

### **Non-Abelian gauge fields** { #subsec_NonAbelianGaugeIC }

The implemented non-Abelian initialization is tied to the SU(2) doublet sector. The model initializer calls the SU(2) initializer when SU(2) doublets are present. First the doublet matter fluctuations are generated, as described in Section [*Doublet scalars*](#subsec_DoubletIC). Then the SU(2) gauge links and gauge momenta are initialized from the non-Abelian Gauss constraint.

The gauge links are set to the identity, equivalently the gauge field starts from zero. As in the Abelian case, the initial magnetic energy vanishes and only the electric components are initialized. In the continuum this corresponds to solving
[](){ #eq_ICSU2ContinuumGauss }
```math
\begin{align}
\label{eq_ICSU2ContinuumGauss}
\partial_i B_i^{b\,\prime}(\mathbf{x})
=
J^b_0(\mathbf{x}),
\hspace{0.6cm}
B_i^{b\,\prime}(\mathbf{k})
=
i{k_i\over k^2}J^b_0(\mathbf{k}),
\hspace{0.6cm}
\mathbf{k}\neq \mathbf{0}.
\end{align}
```
On the lattice, the electric fields are solved in Fourier space from the SU(2) charge density:
[](){ #eq_ICSU2GaussSolve }
```math
\begin{align}
\label{eq_ICSU2GaussSolve}
\tilde\Pi^{(n),b}_{B_i}(\tilde{\bf n})
=
{(k^-_{L,i})^*\over |{\bf k}^-_L|^2}
\tilde j^{(n),b}_0(\tilde{\bf n}),
\hspace{0.6cm}
\tilde j^{(n),b}_0
=
-\delta\tilde x\,
\widetilde{J^{(n),b}_0},
\hspace{0.6cm}
U_i^{(n)}=\mathbb{1}.
\end{align}
```
Here $n$ labels the SU(2) gauge field, $b=1,2,3$ labels the algebra component, and $k^-_{L,i}=1-\exp(-2\pi i n_i/N)$ is the backward lattice momentum. The zero mode of the gauge momentum is set to zero.

The corresponding code path is `initializeSU2` in `include/CosmoInterface/initializers/su2initializer.h`:

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2_gauss_solve)

There is currently no independent `ICtype_SU2` input parameter and no independent non-Abelian vacuum plane-wave initializer in this initialization path.

<!-- ### **Fluids (TBA)** { #subsec_FluidIC }

Fluid initial conditions are not implemented in the current matter-sector initializer. The main model initializer initializes scale factor variables, scalar singlets, gravitational waves, SU(2) doublets and gauge fields, and the Abelian sector; it does not dispatch to a fluid initializer.
-->
