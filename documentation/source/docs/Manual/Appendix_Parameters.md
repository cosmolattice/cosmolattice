In this appendix we list all the different parameters that can be specified when carrying out a simulation. Most of these parameters must be specified in program units, defined by the field and spacetime transformations of Eq. ([*30*][eq_FieldSpaceTimeNaturalVariables]), so that variables are dimensionless. For example, one must introduce the length side of the box as $\tilde{L} \equiv L \omega_*$, the infrared cutoff the lattice as $\tilde{k}_{\rm IR} \equiv k_{\rm IR}/\omega_*$, etc.

The tables below are generated automatically from the parameter database
`source/data/parameters.yaml` by `scripts/gen_param_appendix.py`. Only the
content between the `@gen:params` / `@endgen` HTML-comment markers is generated;
the surrounding text is authored by hand. The *Type* column annotates
vector-valued parameters with their arity (e.g. `×2` for a fixed length, or a
symbolic count such as `×NS` for the number of scalar fields); a `—` default
means the parameter is optional with no default, while *required* marks a
mandatory parameter.

## Run parameters { #subsec_runParam }

These parameters control the lattice, the time evolution and the background
expansion. They are declared in `include/CosmoInterface/runparameters.h`.

<!-- @gen:params:framework:run -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `N` | `int` | *required* | dimensionless | Number of lattice points per spatial dimension (the lattice is N^3 in 3D). |
| `kIR` | `float` | `-1` | program units | Infrared cutoff of the lattice in program units; the minimum (nonzero) momentum on the lattice. |
| `lSide` | `float` | `-1` | program units | Comoving side length of the cubic simulation box in program units. |
| `dx` | `float` | `-1` | program units | Comoving lattice spacing in program units. |
| `dt` | `float` | *required* | program units | Time step of the evolution algorithm in program units. |
| `expansion` | `bool` | `true` | dimensionless | If true, the scale factor evolves self-consistently via the Friedmann equations; if false, the scale factor is fixed to unity and dynamics occur in Minkowski spacetime. |
| `t0` | `float` | `0` | program units | Initial time of the simulation in program units. |
| `tMax` | `float` | `10000*dt` | program units | Final time of the simulation in program units. |
| `fixedBackground` | `bool` | `false` | dimensionless | If true, replaces the self-consistent expansion by a fixed background expansion specified through omegaEoS and H0. |
| `omegaEoS` | `float` | `1/3` | dimensionless | Barotropic equation-of-state parameter w = p/rho used for the fixed background expansion (e.g. 0.333 for radiation domination). |
| `H0` | `float` | *required* | GeV | Initial Hubble rate used for the fixed background expansion (in GeV). |
| `a0` | `float` | `1.0` | dimensionless | Initial value of the scale factor at the start of the simulation. |
| `evolver` | `enum` | `LF` | n/a | Type of time-evolution algorithm: LF (staggered leapfrog), velocity-Verlet (VV), position-Verlet (PV), or Runge-Kutta (RK) schemes of various orders. Allowed values: `LF`, `VV2`, `VV4`, `VV6`, `VV8`, `VV10`, `VV6_2`, `RK2`, `RK3_3`, `RK3_4`, `RK4_5`, `PV2`, `PV4`, `PV6`, `PV8`, `PV10`, `PV6_2`. |
| `tolerance` | `float` | `-1` | dimensionless | Error tolerance used by adaptive (e.g. Runge-Kutta) solvers to control the adaptive time step. |
| `doDiffusion` | `bool` | `false` | dimensionless | Master switch enabling a post-evolution diffusion phase, in which the fields are further relaxed after the main simulation has finished (used e.g. to smooth defect configurations). The diffusion-phase parameters (tmaxdiff, dtdiff, diffusionevolver, tOutputFreqDiff, tOutputRareFreqDiff, snapshots_diffusion) are only read when this is true. |
| `tmaxdiff` | `float` | — | program units | Duration (maximum time) of the diffusion phase in program units. |
| `dtdiff` | `float` | — | program units | Time step used during the diffusion phase in program units. |
| `diffusionevolver` | `enum` | `RK2` | n/a | Time-evolution algorithm used during the diffusion phase (same set of integrators as evolver). Allowed values: `LF`, `VV2`, `VV4`, `VV6`, `VV8`, `VV10`, `VV6_2`, `RK2`, `RK3_3`, `RK3_4`, `RK4_5`, `PV2`, `PV4`, `PV6`, `PV8`, `PV10`, `PV6_2`. |
| `doResolutionPreserving` | `bool` | `false` | dimensionless | Master switch enabling the resolution-preserving technique, which rescales the lattice and fields during the run to keep the relevant physical scales resolved. Its parameters (sRP, tRP0, tRPMax) are only read when this is true. |
| `sRP` | `float` | — | dimensionless | Rescaling (stretch) factor applied by the resolution-preserving technique. |
| `tRP0` | `float` | `t0` | program units | Time at which the resolution-preserving rescaling starts, in program units. |
| `tRPMax` | `float` | `(computed)` | program units | Time at which the resolution-preserving rescaling stops, in program units. |
<!-- @endgen -->

!!! note
    $\tilde{k}_{\rm IR}$ and $\tilde{L}$ obey $\tilde{k}_{\rm IR} = 2\pi /\tilde L$ , so only one of them must be specified for the simulation: the other one will be automatically computed by the code.

## Initial conditions

These parameters control the initial field fluctuations and the random seed.
They are declared in `include/CosmoInterface/runparameters.h`.

<!-- @gen:params:framework:IC -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `kCutOff` | `float` | `-1` | program units | Momentum cutoff imposed in the spectrum of initial scalar fluctuations: modes above this momentum are set to zero. A value of -1 (default) means no cutoff (internally set to 2*kUV); 0 means initially vanishing fluctuations. |
| `ICtype_S` | `enum` | `Default` | n/a | Type of initial conditions used for scalar singlet fields (e.g. default vacuum fluctuations, random-with-matter, defect networks, white-noise defects, or homogeneous). Allowed values: `Default`, `RandomWithMatter`, `DefectsNetwork`, `DefectsWhiteNoise`, `Homogeneous`. |
| `ICtype_U1` | `enum` | `Default` | n/a | Type of initial conditions used for U(1) gauge fields (e.g. default, random-with-matter, plane waves, plane waves with zero magnetic field, Bunch-Davies transverse modes, or defect networks). Allowed values: `Default`, `RandomWithMatter`, `PlaneWaves`, `PlaneWavesZeroB`, `BunchDavisTransverseU1`, `DefectsNetwork`, `DefectsWhiteNoise`. |
| `baseSeed` | `string` | *required* | n/a | Seed for the random generator of initial field fluctuations; if not specified, a seed is generated randomly each run. |
| `lcorr` | `float` | — | program units | Initial comoving correlation length of the defect network used when generating DefectsNetwork initial conditions. |
| `deltaNoise` | `float` | — | dimensionless | Amplitude of the white-noise field distribution used when generating DefectsWhiteNoise initial conditions. |
| `ext_PS` | `string` ×NS | `defaultString` | path | Per-scalar external initial power spectrum: path/name of a file providing the initial spectrum used to set the initial conditions of scalar singlet field i. |
<!-- @endgen -->

!!! note
    Typically, you will also need to add some parameters to get the initial homogeneous components of your fields in the user-defined model file, see Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md) for more details.

## Output parameters

These parameters control how, how often and in which format the simulation
output is written.

<!-- @gen:params:framework:output -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `tOutputFreq` | `float` | `10*dt` | program units | Time interval between the printing of frequent output (e.g. averages) in program units. |
| `tOutputInfreq` | `float` | `100*dt` | program units | Time interval between the printing of infrequent output (e.g. spectra) in program units. |
| `tOutputRareFreq` | `float` | `1000*dt` | program units | Time interval between the printing of very infrequent (rare) output in program units. |
| `tOutputVerb` | `float` | `100*dt` | program units | Time interval between progress updates printed to the terminal in program units. |
| `outputfile` | `string` | `./` | path | Folder where simulation output is saved; defaults to the current/compilation folder. |
| `hdf5Averages` | `bool` | `false` | dimensionless | If true, averaged output quantities are written in HDF5 format instead of text format. |
| `hdf5FlushFreq` | `int` | `10` | dimensionless | Number of writes between flushes of buffered HDF5 output to disk. |
| `hdf5Spectra` | `bool` | `false` | dimensionless | If true, field spectra are printed in HDF5 format; if false, they are printed in text format. |
| `appendToFiles` | `bool` | `false` | dimensionless | If true, output is appended to pre-existing files; if false, existing files cause an error unless overwriteFiles=true. |
| `overwriteFiles` | `bool` | `false` | dimensionless | If true (and appendToFiles=false), existing output files are deleted before writing. |
| `print_headers` | `bool` | `false` | dimensionless | If true, a header line describing the contents is printed at the top of each text output file. |
| `fn_verbosity` | `int` | `0` | dimensionless | Verbosity level controlling how much model/parameter information is embedded in output filenames (0 = none; higher levels append N, kIR, dt, evolver, etc.). |
| `tOutputFreqDiff` | `float` | `10*dtdiff` | program units | Time interval between frequent output during the diffusion phase, in program units (the diffusion-phase counterpart of tOutputFreq). |
| `tOutputRareFreqDiff` | `float` | `100*dtdiff` | program units | Time interval between rare/infrequent output during the diffusion phase, in program units (the diffusion-phase counterpart of tOutputRareFreq). |
| `measureDefectsEnergies` | `bool` | `false` | dimensionless | If true, enables measurement and output of the energy stored in the defect network. |
| `measureDefectsStructure` | `bool` | `false` | dimensionless | If true, enables measurement and output of the defect-network structure/geometry (e.g. defect positions and length/area). |
<!-- @endgen -->

## Spectra parameters

These parameters control the binning and the estimators used when computing the
field power spectra.

<!-- @gen:params:framework:spectra -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `spectraVerbosity` | `int` | `0` | dimensionless | Verbosity level controlling how much additional information is written into the spectra files. |
| `deltaKBin` | `float` | `1` | dimensionless | Width of the momentum bins used in the field spectra; the number of bins is approximately sqrt(3)*N/(2*deltaKBin). |
| `PS_type` | `int` | `1` | dimensionless | Power spectrum type selector controlling the binning/estimator used for the power spectra. |
| `PS_version` | `int` | `1` | dimensionless | Power spectrum version selector choosing among different power spectrum implementations. |
| `flagON` | `bool` | `false` | dimensionless | If true, enables the O(N) power spectrum computation/output for O(N) scalar field models. |
| `flagChiralPS` | `bool` | `false` | dimensionless | If true, enables computation/output of chiral (helicity-decomposed) power spectra. |
| `saveUnbinnedSpectra` | `bool` | `false` | dimensionless | If true, the unbinned (per-mode) power spectra are saved in addition to the binned spectra. |
<!-- @endgen -->

## Snapshot parameters

These parameters select which energy contributions are saved as field snapshots
(in HDF5 format) and over which sub-volume of the lattice.

<!-- @gen:params:framework:snapshots -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `snapshots` | `string` ×14 | `(empty list — no snapshots saved)` | n/a | Space-separated list of the configuration-space quantities to dump as HDF5 field snapshots. Each entry must be one of the 14 recognised labels; you list any subset (in any order), one label per quantity you want saved. Recognised labels: S (scalar-singlet field value), E_S_K / E_S_G (scalar-singlet kinetic / gradient energy), CS (complex-scalar modulus), E_CS_K / E_CS_G (complex-scalar kinetic / gradient energy), E_SU2D_K / E_SU2D_G (SU(2)-doublet kinetic / gradient energy), E_A_K / E_A_G (U(1) electric / magnetic energy), E_B_K / E_B_G (SU(2) electric / magnetic energy), E_V (potential energy), E (total energy). Selecting a label for a sector the model does not contain is harmless and simply produces no file. |
| `energy_snapshot` | `string` ×14 | — | n/a | Deprecated alias for `snapshots`, kept for backward compatibility. If `snapshots` is not given but `energy_snapshot` is, its value fills `snapshots` and a deprecation notice is printed. If both are given, `snapshots` takes precedence and a conflict warning is printed. |
| `snap_lowercoord` | `int` ×NDim | `{0,...} (one 0 per dimension)` | dimensionless | Lower corner (in lattice units) of the sub-volume saved in snapshots, one value per spatial dimension. Together with snap_uppercoord and snap_stepcoord it selects which lattice region is written: snap_lowercoord is the first lattice index kept along each axis. Default 0 in every dimension (start at the box edge). |
| `snap_uppercoord` | `int` ×NDim | `{N,...} (one N per dimension)` | dimensionless | Upper corner (in lattice units) of the sub-volume saved in snapshots, one value per spatial dimension. snap_uppercoord is the (exclusive) last lattice index kept along each axis. Default N (the lattice size) in every dimension, i.e. the full box. |
| `snap_stepcoord` | `int` ×NDim | `{1,...} (one 1 per dimension)` | dimensionless | Stride (in lattice units) used when sampling the snapshot sub-volume, one value per spatial dimension. A value of s keeps every s-th lattice point along that axis (coarsening the saved data); 1 means keep every point. |
| `snapshots_diffusion` | `string` ×15 | `(empty list — no snapshots saved)` | n/a | Space-separated list of the configuration-space quantities to dump as HDF5 field snapshots during the diffusion phase (the diffusion-phase counterpart of snapshots). Same recognised labels as snapshots; you list any subset (in any order), one label per quantity you want saved. |
<!-- @endgen -->

## Gravitational waves

These parameters control the evolution of the tensor perturbations used to
compute the gravitational-wave spectrum.

<!-- @gen:params:framework:GW -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `withGWs` | `bool` | `false` | dimensionless | If true, the simulation evolves the tensor perturbations to compute the gravitational wave spectrum (allocates the GW field tensor and its conjugate momentum, fldGWs and piGWs). |
| `doLFforGWs` | `bool` | `true` | dimensionless | If true, the gravitational wave (tensor) sector is evolved with the leapfrog (LF) evolver; if false, it uses the same evolver as the fields. |
| `GWprojectorType` | `int` | `2` | dimensionless | Type of transverse-traceless projector used for the gravitational waves (1 = real, 2 = backwards/default, 3 = forward). |
<!-- @endgen -->

## Saving and restarting

These parameters control end-of-run saving, periodic backups and restarting from
a previously saved simulation.

<!-- @gen:params:framework:saving -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `tBackupFreq` | `float` | `-1` | program units | Time interval between backups of the simulation state in program units; a negative value disables backups. |
| `save_dir` | `string` | `defaultString` | path | Folder where a copy of the simulation is saved at the end of the run, which can be loaded as the starting point of a new simulation. |
| `backup_dir` | `string` | `defaultString` | path | Folder where a copy of the simulation is saved at backup times during the run. |
| `load_file` | `string` | `defaultString` | path | Full path/name of an HDF5 file from which to restart (load) a previously saved simulation; if provided the simulation runs in restart mode and the parameter parser is switched to override mode so new parameters can supersede the saved ones. |
| `load_dir` | `string` | `defaultString` | path | Backward-compatibility alias for 'load_file' (named to match a typo in the first manual); supplies the restart file name when 'load_file' is not given. The full file name is still required. |
<!-- @endgen -->

## Gauge couplings and charges

These framework-level coupling parameters set the gauge couplings and charges of
the gauge sectors, as well as a few related per-field couplings. They are
declared in the constructor of `include/CosmoInterface/abstractmodel.h` and
in the corresponding gauge-field interface headers.

<!-- @gen:params:framework:couplings -->
| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `gU1s` | `float` ×#U1 | `1.0` | dimensionless | Gauge couplings of the U(1) gauge fields, one per U(1) field; combined with per-couple charges to set the effective charges of complex scalars and SU(2) doublets coupled to U(1). |
| `CSU1_charges` | `float` ×#couples | `1` | dimensionless | Charges of each complex-scalar / U(1)-gauge-field coupled pair; combined with gU1s to form the effective charges of the complex scalars under their U(1) gauge fields. |
| `SU2DoubletU1_charges` | `float` ×#couples | `1` | dimensionless | Charges of each SU(2)-doublet / U(1)-gauge-field coupled pair; combined with gU1s to form the effective charges of the SU(2) doublets under their U(1) gauge fields. |
| `gSU2s` | `float` ×#SU2 | `1.0` | dimensionless | Gauge couplings of the SU(2) gauge fields, one per SU(2) field; combined with per-couple charges to set the effective charges of SU(2) doublets coupled to SU(2). |
| `SU2DoubletSU2_charges` | `float` ×#couples | `1` | dimensionless | Charges of each SU(2)-doublet / SU(2)-gauge-field coupled pair; combined with gSU2s to form the effective charges of the SU(2) doublets under their SU(2) gauge fields. |
| `tNonLinearAxionU1` | `float` | `-1` | program units | Program time at which to switch from linear to non-linear evolution for the axion-U(1) (Chern-Simons) coupling; a default of -1 disables the switch (coupling treated normally / non-linear feature off). |
| `gAxionU1` | `float` ×#couples | `1.0` | dimensionless | Gauge coupling(s) for the axion / U(1) gauge field (Chern-Simons) interaction, one per axion-U(1) gauge entry; combined with alphaLambda_AxionU1 to set the effective axion-U(1) couplings. |
| `alphaLambda_AxionU1` | `float` ×#couples | `1` | dimensionless | Dimensionless axion-U(1) Chern-Simons coupling strength (alpha/Lambda) for each axion / U(1)-gauge-field coupled pair; combined with gAxionU1 to set the effective axion-U(1) charges. |
| `xis` | `float` ×#NMC | `1` | dimensionless | Non-minimal gravitational coupling constants xi for each non-minimally-coupled scalar field, entering the xi*R*phi^2 term in the action. |
<!-- @endgen -->

## Model-specific parameters

In addition to the framework parameters above, each shipped model declares its
own parameters (couplings, masses, and the initial homogeneous amplitudes and
momenta of its fields). The tables below list these per model, using the model
name as it is passed to the compilation. Vector-valued initial conditions are
annotated with their arity (e.g. `×NS` for the number of scalar fields).

<!-- @gen:params:models -->
### `lphi4`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `lambda` | `float` | *required* | dimensionless | Quartic self-coupling of the inflaton field, appearing in the inflaton potential term 0.25*lambda*phi^4; together with q it sets the resonance coupling g=sqrt(q*lambda) and the field/time rescalings (omegaStar=sqrt(lambda)*fStar). |
| `q` | `float` | *required* | dimensionless | Resonance parameter controlling the strength of the interaction term 0.5*q*(phi*chi)^2 between the inflaton and the daughter field; the derived coupling g is defined as sqrt(q*lambda). |
| `initial_amplitudes` | `float` ×2 | *required* | GeV | Initial homogeneous amplitudes of the two scalar fields (inflaton and daughter field), given in GeV; the first component (inflaton) also sets the field rescaling fStar=fldS0[0]. |
| `initial_momenta` | `float` ×2 | `{0,0}` | GeV^2 | Initial homogeneous time-derivatives (canonical momenta) of the two scalar fields, given in GeV^2; optional parameter defaulting to zero velocity for both fields. |

### `defects_ONGlobal`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `lambda` | `float` | `1.` | dimensionless | Quartic self-coupling of the real O(N) scalar multiplet whose spontaneous symmetry breaking produces the global defects (domain walls for N=1, global strings for N=2, monopoles/textures for higher N); it sets the velocity rescaling omegaStar=sqrt(lambda)*vev and enters the derived bias coupling g=qbias/lambda. |
| `vev` | `float` | `1.` | GeV | Vacuum expectation value of the O(N) scalar multiplet, i.e. the radius of the vacuum manifold on which the defect network forms; it sets the field rescaling fStar=vev and the velocity rescaling omegaStar=sqrt(lambda)*vev. |
| `qbias` | `float` | `0.` | dimensionless | Bias parameter adding a term that breaks the vacuum degeneracy, used to bias domain-wall networks; it sets the derived bias coupling g=qbias/lambda. A value of 0 gives an unbiased network. |

### `defects_U1LocalStrings`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `lambda` | `float` | `1.` | dimensionless | Quartic self-coupling of the complex scalar field, controlling the strength of the symmetry-breaking potential whose vacuum manifold supports the formation of local cosmic strings; together with the vacuum expectation value vev it sets the velocity rescaling omegaStar = sqrt(lambda)*vev. |
| `vev` | `float` | `1.` | GeV | Vacuum expectation value of the complex scalar field, i.e. the radius of the vacuum manifold on which the local cosmic strings form; it sets the field rescaling fStar=vev and the velocity rescaling omegaStar=sqrt(lambda)*vev. |

### `NMC_lphi4`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `lambda` | `float` | *required* | dimensionless | Quartic self-coupling of the inflaton (first scalar) in the lphi4 potential 0.25*lambda*phi^4; sets the characteristic frequency omegaStar=sqrt(lambda)*fStar and, together with q, the derived coupling g=sqrt(q*lambda). |
| `initial_amplitudes` | `float` ×2 | *required* | program units | Initial homogeneous amplitudes of the two scalar fields (in program/reduced-Planck units); the first entry fixes fStar=initial_amplitudes[0] used to define omegaStar=sqrt(lambda)*fStar. |
| `initial_momenta` | `float` ×2 | `{0,0}` | program units | Initial homogeneous time-derivatives (conjugate momenta) of the two scalar fields; defaults to zero velocity for both fields if not specified. |

### `lphi4SU2U1`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `initial_amplitudes` | `float` ×NS | *required* | GeV | Initial homogeneous amplitude of the real scalar singlet field (fldS0). Read as get<double> for the single scalar singlet (NScalars=1). |
| `initial_momenta` | `float` ×NS | *required* | GeV^2 | Initial homogeneous time-derivative (conjugate momentum) of the real scalar singlet field (piS0). Read as get<double> for the single scalar singlet (NScalars=1). |
| `cmplx_field_initial_norm` | `float` | *required* | GeV | Initial homogeneous norm of the complex scalar field; the norm is distributed equally between the two real components via Complexify(norm/sqrt(2), norm/sqrt(2)) to set fldCS0(0_c). |
| `cmplx_momentum_initial_norm` | `float` | *required* | GeV^2 | Initial homogeneous norm of the complex scalar field momentum (time derivative); distributed equally between the two real components via Complexify(norm/sqrt(2), norm/sqrt(2)) to set piCS0(0_c). |
| `SU2Doublet_initial_norm` | `float` | *required* | GeV | Initial homogeneous norm of the SU(2) doublet field (in GeV); distributed equally between the four real components via MakeSU2Doublet(a, norm/2) to set fldSU2Doublet0(0_c). Also sets the field rescaling fStar and (with lambda) the frequency rescaling omegaStar = sqrt(lambda)*norm. |
| `SU2Doublet_initial_momenta_norm` | `float` | *required* | GeV^2 | Initial homogeneous norm of the SU(2) doublet field momentum (time derivative, in GeV^2); distributed equally between the four real components via MakeSU2Doublet(a, norm/2) to set piSU2Doublet0(0_c). |
| `qG` | `float` | *required* | dimensionless | Dimensionless coupling ratio controlling the interaction between the SU(2) doublet and the real scalar singlet; enters the potential term qG*(\|H\|*S)^2 and fixes the effective coupling g = sqrt(qG*lambda) (derived). |
| `qH` | `float` | *required* | dimensionless | Dimensionless coupling ratio controlling the interaction between the SU(2) doublet and the complex scalar; enters the potential term 2*qH*(\|H\|*\|phi\|)^2 and fixes the effective coupling h = sqrt(qH*lambda) (derived). |
| `lambda` | `float` | *required* | dimensionless | Quartic self-coupling of the SU(2) doublet (potential term \|H\|^4); also sets the frequency rescaling omegaStar = sqrt(lambda)*SU2Doublet_initial_norm and the derived couplings g = sqrt(qG*lambda), h = sqrt(qH*lambda). |

### `lphi4U1`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `cmplx_field_initial_norm` | `float` | *required* | GeV | Initial homogeneous amplitude (norm) of the complex scalar field; it is distributed equally between the two real components via Complexify and also sets the field rescaling fStar used to define program units. |
| `cmplx_momentum_initial_norm` | `float` | *required* | GeV^2 | Initial homogeneous amplitude (norm) of the complex scalar field's conjugate momentum (time derivative), distributed equally between the two real components via Complexify. |
| `lambda` | `float` | *required* | dimensionless | Quartic self-coupling of the complex scalar in the V = lambda*\|phi\|^4 potential; it sets the velocity rescaling omegaStar = sqrt(lambda)*cmplx_field_initial_norm and, together with the gauge charge, the derived gauge coupling g = sqrt(q*lambda). |

### `axionU1_m2phi2`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `initial_amplitudes` | `float` | *required* | GeV | Initial homogeneous amplitude of the single real (axion) scalar field; its first component sets the field rescaling fStar used to convert to program units. |
| `initial_momenta` | `float` | *required* | GeV^2 | Initial homogeneous velocity (conjugate momentum) of the single real (axion) scalar field. |
| `Mass` | `float` | *required* | GeV | Mass of the scalar field in the quadratic potential V = (1/2) m^2 phi^2; sets the velocity rescaling omegaStar = mass used to convert to program units. |

### `NMC_tanh4_w_mass_P`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `Hstar` | `float` | *required* | GeV | Hubble rate 14 efolds before the end of inflation (begin of linear analysis), in GeV; also used as the velocity rescaling omegaStar to go to program units. |
| `V0` | `float` | *required* | dimensionless | Dimensionless amplitude of the inflaton tanh^4 potential, equal to Lambda4/(4 * mp^2 * Hstar^2). |
| `M` | `float` | *required* | dimensionless | Characteristic field scale M of the inflaton tanh(phi/M)^4 potential, expressed in units of the reduced Planck mass mp. |
| `lambdaNMC` | `float` | *required* | dimensionless | Quartic self-coupling of the non-minimally coupled (NMC) second scalar field. |
| `mNMC` | `float` | *required* | dimensionless | Mass of the non-minimally coupled (NMC) second scalar field, in units of Hstar (it enters the program potential as 0.5*mNMC^2*phi^2). |
| `initial_amplitudes` | `float` ×2 | *required* | GeV | Initial homogeneous field amplitudes for the two scalar fields (inflaton and NMC field), in GeV. |
| `initial_momenta` | `float` ×2 | `{0, 0}` | GeV^2 | Initial homogeneous field velocities (conjugate momenta) for the two scalar fields, in GeV^2; optional, defaults to zero for both fields. |

### `tanh2`

| Parameter | Type | Default | Units | Description |
| --- | --- | --- | --- | --- |
| `M` | `float` | *required* | GeV | Mass scale appearing in the tanh^2 inflaton potential V = (M^2/2) tanh^2(phi/M); together with Lambda4 it sets the inflaton oscillation frequency omega = sqrt(Lambda4/M^2). Default in example input is 2.435e19 GeV (the reduced Planck mass). |
| `Lambda4` | `float` | *required* | GeV^2 | Energy-scale-squared parameter of the tanh^2 inflaton potential; combined with M it fixes the inflaton oscillation frequency omega = sqrt(Lambda4/M^2), which is used as the velocity rescaling omegaStar. Example value 1.7966e65 GeV^2. |
| `q` | `float` | *required* | dimensionless | Resonance parameter setting the strength of the quartic interaction (q/2) phi^2 chi^2 between the inflaton and the daughter field; determines the effective coupling g = sqrt(q)*omega/phii. Example value 4e4. |
| `initial_amplitudes` | `float` ×2 | *required* | GeV | Initial homogeneous amplitudes of the two scalar fields (inflaton, daughter) in GeV; the first entry phii = fldS0[0] sets the field rescaling fStar. Example: 3.39928e18 0. |
| `initial_momenta` | `float` ×2 | `{0,0}` | GeV^2 | Initial homogeneous velocities (conjugate momenta) of the two scalar fields in GeV^2; defaults to zero for both fields if not specified. Example: -3.0714e31 0. |
<!-- @endgen -->
