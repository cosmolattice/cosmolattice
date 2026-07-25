# Physics findings from the documentation review

Branch `docs/agent-review`, baseline `3dba1db5`. Covers all 44 live pages of the site.

**None of the issues below were edited.** Every physics or equation-level finding was
deliberately left in place for a physicist to decide. The commits on the branch contain
only typos, grammar, broken links, malformed Markdown/LaTeX, and code-verifiable name
drift (parameter keys, file names, class names).

## How to read the verification status

| Status | Meaning |
|---|---|
| **Adjudicated** | Two reviewers reached the same verdict independently, at least one by deriving it from the C++ rather than from another page. Highest confidence. |
| **Confirmed vs code** | A specific `file:line` in the C++ was quoted and contradicts the page. Single reviewer. |
| **Reported** | Internal inconsistency or a claim that could not be settled from the code. Needs your judgement, not more grep. |

Every finding carries a verbatim quote so you can locate it without re-deriving the search.

---

## 1. Adjudicated contradictions

These are cases where two pages disagreed. A second reviewer was given the conflict and
asked to settle it from the code. All three came back with a clear verdict.

### 1.1 `Manual/ALP.md:50` — missing factor of 2 on the kinetic term

```
\ddot{a} &= -\frac{a}{3m_p^2}\big( E_{K} - E_{V} + E_{EM} \big)
```

**Verdict: ALP.md is wrong.** The coefficient on `E_K` should be `2E_K` (at `alpha = 0`).

Three independent lines of evidence:

- `include/CosmoInterface/evolvers/kernels/scalefactorkernels.h:73-76` carries the
  coefficient `(alpha-2)` on `E_K`, which at `alpha = 0` gives `-2E_K`.
- The same equation in program units *on ALP.md itself* (line 264) carries `(alpha-2)`.
- `Manual/My first model of gauge fields.md` `eq_FriedmannDDa` also carries `(alpha-2)`,
  and was separately verified against `scalefactorkernels.h` **and** by an independent
  derivation from `a''/a = (a^{2alpha}/omega_*^2)(alpha H^2 + adotdot/a)`.
- General relativity gives `rho + 3p = 4E_K - 2E_V` for a scalar, consistent with `2E_K`.

The `E_V` and `E_EM` coefficients on ALP.md are correct. Only the kinetic term is affected.

### 1.2 `th_framework/Brief_Review_On_Continuum_Dynamics.md:223` — two of three signs wrong

```
E'_i + (1 - \alpha)\mathcal{H} E_i - a^{2(\alpha-1)}\epsilon_{ijk}\partial_jB_k
  &=& \frac{\alpha_\Lambda}{m_p} a^{\alpha-1}\left(\phi'B_i+\epsilon_{ijk}\partial_j\phi E_k\right)
```

**Verdict: the continuum-review page is wrong; `Manual/ALP.md` is right.**

The curl term and the `phi' B_i` term both have the wrong sign. The
`eps_{ijk} d_j phi E_k` term happens to come out right.

Three independent checks:

- **Code.** `include/CosmoInterface/definitions/axioncouplings.h::U1AxionCoupling` has
  `AxionCoupl1 = -0.5 * alphaLambda * (piS * B4 + ...)`. Substituting
  `(pi_A)_i = a^{1-alpha} E_i` and `pi_phi = a^{3-alpha} phi'` gives a **minus** on the
  `alpha_Lambda phi' B_i` term.
- **Internal.** This page's own `eq_U1eom` (line 106) has `-a^{-2(1-alpha)} d_j F_{ji}`,
  and `F_{ji} = -eps_{ijk} B_k`, so the curl term on the LHS must be
  `+a^{2(alpha-1)} eps_{ijk} d_j B_k` — the opposite of what is printed.
- **Analytic.** Taking `d_i` of the E-field EOM and using `d_i B_i = 0`,
  `B'_i = eps_{ijk} d_j E_k` reproduces the eta-derivative of `eq_axion_gauss`
  (line 229, identical on both pages) *only* with the ALP.md right-hand side.

The intended form is
`E'_i + (1-alpha)H E_i + a^{2(alpha-1)} eps_{ijk} d_j B_k = -(alpha_Lambda/m_p) a^{alpha-1} (phi'B_i - eps_{ijk} d_j phi E_k)`.

**This block is duplicated.** `Manual/Introduction to CosmoLattice.md:353` carries the
identical wrong signs (copied in commit `80447e7c`, "needs to be incorporated properly").
Both copies need the same correction, or the duplicate should be deleted.

### 1.3 Discrete Fourier transform sign convention

`Manual/Conventions and Notation.md:29` (the notation authority) and two `th_framework`
chapters define the DFT with **opposite exponent signs**.

**Verdict: the authority page is right; the two chapters are reversed.**

Evidence from the code:

- TempLat maps configuration → Fourier onto FFTW's `fftw_plan_dft_r2c`
  (`templat/include/TempLat/fft/external/fftw/fftwplanner.h:87-89, 143-145`), whose
  exponent sign is `-1`, i.e. `e^{-i}`.
- The KokkosFFT backend uses `KokkosFFT::Direction::forward` for the r2c plan
  (`kokkosfft/kokkosfftplanner.h:96`) — same sign.
- Corroborating: the chiral projector
  (`include/CosmoInterface/definitions/chiralpowerspectrum.h:43, 84`) applies
  `phase = -pi*n/N` to move Fourier-space gauge fields off half-integer sites, which is
  the correct half-site correction **only** if the reconstruction carries `e^{+i}` — the
  authority convention.

**Do not fix this with a one-line edit.** `Brief_Review_On_Lattice_Techniques.md:57` is
internally self-consistent with its own `eqn_latticemomentum` (line 150), `eqn_k0`
(line 156) and `eqn_kpm` (line 164). Flipping line 57 alone silently inverts the sign of
`eqn_k0` and the phase of `eqn_kpm`. Two coherent repairs exist:

- **(a)** swap the exponents in `eq_FTdiscrete` **and** flip `eqn_latticemomentum` from
  `-i k_L` to `+i k_L`, leaving `eqn_k0`/`eqn_kpm` untouched. Smaller diff, but changes
  the sign convention of `k_L` relative to The Art-I.
- **(b)** swap the exponents **and** negate `k^0_L` in `eqn_k0` while flipping `e^{∓}` to
  `e^{±}` in `eqn_kpm`. Preserves the `-i k_L` relation.

The same reversed DFT is also on `th_framework/LatticeIntro.md:39`, an orphan page that is
nevertheless built and published.

---

## 2. Confirmed discrepancies between the docs and the code

### 2.1 `Manual/GW.md:171` — Forward/Backward derivative tags swapped

```
[\texttt{Forward derivative}],
```

The bracket tags on `k^-_{L,j}` and `k^+_{L,j}` in `eq_GWLatticeMomenta_1` are swapped.

The page contradicts *itself*: line 181 says "the forward and backward momenta,
`k^{\pm}_L`" (+ = forward), and the projector table at lines 402-404 says type 2 =
"Backward projector, built from `k^-_L`" / type 3 = "Forward projector, built from
`k^+_L`".

The code agrees with the table, not the tags: `gwsprojector.h` `GWProjectorType2` builds
`kL = complex(sin, -1+cos) = sin - i(1-cos) = k^-_L`; `GWProjectorType3` builds
`sin + i(1-cos) = k^+_L`.

Fix is to swap the two tags. Confirm against The Art-II before editing.

### 2.2 `Manual/My first model of gauge fields.md:590` — missing minus sign

```
(\tilde\pi_\varphi)' = \mathcal{K}_{\varphi}[...] \equiv a^{3+\alpha} \widetilde V_{,|\tilde\varphi|} \frac{1}{2} \frac{\tilde\varphi}{|\tilde\varphi|} + a^{1+\alpha} \vec{\widetilde D}_A^2 \tilde\varphi
```

The potential-derivative term has no minus sign, unlike the singlet kernel (line 589) and
the doublet kernel (line 591), which both start with `- a^{3+\alpha}`.

`include/CosmoInterface/evolvers/kernels/complexscalarkernels.h:33` returns
`pow(aI,1+alpha)*covLaplacianCS(...) - pow(aI,3+alpha)/2*Potential::derivCS(...)` — a
**minus** on the potential term.

Found independently by two reviewers (the gauge-fields reviewer and the
`What CosmoLattice does in detail` reviewer, the latter noting that a reader following the
cross-link sees the opposite sign from the snippet embedded next to it).

`Manual/What CosmoLattice does in detail.md:147` links directly to this anchor.

### 2.3 `Manual/IC.md:444` — wrong claim about U(1) initial-condition types

> Other names may be accepted by the parser, such as `PlaneWaves`, `DefectsNetwork`, and
> `DefectsWhiteNoise`, but they are not dispatched by the U(1) initializer and lead to an
> error.

False for two of the three. `include/CosmoInterface/initializers/u1initializer.h:61-64`
dispatches `DefectsNetwork` → `initializeStringNetwork` and `DefectsWhiteNoise` →
`initializeStringNoise`; lines 46-47 make one of the two **mandatory** for
`Model::DefectsModel`. Only `PlaneWaves` falls through to the `U1ICNotImplemented` throw.

The preceding sentence ("The U(1) initializer implements three choices") is therefore also
wrong — five of the seven enum values are dispatched.

Independently confirmed by the `What CosmoLattice does in detail` reviewer, which found the
same claim contradicted by the `u1initializer.h#L41-L67` snippet embedded two lines below it.

### 2.4 `Manual/Defects.md` — several

| Line | Issue | Code reference |
|---|---|---|
| 373 | `K_{U(1)}` puts the resolution-preserving factor in the denominator, but the code **multiplies** for electric energy and **divides** for magnetic. The page gives both `1/Λ_RP`. | `energies.h::electricU1` (multiplies, with an explicit comment) vs `magneticU1` (divides); `PITensor.h` does the same |
| 103 | Fluctuation amplitude has three mismatches: `2π` where the code has `√(2π)`; missing factor `1/2`; exponent `-k²/ℓ²` where the code has `-k²ℓ²/2`. The exponent also contradicts `eq_defectsNetworkIC` eight lines above. | `initializers/fluctuationsgenerator.h:72-79` |
| 172 | Kronecker delta `δ_{N,0}` should be `δ_{N,1}` — the bias term exists only for `NScalars = 1`. Field written `φ_0` where the multiplet is indexed `a = 1..N`. | `models/defects_ONGlobal.h` uses `IfElse(ModelPars::NScalars == 1, ...)` |
| 40 | Same `φ_0²` vs `φ_1²` drift in the physical EOM; `V_bias[φ_1] = qvφ_1³` differentiates to `3qvφ_1²`. Looks like leftover 0-based C++ indexing. | — |
| 395 | `E_{E,str}` and `E_{B,str}` carry `a/2` and `1/(2a)` where `eq_localEnergiesFattening` twenty lines above gives both `1/(2a⁴)`. Working through the code gives `1/a`, not `a`, and neither equation carries the `Λ_RP` the code applies. | `energies.h` + `defectsmeasurer.h` |
| 120 | Amplitude `A` enters linearly, but `deltaNoise` enters the **squared** amplitude quadratically in the code. Also a spare overall factor `1/2`. | `fluctuationsgenerator.h:80-92` |
| 75 | "only simulations with fixed expansion or flat background are allowed" — the code **throws** whenever `!fixedBackground && Model::DefectsModel`, and forces `fixedBackground = false` when `expansion = false`. So a flat-background defect run is currently rejected. | `scalefactorinitializer.h` |
| 269 | Area parameter `A`: the code's numerator is `\|Σ_j ∇_j φ\|` (absolute value of the *sum*), not the vector magnitude `\|∇φ\|` the notation implies. No `a²` prefactor either, so the reported quantity is comoving. | `defectsmodule/defectsobservables.h::computeAreaParameter` |
| 63 | `qbias` documented as "in the same physical units as `v`", but `V_bias = qvφ³` has mass dimension 4 with `vφ³` already dimension 4, so `q` is dimensionless. The code (`g = q / lambda`, no `f_*`/`ω_*` rescaling) agrees. | `models/defects_ONGlobal.h` |
| 150 | Stability bound given as `δτ ≲ δx²/3`; the explicit bound for a 3D diffusion equation is `δx²/(2d) = δx²/6`, a factor 2 stricter. No check on `dtdiff` exists in the code, so the docs are the only guard. | — |
| 166 | `w ~ 1/√(2λ) v` renders as `(1/√(2λ))·v`, dimensionally wrong for a width. Should be `1/(√(2λ) v)`. | — |

### 2.5 `Manual/Observables.md` — several

| Line | Issue | Code reference |
|---|---|---|
| 339 | Occupation-number definition does not match the code: normalisation is `a²(δx/N)^NDim / 2` (factor `1/2`, not `1/3`); the code weights the two terms by `ω_k` and `a^{2(1-α)}/ω_k` with `ω_k = √(a²m² + k²)` — the doc formula has no `ω_k` at all; and the momentum is `π_S·a^{α-3}`, not `φ'`. | `measurements/occupationnumber.h:41-54` |
| 213 | `average_norm.txt` columns wrong: the code header is `{"t","norm(phi)","norm(phi)^2","var(norm(phi))"}` and the 4th value is `<\|φ\|>² − <\|φ\|²>`, which is neither an rms nor the usual variance. Third column also mis-bracketed (`<\|φ²\|>` should be `<\|φ\|²>`). `Defects.md:241` makes the same rms claim. | `defectsmeasurer.h` |
| 119 | SU(2) averages write the summation **outside** the norm and the volume average, but the code averages `sqrt(pi2SU2)` where `pi2SU2` already sums over spatial *and* colour index inside the square root. `<sqrt(Σ)> ≠ Σ_i <\|...\|>`. | `su2measurer.h` |
| 157 | Column list omits three columns (`rhoNMC1`, `rhoNMC2`, `rhoNMC`) that the code inserts for non-minimally coupled models. A reader of an NMC run mis-indexes every column after `E_V`. | `energiesmeasurer.h` |
| 144 | Column called "fattening factor"; the code column is `resolutionPreservingFactor`, and fattening is only the `s = 0` special case of that technique. | `scalefactormeasurer.h`; `Defects.md:212` |

### 2.6 `Manual/IC.md:421` — extra `1/√2` in the SU(2) doublet Hubble drag

The doublet momentum's Hubble-drag term carries a `1/√2` absent from the otherwise
identical singlet (line 55) and complex-scalar (line 327) equations.

The page is **faithful to the code** — `su2initializer.h:179` has
`- aDot * model.fldSU2Doublet(i)(a).inFourierSpace() / sqrt(2)` where
`u1initializer.h:221` has no such factor. So this is either an upstream code bug or a
deliberate choice the chapter never explains. The `√2` that converts a doublet component to
its two real parts is already inside `fldSU2Doublet`, so dividing again looks unintended.

### 2.7 `Manual/Appendix_Generic_Model_variables.md:28` — initial-condition variables mis-typed

The six IC rows (`fldS0`, `fldCS0`, `fldSU2Doublet0`, `piS0`, `piCS0`, `piSU2Doublet0`) are
written as **tilded program variables**, but the code stores them in physical, dimensionful
units.

`initializers/scalarsingletinitializer.h:66` states outright that "`model.fldS0(i)` and
`model.piS0(i)` are introduced in physical (dimensionful variables)" and then divides by
`fStar` / `fStar*omegaStar`. `Appendix_Parameters.md` gives their units as GeV and GeV².
So `fldS0` is `φ_*`, not `φ̃_*`.

(`pot0` on line 34 *is* correctly tilded, since it is computed from the program-variable
potential.)

---

## 3. Open physics questions

Not settleable from the code — these need an author's decision.

- **`Manual/ALP.md:148`** — the page claims that in the linear regime the energy-conservation
  check excludes the gauge sector. No such behaviour was found: `tNonLinearAxionU1` is only
  consumed in `axioncouplings.h` and `scalefactorkernels.h`, while the Hubble constraint goes
  through `Energies::rho` (`definitions/energies.h:216-228`), which unconditionally sums
  `EelU1 + EmagU1`. Either point at the code that implements the exclusion, or correct the
  bullet to say only the scale-factor kernel drops the gauge energy (so the Hubble constraint
  is *expected* to be violated during the linear stage).

- **`Manual/My first model of (singlet) scalar fields.md:541`** — an unresolved reviewer note
  is published in orange on the live page:

  > **Jorge comment: THIS IS NOT TRUE. FOR SCALAR SINGETS COSMOLATTICE SAMPES FROM A GAUSSIAN**

  It contradicts the paragraph at line 563 ("Drawing both phases and modulus amplitudes as
  above is mathematically equivalent to drawing ... as Gaussian random fields"). Either the
  left/right-moving-wave description is wrong for scalar singlets, or the comment is stale.

- **`Manual/My first model of gauge fields.md:58`** — with `D_μ` defined at line 51 as
  containing `-i g_B Q_B B^a_μ T_a`, the commutator in `G_{μν}` (and the `f^{abc}B^b_μ B^c_ν`
  term at line 66) should carry a factor `g_B Q_B` for `G_{μν}` to be the covariant field
  strength. `Canonical_Field_Theory.md:77` writes the same expression in terms of a rescaled
  field, so this may be an inherited convention rather than an error.

- **`th_framework/Brief_Review_On_Continuum_Dynamics.md:132`** — `T_{μν}` defined with `√g`
  where the signature `(-,+,+,+)` requires `√(-g)`. Line 64 of the same page writes the action
  as `∫d⁴x √(-g) L`, so the two are internally inconsistent.

- **`th_framework/Brief_Review_On_Continuum_Dynamics.md:196`** — an orphan sentence glosses
  `⟨...⟩`, but no such symbol appears in the Friedmann equations it follows. Those equations
  introduce `E_K^φ, E_G^φ, E_V, E_K^A, E_K^B, E_G^A, E_G^B`, none of which is defined anywhere
  on the page; `eq_energy-contributions` defines the *unaveraged local* densities under
  different names.

---

## 4. Notation and convention

Individually minor; collectively they make the manual hard to read across chapters.

| Where | Issue |
|---|---|
| `Conventions and Notation.md:9` vs `:7` | `α` is given two incompatible meanings two bullets apart on the authority page: a spacetime index (`α, β, μ, ν = 0..3`) and the metric exponent labelling `α`-time. `a^{2α}` is unambiguous only by context. A carve-out clause on line 7 would fix it without a site-wide rename. |
| `Conventions and Notation.md:9` vs `:11` | The `α = 0` variable is called *coordinate time* on line 9 and *cosmic time* on line 11. The rest of the site uses "cosmic time". |
| `Continuum Dynamics` vs the whole Manual | Non-Abelian gauge field is `C_μ^a, g_C, Q_C` in the theory review but `B_μ^a, g_B, Q_B` throughout the Manual. The review page is even inconsistent with itself — its Friedmann equations label the energies `E_K^B`, `E_G^B`. |
| `Observables.md:58` | Doublet components indexed 1..4, while `Introduction to CosmoLattice.md` and `My first model of gauge fields.md` use 0-based, and the output file suffix `[n]` runs 0..3. A reader cannot tell whether `average_SU2Doublet_0_0.txt` holds `φ_1` or `φ_0`. Commit `2c36c28d` looks half-finished. |
| `ALP.md:24` | The reduced Planck mass is typeset three ways on one page: `m_\text{p}`, `m_p`, `m_{\text{p}}`. The authority page uses `m_p`. |
| `ALP.md:82` | `N_\mathrm{s}`, `N_{\mathrm{U}(1)}`, `N_{\rm pot}` where the rest of the manual uses `N_s`, `N_{u1}`, `N_p`. |
| `CL.md:31` | Species index `b` summed implicitly, where the action on the same page and `GW.md` both write `Σ_b` explicitly. |
| `Continuum Dynamics:37` | `m_p ≈ 2.435e18 GeV` vs the authority page's `2.44e18 GeV`. |
| `singlet scalars:127` | Equation-of-state parameter is `w` in prose but `ω` in the equation and the `omegaEoS` table row — and `ω` collides with `ω_*`. |

### Index and typesetting slips

Small, mechanical, but they are wrong as printed:

- `Brief_Review_On_Continuum_Dynamics.md:57` — `eq_ChargedScalars` has `φ_3 + iφ_3`
  (repeated index 3); the pattern requires `φ_3 + iφ_4`. Same typo in the orphan
  `Canonical_Field_Theory.md:53`.
- `Brief_Review_On_Continuum_Dynamics.md:109` and
  `My first model of gauge fields.md:158` — SU(2) EOM friction term carries free index `b`
  where every other term has free index `a`, with nothing to contract against.
- `Introduction to CosmoLattice.md:241` — same free-index mismatch, inherited from the
  continuum page.
- `Brief_Review_On_Lattice_Techniques.md:505` — leapfrog drift reads `φ̃_a`; the scale-factor
  subscript `a` makes no sense on a field amplitude. The Velocity-Verlet copy at line 528
  correctly reads `φ̃_i`.
- `Brief_Review_On_Lattice_Techniques.md:525` — `π̃^{(b)}` is undefined; appears nowhere else.
- `singlet scalars:200, 203` — stray subscript on `φ̃'_a` / `χ̃'_a` in the friction term; the
  continuum equations at lines 93/96 correctly have no subscript.
- `singlet scalars:575` — missing tilde: LHS should be `φ̃'` since the RHS is `π̃_φ`.
- `singlet scalars:58` — the LatticeEasy definition is garbled:
  `d\tilde \equiv x^i B dx^i` puts the accent on nothing. Should be `dx̃^i ≡ B dx^i`.
- `My first model of gauge fields.md:213` — inside `eq_energy-contrib`, the first factor uses
  unrescaled `φ` while the second uses `φ̃`.
- `Manual/IC.md:522` — the transverse projector is called `Π^L`, colliding with `Π` used for
  the gauge conjugate momentum on the same page; and `k_{L,i}` is never defined (the code uses
  `sin(π n_i/N)`, a different momentum from the `k^-_L` defined at lines 486/571).
- `NMC.md:280` — self-referential definition: the symbol being defined appears on both sides.
  The value is correct; only the notation is circular.

---

## 5. Code defects surfaced by the review

Found while checking documentation claims. These are bugs in the C++/input files, not in the docs.

> **Status.** Items 2 and 4 are **fixed** on branch `fix/review-code-defects` (commits
> `8b4ff6d5`, `1f722466`). Items 1, 3, 5 and 6 remain open and are written up in detail in
> `code-defects-open.md` — item 3 was applied and then reverted at the maintainer's
> instruction, and item 5 was attempted and reverted because the fix is not mechanical.
> That document also corrects two details below: item 5 involves **three** duplicated labels
> rather than two, and item 6 is a deliberate guard that over-triggers, not an accidental
> exclusion.

- **`include/CosmoInterface/measurements/defectsmeasurer.h:49`** — *(open — see `code-defects-open.md` §1)*
  `varnorm = pow<2>(norm) - norm2;` computes `<|φ|>² − <|φ|²>`, i.e. the **negated**
  variance. `MeansMeasurer` uses the correct order (`phi2 - phi^2`). The column labelled
  `var(norm(phi))` in `average_norm.txt` and `average_norm_diffusion.txt` is sign-flipped.
  *Found independently by two reviewers and verified directly.*

- **`models/parameter-files/lphi4SU2U1.in:34-36`** — writes `CSU1Charges`,
  `SU2DoubletU1Charges`, `SU2DoubletSU2Charges`, but the parser reads `CSU1_charges`
  (`abstractmodel/complexscalarbase.h:72`), `SU2DoubletU1_charges` and
  `SU2DoubletSU2_charges` (`su2doubletbase.h:75,80`). `ParameterParser` does no name
  normalisation, so **all three lines are silently ignored** and the charges default to 1.
  Same problem in `lphi4U1.in` and `defects_U1LocalStrings.in`. *Verified directly.*

- **`models/lphi4SU2U1.h`** — *(disputed; a fix was applied and reverted on instruction — see
  `code-defects-open.md` §2)* `potDeriv2NormCS(Tag<0>)` returns `4 * qH`, where the reviewer's
  reading of the potential gives `∂²Ṽ/∂|φ̃|² = 4 q_H |Φ̃|²`. Whether the
  `pow<2>(norm(fldSU2Doublet(0_c)))` factor is genuinely missing, or these functions follow a
  convention the reviewer misread, is unresolved.

- **`models/parameter-files/defects_U1LocalStrings.in`** — label markers written as
  `// @label:model_pars` / `// @endlabel` where every other parameter file uses `#`. `//` is
  not the parser's comment character, so those lines are likely parsed as parameters at runtime.

- **`include/CosmoInterface/initializers/su2initializer.h:38,42` and `:114,129`** — the labels
  `su2initializer_initsu2` and `su2initializer_fluctuations` are each **opened twice**, so
  `label_scanner` keeps only the inner span. Two manual snippets are truncated: one to a single
  line, hiding the Gauss-law inversion and link unitarisation the prose describes; the other
  stopping before the constrained right-moving phases.

- **`include/CosmoInterface/initializers/scalefactorinitializer.h`** — throws
  `RunParametersInconsistent` whenever `!rPar.fixedBackground && Model::DefectsModel`, and
  `runparameters.h` forces `fixedBackground = false` when `expansion = false`. Net effect: a
  no-expansion defect run is rejected, contradicting `Defects.md:75` and `:327`. Looks like an
  unintended exclusion.

---

## 6. Verified correct

Recorded so nobody re-derives them.

- **`Manual/GW.md` `eq_GWvEOM`** — self-consistent with the page's own first-order system,
  reproduces the `gwskernels.h` kernel exactly, and matches
  `Brief_Review_On_Continuum_Dynamics.md` `eq_GWEOMcontinuum`. The prime on the
  `(3−α)(a'/a)v_ij'` friction term is correct. This is what established that `CL.md:29` was
  missing its prime — the one physics-adjacent edit actually applied on the branch.

- **`Manual/My first model of gauge fields.md` `eq_FriedmannDDa`** — its
  `(α−2)/α/(α−1)/(α+1)` coefficients match `scalefactorkernels.h:73-76` exactly, and match an
  independent derivation. This is the reference that convicts `ALP.md:50`.

- **`Manual/NMC.md:461`** — the Ricci scalar entering the NMC kernels was checked term by term
  against `NonMinimalCoupling::R` in `definitions/nonminimalcoupling.h`. Correct (the
  surrounding *sentence* is broken, but the equation is not).

- **`Manual/NMC.md:280`** — despite the circular notation, the value is right: the code's
  `FixedBackgroundExpansion::R` at `deltaT = 0` gives exactly `−9H̃_*²(ω_EoS − 1/3)`.

- **`Manual/Appendix_Generic_Model_variables.md`** — all 36 symbols in the table resolve to a
  real declaration in `abstractmodel.h` or its sector base classes (after two wrong names were
  corrected on the branch).

- **`Manual/Defects.md`** — all `\eqref` targets resolve to a real label and all section
  references to a real anchor, after the branch's link fixes.

- **`events/Workshops/School2026.md`** — all seven people's names and affiliations match the
  Indico record character for character, including an affiliation change that looked like an
  error but is real.

---

## Suggested order of attack

1. **§1** — three adjudicated contradictions. Highest confidence, and two of them
   (`ALP.md`, the continuum axion EOM) are equations a reader would copy into their own work.
2. **§5** — the code defects, especially the negated variance and the silently-ignored
   charge keys, since those affect *results*, not just documentation.
3. **§2** — confirmed doc↔code drift, worst first: `GW.md` swapped tags, the gauge-fields
   missing minus, the `IC.md` false claim.
4. **§3** — the open questions, which need you rather than more searching.
5. **§4** — notation, best done as one deliberate sweep rather than page by page.
