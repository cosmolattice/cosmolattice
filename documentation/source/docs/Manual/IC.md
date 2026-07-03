<!-- <div style="text-align: justify;"> -->

Here we explain how to implement the initial conditions

### **Singlet scalars** { #subsec_SingletIC }

#### Default scalar singlet initialization { #subsubsec_DefaultSingletIC }

The default initialization of scalar singlet fluctuations follows the prescription described in Section [*Initialization of fluctuations*](My first model of (singlet) scalar fields.md#sec_InitScalar) of the scalar-singlet example. If no external spectrum is provided for scalar field `i`, meaning that `ext_PSi` is absent or set to `none`, CosmoLattice samples the field and its conjugate momentum from the default Gaussian vacuum prescription.

For a scalar singlet $f$, the default continuum spectrum is
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

In program variables, the initial Fourier modes are built from two independent Gaussian realizations,
[](){ #eq_ICDefaultScalarModes }
```math
\begin{align}
\label{eq_ICDefaultScalarModes}
\delta\tilde f(\tilde{\bf n})
&=
{1\over\sqrt{2}}
\left[
\delta\tilde f_L(\tilde{\bf n})
+\delta\tilde f_R(\tilde{\bf n})
\right],
\\
\delta\tilde f'(\tilde{\bf n})
&=
{i\tilde\omega_k\over\sqrt{2}}
\left[
\delta\tilde f_L(\tilde{\bf n})
-\delta\tilde f_R(\tilde{\bf n})
\right]
-\tilde{\mathcal H}\delta\tilde f(\tilde{\bf n}) .
\end{align}
```

The variance of the independent real Gaussian components is fixed by the normalization
[](){ #eq_ICDefaultScalarVariance }
```math
\begin{align}
\label{eq_ICDefaultScalarVariance}
\sigma_f(\tilde{\bf n})
=
H(\tilde k_{\rm cut}-\tilde k)
\left({\omega_*\over f_*}\right)
\left({\tilde L\over \delta\tilde x^2}\right)^{3/2}
{1\over\sqrt{2}}
{1\over \sqrt{2\tilde\omega_k}},
\hspace{0.6cm}
\tilde\omega_k=\sqrt{\tilde k^2+\tilde m_f^2}.
\end{align}
```

Eq.$~$\eqref{eq_ICDefaultScalarVariance} is the three-dimensional expression implemented by the code. In one- and two-dimensional runs the corresponding dimensional prefactors are modified as shown in the same function.

The scalar initializer selects this path through

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

### **Doublet scalars** { #subsec_DoubletIC }

### **Abelian gauge fields** { #subsec_AbelianGaugeIC }

### **Non-Abelian gauge fields** { #subsec_NonAbelianGaugeIC }

### **Fluids (TBA)** { #subsec_FluidIC }
