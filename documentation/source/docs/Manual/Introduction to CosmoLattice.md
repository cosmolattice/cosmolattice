Here we first present an [*Overview*][subsec_OV] of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, where we briefly comment about the basic features of the code. In Section [*Capabilities and structure*][subsec_Purpose] we further discuss the capabilities of the code and introduce its file structure. In Section [*Basic Field Equations implemented (so far)*][subsec_BasicEOM] we review the basic field equations that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to solve. 

## Overview { #subsec_OV }

The application of numerical methods to studying non-linear field dynamics in the early Universe has grown substantially in recent years. Building increasingly efficient, accurate, and robust numerical tools is essential for placing predictions about the early Universe on a firm theoretical foundation. These developments have led to the emergence of a distinct field—**Lattice Cosmology**—whose strength lies in its ability to resolve detailed non-linear field dynamics and provide reliable predictions for the resulting observables. **Lattice Cosmology Techniques** (LCT) represent nowadays a well-established route towards a deeper understanding of the early Universe, and we expect them to play an increasingly prominent role in shaping observational strategies designed to probe its physics. It was in this context that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ was originally developed with a clear aim: to provide a powerful, flexible and robust package, publicly available, and dedicated to solving the non-linear dynamics of field theories in an expanding Universe, thus allowing to investigate the phenomenology and observational implications of early Universe scenarios. At the time of writing (July 2026), the latest version the code, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ${\tt v2.0}$, can simulate: 

<div class="cl-sector-chips" markdown>
[*Singlet scalar field interactions*](My first model of (singlet) scalar fields.md){ .cl-chip .cl-chip-scalar }
[*Abelian U(1) scalar-gauge interactions*](My first model of gauge fields.md){ .cl-chip .cl-chip-u1 }
[*Non-Abelian SU(2) scalar-gauge interactions*](My first model of gauge fields.md){ .cl-chip .cl-chip-su2 }
[*Axion like particle - Abelian U(1) gauge field interactions*](ALP.md){ .cl-chip .cl-chip-alp }
[*Non-minimally coupled-to-gravity scalar field dynamics*](NMC.md){ .cl-chip .cl-chip-nmc }
[*Global cosmic defects (domain walls, strings, monopoles, textures)*](Defects.md){ .cl-chip .cl-chip-defects }
[*Local topological defects (local strings)*](Defects.md){ .cl-chip .cl-chip-defects }
[*Gravitational waves from scalar field theories*](GW.md){ .cl-chip .cl-chip-gw }
[*Gravitational waves from Abelian U(1) scalar-gauge theories*](GW.md){ .cl-chip .cl-chip-gw }
<!-- <span class="cl-chip">[...]</span> -->
</div>

In other words, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can handle scenarios with: interacting singlet scalar fields; charged scalar fields under a U(1) and/or SU(2) gauge symmetries, with the corresponding Abelian and/or non-Abelian gauge vector fields; axion-like-particles (ALP) interacting with Abelian gauge sectors, non-minimal gravitationally coupled scalar fields, gravitational wave (GW) emission from singlet scalar fields, GW emission from charged scalars and Abelian gauge fields, global and local cosmic defects, and soon enough (see below [*Upcoming Physics*][UpcomingPhysics]) fluid dynamics and other interactions. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can simulate the dynamics of such fields either in a flat space-time, or in a homogeneous and isotropic (spatially flat) expanding background. In the latter case, the fields can evolve either over a fixed background (e.g. with a power-law scale factor), or with **self-consistent expansion of the Universe, *i.e.* with the fields involved in a simulation determining themselves the expansion rate through the Friedmann equations**.

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ provides both symplectic integrators, with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, and non-symplectic integrators with accuracies from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{4})$ (See Sections 3.3, 3.4 and 3.5 of $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]), for a discussion on integrators and their properties). Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for scalar-gauge theories, either Abelian or non-Abelian, respect the Gauss constraint to machine precision, independently of the integrator, even in the case of self-consistent expansion. This is thank to the fact that we write the discretization of our scalar-gauge theories with lattice-gauge-invariant techniques (to know more about the latter, see *e.g.* Section 3 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ (Ref. [@Figueroa_2020rrl]), or the [*Lattice gauge invariant techniques*][subsec_LatticeGaugeInv] tab in this website). 

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is written in C++, and fully exploits the *object oriented  programming* nature of this language, with a modular structure that separates well all the ingredients involved. This allows $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to have a clear separation between the physics and the technical implementation details. **The code is designed so that the user can simulate a given scenario with different parameters, without requiring to re-compile each time that parameter values are changed**. <!-- More importantly, the code allows for an easy implementation of new models with either scalar or gauge interactions.  -->

**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is fully parallelized using both shared memory parallelization (*OpenMP*, *POSIX Threads* or GPU acceleration) and distributed parallelization (*Message Passing Interface* (MPI)) for use in high-performance clusters, and uses a discrete Fourier Transform parallelized in multiple spatial dimensions**. This makes it ideal for probing physical problems with well-separated mass/length scales, running very high resolution simulations, or simply shortening the running time of long simulations. To provide these capabilities, **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ relies on `TempLat`, a general package for lattice simulations that defines field variables and their operations, by introducing its own symbolic language, and managing all aspects of dispatching workload to the available computational resources. Once you become familiar with the basic ‘vocabulary’ of the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ and `TempLat` language, editing the code or implementing your own model (resembling how you would write it in the continuum), should become a simple task.**

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can be used at multiple levels of complexity: 

<div class="cl-prose-grid" markdown>

<div class="cl-prose-card" markdown>

**Basic level**
{: .cl-minihead }


A **basic level** user with no experience at all in parallelization techniques and with little to no programming experience will be able to run fully parallelized simulations of their favourite models on hundreds of cores in a cluster, while being completely oblivious to the technical details of the algorithm implementation or parallelization. CosmoLattice automatically prints a collection of relevant observables, such as volume averages, field spectra, or dynamical constraints, which can be used to monitor the evolution of the system. 


</div>

<div class="cl-prose-card" markdown>

**Intermediate level**
{: .cl-minihead }

An **intermediate level** user with a certain programming experience, will want to modify the type of output CosmoLattice generates, change the initial condition routine (e.g. turn it into a Monte-Carlo generator for thermal configurations), or even design their own evolution algorithms and add these to the family of integrators available in CosmoLattice. 

</div>

<div class="cl-prose-card" markdown>

**Advanced level**
{: .cl-minihead }

An **advanced level** user may want to play as well with the hard core inner parts of CosmoLattice and TempLat, in an attempt to understand or to improve the most advanced technical aspects of the code. This could include e.g. the handling of the parallelization, which is something that typically will remain as a black box for the majority of users. This means contributing to TempLat, the lattice framework at the core of CosmoLattice.

</div>

</div>

What type of user you want to be is entirely up to you: it simply depends on your programming expertise (or lack of it), and on your will to learn how the code works internally. 

[](){ #UpcomingPhysics }
**Upcoming Physics**. We have developed $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ with the intention of providing an evolving and up-to-date relevant numerical tool for the scientific community working on the physics of the early universe. While presently (at the time of writing, July 2026) $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is already capable of simulating canonical scalar-gauge field theories, non-minimal scalar theories, Abelian gauge-ALP interactions, global and local cosmic defects, gravitational waves from scalar and/or gauge fields, etc, we have conceived $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ as an evolving package that we plan to upgrade regularly with new features or specialized tasks, or extra modules dedicated to new physics. For example, to mention just a few of the latter, we are currently working on enhancing the capabilities of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate as well: 

<div class="cl-sector-chips" markdown>
<span class="cl-chip cl-chip-upcoming">*Fluid dynamics (fully relativistic and non-relativistic)*</span>
<span class="cl-chip cl-chip-upcoming">*Fluid - Abelian U(1) gauge field interactions*</span>
<span class="cl-chip cl-chip-upcoming">*Fluid - scalar field interactions*</span>
<span class="cl-chip cl-chip-upcoming">*Non-minimal kinetic scalar theories*</span>
<span class="cl-chip cl-chip-upcoming">*Gravitational waves from Non-Abelian SU(2) scalar-gauge theories*</span>
<span class="cl-chip cl-chip-upcoming">*Axion like particle - Non-Abelian SU(2) gauge field interactions*</span>
<span class="cl-chip cl-chip-upcoming">[...]</span>
</div>

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is freely available to anyone who wants to use or modify it, as long as you give us credit for its creation, see [**How to cite $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$**](./HowToCiteCL.md). If you have any questions or comments about $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, please contact us, as we would love to hear how the program performs for you, and we will be happy to assist you with questions you might have, bug reports, suggestions for future improvements, etc. 

<!--You can download $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ at any time from [ http://www.cosmolattice.net](http://www.cosmolattice.net)-->

!!! note "**Note**"
    If you would like to help developing some aspect of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, or even implement your own modules with some new functionality we have not envisaged, please contact us and let us know about your idea(s). TempLat, which is developed by the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ team, introduces a natural language describing fields and operations between them, so it is a natural platform to implement new libraries (related or not to cosmology).

## Capabilities and structure { #subsec_Purpose .cl-sec-fold }

As explained in the previous [Overview][subsec_OV] section, rather than being a conventional code designed to solve a predetermined set of equations and compute a fixed collection of observables, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ constitutes a *platform* for the implementation of field-theoretical systems governed by partial differential equations that can be discretized on a lattice. Written in C++, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ follows a modular architecture that cleanly separates the underlying physics from its technical implementation. It also introduces a dedicated symbolic language in which field variables and the operations acting on them are expressed in a form closely resembling their continuum counterparts. To be more specific, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is characterized by a number of aspects that we list below, which constitute all together a clear advantage for using $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to investigate early Universe scenarios, over writing your own code from scratch:


$~~~~~\bullet$ To begin with, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to simulate not only the evolution of singlet scalar theories, but also Abelian U(1) and non-Abelian SU(2) gauge theories. Since $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ $\tt v2.0$ was released (on July 2026), one can also simulate scalars with a non-minimal coupling to gravity, $\phi^2R$, and axion-like particle (ALP) interactions with gauge fields, $\phi F_{\mu\nu}\tilde F^{\mu\nu}$; it incorporates methods to set up special field configurations, like cosmic defect networks in *scaling* (*e.g.* cosmic strings and domain walls), or arbitrary field power spectra; it incorporates non-symplectic evolution algorithms suitable *e.g.* for non-minimal scalar kinetic theories of the type $\mathcal{G}_{ab}(\lbrace\phi_c\rbrace)\partial_\mu\phi^a\partial^\mu\phi^b$; it also allows for optimized gravitational wave dynamics on the lattice sourced by scalar and gauge fields, as well as for scalar field dynamics in $d + 1$ dimensions with $d = 1, 2$. All simulations can be done either in a flat space-time background, or in an expanding background.

$~~~~~\bullet$ The *lattice engine* of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is `TempLat`, which has been designed as a *platform* to implement any system of dynamical equations suitable for discretization on a lattice. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is therefore not just a code for one type of simulation, but rather a more evolved concept. Via `TempLat`, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ uses a **symbolic language** that incorporates real, complex and SU(2) algebras, allowing to use vectorial and matrix notations without sacrificing performance. Once the user becomes familiar with the basic 'vocabulary' of the new language, they can modify $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ for their own purpose: let it be for the time evolution of new field variables in a given relevant model of interest, or for some other operation, like *e.g.* a Monte-Carlo generator for thermal configurations. 

$~~~~~\bullet$ The *heart* of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is `CosmoInterface`, which handles all relevant aspects of the physical theories, such as the field content, initialization, evolution equations, or relevant output observables. This makes the physics part of the code easy to understand and well separated from technical details from `TempLat`. This separation significantly simplifies the process of writing new operations for your own purposes. `CosmoInterface` represents essentially a *library* where $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ includes all the necessary ingredients that make possible to simulate a rich variety of scenarios. At the time of writing (July 2026), $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ${\tt v2.0}$ is ready to simulate scenarios that include interacting singlet scalar fields; charged scalar fields under a U(1) and/or SU(2) gauge symmetries, and their corresponding Abelian and/or non-Abelian gauge vector fields; axion-like-particles (ALP) interacting with gauge sectors, non-minimal scalar field dynamics, and soon enough fluid dynamics and other interactions (see above [*Upcoming Physics*][UpcomingPhysics]). Simulations can be done in a flat space-time background, or in a spatially flat expanding FLRW background. In the latter case the fields can evolve either over an external background (e.g. with a power-law scale factor), or **self-consistently**, *i.e.* 'dictating' themselves the expansion of the universe as sourced by their volume averaged energy and pressure densities. 

$~~~~~\bullet$ One of the main advantages of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is that it clearly separates the *physics* (*e.g.* definition of the field content, operations between fields, evolution equations, etc) in `CosmoInterface`, from the more *technical details* (e.g. parallelization aspects, Fourier transforms, etc) in `TempLat` [which is actually not stored inside the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ source tree, as it is an external dependency fetched upon compilation of the code]. `TempLat` automates tecnical tasks fully, and many user will not never need to edit it. For example, if we have two fields `f` and `g` on a lattice and we want to sum them. Typically, one would explicitly write a loop that sums the amplitudes of both fields at each node of the lattice. Instead, in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we can just write `f + g`, and `TempLat` handles the whole operation of summing their values everywhere on the lattice. Essentially, all the technicalities such as memory handling or parallelization tasks, remain mostly hidden from a typical user. 

$~~~~~\bullet$ As a consequence of the above separation between physics and technicalities, a beginner user, say with little experience in programming and no experience at all in parallelization techniques, will be able to run a fully parallelized simulation of their favourite model (perhaps using thousands of processors in a cluster), while being completely oblivious to the technical details. They will just need to write a basic *model file* in the language of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, containing the details of the model being simulated. If, on the contrary, the user is rather an experienced programmer and wants to look inside the core routines of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ and `TempLat`, and modify, say the MPI-implementation, they can always do so, and perhaps even contribute to their improvement. 

$~~~~~\bullet$ Furthermore, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ provides symplectic integrators in `CosmoInterface`, with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, to simulate the non-linear dynamics of the appropriate fields in comoving two- or three-dimensional lattices. Appropriate observables are also provided for each case, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching down to machine precision in the case of the highest order integrators. Algorithms for gauge theories, either Abelian or non-Abelian, respect (independently of the integrator) the Gauss constraint to machine precision, even when considering self-consistent expansion. Moreover, through `TempLat` $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ uses a discrete Fourier Transform parallelized in multiple spatial dimensions [@Pi13], this enables the code to run simulations with very high resolution, very long time scales, or well-separated scales. 

The basic folder tree structure of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is the following:

=== "Folder tree"

    <div class="cl-tree">
    <ul>
    <li><a class="cl-tnode cl-tnode-root" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha" target="_blank" rel="noopener">cosmolattice</a>
    <ul>
    <li><a class="cl-tnode cl-tnode-file" href="https://github.com/cosmolattice/cosmolattice/blob/CLV2.0Alpha/CMakeLists.txt" target="_blank" rel="noopener">CMakeLists.txt</a></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/cmake" target="_blank" rel="noopener">cmake</a><ul><li><a class="cl-tnode cl-tnode-file" href="https://github.com/cosmolattice/cosmolattice/blob/CLV2.0Alpha/cmake/title.cmake" target="_blank" rel="noopener">title.cmake</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/source" target="_blank" rel="noopener">source</a><ul><li><a class="cl-tnode cl-tnode-file" href="https://github.com/cosmolattice/cosmolattice/blob/CLV2.0Alpha/source/cosmolattice.cpp" target="_blank" rel="noopener">cosmolattice.cpp</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include" target="_blank" rel="noopener">include</a><ul>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface" target="_blank" rel="noopener">CosmoInterface</a><ul>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface/abstractmodel" target="_blank" rel="noopener">abstractmodel</a></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface/definitions" target="_blank" rel="noopener">definitions</a></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface/evolvers" target="_blank" rel="noopener">evolvers</a><ul><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface/evolvers/kernels" target="_blank" rel="noopener">kernels</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface/initializers" target="_blank" rel="noopener">initializers</a></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface/measurements" target="_blank" rel="noopener">measurements</a><ul><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/include/CosmoInterface/measurements/measurementsIO" target="_blank" rel="noopener">measurementsIO</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-file" href="https://github.com/cosmolattice/cosmolattice/blob/CLV2.0Alpha/include/CosmoInterface/cosmointerface.h" target="_blank" rel="noopener">cosmointerface.h</a></li>
    <li><a class="cl-tnode cl-tnode-file" href="https://github.com/cosmolattice/cosmolattice/blob/CLV2.0Alpha/include/CosmoInterface/runparameters.h" target="_blank" rel="noopener">runparameters.h</a></li>
    <li><a class="cl-tnode cl-tnode-file" href="https://github.com/cosmolattice/cosmolattice/blob/CLV2.0Alpha/include/CosmoInterface/simulationmanager.h" target="_blank" rel="noopener">simulationmanager.h</a></li>
    </ul></li>
    </ul></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/models" target="_blank" rel="noopener">models</a><ul><li><span class="cl-tnode cl-tnode-gen">*.h</span></li><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/models/parameter-files" target="_blank" rel="noopener">parameter-files</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/tests" target="_blank" rel="noopener">tests</a><ul><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/tests/CosmoInterface" target="_blank" rel="noopener">CosmoInterface</a></li><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/tests/benchmarks" target="_blank" rel="noopener">benchmarks</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/documentation" target="_blank" rel="noopener">documentation</a><ul><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/documentation/source" target="_blank" rel="noopener">source</a></li><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/documentation/scripts" target="_blank" rel="noopener">scripts</a></li><li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/documentation/tools" target="_blank" rel="noopener">tools</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/profile" target="_blank" rel="noopener">profile</a></li>
    <li><a class="cl-tnode cl-tnode-dir" href="https://github.com/cosmolattice/cosmolattice/tree/CLV2.0Alpha/thoughts" target="_blank" rel="noopener">thoughts</a></li>
    <li><span class="cl-tnode cl-tnode-gen">build</span><ul>
    <li><span class="cl-tnode cl-tnode-gen">_deps</span><ul>
    <li><a class="cl-tnode cl-tnode-ext" href="https://github.com/cosmolattice/templat" target="_blank" rel="noopener">templat-src</a><ul><li><a class="cl-tnode cl-tnode-ext" href="https://github.com/cosmolattice/templat/tree/main/include/TempLat" target="_blank" rel="noopener">include/TempLat</a></li><li><a class="cl-tnode cl-tnode-ext" href="https://github.com/cosmolattice/templat/tree/main/cmake" target="_blank" rel="noopener">cmake</a></li><li><a class="cl-tnode cl-tnode-ext" href="https://github.com/cosmolattice/templat/tree/main/tests" target="_blank" rel="noopener">tests</a></li><li><a class="cl-tnode cl-tnode-ext" href="https://github.com/cosmolattice/templat/tree/main/external" target="_blank" rel="noopener">external</a></li></ul></li>
    <li><a class="cl-tnode cl-tnode-ext" href="https://github.com/kokkos/kokkos" target="_blank" rel="noopener">kokkos-src</a></li>
    <li><a class="cl-tnode cl-tnode-ext" href="https://github.com/cosmolattice/parafaft" target="_blank" rel="noopener">parafaft-src</a></li>
    <li><span class="cl-tnode cl-tnode-gen">&hellip;</span></li>
    </ul></li>
    </ul></li>
    </ul>
    </li>
    </ul>
    </div>
    <p class="cl-tree-hint">Click any node to open it on GitHub (branch <code>CLV2.0Alpha</code>); dashed nodes are generated locally at build time.</p>

=== "What each folder contains"

    with the content of each folder summarized as:

    | **folder name(s)**         | **brief description of each folder**                                                                                                                      |
    | -------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
    | `CMakeLists.txt`           | top-level build configuration; fetches `TempLat`, selects the model, creates the executable, and links `TempLat::TempLat` with `CosmoInterface`           |
    | `cmake`                    | CosmoLattice-specific CMake helper files                                                                                                                  |
    | `source`                   | executable entry point, currently `cosmolattice.cpp`                                                                                                      |
    | `include/CosmoInterface`   | CosmoLattice physics interface: model abstraction, field definitions, evolution algorithms, initial conditions, measurements, and simulation management   |
    | `abstractmodel`            | base classes and shared data structures for scalar, gauge, scale-factor, gravitational-wave, and non-minimal-coupling model sectors                       |
    | `definitions`              | reusable physical definitions such as potentials, gauge derivatives, Gauss laws, power-spectrum helpers, and anisotropic-stress tensors                   |
    | `evolvers`                 | evolution algorithms and evolution kernels                                                                                                                |
    | `initializers`             | initialization for fields, scale factor and model state                                                                                                   |
    | `measurements`             | observables, spectra, energy outputs, gravitational-wave outputs, and measurement I/O helpers                                                             |
    | `models`                   | user-facing model files selected with `-DMODEL=...`                                                                                                       |
    | `models/parameter-files`   | example input parameter files for the implemented models                                                                                                  |
    | `tests`                    | CosmoInterface tests and benchmarks, enabled with `-DCOSMOINTERFACE_TEST=ON`                                                                              |
    | `documentation`            | Documentation source                                                                                                                                      |
    | `build`                    | local build directory generated by CMake; contains object files and fetched dependency sources under `_deps/`. This directory is **created by the user.** |
    | `build/_deps/templat-src`  | generated checkout of the external `TempLat` repository, which provides the lattice language and technical infrastructure                                 |
    | `build/_deps/kokkos-src`   | generated Kokkos source tree used by `TempLat` as the default device backend                                                                              |
    | `build/_deps/parafaft-src` | generated optional ParaFaFT source tree for MPI parallel Fourier transforms                                                                               |
    | `build/_deps/...`          | generated dependency trees for optional packages such as KokkosFFT, FFTW, or HDF5, depending on the build options                                         |

The CMake configuration first selects the model to compile from `models/`, then builds the executable from `source/cosmolattice.cpp`. The executable includes `CosmoInterface` from this repository and links against the external `TempLat::TempLat` target. `TempLat` is fetched at configure time and is responsible for the lattice-expression machinery, memory layout, parameter parsing, Fourier-transform infrastructure, and device backend setup. Depending on the selected CMake options, `TempLat` may in turn fetch or find external packages such as Kokkos, KokkosFFT, ParaFaFT, FFTW, HDF5, MPI, OpenMP, or C++ threads.

!!! note
    A remarkable feature of `TempLat` is that its operations and parallelization can work in an arbitrary number of spatial dimensions $d$, including $d < 3$ and $d > 3$. This makes it a natural basis for future interfaces dealing with field dynamics on lower- or higher-dimensional lattices. In the latest version of the code, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ $\tt v2.0$, we have exploited this feature to run scalar field simulations in $d = 1$ or $2$ spatial dimensions. Keep an eye into the [$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ News](../CL_News/CLnews.md) tab in case we eventually consider other cases. 


## Basic Field Equations implemented (so far) <!-- in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$  --> { #subsec_BasicEOM .cl-sec-fold }

We introduce now the equations of motion (EOM) that the latest version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can solve. These include EOM of canonical scalar-gauge theories, various non-canonical theories, and gravitational waves, all embedded in an expanding FLRW background. The derivation of these EOM can be found in our monographic reviews: $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]), and $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]), see [Lattice-Cosmology Reviews](../th_framework/MonographicReviews.md).

<span style="font-size: 30px; font-weight: bold;">
**Canonical Scalar-Gauge Theories**
</span>

We start with canonical scalar-gauge theories with three kinds of canonically-normalized scalar fields: a real scalar singlet $\phi$, a U(1)-charged complex scalar field $\varphi$, and a $[SU(2) \times U(1)]$-charged doublet scalar field $\Phi$. In the last two cases, there are also present, respectively, Abelian and non-Abelian gauge fields, $A_{\mu}$ and $C_{\mu} \equiv C_{\mu}^a T_a$, with $T_a \equiv \sigma_a / 2$ the SU(2) group generators, where $a=1,2,3$ and $\sigma_a$ are the *Pauli matrices*. Like any $SU(N)$ Lie algebra, the generators satisfy that $[T_a, T_b] = i f_{abc} T_c$ , ${\rm Tr}(T_a) = 0$ , ${\rm Tr}(T_a T_b) = \frac{1}{2} \delta_{ab}$, and $T^{\dagger}_a = T_a$, with $[\cdot,\cdot]$ the usual matrix commutator, and $f_{abc}$ the totally anti-symmetric *structure constants*, which in SU(2) are the totally anti-symmetric tensor, $f_{abc} = \epsilon_{abc}$. For further details see Chapter 2 of $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]). The scalar fields can be explicitly written in terms of real components as follows,
[](){ #eq_ChargedScalars }
```math
\begin{eqnarray} \label{eq_ChargedScalars}
    \begin{array}{ccccc}
        \phi \in \mathcal{R}e & , &  \varphi \equiv {1\over\sqrt{2}}(\varphi_1 +i\varphi_2) & , & \Phi = \left(
        \begin{array}{c}
            \varphi^{(1)} \\ \varphi^{(2)} \\ \vdots \\ \varphi^{(N)}
        \end{array}
        \right) =
        {1\over\sqrt{2}}
        \left(
        \begin{array}{c}
            \varphi_1 +i\varphi_2 \\ \varphi_3 +i\varphi_3 \\ \vdots \\ \varphi_{2N -1} +i\varphi_{2N}
        \end{array}
        \right) \,.
    \end{array}
\end{eqnarray}
```

More specifically, for canonical scalar-gauge theories we consider the action $S = \int d^4x  \sqrt{-g}\, \mathcal{L}$, with $g \equiv {\rm det} (g_{\mu \nu})$, and the Lagrangian
[](){ #eq_lagrangian }
```math
\begin{align} 
-\mathcal{L} = \frac{1}{2}\partial_{\mu} \phi \partial ^{\mu}\phi + (D_{\mu}^A \varphi)^{*}(D_A^{\mu} \varphi) +  (D_{\mu}\Phi )^{\dagger} (D^{\mu} \Phi) + \frac{1}{4} F_{\mu \nu} F^{\mu \nu} + \frac{1}{2}{\rm Tr}\{G_{\mu \nu}G^{\mu \nu}\} + V \ ,
\label{eq_lagrangian} 
\end{align}
```
with $V \equiv V(\phi,|\varphi|, |\Phi|)$ the potential describing the interactions between the scalar fields. The *covariant derivatives* and *field strength tensors* associated to the gauge fields, are defined as
[](){ #eq_AbCovDerivCont }
[](){ #eq_CovDerivCont }
```math
\begin{eqnarray}
D_{\mu}^{\rm A}  &\equiv &  \partial _{\mu} - i  g_A Q_AA_\mu \ , \hspace{4.5cm} F_{\mu \nu}\equiv  \partial_{\mu}  A_{\nu} - \partial_{\nu} A_{\mu} \ , \label{eq_AbCovDerivCont} \\
D_{\mu} & \equiv  &
\mathcal{I}D^{\rm A}_\mu
- i g_C Q_C C_{\mu}^a \,T_a
\ , \hspace{2.76cm}  G_{\mu \nu} \equiv \partial_{\mu} C_{\nu} - \partial_{\nu} C_{\mu} - i[C_\mu,C_\nu]\,,  \label{eq_CovDerivCont}  
\end{eqnarray}
```
with $g_{A}$ and $g_C$ the Abelian and non-Abelian gauge couplings, $Q_{A}$ and $Q_C$ the Abelian and non-Abelian charges of the scalar fields, $\mathcal{I}$ the $N\times N$ identity matrix. The gauge-invariant electric and magnetic fields associated to the Abelian and non-Abelian fields can be written as
[](){ #eq_ElectricMagneticDefs }
```math
\begin{equation}\label{eq_ElectricMagneticDefs}
E_i \equiv F_{0i} , \,\,\,\,\,\,\,\,  B_i \equiv \frac{1}{2} \epsilon_{i j k} F^{j k} , \,\,\,\,\,\,\,\,   E_i^a \equiv G_{0i}^a , \,\,\,\,\,\,\,\,  B_i^a \equiv \frac{1}{2} \epsilon_{i j k} G^{j k}_a \ , \end{equation}
```
where $\epsilon_{ijk}$ is the Levi-Civita symbol in three dimensions with normalization $\epsilon_{123}=+1$, and $G_{\mu \nu}^a \equiv {\rm Tr}(2G_{\mu \nu} T_a) = {\rm Tr}(G_{\mu \nu} \sigma_a)$. Here it is important to note that **the electric field definitions above depend on the $\alpha$-time $\eta$, as $F_{0i}$ and $G_{0i}$ are defined with respect to $\eta$, not $t$.**

The equations of motion for the matter fields and the scale factor have been derived in more detail in $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]). Here we simply quote their resulting form, which read
[](){ #eq_singlet-eomCONT }
[](){ #eq_higgsU1-eom }
[](){ #eq_higgsSU2-eom }
[](){ #eq_U1eom }
[](){ #eq_SU2eom }
[](){ #eq_GaussU1-eom }
[](){ #eq_GaussSU2-eom }
```math
\begin{eqnarray}
    \phi'' - a^{-2(1 - \alpha)} {\vec\nabla}^{\,2} \hspace{-1mm}\phi + (3 - \alpha)\mathcal{H} {\phi'} &=& - a^{2 \alpha} V_{,\phi} \ , \label{eq_singlet-eomCONT} \\
    \varphi'' - a^{-2(1 - \alpha)} {\vec D}_{\hspace{-0.5mm}A}^{\,2}\varphi + (3 - \alpha) \mathcal{H}  {\varphi'} &=& - \frac{a^{2 \alpha}V_{,|\varphi|} }{2} \frac{\varphi}{|\varphi |} \ , \label{eq_higgsU1-eom}\\
    \Phi'' - a^{-2(1 - \alpha)} {\vec D}^{\,2}\Phi + (3 - \alpha) \mathcal{H}  {\Phi'} &=& - \frac{a^{2 \alpha} V_{,|\Phi|}}{2} \frac{\Phi}{|\Phi |} \ , \label{eq_higgsSU2-eom}
    \\
    \partial_0 F_{0i} - a^{-2(1 - \alpha )}\partial_j F_{ji} + (1 - \alpha) \mathcal{H} F_{0i} &=&
    a^{2 \alpha}J^A_i \ , \label{eq_U1eom}
    \\
    (\mathcal{D}_0 )_{a b} (G_{0i})^b - a^{-2(1 - \alpha )} ( \mathcal{D}_j )_{a b} (G_{ji} )^b + (1 - \alpha) \mathcal{H} (G_{0i} )^b &=& a^{2 \alpha}(J_i)_a \ , \label{eq_SU2eom}
\end{eqnarray}
```
where we have introduced the derivative operator $(\mathcal{D}_{\nu}O)_a = (\mathcal{D}_{\nu})_{a b}O_b \equiv ( \delta_{a b}  \partial_{\nu} - f_{abc} C_{\nu}^c ) O_b$, and defined the matter currents
[](){ #eq_AbelianCurrent }
[](){ #eq_NonAbelianCurrent }
```math
\begin{eqnarray}
    \label{eq_AbelianCurrent}
    J_A^\mu & \equiv & 2g_A Q_A^{(\varphi)} \mathcal{I}m [ \varphi^{*} ( D_A^{\mu} \varphi )] + 2g_A Q_A^{(\Phi)} \mathcal{I}m [ \Phi^\dagger (D^{\mu} \Phi  )]\,,\\
    \label{eq_NonAbelianCurrent}
    J_a^\mu & \equiv & 2g_C Q_C\mathcal{I}m [ \Phi^{\dagger} T_a( D^{\mu} \Phi )]\,.
\end{eqnarray}
```
We note that the Gauss constraints of the Abelian and non-Abelian sectors, respectively, must be preserved at all times during the evolution:
[](){ #eq_GaussU1-eom }
[](){ #eq_GaussSU2-eom }
```math
\begin{eqnarray}
    \partial_i F_{0i} &=& a^2J^A_0 \ , \label{eq_GaussU1-eom}\\
    (\mathcal{D}_i )_{a b} (G_{0i})^b &=& a^2(J_0)_a \ , \label{eq_GaussSU2-eom}
\end{eqnarray}
```

If the fields dominate the energy budget of the Universe, the expansion rate can be determined through the Friedmann equations, which in our case can be written as
[](){ #eq_FriedmannHub }
[](){ #eq_FriedmannD2a }
```math
\begin{align}
\label{eq_FriedmannHub}
\mathcal{H}^2    \equiv    \frac{a'^{ 2}}{a^2} &=  \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ E_K^{\phi} + E_K^{\varphi} + E_K^{\Phi} + E_G^{\phi} + E_G^{\varphi} + E_G^{\Phi} + E_K^A + E_K^B + E_G^A + E_G^B + E_V \right]  ,
\\
\label{eq_FriedmannD2a}
{a''\over a} &= \frac{a^{2 \alpha}}{3} \left( \frac{f_*}{m_p}\right)^2 \left[ (\alpha-2)(E_K^{\phi} + E_K^{\varphi} + E_K^{\Phi}) + \alpha(E_G^{\phi} + E_G^{\varphi} + E_G^{\Phi}) + (\alpha + 1)E_V \right.\\
& \hspace{2cm} \left. +  (\alpha-1)(E_K^A + E_K^B + E_G^A + E_G^B) \right]  ,\nonumber
\end{align}
```
where $\langle \dots \rangle$ denotes an average over sufficiently large volumes that encompass all relevant wavelengths of the fields, and we have defined the volume-averaged energy contributions as $E_{K}^{f} = \langle K_{f} \rangle$ and $E_{G}^{f} = \langle G_{f} \rangle$ for the scalar fields $f=\phi,\varphi,\Phi$, $E_{K}^{A} = \langle K_{U(1)} \rangle$, $E_{G}^{A} = \langle G_{U(1)} \rangle$, $E_{K}^{B} = \langle K_{SU(2)} \rangle$, and $E_{G}^{B} = \langle G_{SU(2)} \rangle$ for the gauge fields, and ${E}_V = \langle {V} \rangle$ for the potential energy, where the different energy density contributions are given by
[](){ #eq_energy-contributions }
```math
\begin{align}
\label{eq_energy-contributions}
\hspace{-1cm}\left\lbrace
\begin{array}{rcl}
{K}_{\phi} &=& \frac{1}{2 a^{2\alpha} } \phi'^2 \\
{K}_{\varphi} &=& \frac{1}{a^{2\alpha} } (D_0^A \varphi)^*(D_0^A \varphi)
\\
{K}_{\Phi} &=& \frac{1}{a^{2\alpha} } (D_0 \Phi )^\dagger(D_0 \Phi)
\\
\end{array}\right.
\hspace{0.1cm};\hspace{0.75cm}
\left\lbrace
\begin{array}{rcl}
{G}_{\phi} &=& \frac{1}{2 a^2} \sum_i (\partial_i \phi)^2
\\
{G}_{\varphi} &=& \frac{1}{a^2} \sum_i (D_i^A \varphi)^*(D_i^A \varphi)
\\
{G}_{\Phi} &=& \frac{1}{a^2} \sum_i (D_i\Phi)^\dagger(D_i \Phi)
\\
\end{array}\right.
\hspace{0.1cm};\hspace{0.75cm}
\left\lbrace
\begin{array}{rcl}
{K}_{U(1)} &=& \frac{1}{2 a^{2 + 2 \alpha}} \sum_{i} F_{0i}^2
\\
{K}_{SU(2)} &=& \frac{1}{2 a^{2 + 2 \alpha}}  \sum_{a,i} (G_{0i}^a)^2
\\
{G}_{U(1)} &=& \frac{1}{2 a^4}  \sum_{i,j < i} F_{ij}^2
\\
{G}_{SU(2)} &=& \frac{1}{2 a^4}  \sum_{a,i,j < i}  (G_{ij}^a)^2    . \\
\end{array}\right.
\\\nonumber\\
\text{(Kinetic-Scalar)} \hspace{5cm} \text{(Gradient-Scalar)} \hspace{6.5cm} \text{(Electric & Magnetic)} \hspace{3.0cm}\nonumber\\
\end{align}
```
In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we use Eq. (\ref{eq_FriedmannD2a}) to solve for the scale factor, while monitoring that the constraint equation (\ref{eq_FriedmannHub}) is verified throughout the evolution to some desired accuracy, see Section [*Evolution Algorithms*][subsec_Algorithms].

<div style="height: 20px;"></div>

<span style="font-size: 30px; font-weight: bold;">
**Non-Canonical Field Theories**
</span>

Among the possible field theories with non-canonical interactions, we consider scalar fields non-minimally coupled (NMC) to gravity through a term of the form $\propto\phi^2R$, with $R$ the Ricci scalar, and axion-like particles (ALPs) interacting with gauge fields through a coupling of the form $\phi F\tilde F$. Further details on these cases can be found in Chapters 3 and 4 of $\mathtt{The~Art-II}$ (Ref. [@BaezaBallesteros_2025tme]).

<!-- <div style="height: 20px;"></div> -->

[](){ #NMC_th }
**Scalar Non-Minimally Coupled to Gravity**

We consider a theory containing a curvature interaction proportional to $\xi R\phi^2$ and a generic potential $V(\phi,\{\varphi_{\rm m}\})$ for $\phi$ and the remaining matter fields $\{\varphi_{\rm m}\}$. In the Jordan-frame, restricting the metric to a spatially flat FLRW background, and hence neglecting gravitational perturbations, the equation of motion for $\phi$ becomes

[](){ #eq_eom }
```math
\begin{eqnarray}
\phi''
+(3-\alpha)\frac{a'}{a}\phi'
-a^{-2(1-\alpha)}\nabla^2\phi
+a^{2\alpha}
\left(
\xi\bar R\phi+\frac{\partial V}{\partial\phi}
\right)
=0\,,
\label{eq_eom}\\
{\rm where} ~~~~~~\bar R =
\frac{6}{a^{2\alpha}}
\left[
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
\right]\,.~~~~~~~~~~~~~~
\label{eq_cosmic_R}
\end{eqnarray}
```
where $R$ is the backgorund Ricci scalar, and $\xi$ the non-minimal coupling parameter. 

The Friedmann equations in $\alpha$-time read 

[](){ #eq_Hu }
[](){ #eq_2FE }
```math
\begin{eqnarray}
\mathcal{H}^{2}
\equiv
\left(\frac{a'}{a}\right)^2
=
\frac{a^{2\alpha}}{3m_p^2}
\left(\bar\rho_\phi+\bar\rho_{\rm m}\right)
\label{eq_Hu}\,~~;~~~~~~~~~~
\frac{a''}{a}
=
-\frac{a^{2\alpha}}{6m_p^2}
\left[
(1-2\alpha)
\left(\bar\rho_\phi+\bar\rho_{\rm m}\right)
+3\left(\bar p_\phi+\bar p_{\rm m}\right)
\right]\,,
\end{eqnarray}
```
where the energy density and pressure of the non-minimally coupled scalar field are [@Figueroa:2021iwm]

[](){ #eq_nmcrho }
```math
\bar\rho_\phi(\eta)
=
\frac{1}{2a^{2\alpha}}\left\langle\phi'^2\right\rangle
+\frac{1}{2a^2}\left\langle(\nabla\phi)^2\right\rangle
+\left\langle V(\phi)\right\rangle
+\frac{3\xi}{a^{2\alpha}}\mathcal{H}^2
 \left\langle\phi^2\right\rangle
+\frac{6\xi}{a^{2\alpha}}\mathcal{H}
 \left\langle\phi\phi'\right\rangle
-\frac{\xi}{a^2}\left\langle\nabla^2\phi^2\right\rangle\,.
\label{eq_nmcrho}\\
```
[](){ #eq_nmcp }
```math
\begin{aligned}
\bar p_\phi(\eta)
={}&
\frac{1-4\xi}{2a^{2\alpha}}\left\langle\phi'^2\right\rangle
-\frac{1-12\xi}{6a^2}\left\langle(\nabla\phi)^2\right\rangle
-\left\langle V(\phi)\right\rangle
+\frac{2\xi}{a^{2\alpha}}\mathcal{H}
 \left\langle\phi\phi'\right\rangle
-\frac{\xi}{3a^2}\left\langle\nabla^2\phi^2\right\rangle
\\
&+
2\xi\left\langle\phi V_{,\phi}\right\rangle
+\frac{\xi}{a^{2\alpha}}
\left[
\mathcal{H}^2
+12\left(\xi-\frac{1}{6}\right)
\left(
\frac{a''}{a}
+(1-\alpha)\mathcal{H}^2
\right)
\right]
\left\langle\phi^2\right\rangle\,,
\end{aligned}
\label{eq_nmcp}
```
with $V_{,\phi}\equiv\partial V/\partial\phi$, and $\langle \dots \rangle$ denoting volume-averaging over sufficiently large scales that encompass all relevant wavelengths of the fields. The scale factor may, in principle, be evolved using the Friedmann equations in $~$\eqref{eq_Hu}. We note, however, that contrary to canonical scenarios, the *r.h.s.* of the Friedmann equations depend on time derivatives of $a(\eta)$, given the expression of $\bar\rho_\phi(\eta), \bar p_\phi(\eta)$.

As proposed in Ref. [@Figueroa_2021iwm], the scale factor evolution can be obtained alternatively from the trace of the energy-momentum tensor of the non-minimally coupled field, $T_\phi$. Given the traced Einstein equations $R = -\frac{1}{m_p^2}g^{\mu\nu}\left(T^\phi_{\mu\nu}+T^{\rm m}_{\mu\nu}\right)$ = $-\frac{1}{m_p^2}\left(T_\phi+T_{\rm m}\right)$, it follows that the background curvature satisfies

[](){ #eq_EFEtrBack }
```math
m_p^2\bar R =
(1-6\xi)
\left[
\left\langle
\partial^\mu\phi\,\partial_\mu\phi
\right\rangle
+\xi\bar R\left\langle\phi^2\right\rangle
\right]
-6\xi\left\langle\phi V_{,\phi}\right\rangle
+4\left\langle V\right\rangle
-\left\langle T_{\rm m}\right\rangle\,,
\label{eq_EFEtrBack}
```
where $\langle\cdots\rangle$ denotes again volume-averaging of the corresponding local spatial inhomogeneities, over length scales much larger than the inverse gradient-scales of the problem. Solving the above expression for $\bar R$, then gives

[](){ #eq_eomR }
[](){ #eq_Fphi }
```math
\begin{eqnarray}
\bar R
=
\frac{F(\phi)}{m_p^2}
\left[
(1-6\xi)
\left\langle
\partial^\mu\phi\,\partial_\mu\phi
\right\rangle
+4\left\langle V\right\rangle
-6\xi\left\langle\phi V_{,\phi}\right\rangle
-\left\langle T_{\rm m}\right\rangle
\right]\,.
\label{eq_eomR}\\
~~~~{\rm where}~~~
F(\phi)
\equiv
\frac{1}{
1+(6\xi-1)\xi\left\langle\phi^2\right\rangle/m_p^2
}\,.~~~~~~~~~~~~~~~
\label{eq_Fphi}
\end{eqnarray}
```

Using Eq.$~$\eqref{eq_cosmic_R}, we then obtain the following differential equation for the scale factor,

[](){ #eq_piadot }
```math
\frac{a''}{a}
+(1-\alpha)\left(\frac{a'}{a}\right)^2
=
\frac{a^{2\alpha}F(\phi)}{6m_p^2}
\left[
(1-6\xi)
\left\langle
\partial^\mu\phi\,\partial_\mu\phi
\right\rangle
+4\left\langle V\right\rangle
-6\xi\left\langle\phi V_{,\phi}\right\rangle
-\left\langle T_{\rm m}\right\rangle
\right]\,.
\label{eq_piadot}
```

This equation can be evolved simultaneously with the equations of motion of the non-minimally coupled scalar and the remaining matter fields. In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ we actually follow this method.

<!-- <div style="height: 20px;"></div> -->

**Axion-Gauge interactions**

Axion-like particles (ALPs) enjoy a shift-symmetry $\phi=\phi+C$ that allows them to couple derivatively to gauge fields through *Chern–Simons* terms, as $\phi F\tilde{F}$ (Abelian) or $\phi G\tilde{G}$ (non-Abelian). Since $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ $\tt v2.0$, the code considers an axion–$U(1)$ sector described by the action

[](){ #eq_AxionAction }

```math
S =
\int d^4x\,\sqrt{-g}
\left[
\frac{1}{2}m_p^2R
-\frac{1}{2}\partial_\mu\phi\,\partial^\mu\phi
-V(\phi)
-\frac{1}{4}F_{\mu\nu}F^{\mu\nu}
+\frac{1}{4}\frac{\phi}{\Lambda}
F_{\mu\nu}\tilde F^{\mu\nu}
\right]\,.
\label{eq_AxionAction}
```
where $\phi$ is a real pseudo-scalar field, the axion-like particle (ALP), $V(\phi)$ is its interaction potential (which might break the shift symmetry explicitly in some scenarios), and the field-strength tensor of the Abelian gauge field $A_\mu$ is defined as in standard canonical theories, $F_{\mu\nu}\equiv\partial_\mu A_\nu-\partial_\nu A_\mu$. To characterize the ALP-gauge coupling strength, we define a dimensionless parameter $\alpha_\Lambda\equiv m_p/\Lambda$ associated to the scale $\Lambda$. The dual field-strength tensor is defined as $\tilde F_{\mu\nu}\equiv\frac{1}{2}\epsilon_{\mu\nu\rho\sigma}F^{\rho\sigma}$, where $\epsilon_{\mu\nu\rho\sigma}$ is the four-dimensional Levi-Civita pseudotensor in curved spacetime, normalized as $\epsilon_{0123}=1/\sqrt{-g}$. Varying the action with respect to $\phi$ and $A_i$ in a FLRW background, and considering the temporal gauge, $A_0=0$, leads to the equations of motion in $\alpha$-time as

[](){ #eq_axion_eom }
[](){ #eq_axion_gauge_eom }

```math
\begin{eqnarray}
\phi''
+(3-\alpha)\mathcal{H}\phi'
-a^{2(\alpha-1)}\vec{\nabla}^{\,2}\phi
+a^{2\alpha}V_{,\phi}
&=&
\frac{\alpha_\Lambda}{m_p}
a^{\alpha-3}\vec E\cdot\vec B\,,
\label{eq_axion_eom}
\\
E_i'
+(1-\alpha)\mathcal{H}E_i
-a^{2(\alpha-1)}\epsilon_{ijk}\partial_jB_k
&=&
\frac{\alpha_\Lambda}{m_p}a^{\alpha-1}
\left(
\phi'B_i+\epsilon_{ijk}\partial_j\phi\,E_k
\right)\,.
\label{eq_axion_gauge_eom}
\end{eqnarray}
```
where primes denote derivatives with respect to $\alpha$-time, $\eta$, and $E_i \equiv F_{0i} = A_i'$. The system is subject to the Gauss constraint, which, in the absence of external charged currents, reads as

[](){ #eq_axion_gauss }

```math
\partial_iE_i
=
-\frac{\alpha_\Lambda}{m_p}
a^{\alpha-1}\partial_i\phi\,B_i\,.
\label{eq_axion_gauss}
```

The local energy density and pressure of the combined axion–gauge sector are

[](){ #eq_axion_rho }

[](){ #eq_axion_p }

```math
\begin{eqnarray}
\rho
&=&
K_\phi+G_\phi+V(\phi)+K_{U(1)}+G_{U(1)}\,,
\label{eq_axion_rho}
\\
p
&=&
K_\phi-\frac{1}{3}G_\phi-V(\phi)
+\frac{1}{3}
\left[
K_{U(1)}+G_{U(1)}
\right]\,.
\label{eq_axion_p}
\end{eqnarray}
```

The individual kinetic and gradient contributions of the scalar and vector fields coincide with those defined in Eq.$~$\eqref{eq_energy-contributions}. If the axion–$U(1)$ gauge sector dominates the energy budget of the Universe, the expansion is governed by the Friedmann equations, Eqs.$~$\eqref{eq_FriedmannHub} and \eqref{eq_FriedmannD2a}, retaining only the contributions from this sector.

<div style="height: 20px;"></div>

<span style="font-size: 30px; font-weight: bold;">
**Gravitational Waves**
</span>

Gravitational waves (GWs) are transverse and traceless tensor perturbations, $h_{ij}$, of the background metric. Considering the FLRW solution as the background metric, the perturbed line element (in $\alpha$-time) is
[](){ #eq_GWmetric }
```math
\begin{align}
\label{eq_GWmetric}
ds^2
=
-a^{2\alpha}(\eta)d\eta^2
+a^2(\eta)\left(\delta_{ij}+h_{ij}\right)dx^idx^j\,,
\end{align}
```
which are transverse, $\partial_i h_{ij}=0$, and traceless, $h_{ii}=0$. Linearizing the Einstein equations in $h_{ij}$, the equation of motion of GWs reads
[](){ #eq_GWEOMcontinuum }
```math
\begin{align}
\label{eq_GWEOMcontinuum}
h_{ij}''
+(3-\alpha){a'\over a}h_{ij}'
-a^{-2(1-\alpha)}\nabla^2h_{ij}
=
{2\over m_p^2a^{2(1-\alpha)}}\Pi_{ij}^{\rm TT} \,.
\end{align}
```
GWs are sourced by the transverse-traceless (TT) part of the anisotropic stress tensor, $\Pi_{ij}^{\rm TT}$. For a generic background fluid, this anisotropic stress tensor takes the form $\Pi_{ij} \equiv T_{ij}-\bar p\,g_{ij}$. In practice, however, it is more convenient to define an effective anisotropic stress tensor containing only those contributions to $\Pi_{ij}$ that have a non-zero TT projection. For example, for a generic model consisting of canonically normalized scalars and Abelian gauge fields, this takes the form (Ref. [@PhDthesisFigueroa])

[](){ #eq_GWEffectiveAnisotropicStress }
```math
\begin{align}
\label{eq_GWEffectiveAnisotropicStress}
\Pi_{ij}^\mathrm{eff}=\sum_a \nabla_i\phi_a \nabla_j\phi_a+2\sum_b \text{Re}\left[ \left(D_i^A\varphi_b\right)^*D_j^A\varphi_b \right]-a^{-2\alpha}E_iE_j-a^{-2}B_i B_j \,,
\end{align}
```
from which the source of GWs is obtained after TT projection. This operation is non-local in real space, but corresponds to an algebraic relation in Fourier space,
[](){ #eq_GWTTsourceProjection }
```math
\begin{align}
\label{eq_GWTTsourceProjection}
\Pi_{ij}^{\rm TT}({\bf k},\eta)
=
\Lambda_{ij,lm}(\hat{\bf k})\Pi_{lm}^{\rm eff}({\bf k},\eta)\,,
\end{align}
```
where the Fourier-space TT projector takes the form,
[](){ #eq_GWTTprojector }
```math
\begin{align}
\label{eq_GWTTprojector}
\Lambda_{ij,lm}(\hat{\bf k})
\equiv
P_{il}(\hat{\bf k})P_{jm}(\hat{\bf k})
-{1\over2}P_{ij}(\hat{\bf k})P_{lm}(\hat{\bf k})\,,\quad\quad \text{with}\quad\quad P_{ij}(\hat{\bf k})
\equiv
\delta_{ij}-\hat k_i\hat k_j\,,
\end{align}
```
and $\hat k_i\equiv {k_i\over k}$. This guarantees the projected tensor is both transverse, $k_i\Pi_{ij}^{\rm TT}=0$, and traceless, $\Pi_{ii}^{\rm TT}=0$.

Finally, it is worth mentioning GW observables. The most relevant quantity related to GWs is the energy density of the GW background,
[](){ #eq_GWrhoContinuum }
```math
\begin{align}
\label{eq_GWrhoContinuum}
\rho_{\rm GW}(\eta)
&=
{m_p^2\over 4a^{2\alpha}V}
\int_V d^3{\bf x}\,
h'_{ij}({\bf x},\eta)h'_{ij}({\bf x},\eta) \nonumber
\\
&\simeq
{m_p^2\over 4a^{2\alpha}V}
\int_V {d^3{\bf k}\over(2\pi)^3}
h'_{ij}({\bf k},\eta)h_{ij}^{\prime *}({\bf k},\eta) \nonumber
\\
&\equiv
\int {d\rho_{\rm GW}\over d\log k}d\log k \,,
\end{align}
```
from which the spectral density can be defined as
[](){ #eq_GWrhoSpectrumContinuum }
```math
\begin{align}
\label{eq_GWrhoSpectrumContinuum}
\left({d\rho_{\rm GW}\over d\log k}\right)(k,\eta)
=
{m_p^2 k^3\over 8\pi^2a^{2\alpha}V}
\int {d\Omega_k\over4\pi}\,
h'_{ij}(\hat{\bf k},k,\eta)h_{ij}^{\prime *}(\hat{\bf k},k,\eta)\,.
\end{align}
```
For stochastic sources the volume average can be replaced by an ensemble average $\langle...\rangle$ over the independent realizations of the tensor fluctuations,
[](){ #eq_stochasticPS }
```math
\begin{align}
\rho_{\rm GW}(\eta)
&= \dfrac{m_p^2}{4a^{2\alpha}}
\left\langle h'_{ij}({\bf x},\eta) h_{ij}^{\prime *}({\bf x},\eta)\right\rangle
\nonumber \\
&= \dfrac{m_p^2}{4a^{2\alpha}}
\int \dfrac{\text{d}^3{\bf k}}{(2\pi)^3}
\dfrac{\text{d}^3{\bf k'}}{(2\pi)^3}
e^{-i {\bf x}\cdot({\bf k} - {\bf k'})}
\left\langle h'_{ij}({\bf k},\eta) h_{ij}^{\prime *}({\bf k'},\eta)\right\rangle
\nonumber \\
&\equiv
\dfrac{m_p^2}{8\pi^2a^{2\alpha}}
\int\dfrac{\text{d}k}{k} k^3 P_{h'}(k,\eta)\,,
\label{eq_stochasticPS}
\end{align}
```
where we have introduced the power spectrum of the time derivative of $h_{ij}$,
[](){ #eq_stochasticPS_2 }
```math
\begin{align}
\left\langle h'_{ij}({\bf k},\eta)h_{ij}^{\prime *}({\bf k'},\eta) \right\rangle
=
(2\pi)^3 P_{h'}(k,\eta)\delta^{(3)}({\bf k} - {\bf k'})\,.
\label{eq_stochasticPS_2}
\end{align}
```
In addition to the energy density, one usually also defines the fractional GW energy density power spectrum as
[](){ #eq_GWOmegaContinuum }
```math
\begin{align}
\label{eq_GWOmegaContinuum}
\Omega_{\rm GW}(k,\eta)
\equiv
{1\over\rho_\text{c}}{d\rho_{\rm GW}\over d\log k}
=
{k^3\over 24\pi^2\mathcal H^2}P_{h'}(k,\eta)
=
{k^3\over 24\pi^2a^{2\alpha}H^2}P_{h'}(k,\eta)\,.
\end{align}
```
where $\rho_\text{c}=3m_p^2 H^2$ is the critical energy density.
