In this section, our aim is to provide the reader with a better understanding of our physics interface, such as how the fields are actually initialized, how they are evolved, or how the measurements are actually done. In order to do this, we guide the reader through the `main` function of CosmoLattice, highlighting the important classes/routines responsible for some specific tasks. We will then explain some of these classes in detail, namely the ones responsible for initializing, evolving and measuring the fields. These are the ones that a user will most likely want to modify to suit their specific purposes.

The `main` function is located in the file `source/cosmolattice.cpp`, which we present now:

@emgithub(source/cosmolattice.cpp:main_includes)

For completeness, we explain first the top lines in the file, which take care of the model selection. The variable `MODELINCLUDE` is passed at compilation through `CMake` and contains the path to the model file. It is transformed into a string by the macro `STRINGIFY`, declared in the file `"TempLat/util/stringify.h"`. Next, we declare for convenience that we are using the namespace `TempLat`, to which all the CosmoLattice functions belong. Lastly, we redeclare the custom model passed through CMake, `MODELTYPE`, as a type `ModelType`, and its floating-point type as `FloatType` (so that the same code can be compiled to run in single or double precision). After this, we are ready to enter the main function, which is executed when the software runs:

@emgithub(source/cosmolattice.cpp:main_sessionguard)

We instantiate a `SessionGuard` object, which is in charge of allocating and deallocating internal memory space needed by the library (MPI and FFT resources in particular). The file continues with:

@emgithub(source/cosmolattice.cpp:main_paramparser)

Here we create a `ParameterParser` called `parser`. As we saw in the previous sections, this object is used throughout the library to declare and read parameters, which are parsed from the input file, the command line, or from a previous (restarted) simulation. The file continues with:

@emgithub(source/cosmolattice.cpp:main_simmanager)

Here we create an object `SimulationManager` called `manager`. This object deals with printing the simulation-related information file, and with the backing up and restarting of simulations. If a restart is requested, the overridable parameters are read back from the previous run at this point. The file continues with:

@emgithub(source/cosmolattice.cpp:main_runparams)

We collect all the relevant simulation parameters in a `RunParameters` object called `runParams`. This object holds the information on the model-independent parameters, such as the number of points on the lattice, the frequency at which measurements are performed, etc. It also receives from `manager` the restart flag (via `setDoWeRestart`), so that the rest of the program knows whether we are starting fresh or restarting. The file continues with:

@emgithub(source/cosmolattice.cpp:main_toolbox)

Here we define quantities and objects useful to run the simulation. The first of these is the number of ghost cells (see Section [*Concepts*][subsec_para] of [Parallelization](Parallelization.md) for the technical details on this). CosmoLattice currently uses a spatial discretization that requires only one layer of the so-called "ghost cells" for parallelization purposes. However, if you were to implement a spatial lattice derivative that requires calling lattice sites beyond the nearest-neighbor positions, you would need to change this number, e.g. to `nGhost = 2`, and recompile — it is a hard-coded variable, not an input parameter.

Following this, we create a `MemoryToolBox` object (or, more precisely, a shared pointer to one). It holds many internal functions related to memory, in particular the objects that allow iterating over the lattice, and it is needed to create fields and several other objects, so it is passed around to many functions. We also retrieve whether the current process is the root process, which is used so that console output and the information file (see below) are written only once, by a single process, when running in parallel. The file continues with:

@emgithub(source/cosmolattice.cpp:main_model)

Here we create our model as defined in the model header file we specified, for instance in `models/lphi4.h` of Section [*Definition and declaration of the model*][subsubsec_DefAndDeclModel]. We also allocate an `ExtraFields` object: a small collection of temporary field buffers that some evolvers need internally (for instance, the generic Runge-Kutta evolver described below uses it to store its stage increments), and that we allocate once here so it can be shared between the initializer and the evolver. Finally, we print the model's name, so we can make sure that the model running is actually the one we wanted. We are then ready to initialize the fields in the model, as described in Sections [*Initialization of fluctuations*][sec_InitScalar] and [*Initial conditions*][subsubsec_initialConditionsNonAb]:

@emgithub(source/cosmolattice.cpp:main_initializer)

If we are starting a new simulation, the initialization is taken care of by an object of type `ModelInitializer<FloatType>`, called `initializer`, which we describe in detail in Section [*Initializers*][subsec_Initializers]. We also set the initial time from the run parameters. Otherwise, if we are restarting a previous simulation, the file continues with:

@emgithub(source/cosmolattice.cpp:main_restart)

Here the model (fields, momenta, scale factor, etc.) is reloaded from a file created by a previous run, and the manager takes care of finding the appropriate time at which the simulation left off. In either case, we communicate the initial time `t0` to the model, since some models need it internally (e.g. for a time-dependent coupling). From the point of view of the physics, we have two more classes to instantiate:

@emgithub(source/cosmolattice.cpp:main_evolver_measurer)

We first create an object called `evolver` of type `Evolver<ModelType>`. This class, further explained in Section [*Evolvers*][subsec_Evolvers], is in charge of dispatching to the appropriate evolution algorithm, chosen at run time through the input parameter file. We also create a `Measurer<ModelType, FloatType>` called `measurer`, in charge of performing and outputting all the standard measurements. This class is described in Section [*Measurers*][subsec_Measurers]. Before starting the actual simulation, the file continues with:

@emgithub(source/cosmolattice.cpp:main_infofile)

Here we print the parameters the program is running with, and we ask `manager` to create an *information file*, which records these parameters (so their values can be checked after the simulation has concluded), together with the time at which the simulation started and the type of parallelization used. We are now ready to proceed to the time evolution of the system:

@emgithub(source/cosmolattice.cpp:main_timeloop)

At every time step, we first check whether we are due to perform any measurements. If so, we ask the evolver to `sync` itself: if the chosen algorithm keeps fields and momenta at different times internally (as staggered leapfrog does), this brings everything to a common, integer time step before measuring; for algorithms that already keep fields and momenta synchronized (Velocity/Position Verlet, Runge-Kutta), it does essentially nothing. After such a synchronized measurement, the momenta are automatically advanced by the missing half step, so that a staggered algorithm resumes its regular cadence (see the `weight` discussion in the LeapFrog section below). We then let `evolver` advance the system by one time step `dt`, which updates the scale factor, the field content, and all their associated derivatives. Finally, we check whether we are required to back up the simulation at the current time step, in which case `manager` takes care of it. Once the time evolution loop is done, we finalize the simulation:

@emgithub(source/cosmolattice.cpp:main_savesim)

If requested, the simulation state is saved to a timestamped file so that it can later be used to restart the run. Finally, we close the information file, recording the time at which the simulation ended and the total time it ran for.

### **Evolvers** { #subsec_Evolvers }

In this section we present the main routines responsible for the evolution of the fields, showing explicitly how different evolution algorithms are implemented. The relevant classes are located in `include/CosmoInterface/evolvers/`. As presented above, the class instantiated in `main` is `Evolver<Model>`. Its sole purpose is to choose, at run time, between the different algorithms implemented in their own dedicated classes: `LeapFrog`, `VelocityVerlet`, `PositionVerlet`, and `RK2NStorage`. The concrete algorithm names a user can select in the input file (the `EvolverType` enum, declared in `include/CosmoInterface/evolvers/evolvertype.h`) are `LF` (staggered leapfrog), `VV2`/`VV4`/`VV6`/`VV8`/`VV10` and the alternative composition `VV6_2` (Velocity Verlet, at increasing order), `PV2`/`PV4`/`PV6`/`PV8`/`PV10` and `PV6_2` (the Position Verlet counterparts), and the low-storage Runge-Kutta family `RK2`, `RK3_3`, `RK3_4`, `RK4_5`.

@emgithub(include/CosmoInterface/evolvers/evolver.h:evolver_members)

The class holds one shared pointer per algorithm family, only one (or, for gravitational waves, possibly two) of which is actually constructed for a given run:

@emgithub(include/CosmoInterface/evolvers/evolver.h:evolver_constructor)

Which pointer gets constructed depends on the requested `EvolverType`, and the corresponding "is this evolver type in this family" check is delegated to each algorithm's own parameters class (e.g. `VelocityVerletParameters<T>::isVerlet(type)`). Gravitational waves (GWs), when present, are tracked through a *second*, independent `EvolverType`, `typeGW`, since it is possible (and sometimes required) to evolve the GW tensor with a different algorithm than the matter/gauge sector: for instance, `LeapFrog` is always available to evolve the GWs even when the matter fields are evolved with a higher-order Verlet or Runge-Kutta scheme. The first `throw` statement simply rejects an unknown evolver type; the two `InvalidEvolverTypeGW` throws guard against algorithm combinations that would silently break the synchronization between the U(1) electric/magnetic fields and the GW source term.

The dispatch itself happens in the `evolve` and `sync` member functions:

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_dispatch)

@emgithub(include/CosmoInterface/evolvers/evolver.h:gws_evolver_sync)

Note the `template<class Model>` argument of these functions: this is the mechanism used throughout CosmoLattice so that a single `Evolver` (or, as we will see, any other class) can operate on an arbitrary user model. The `sync` function is the one used to synchronize all the fields (when needed) to a common, integer time, before a measurement; it is used, for instance, by staggered leapfrog.

To summarize, `Evolver<Model>` allows choosing among evolvers at run time. If you implement your own evolution algorithm, you can add it here to make it selectable from the input file.

#### LeapFrog

The actual characterization of each evolution algorithm lives in its own file: `leapfrog.h`, `velocityverlet.h`, `positionverlet.h`, and `rk2nstorage.h`. We present the leapfrog evolver in detail, as a worked example.

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_constructor)

The constructor records whether the scale factor is being evolved at all (`expansion`), whether the momenta currently live at an integer time step (`synced`, true at the start of a fresh simulation), and whether the background expansion is fixed rather than self-consistent (`fixedBackground`), together with a `FixedBackgroundExpansion<T>` helper used in the latter case. The structure of the algorithm itself is laid out in the `evolve` function:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_evolve)

As explained in detail in $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl], the leapfrog algorithm consists of two parts. First, we evolve the conjugate momenta by computing "kicks", then we evolve the field variables by means of "drifts", using the freshly updated momenta. This is reflected directly in the code: the kick functions are called first (the scale factor's momentum is only kicked if the background is self-consistent), then the scale factor is advanced, and finally the drift functions are called. We note a small subtlety: after a synchronization (performed for a measurement), the momenta only need to be advanced by half a time step to resume the regular leapfrog cadence — this is what the `weight` variable (`0.5` vs `1.0`) controls. The file continues with:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_sync)

The `sync` function is responsible for evolving the momenta so that they live at an integer time, ready for a measurement (e.g. an energy output); the `weight = 0.5` case just discussed then takes over at the next `evolve` call. Next, all the kicks are constructed in a similar way:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_kicks)

The file calls the appropriate "kick" functions, defined in the `kernels` folder discussed in the next section. Note the use of `ForLoop` to iterate over the fields of a given species: this is necessary because the iteration must happen at compile time, so that the correct kernel overload can be resolved for each field index.

Next, the drifts are implemented in a similar way:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_drifts)

Here we take the opportunity to illustrate some of the versatility of CosmoLattice's field syntax. Note that `driftScalar` does *not* iterate over the collection of fields with a `ForLoop`: instead, we operate directly at the level of the whole field collection, and the appropriate loop is expanded internally. Of course, one could write this with an explicit `ForLoop`, as is done for the kernels, and the result would be identical; both styles are supported and it is a matter of convenience which one to use. Note also that, for the scale factor, we allow for a fixed background expansion, in which case the scale factor is simply given by a prescribed function of time (`FixedBackgroundExpansion`), and for the non-Abelian gauge links, the update is a group exponential map rather than a simple additive update, consistent with the links living on the gauge group rather than in a vector space.

The last relevant methods store the volume averages needed for the scale-factor evolution (in the self-consistent case):

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_averages)

Here we call the `Averages` routines, where the appropriate averages over fields squared are computed (see next section), together with the volume average of the potential, obtained directly from `Potential::potential(model)`.

#### VelocityVerlet, PositionVerlet, and RK2NStorage

The structure of `VelocityVerlet` differs from leapfrog's in two ways: fields and momenta always live at the *same* (integer) time step, and the higher-order members of the family (`VV4`, `VV6`, `VV8`, `VV10`) are built as a symmetric (Yoshida/Suzuki) composition of ordinary kick-drift-kick sub-steps, using a small set of numerical coefficients:

@emgithub(include/CosmoInterface/evolvers/velocityverlet.h:velocityverlet_constructor)

@emgithub(include/CosmoInterface/evolvers/velocityverlet.h:velocityverlet_composition)

The `ws` vector, obtained from `VelocityVerletParameters<T>::getCoeffs(rPar.eType)`, holds one coefficient for `VV2` (which then reduces to an ordinary kick-drift-kick step), and 2, 4, or 8 coefficients respectively for `VV4`, `VV6`, `VV8` (with `VV10` needing 16). The `evolve` loop reuses this list palindromically to build a longer, symmetric composition of `VV2`-like sub-steps whose leading error terms cancel, yielding higher formal accuracy at the cost of more kernel evaluations per time step. Because every sub-stage kicks *before and after* its drift, `sync` is essentially a no-op for `VelocityVerlet` (it only ever needs to fix the scale factor's time derivative for a fixed background run).

`PositionVerlet` (`include/CosmoInterface/evolvers/positionverlet.h`) is built with the exact same Yoshida composition machinery, but mirrors the substep order: drift, kick, drift, instead of kick, drift, kick. We refer the interested reader directly to the code, whose structure closely parallels `VelocityVerlet`'s.

`RK2NStorage` (`include/CosmoInterface/evolvers/rk2nstorage.h`) is structurally different from the Verlet family: rather than a hand-written kick/drift pair per field species, it is a generic, low-storage ("2N-storage") explicit Runge-Kutta integrator, driven by three coefficient vectors specific to each member of the family (`RK2`, `RK3_3`, `RK3_4`, `RK4_5`). It iterates over *every* field type present in the model through the field-agnostic `ForEachField` macro:

@emgithub(include/CosmoInterface/evolvers/rk2nstorage.h:rk2nstorage_genericloop)

Instead of a dedicated `kickScalar`/`driftScalar`-style function per species, a single `delta`/`advance` pair (dispatched through the tag-based `Kernels::get` function discussed below) is applied uniformly to whichever fields are active, storing each stage's increment in the `ExtraFields` collection allocated in `main`. The one exception to this uniformity are the non-Abelian gauge links, which get a dedicated `advance` overload implementing the group exponential map, as in the Verlet family. This is also the mechanism used by CosmoLattice's `activate`/`deactivate` functionality, which lets a model turn a given field species on or off mid-simulation (currently supported only by the Runge-Kutta family — the other evolvers throw an error if it is requested).

### **Kernels and other physics formulae** { #subsec_Kernels }

CosmoLattice has been designed to be as compartmentalized as possible, so that a change in the code has only "local" consequences and does not propagate throughout the whole code. This is particularly true of the field-update kernels used by the evolvers, and of the physics building blocks they are built from. All physical formulae potentially needed by more than one routine are extracted into their own dedicated class. We have already encountered an example of this in the *kernels* used by the evolution algorithms above; the same is true of the volume averages, used both in the evolution routines and in the measurements.

Let us first look at one of the field-update kernels, to gain a better understanding of the code structure. As an example we look at the complex-scalar kernel:

@emgithub(include/CosmoInterface/evolvers/kernels/complexscalarkernels.h:complexscalarkernels_class)

As we can see, `ComplexScalarKernels` is extremely simple, and its sole purpose is to contain the complex-scalar field's equation-of-motion kernel, formed from the covariant Laplacian and the potential-derivative term (see [Scalar-Gauge Interactions](My first model of gauge fields.md#eq_kernelcomplexscalar)), plus the corresponding kernel for the field's time derivative in terms of its canonical momentum. It relies on two `definitions` classes, `GaugeDerivatives` and `Potential`, discussed below: the first stores the generic gauge-covariant-derivative expressions, while the second computes the potential derivative with respect to the field components (as well as the potential itself, from the different potential terms defined in the model).

Every other field species (scalar singlets, SU(2) doublets, U(1)/SU(2) gauge fields, and the gravitational-wave tensor) has its own kernel class in `include/CosmoInterface/evolvers/kernels/`, following the same pattern: `scalarsingletkernels.h`, `su2doubletkernels.h`, `u1kernels.h`, `su2kernels.h`, `scalefactorkernels.h` (the source term for the scale factor's own equation of motion), and `gwskernels.h` (the source term for the GW tensor, shown below). These kernels are what `LeapFrog`, `VelocityVerlet`, and `PositionVerlet` call directly in their kick/drift functions.

The generic Runge-Kutta evolver, `RK2NStorage`, cannot call these kernels by name (it does not know, at compile time, which field species it is currently updating), so it goes through an extra tag-dispatch layer instead: `kernelstypes.h` defines the small `KernelsTypes::EoM<Model>` carrier object (used to cache the current time, so that time-dependent kernels, e.g. axion couplings switched on after a given time, can access it without changing every kernel's signature), and `kernels.h` defines overloads of `Kernels::get(FieldsNumbering::..., model, n, kt)`, one per field/momentum pair, each simply forwarding to the corresponding kernel class. This is the layer that lets `RK2NStorage`'s `ForEachField` loop, shown above, resolve the right kernel for whichever field it is currently looping over.

Let us now turn to the `definitions` folder, which holds the underlying physics formulae the kernels are built from. We start with `GaugeDerivatives`, which computes spatial and gauge-covariant derivatives:

@emgithub(include/CosmoInterface/definitions/gaugederivatives.h:gaugederivatives_laplacians)

This class has no internal state; it simply regroups, under one roof, the functions needed to compute the Laplacian and covariant Laplacian of the matter/gauge content of a user's model (shown here for a real scalar singlet, the GW tensor, a complex scalar, and an SU(2) doublet). They rely on helper functions that compute the appropriate combination of link variables, depending on which matter fields couple to which gauge fields:

@emgithub(include/CosmoInterface/definitions/gaugederivatives.h:u1sforcscovderivs)

This is a good place to highlight one of the advantages of CosmoLattice's expression-template-based "functional" interface (see [Expression Templates](Expression Templates.md)). Calling e.g. `U1sForCSCovDerivs` does not lead to any computation on its own: it builds an abstract expression, to be evaluated later. `fold_multiply` takes a vector-like object built by `MakeArray(a, 0, Model::NU1-1, ...)`, iterating over every U(1) gauge field `a`, and multiplies all its elements together. For each `a`, `IfElse(Model::CsU1Couplings::couples(...), complexPhase(...), OneType())` selects either the actual U(1) link phase, if the matter field is declared to couple to that gauge field, or the placeholder `OneType()` otherwise — a compile-time representation of the multiplicative identity, which the compiler can discard algebraically. This means an uncoupled gauge field costs nothing at run time.

All the other functions in `definitions` follow a similar spirit. Rather than reviewing every one in detail, we summarize below what each is used for:

- `include/CosmoInterface/definitions/averages.h`: computes the volume-averaged quadratic field functionals (momentum-squared, gradient-squared, magnetic-field-squared, and, for non-minimally-coupled models, the extra averages the non-minimal coupling requires) that feed the scale-factor evolution and the energy measurements.
- `include/CosmoInterface/definitions/energies.h`: converts those raw averages (or the per-lattice-point `FieldFunctionals`, below) into physical energy densities, applying the correct powers of the scale factor and coupling normalizations; used by the Hubble constraint, the non-minimal-coupling Ricci scalar, and the energy measurer.
- `include/CosmoInterface/definitions/fieldfunctionals.h`: defines the un-normalized, per-lattice-point quadratic quantities (gradient-squared, momentum-squared, field-strength-squared, E·B) that both `Averages` and the energy/snapshot measurers are built from.
- `include/CosmoInterface/definitions/gausslaws.h`: checks the numerical preservation of the U(1) and SU(2) Gauss constraints, comparing the lattice divergence of the electric field against the matter (and, if present, axion) charge-density source.
- `include/CosmoInterface/definitions/hubbleconstraint.h`: checks preservation of the first Friedmann equation, comparing $\dot a^2$ against the total energy density.
- `include/CosmoInterface/definitions/mattercurrents.h`: computes the U(1) and SU(2) matter charge densities and currents, used both as the Gauss-law source and as the source term in the gauge-field kernels.
- `include/CosmoInterface/definitions/potential.h`: sums the potential terms defined in the user's model, and converts the model-supplied derivatives with respect to the field *norm* into derivatives with respect to the field *components*, for complex scalars and SU(2) doublets.
- `include/CosmoInterface/definitions/axioncouplings.h`: implements an axion(-like pseudoscalar)–U(1) Chern-Simons coupling $\propto \phi F\tilde F$, providing the extra source terms it contributes to the scalar equation of motion, to the U(1) kernel, and to the U(1) Gauss law.
- `include/CosmoInterface/definitions/nonminimalcoupling.h`: computes the effective Planck-mass rescaling and effective Ricci scalar sourced by the matter/gauge sector, for models with a non-minimal coupling to gravity.
- `include/CosmoInterface/definitions/fixedbackgroundexpansion.h`: implements a prescribed (non-self-consistent) power-law expansion history, used by the evolvers when a run is configured with a fixed rather than self-consistent background.
- `include/CosmoInterface/definitions/phaseBunchDavies.h`: computes the Bunch-Davies vacuum phase used to imprint physically motivated initial fluctuations onto a field or gauge momentum in Fourier space.
- `include/CosmoInterface/definitions/chiralpowerspectrum.h`: defines the `ChiralProjector` class, which projects a U(1) gauge field (or its momentum) onto positive/negative helicity modes in Fourier space, so that helicity-resolved ("chiral") power spectra can be measured for axion-sourced gauge fields.
- `include/CosmoInterface/definitions/gwsprojector.h` and `include/CosmoInterface/definitions/PITensor.h`: source and project the gravitational-wave sector, discussed next.

Gravitational waves are sourced by the transverse, traceless part of the anisotropic stress tensor of the matter and gauge sectors. `PITensor` assembles it, species by species:

@emgithub(include/CosmoInterface/definitions/PITensor.h:pitensor_total_source)

and `GWsKernels` (in the evolvers' kernels folder) simply adds it to the Laplacian of the tensor perturbation to form the GW equation-of-motion kernel:

@emgithub(include/CosmoInterface/evolvers/kernels/gwskernels.h:gws_kernel_source)

The companion class `GWProjector` (in `gwsprojector.h`) performs the transverse-traceless projection of the GW momentum in Fourier space, needed when *measuring* the gravitational-wave spectrum (see Section [*Measurers*][subsec_Measurers] below).

### **Initializers** { #subsec_Initializers }

Another important aspect of the lattice simulations is the initialization. In this section we look more closely at how it happens in the code. A class `ModelInitializer` synchronizes the initialization of each field type, while the field-species-specific work happens in dedicated classes.

@emgithub(include/CosmoInterface/initializers/modelinitializer.h:modelinitializer_class)

`ModelInitializer` owns the two objects that generate the initial random fluctuations, `FluctuationsGenerator<T>` (the default Gaussian-vacuum prescription) and `ExternalPowerSpectrumInitializer<T>` (an alternative, user-supplied power spectrum, discussed below), both constructed once and threaded through to the sub-initializers that need them. Its `initialize` function calls, in order: `ScaleFactorInitializer::initializeScaleFactor`, if expansion is on; `ScalarSingletInitializer::initializeScalars`, for the scalar singlets; `GWsInitializer::initializeGWs`, if the model has a gravitational-wave field; `SU2Initializer::initializeSU2`, for the SU(2) sector — *before* the U(1) sector, since the SU(2) doublet contributes to the U(1) matter current; and `U1Initializer::initializeU1`, for the U(1)/complex-scalar sector. Once every field species has been initialized, it recomputes the volume averages and, for a self-consistent expansion, corrects the scale factor's time derivative using the just-generated fluctuations, via the Hubble constraint.

We start with the `ScaleFactorInitializer`, whose only method sets the initial time derivative of the scale factor (and, for non-minimally-coupled models, the initial Ricci scalar) either directly from a user-supplied Hubble rate (fixed background) or, together with the initial scale factor itself, self-consistently from the initial kinetic and potential energy, through the first Friedmann equation:

@emgithub(include/CosmoInterface/initializers/scalefactorinitializer.h:scalefactorinitializer_class)

The `ScalarSingletInitializer` sets the fluctuations and homogeneous background of the scalar singlets:

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:initializescalars)

After the fluctuations are set, the homogeneous background components are added; `model.fldS0(i)` and `model.piS0(i)` are given in physical (dimensionful) units, so they are rescaled to program variables by dividing by $f_*$ and $f_* \omega_*$ respectively:

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:default_scalar_homogeneous_modes)

The fluctuations themselves are set field by field, in `initializeRandomScalar`:

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:initializerandomscalar)

For each scalar field, this function checks the field's `ext_PS<n>` input parameter (`ext_PS0`, `ext_PS1`, ...): if it is left at its default (or set to `None`), the field is initialized with the default Gaussian-vacuum prescription described in Section [*Initialization of fluctuations*][sec_InitScalar],

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:default_scalar_dispatch)

otherwise the field is initialized from a user-supplied external power spectrum instead (discussed below):

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:externalps_scalar_dispatch)

Two special cases bypass the default fluctuations: the scalar sector also has a run-wide IC-type parameter, `ICtype_S` (declared, together with its U(1) counterpart discussed below, in `include/CosmoInterface/initializers/initialconditionstype.h`), whose `Homogeneous` value skips the fluctuations entirely; and scalars entering an axion–U(1) coupling are, by default, likewise left without vacuum fluctuations.

The default-vacuum path is implemented in `FluctuationsGenerator` (`include/CosmoInterface/initializers/fluctuationsgenerator.h`), which draws Gaussian random fluctuations as a sum of left- and right-moving waves in momentum space, as described in Section [*Initialization of fluctuations*][sec_InitScalar]. Its normalization, computed in `getFluctuationsNorm`, applies a comoving momentum cutoff,

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_frequency_cutoff)

and the rms amplitude appropriate to the number of spatial dimensions (shown here for the physically relevant 3-dimensional case):

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_norm_3d)

`getNormedFluctuations` then turns this normalization into one actual Gaussian-random Fourier mode:

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_random_gaussian_field)

and `conjugateGaussianFluctuations` combines a left- and a right-moving mode into the field's fluctuation,

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_field_modes)

and its conjugate-momentum counterpart:

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:default_scalar_momentum_modes)

The next initializer we present is the one for the SU(2) sector:

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2initializer_initsu2)

The main function above, `initializeSU2`, first calls `initializeSU2Doublet`, responsible for initializing the matter sector in a way compatible with the Gauss law (see below); once this is done, it sets the initial fluctuations of the gauge fields' time derivatives by inverting the Gauss law in Fourier space, as recalled in Section [*Initial conditions*][subsubsec_initialConditionsNonAb] and described in detail in $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl], and sets the gauge links themselves to unity. Let us now look at how the matter fluctuations are imposed:

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2initializer_doublet)

`initializeSU2Doublet` first imposes the random fluctuations by calling `addFluctuationsSU2DoubletFromPhases`, then adds the homogeneous background components:

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2initializer_fluctuations)

What is implemented here corresponds to the procedure given in [Scalar-Gauge Interactions](My first model of gauge fields.md#eq_fpr_influct3): random amplitudes and left-moving phases are drawn independently for all four doublet components, together with the right-moving phase of component 0; the right-moving phases of components 1–3 are then constructed as functions of the others (see [Scalar-Gauge Interactions](My first model of gauge fields.md#InConstr_2)), so that the initial net gauge charge on the lattice vanishes and the Gauss constraint holds from the start.

Once the SU(2) sector is initialized, the last part to initialize is the U(1) sector, taken care of by `U1Initializer::initializeU1`. As for the scalar singlets, the choice of U(1) initial condition is a single, run-wide setting (the `ICtype_U1` input parameter), and it now offers three implemented paths: `RandomWithMatter` (structurally identical to the SU(2) case above: random doublet-like fluctuations for the complex scalars, then a Gauss-law inversion for the gauge field), `PlaneWavesZeroB` (the vector potential is set to zero and transverse plane-wave fluctuations are imprinted on its conjugate momentum, i.e. the electric field, so that the initial magnetic field vanishes identically — useful e.g. for defect-formation studies), and `BunchDavisTransverseU1` (an analytic Bunch-Davies vacuum on the transverse gauge field, delegated to `ExternalPowerSpectrumInitializer`, discussed next). A `Default` value additionally lets the model class itself select its preferred IC type, and a few further values accepted by the parser (`PlaneWaves`, `DefectsNetwork`, `DefectsWhiteNoise`) are currently placeholders that throw a "not implemented" error. We let the interested reader explore the dispatch directly in the code:

@emgithub(include/CosmoInterface/initializers/u1initializer.h:u1initializer_initializeu1)

`ExternalPowerSpectrumInitializer` (`include/CosmoInterface/initializers/externalpowerspectruminitializer.h`) lets a user replace the default Gaussian-vacuum spectrum of a given scalar singlet with a custom power spectrum, supplied as an external ASCII file with three columns — the momentum, the power spectrum of the field, and that of its conjugate momentum — (via the field's `ext_PS<n>` parameter, seen above), or provides the analytic Bunch-Davies transverse U(1) initial condition just mentioned. Two normalization conventions are supported, selected by the `PS_type` input parameter (which also fixes the convention used when *measuring* power spectra, see the next section); the first, `Type I`, is only valid in 3 spatial dimensions,

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_typeI_norm)

while the more general `Type II` normalization interpolates the supplied power spectrum directly at each lattice momentum, without the extra phase-space factor `Type I` includes:

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_interpolators)

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_sample_field_modes)

@emgithub(include/CosmoInterface/initializers/externalpowerspectruminitializer.h:externalps_sample_momentum_modes)

Finally, gravitational waves are initialized by `GWsInitializer`:

@emgithub(include/CosmoInterface/initializers/gwsinitializer.h:gwsinitializer_initializegws)

This is intentionally trivial: the GW tensor and its time derivative are initialized to exactly zero at every lattice point. Gravitational waves are sourced dynamically, during the evolution, by the anisotropic stress of the matter and gauge sectors (see `PITensor` above) rather than from initial vacuum fluctuations, so there is nothing to randomize here.

### **Measurers** { #subsec_Measurers }

The measurements are built in a similar way to the initializers. All the measurements are synchronized by the `Measurer` class, which holds one dedicated measurer object per field species (plus a few cross-cutting ones):

@emgithub(include/CosmoInterface/measurements/measurer.h:measurer_members)

Most of the individual measurers share a tiny common base class, `AbstractMeasurer`, whose only job is to hold the `lastMeas` flag used to tell the HDF5 backend when it is safe to perform its final, collective close of a file. The single method `measure`, shown below, coordinates the different measurements and calls the field-specific classes that actually perform them:

@emgithub(include/CosmoInterface/measurements/measurer.h:measurer_measure)

We start with what we call "frequent measurements". These call the different field and energy measurers, and ask them to compute their respective observables at the "frequent" rate the user set in the input parameter file. As we will see, this mostly consists of field averages and variances, together with the Gauss laws, energy conservation/Hubble constraint, and the topological charge. Once this is done, we move on to the "infrequent measurements", which consist mostly of field spectra, measured at a second, less frequent user-set rate; note that the very same call is routed either through the binned (`PSMeasurer`) or the unbinned (`UPSMeasurer`) power-spectrum engine, depending on whether the user requested unbinned spectra. After this, we proceed to the "rare measurements", the most resource-consuming ones — this is where, for instance, the three-dimensional snapshots of energy densities are taken. Finally, the measurer prints an occasional update to the console.

To understand better what measurements are performed, let us inspect a few specific measurers, starting with `ScalarSingletMeasurer`:

@emgithub(include/CosmoInterface/measurements/scalarsingletmeasurer.h:scalarsingletmeasurer_constructor)

It contains one `MeasurementsSaver` and one `SpectrumSaver` per scalar singlet, and, if the occupation-number output is requested, an additional `SpectrumSaver` file for it. Volume averages are computed in `measureStandard`:

@emgithub(include/CosmoInterface/measurements/scalarsingletmeasurer.h:scalarsingletmeasurer_measurestandard)

which delegates the actual computation to the external helper class `MeansMeasurer`, common to every field species, whose `measure` function simply computes the mean, mean-square, and rms of a field and its conjugate momentum, and hands them to a `MeasurementsSaver` for output:

@emgithub(include/CosmoInterface/measurements/meansmeasurer.h:meansmeasurer_measure)

After the averages, we move on to the power spectra, computed by `measureSpectra`:

@emgithub(include/CosmoInterface/measurements/scalarsingletmeasurer.h:scalarsingletmeasurer_spectra)

For each field, this saves its power spectrum, the (rescaled) power spectrum of its conjugate momentum, and, if requested, its occupation number (see [Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md#eq_OccuppationNum)). The actual spectrum computation, common to every field species, lives in `PowerSpectrumMeasurer::powerSpectrum`:

@emgithub(include/CosmoInterface/measurements/powerspectrum.h:powerspectrum_measure)

The normalization applied here ensures that the discrete power spectrum recovers the expected continuum-limit expression (see the [*discrete power spectrum*][eq_discretePS] definition), and branches on the requested `PS_type`, the `PS_version`, and the number of spatial dimensions of the model. A companion class, `UnbinnedPowerSpectrumMeasurer` (`unbinnedpowerspectrum.h`), exposes the same interface but returns the individual Fourier modes rather than radially-binned averages, and is used instead whenever unbinned spectra are requested; it requires the HDF5 output backend.

Let us move briefly to `SU2Measurer`, to highlight a few features not present in the scalar case:

@emgithub(include/CosmoInterface/measurements/su2measurer.h:su2measurer_constructor)

Here, rather than one file per gauge field component, we store the volume-averaged norms of the electric and magnetic fields, and, since we can now check the conservation of the Gauss law, an additional file for it. The means and the Gauss-law check are computed together in `measureStandard`:

@emgithub(include/CosmoInterface/measurements/su2measurer.h:su2measurer_measurestandard)

`GaussLaws::checkSU2` returns the relative violation of the Gauss constraint together with its left- and right-hand sides, which are all saved for monitoring purposes. Finally, the electric and magnetic spectra are computed together in `measureSpectra`:

@emgithub(include/CosmoInterface/measurements/su2measurer.h:su2measurer_measurespectra)

`SU2DoubletMeasurer`, `U1Measurer`, and `ComplexScalarMeasurer` all follow this same pattern, adapted to their respective field content: the SU(2) doublet and complex-scalar measurers additionally split their output into real/imaginary (or per-component) averages before summing them into a single norm spectrum, while `U1Measurer` computes the magnetic field directly from a finite-difference of the vector potential, checks the U(1) Gauss law, and, if requested, also measures helicity-resolved ("chiral") spectra of the gauge field and its electric field, using the `ChiralProjector` introduced in the previous section.

The energy contributions of every field species are measured together by `EnergiesMeasurer`:

@emgithub(include/CosmoInterface/measurements/energiesmeasurer.h:energiesmeasurer_measure)

Species by species, it computes the kinetic and gradient (or electric and magnetic) energy contributions, the potential-energy terms, and, for non-minimally-coupled models, the extra non-minimal-coupling energy pieces, summing everything into a total energy. It then checks either the first Friedmann equation, via `HubbleConstraint::get` (self-consistent expansion), or plain energy conservation (no expansion); for a fixed background neither check applies, and none is performed.

The last measurer type we present is `EnergySnapshotsMeasurer`, which, when compiled with HDF5, can print the three-dimensional distribution of the requested energy components (or field values) to file (if the code was compiled without HDF5, any requested snapshots are silently skipped):

@emgithub(include/CosmoInterface/measurements/energysnapshotmeasurer.h:energysnapshotsmeasurer_constructor)

Upon initialization, it determines which of the available snapshot quantities the user requested, and creates the corresponding HDF5 files. The snapshots themselves are taken in `measure`, shown here only for the scalar sector, the other field species following the same pattern:

@emgithub(include/CosmoInterface/measurements/energysnapshotmeasurer.h:energysnapshotsmeasurer_measure)

A few more measurers, added since the initial release of the code, are worth mentioning briefly. `GWsMeasurer` (`gwsmeasurer.h`) computes the gravitational-wave power spectrum and integrates it into the GW energy-density fraction, using the transverse-traceless `GWProjector` introduced above:

@emgithub(include/CosmoInterface/measurements/gwsmeasurer.h:gwsmeasurer_constructor)

@emgithub(include/CosmoInterface/measurements/gwsmeasurer.h:gwsmeasurer_measurespectra)

`TopologicalChargesMeasurer` (`topologicalchargesmeasurer.h`) tracks the volume-averaged E·B pseudoscalar of the U(1) sector and its mean square, the ingredients of the axionic/Chern-Simons topological charge density, and `ScaleFactorMeasurer` (`scalefactormeasurer.h`) simply records the scale factor, its time derivative, the Hubble parameter, and, for non-minimally-coupled models, the Ricci scalar, at every "frequent" measurement (when the expansion is dynamical). Finally, the occupation-number computation mentioned above lives in its own small class, `OccupationNumberMeasurer` (`occupationnumber.h`).

#### Measurements Input/Output

To conclude the section devoted to measurements, we want to briefly present the structure of the classes responsible for outputting the measurements to file, so that a user who wants to modify them, or add their own output format, can do so easily. They are located in `include/CosmoInterface/measurements/measurementsIO/`. They are of two kinds: `MeasurementsSaver`, to save volume averages, and `SpectrumSaver`, to save spectra. Both are designed as thin "interface" classes, in the same spirit as `Evolver`: they redirect to the appropriate concrete "implementation" class, so that a new output format can be added without touching any other part of the code. Both work together with `FilesManager`, a helper class that holds global information about the output (the working directory, whether to use HDF5, whether to append or overwrite, flush frequency, and so on), and that, when HDF5 is available, eagerly creates the top-level HDF5 containers used to store averages and spectra (in append mode, existing containers are simply reused).

`MeasurementsSaver` can be constructed either from a plain filename or from a `Field`, in which case the filename is derived automatically from the field's name:

@emgithub(include/CosmoInterface/measurements/measurementsIO/measurementssaver.h:measurementssaver_constructor)

Both constructors dispatch, based on `FilesManager::getUseHDF5()`, to either a plain-text or an HDF5 backend. Its use is straightforward, relying on `addAverage`, which registers a value to be saved, and `save`, which writes everything out:

@emgithub(include/CosmoInterface/measurements/measurementsIO/measurementssaver.h:measurementssaver_save)

The `lastMeas` argument only matters for the HDF5 backend, where it controls when the final, collective close of the file happens.

`SpectrumSaver` works in a similar fashion, but now dispatches between *three* implementations rather than two: a plain-text backend, a binned-HDF5 backend, and, since the introduction of unbinned spectra, a dedicated unbinned-HDF5 backend (unbinned spectra require HDF5):

@emgithub(include/CosmoInterface/measurements/measurementsIO/spectrumsaver.h:spectrumsaver_constructor)

Its use is likewise straightforward, and based on a variadic `save` function that can take an arbitrary number of spectra at once (e.g. to save the electric and magnetic spectra of a gauge field together in a single call), forwarding to the binned or unbinned backend as appropriate:

@emgithub(include/CosmoInterface/measurements/measurementsIO/spectrumsaver.h:spectrumsaver_save)

To learn about the actual implementation of a given backend, we invite the interested reader to explore the `std` and `hdf5` sub-folders directly.
