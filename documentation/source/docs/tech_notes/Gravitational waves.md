# CosmoLattice Technical Note II: Gravitational Waves

**Written on May 6, 2022** 
**(Corrected on June 20, 2023)**

## Authors

- Jorge Baeza-Ballesteros* - Instituto de Física Corpuscular (IFIC), Consejo Superior de Investigaciones Científicas (CSIC) and Universitat de Valencia (UV), Valencia, Spain
- Daniel G. Figueroa† - Instituto de Física Corpuscular (IFIC), Consejo Superior de Investigaciones Científicas (CSIC) and Universitat de Valencia (UV), Valencia, Spain
- Nicolás Loayza‡ - Instituto de Física Corpuscular (IFIC), Consejo Superior de Investigaciones Científicas (CSIC) and Universitat de Valencia (UV), Valencia, Spain
- Adrien Florio§ - Center for Nuclear Theory, Department of Physics and Astronomy, Stony Brook University, New York 11794, USA

**Contact:**
- \* jorge.baeza@ific.uv.es
- † daniel.figueroa@ific.uv.es
- ‡ nicolas.loayza@ific.uv.es
- § adrien.florio@stonybrook.edu

## Abstract

This is a technical note about the dynamics of gravitational waves (GWs) in a lattice. We present lattice analogues of tensor metric perturbations representing GWs, a proper lattice definition of the energy density power spectrum of a stochastic GW background, and a discretized version of the equations of motion of GWs sourced by scalar fields in an expanding background. All these features are implemented in the GW module released as part of CosmoLattice v1.1, which is publicly available at [http://www.cosmolattice.net](http://www.cosmolattice.net). We recommend the reader to check out other technical notes available there as well.

## Contents

- [CosmoLattice Technical Note II: Gravitational Waves](#cosmolattice-technical-note-ii-gravitational-waves)
  - [Authors](#authors)
  - [Abstract](#abstract)
  - [Contents](#contents)
  - [1 Gravitational Waves in the Continuum](#1-gravitational-waves-in-the-continuum)
  - [2 Gravitational Waves in the Lattice](#2-gravitational-waves-in-the-lattice)
  - [3 Gravitational Waves in CosmoLattice](#3-gravitational-waves-in-cosmolattice)
    - [3.1 Equation of Motion](#31-equation-of-motion)
    - [3.2 GW Power Spectrum: Type I](#32-gw-power-spectrum-type-i)
      - [3.2.1 Type I - Version 1](#321-type-i---version-1)
      - [3.2.2 Type I - Version 2](#322-type-i---version-2)
      - [3.2.3 Type I - Version 3](#323-type-i---version-3)
    - [3.3 GW Power Spectrum: Type II](#33-gw-power-spectrum-type-ii)
      - [3.3.1 Type II - Version 1](#331-type-ii---version-1)
      - [3.3.2 Type II - Version 2](#332-type-ii---version-2)
      - [3.3.3 Type II - Version 3](#333-type-ii---version-3)
  - [4 A Working Example: Inflationary Potential { #sec\_AWorkingExampleInflationaryPotential }](#4-a-working-example-inflationary-potential--sec_aworkingexampleinflationarypotential-)
    - [4.1 GW Energy Density Power Spectra Examples](#41-gw-energy-density-power-spectra-examples)
  - [5 Use of GW Module for Complex Scalar Fields](#5-use-of-gw-module-for-complex-scalar-fields)
  - [Appendix A: Where do Gravitational Waves Live in the Lattice?](#appendix-a-where-do-gravitational-waves-live-in-the-lattice)

---

## 1 Gravitational Waves in the Continuum

We first review the definition of gravitational waves (GWs) and their energy density power spectrum in a spatially-flat Friedmann-Lemaître-Robertson-Walker (FLRW) metric. GWs are identified with perturbations $h_{ij}$ of the background metric which are transverse and traceless, i.e.,

```math
ds^2 = -dt^2 + a^2(t)(\delta_{ij} + h_{ij})dx^i dx^j, \quad \text{with} \quad \partial_i h_{ij} = 0 \quad \text{and} \quad h_{ii} = 0\, , \tag{1}
```

where $t$ represents coordinate time and $x^i$ are spatial coordinates, with Latin indices running from 1 to 3. Throughout the note, summation is assumed over repeated indices, unless otherwise stated. In a FLRW background, the dynamics of GWs are described by equations of motion of the form [@Caprini_2018]

```math
\ddot{h}_{ij} + 3H\dot{h}_{ij} - \frac{\nabla^2}{a^2}h_{ij} = \frac{2}{m_p^2 a^2}\Pi^{TT}_{ij}\, , \tag{2}
```

where $\dot{h}_{ij} = dh_{ij}/dt$, $H = \dot{a}/a$ is the Hubble rate, $m_p = 1/\sqrt{8\pi G} = 2.44 \times 10^{18}$ GeV is the reduced Planck mass and $\Pi^{TT}_{ij}$ is the transverse-traceless (TT) part of the anisotropic tensor $\Pi_{ij}$, which we define below. The TT conditions $\partial_i \Pi^{TT}_{ij} = \Pi^{TT}_{ii} = 0$ hold $\forall \mathbf{x}, t$. Obtaining the TT part of a tensor in coordinate space amounts to a non-local operation. It is more convenient to perform this determination in Fourier space, where a projector filtering out only the TT degrees of freedom of a tensor can be easily constructed. The GW source can be written as

```math
\Pi^{TT}_{ij}(\mathbf{k}, t) = \Lambda_{ij,kl}(\hat{k})\Pi_{kl}(\mathbf{k}, t)\, , \tag{3}
```

where $\Lambda_{ij,kl}$ is a projection operator defined as

```math
\Lambda_{ij,lm}(\hat{k}) \equiv P_{il}(\hat{k})P_{jm}(\hat{k}) - \frac{1}{2}P_{ij}(\hat{k})P_{lm}(\hat{k}), \quad \text{with} \quad P_{ij}(\mathbf{k}) = \delta_{ij} - \hat{k}_i\hat{k}_j, \quad \hat{k}_i = k_i/k\, , \tag{4}
```

where $\mathbf{k} = (k_1, k_2, k_3)$ is the three-momentum and $k = |\mathbf{k}|$. Thanks to the fact that $P_{ij}\hat{k}_j = 0$ and $P_{ij}P_{jm} = P_{im}$, one can easily see that the transverse-traceless conditions in Fourier space, $k_i \Pi_{ij}(\mathbf{k}, t) = \Pi_{ii}(\mathbf{k}, t) = 0$, are satisfied at any time.

Coming back to the anisotropic stress tensor, $\Pi^{\mu\nu}$, it describes the deviation of an energy-momentum tensor $T^{\mu\nu}$ with respect to a perfect fluid. The spatial components read

```math
\Pi_{ij} \equiv T_{ij} - p\, g_{ij}\, , \tag{5}
```

with $p$ the homogeneous background pressure and $g_{ij} = a^2(t)(\delta_{ij} + h_{ij})$ the spatial-spatial part of the FLRW perturbed metric.

The energy density of a stochastic GW background (SGWB) is defined as [@Caprini_2018]

```math
\rho_{GW}(t) = \frac{m_p^2}{4}\langle\dot{h}_{ij}(\mathbf{x},t)\dot{h}_{ij}(\mathbf{x},t)\rangle_V \tag{6}
```

```math
\approx \frac{m_p^2}{4V}\int \frac{d^3k}{(2\pi)^3}\dot{h}_{ij}(\mathbf{k},t)\dot{h}^*_{ij}(\mathbf{k},t) \tag{7}
```

```math
\equiv \int \frac{d\rho_{GW}}{d\log k}\, d\log k\, , \tag{8}
```

where $\langle...\rangle_V$ denotes spatial average over a volume $V$ assumed to encompass all relevant wavelengths of the perturbations $h_{ij}$, and we have used the Fourier transformed convention explained in the CosmoLattice manual [@Figueroa_2021_cpc]. We note that the approximate expression in Eq. (7) is only valid in the limit $kV^{1/3} \gg 1$, where $\int_V e^{-i\mathbf{x}(\mathbf{k}-\mathbf{k}')} \to (2\pi)^3 \delta^{(3)}(\mathbf{k}-\mathbf{k}')$. The energy density per logarithmic interval is then defined as

```math
\frac{d\rho_{GW}}{d\log k} = \frac{m_p^2 k^3}{8\pi^2 V}\int \frac{d\Omega_k}{4\pi}\dot{h}_{ij}(\mathbf{k},t)\dot{h}^*_{ij}(\mathbf{k},t)\, , \tag{9}
```

where $d\Omega_k$ represents a solid angle measure in momentum space.

For stochastic sources the volume average can be replaced by an ensemble average $\langle...\rangle$ over realizations of the stochastic background,

```math
\rho_{GW}(t) = \frac{m_p^2}{4}\langle\dot{h}_{ij}(\mathbf{x},t)\dot{h}^*_{ij}(\mathbf{x},t)\rangle = \frac{m_p^2}{4}\int \frac{d^3k\, d^3k'}{(2\pi)^3(2\pi)^3} e^{-i\mathbf{x}(\mathbf{k}-\mathbf{k}')} \times \langle\dot{h}_{ij}(\mathbf{k},t)\dot{h}_{ij}(\mathbf{k}',t)\rangle \equiv \frac{m_p^2}{8\pi^2}\int \frac{dk}{k}\, k^3 P_{\dot{h}}(k,t)\, , \tag{10}
```

where we have defined the power spectrum of the tensor time derivative in the third line, assuming homogeneity and isotropy,

```math
\langle\dot{h}_{ij}(\mathbf{k},t)\dot{h}_{ij}(\mathbf{k}',t)\rangle = (2\pi)^3 P_{\dot{h}}(k,t)\delta^{(3)}(\mathbf{k} - \mathbf{k}')\, . \tag{11}
```

Comparing Eqs. (11) and (10) we can obtain the GW power spectrum,

```math
\frac{d\rho_{GW}}{d\log k} = \frac{m_p^2 k^3}{8\pi^2}P_{\dot{h}}(k,t)\, . \tag{12}
```

The GW energy density power spectrum is typically normalized by the critical energy density, $\rho_c \equiv 3H^2 m_p^2$, and expressed with the following notation

```math
\Omega_{GW} = \frac{1}{\rho_c}\frac{d\rho_{GW}}{d\log k}\, . \tag{13}
```

Studying the dynamics of GWs is a numerically expensive task, given that the TT projection is a non-local operation in position space. In Ref. [@GarciaBellido_2008] a workaround was proposed to overcome this problem: Noting that $\Pi^{TT}_{ij}(\mathbf{k},t)$ is just a linear combination of the components of the full tensor $\Pi_{ij}(\mathbf{k},t)$, and that the solution to Eq. (2) is linear in $\Pi_{ij}$, one can write the TT tensor perturbations (i.e. GWs) as

```math
h_{ij}(\mathbf{k},t) = \Lambda_{ij,kl}(\hat{k})u_{kl}(\mathbf{k},t)\, , \tag{14}
```

where $u_{ij}(\mathbf{k},t)$ is the Fourier transform of the solution to the following equation

```math
\ddot{u}_{ij} + 3H\dot{u}_{ij} - \frac{\nabla^2}{a^2}u_{ij} = \frac{2}{m_p^2 a^2}\Pi^{eff}_{ij}\, , \tag{15}
```

where $\Pi^{eff}_{ij}$ is an effective anisotropic tensor that contains the parts of $\Pi_{ij}$ with non-vanishing TT projection. For real scalar fields [@GarciaBellido_2008]

```math
\Pi^{eff}_{ij} = \partial_i\phi_a\partial_j\phi_a\, , \tag{16}
```

where $\phi_a$ are real scalar fields and $a = 1, 2, \ldots$.

Eq. (15) can be evolved in configuration space for as long as we want, and only when we desire to obtain the physical degrees of freedom (dof) $h_{ij}$, we Fourier transform its solution, $u_{ij}(\mathbf{x},t) \to u_{ij}(\mathbf{k},t)$, and apply the projector in Eq. (4) as in Eq. (14). The viability of the method relies on the following observation. To compute the GWs we could first project the TT part of the source $\Pi_{ij}$, and then solve Eq. (2) directly for the physical tensor fields $h_{ij}$. This would require however to do this operation at every time step, making the procedure numerically expensive, as obtaining $\Pi^{TT}_{ij}$ in real space is a non-local operation. Instead, we can achieve the same result if we commute the operations such that, first we solve Eq. (15) for the unphysical fields $u_{ij}$ for as long as we desire, and then we apply the TT projector to the solution only when we wish to obtain the physical dof $h_{ij}$, as in Eq. (14). We can do this because the TT projection and the solution as a function of the source are linear operations in the reciprocal space, and hence they commute. See Ref. [@GarciaBellido_2008] for further details.

---

## 2 Gravitational Waves in the Lattice

Before considering the discretized version of GWs, we review some basic definitions regarding the lattice. The 3-dimensional space contains $N^3$ sites in total, labelled by

```math
\mathbf{n} = (n_1, n_2, n_3), \quad \text{with} \quad n_i = 0, 1, \ldots, N-1, \quad i = 1, 2, 3\, . \tag{17}
```

This is defined such that any continuum function $f(\mathbf{x})$ is represented in the lattice by a lattice function $f(\mathbf{n})$, which has the same value as $f(\mathbf{x})$ at $\mathbf{x} = \mathbf{n}\,\delta x$. Here $\delta x = L/N$ is the lattice spacing, $L$ is the comoving size of the lattice, and both $\mathbf{x}$ and $\mathbf{n}$ refer to comoving spatial coordinates.

The reciprocal lattice representing Fourier modes is also a periodic and discretized 3-dimensional lattice. The Fourier modes live in the sites of the reciprocal lattice, which we label as

```math
\tilde{\mathbf{n}} = (\tilde{n}_1, \tilde{n}_2, \tilde{n}_3), \quad \text{with} \quad \tilde{n}_i = -\frac{N}{2} + 1, -\frac{N}{2} + 2, \ldots, -1, 0, 1, \ldots, \frac{N}{2} - 1, \frac{N}{2}, \quad i = 1, 2, 3\, . \tag{18}
```

We define the Discrete Fourier Transform (DFT),

```math
f(\mathbf{n}) = \frac{1}{N^3}\sum_{\tilde{\mathbf{n}}} e^{2\pi i \tilde{\mathbf{n}}\cdot\mathbf{n}/N} f(\tilde{\mathbf{n}}), \quad f(\tilde{\mathbf{n}}) = \sum_{\mathbf{n}} e^{-2\pi i \tilde{\mathbf{n}}\cdot\mathbf{n}/N} f(\mathbf{n})\, , \tag{19}
```

and distinguish between a function and its Fourier transform only by their arguments. Finally, note there is a minimum momentum in the reciprocal lattice, $k_{IR} = 2\pi/L$, which defines an infrared cutoff scale for the lattice.

In a discretized space-time, the GW fields evolve according to a discretized version of Eq. (2). The energy density power spectrum of GWs is then computed with the discrete equivalent of Eq. (10),

```math
\rho_{GW}(t) = \frac{m_p^2}{4N^3}\sum_{\mathbf{n}} \dot{h}_{ij}(\mathbf{n},t)\dot{h}_{ij}(\mathbf{n},t) = \frac{m_p^2}{4}\frac{1}{N^6}\sum_{\tilde{\mathbf{n}}} \dot{h}_{ij}(\tilde{\mathbf{n}},t)\dot{h}^*_{ij}(\tilde{\mathbf{n}},t) = \frac{m_p^2}{4}\frac{1}{N^6}\sum_l\sum_{\tilde{\mathbf{n}}\in R(l)} \dot{h}_{ij}(\tilde{\mathbf{n}},t)\dot{h}^*_{ij}(\tilde{\mathbf{n}},t)\, , \tag{20}
```

where in the second line we have applied the DFT on the two $h$-fields, and used $\sum_{\mathbf{n}} e^{i k_{IR}\delta x\, \mathbf{n}(\tilde{\mathbf{n}}-\tilde{\mathbf{n}}')} = N^3 \delta_{\tilde{\mathbf{n}}\tilde{\mathbf{n}}'}$. In the last line we have split the summation over spherical bins. In general, an arbitrary binning $R(l) \equiv [l, l + \Delta\tilde{n})$ with $l = 1, 2, \ldots$ labelling the bins, does not have bins of equal width, and can be simply specified through an $l$-dependent width $\Delta\tilde{n}(l)$. The multiplicity $\#_l$ of a given bin is the number of modes that fit inside the spherical shell defined by such bin. As explained in [Technical Note I](../tech_notes/Power%20spectra.md), the construction of the power spectrum depends on the different ways of counting the multiplicity of modes within each bin. For now we follow the approach from Ref. [@Figueroa_2011] and approximate the number of points in a given bin $R(|\tilde{\mathbf{n}}|)$ as $\#_{|\tilde{\mathbf{n}}|} \approx 4\pi|\tilde{\mathbf{n}}|^2$. This corresponds to a canonical binning with regular width $\Delta k = k_{IR}$ around the radius $k(|\tilde{\mathbf{n}}|) = k_{IR}|\tilde{\mathbf{n}}|$, i.e. $R(|\tilde{\mathbf{n}}|) \equiv [|\tilde{\mathbf{n}}| - 1/2, |\tilde{\mathbf{n}}| + 1/2)$. Using this we then obtain

```math
\rho_{GW}(t) = \frac{m_p^2}{4N^6}\sum_{|\tilde{\mathbf{n}}|} 4\pi|\tilde{\mathbf{n}}|^2 \langle\dot{h}_{ij}(\tilde{\mathbf{n}},t)\dot{h}^*_{ij}(\tilde{\mathbf{n}},t)\rangle_{R(|\tilde{\mathbf{n}}|)} = \sum_{|\tilde{\mathbf{n}}|} \left\{\frac{m_p^2 \delta x^6}{8\pi^2 L^3} k^3(|\tilde{\mathbf{n}}|) \langle\dot{h}_{ij}(\tilde{\mathbf{n}},t)\dot{h}^*_{ij}(\tilde{\mathbf{n}},t)\rangle_{R(|\tilde{\mathbf{n}}|)}\right\} \Delta\log k\, , \tag{21}
```

where $\langle...\rangle_{R(|\tilde{\mathbf{n}}|)}$ denotes average over the spherical shell and $\Delta\log k \equiv k_{IR}/k$. From here, we can define the GW energy density power spectrum in the lattice as

```math
\frac{d\rho_{GW}}{d\log k}(|\tilde{\mathbf{n}}|) = \frac{m_p^2 k(|\tilde{\mathbf{n}}|)^3}{8\pi^2 L^3}\left\langle\left[\delta x^3\, \dot{h}_{ij}(|\tilde{\mathbf{n}}|,t)\right]\left[\delta x^3\, \dot{h}_{ij}(|\tilde{\mathbf{n}}|,t)\right]^*\right\rangle_{R(|\tilde{\mathbf{n}}|)}\, . \tag{22}
```

As mentioned before other prescriptions for the binning can be made. We discuss the different possibilities later on in Sec. 3.2 and 3.3, and more in detail in [Technical Note I](../tech_notes/Power%20spectra.md).

In order to obtain the GW power spectrum we need the Fourier transform of $\dot{h}_{ij}(\mathbf{n},t)$ at each time we want to compute it. The procedure we follow is the one outlined at the end of Section 1: we evolve the field $u_{ij}(\mathbf{n},t)$ according to Eq. (15), and relate them to $h_{ij}(\mathbf{n},t)$ at any time through

```math
h_{ij}(\tilde{\mathbf{n}},t) = \Lambda^L_{ij,kl}(\tilde{\mathbf{n}})u_{kl}(\tilde{\mathbf{n}},t)\, , \tag{23}
```

where

```math
\Lambda^L_{ij,lm}(\tilde{\mathbf{n}}) \equiv P_{il}(\tilde{\mathbf{n}})P_{jm}(\tilde{\mathbf{n}}) - \frac{1}{2}P_{ij}(\tilde{\mathbf{n}})P_{lm}(\tilde{\mathbf{n}}), \quad \text{with} \quad P_{ij} = \delta_{ij} - \frac{k(\tilde{\mathbf{n}})_{L,i}\, k(\tilde{\mathbf{n}})_{L,j}}{k_L^2}\, , \tag{24}
```

being $k_L(\tilde{\mathbf{n}})$ a lattice momentum, which we define below. Its definition is not unique in a lattice, as it depends on the way spatial derivatives are discretized. The lattice TT projector then ensures transversality only with respect to the chosen discretized derivatives. For instance, three basic choices of lattice derivatives are the following: the neutral derivative centered in a lattice site

```math
[\nabla^0_i f](\mathbf{n}) = \frac{f(\mathbf{n} + \hat{i}) - f(\mathbf{n} - \hat{i})}{2\delta x}\, , \tag{25}
```

and the forward/backward derivatives

```math
[\nabla^\pm_i f](\mathbf{n}) = \frac{\pm f(\mathbf{n} \pm \hat{i}) \mp f(\mathbf{n})}{\delta x}\, . \tag{26}
```

Here $\hat{i}$ refers to a vector of length $\delta x$ in the $i$ spatial direction. The lattice momentum $k_L$ is then defined by computing the Fourier transform of these derivatives acting on an arbitrary function,

```math
[\nabla_i f](\tilde{\mathbf{n}}) = -ik_L(\tilde{\mathbf{n}})f(\tilde{\mathbf{n}})\, . \tag{27}
```

The components of the lattice momenta for the derivatives defined in Eqs. (25) and (26) are, respectively,

```math
k^0_{L,i} = \frac{\sin(2\pi\tilde{n}_i/N)}{\delta x}\, , \tag{28}
```

```math
k^\pm_{L,i} = 2e^{\mp i\pi\tilde{n}_i/N}\frac{\sin(\pi\tilde{n}_i/N)}{\delta x} = \frac{\sin(2\pi\tilde{n}_i/N)}{\delta x} \mp i\frac{1 - \cos(2\pi\tilde{n}_i/N)}{\delta x}\, . \tag{29}
```

As can be seen, the lattice momenta can be either real or complex, depending on the choice of lattice derivative. This extends to the TT projector. In the neutral case we define a real one,

```math
P^0_{ij} = \delta_{ij} - \frac{k^0_{L,i}\, k^0_{L,j}}{|k^0_L|^2}\, , \tag{30}
```

```math
\Lambda^0_{ij,kl} = P^0_{ik}P^0_{jl} - \frac{1}{2}P^0_{ij}P^0_{kl}\, , \tag{31}
```

while it is complex for $k^\pm_L$,

```math
P^\pm_{ij} = \delta_{ij} - \frac{(k^\pm_{L,i})^*\, k^\pm_{L,j}}{|k^\pm_L|^2}\, , \tag{32}
```

```math
\Lambda^\pm_{ij,kl} = P^\pm_{ik}P^{\pm *}_{jl} - \frac{1}{2}P^\pm_{ij}P^{\pm *}_{kl}\, . \tag{33}
```

The complex projectors obey the following properties

```math
\begin{aligned}
&1)\; \sum_i k^\pm_{L,i} P^\pm_{ij} = 0, &\quad &2)\; \sum_i (k^\pm_{L,i})^* P^\pm_{ij} \neq 0, \\
&3)\; \sum_j k^\pm_{L,j} P^\pm_{ij} \neq 0, &\quad &4)\; \sum_j (k^\pm_{L,i})^* P^\pm_{ij} = 0, \\
&5)\; P^{\pm *}_{ij} = P^\pm_{ji}, &\quad &6)\; P^\pm_{ij}(-\tilde{\mathbf{n}}) = P^\pm_{ji}(\tilde{\mathbf{n}}), \\
&7)\; P^\pm_{ij}P^\pm_{jk} = P^\pm_{ik}, &\quad &8)\; P^\pm_{ij}P^\pm_{ki} \neq P^\pm_{ik},
\end{aligned} \tag{34}
```

the most relevant of which are the idempotence of the projector (property 7) and its hermiticity (property 5). The real projector like $P^0_{ij}$ obeys a similar set of properties, except for the fact that it is symmetric instead of hermitian. A proof of these properties can be found in Ref. [@Figueroa_2011].

In light of Eq. (22), we are interested in the bilinear product $\dot{h}_{ij}(\tilde{\mathbf{n}})\dot{h}^*_{ij}(\tilde{\mathbf{n}})$. In terms of the $u$-fields, see Eqs. (23) and (24), it can be written as a linear combination of two traces

```math
\dot{h}_{ij}\dot{h}^*_{ij} = \text{Tr}(P\dot{u}\, P\dot{u}^*) - \frac{1}{2}\text{Tr}(P\dot{u})\text{Tr}(P\dot{u}^*)\, , \tag{35}
```

where $\dot{u}$ and $P$ are matrices with elements $(\dot{u})_{ij} = \dot{u}_{ij}$ and $(P)_{ij} = P_{ij}$. Eq. (35) is valid for both real and complex valued projectors. In CosmoLattice, it is explicitly implemented in the following way: first, we define the matrix products $v_{ij} \equiv P_{ik}\dot{u}_{kj}$ and $\tilde{v}_{ij} \equiv P_{ik}\dot{u}^*_{kj}$, and then the trace values are determined from

```math
\text{Tr}(P\dot{u}\, P\dot{u}^*) = v_{11}\tilde{v}_{11} + v_{22}\tilde{v}_{22} + v_{33}\tilde{v}_{33} + v_{12}\tilde{v}_{21} + v_{21}\tilde{v}_{12} + v_{13}\tilde{v}_{31} + v_{31}\tilde{v}_{13} + v_{23}\tilde{v}_{32} + v_{32}\tilde{v}_{23}\, , \tag{36}
```

```math
\text{Tr}(P\dot{u}) = v_{11} + v_{22} + v_{33}\, , \tag{37}
```

```math
\text{Tr}(P\dot{u}^*) = \tilde{v}_{11} + \tilde{v}_{22} + \tilde{v}_{33}\, . \tag{38}
```

In the real case, these computations can be shortened since $\tilde{v} = v^*$.

---

## 3 Gravitational Waves in CosmoLattice

### 3.1 Equation of Motion

In order to numerically study the dynamics of the fields, we work with dimensionless quantities, also known as program variables. In CosmoLattice these are defined from the physical quantities as

```math
\tilde{\phi}_a = \frac{\phi_a}{f_*}, \quad d\tilde{\eta} = a^{-\alpha}\omega_* dt, \quad d\tilde{x}^i = \omega_* dx^i, \quad \kappa^i = \frac{k^i}{\omega_*}, \quad \tilde{u}_{ij} = \frac{m_p}{f_*^2}u_{ij}\, , \tag{39}
```

where $\phi_a$ refers to a scalar field, and $\alpha, f_*$, and $\omega_*$ are constants. The last two have dimensions of energy, whereas $\alpha$ is dimensionless. Their particular value should be chosen based on the matter model which is being simulated, see Ref. [@Figueroa_2021_jcap] for a detailed discussion about this. We denote the time derivative with respect to program time by $' = d/d\tilde{\eta}$ and the gradient $\tilde{\nabla}_i = d/d\tilde{x}^i$. Note we have also redefined the $u$ fields, even if they were already dimensionless.

Numerically, $\tilde{u}$-fields are evolved by defining a conjugate momenta, $(\pi_{\tilde{u}})_{ij} = a^{3-\alpha}\tilde{u}'_{ij}$, which allows to rewrite Eq. (15) as a system of first order differential equations

```math
\tilde{u}'_{ij} = a^{\alpha-3}(\pi_{\tilde{u}})_{ij}\, , \tag{40a}
```

```math
(\pi_{\tilde{u}})'_{ij} = a^{1+\alpha}\tilde{\nabla}^2\tilde{u}_{ij} + 2a^{1+\alpha}\tilde{\Pi}^{eff}_{ij}\, . \tag{40b}
```

For real scalar fields $\tilde{\Pi}^{eff}_{ij} = \tilde{\partial}_i\tilde{\phi}_a\tilde{\partial}_j\tilde{\phi}_a$, $a = 1, 2, \ldots$

Eqs. (40) can then be solved using finite difference methods, see Ref. [@Figueroa_2021_cpc] for a description of the different available algorithms available in CosmoLattice. The energy density power spectrum is computed with Eqs. (22) and (35), by relating the physical time derivative of the $h$-fields to the program conjugate momenta,

```math
\dot{h}_{ij} = \frac{\omega_*}{a^3}\Lambda^L_{ij,kl}\pi_{u,kl} = \frac{\omega_*}{a^3}\left(\frac{f_*}{m_p}\right)^2\Lambda^L_{ij,kl}\pi_{\tilde{u},kl}\, . \tag{41}
```

There are several different ways in which the power spectrum may be calculated, depending on how the number of points per bin $\#_l$ is estimated and on the assignment of a momentum $k$ to each bin. Different possibilities are discussed in detail in [Technical Note I](../tech_notes/Power%20spectra.md). Here we summarize how each one of them is applied to compute the GW energy density power spectrum. In the following subsections we enumerate all the different types and versions implemented in CosmoLattice to compute the GW energy density power spectrum.

### 3.2 GW Power Spectrum: Type I

Power spectrum Type I is based on taking the exact number of modes inside a bin $\#_l$. For a general binning $R(l)$ labeled by $l = 1, 2, \ldots, l_{max}$ and width $\Delta\tilde{n}(l)$, the average of a scalar field is defined according to

```math
\langle f^2\rangle_V = \frac{1}{N^6}\sum_l \sum_{\tilde{\mathbf{n}}\in R(l)} |f(\tilde{\mathbf{n}})|^2 = \frac{1}{N^6}\sum_l \#_l\langle |f(\tilde{\mathbf{n}})|^2\rangle_{R(l)}\, , \tag{42}
```

where we have defined an angular average as $\langle |f(\tilde{\mathbf{n}})|^2\rangle_{R(l)} = \frac{1}{\#_l}\sum_{\tilde{\mathbf{n}}\in R(l)} |f(\tilde{\mathbf{n}})|^2$. We now introduce different versions of the GW energy density power spectrum normalized by the critical energy density, as follows:

#### 3.2.1 Type I - Version 1

The GW energy density power spectrum normalized by the critical energy density for Type I - Version 1 is

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\rho_c}\frac{m_p^2 k(l)\,\delta x}{8\pi N^5}\#_l\left\langle\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, , \tag{43}
```

where $k(l) = k_{IR}\, l$. In program variables this is expressed as

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\tilde{\rho}_c(8\pi a^{2\alpha})}\frac{\kappa(l)}{\delta\tilde{x}}\frac{1}{N^5}\left(\frac{f_*}{m_p}\right)^2 \#_l\, a^{-2(3-\alpha)}\left\langle\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, . \tag{44}
```

#### 3.2.2 Type I - Version 2

The GW energy density power spectrum normalized by the critical energy density for Type I - Version 2 is

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\rho_c}\frac{m_p^2 \langle k(\tilde{\mathbf{n}})\rangle_l\,\delta x}{8\pi N^5}\#_l\left\langle\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, , \tag{45}
```

where $\langle k(\tilde{\mathbf{n}})\rangle_l \equiv \frac{k_{IR}}{\#_l}\sum_{\tilde{\mathbf{n}}\in R(l)} |\tilde{\mathbf{n}}|$. In program variables this is expressed as

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\tilde{\rho}_c(8\pi a^{2\alpha})}\frac{\langle\kappa(\tilde{\mathbf{n}})\rangle_l}{\delta\tilde{x}}\frac{1}{N^5}\left(\frac{f_*}{m_p}\right)^2 \#_l\, a^{-2(3-\alpha)}\left\langle\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, . \tag{46}
```

#### 3.2.3 Type I - Version 3

The GW energy density power spectrum normalized by the critical energy density for Type I - Version 3 is

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\rho_c}\frac{m_p^2\,\delta x}{8\pi N^5}\#_l\left\langle k(\tilde{\mathbf{n}})\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, , \tag{47}
```

and is expressed in program variables as

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\tilde{\rho}_c(8\pi a^{2\alpha})}\frac{1}{\delta\tilde{x}}\frac{1}{N^5}\left(\frac{f_*}{m_p}\right)^2 \#_l\, a^{-2(3-\alpha)}\left\langle\kappa(\tilde{\mathbf{n}})\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, . \tag{48}
```

### 3.3 GW Power Spectrum: Type II

The Power Spectrum Type II relies on estimating the number of modes in each bin of radius $|\tilde{\mathbf{n}}|$ as $\#_{|\tilde{\mathbf{n}}|} \approx 4\pi|\tilde{\mathbf{n}}|^2$. The average over each spherical shell is approximated as

```math
\langle f^2\rangle_{R(l)} \simeq \frac{1}{4\pi|\tilde{\mathbf{n}}|^2}\sum_{\tilde{\mathbf{n}}\in R(l)} |f(\tilde{\mathbf{n}})|^2\, . \tag{49}
```

#### 3.3.1 Type II - Version 1

The GW energy density power spectrum normalized by the critical energy density for Type II - Version 1 is

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\rho_c}\frac{m_p^2 k^3(l)\,\delta x^3}{8\pi^2 N^3}\left\langle\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, , \tag{50}
```

and is expressed in program variables as

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\tilde{\rho}_c(8\pi^2 a^{2\alpha})}\frac{\kappa^3(l)}{N}\left(\frac{\delta\tilde{x}}{N}\right)^2\left(\frac{f_*}{m_p}\right)^2 a^{-2(3-\alpha)}\left\langle\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, . \tag{51}
```

#### 3.3.2 Type II - Version 2

The GW energy density power spectrum normalized by the critical energy density for Type II - Version 2 is

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\rho_c}\frac{m_p^2 \langle k(\tilde{\mathbf{n}})\rangle_l^3\,\delta x^3}{8\pi^2 N^3}\left\langle\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(\tilde{\mathbf{n}})}\, , \tag{52}
```

and is expressed in program variables as

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\tilde{\rho}_c(8\pi^2 a^{2\alpha})}\frac{\langle\kappa(\tilde{\mathbf{n}})\rangle_l^3}{N}\left(\frac{\delta\tilde{x}}{N}\right)^2\left(\frac{f_*}{m_p}\right)^2 a^{-2(3-\alpha)}\left\langle\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, . \tag{53}
```

#### 3.3.3 Type II - Version 3

The GW energy density power spectrum normalized by the critical energy density for Type II - Version 3 is

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\rho_c}\frac{m_p^2\,\delta x^3}{8\pi^2 N^3}k^3(\tilde{\mathbf{n}})\left\langle\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, , \tag{54}
```

and is expressed in program variables as

```math
\Omega_{GW}(\tilde{\mathbf{n}},t) = \frac{1}{\tilde{\rho}_c(8\pi^2 a^{2\alpha})}\frac{1}{N}\left(\frac{\delta\tilde{x}}{N}\right)^2\left(\frac{f_*}{m_p}\right)^2 a^{-2(3-\alpha)}\kappa^3(\tilde{\mathbf{n}})\left\langle\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R(l)}\, . \tag{55}
```

---

## 4 A Working Example: Inflationary Potential { #sec_AWorkingExampleInflationaryPotential }

Here we present an example of gravitational wave production due to the self-resonance of an inflaton with monomial potential $V(\phi) = \frac{1}{4}\lambda\phi^4$. The self-resonance of $\phi$ produces a series of peaks in its power spectrum, which will then be imprinted as well in the GW power spectrum. Whereas the model file does not need to be modified (i.e. the model file remains the same as in the absence of GWs), to tell CosmoLattice that we want to run the field dynamics including GW production, we simply need to indicate this in the parameter file. Below we present an example of the parameter file to study GW production in the mentioned example model.

`src/models/parameter-files/lph4.in`:

```
#Output
outputfile = ./

#Evolution
expansion = true
evolver = LF

#Lattice
N = 256
dt = 0.05
kIR = 0.2

#Times
tOutputFreq = 5
tOutputInfreq = 5
tMax = 2000
baseSeed = 1234

#Power spectrum options
PS_type = 1
PS_version = 1

#GWs
GWprojectorType = 1
withGWs=true

#IC
kCutOff = 4
initial_amplitudes = 5.6964e18 # homogeneous amplitudes in GeV
initial_momenta = -4.86735e30 # homogeneous amplitudes in GeV2

#Model Parameters
lambda = 9e-14
```

The parameters that control the GW module are:

- **`withGWs`**: Boolean parameter to turn On or Off the GW evolution.
- **`GWprojectorType`**: Numerical parameter that allows to choose between different GW projectors $P_{ij}$ according to the choice of lattice momentum $k_L$, see Eqs. (28) and (29).
  - `GWprojectorType = 1`: implies choosing $k_L = k^0_L$
  - `GWprojectorType = 2`: implies choosing $k_L = k^-_L$
  - `GWprojectorType = 3`: implies choosing $k_L = k^+_L$

  default option is `GWprojectorType = 2`.

The output related to GW production is presented in the following generated files:

- **`spectra_gws.txt`**: This file contains the normalized GW energy density power spectrum. For the default choice of `spectraVerbosity` this file prints:

```math
\kappa,\quad \Omega_{GW}(k,t),\quad \#_l\, . \tag{56}
```

  Extra columns are printed for different choices of the `spectraVerbosity`, see [Technical Note I](../tech_notes/Power%20spectra.md) for a complete explanation on the spectra output.

- **`energy_gws.txt`**: This file contains the total energy density in GWs, computed from numerically integrating the PS as in Eq. (21). It prints:

```math
\tilde{\eta},\quad \frac{\tilde{\rho}_{GW}}{\tilde{\rho}_{tot}}(t),\quad \tilde{\rho}_{GW}(t)\, . \tag{57}
```

!!! warning
    **Important Note:** While the GW energy density spectrum at the time of production $\Omega_{GW}$ is typically normalized in an expanding universe by the critical energy density $\tilde{\rho}_c$, in CosmoLattice we rather normalize it by the total energy density of the matter field sector $\tilde{\rho}_{tot}$ (let it be composed of scalar fields only, or scalar and gauge fields), independently of whether we simulate the dynamics in an expanding background or in Minkowski. In the case of self-consistent expansion $\tilde{\rho}_{tot} = \tilde{\rho}_c$, and hence we recover the standard definition. However, for a fixed-background expansion, if the user wishes to obtain the spectrum normalized to the critical energy density, they should multiply the CosmoLattice output (second column of `spectra_gws.txt`) by the ratio $\tilde{\rho}_{tot}/\tilde{\rho}_c$.

### 4.1 GW Energy Density Power Spectra Examples

The model $\lambda\phi^4$ excites a series of peaks in the GW energy density power spectrum due to self resonance. The program variables as defined in Eq. (39) for this particular model are

```math
f_* = \phi_*, \quad \omega_* = \sqrt{\lambda}\phi_*, \quad \text{and} \quad \alpha = 1\, , \tag{58}
```

where $\phi_*$ is the initial amplitude of the field. We performed several simulations with the same initial conditions for all the different types and versions of the power spectrum, and all three variants of the GW projectors. Each spectrum is measured up to time $\tilde{\eta} = 2000$ every $\Delta\tilde{\eta} = 25$ time units. In the top panels of Fig. 1 we show the difference in the spectra depending on the type of power spectrum, with fixed GW projector type and PS version. As expected, Type I captures better the UV tail of spectra, as it takes into account the exact multiplicity of modes in the outer shells of the binning, in contrast to the approximated multiplicity of Type II. For a complete explanation of the difference between power spectrum types see [Technical Note I](../tech_notes/Power%20spectra.md). In the bottom panels, we show the difference in the spectra depending on the GW projector for a fixed PS type and version. The spectra are almost identical besides small differences in the UV tails. This agrees with the results of Ref. [@Figueroa_2011]. Finally, we checked the transversality and tracelessness conditions of the $h_{ij}(\mathbf{n},t)$ in the lattice. For this we compute the average of the following dimensionless ratios:

```math
\delta(t) \equiv \frac{\langle\nabla^L_i h_{ij}(\mathbf{n},t)\rangle}{\langle D^L_i h_{ij}(\mathbf{n},t)\rangle}\, , \tag{59}
```

```math
\lambda(t) \equiv \frac{\langle\sum_i |h_{ii}(\mathbf{n},t)|\rangle}{\langle|\sum_i h_{ii}(\mathbf{n},t)|\rangle}\, , \tag{60}
```

where $\nabla^L$ are the different discretized spatial derivatives defined in Eqs. (25) and (26), and $D^L_i$ are defined as follows

```math
D^0_i h_{ij} \equiv \frac{h_{ij}(\mathbf{n} + \hat{i},t) + h_{ij}(\mathbf{n} - \hat{i},t)}{2\delta x}\, , \tag{61}
```

```math
D^\pm_i h_{ij} \equiv \frac{h_{ij}(\mathbf{n} + \hat{i},t) + h_{ij}(\mathbf{n},t)}{\delta x}\, . \tag{62}
```

In Fig. 2 we see that both transversality and tracelessness are satisfied to machine precision. The jump in the curve just before $\tilde{\eta} \sim 1000$ corresponds to the backreaction of the inflaton onto itself.

---

## 5 Use of GW Module for Complex Scalar Fields

In the previous example and all along the note, we have only considered real scalar fields as sources for the GWs. However, CosmoLattice is also prepared to simulate the GW production of models containing complex scalar fields in the absence of gauge fields, just by setting `withGWs = true` as before in the parameter file. For any complex field, defined as $\varphi = (\phi_1 + i\phi_2)/\sqrt{2}$, the contribution to the anisotropic stress tensor is computed as

```math
\Pi^{eff}_{ij} = 2\text{Re}\{\partial_i\varphi\partial_j\varphi^*\} = \partial_i\phi_1\partial_j\phi_1 + \partial_i\phi_2\partial_j\phi_2\, . \tag{63}
```

For U(1) Abelian gauge theories (including charged complex scalar fields and Abelian gauge bosons), see Technical Note III.

---

## Appendix A: Where do Gravitational Waves Live in the Lattice?

In order to compute the power spectrum of gravitational waves in the lattice, we have to address the question of where the GWs (or the $u_{ij}$ fields) live in the lattice. Looking at Eq. (15), the $u_{ij}$ fields live where the source lives. If scalar fields live at lattice sites then the product $\partial_i\phi\partial_j\phi$ lives at the middle of the plaquettes

```math
\left(\partial_i\phi\,\partial_j\phi\right)\left(\mathbf{n} + \frac{\hat{i}}{2} + \frac{\hat{j}}{2}\right)\, , \tag{64}
```

and so we choose to define the $u_{ij}$ fields to live in those same positions

```math
u_{ij}\left(\mathbf{n} + \frac{\hat{i}}{2} + \frac{\hat{j}}{2}\right)\, . \tag{65}
```

If we wish to ascribe the product $u_{ij}u_{ij}$ to live at the lattice sites $\mathbf{n}$, we can obtain this by computing the clover averaging over neighboring plaquettes

```math
\langle u_{ij}u_{ij}(\mathbf{n})\rangle_{clov} = \frac{1}{4}\left[(u_{ij}u_{ij})\left(\mathbf{n} + \frac{\hat{i}}{2} + \frac{\hat{j}}{2}\right) + (u_{ij}u_{ij})\left(\mathbf{n} + \frac{\hat{i}}{2} - \frac{\hat{j}}{2}\right) + (u_{ij}u_{ij})\left(\mathbf{n} - \frac{\hat{i}}{2} + \frac{\hat{j}}{2}\right) + (u_{ij}u_{ij})\left(\mathbf{n} - \frac{\hat{i}}{2} - \frac{\hat{j}}{2}\right)\right]\, . \tag{66}
```

We now consider the following summation over all lattice sites, $\sum_{\mathbf{n}} \langle\dot{u}_{ij}\dot{u}_{ij}(\mathbf{n})\rangle_{clov}$. We can show that this sum is equal up to an error $O(\delta x^2)$ to the sum over the product $\dot{u}_{ij}\dot{u}_{ij}$ as if we considered that $u_{ij}$ live on the lattice sites $\mathbf{n}$, instead of in the middle of the plaquettes. We Taylor expand each of the terms of Eq. (66) around $\mathbf{n}$ such that the sum becomes

```math
\sum_{\mathbf{n}} \langle\dot{u}_{ij}\dot{u}_{ij}(\mathbf{n})\rangle_{clov} = \sum_{\mathbf{n}} \frac{1}{4}\left[(u_{ij}u_{ij})(\mathbf{n}) + \frac{\delta x}{2}\partial_{\hat{i}}(u_{ij}u_{ij})(\mathbf{n}) + \frac{\delta x}{2}\partial_{\hat{j}}(u_{ij}u_{ij})(\mathbf{n}) + (u_{ij}u_{ij})(\mathbf{n}) + \frac{\delta x}{2}\partial_{\hat{i}}(u_{ij}u_{ij})(\mathbf{n}) - \frac{\delta x}{2}\partial_{\hat{j}}(u_{ij}u_{ij})(\mathbf{n}) + (u_{ij}u_{ij})(\mathbf{n}) - \frac{\delta x}{2}\partial_{\hat{i}}(u_{ij}u_{ij})(\mathbf{n}) + \frac{\delta x}{2}\partial_{\hat{j}}(u_{ij}u_{ij})(\mathbf{n}) + (u_{ij}u_{ij})(\mathbf{n}) - \frac{\delta x}{2}\partial_{\hat{i}}(u_{ij}u_{ij})(\mathbf{n}) - \frac{\delta x}{2}\partial_{\hat{j}}(u_{ij}u_{ij})(\mathbf{n})\right] + O(\delta x^2)\, ; \tag{67}
```

it turns out that all linear terms cancel out with each other and hence we obtain

```math
\sum_{\mathbf{n}} \langle u_{ij}u_{ij}(\mathbf{n})\rangle_{clov} = \sum_{\mathbf{n}} (u_{ij}u_{ij})(\mathbf{n}) + O(\delta x^2)\, . \tag{68}
```

We can safely choose that our $u_{ij}$ fields, and therefore $h_{ij}$, live at lattice sites $\mathbf{n}$ instead of in the center of the plaquettes.