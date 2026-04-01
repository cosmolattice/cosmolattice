In this section, our aim is to provide the reader with a better understanding of our physics interface, such as how the fields are actually initialized, how they are evolved, or how the measurements are actually done. In order to do this, we guide the reader through the `main` function of CosmoLattice, highlighting the important class/routines responsible for some specific tasks. We will then explain some of these classes in detail, namely the ones responsible for initializing, evolving and measuring the fields. These are the ones that a user will most likely want to modify to suit their specific purposes.

The `main` function is located in the file `src/cosmolattice.cpp`, which we present now:

@emgithub(source/cosmolattice.cpp:main_includes)

For completeness, we explain first the top lines in the file, which take care of the model selection. The variable `MODELINCLUDE` is passed at compilation through the `CMake` and contains the path to the model file. It is transformed into a string by the macro `STRINGIFY`, declared in the file  `"TempLat/util/stringify.h"`. Next, on line `10`, we declare for convenience that we are using the namespace `TempLat`, to which all the CosmoLattice functions belong to. Lastly, we redeclare the custom model passed through CMake `MODELTYPE` to `ModelType` in line `15`. After this, we are ready to go into the main function which is executed when the software runs:

@emgithub(source/cosmolattice.cpp:main_sessionguard)

Line `21` is the standard `C++` main declaration that accepts arguments from the command line. We then instantiate a `SessionGuard` object, which is in charge of allocating and deallocating internal memory space needed by the library. The file follows as:

@emgithub(source/cosmolattice.cpp:main_paramparser)

Here we have created a `ParameterParser` called `parser`. As we saw in the previous sections, this object is used throughout the library to declare and read parameters which are parsed from the input file and the command line. The file then follows like:

@emgithub(source/cosmolattice.cpp:main_simmanager)

Here we have created an object `SimulationManager` called `manager`. This object is used to deal, for instance, with printing the simulation-related information file, or with the backing up of the simulation. It also deals with the fact of whether or not the simulation is restarting from a previous one. In the first case, parameters are retrieved from the restarting simulations, as illustrated on line `39`.

@emgithub(source/cosmolattice.cpp:main_runparams)

We collect all the relevant simulation parameters in a `RunParameters` object called `runParams`. This object receives the information on the parameters, such as the number of points on the lattice, the frequency at which measurements are performed, etc. The file then follows as:

@emgithub(source/cosmolattice.cpp:main_toolbox)

Here we went on defining quantities and objects useful to run the simulation. The first of these is the so-called number of ghost-cells (see Section [*Parallelization*][subsec_para] for the technical details on this). In the current version of CosmoLattice, we use a spatial discretization that requires only one layer of the so called 'ghost-cells' for parellization purposes. However, if you were to implement and use a spatial lattice derivative that requires to call lattice sites beyond the nearest neighbor positions, you would need to change this number, say to e.g. `nGhost = 2` if your derivatives require the next to nearest neighbour.

Following we create a `MemoryToolBox` object, or more specifically, a pointer to such an object. It holds many internal functions related to memory. For instance, this is where the objects that allow to iterate over the lattice are stored. This object is needed to create fields and some other objects, and as a result, it will be passed around to many functions.

We also retrieve on line `66` whether or not the current process is the root process when running in parallel. The only reason why we do this here is to be able to output in a clear manner some information about the run (typically the running time) in the command line while running in parallel. The file then follows with:

@emgithub(source/cosmolattice.cpp:main_model)

There we just created our model as defined in the model header file we specified, for instance in the `src/models/lphi4.h` of Section [*Definition and declaration of the model*][subsubsec_DefAndDeclModel].
We also print its name, so that we can make sure that the model running is actually the model we wanted. We are then ready to initialize the files in the model, as described in Section [*Initialization of fluctuations*][sec_InitScalar] and [*Initial conditions*][subsubsec_initialConditionsNonAb]:

@emgithub(source/cosmolattice.cpp:main_initializer)

Here the initialization was taken care by an object of the type `ModelInitializer<double>`, called `initializer`, which we created on line `85` after having checked that we were starting a new simulation and not continuing another one. The `ModelInitializer<double>` class will be explained in detail in Section [*Initializers*][subsec_Initializers]. We also set the initial time. The file now follows with:

@emgithub(source/cosmolattice.cpp:main_restart)

This is in case we are restarting from some previous simulation, so the model is reloaded from the saved state. This is taken care of by the simulation manager.

From the point of view of the physics, we have two more classes to instantiate. This is what we do next:

@emgithub(source/cosmolattice.cpp:main_evolver_measurer)

Above, we first create an object called `evolver` of type `Evolver<double>`. This class, as further explained in Section [*Evolvers*][subsec_Evolvers], is in charge of calling the appropriate evolution algorithms. We also create in line `112` a `Measurer<double>` `measurer` in charge of doing and outputing all the standard measurements. This class is explained at great length in Section [*Measurers*][subsec_Measurers]. Before starting the actual simulation, the file continues with:

@emgithub(source/cosmolattice.cpp:main_infofile)

Here we print in line `117` the parameters the program is running with. We also mandate the `manager` to create an *information file* which contains these parameters (so their values can be checked after the simulation is concluded). This file also indicates the time at which the simulation started, as well as the type of parallelization that was used. We are now ready to proceed to the time evolution of the system:

@emgithub(source/cosmolattice.cpp:main_timeloop)

Here we started by checking whether we are performing any measurements at the given time step. If so, we ask the evolvers to synchronize themselves. That means that if a given algorithm works with fields which do not live at integer time step (e.g. in leap frog conjugate momenta live at semi-integer times), before measuring we evolve the corresponding quantities so that they live at integer time steps (together with the rest of field and scale factor variables). Other algorithms like Velocity Verlet do not need this synchronization as, so the previous step is just ignored by them. Next, we let the `evolver` object to `evolve` the system, i.e. this calls the routines to perform one iteration step of the evolution of both the scale factor, the field content, and all their associated derivatives. Finally, we check whether we are require to backup the simulation or not at the given time step. If we do, the `manager` takes care of it. Once the time evolution is done, we are ready to finalize the simulation:

@emgithub(source/cosmolattice.cpp:main_savesim)

We checked on line `158` whether or not we are required to save the simulation at the end of the execution, in case we desire to restart the simulation from the same time in a future run. If so, the `manager` does it on line `169`. Finally we close the information file, printing the time at which the simulation ended and the total time it ran for.

### Evolvers { #subsec_Evolvers }

In this section we present the main routines responsible for the evolution of the fields, showing explicitly how different evolution algorithms are implemented. The relevant classes are located in \path{src/include/evolvers/}. As presented above, the class which is instantiated in the main is the `Evolver<double>`. Its sole purpose is to be able to choose at runtime between different algorithms, which are implemented in their respective classes. We show here the full class:

@emgithub(include/CosmoInterface/evolvers/evolver.h:evolver_class)

The class holds a pointer to any of the implemented algorithm classes, namely  `LeapFrog` and `VelocityVerlet` for the time being, as declared on lines `75` and `76`. The relevant pointer is initialized in the constructor, depending on which algorithm was requested. This is what happens between lines `32` and `41`. The argument `pType` is an `EvolverType` enumerator. These enumerators are defined in the file `src/CosmoInterface/evolvers/evolvertype.h`, and are used to differentiate between evolvers. The `VelocityVerlet` pointer is initialized in the default case because all the different higher-order Velocity-Verlet schemes are implemented in the same class.
This structure is repeated in the `evolve` function on line `43`, which uses the appropriate pointer to call the evolution function\footnote{Note than a more oriented object manner to implement this mechanism would have been to use *polymorphism*. We decided against this option to make it more accessible to users less familiar with C++.}. Note that the `template<class Model>` template argument of the function is the mechanism we use to be able to have the `Evolver` (or as we will see later on any class) to operate on arbitrary models. The last function we have is the `sync` function, which is used to synchronize all the fields (when needed) at integer times. It is used before measuring, and it is useful for instance in the case of staggered leapfrog.

To summarize, the `Evolver<double>` class allows us to choose among evolvers. If the user decides to implement their own evolver, they can simply add it here to be able to choose it at run-time.

The actual characterization of each evolution algorithm is in the `evolvers` files `src/include/CosmoInterface/evolvers/leapfrog.h` and `src/include/CosmoInterface/evolvers/velocityverlet.h`. In the following, we present in detail the leapfrog evolver as an example\footnote{We invite the reader interested in the details of the velocity-verlet algorithm to look them directly in the code, as the structure is pretty similar to the one of the leapfrog method presented here.}

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_constructor)

We see here that the only parameter that our class takes is a boolean, which specifies whether we are considering an expanding universe or not. The structure of the algorithm is really laid out in the `evolve` function:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_evolve)

As explained in detail in Ref. [@Figueroa_2020rrl] and outlined on the comments on lines \codeline[V2]{45-46}, the Leapfrog algorithm consists of two parts. First, we need to evolve the conjugate momenta by computing ''kicks", and then we evolve the field variables by means of the ''drifts", using the previously updated momenta. This also reflects itself in the code. The kick function are first called between line `58` and `63`, and then the drift functions are called between line `73` and `81`. In order to evolve the scale factor, we store the averages of the momenta squared after the kicks, and the averages of the fields squared after the drifts, as can be seen on lines `66` and `83`. We note a small subtlety, which manifests itself in line `56`: In order to perform the measurements, we synchronize the momenta to live at integer time steps. After a synchronization, conjugate momenta only need the to be evolved by half a time step, immediately after the call to a measurement. This is what happens when the weight variable is set to $0.5$. The file follows with:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_sync)

Here the sync function is responsible of evolving the momenta, so that they live at integer times for a measurement (e.g. of energy outputs). It also computes the resulting momenta averages. Following, all kicks are constructed in the same way:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_kicks)

The file calls the appropriate ''kick" functions defined in the folder `src/include/CosmoInterface/kernels/`. The structure of the kernels and their implementation will be presented in detail in the next section. What is perhaps worth noting here is the use of `ForLoop` to iterate over the field. This is necessary because the iteration needs to happen during compilation, so that the kernel for the appropriate field can be returned by the kernels function.

Following, the drifts are implemented in a very similar way:

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_drifts)

Here we take the opportunity to present some of the versatility of CosmoLattice for introducing a new syntax to perform operations on fields. For instance, we see on line `191` that we do not iterate over the collection of fields. Indeed, with CosmoLattice we can directly operate at the level of collections, and the appropriate `ForLoop` is expanded internally. Of course, we could write all the drifts using `ForLoop` as for the kicks, and the result would be the same. Note also that for the scale factor, we also allow for the possibility of a fixed background expansion. In this case, the scale factor is simply given by a function of time.

The last relevant methods to be defined are the ones that store the appropriate averages, which are needed for the scale factor evolution (in the case of self-consistent evolution):

@emgithub(include/CosmoInterface/evolvers/leapfrog.h:leapfrog_averages)

Here we call the `Averages` routines where the appropriate averages over fields squared are defined, see next section. It also uses the averaging function `average` on the model potential directly, returned by `Potential::potential(model)` (which computes the sum of the potential terms defined in the model).

The structure of the Velocity-Verlet evolver is exactly the same, so we invite the reader to have a look at the code for themselves.

### Kernels and other physics formulae { #subsec_Kernels }

CosmoLattice has been designed to be as compartmentalized as possible, so that any potential change in the code has only ''local" consequences and does not propagate throughout the whole code. This is particularly true for the main field operations, and we have also attempted this in the implementation of the physics. All physical formulae that can be potentially needed for more than one routine have been extracted out and implemented in their own separate class. We have already encountered one example of this in the *kernels* for the evolution algorithms, as different *evolvers* may use the same kernels or parts of them. This aspect is also true for the averages of some fields, which are both used in the evolution routines and in the measurements.

Let us first have a look at some of the kernels, to gain a better understanding of the code structure. As an example we will have a look at the complex scalar kernel first:

@emgithub(include/CosmoInterface/evolvers/kernels/complexscalarkernels.h:complexscalarkernels_class)

As we can see, the `class ComplexScalarKernels` is extremely simple, and its sole purpose is to contain the complex scalar field's evolution kernel defined in Eq. ([*85*][eq_kernelcomplexscalar]). As we can see on lines `29` and `30`, and correspondingly from the includes on lines `10` and `11`, the kernel uses two `definitions` classes, namely `GaugeDerivatives` and `Potential`. The first one stores the generic gauge covariant derivative expressions and related expressions, while the later is used to compute the potential derivative with respect to the field components, as well as to compute the potential from the different potential terms defined in the model.

@emgithub(include/CosmoInterface/definitions/gaugederivatives.h:gaugederivatives_laplacians)

Let us start with the `GaugeDerivatives` class from `definitions`. We see that this class has no internal variable, but it is just used to regroup some functions under one hood. In particular, it holds the functions capable of computing the covariant derivatives and covariant Laplacians of the matter/gauge content of the users' models (for conciseness, we show here only the covariant Laplacians). They rely on the functions `U1sForCSCovDerivs`, `U1sForSU2DoubletCovDerivs` and `SU2sForSU2DoubletCovDerivs`, which compute the appropriate combinations of link variables, that depend on which matter fields couple to which gauge field.

This is a good time to emphasize one of the advantages of having implemented  a 'functional' interface through expression templates in CosmoLattice. The expressions computed by these Laplacian functions (as well as most of the other functions in `definitions`) are really abstract expressions (formulae), in the sense that they are left to be evaluated. Calling e.g. `covLaplacianCS` does not lead to any actual computation, it simply creates an abstract expression which can be evaluated later on. In particular, it gives the opportunity to perform some ''analytical" simplifications to the expressions at compilation. For example, a redundant multiplication of the type ''$1 \cdot \phi$" with $\phi$ some field, can be detected at compilation time and replaced by $\phi$ only. This mechanism is used for instance in the `U1sForCSCovDerivs` function.

Following in the same file,

@emgithub(include/CosmoInterface/definitions/gaugederivatives.h:u1sforcscovderivs)

We encounter the function `fold_multiply` in line `87`, which takes vectorial objects as an argument, and returns the multiplication of all the elements inside the object. In this case we use it on an array, created on line `88` by the macro `MakeArray`, which contains either the link `U1Links(model.fldU1(a),i)` or ''$1$", depending on whether the matter field $N$ couples to the $a^{th}$ $U(1)$ gauge field. The constant $1$ is represented by the object `OneType` on line `91`, which is then automatically discarded by the compiler in multiplications. For more information about how this works, we refer the reader to Appendix [Under the Hood: Expression Templates and CosmoLattice](Under the Hood: Expression Templates and CosmoLattice.md).

The rest of the `GaugeDerivatives` class contains similar functions to compute the other covariant derivatives, as well as functions which implement gradients. As they are all implemented in a very similar fashion, we let the reader explore the code by themselves.

All the other functions in `definitions` are implemented in a similar fashion. Rather than going through all the code, we explain below what they are used for.

-  `src/include/CosmoInterface/definitions/averages.h`: Computes the appropriate averages of the momenta squared and the gradient squared, used in the evolution of the scale factor. It relies on the `FieldFunctionals` class, which defines the correct sum over components.

-  `src/include/CosmoInterface/definitions/energies.h`: Contains the correct rescaling of the conjugate momenta and the correct normalization to compute the energy contributions. These methods can be called either with averages, to obtain a single number, or with the `FieldFunctionals`, to obtain the energy distributions over the whole lattice.

-  `src/include/CosmoInterface/definitions/fieldfunctionals.h`: Defines the appropriate sum over component to compute the relevant energy contributions of the different fields species to the Hubble laws.
-  `src/include/CosmoInterface/definitions/gaugederivatives.h`: As reviewed in the text, defines the expressions to compute the gauge covariant derivatives.
-  `src/include/CosmoInterface/definitions/gausslaws.h`: Defines the expressions to compute the Gauss laws in the Abelian and non-Abelian sectors.
-  `src/include/CosmoInterface/definitions/hubblelaws.h`: Defines the expressions of the Hubble constraint.
-  `src/include/CosmoInterface/definitions/mattercurrents.h`: Defines the expressions of the matter currents for the equations of motion. It is implemented in the same spirit as the `GaugeDerivatives`, and it is able to compute the current for generic matter content.
-  `src/include/CosmoInterface/definitions/potential.h`: Computes the potential from the `potentialTerms` defined in the user's model. Also computes, for the complex scalar fields and the $SU(2)$ doublets, the potential derivative with respect to the field's component in terms of the potential derivative with respect the norm.

These functional forms from `definitions` are used all throughout `CosmoInterface`.

### Initializers { #subsec_Initializers }

Another important aspect of the lattice simulations is the initialization. In this section, we will have a closer look at how this initialization happens in the code. First, there is a class `ModelInitializer` that synchronizes the initialization of each field type, while the specific initialization happens in dedicated classes.

@emgithub(include/CosmoInterface/initializers/modelinitializer.h:modelinitializer_class)

As previously claimed, the only purpose of the `ModelInitializer` class is to call the specific initializers in the correct order and to initialize the required averages and Hubble laws; this is what happen in the `initialize` function between lines `36` and `60`. It also holds a dedicated object `FluctuationsGenerator<T>`, which is used by the specific class to generate random Gaussian fluctuations.

@emgithub(include/CosmoInterface/initializers/scalefactorinitializer.h:scalefactorinitializer_class)

We start with the the `ScaleFactorInitializer`. The only method it contains is `initializeScaleFactor`, shown above. We initialize the scale factor from the homogeneous initial values of the fields, assuming that the gauge fields have zero initial homogeneous components.

@emgithub(include/CosmoInterface/initializers/scalarsingletinitializer.h:initializescalars)

The `ScalarSingletInitializer` is also very simple and holds only the `initializeScalars` methods. There, on line `30`, we set the initial fluctuations of the scalar fields using the `FluctuationsGenerator`. We then add on lines `37` and `38` the homogeneous initial conditions.

The `FluctuationsGenerator` generator class implements Gaussian fluctuations of the type described in Section [*Initialization of fluctuations*][sec_InitScalar]:

@emgithub(include/CosmoInterface/initializers/fluctuationsgenerator.h:fluctuationsgenerator_class)

The most important function here is `getNormedFluctuations`, defined between lines `62` and `67`. It first computes the correct normalization factor in momentum space by calling the `getFluctuationsNorm` function, and returning a correctly normalized `RandomGaussianField`. This `RandomGaussianField` operates in Fourier space returning Gaussian distributed random modes at every point of the Fourier lattice.
To facilitate the initialization, it also provides functions to directly compute the amplitudes as described in Eqs. \eqref{eq:fpr_influct}-\eqref{eq:fpr_influct2}.
The function `gaussianFluctuations` initializes fluctuations of only one given field, while `conjugateGaussianFluctuations` sets the fluctuations of a field and its conjugate momentum. As we will see shortly in the rest of the initializers, this class exists because Gaussian fluctuations enter in some way or another in the initialization of all the different type of matter fields.

The next initializer we want to present is the one for the $SU(2)$ sector:

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2initializer_initsu2)

The main function above is `initializeSU2`. It first calls the `initializeSU2Doublet` functions, which are responsible to initialize the matter sector in a way that is compatible with the Gauss law, see later on. Once this is done, it sets the initial fluctuations of the gauge fields by inverting the Gauss law, as recalled in Section [*Initial conditions*][subsubsec_initialConditionsNonAb] and described in detail in Ref. [@Figueroa_2020rrl]. First, on line `67` we compute the matter current in real space. Then, on line `75` we invert the Gauss law in Fourier space. The current is computed in Fourier space by calling the `inFourierSpace()` method of the `Field` class. Lastly, we set the gauge links to unity on line `82`. Let us know look at how the matter fluctuations are imposed:

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2initializer_doublet)

We see that the  `initializeSU2Doublet` is in charge of first imposing the random fluctuations by calling `addFluctuationsSU2DoubletFromPhases`. Then, the homogeneous components are added to the fields.

@emgithub(include/CosmoInterface/initializers/su2initializer.h:su2initializer_fluctuations)

What is implemented in the function `addFluctuationsSU2DoubletFromPhases` simply corresponds to the procedure given in Eqs ([*77*][eq_fpr_influct3])-([*80*][InConstr_2]). The independent amplitudes are created first, and then these are used to construct the dependent ones afterwards. All of this is then used to initialize the fluctuations of the fields and momenta.

Once the $SU(2)$ sector is initialized, the last part which needs to be initialized is the $U(1)$ sector. This is taken care of by the `U1Initializer`. It works in a similar was a the `SU2Initializer`, so we let the interested reader to go directly to explore the code located at: `src/include/CosmoInterface/initializers/u1initializer.h`.

### Measurers { #subsec_Measurers }

The measurements are built in a similar way as the initializers. All the measurements are synchronized by the `measurer` class:

@emgithub(include/CosmoInterface/measurements/measurer.h:measurer_class_decl)

@emgithub(include/CosmoInterface/measurements/measurer.h:measurer_measure)

It possesses a single method `measure`, displayed above, whose aim is to coordinate the different measurements and call field specific classes which perform field specific measurements. We start with what we call ''frequent  measurements". These call the different field and energy measurers and ask them to compute their respective observable at a ''frequent" rate defined by the user in the input parameter file. As we will see, it mostly consists in field averages and variances together with the Gauss laws and energy conservation/Hubble constraints. Once this is done, we move on to the ''infrequent measurements", in line `101`, and proceed in a similar fashion. Infrequent measurements mostly consists in fields' spectra measured at a more infrequent rate determined by the user (again in the input parameter file). Also, when using `txt` output, we store the ''infrequent" times in a file, on line `120`, to facilitate the data analysis. After this, we proceed with the ''rare measurements", corresponding to the most resource consuming. This is where for instance the three-dimensional snapshots of energy densities are measured.
Finally, the measurer is also used to print out some information to the console, see line `129`.

To understand better what measurements are performed, we will inspect specific measurers:

@emgithub(include/CosmoInterface/measurements/measurer.h:measurer_members)

We show above the different measurers that can be used by CosmoLattice. All the field measurers are similar, so we will only present in details of `ScalarSingletMeasurer` and `SU2Measurer`. We will then move on to the `EnergiesMeasurer` and the `EnergySnapshotMeasurer`:

@emgithub(include/CosmoInterface/measurements/scalarsingletmeasurer.h:scalarsingletmeasurer_constructor)

We start by looking here at how our `ScalarSingletMeasurer` is initialized. It contains two arrays, namely `standardOut` and `spectraOut` which contains objects which can save respectively mean values and spectra, one for each scalar field. Here, the files are created by passing the fields and the file names are automatically generated through the fields' names.

@emgithub(include/CosmoInterface/measurements/scalarsingletmeasurer.h:scalarsingletmeasurer_measurestandard)

The averages are actually computed in the `measureStandard` function. As the names suggest, the measurements we want to perform are very standard and will be the same for all the fields. As such they are performed by an external class, called `MeansMeasurer`, whose `measure` function takes a `MeasurementsSaver`, a field, the corresponding velocity and the time at which the measurements have to be conducted.

@emgithub(include/CosmoInterface/measurements/meansmeasurer.h:meansmeasurer_measure)

As we see now, this functions simply computes the average of the field, its average square and its variance, as well for the velocity, and adds it to the `MeasurementsSaver`.

After the averages are measured, we move on to the power spectra.

@emgithub(include/CosmoInterface/measurements/scalarsingletmeasurer.h:scalarsingletmeasurer_spectra)

This is taken care of by the `measureSpectra` function of the `ScalarSingletMeasurer`. There, for each field, we compute its power spectrum as defined in Eq. ([*24*][eq_discretePS]), we compute the power spectrum of its associated velocity and the occupation number, defined in Eq. ([*44*][eq_OccuppationNum]). They are all saved in the same file. As noted in the comment, in the current implementation, it is better to perform the scale factor re-scaling of the momentum outside the `powerSpectrum` function. Indeed, when called on a `Field`, the function does not allocate extra memory to perform the Fourier transform. It does do that for any argument which is not purely a `Field`, as it would be that case had we called for instance `powerSpectrum(piS(i) * pow(model.aI,model.alpha - 3))`.

Let us move briefly to the `SU2Measurer`, to highlight some features not used in the `ScalarSingletMeasurer`. First, at initialization, we do not create files for individual quantities. We will store the averages of the norms of the  fields. As such, the `MeasurementsSaver` are created with customs name. In the case of the $SU(2)$ sector, we are also interested in checking the conservation of Gauss' law and as a result create another file to store them. The spectra are computed also only for the norms.

@emgithub(include/CosmoInterface/measurements/su2measurer.h:su2measurer_constructor)

The measurements of the means are performed in the `measureStandard` function:

@emgithub(include/CosmoInterface/measurements/su2measurer.h:su2measurer_measurestandard)

We see here that we can use again the `MeansMeasurer` to measure the mean values and variances of the electric and magnetic fields. We also measure the Gauss law and how well it is satisfied. The first component of the `gaussArr` contains the violation degree of the Gauss law, while the second and third contain respectively the left-hand and right-hand sides of the Gauss law.

Finally, electric and magnetic spectra are computed in the `measureSpectra` function:

@emgithub(include/CosmoInterface/measurements/su2measurer.h:su2measurer_spectra)

Contrary to the scalar case, since we are anyhow measuring composite field expressions, it does not matter where we do the momentum rescaling. For readability we do it in an auxiliary field variable `ESU2`.

The `EnergyMeasurer` is also built in very similar way. It contains two `MeasurementsSaver`, one to store the different energy components and one to store the energy conservation check in the case without expansion or the Hubble law check in the case with expansion. The measurements are then performed in the `measure` function.

@emgithub(include/CosmoInterface/measurements/energiesmeasurer.h:energiesmeasurer_measure)

One by one field species, we compute their energy contributions (this is why we do not used the stored averages, as these are summed over field species). We save them one after another in a file. We also compute the total energy of the system. Above we show explicitly only the scalar contributions, the reader can see the other contribution directly in the file of the code. After this, we also compute the contribution from the potential, term by them. We store finally the total energy of the system in the last column of the file.

The last distinct type of measurer is the `EnergySnapshotMeasurer`. In the current implementation, it can only be used with the `HDF5` library. When asked, it prints the three-dimensional distribution of the requested energy components. Below we show the code only for the scalar sectors, other being similar.

@emgithub(include/CosmoInterface/measurements/energysnapshotmeasurer.h:energysnapshotsmeasurer_constructor)

First, upon initialization, it determines which energy components it needs to save to file.
Then, again shown only for the scalar sector, if needed, the binary files which will contain the snapshots are created.
Finally, the energy snapshots are taken in the `measure` function:

@emgithub(include/CosmoInterface/measurements/energysnapshotmeasurer.h:energysnapshotsmeasurer_measure)

Species by species, we measure their energy contribution and save the three-dimensional distribution to file.

#### Measurements Input/Output

To conclude the section devoted to measurements, we want to briefly present the structure of the classes responsible to output the measurements to file, so that a user who wants to modify them or add its own output format, can do so easily. They are located in the `src/include/CosmoInterface/measurements/measurementsIO/` folder. They are of two types: the `MeasurementsSaver`'s, to save mean values, and the `SpectrumSaver`'s,to save spectra to file. They are designed in a similar way than the `Evolver`'s, in the sense that they both have an ''interface" class, called `MeasurementsSaver` and `MeasurementsSaver`, which redirect to the appropriate ''implementation" class, which is in charge of actually saving the output. In this way, it is straightforward to implement different output format without having to modify any other part of the code. These interfaces work together with a helper class called `FilesManager`, which has global information over all the measurement files.

Let us first discuss the `MeasurementsSaver` interface. In the current state of the code, there is only one ''standard" implementation to save the measurements, so strictly speaking there would be no need for an interface; we kept this design for future development purposes. To create a `MeasurementsSaver`, you have two different options, one which takes a `std::string` as input and names the file this way, and another that takes a `Field` variable, naming the files according to its name (this constructor can also accept algebraic expression and name the file accordingly).

@emgithub(include/CosmoInterface/measurements/measurementsIO/measurementssaver.h:measurementssaver_constructor)

Both methods take the same arguments. First, the `FilesManager`, which is created and stored in the `Measurer` class. Its purpose is to collect generic information about the outputing procedure. This is where the choice between different interfaces should be stored. Even if it is not implemented yet, it could also be of use to create a folder structure to save the different outputs. The `amIRoot` parameter tells the `MeasurementsSaver` whether it belongs to the root `MPI` process or not, as only the root process is allowed to save to file (in the standard implementation at least). The `appendMode` specify whether the old measurements files potentially present (by mistake or when restarting a simulation) should be appended to or overwritten. The `headers` file is an array, by default empty, containing the headers for the file. Lastly, the `dontCreate` allows to call one of the `MeasurementsSaver` saver constructors without actually creating a file; this is useful to switch between different physical scenarios which do not need the same output (e.g. expanding versus non expanding universe).

The use of the `MeasurementsSaver` is intended to be very straightforward. It relies on the method `addAverage`, which registers a value to be stored, and a `save` function which actually saves all the registered values to file.

@emgithub(include/CosmoInterface/measurements/measurementsIO/measurementssaver.h:measurementssaver_save)

The `SpectrumSaver` interface works in a similar fashion, except that in this case, two interfaces are already implemented: the standard one and the `HDF5` one (see Section [*A more user-friendly format for the spectra*][subsubsec_hdf5spectra]). Filenames are also constructed either from a `std::string` or a `Field`'s name.

@emgithub(include/CosmoInterface/measurements/measurementsIO/spectrumsaver.h:spectrumsaver_constructor)

Its use is also straightforward and based on a single `save` function.

@emgithub(include/CosmoInterface/measurements/measurementsIO/spectrumsaver.h:spectrumsaver_save)

It takes as arguments a time $t$, and an arbitrary number of spectra. It then forwards them to the correct implementation (standard or `HDF5`) and save them to a file. The $\dots$ syntax is the modern way of `C++` to create functions for an arbitrary number of arguments. To learn about the actual implementation, we invite the interested reader explore the files in the `std` and `hdf5` sub-folders.

