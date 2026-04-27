This is the manual for $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, a modern package for lattice simulations of the dynamics of interactive fields in an expanding Universe. **This manual focuses on explaining how to use $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and not on the theory and/or techniques behind the code**. The theoretical basis for the equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can be found in our monographic series on latttice techniques: 

*The art of simulating the early Universe. Part I. Integration techniques and canonical cases.* (Ref. [@Figueroa_2020rrl]), 


*The art of simulating the early Universe, Part II. Non-canonical cases and gravitational waves.* (Ref. [@Baeza-Ballesteros:2025tme]), 

*The art of simulating the early Universe, Part III. Scalar-Gauge-Fluid dynamics.* (Ref. [@Figueroa_2026XYZ]), 


which, from now on,  will be referred to, colloquially, as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$ and $\mathtt{The~Art-III}$ monographs. <!--with $\mathtt{X = I, II, III, ...}$  --> These references will help the reader to understand the motivation(s) that sustain the equations and techniques used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. <!--, particularly for the less experienced user.-->  The manual is in any case self-contained, and reading Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] is not strictly required in order to follow it. However, whenever needed, lattice methods or theoretical results <!-- from Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] --> will be quoted in the manual without further clarification, and the user will be referred to the appropriate explanation and/or derivation at the corresponding section/s of the monograph series. Having these monographs at hand might therefore prove useful, so a brief description of their content <!-- of our lattice theory monographs -- $\mathtt{The~Art-I}$, $\mathtt{-II}$ and $\mathtt{-III}$ -- --> can be found below in the **Theory Monographs Guide**.

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is an ever-evolving package, and sucessive improved versions are continuously developed. We typically release a new version publicly whenever either of the following aspects has been added: code algorithm improvements, new lattice methods, or new physics modules. Sucessive versions of the code always maintain previous lattice methods and physics modules, whereas algorithmic improvements might however supersede (and hence substitute) previous parts of the code (these typically concern the very internal tripes of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, as *e.g.* the Fourier transform or internal communication between cores in a cluster, that the majority of users will not touch). <!-- While the more-in-depth  --> Details of the different code versions can be found in the **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Versions Guide**. 

<!-- a brief summary of the code versions is provided below: 

**Version 1.0**: Original release of the code (Feb. 2nd 2021), it is written in {\tt C++} and based on {\tt MPI}, and uses discrete Fourier transforms parallelized either in one (FFTw3) or multiple (PFFT) spatial dimensions. It can simulate any model characterized by a scalar potential and a set of scalar fields, either singlets, or charged under $U(1)$ and/or $SU(2)$ (and hence interacting with the correspoing $U(1)$ and/or $SU(2)$ gauge fields). 

**Version 1.1**: 

**Version 1.2**: 

**Version 2.0**: -->



-----------------

The present manual is structured as follows:

-  In Section [Overview](Overview.md) we provide an overview on $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, introduce the file structure of the code, and present (the continuum version of) the field equations and features that sucessive versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ have gone incorporating in time.   
 <!--
-  In Section [Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$](Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.md) we briefly review physically-motivated scenarios suitable for lattice simulations, we introduce the file structure of the code, and we present the basic field equations that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to solve.

-  In Section [Brief review on lattice techniques](Brief review on lattice techniques.md) we review basic but necessary concepts on lattice techniques, including lattice gauge invariant field theory formulations. 
-->

-  In Section [Scalar Field Dynamics](My first model of (singlet) scalar fields.md) we present all necessary steps to run an example model with interacting singlet scalar fields. This chapter is particularly relevant for a newbie, as we introduce the concept of **program variables**, relevant for choosing appropriate re-scalings of both field and space-time variables, and we review how to to compile and run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, how to define a new model, how to introduce the different parameters of the simulation, and how to interpret the output produced by the code. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.
   
 <!--
  space
-->
-  In Section [Abelian $U(1)$ Scalar-Gauge Dynamics](My first model of gauge fields.md) we explain how to simulate models with scalar fields interacting among themselves and charged under a U(1) gauge symmetry, and hence (gauge-)coupled to Abelian gauge fields. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.
   
 <!--
  space
-->

-  In Section [Non-Abelian $SU(2)$ Scalar-Gauge Dynamics](My first model of gauge fields.md) we explain how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can simulate models with scalar fields interacting among themselves and charged under a SU(2) gauge symmetry, and hence (gauge-)coupled to non-Abelian SU(2) gauge fields. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.
   
 <!--
  space
-->

-  In Section [Axion-Gauge Dynamics](Axion-Gauge.md) we discuss the lattice formulation in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ of the interaction of an axion-like particle (ALP) with an Abelian gauge sector through the coupling $\phi F\tilde F$. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

   
 <!--
  space
-->

-  In Section [Cosmic Defects](Defects.md) we discuss how the creation of cosmic defects can be studied with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. We explain techniques to accelerate the achievement of {\it scaling} in a cosmic defect network (in particular for cosmic strings and domain walls), and also introduce specific observables for each type of defect. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.
   
 <!--
  space
-->

-  In Section [Non-minimal Scalar Fields Dynamics](NMC.md) we expand over the previous section on singlet scalar fields, explaining how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ also deals with lattice formulations of non-canonical interactions of scalar fields, either non-minimally coupled to gravity via $\phi^2 R$, or with non-minimal kinetic terms, $\mathcal{G}_{ab}\partial_{\mu}\phi_a \partial^{\mu}\phi_b$. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.
   
 <!--
  space
-->

-  In Section [Gravitational Wave Dynamics](GW.md) we .... While this section is suitable for $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *versions 1.1, 1.2* and *1.3*, we actually recommend to use instead $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.
   
 <!--
  space
-->


-  In Section [Initial Conditions](IC.md) we . Different initial considitions require different versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

   
 <!--
  space
-->

-  In Section [Simulations in (2+1) and (1+1) dimensions](2dSims.md) we .... Different initial considitions require different versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. This section requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.


   
 <!--
  space
-->


-  In Section [What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail](What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail.md) we elaborate on the physics captured by $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including details on how fields are initialized, how the equations of motion are solved, and what are the relevant observables that can be measured in a run. 
   
 <!--
  space
-->
-  In Section [Useful features: parallel support, backing up and others](Useful features: parallel support, backing up and others.md) we describe some of the technical features implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including its parallel support and back-up options.
    
 <!--
  space
-->

The manual is also complemented with few appendices: 

- [Installation](Installation.md) describes the installation process in detail, of both $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ itself and of different tools and libraries that it uses (some of them compulsory, others optional). 

- The sections [Appendix: Parameters](Appendix: Parameters.md), [Appendix: Generic Model variables](Appendix: Generic Model variables.md), [Appendix: CMake Flags](Appendix: CMake Flags.md), and [List of Implemented Functions](List of Implemented Functions.md) contain, respectively, a list of the most relevant parameters, variables, functions, and CMake flags, used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

------

** Theory Monographs Guide. ** The monographs [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] provide comprehensive discussions of lattice methods for scalar–gauge field theories in an expanding universe ($\mathtt{The~Art-I}$ [@Figueroa_2020rrl]), gravitational waves and non-canonical aspects of field theory - including interactions, initial conditions, dimensionality - ($\mathtt{The~Art-II}$ [@Baeza-Ballesteros:2025tme]), and fluid dynamics as a description of an ensemble of interacting scalar, gauge, and fermion fields ($\mathtt{The~Art-III}$ [@Figueroa_2026XYZ]). 

We recommend users without previous experience on lattice techniques, that before diving into the code, they read at least sections 2, 3 and 4 of Ref. [@Figueroa_2020rrl], in order to have a better understanding of the basic lattice concepts used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. In particular, Section 2 of [@Figueroa_2020rrl] reviews the formulation of scalar and gauge field interactions in the continuum, both in flat and *Friedmann-Lemâitre-Robertson-Walker* (FLRW) backgrounds, whereas Section 3 of [@Figueroa_2020rrl] introduces the basic tools for discretizing any bosonic field theory in an expanding background, including a discussion on *lattice gauge-invariant* techniques for both *Abelian* and *non-Abelian* gauge theories. Section 4 of Ref. [@Figueroa_2020rrl] describes the formulation and properties of the evolution algorithms used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate the dynamics of interacting singlet scalar fields. If the user wants to run simulations of the dynamics of Abelian and non-Abelian scalar-gauge theories, we recommend them to read sections 5 and 6 of Ref. [@Figueroa_2020rrl], as we discuss there in detail the different evolution algorithms implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ for dealing with gauge theories.







