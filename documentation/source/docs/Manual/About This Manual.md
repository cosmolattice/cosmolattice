This is the manual for $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, a modern package for lattice simulations of the dynamics of interactive fields in an expanding Universe. The theoretical basis for the equations implemented in the code can be found in our monographic series on latttice techniques: 

*The art of simulating the early Universe -- Part I* (Ref. [@Figueroa_2020rrl]), *Part II* (Ref. [@Baeza-Ballesteros:2025tme]), and *Part III* (Ref. [@Figueroa_2026XYZ])   

<!--
*The art of simulating the early Universe, Part II* (Ref. [@Baeza-Ballesteros:2025tme]) 

*The art of simulating the early Universe, Part III* (Ref. [@Figueroa_2026XYZ]) 
-->

Colloquially, we refer to these references as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$ and $\mathtt{The~Art-III}$ monographs. <!--with $\mathtt{X = I, II, III, ...}$  --> These monographic reviews will help the reader to understand the motivation(s) that sustain the equations and techniques used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. <!--, particularly for the less experienced user.--> **The focus of this manual, however, is on explaining how to use $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and not on the theory behind or on the lattice techniques**. The manual is actually self-contained, and reading Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] is not strictly required in order to follow it. However, whenever needed, lattice methods or theoretical results <!-- from Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] --> will be quoted in the manual without further clarification, as the user will be referred for the appropriate explanation and/or derivation to the relevant section/s in the monographs. Having the monographs at hand might therefore prove useful, so a brief description of their content <!-- of our lattice theory monographs -- $\mathtt{The~Art-I}$, $\mathtt{-II}$ and $\mathtt{-III}$ -- --> can be found below in the Theory Monographs Guide.


$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *Versions* :


-----------------

The present manual is structured as follows:

-  In Section [Overview](Overview.md) we provide an overview on $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, introduce the file structure of the code, and present (the continuum version of) the field equations that sucessive versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ have gone incorporating in time.   
 <!--
-  In Section [Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$](Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.md) we briefly review physically-motivated scenarios suitable for lattice simulations, we introduce the file structure of the code, and we present the basic field equations that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to solve.

-  In Section [Brief review on lattice techniques](Brief review on lattice techniques.md) we review basic but necessary concepts on lattice techniques, including lattice gauge invariant field theory formulations. 
-->

-  In Section [Scalar Field Dynamics](My first model of (singlet) scalar fields.md) we present all necessary steps to run an example model with interacting scalar (singlet) fields. We introduce the important concept of *program variables*, corresponding to appropriate re-scaling of the scalar field amplitudes and space-time variables, so that a given scenario can be simulated in a computer. We also explain there the basic commands to compile and run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, how to define a new model, how to introduce the different parameters of the simulation, and how to interpret the output produced by the code. 

-  In Section [Abelian $U(1)$ Scalar-Gauge Dynamics](My first model of gauge fields.md) we expand over the previous section on singlet scalar fields, explaining now how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can also simulate  models with scalar fields interacting among themselves and charged under U(1) or SU(2) gauge symmetries, and hence (gauge-)coupled to Abelian and non-Abelian gauge fields. 

-  In Section [Non-Abelian $SU(2)$ Scalar-Gauge Dynamics](My first model of gauge fields.md) we expand over the previous section on singlet scalar fields, explaining now how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can also simulate  models with scalar fields interacting among themselves and charged under U(1) or SU(2) gauge symmetries, and hence (gauge-)coupled to Abelian and non-Abelian gauge fields. 

-  In Section [Axion-Gauge Dynamics](Axion-Gauge.md) we discuss our lattice formulation in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ of the interaction of an axion-like particle (ALP) with an Abelian gauge sector, through the coupling $\phi F\tilde F$. We explain how to build a proper lattice representation of $F\tilde F$ as a total derivative, and discuss the notion of chirality on the lattice.

-  In Section [Cosmic Defects](Defects.md) we discuss how the creation of cosmic defects can be studied with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. We explain techniques to accelerate the achievement of {\it scaling} in a cosmic defect network (in particular for cosmic strings and domain walls), and also introduce specific observables for each type of defect.

-  In Section [Non-minimal Scalar Fields Dynamics](NMC.md) we expand over the previous section on singlet scalar fields, explaining how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ also deals with lattice formulations of non-canonical interactions of scalar fields, either non-minimally coupled to gravity via $\phi^2 R$, or with non-minimal kinetic terms, $\mathcal{G}_{ab}\partial_{\mu}\phi_a \partial^{\mu}\phi_b$. 

-  In Section [Initial Conditions](IC.md) we . 

-  In Section [What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail](What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail.md) we elaborate on the physics captured by $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including details on how fields are initialized, how the equations of motion are solved, and what are the relevant observables that can be measured in a run. 

-  In Section [Useful features: parallel support, backing up and others](Useful features: parallel support, backing up and others.md) we describe some of the technical features implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including its parallel support and back-up options.
 
The manual is also complemented with few appendices: 

- [Installation](Installation.md) describes the installation process in detail, of both $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ itself and of different tools and libraries that it uses (some of them compulsory, others optional). 

- The sections [Appendix: Parameters](Appendix: Parameters.md), [Appendix: Generic Model variables](Appendix: Generic Model variables.md), [Appendix: CMake Flags](Appendix: CMake Flags.md), and [List of Implemented Functions](List of Implemented Functions.md) contain, respectively, a list of the most relevant parameters, variables, functions, and CMake flags, used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

------

** Theory Monographs Guide. ** The monographs [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] provide comprehensive discussions of lattice methods for scalar–gauge field theories in an expanding universe ($\mathtt{The~Art-I}$ [@Figueroa_2020rrl]), gravitational waves and non-canonical aspects of field theory - including interactions, initial conditions, dimensionality - ($\mathtt{The~Art-II}$ [@Baeza-Ballesteros:2025tme]), and fluid dynamics as a description of an ensemble of interacting scalar, gauge, and fermion fields ($\mathtt{The~Art-III}$ [@Figueroa_2026XYZ]). 

We recommend users without previous experience on lattice techniques, that before diving into the code, they read at least sections 2, 3 and 4 of Ref. [@Figueroa_2020rrl], in order to have a better understanding of the basic lattice concepts used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. In particular, Section 2 of [@Figueroa_2020rrl] reviews the formulation of scalar and gauge field interactions in the continuum, both in flat and *Friedmann-Lemâitre-Robertson-Walker* (FLRW) backgrounds, whereas Section 3 of [@Figueroa_2020rrl] introduces the basic tools for discretizing any bosonic field theory in an expanding background, including a discussion on *lattice gauge-invariant* techniques for both *Abelian* and *non-Abelian* gauge theories. Section 4 of Ref. [@Figueroa_2020rrl] describes the formulation and properties of the evolution algorithms used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate the dynamics of interacting singlet scalar fields. If the user wants to run simulations of the dynamics of Abelian and non-Abelian scalar-gauge theories, we recommend them to read sections 5 and 6 of Ref. [@Figueroa_2020rrl], as we discuss there in detail the different evolution algorithms implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ for dealing with gauge theories.







