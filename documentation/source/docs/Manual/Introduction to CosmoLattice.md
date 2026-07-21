Here <!-- after discussing how to cite $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ on your research,  --> we first break the ice by presenting an [*Overview*][subsec_OV] on $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, where we comment briefly about the basic features of the code. <!-- in Section [*The Numerical Early Universe*][subsec_EU], physically-motivated early universe scenarios, suitable for lattice simulations.  --> In Section [*Purpose, capabilities and structure of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*][subsec_Purpose] we discuss in more detail the purpose and capabilities of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, introducing also its file structure. In Section [*Basic Field Equations implemented (so far) in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*][subsec_BasicEOM] we present the basic field equations that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to solve. <!-- A reader familiar that has read Section 1 of Ref. [@Figueroa_2020rrl], can skip Section [*The Numerical Early Universe*][subsec_EU] and jump directly into Sections [*Purpose, capabilities and structure of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*][subsec_Purpose] or [*Basic Field Equations implemented (so far) in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*][subsec_BasicEOM]. -->

### Overview { #subsec_OV }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is a program designed to simulate the evolution of interacting fields in an expanding Universe. At the time of writing (July 2026), the public branch of the code, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0*, can simulate 

<div class="cl-sector-chips" markdown>
[*Singlet scalar field dynamics*](My first model of (singlet) scalar fields.md){ .cl-chip .cl-chip-scalar }
[*Abelian $U(1)$ scalar-gauge interactions*](My first model of gauge fields.md){ .cl-chip .cl-chip-u1 }
[*Non-Abelian $SU(2)$ scalar-gauge interactions*](My first model of gauge fields.md){ .cl-chip .cl-chip-su2 }
[*Axion like particle - Abelian $U(1)$ gauge field interactions*](ALP.md){ .cl-chip .cl-chip-alp }
[*Non-minimally coupled-to-gravity scalar field dynamics*](NMC.md){ .cl-chip .cl-chip-nmc }
[*Global cosmic defects (domain walls, strings, monopoles, textures)*](Defects.md){ .cl-chip .cl-chip-defects }
[*Local topological defects (local strings)*](Defects.md){ .cl-chip .cl-chip-defects }
[*Gravitational waves from scalar field theories*](GW.md){ .cl-chip .cl-chip-gw }
[*Gravitational waves from Abelian $U(1)$ scalar-gauge theories*](GW.md){ .cl-chip .cl-chip-gw }
<span class="cl-chip">[...]</span>
</div>

In other words, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can handle scenarios that include interacting singlet scalar fields, charged scalar fields under a $U(1)$ and/or $SU(2)$ gauge symmetry, and the corresponding Abelian and/or non-Abelian gauge vector fields, and also axion-like-particles (ALP) interacting with gauge sectors, non-minimal scalar field dynamics, and soon enough (see below) fluid sectors. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can simulate the dynamics of such fields either in a flat space-time, or in a homogeneous and isotropic (spatially flat) expanding background. In the latter case, the fields can evolve either over a fixed background (e.g. with a power-law scale factor), or with **self-consistent expansion of the Universe, i.e. with the fields involved in a simulation determining themselves the expansion rate throuigh the Friedmann equations**. 

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ provides both symplectic integrators, with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, and non-symplectic integrators with accuracies from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{4})$ (See Sections 3.3, 3.4 and 3.5 of $\mathtt{The~Art-I}$[@Figueroa_2020rrl], for a discussion on integrators and their properties). Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for scalar-gauge theories, either Abelian or non-Abelian, always respect the Gauss constraint to machine precision, independently of the integrator, even in the case of self-consistent expansion. 

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

What type of user you want to be is entirely up to you: it simply depends on your programming expertise (or lack of it), and on your will to learn how the code works internally. We have developed $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ with the intention of providing a new up-to-date, relevant numerical tool for the scientific community working in the physics of the early universe. Presently $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is able to simulate canonical scalar-gauge field theories, non-minimal scalar theories, Abelian gauge-ALP interactions, gravitational waves from scalar and/or gauge fields, etc, *i.e.* the code represents already a powerful tool to tackle many physics scenarios. We have conceived $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ as an evolving package that we plan to upgrade constantly, for instance, by incorporating more evolution algorithms, or more modules dedicated to new physics or to specialized tasks. In that regard, we are constantly developing $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ new modules, which will be made publicly available in due time. For example, to mention just a few, we are currently (2026) working on enhancing the capabilities of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, to also simulate 

<div class="cl-sector-chips" markdown>
<span class="cl-chip cl-chip-upcoming">*Fluid dynamics (fully relativistic and non-relativistic)*</span>
<span class="cl-chip cl-chip-upcoming">*Fluid - Abelian $U(1)$ gauge field interactions*</span>
<span class="cl-chip cl-chip-upcoming">*Fluid - scalar field interactions*</span>
<span class="cl-chip cl-chip-upcoming">*Non-minimal kinetic scalar theories*</span>
<span class="cl-chip cl-chip-upcoming">*Gravitational waves from Non-Abelian $SU(2)$ scalar-gauge theories*</span>
<span class="cl-chip cl-chip-upcoming">*Axion like particle - Non-Abelian $SU(2)$ gauge field interactions*</span>
<span class="cl-chip cl-chip-upcoming">[...]</span>
</div>

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is freely available to anyone who wants to use or modify it, as long as you give us credit for its creation, see **How to cite CL**. If you have any questions or comments about $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, please contact us, as we would love to hear how the program performs for you, and we will be happy to assist you with questions you might have, bug reports, suggestions for future improvements, etc. 

<!--You can download $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ at any time from [ http://www.cosmolattice.net](http://www.cosmolattice.net)-->

!!! note "**Note**"
    If you would like to help developing some aspect of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, or even implement your own modules with some new functionality we have not envisaged, please contact us and let us know about your idea(s). TempLat, which is developed by the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ team, introduces a natural language describing fields and operations between them, so it is a natural platform to implement new libraries (related or not to cosmology).

### Purpose, capabilities and structure of the code { #subsec_Purpose .cl-sec-fold }

Lattice Cosmology has gained significant attention in the recent times, as reflected by the number of specialized LCT packages created over recent years, see *e.g.* `LatticeEasy` [@Felder_2000hq], `ClusterEasy` [@Felder_2007nz],  `Defrost` [@Frolov_2008hy], `CUDAEasy` [@Sainio_2009hm], `HLattice` [@Huang_2011gf],  `PyCOOL` [@Sainio_2012mw], `GABE` [@Child_2013ria], `GABERel` [@Giblin_2019nuv], `GRChombo` [@Andrade_2021rbd], `PSpectRe` [@Easther_2010qz],  `Stella` [@Amin_2018xfe], `Latfield2` [@Daverio_2015ryl] or `GFiRe` [@Lozanov_2019jff]. We expect Lattice Cosmology to become an increasingly influential approach in determining observational strategies to probe the early Universe. It is in this context that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ was originally developed, created purposely to explore the phenomenology and observational implications of non-linearities in field theory early Universe scenarios. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ differs however from the above codes in a number of aspects. To begin with, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to simulate not only the evolution of global scalar and Abelian $U(1)$ gauge theories, but also non-Abelian $SU(2)$ gauge theories. Since $\tt version ~2.0$ it can also simulate scalars with a non-minimal coupling to gravity, $\phi^2R$, and axion-like particle (ALP) interactions with gauge fields, $\phi F_{\mu\nu}\tilde F^{\mu\nu}$; it incorporates methods to set up special field configurations, like cosmic defect networks in *scaling* (*e.g.* cosmic strings and domain walls), or arbitrary field power spectra or spatial profiles; it incorporates non-symplectic evolution algorithms suitable *e.g.* for non-minimal scalar kinetic theories of the type $\mathcal{G}_{ab}(\lbrace\phi_c\rbrace)\partial_\mu\phi^a\partial^\mu\phi^b$; it also allows for optimized gravitational wave dynamics on the lattice sourced by scalar and gauge fields, and scalar field dynamics in $d + 1$ dimensions with $d = 1, 2$. 

Simulating such a rich variety of cases with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is possible because `TempLat`, the lattice engine of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, has been designed as a *platform* to implement any system of dynamical equations suitable for discretization on a lattice: *i.e.* $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is not just meant as a code for one type of simulation, but it is rather a more evolved concept. With `TempLat`, it introduces a **symbolic language**, by defining field variables and operations over them. Once the user becomes familiar with the basic 'vocabulary' of the new language, they can write their own code: let it be for the time evolution of the relevant field variables in a given model of interest, or for some other operation, like e.g. a Monte-Carlo generator for thermal configurations. One of the main advantages of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is that it clearly separates the *physics* (*e.g.* definition of the field content, operations between fields, evolution equations, etc) from the *implementation details* (e.g. parallelization aspects, Fourier transforms, etc). For example, let us consider a beginner user with little experience in programming, and with no experience at all in parallelization techniques. With $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, they will be able to run a fully parallelized simulation of their favourite model (say using hundreds of processors in a cluster), while being completely oblivious to the technical details. They will just need to write a basic *model file* in the language of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, containing the details of the model being simulated. If, on the contrary, the user is rather an experienced programmer and wants to look inside the core routines of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ and `TempLat`, and modify, say the MPI-implementation, they can always do so, and perhaps even contribute to their improvement.

`TempLat` comes with symbolic scalar, complex and $SU(2)$ algebras, which allow to use vectorial and matrix notations without sacrificing performance. The code includes also a *library* of basic field theory equations, as well as routines and field-theoretical operations. At the time of writing (July 2026), $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to simulate scenarios that include interacting singlet scalar fields, charged scalar fields under a $U(1)$ and/or $SU(2)$ gauge symmetry, and the corresponding Abelian and/or non-Abelian gauge vector fields, axion-like-particles (ALP) interacting with gauge sectors, non-minimal scalar field dynamics, and soon enough (expected for early 2027) fluid sectors Simulations can be done either in a flat space-time background, or in a homogeneous and isotropic (spatially flat) expanding FLRW background. In the latter case the fields can evolve either over a fixed background (e.g. with a power-law scale factor), or *self-consistently*, i.e. 'dictating' themselves the expansion of the universe as sourced by their volume averaged energy and pressure densities. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ provides symplectic integrators, with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, to simulate the non-linear dynamics of the appropriate fields in comoving two- or three-dimensional lattices. Appropriate observables are also provided for each case, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even down to machine precision in the case of the highest order integrators. Our algorithms for gauge theories, either Abelian or non-Abelian, respect (independently of the integrator) the Gauss constraint to machine precision, even in the case of self-consistent expansion. Furthermore, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can use a discrete Fourier Transform parallelized in multiple spatial dimensions [@Pi13], which makes it a very powerful code for probing physical problems with well-separated scales, running very high resolution simulations, or simply very long ones. All the above aspects constitute clear advantages for using $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ as a platform to implement any scenario desired, over writing your own code from scratch.

As explained above, all the technicalities, such as memory handling or parallelization tasks, remain mostly hidden from a typical user. The `TempLat` library automates these tasks fully, and a standard user will never need to edit.
For example, let us imagine that we have two fields `f` and `g` on a lattice and we want to sum them. Without `TempLat`, we would need to explicitly write a loop that sums the amplitudes of both fields at each node of the lattice. 
Instead, with `TempLat` we can just write `f + g`, and the hidden structure handles the whole operation of summing their values everywhere on the lattice. 
On the other hand, the heart of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, `CosmoInterface`, handles all relevant aspects of the physics of scalar-gauge theories, such as the initialization, evolution equations, or relevant field observables. This makes the physics part of the code easy to understand and well separated from technical details. This separation significantly simplifies the process of writing new operations for your own purposes.

!!! note
    **Note:** `TempLat` is not stored inside the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ source tree. It is an external dependency fetched by CMake into the local build directory. A remarkable feature of `TempLat` is that its operations and parallelization can work in an arbitrary number of spatial dimensions $d$, including $d < 3$ and $d > 3$. This makes it a natural basis for future interfaces dealing with field dynamics on lower- or higher-dimensional lattices. Visit [ https://cosmolattice.net/technicalnotes/](https://cosmolattice.net/technicalnotes/) to check for additional modules incorporated in successive updated versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to run in $d \neq 3$ spatial dimensions.


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


### Basic Field Equations implemented (so far) <!-- in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$  --> { #subsec_BasicEOM .cl-sec-fold }

Let us consider scalar fields of the type

[](){ #eq_ChargedScalars }
```math
\begin{align}
\label{eq_ChargedScalars}
\begin{array}{c|c|c}
{\rm Singlet} & U(1){\rm-charged} & SU(2){\rm-charged~Doublet}
\\\hline
& &\\
\phi \in \mathcal{R}e
&
\varphi \equiv {1\over\sqrt{2}}(\varphi_0+i\varphi_1)
&
\Phi =
\left(
\begin{array}{c}
\varphi^{(0)}\\
\varphi^{(1)}
\end{array}
\right)
=
{1\over\sqrt{2}}
\left(
\begin{array}{c}
\varphi_0+i\varphi_1\\
\varphi_2+i\varphi_3
\end{array}
\right)
\end{array},
\end{align}
```

and define standard *gauge covariant derivatives* $D_{\mu}^{\rm A}\equiv\partial_{\mu}-iQ_Ag_AA_\mu$ and $D_{\mu}\equiv\mathcal{I}D^{\rm A}_\mu-ig_BQ_BB_{\mu}^aT_a$ (here $Q_A$ and $Q_B$ denote the Abelian and non-Abelian charges), and *field strength tensors* $F_{\mu\nu}\equiv\partial_\mu A_\nu-\partial_\nu A_\mu$ and $G_{\mu\nu}\equiv\partial_\mu B_\nu-\partial_\nu B_\mu-i[B_\mu,B_\nu]$, where $A_\mu$ and $B_\mu=B_\mu^aT_a$ are Abelian and non-Abelian gauge fields, $\mathcal{I}$ is the $2\times2$ identity matrix, and $\lbrace T_a\equiv\sigma_a/2\rbrace$ ($a=1,2,3$) are the $SU(2)$ group generators, with $\sigma_a$ the *Pauli matrices*. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is then ready to solve the following type of equations (here written in cosmic time, with $a(t)$ the scale factor):

[](){ #eq_singletEOM }
[](){ #eq_higgsU1EOM }
[](){ #eq_higgsSU2EOM }
[](){ #eq_U1EOM }
[](){ #eq_SU2EOM }
```math
\begin{align}
\label{eq_singletEOM}
\class{cl-tag-scalar}{\ddot{\phi}}
-a^{-2}{\vec\nabla}^{2}\hspace{-1mm}\phi
+3\frac{\dot a}{a}\dot\phi
&=-V_{,\phi},
\\[0.9em]
\label{eq_higgsU1EOM}
\class{cl-tag-u1s}{\ddot{\varphi}}
-a^{-2}{\vec D}_{\hspace{-0.5mm}A}^{2}\varphi
+3\frac{\dot a}{a}\dot\varphi
&=-{1\over2}{\varphi\over|\varphi|}V_{,|\varphi|},
\\[0.9em]
\label{eq_higgsSU2EOM}
\class{cl-tag-su2s}{\ddot{\Phi}}
-a^{-2}{\vec D}^{2}\Phi
+3\frac{\dot a}{a}\dot\Phi
&=-{1\over2}{\Phi\over|\Phi|}V_{,|\Phi|},
\\[0.9em]
\label{eq_U1EOM}
\class{cl-tag-u1g}{\partial_0F_{0i}}
-a^{-2}\partial_jF_{ji}
+\frac{\dot a}{a}F_{0i}
&=J_i^A,
\\[0.9em]
\label{eq_SU2EOM}
\class{cl-tag-su2g}{(\mathcal{D}_0)_{ab}(G_{0i})^b}
-a^{-2}(\mathcal{D}_j)_{ab}(G_{ji})^b
+\frac{\dot a}{a}(G_{0i})^b
&=(J_i)_a,
\end{align}
```

with as many copies as desired of each type of field\footnote{$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ version 1.0 allows one to simulate field theories with one gauge field of each kind, i.e. one Abelian field $A_\mu$ and one non-Abelian field $B_\mu^a$, and only one $SU(2)$ doublet when it couples to an $SU(2)$ gauge field. We are currently testing the possibility of simulations with multiple gauge fields in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, so we will make this option available in a future update of the code.}, and where $V\equiv V(\phi,|\varphi|,|\Phi|)$ is the potential describing the interactions among the scalar fields. The Abelian and non-Abelian currents on the right-hand sides of the gauge-field EOMs, Eqs.$~$\eqref{eq_U1EOM}-\eqref{eq_SU2EOM}, correspond to $J_A^\mu\equiv2g_AQ_A^{(\varphi)}\mathcal{I}m[\varphi^*(D_A^\mu\varphi)]+2g_AQ_A^{(\Phi)}\mathcal{I}m[\Phi^\dagger(D^\mu\Phi)]$ and $J_a^\mu\equiv2g_BQ_B\mathcal{I}m[\Phi^\dagger T_a(D^\mu\Phi)]$. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ guarantees that the constraint equations

[](){ #eq_GaussU1 }
[](){ #eq_GaussSU2 }
```math
\begin{align}
\label{eq_GaussU1}
\class{cl-panel-gauss}{\partial_iF_{0i}}
&=a^2J_0^A,
\\
\label{eq_GaussSU2}
(\mathcal{D}_i)_{ab}(G_{0i})^b
&=a^2(J_0)_a,
\end{align}
```

which represent the $U(1)$ and $SU(2)$ Gauss constraints in an expanding background, are preserved throughout the evolution.

In the case of self-consistent expansion, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ obtains numerically the scale factor $a(t)$ by solving the *Friedmann equation* (here written in cosmic time)

[](){ #eq_Friedmann-full }
```math
\begin{align}
\label{eq_Friedmann-full}
\hspace{0.6cm}
\class{cl-panel-friedmann}{{\ddot a\over a}}
&=
-\frac{1}{6m_p^2}
\left[\bar\rho+3\bar p\right],
\end{align}
```

while checking that the other Friedmann equation—the *Hubble constraint*—also written in cosmic time,

[](){ #eq_HC }
```math
\begin{align}
\label{eq_HC}
\class{cl-panel-hubble}{H^2}
\equiv
\left({\dot a\over a}\right)^2
&=
\frac{\bar\rho}{3m_p^2},
\end{align}
```

is verified throughout the evolution. Here $\bar\rho\equiv\langle\rho\rangle$ and $\bar p\equiv\langle p\rangle$ are the background energy and pressure densities, obtained from a volume average of the local expressions contributed by the matter fields (both scalar and gauge fields),

[](){ #eq_rhoLocal }
[](){ #eq_pLocal }
```math
\begin{align}
\label{eq_rhoLocal}
\rho
&=
K_\phi+K_\varphi+K_\Phi
+G_\phi+G_\varphi+G_\Phi
+K_{U(1)}+G_{U(1)}
+K_{SU(2)}+G_{SU(2)}
+V,
\\
\label{eq_pLocal}
p
&=
K_\phi+K_\varphi+K_\Phi
-\frac{1}{3}(G_\phi+G_\varphi+G_\Phi)
+\frac{1}{3}(K_{U(1)}+G_{U(1)})
+\frac{1}{3}(K_{SU(2)}+G_{SU(2)})
-V,
\end{align}
```

with $V$ the interacting scalar potential, and $K_x$ and $G_x$ the kinetic and gradient energy densities of each field species [for their exact expressions, see Eq. (51) in Ref. [@Figueroa_2020rrl], or, e.g., Eq.$~$\eqref{eq_energy-contrib} in this manual]. This procedure determines the evolution of the background metric of the Universe within a given volume $L^3$, with $L$ the length scale of the simulation box. As long as $L$ is sufficiently large compared to the typical wavelengths excited in the fields, this procedure should lead to a well-defined notion of a homogeneous and isotropic expanding background within the given volume $L^3$ of the box.

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can also allow for a fixed expansion rate of the Universe, with the scale factor given by a power-law function (again written in cosmic time),

[](){ #eq_aFixed }
```math
\begin{align}
\label{eq_aFixed}
\class{cl-panel-fixed}{a(t)}
&=
a(t_*)
\left[
1+\frac{3(1+w)}{2}H(t_*)(t-t_*)
\right]^{\frac{2}{3(1+w)}},
\end{align}
```

where $a(t_*)$ and $H(t_*)$ are the scale factor and Hubble parameter evaluated at the initial time of the simulation $t=t_*$, and $w$ is the constant equation of state of an external fluid sourcing the expansion (assumed to be energetically dominant with respect to the fields actually being simulated). Eq.$~$\eqref{eq_aFixed} acts as an input for the field Eqs.$~$\eqref{eq_singletEOM}-\eqref{eq_SU2EOM}.

We note that the dynamical equations presented above were expressed in cosmic time only for simplicity. In reality, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can solve them using any time variable preferred by the user, such as conformal time. More importantly, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does not solve exactly the continuum differential equations formulated in Eqs.$~$\eqref{eq_singletEOM}-\eqref{eq_SU2EOM} or Eq.$~$\eqref{eq_Friedmann-full}, nor does directly check the differential constraints in Eqs.$~$\eqref{eq_GaussU1}-\eqref{eq_GaussSU2} or Eq.$~$\eqref{eq_HC}$. Instead, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ solves and/or checks a set of finite-difference equations—the *lattice equations*—that approximate the corresponding continuum equations. The lattice equations, and hence their numerical solutions, can reproduce the continuum results with higher or lower accuracy depending on the integrator algorithm on which they are based. Some algorithms can solve lattice equations with numerical solutions that satisfy the lattice constraint equations down to machine precision. For a detailed description of the different numerical integration algorithms, we refer the interested reader to Sections 3.3–3.5 of $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]). Their detailed implementation in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, specialized for the dynamics of singlet scalar fields, Abelian gauge theories, and non-Abelian gauge theories, can be found in Sections 4, 5, and 6 of $\mathtt{The~Art-I}$ (Ref. [@Figueroa_2020rrl]), respectively.