<!-- <div style="text-align: justify;"> -->

Here we explain how to implement the initial conditions

### Singlet Scalars { #subsec_SingletIC }

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
\Upsilon_{|\tilde{\bf n}|}
\equiv
{\#_{R(\tilde{\bf n})}\over 4\pi|\tilde{\bf n}|^2} .
\end{align}
```
The input flag `PS_type`, referred to as `PSType` internally, selects how this multiplicity factor is treated. `PSType = 1` selects Type-I spectra, which use the exact lattice multiplicity $\#_{R(\tilde{\bf n})}$ and keep $\Upsilon_{|\tilde{\bf n}|}$ explicitly. `PSType = 2` selects Type-II spectra, which use the continuum shell multiplicity $\#_{R(\tilde{\bf n})}=4\pi|\tilde{\bf n}|^2$, so $\Upsilon_{|\tilde{\bf n}|}=1$.

Identifying the shell average with the statistical average of the Gaussian realization gives the variances used to initialize each Fourier mode from the external spectrum. In program units,
[](){ #eq_ICExternalSpectrumVariance }
```math
\begin{align}
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
CosmoLattice then draws the real and imaginary parts of $\tilde f(\tilde{\bf n})$ and $\tilde f'(\tilde{\bf n})$ from Gaussian distributions with zero mean and variance equal to one half of the expressions above. Equivalently, one may view the same initialization as drawing random phases and Rayleigh-distributed amplitudes whose expected square amplitudes are fixed by the supplied spectrum.

The same power-spectrum normalization also applies to any supplied initial spectrum for $\delta f'$. In that case the spectrum fixes $\left\langle \delta f'^2 \right\rangle$, with the additional $a^{2\alpha}$ scaling associated with the $\alpha$-time derivative used by CosmoLattice. The convention used to produce the external file should match the value of `PS_type` in the run that reads it, and momenta in the external file must be expressed in the units expected by the initializer, namely the same units as $\omega_*$.

!!! note
    Type-I external-spectrum initialization is supported for three-dimensional lattices. For lower-dimensional runs, adapt $\Upsilon_{|\tilde{\bf n}|}$ according to the dimension $d$.

### Complex Scalars { #subsec_ComplexIC }

### Doublet Scalars { #subsec_DoubletIC }

### Abelian Gauge Fields { #subsec_AbelianGaugeIC }

### Non-Abelian Gauge Fields { #subsec_NonAbelianGaugeIC }

### Fluids (TBA) { #subsec_FluidIC }
