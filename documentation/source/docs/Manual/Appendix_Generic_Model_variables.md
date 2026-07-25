Below we present the variables used throughout the `CosmoInterface` which are declared in the `AbstractModel` class and thus shared by all models. See `include/CosmoInterface/abstractmodel.h`, together with the per-sector base classes it inherits from, in `include/CosmoInterface/abstractmodel/`. We group them below by sector, in the same order as those base classes.

The names are systematic, so there is less to remember than the list suggests: `fld` denotes a field and `pi` its conjugate momentum, a trailing `0` the homogeneous (zero) mode, `masses2` a mass squared, and `2Av` a volume average of a square. The sector sits in the middle of the name — `S`, `CS`, `SU2Doublet`/`SU2Dbl`, `U1`, `SU2`, `GWs` — so that `piCS` is the momentum of a complex scalar, and `SU2DblGrad2AvI` the averaged gradient-squared of an SU(2) doublet.

The trailing letters of the averages and of the scale factor record *when* the quantity lives, which matters for the staggered leapfrog evolver: `I` is an integer time step, `SI` a semi-integer (half) step, and a further `M` the previous value of that same quantity. Only the integer-step variables are listed below; each one has `SI`, `IM` and `SIM` counterparts declared alongside it.

<div class="cl-defs cl-vars" markdown>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">Scalar singlets</p>

- `fldS`{ .cl-vname } $\tilde \phi$
- `piS`{ .cl-vname } $\tilde \pi_{\phi}$
- `fldS0`{ .cl-vname } $\langle \tilde \phi_* \rangle$
- `piS0`{ .cl-vname } $\langle \tilde \pi_{\phi,*} \rangle$
- `masses2S`{ .cl-vname } $\tilde{m}_{\phi}^2$
- `pi2AvI`{ .cl-vname } $\langle \tilde \pi_{\phi}^2 \rangle$
- `grad2AvI`{ .cl-vname } $\sum_i \langle (\tilde \partial_i \tilde \phi)^2 \rangle$

</div>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">Complex scalars</p>

- `fldCS`{ .cl-vname } $\tilde \varphi$
- `piCS`{ .cl-vname } $\tilde \pi_{\varphi}$
- `fldCS0`{ .cl-vname } $\langle \tilde \varphi_* \rangle$
- `piCS0`{ .cl-vname } $\langle \tilde \pi_{\varphi,*} \rangle$
- `masses2CS`{ .cl-vname } $\tilde{m}_{\varphi}^2$
- `CSpi2AvI`{ .cl-vname } $\langle \tilde \pi_{\varphi}^2 \rangle$
- `CSgrad2AvI`{ .cl-vname } $\sum_i \langle(\widetilde D_i^A \widetilde \varphi)^*(\widetilde D_i^A \widetilde \varphi) \rangle$

</div>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">SU(2) doublets</p>

- `fldSU2Doublet`{ .cl-vname } $\widetilde\Phi$
- `piSU2Doublet`{ .cl-vname } $\widetilde \pi_{\Phi}$
- `fldSU2Doublet0`{ .cl-vname } $\langle \tilde \Phi_* \rangle$
- `piSU2Doublet0`{ .cl-vname } $\langle \tilde \pi_{\Phi,*} \rangle$
- `masses2SU2Doublet`{ .cl-vname } $\tilde{m}_{\Phi}^2$
- `SU2DblPi2AvI`{ .cl-vname } $\langle \widetilde \pi_{\Phi}^2 \rangle$
- `SU2DblGrad2AvI`{ .cl-vname } $\sum_i \langle (\widetilde D_i\widetilde \Phi)^\dagger(\widetilde D_i \widetilde \Phi) \rangle$

</div>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">U(1) gauge fields</p>

- `fldU1`{ .cl-vname } $\widetilde{A}_{i}$
- `piU1`{ .cl-vname } $\left(\tilde\pi_A\right)_i$
- `U1pi2AvI`{ .cl-vname } $\sum_i \langle \mathcal{E}_i^2 \rangle$
- `U1Mag2AvI`{ .cl-vname } $\sum_i \langle \mathcal{B}_i^2 \rangle$

</div>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">SU(2) gauge fields</p>

- `fldSU2`{ .cl-vname } $\widetilde{B}_{i}^a$
- `piSU2`{ .cl-vname } $\left(\tilde\pi_B\right)^{a}_i$
- `SU2pi2AvI`{ .cl-vname } $\sum_{i,a} \langle (\mathcal{E}_i^a)^2 \rangle$
- `SU2Mag2AvI`{ .cl-vname } $\sum_{i,a} \langle (\mathcal{B}_i^a)^2 \rangle$

</div>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">Gravitational waves<span class="cl-defs-note">only allocated when <code>withGWs = true</code></span></p>

- `fldGWs`{ .cl-vname } $\tilde v_{ij}$
- `piGWs`{ .cl-vname } $\left(\tilde\pi_v\right)_{ij}$

</div>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">Scale factor and potential</p>

- `aI`{ .cl-vname } $a$
- `aDotI`{ .cl-vname } $a'$
- `potAvI`{ .cl-vname } $\langle \widetilde V \rangle$
- `pot0`{ .cl-vname } $\langle \widetilde V_{*} \rangle$

</div>

<div class="cl-defs-group" markdown>
<p class="cl-defs-label">Program-variable normalization</p>

- `alpha`{ .cl-vname } $\alpha$
- `fStar`{ .cl-vname } $f_*$
- `omegaStar`{ .cl-vname } $\omega_*$

</div>

</div>

The gravitational-wave variables are the five independent components of a symmetric, traceless tensor, evolved as described in [**Gravitational Waves**](GW.md). Unlike the matter fields, they are held as `std::unique_ptr` and are only allocated when `withGWs = true`, so they are accessed through a dereference — `*model.fldGWs` — and are `nullptr` otherwise.
