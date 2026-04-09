This is the manual for $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, a modern package for lattice simulations of the dynamics of interactive fields in an expanding Universe. The theoretical basis for the physics equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can be found in our dissertation series on latttice techniques: *The art of simulating the early Universe, Part I* (Ref. [@Figueroa_2020rrl]), *The art of simulating the early Universe, Part II* (Ref. [@Baeza-Ballesteros:2025tme]), and *The art of simulating the early Universe, Part III* (Ref. [@Figueroa_2026XYZ]), which we shall refer to, colloquially, as $\mathtt{The~Art-X}$ monographs, with $X = I, II, III, ...$ <!-- and $\mathtt{The~Art-II}$ and $\mathtt{The~Art-III}$ -->. **The focus of this manual is, instead, on explaining how to use $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$**. Theoretical results from Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] will be quoted in this manual without further clarification, as their explanation and/or derivation can be found in such monographs. The manual is in any case self-contained, and does not require to read reading either of Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] in order to follow it. However, reading those references will certatinly help to grasp better the motivation(s) that sustain the equations and techniques used in the code, particularly for the less experienced user. A brief guide to the monographs' content <!-- of our lattice theory monographs -- $\mathtt{The~Art-I}$, $\mathtt{-II}$ and $\mathtt{-III}$ -- --> can be found below in the Theory Monographs Guide. 

-----------------

The present manual is structured as follows:

-  In Section [Overview](Overview.md) we provide an overview on $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, introduce the file structure of the code, and present (the continuum version of) the field equations that sucessive versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ have gone incorporating in time.   

 <!--
-  In Section [Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$](Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.md) we briefly review physically-motivated scenarios suitable for lattice simulations, we introduce the file structure of the code, and we present the basic field equations that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to solve.

-  In Section [Brief review on lattice techniques](Brief review on lattice techniques.md) we review basic but necessary concepts on lattice techniques, including lattice gauge invariant field theory formulations. 
-->

-  In Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md) we present all necessary steps to run an example model with interacting scalar (singlet) fields. We introduce the important concept of *program variables*, corresponding to appropriate re-scaling of the scalar field amplitudes and space-time variables, so that a given scenario can be simulated in a computer. We also explain there the basic commands to compile and run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, how to define a new model, how to introduce the different parameters of the simulation, and how to interpret the output produced by the code. 


-  In Section [My first model of gauge fields](My first model of gauge fields.md) we expand over the previous section, explaining how to use $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate  models with scalar fields interacting among themselves and charged under U(1) or SU(2) gauge symmetries, and hence coupled to Abelian and non-Abelian gauge fields. 


-  In Section [What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail](What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail.md) we elaborate on the physics captured by $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including details on how fields are initialized, how the equations of motion are solved, and what are the relevant observables that can be measured in a run. 

-  In Section [Useful features: parallel support, backing up and others](Useful features: parallel support, backing up and others.md) we describe some of the technical features implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including its parallel support and back-up options.
 
The manual is also complemented with few appendices: 

- [Installation](Installation.md) describes the installation process in detail, of both $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ itself and of different tools and libraries that it uses (some of them compulsory, others optional). 

- The sections [Appendix: Parameters](Appendix: Parameters.md), [Appendix: Generic Model variables](Appendix: Generic Model variables.md), [Appendix: CMake Flags](Appendix: CMake Flags.md), and [List of Implemented Functions](List of Implemented Functions.md) contain, respectively, a list of the most relevant parameters, variables, functions, and CMake flags, used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

------

** Theory Monographs Guide. ** The monographs [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] provide comprehensive discussions of lattice methods for scalar–gauge field theories in an expanding universe ($\mathtt{The~Art-I}$ [@Figueroa_2020rrl]), gravitational waves and non-canonical aspects of field theory - including interactions, initial conditions, dimensionality - ($\mathtt{The~Art-II}$ [@Baeza-Ballesteros:2025tme]), and fluid dynamics as a description of an ensemble of interacting scalar, gauge, and fermion fields ($\mathtt{The~Art-III}$ [@Figueroa_2026XYZ]). 

We recommend users without previous experience on lattice techniques, that before diving into the code, they read at least sections 2, 3 and 4 of Ref. [@Figueroa_2020rrl], in order to have a better understanding of the basic lattice concepts used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. In particular, Section 2 of [@Figueroa_2020rrl] reviews the formulation of scalar and gauge field interactions in the continuum, both in flat and *Friedmann-Lemâitre-Robertson-Walker* (FLRW) backgrounds, whereas Section 3 of [@Figueroa_2020rrl] introduces the basic tools for discretizing any bosonic field theory in an expanding background, including a discussion on *lattice gauge-invariant* techniques for both *Abelian* and *non-Abelian* gauge theories. Section 4 of Ref. [@Figueroa_2020rrl] describes the formulation and properties of the evolution algorithms used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate the dynamics of interacting singlet scalar fields. 

If the user is inexperienced in the simulation of the dynamics of Abelian and non-Abelian gauge theories, we recommend them to read sections 5 and 6 of Ref. [@Figueroa_2020rrl] before using the code, as we discuss there in detail the different evolution algorithms implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ for dealing with gauge theories. 



