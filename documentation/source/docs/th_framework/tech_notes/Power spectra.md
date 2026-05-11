# CosmoLattice Technical Note I: Power Spectra

**Written on May 6, 2022** 
**(Corrected on June 8, 2022)**

## Authors

- Daniel G. Figueroa* - Instituto de Física Corpuscular (IFIC), Consejo Superior de Investigaciones Científicas (CSIC) and Universitat de Valencia (UV), Valencia, Spain.
- Adrien Florio† - Center for Nuclear Theory, Department of Physics and Astronomy, Stony Brook University, New York 11794, USA.

**Contact:**
- \* daniel.figueroa@ific.uv.es
- † adrien.florio@stonybrook.edu

## Abstract

This is a technical note about the notion of power spectrum in a lattice. All features described in the note are implemented in CosmoLattice v1.1, which is publicly available in [http://www.cosmolattice.net](http://www.cosmolattice.net). We recommend the reader to check out as well other technical notes available there.

## Contents

  - [Basic Definitions](#basic-definitions)
    - [Power Spectrum in a Lattice](#power-spectrum-in-a-lattice)
  - [Power Spectrum - Type I](#power-spectrum---type-i)
    - [Type I - Version 1](#type-i---version-1)
    - [Type I - Version 2](#type-i---version-2)
    - [Type I - Version 3](#type-i---version-3)
  - [Power Spectrum - Type II](#power-spectrum---type-ii)
    - [Type II - Version 1](#type-ii---version-1)
    - [Type II - Version 2](#type-ii---version-2)
    - [Type II - Version 3](#type-ii---version-3)
  - [Comparison: Type I vs Type II Spectra](#comparison-type-i-vs-type-ii-spectra)
    - [Type I Advantages](#type-i-advantages)
    - [Why Type II is Still Implemented](#why-type-ii-is-still-implemented)
    - [Example Comparison](#example-comparison)
  - [Implementation in CosmoLattice](#implementation-in-cosmolattice)
    - [Output Files](#output-files)
    - [Momentum Scale Assignment](#momentum-scale-assignment)
  - [Additions/Corrections on June 7, 2022](#additionscorrections-on-june-7-2022)
  - [Acknowledgements](#acknowledgements)
  - [References](#references)

---

## Basic Definitions

In this note we aim to define the concept of power spectrum of a field in a lattice. In three spatial dimensions a lattice has $N^3$ sites in total, labeled as:

```math
n = (n_1, n_2, n_3), \quad \text{with} \quad n_i = 0, 1, \ldots, N-1, \quad i = 1, 2, 3
```

For convenience we define $\hat{1} \equiv (1, 0, 0)$, $\hat{2} \equiv (0, 1, 0)$ and $\hat{3} \equiv (0, 0, 1)$ as unit vectors in the lattice. They correspond to a positive displacement in each of the independent directions $x, y$, or $z$, of length equal to the lattice spacing:

```math
\delta x \equiv \frac{L}{N}
```

where $L$ is the length of the lattice[^1]. A continuum function $f(x)$ in space is represented by a lattice function $f(n)$, which has the same value as $f(x)$ at $x = n \delta x$. We consider periodic boundary conditions in the three spatial directions, so that $f(n + \hat{\imath}N) = f(n)$, $i = 1, 2$ or $3$.

The periodic boundary conditions in coordinate space imply that momenta must be discretized, whereas the discretization of the spatial coordinates implies that any definition of a discrete Fourier transform must be periodic. For each lattice $\{n\}$ we can always consider a reciprocal lattice representing Fourier modes, with sites labeled as:

```math
\tilde{n} = (\tilde{n}_1, \tilde{n}_2, \tilde{n}_3), \quad \text{with} \quad \tilde{n}_i = -\frac{N}{2} + 1, -\frac{N}{2} + 2, \ldots, -1, 0, 1, \ldots, \frac{N}{2} - 1, \frac{N}{2}, \quad i = 1, 2, 3
```

We define the discrete Fourier transform (DFT) as:

```math
f(n) \equiv \frac{1}{N^3} \sum_{\tilde{n}} e^{i \frac{2\pi}{N} \tilde{n} \cdot n} \tilde{f}(\tilde{n}) \quad \Leftrightarrow \quad \tilde{f}(\tilde{n}) \equiv \sum_{n} e^{-i \frac{2\pi}{N} \tilde{n} \cdot n} f(n)
```

and we note the following identity $\sum_n e^{i \frac{2\pi}{N} n\tilde{n}} = N^3 \delta_{0,\tilde{n}}$. As expected, it follows that Fourier-transformed functions are periodic in the reciprocal lattice, with periodic boundary conditions as $\tilde{f}(\tilde{n} + \hat{\imath}N) = \tilde{f}(\tilde{n})$. Here $\hat{\imath}$ are analogous unit vectors defined in the reciprocal lattice, corresponding to a momentum displacement equal to the minimum momentum captured in a lattice:

```math
k_{IR} = \frac{2\pi}{L} = \frac{2\pi}{N \delta x}
```

The momentum $k_{IR}$ represents by construction the infrared (IR) cut-off scale of the lattice. Each location $\tilde{n}$ in the reciprocal lattice hence labels the continuum momenta $k = (\tilde{n}_1, \tilde{n}_2, \tilde{n}_3) k_{IR}$. The maximum ultraviolet (UV) momentum we can capture in each spatial dimension is $k_{i,UV} = \frac{N}{2} k_{IR} = \frac{\pi}{\delta x}$. The maximum momentum we can capture in a 3-dimensional reciprocal lattice is therefore:

```math
k_{max} = \sqrt{k_{1,UV}^2 + k_{2,UV}^2 + k_{3,UV}^2} = \sqrt{3} \frac{N}{2} k_{IR} = \sqrt{3} \frac{\pi}{\delta x}
```

Modes with modulus $|\tilde{n}| \leq N/2$ belong to spherical shells of radius equal or smaller than the 'half-length'[^2] of the reciprocal lattice, so their angular distribution is as isotropic as the momentum discretisation of the reciprocal lattice allows for (we will refer to this concept as **lattice-isotropic**). Modes with larger radii belong to spherical shells that intersect the reciprocal cube only at finite solid angles around the diagonal directions of the reciprocal lattice, and hence are not lattice-isotropically distributed.

The solid angles become smaller the larger the modulus $|\tilde{n}|$, till we reach a final shell with maximum radius $|\tilde{n}| = \sqrt{3}N/2$, which contains only the lattice site $\tilde{n} = (N/2, N/2, N/2)$. In practice, only modes with $\tilde{n}_3 \geq 0$ are stored in the reciprocal lattice, since $\tilde{f}(-\tilde{n}) = \tilde{f}^*(\tilde{n})$ holds as long as $f(n)$ is real $\forall n$. As in Fourier space we need both real and imaginary parts of $\tilde{f}(\tilde{n})$, $2 \cdot N^2$ numbers are stored in the $(\tilde{n}_x, \tilde{n}_y)$ plane at $\tilde{n}_z = 0$, whereas $2 \cdot N^2 \cdot (N/2 - 1) = N^3 - 2 \cdot N^2$ numbers are stored in the $1 \leq \tilde{n}_z \leq N/2 - 1$ positive-$k_z$ semi-volume, and finally another $2 \cdot N^2$ numbers are stored in the $(\tilde{n}_x, \tilde{n}_y)$ plane at $\tilde{n}_z = N/2$. In total, a real function defined by $N^3$ numbers $\{f(n)\}$, needs $N^3 + 2 \cdot N^2$ numbers to define its Fourier transform amplitudes $\{\tilde{f}(\tilde{n})\}$. The memory allocated to store the original $N^3$ entries in real space, needs therefore to be expanded by an additional memory to allocate another $2 \cdot N^2$ extra numbers in the reciprocal lattice. These are the $2 \cdot N^2$ entries defining the Fourier components of a field at its Nyquist frequency $n_z = N/2$.

[^1]: In an expanding background, positions $\{x\}$ and their corresponding lattice sites $\{n\}$, represent comoving spatial coordinates. Hence $L$ represents, in such a case, the comoving length of the lattice.
[^2]: Here length refers to momentum units.
[^3]: Alternatively one could also choose $R(\tilde{n}) \equiv [|\tilde{n}|, |\tilde{n}| + \Delta\tilde{n}]$, with $\Delta\tilde{n} = 1$.
[^4]: The finest binning consists on considering all momentum moduli in the reciprocal lattice, and choose intervals in between every two consecutive momentum moduli.
[^5]: This is analogous to the cosmic variance sampling problem for the lowest multi-poles in the CMB temperature angular power spectrum.

### Power Spectrum in a Lattice

In the continuum, we define the power spectrum of a real function $f(x)$, as a function in Fourier-space characterizing its ensemble average as:

```math
\langle f^2 \rangle = \int d \log k \, \Delta_f(k), \quad \Delta_f(k) \equiv \frac{k^3}{2\pi^2} P_f(k), \quad \langle f_k f_{k'} \rangle = (2\pi)^3 P_f(k) \delta(k - k')
```

In a lattice, the ensemble average is substituted by a volume average,

```math
\langle f^2 \rangle_V = \frac{\delta x^3}{V} \sum_n f^2(n) = \frac{1}{N^3} \sum_n f^2(n)
```

so that using the discrete Fourier transform (4), we obtain

```math
\langle f^2 \rangle_V = \frac{1}{N^6} \sum_{\tilde{n}} |\tilde{f}(\tilde{n})|^2 = \frac{1}{N^6} \sum_l \sum_{\tilde{n}' \in R_l} |\tilde{f}(\tilde{n}')|^2
```

In the last expression we have decomposed the sum in two parts: angular direction $\sum_{\tilde{n}' \in R_l} (\ldots)$ summing over all modes with moduli within spherical bin shells of radius $|\tilde{n}'| \in R_l \equiv [l - \Delta\tilde{n}_l^-, l + \Delta\tilde{n}_l^+]$, with $l = 1, 2, 3, \ldots$ counting the bins and $\Delta\tilde{n}_l^\pm$ a given radial width for the bin $l$; and radii summation $\sum_l$ summing over the actual bins defined by the given radial binning.

This separation allow us to define the notion of isotropic modulus of a mode function as $|f_l|^2 \equiv \sum_{|\tilde{n}| \in R_l} |\tilde{f}(\tilde{n})|^2$, so that $\langle f^2 \rangle_V = \frac{1}{N^6} \sum_l |f_l|^2$. In general, an arbitrary binning $R_l$ does not need to have regular bins of equal width, and can be simply specified through its l-dependent intervals $[l - \Delta\tilde{n}_l^-, l + \Delta\tilde{n}_l^+]$.

For consistency it must be ensured that $\Delta\tilde{n}_l^+ + \Delta\tilde{n}_{l+1}^- = 1$, in order to guarantee the continuity of momentum. Of course, a natural choice for binning is to consider $\Delta\tilde{n}_l^+ = \Delta\tilde{n}_l^- \equiv \Delta\tilde{n}$, with $\Delta\tilde{n} = 1/2$, so that $|f_l|^2 = |f_{|\tilde{n}|}|^2 \equiv \sum_{\tilde{n}' \in R(\tilde{n})} |\tilde{f}(\tilde{n}')|^2$ represents a summation over the modes $\tilde{n}'$ within an spherical shell[^3] $R(\tilde{n}) \equiv [|\tilde{n}| - 1/2, |\tilde{n}| + 1/2]$, with $|\tilde{n}| = 1, 2, 3, \ldots$. We will refer to this latter option as the **canonical binning**. By default, CosmoLattice uses the canonical binning, although one can also choose finer or thicker binning options[^4].

The **multiplicity** $\#$ of a given bin is the number of modes that fit inside the spherical shell defined by such bin. We can therefore write the isotropised module introduced before as an angular average, $|f_l|^2 = \#_l \times \langle |\tilde{f}(\tilde{n})|^2 \rangle_{R_l}$, where by construction $\langle |\tilde{f}(\tilde{n})|^2 \rangle_{R_l} \equiv \frac{1}{\#_l} \sum_{\tilde{n} \in R_l} |\tilde{f}(\tilde{n})|^2$. For instance, in the canonical binning that CosmoLattice uses by default, we can count a number of modes within the first bins as 18, 62, 98, 210, 350, 450, 602, ...These are easily accounted for by considering that there are $4\pi|\tilde{n}|^2$ modes within a spherical surface of radius $|k| = k_{IR}|\tilde{n}|$. The most IR bins do not contain many modes[^5] and because of discretization effects, $4\pi|\tilde{n}|^2$ is not a good counting for the very first bins: under-estimating the multiplicity of the very first and most IR bin in ~30%, of the second most IR bin in ~20%, of the third most IR bin in ~15%, and then in general failing to estimate (over or under) by less than ~5% the multiplicity of bins up to the 25th (except for the 5th, 9th and 10th), and in less than ~1-2% the multiplicity of bins 50th and above. Above the (N/2)-th bin, the counting $4\pi|\tilde{n}|^2$ starts over-estimating the mutiplicity of such ultra-violet (UV) modes, more and more the larger the bin radius. This occurs because they belong to shells with radius larger than N/2, which as explained before, only intersect the (reciprocal) lattice cube in increasingly smaller solid angles as we enlarge the radius of the bin. The counting of bin multiplicity for a general binning choice can only be done numerically once the binning width has been specified.

---

## Power Spectrum - Type I

For a general binning, say characterised by $l_{max}$ bins labeled as $l = 1, 2, \ldots, l_{max}$, and with some radial bin width $\Delta\tilde{n}_l^\pm$, we can re-write Eq. (9) as

```math
\langle f^2 \rangle_V = \frac{1}{N^6} \sum_l \sum_{\tilde{n}' \in R(l)} |\tilde{f}(\tilde{n}')|^2 = \frac{1}{N^6} \sum_l \#_l \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R_l}
```

where we have defined an angular average as $\langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R_l} \equiv \frac{1}{\#_l} \sum_{\tilde{n}' \in R(l)} |\tilde{f}(\tilde{n}')|^2$, with $R(l) \equiv \{\tilde{n}' / |\tilde{n}'| \in [k_{min}^{(l)}, k_{max}^{(l)})\}$ indicating the modes within bins (possibly of irregular width) with momentum between $k_{min}^{(l)} \equiv (l - \Delta\tilde{n}_l^-) k_{IR}$ and $k_{max}^{(l)} \equiv (l + \Delta\tilde{n}_l^+) k_{IR}$, and with $\#_l$ the exact multiplicity of the bin, instead of its approximated counting $4\pi l^2$. Of course, if we chose the canonical binning such that $k_{min}^{(l)} \equiv k_{IR}(l - 1/2)$ and $k_{max}^{(l)} \equiv k_{IR}(l + 1/2)$, the bin multiplicity will start reproducing naturally (specially after the first most IR bins) that $\#_l \simeq 4\pi l^2$, at least up to $l = N/2$. Above that, for $l > N/2$, the multiplicity $\#_l$ starts dropping gradually below $4\pi l^2$, more and more the larger the $l$. In Type I spectra, we will not make any theoretical assumption about the multiplicity of the bins, and rather consider numerically the actual exact multiplicity given by each binning scheme.

Let us first consider the canonical binning, and choose the central momentum within each bin, $k(l) \equiv k_{IR} l$, as the scale to be paired with the power spectrum amplitude $\Delta_f(l)$ of that bin, where $l = 1, 2, \ldots, [\sqrt{3}N/2]$, where $[(\ldots)]$ means interger part of its argument. This allows us to define $\Delta \log k(\tilde{n}) \equiv \frac{k_{IR}}{k(\tilde{n})} = \frac{1}{|\tilde{n}|}$ as the element imitating the differential weight $d \log k$ in the continuum. We can then think of building

```math
\langle f^2 \rangle_V = \frac{1}{N^6} \sum_l \sum_{\tilde{n}' \in R(l)} |\tilde{f}(\tilde{n}')|^2 = \sum_{|\tilde{n}|} \Delta \log k_{IR} \times \frac{|\tilde{n}|}{N^6} \#_{|\tilde{n}|} \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(\tilde{n})}
```

```math
= \sum_{|\tilde{n}|} \Delta \log k_{IR} \times \frac{|k(\tilde{n})| \delta x}{2\pi N^5} \#_{|\tilde{n}|} \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(\tilde{n})}
```

from where we identify the Type I power spectrum as

```math
\Delta^{(I)}_f(|\tilde{n}|) \equiv \frac{|\tilde{n}|}{N^6} \sum_{\tilde{n}' \in R(\tilde{n})} |\tilde{f}(\tilde{n}')|^2 = \frac{|k(\tilde{n})| \delta x}{2\pi N^5} \#_{|\tilde{n}|} \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(\tilde{n})}
```

Eq. (13) suggests in fact to define different versions of power spectrum type I, based on different choices of the momentum modulus $|k|$ in the numerator of the rhs of such equation. We present such versions below.

### Type I - Version 1

Let us consider a general binning with $l_{max}$ bins, labeled as $l = 1, 2, 3, \ldots, l_{max}$. We can then write:

```math
\Delta^{(I)}_f(l) = \frac{k(l) \delta x}{2\pi N^5} \#_l \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(l)}, \quad l = 1, 2, 3, \ldots, l_{max}
```

which represent the **Type I.v1** lattice power spectrum. For a canonical binning, we would use $R(l) \equiv [l - 1/2, l + 1/2]$, $k(l) = k_{IR} l$, and $l_{max} = [\sqrt{3}N/2]$. For a general binning, instead, we would use shells $R(l) \equiv \{\tilde{n}' / k_{IR} |\tilde{n}'| \in [k_{min}^{(l)}, k_{max}^{(l)})\}$ of (possibly of irregular) width between momenta $k_{min}^{(l)}$ and $k_{max}^{(l)}$, $k(l) \equiv (k_{min}^{(l)} + k_{max}^{(l)})/2$, and $l_{max}$ some integer (positive) number of bins.

To reproduce the field expectation value, we then do:

```math
f^2 \equiv \sum_{l=1}^{[\sqrt{3}N/2]} \frac{1}{l} \Delta^{(I)}_f(l), \quad \text{[Canonical binning]}
```

```math
f^2 \simeq 2 \sum_{l=1}^{l_{max}} \frac{k_{max}^{(l)} - k_{min}^{(l)}}{k_{max}^{(l)} + k_{min}^{(l)}} \Delta^{(I)}_f(l), \quad \text{[General Binning]}
```

We note that $f^2$ obtained in the canonical binning with Eq. (15) is exactly identical to the volume expectation value $\langle f^2 \rangle_V$. Because of this, Type I.v1 with canonical binning has become our **default choice** in CosmoLattice.

### Type I - Version 2

Consider again an arbitrary binning with $l_{max}$ bins, labeled as $l = 1, 2, 3, \ldots, l_{max}$. The expression:

```math
\Delta^{(I)}_f(l) = \frac{\langle k(\tilde{n}')\rangle_l \delta x}{2\pi N^5} \#_l \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(l)}, \quad l = 1, 2, 3, \ldots, l_{max}
```

defines the **Type I.v2** lattice power spectrum, where the mean value of the modulus momentum within each bin is defined by:

```math
\langle k(\tilde{n})\rangle_l \equiv \frac{k_{IR}}{\#_l} \sum_{\tilde{n}' \in R(l)} |\tilde{n}'|
```

For a canonical binning, we would use $R(l) \equiv [l - 1/2, l + 1/2]$ and $l_{max} = [\sqrt{3}N/2]$; for a general binning, we would use shells $R(l) \equiv \{\tilde{n}' / k_{IR} |\tilde{n}'| \in [k_{min}^{(l)}, k_{max}^{(l)})\}$ of width between momenta $k_{min}^{(l)}$ and $k_{max}^{(l)}$, and $l_{max}$ some integer (positive) number of bins.

To reproduce the field expectation value, we then do:

```math
f^2 \equiv \sum_{l=1}^{[\sqrt{3}N/2]} \frac{k_{IR}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(I)}_f(l), \quad \text{[Canonical binning]}
```

```math
f^2 \simeq \sum_{l=1}^{l_{max}} \frac{k_{max}^{(l)} - k_{min}^{(l)}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(I)}_f(l), \quad \text{[General Binning]}
```

We note that $f^2$ obtained in the canonical binning with Eq. (18) is exactly identical to the volume expectation value $\langle f^2 \rangle_V$ (as it was Type I.v1 with canonical binning). Because of this, Type I.v2 with canonical binning has become our default choice in CosmoLattice for computing **'weighted' spectra**, that is spectra based in pairs calling the mean value of the modulus momentum $k(\tilde{n})$ within each bin, $\langle k(\tilde{n})\rangle_l \equiv \frac{k_{IR}}{\#_l} \sum_{\tilde{n}' \in R(l)} |\tilde{n}'|$.

### Type I - Version 3

Finally, considering once again an arbitrary binning with $l_{max}$ bins, labeled as $l = 1, 2, 3, \ldots, l_{max}$, the expression:

```math
\Delta^{(I)}_f(l) = \frac{\delta x \#_l}{2\pi N^5} \langle k(\tilde{n}')|\tilde{f}(\tilde{n}')|^2 \rangle_{R(l)}, \quad l = 1, 2, 3, \ldots, l_{max}
```

defines the **Type I.v3** lattice power spectrum. For a canonical binning, $R(l) \equiv [l - 1/2, l + 1/2]$ and $l_{max} = [\sqrt{3}N/2]$, as usual; whereas for a general binning, $R(l) \equiv \{\tilde{n}' / k_{IR} |\tilde{n}'| \in [k_{min}^{(l)}, k_{max}^{(l)})\}$ define shells of width from momenta $k_{min}^{(l)}$ to $k_{max}^{(l)}$, and $l_{max}$ some integer (positive) number of bins.

To reproduce the field expectation value, we do:

```math
f^2 \equiv \sum_{l=1}^{[\sqrt{3}N/2]} \frac{k_{IR}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(I)}_f(l), \quad \text{[Canonical binning]}
```

```math
f^2 \simeq \sum_{l=1}^{l_{max}} \frac{k_{max}^{(l)} - k_{min}^{(l)}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(I)}_f(l), \quad \text{[General Binning]}
```

Note that neither version of $f^2$ will reproduce exactly the volume expectation value $\langle f^2 \rangle_V$.

---

## Power Spectrum - Type II

To define this type of spectrum, let us begin by sticking initially to the canonical binning. As $\Delta \log k_{IR} = 1/|\tilde{n}|$ in this case, we can write:

```math
\langle f^2 \rangle_V = \frac{1}{N^6} \sum_{|\tilde{n}|} \sum_{\tilde{n}' \in R(\tilde{n})} |\tilde{f}(\tilde{n}')|^2 \simeq \frac{4\pi}{N^6} \sum_{|\tilde{n}|} |\tilde{n}|^2 \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(\tilde{n})}
```

where in the second equality we have used that the multiplicity in the canonical binning is approximately $4\pi |\tilde{n}|^2$ (at least for $|\tilde{n}| \leq N/2$), hence introducing:

```math
\langle (\cdots) \rangle_{R(\tilde{n})} \equiv \frac{1}{4\pi |\tilde{n}|^2} \sum_{\tilde{n}' \in R(\tilde{n})} (\cdots)
```

as an angular average over the spherical shell of radius $R(\tilde{n}) \equiv [|\tilde{n}| - \Delta\tilde{n}/2, |\tilde{n}| + \Delta\tilde{n}/2]$, with $\Delta\tilde{n} = 1$.

This leads to:

```math
\langle f^2 \rangle_V \simeq \frac{4\pi}{k_{IR}^3 N^6} \sum_{|\tilde{n}|} \Delta \log k(\tilde{n}) \, k^3(\tilde{n}) \langle |\tilde{f}(\tilde{n})|^2 \rangle_{R(\tilde{n})} = \sum_{|\tilde{n}|} \Delta \log k(\tilde{n}) \frac{k^3(\tilde{n}) \delta x^3}{2\pi^2 N^3} \langle |\tilde{f}(\tilde{n})|^2 \rangle_{R(\tilde{n})}
```

where $k(\tilde{n}) \equiv k_{IR} \tilde{n}$ and $k_{IR} \equiv 2\pi / (N \delta x)$. The expression in Eq. (24) immediately invites to define the lattice power spectrum as:

```math
\Delta^{(II)}_f(|\tilde{n}|) \equiv \frac{k^3(\tilde{n}) \delta x^3}{2\pi^2 N^3} \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(\tilde{n})}, \quad k(\tilde{n}) = k_{IR} |\tilde{n}|, \quad |\tilde{n}| = 1, 2, 3, \ldots, N/2, \ldots, [\sqrt{3}N/2]
```

With this choice, we reproduce exactly the definition given in Eq. (7) in the continuum limit:

```math
\langle f^2 \rangle_V = \sum_{|\tilde{n}|} \Delta \log k(\tilde{n}) \Delta^{(II)}_f(\tilde{n}) \quad \rightarrow \quad \int d \log k \, \Delta_f(k)
```

As in type I, multiple definitions for the amplitude of the power spectrum of the type II can be made, based on the choice of the momentum modulus in the rhs of Eq. (25). Furthermore, multiple choices for the momenta to which such amplitudes are ascribed to, are also possible.

### Type II - Version 1

For the canonical binning, an obvious choice is to take the central momentum of each bin, $k(l) \equiv k_{IR} l$ with $l = 1, 2, 3, \ldots$, as the momentum scale to which we associate the power spectrum amplitude $\Delta_f(l)$. That is:

```math
\Delta^{(II)}_f(l) = \frac{k^3(l) \delta x^3}{2\pi^2 N^3} \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(l)}, \quad l = 1, 2, 3, \ldots, [\sqrt{3}N/2]
```

with $R(l) \equiv [l - 1/2, l + 1/2]$, represent the **Type II.v1** lattice power spectrum. To reproduce the field expectation value, we write:

```math
f^2 \simeq \sum_{l=1}^{[\sqrt{3}N/2]} \frac{1}{l} \Delta^{(II)}_f(l)
```

We can also envisage the use of a Type II.v1 spectrum with a general binning, say with a total of $l_{max}$ bins, defining:

```math
\Delta^{(II)}_f(l) = \frac{k^3(l) \delta x^3}{2\pi^2 N^3} \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(l)}, \quad l = 1, 2, 3, \ldots, l_{max}
```

numbering the bins as $l = 1, 2, 3, \ldots, l_{max}$, with $R(l) \equiv \{\tilde{n}' / |\tilde{n}'| \in [k_{min}^{(l)}, k_{max}^{(l)})\}$ indicating bin intervals with (possibly irregular) widths between a minimum $k_{min}^{(l)}$ and a maximum momentum $k_{max}^{(l)}$, and $k(l) = (k_{max}^{(l)} + k_{min}^{(l)})/2$ the central momentum within the l-th bin.

To reproduce the field expectation value, we then write in this case

```math
f^2 \simeq 2 \sum_{l=1}^{l_{max}} \frac{k_{max}^{(l)} - k_{min}^{(l)}}{k_{max}^{(l)} + k_{min}^{(l)}} \Delta^{(II)}_f(l)
```

### Type II - Version 2

We introduce here the expression:

```math
\Delta^{(II)}_f(l) = \frac{\langle k(\tilde{n}')\rangle_l^3 \delta x^3}{2\pi^2 N^3} \langle |\tilde{f}(\tilde{n}')|^2 \rangle_{R(l)}, \quad l = 1, 2, 3, \ldots, l_{max}
```

which defines the **Type II.v2** lattice power spectrum. If we consider a canonical binning, then $R(l) \equiv [l - 1/2, l + 1/2]$ and $l_{max} = [\sqrt{3}N/2]$. However, since all momenta involved are now bin-averaged, we can easily consider an arbitrary binning, such that $R(l)$ is a function of $l$ representing (possibly irregular) bin widths, and bins are labelled as $l = 1, 2, 3, \ldots, l_{max}$, with $l_{max}$ indicating the total number of bins.

To reproduce the field expectation value in this version, we do:

```math
f^2 \simeq \sum_{l=1}^{[\sqrt{3}N/2]} \frac{k_{IR}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(II)}_f(l), \quad \text{[Canonical Binning]}
```

```math
f^2 \simeq \sum_{l=1}^{l_{max}} \frac{k_{max}^{(l)} - k_{min}^{(l)}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(II)}_f(l), \quad \text{[General Binning]}
```

### Type II - Version 3

Another possible way to compute the power spectrum is to bring the momentum factor inside the average:

```math
\Delta^{(II)}_f(l) = \frac{1}{2\pi^2} \frac{\delta x^3}{N^3} \langle k^3(\tilde{n}') |\tilde{f}(\tilde{n}')|^2 \rangle_{R(l)}, \quad l = 1, 2, 3, \ldots, l_{max}
```

defining again a new **Type II.v3** lattice power spectrum. If we consider a canonical binning, then $R(l) \equiv [l - 1/2, l + 1/2]$ and $l_{max} = [\sqrt{3}N/2]$. If we consider an arbitrary binning, then $R(l)$ is a function of $l$ representing the bin widths, and $l = 1, 2, 3, \ldots, l_{max}$, and $l_{max}$ counts the total number of bins.

To reproduce the field expectation value in version 3, we do, similarly as with version 2:

```math
f^2 \simeq \sum_{l=1}^{[\sqrt{3}N/2]} \frac{k_{IR}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(II)}_f(l), \quad \text{[Canonical Binning]}
```

```math
f^2 \simeq \sum_{l=1}^{l_{max}} \frac{k_{max}^{(l)} - k_{min}^{(l)}}{\langle k(\tilde{n}')\rangle_l} \Delta^{(II)}_f(l), \quad \text{[General Binning]}
```

---

## Comparison: Type I vs Type II Spectra

A clear problem of a lattice power spectrum of the Type II is it completely fails, no matter the version, on reproducing well the amplitude of the continuum spectrum in the UV scales of the lattice: the general form of the spectrum is ascribing $4\pi|\tilde{n}|^2$ modes to the bins with radius $|\tilde{n}| > N/2$, when in reality there are less modes available in such high-frequency bins, the less the larger the radius $|\tilde{n}|$.

This is typically not much of a problem if the physics under study is such that the spectrum exhibits a peak at intermediate scales, and falls off exponentially at UV scales. Such feature is desirable in any well-behaved lattice simulation, and hence with the definition of power spectrum given by either Eq. (27), (31), or (34), even though we are distorting the UV tail of the spectrum, this should be in any case exponentially suppressed at the UV scales compared to its amplitudes around the central modes ($|\tilde{n}| \sim N/4$).

Another (less severe) problem with Type II spectra is that it also fails to capture well the power spectrum in the most IR bins. In this occasion the failure is only of a few %, depending on the bin. This is simply related to the discretisation of momenta, which makes the counting of modes as $4\pi|\tilde{n}|^2$ in the most IR bins, simply imprecise, as we noted in Sect. 1.1.

One consequence of the bad IR and UV reconstructions just mentioned, is that no version of the Type II spectra can reproduce well the actual lattice volume average $\langle f^2 \rangle_V$. All versions of Type II will actually suffer from the same problem, a wrong reconstruction of the UV sector $|\tilde{n}| > N/2$, and an imprecise reconstruction of the most IR bins.

Depending of the actual shape of the spectrum, the reconstructions (28), (30), (32), (33), (35) and (36), will reproduce better or worse the actual volume average $\langle f^2 \rangle_V$, but never exactly. If the spectrum is peaked at intermediate scales, say at $|\tilde{n}| \in (10, N/2)$, with a UV tail exponentially falling off and an IR tail well below the peak of the spectrum, the reconstruction of $\langle f^2 \rangle_V$ by either type II spectra will be pretty good, as any version of the spectrum will capture correctly the physics.

However if the spectrum is broad and its IR and UV tails are not well suppressed below the peak, noticeable differences in the reconstruction of $\langle f^2 \rangle_V$ may emerge, depending on the case.

### Type I Advantages

Contrary to Type II, **Type I spectrum does not assume any theoretical prediction for counting the modes** within each bin. It rather uses the **exact multiplicity** $\#_l$ of each bin, once a given binning scheme is chosen. As a consequence, type I, as opposed to type II, does **not over-estimate the power spectrum amplitude** on the lattice UV scales, and the counting of modes within the most IR bins is actually considered exactly.

A consequence following from a precise mode counting is that all versions of the Type I reconstructions of the field expectation value $\langle f^2 \rangle_V$, (15), (16), (18), (19), and (21), (22), all approximate quite well the actual lattice volume average $\langle f^2 \rangle_V$. In particular, the estimators (15) and (18), reproduce **exactly** the volume expectation value $\langle f^2 \rangle_V$ for a canonical binning.

### Why Type II is Still Implemented

Despite the clear advantages of Type I, we till decided to define the Type II spectrum, based on the direct discretization of the continuum expression. The reason to define this less accurate power spectrum is that its use is ubiquitous in the literature, so for comparison purposes we prefer retain this definition implemented in CosmoLattice. We choose however that **CosmoLattice computes by default a Type I spectrum**.

### Example Comparison

As an example, in Fig. 1 we plot the power spectra for the field content in a preheating scenario:

```math
V(\phi) = \frac{1}{2} \Lambda^4 \tanh^2\left(\frac{\phi}{M}\right) + \frac{1}{2} g^2 \phi^2 \chi^2
```

where $\phi$ is the inflaton and $\chi$ a daughter-preheat field coupled to the inflaton, $M = 10 m_p$, $\Lambda \simeq 1.77 \cdot 10^{16}$ GeV, and we considered an initial resonance parameter $q_* = 4 \cdot 10^4$. 

Different colors denote different moments, $mt = 40$ (red), $mt = 140$ (orange) and $mt = 380$ (blue), with $m \equiv \Lambda^2 / M$, whereas solid lines correspond to Type I spectra and dashed lines to Type II (both shown in version 1). As expected, the UV tail of the Type I spectra falls off exponentially faster than the tail of the Type II spectra, as the latter over-counts the multiplicity of the UV bins with momentum modulus larger than the (reciprocal) lattice length. As expected as well, some discrepancy is also appreciated in the most IR scales, again due to the inexact counting of modes in Type II.

---

## Implementation in CosmoLattice

All the above spectra implementation are available in CosmoLattice. The type can be chosen by specifying the argument `PS type= 1, 2`. Similarly, the version is specified through the argument `PS version= 1, 2, 3`. The binning, if not specified, is taken to be the canonical one defined before. We have not implemented an irregular binning in CosmoLattice, as we do not see the advantage of this. However, the user is allowed to specify a different bin-width common to all bins, simply through the argument `deltaKBin` (the canonical being $k_{IR}$).

Finally, in order to output a power spectrum we need to pair its amplitude for each bin to a modulus-momentum scale representative of the momenta radii within such bin. Various choices of such scale are available in CosmoLattice, determined by the value of `spectraVerbosity`, see text below.

### Output Files

What we output in the spectra files depends on the type of field for which we compute the spectra. The first column always corresponds to a momentum scale associated to the bin. The following columns represent various spectral quantities, like the power spectrum of the field amplitude, of its conjugate momentum, its occupation number, etc. This depends on whether we are obtaining the spectrum for a scalar field or for a gauge field. See the [CosmoLattice manual](http://www.cosmolattice.net) [1] for further information, and in particular Sects. 4.3.3 and 5.4 there.

The last column finally indicates the multiplicity of each bin, i.e. the number of sites within each bin, which we denote now as $\Delta n_{bin}$. Namely, the files print:

- **spectra_scalar_[nfld].txt**: $\tilde{k}, \tilde{\Delta}_{\tilde{\phi}}(\tilde{k}), \tilde{\Delta}_{\tilde{\phi}'}(\tilde{k}), \tilde{n}_{\tilde{k}}, \Delta n_{bin}$
- **spectra_norm_cmplx_scalar_[nfld].txt**: $\tilde{k}, \tilde{\Delta}_{\tilde{\varphi}}(\tilde{k}), \tilde{\Delta}_{\tilde{\varphi}'}(\tilde{k}), \tilde{n}_{\tilde{k}}, \Delta n_{bin}$
- **spectra_norm_SU2Doublet_scalar_[nfld].txt**: $\tilde{k}, \tilde{\Delta}_{\tilde{\Phi}}(\tilde{k}), \tilde{\Delta}_{\tilde{\Phi}'}(\tilde{k}), \tilde{n}_{\tilde{k}}, \Delta n_{bin}$
- **spectra_norm_[U1/SU2]_[nfld].txt**: $\tilde{k}, \tilde{\Delta}_{\tilde{E}}(\tilde{k}), \tilde{\Delta}_{\tilde{B}}(\tilde{k}), \Delta n_{bin}$

### Momentum Scale Assignment

What momentum $\tilde{k}$ we associate to each bin depends on the `spectraVerbosity= 0, 1, 2`. In particular:

- **For `spectraVerbosity= 0`**: The first column corresponds to the central value of the momentum bins:
  ```math
  k(l) = \frac{1}{2}(k_{max}^{(l)} + k_{min}^{(l)})
  ```
  Note that for our canonical binning, $k(l) = l k_{IR}$, $k_{min}^{(l)} = (l - 1/2) k_{IR}$, and $k_{max}^{(l)} = (l + 1/2) k_{IR}$.

- **For `spectraVerbosity= 1`**: The first column corresponds to the mean momentum of the bin:
  ```math
  \langle k(\tilde{n}')\rangle_l \equiv \frac{k_{IR}}{\#_l} \sum_{\tilde{n}' \in R(l)} |\tilde{n}'|
  ```

- **For `spectraVerbosity= 2`**: The user can access to exhaustive information about the binning. The first column corresponds to the central value of the momentum bins $k(l) = (k_{max}^{(l)} + k_{min}^{(l)}) / 2$, the second corresponds to the mean momentum of the bin:
  ```math
  \langle k(\tilde{n}')\rangle_l \equiv \frac{k_{IR}}{\#_l} \sum_{\tilde{n}' \in R(l)} |\tilde{n}'|
  ```
  The third column shows the variance of the momentum in a given bin $\langle k(\tilde{n}')^2 \rangle_l$. The fourth column displays the minimum value of the momentum in the given bin, and the fifth column its maximum. After this, the spectrum amplitudes follows, with similar information: mean value, variance in a given bin, minimum value and maximum value in a given bin. The last column shows as usual the bin multiplicity.

!!! note
    In order to keep track of all the columns, the user is highly recommended to turn on the `print header=true` option in the input file.

---

## Additions/Corrections on June 7, 2022

!!! warning
    ⋆ The variable `spectraVerbosity` was wrongly indicated to take values 1, 2, 3. The correct values **0, 1, 2** are now indicated. If no value is specified in the input parameter file, CosmoLattice runs by default considering `spectraVerbosity= 0`.

!!! warning
    ⋆ Independently of whether one uses `PS type= 1` or `PS type= 2` to output field spectra, the occupation number for scalar fields in CosmoLattice v1.1 is still obtained as in CosmoLattice v1.0, i.e. using expectation values computed based on the theoretical multiplicity used in `PS type= 2`. We hope to correct this in a near future.