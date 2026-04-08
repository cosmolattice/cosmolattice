HOLA This document is the manual for the code CosmoLattice, a modern package for lattice simulations of the dynamics of interacting scalar and gauge fields in an expanding universe. The present document complements Ref. [@Figueroa_2020rrl] -- *The art of simulating the early Universe* -- , a dissertation meant as a primer on lattice techniques for the simulation of scalar-gauge field theories. The theoretical basis for the equations implemented in CosmoLattice is described in full detail in Ref. [@Figueroa_2020rrl]. The focus of this manual is, instead, on explaining *how* to use CosmoLattice. Therefore, we will just quote results from [@Figueroa_2020rrl] whenever needed, without explaining meticulously their origin and/or derivation. In that regard, we recommend to any user without previous experience on lattice techniques to read first sections 2, 3 and 4 of Ref. [@Figueroa_2020rrl], in order to have a better understanding of the basic lattice concepts used in CosmoLattice. In particular, Section 2 of [@Figueroa_2020rrl] reviews the formulation of scalar and gauge field interactions in the continuum, both in flat and *Friedmann-Lemâitre-Robertson-Walker* (FLRW) backgrounds, whereas Section 3 of [@Figueroa_2020rrl] introduces the basic tools for discretizing any bosonic field theory in an expanding background, including a discussion on *lattice gauge-invariant* techniques for both *Abelian* and *non-Abelian* gauge theories. Section 4 of Ref. [@Figueroa_2020rrl] describes the formulation and properties of the evolution algorithms used in CosmoLattice to simulate the dynamics of interacting singlet scalar fields. Furthermore, if the user is inexperienced in the simulation of the dynamics of Abelian and non-Abelian gauge theories, we recommend them to also read sections 5 and 6 of Ref. [@Figueroa_2020rrl], where we discuss in detail different evolution algorithms implemented in CosmoLattice specialized for gauge theories. This manual is self-contained, so it is not really mandatory to read all the mentioned sections in Ref. [@Figueroa_2020rrl] in order to follow it. However, it will definitely help users to grasp better the motivations sustaining the equations and techniques used in CosmoLattice, particularly to those users with less experience in lattice simulations.

-----------------

The present manual is structured as follows:

-  In Section [Overview](Overview.md) we provide a short overview on CosmoLattice. 
 
-  In Section [Introduction to CosmoLattice](Introduction to CosmoLattice.md) we briefly review physically-motivated scenarios suitable for lattice simulations, we introduce the file structure of the code, and we present the basic field equations that CosmoLattice is ready to solve.

-  In Section [Brief review on lattice techniques](Brief review on lattice techniques.md) we review basic but necessary concepts on lattice techniques, including lattice gauge invariant field theory formulations. 

-  In Section [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md) we present all necessary steps to run an example model with interacting scalar (singlet) fields. We introduce the important concept of *program variables*, corresponding to appropriate re-scaling of the scalar field amplitudes and space-time variables, so that a given scenario can be simulated in a computer. We also explain there the basic commands to compile and run CosmoLattice, how to define a new model, how to introduce the different parameters of the simulation, and how to interpret the output produced by the code. 

-  In Section [My first model of gauge fields](My first model of gauge fields.md) we expand over the previous section, explaining how to use CosmoLattice to simulate  models with scalar fields interacting among themselves and charged under U(1) or SU(2) gauge symmetries, and hence coupled to Abelian and non-Abelian gauge fields. 

-  In Section [What CosmoLattice does in detail](What CosmoLattice does in detail.md) we elaborate on the physics captured by CosmoLattice, including details on how fields are initialized, how the equations of motion are solved, and what are the relevant observables that can be measured in a run. 

-  In Section [Useful features: parallel support, backing up and others](Useful features: parallel support, backing up and others.md) we describe some of the technical features implemented in CosmoLattice, including its parallel support and back-up options.
 
The manual is also complemented with few appendices: 

- [Installation](Installation.md) describes the installation process in detail, of both CosmoLattice itself and of different tools and libraries that it uses (some of them compulsory, others optional). 

- The sections [Appendix: Parameters](Appendix: Parameters.md), [Appendix: Generic Model variables](Appendix: Generic Model variables.md), [Appendix: CMake Flags](Appendix: CMake Flags.md), and [List of Implemented Functions](List of Implemented Functions.md) contain, respectively, a list of the most relevant parameters, variables, functions, and CMake flags, used in CosmoLattice.


