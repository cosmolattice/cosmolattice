# CosmoLattice Technical Note III: Gravitational Waves from U(1) Gauge Theories

**Written on June 16, 2023**

## Authors

- Jorge Baeza-Ballesteros* - Instituto de Física Corpuscular (IFIC), Consejo Superior de Investigaciones Científicas (CSIC) and Universitat de Valencia (UV), 46980, Valencia, Spain
- Daniel G. Figueroa† - Instituto de Física Corpuscular (IFIC), Consejo Superior de Investigaciones Científicas (CSIC) and Universitat de Valencia (UV), 46980, Valencia, Spain
- Nicolás Loayza‡ - Instituto de Física Corpuscular (IFIC), Consejo Superior de Investigaciones Científicas (CSIC) and Universitat de Valencia (UV), 46980, Valencia, Spain

**Code Developers:**
- Jorge Baeza-Ballesteros
- Nicolás Loayza

**Contact:**
- \* jorge.baeza@ific.uv.es
- † daniel.figueroa@ific.uv.es
- ‡ nicolas.loayza@ific.uv.es

## Abstract

This is a technical note about the dynamics of gravitational waves (GWs) sourced by U(1) charged complex scalars and Abelian gauge fields in a lattice. All new features are implemented in the (updated) GW module that forms part of CosmoLattice v1.2, which is publicly available in [http://www.cosmolattice.net](http://www.cosmolattice.net). We recommend the reader to check out as well other technical notes available there.

**Important Note:** If the reader is not familiar with the concept of gravitational waves in a lattice, we recommend them to read first our [`Technical Note II`](../tech_notes/Gravitational%20waves.md), for a more detailed introduction of the relevant concepts.

---

## 1 Gravitational Waves in the Continuum

We assume the reader is familiar with the concept of gravitational waves (GWs) in the continuum. Nevertheless, we review briefly the definition of GWs in a spatially-flat Friedman-Lemaître-Robertson-Walker (FLRW) metric. This allows us to remind the reader of the strategy for numerically solving the GW dynamics and to introduce the field variables we need in an Abelian gauge theory. GWs are identified with perturbations $h_{ij}$ of the background metric which are transverse and traceless (TT), i.e.,

```math
ds^2 = -dt^2 + a^2(t)(\delta_{ij} + h_{ij})dx^i dx^j, \quad \text{with} \quad \partial_i h_{ij} = 0 \quad \text{and} \quad h_{ii} = 0\,, \tag{1}
```

where $a$ is the scale factor, $t$ represents coordinate time and $x^i$ are spatial coordinates, with latin indices running from 1 to 3. Throughout this note, summation is assumed over repeated indices, unless otherwise stated. In a FLRW background, the dynamics of GWs are described by equations of motion of the form [@Caprini_2018]

```math
\ddot{h}_{ij} + 3H\dot{h}_{ij} - \frac{\nabla^2}{a^2} h_{ij} = \frac{2}{m_p^2 a^2}\Pi^{TT}_{ij}\,, \tag{2}
```

where $\dot{h}_{ij} = dh_{ij}/dt$, $H = \dot{a}/a$ is the Hubble rate, $m_p = 1/\sqrt{8\pi G} = 2.44 \times 10^{18}$ GeV is the reduced Planck mass and $\Pi^{TT}_{ij}$ is the transverse-traceless (TT) part of the anisotropic tensor $\Pi_{ij}$. The conditions $\partial_i \Pi^{TT}_{ij} = \Pi^{TT}_{ii} = 0$ hold $\forall \mathbf{x}, t$. Obtaining the TT part of a tensor is a non-local operation, and so is computationally very expensive. Thus, instead of solving directly Eq. (2), we follow the procedure introduced in Ref. [@GarciaBellido_2008]. In particular, we write the TT tensor perturbations as

```math
h_{ij}(\mathbf{k}, t) = \Lambda_{ij,kl}(\hat{k})u_{kl}(\mathbf{k}, t)\,, \tag{3}
```

where $\Lambda_{ij,kl}$ is a projection operator defined in Fourier space as

```math
\Lambda_{ij,lm}(\hat{k}) \equiv P_{il}(\hat{k})P_{jm}(\hat{k}) - \frac{1}{2}P_{ij}(\hat{k})P_{lm}(\hat{k}), \quad \text{with} \quad P_{ij}(\mathbf{k}) = \delta_{ij} - \hat{k}_i\hat{k}_j, \quad \hat{k}_i = k_i/k\,, \tag{4}
```

with $k$ is the momentum. In Eq. (3) the functions $u_{ij}(\mathbf{k}, t)$ are the Fourier transform of the solution to

```math
\ddot{u}_{ij} + 3H\dot{u}_{ij} - \frac{\nabla^2}{a^2} u_{ij} = \frac{2}{m_p^2 a^2}\Pi^{\text{eff}}_{ij}\,, \tag{5}
```

where $\Pi^{\text{eff}}_{ij}$ is an effective anisotropic stress tensor that contains only those contributions from $\Pi_{ij}$ which have non-vanishing TT-projection.

Consider now a theory with one real singlet field and one complex scalar field charged under an Abelian U(1) gauge symmetry, and the corresponding gauge boson. The action is

```math
S = -\int d^4x\sqrt{-g}\,\mathcal{L} = -\int d^4x\sqrt{-g}\left[\frac{1}{2}\partial_\mu\varphi\partial^\mu\varphi + (D_\mu\phi)^*(D^\mu\phi) + \frac{1}{4}F_{\mu\nu}F^{\mu\nu} + V(\varphi, |\phi|)\right], \tag{6}
```

where $\mathcal{L}$ is the Lagrangian of the model, $g = \det(g_{\mu\nu})$ is the determinant of the metric and

```math
\varphi \in \mathbb{R}, \quad \phi = \frac{1}{\sqrt{2}}(\varphi_1 + i\varphi_2) \in \mathbb{C}, \quad \text{with} \quad \varphi_1, \varphi_2 \in \mathbb{R}\,. \tag{7}
```

The U(1) gauge field stress tensor and gauge covariant derivative are defined as

```math
F_{\mu\nu} = \partial_\mu A_\nu - \partial_\nu A_\mu, \quad \text{and} \quad D_\mu\phi = \partial_\mu\phi - ig_A Q_A A_\mu\phi\,, \tag{8}
```

where $g_A$ is the gauge coupling and $Q_A$ is the U(1) charge of $\phi$. From the Lagrangian in Eq. (6), we can write down the stress-energy tensor of the scalar and gauge fields.

```math
T_{\mu\nu} \equiv -\frac{2}{\sqrt{g}}\frac{\delta(\sqrt{g}\,\mathcal{L})}{\delta g^{\mu\nu}} = g_{\mu\nu}\mathcal{L} - 2\frac{\delta\mathcal{L}}{\delta g^{\mu\nu}}
```

```math
= -g_{\mu\nu}\left[g^{\alpha\beta}\left((D_\alpha\phi)^*(D_\beta\phi) + \frac{1}{2}(\partial_\alpha\varphi)(\partial_\beta\varphi) + \frac{1}{4}g^{\alpha\delta}g^{\beta\lambda}(F_{\alpha\beta}F_{\delta\lambda})\right) + V(\varphi, \phi)\right]
```

```math
+ (\partial_\mu\varphi)(\partial_\nu\varphi) + \left[2\text{Re}\{(D_\mu\phi)^*(D_\nu\phi)\} + g^{\alpha\beta}F_{\mu\alpha}F_{\nu\beta}\right]\,. \tag{9}
```

From here, one can determine with a bit of work [@Figueroa_2010_phd] the effective source of Eq. (5), which contains only those parts of $\Pi_{ij}$ that do not vanish after TT-projecting, namely

```math
\Pi^{\text{eff}}_{ij} = \partial_i\varphi\,\partial_j\varphi + 2\text{Re}\{(D_i\phi)^*(D_j\phi)\} - a^{-2\alpha}E_i E_j + a^{-2}B_i B_j\,, \tag{10}
```

where $E_i$ and $B_i$ are the Abelian electric and magnetic fields, defined as

```math
E_i \equiv F_{0i}, \quad \text{and} \quad B_i \equiv \frac{1}{2}\epsilon_{ijk}F^{jk}\,. \tag{11}
```

---

## 2 Gravitational Waves in CosmoLattice

In order to numerically study the dynamics of the fields, one needs to work with dimensionless quantities, also known as program variables. In CosmoLattice they are defined as

```math
\tilde{\varphi}_a = \frac{\varphi_a}{f_*}, \quad d\tilde{\eta} = a^{-\alpha}\omega_* dt, \quad d\tilde{x}^i = \omega_* dx^i, \quad \kappa_i = \frac{k_i}{\omega_*}, \quad \tilde{A}_\mu = \frac{A_\mu}{\omega_*}\,, \tag{12}
```

where $\varphi_a$ refers to any scalar field (real or complex), $A_\mu$ to an Abelian gauge field, and $\alpha$, $f_*$ and $\omega_*$ are constants. The last two have dimensions of energy, whereas $\alpha$ is dimensionless. Their particular values should be chosen based on the model that one wishes to simulate, see Ref. [@Figueroa_2021_jcap] for a detailed discussion. We denote the time derivative with respect to program time by $' = d/d\tilde{\eta}$ and the gradient $\tilde{\nabla}_i = d/d\tilde{x}^i$. The source of gravitational waves is the transverse-traceless part $\Pi^{TT}_{ij}$ of the anisotropic stress-energy tensor. For the case of the $u$ fields we can write down their source as the effective anisotropic tensor,

```math
\tilde{\Pi}^{\text{eff}}_{ij} = \frac{\Pi^{\text{eff}}_{ij}}{\omega_*^2 f_*^2} = \tilde{\nabla}_i\tilde{\varphi}\,\tilde{\nabla}_j\tilde{\varphi} + 2\text{Re}\{(\tilde{D}_i\tilde{\phi})^*(\tilde{D}_j\tilde{\phi})\} - \frac{\omega_*^2}{f_*^2}\left[a^{-2\alpha}\tilde{E}_i\tilde{E}_j + a^{-2}\tilde{B}_i\tilde{B}_j\right], \tag{13}
```

where $\tilde{E}_i$ and $\tilde{B}_i$ are the Abelian electric and magnetic fields, in program units, defined as

```math
\tilde{E}_i \equiv \frac{E_i}{\omega_*^2} \quad \text{and} \quad \tilde{B}_i \equiv \frac{B_i}{\omega_*^2}\,. \tag{14}
```

and $\tilde{D}_i = \tilde{\nabla}_i - ig_A Q_A \tilde{A}_i$. In CosmoLattice, we define normalized $\tilde{u}$ fields as

```math
h_{ij}(\mathbf{k},t) = \Lambda_{ij,kl}(\hat{k})u_{kl}(\mathbf{k},t) = \left(\frac{f_*}{m_p}\right)^2 \Lambda_{ij,kl}^L(\hat{k})\tilde{u}_{kl}(\mathbf{k},t)\,, \tag{15}
```

and their conjugate momentum as $(\pi_{\tilde{u}})_{ij} = a^{3-\alpha}\tilde{u}'_{ij}$. The lattice definition of $\Lambda^L_{ij,kl}$ depends on the particular choice of the lattice momenta, see [`Technical Note II`](../tech_notes/Gravitational%20waves.md) for a detailed discussion. This allows to rewrite Eq. (5) as a system of first order differential equations

```math
\begin{cases}
\tilde{u}'_{ij} = a^{\alpha-3}(\pi_{\tilde{u}})_{ij} \\
(\pi_{\tilde{u}})'_{ij} = a^{1+\alpha}\tilde{\nabla}^2\tilde{u}_{ij} + 2a^{1+\alpha}\tilde{\Pi}^{\text{eff}}_{ij}\,,
\end{cases} \tag{16}
```

which can be solved using finite difference methods, see Ref. [@Figueroa_2021_cpc] for a description of the available algorithms in CosmoLattice. The gravitational wave energy density power spectrum normalized by the critical energy can then be computed in different ways, see [Technical Note I](../tech_notes/Power%20spectra.md) and [Technical Note II](../tech_notes/Gravitational%20waves.md) for a discussion of the options available in CosmoLattice. By default, the Type I - Version 1 power spectrum is computed, in which the exact multiplicity of modes per bin, $\#_l$, is counted and all modes are assigned the central $k$ value for the bin,

```math
\Omega_{GW}(\tilde{\mathbf{n}}, t) = \frac{1}{\rho_c}\frac{m_p^2\, k^{(l)}\,\delta x}{8\pi N^5}\#_l\left\langle\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R^{(l)}}\,, \tag{17}
```

where the different variables are defined in [`Technical Note II`](../tech_notes/Gravitational%20waves.md). In program variables this is expressed as

```math
\Omega_{GW}(\tilde{\mathbf{n}}, t) = \frac{1}{\tilde{\rho}_c}\frac{\kappa^{(l)}}{8\pi a^{2\alpha}}\frac{\delta\tilde{x}}{N^5}\left(\frac{f_*}{m_p}\right)^2\#_l\, a^{-2(3-\alpha)}\left\langle\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R^{(l)}}\,. \tag{18}
```

For completeness, we also quote the Type II - Version 1 power spectrum, in which the number of modes per bin is approximated $\#_l \approx 4\pi l^2$. In this case, the normalized GW energy density power spectrum is defined as

```math
\Omega_{GW}(\tilde{\mathbf{n}}, t) = \frac{1}{\rho_c}\frac{m_p^2\, k^3(l)\,\delta x^3}{8\pi^2 N^3}\left\langle\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]\left[\dot{h}_{ij}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R^{(l)}}\,, \tag{19}
```

and is expressed in program variables as

```math
\Omega_{GW}(\tilde{\mathbf{n}}, t) = \frac{1}{\tilde{\rho}_c}\frac{\kappa^3(l)}{8\pi^2 a^{2\alpha}}\left(\frac{\delta\tilde{x}}{N}\right)^3\left(\frac{f_*}{m_p}\right)^2 a^{-2(3-\alpha)}\left\langle\left[\Lambda_{ij,kl}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{kl}(\tilde{\mathbf{n}},t)\right]\left[\Lambda_{ij,mn}(\tilde{\mathbf{n}})(\pi_{\tilde{u}})_{mn}(\tilde{\mathbf{n}},t)\right]^*\right\rangle_{R^{(l)}}\,. \tag{20}
```

> **Important Note:** While the GW energy density spectrum at the time of production $\Omega_{GW}$ is typically normalized in an expanding universe by the critical energy density $\tilde{\rho}_c$, in CosmoLattice we rather normalize it by the total energy density of the matter field sector $\tilde{\rho}_{\text{tot}}$ (let it be composed of scalar fields only, or scalar and gauge fields), independently of whether we simulate the dynamics in an expanding background or in Minkowski. In the case of self-consistent expansion $\tilde{\rho}_{\text{tot}} = \tilde{\rho}_c$, and hence we recover the standard definition. However, for a fixed-background expansion, if the user wishes to obtain the spectrum normalized to the critical energy density, they should multiply the CosmoLattice output (second column of `spectra_gws.txt`) by the ratio $\tilde{\rho}_{\text{tot}}/\tilde{\rho}_c$.
