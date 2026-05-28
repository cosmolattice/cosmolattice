This is the (online) user manual for $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, a modern package for lattice simulations of the dynamics of interactive fields in an expanding Universe. **This manual focuses on explaining how to use $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and not on the theory and/or techniques behind the code**. The theoretical basis for the equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can be found, instead, in our monographic series on latttice techniques, *"The art of simulating the early Universe"*, 

*Part I. Integration techniques and canonical cases.* (Ref. [@Figueroa_2020rrl])

*Part II. Non-canonical cases and gravitational waves.* (Ref. [@BaezaBallesteros_2025tme]) 

*Part III. Scalar-Gauge-Fluid dynamics.* (Ref. [@Figueroa_2026XYZ]) 

which will be refered to, colloquially, as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$ and $\mathtt{The~Art-III}$ monographs. <!--with $\mathtt{X = I, II, III, ...}$   These references  explain the motivation(s) that sustain the equations and techniques used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.--> <!--, particularly for the less experienced user.-->  Reading Refs. [@Figueroa_2020rrl],[@BaezaBallesteros_2025tme],[@Figueroa_2026XYZ] is however not necessary in order to follow this manual, which is self-contained. Whenever lattice methods or theoretical results <!-- from Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] --> are quoted in the manual without further explanation, the user will be referred to the corresponding part of the monograph series, for appropriate clarification and/or demonstration. Having these monographs at hand, might therefore prove to be useful, so a brief description of their content <!-- of our lattice theory monographs -- $\mathtt{The~Art-I}$, $\mathtt{-II}$ and $\mathtt{-III}$ -- --> is given in [**Monographic Reviews**](../th_framework/MonographicReviews.md), inside the [**Theoretical Framework**](../th_framework/TF_readme.md) tab.

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is an ever-evolving package, and sucessive improved versions are continuously developed. We typically release a new version publicly whenever either of the following aspects has been added: code algorithm improvements, new lattice methods, or new physics modules. Sucessive versions of the code always maintain previous lattice methods and physics modules, whereas algorithmic improvements might however supersede (and hence substitute) previous parts of the code. The latter typically concern the very internal tripes of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, as *e.g.* the Fourier transform or internal communication between cores in a cluster, which the majority of users will never touch. <!-- While the more-in-depth  --> Details of the different versions of the code can be found in the **$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Versions Guide**. This manual can be approached, in any case, independently of the version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ that the reader is using (or intending to use). Most sections of the manual are common to all $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ versions, and whenever new physics modules and/or features presented in the manual require specific versions of the code, this will be clearly specified. We recommend to download and work always with the latest version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ (*version 2.0* at the time of writing, May 2026).

<!-- a brief summary of the code versions is provided below: 

**Version 1.0**: Original release of the code (Feb. 2nd 2021), it is written in {\tt C++} and based on {\tt MPI}, and uses discrete Fourier transforms parallelized either in one (FFTw3) or multiple (PFFT) spatial dimensions. It can simulate any model characterized by a scalar potential and a set of scalar fields, either singlets, or charged under $U(1)$ and/or $SU(2)$ (and hence interacting with the correspoing $U(1)$ and/or $SU(2)$ gauge fields). 

**Version 1.1**: 

**Version 1.2**: 

**Version 2.0**: -->



-----------------

The present manual is structured as follows:

<!-- Common to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$: -->

-  Section [Overview](Overview.md) provides an overview on $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, introducing the file structure of the code, and the continuum version of the field equations, as well as features/capabilities of the code, that sucessive versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ have gone incorporating in time. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and we strongly recommend newcomers to read it.  
 
 <!--
-  In Section [Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$](Introduction to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.md) we briefly review physically-motivated scenarios suitable for lattice simulations, we introduce the file structure of the code, and we present the basic field equations that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is ready to solve.

-  In Section [Brief review on lattice techniques](Brief review on lattice techniques.md) we review basic but necessary concepts on lattice techniques, including lattice gauge invariant field theory formulations. 
-->

-  Section [Scalar Field Dynamics](My first model of (singlet) scalar fields.md) presents all necessary steps to run an example model with interacting singlet scalar fields. This chapter is particularly relevant for a newbie, as we introduce the concept of **program variables**, relevant for choosing appropriate re-scalings of both field and space-time variables, and we review how to to compile and run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, how to define a new model, how to introduce the different parameters of the simulation, and how to interpret the output produced by the code. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. 
   
 <!--
  space
-->

-  Section [Scalar-Gauge Dynamics](My first model of gauge fields.md) explains how to simulate models with scalar fields interacting among themselves and charged under a U(1) gauge symmetry (and hence interacting also with Abelian gauge fields), and under a SU(2) gauge symmetry [and thus interacting also with non-Abelian SU(2) gauge fields]. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. 
   
 <!--
  space
-->

 <!--
-  Section [Non-Abelian $SU(2)$ Scalar-Gauge Dynamics](My first model of gauge fields.md) explains how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can simulate models with scalar fields interacting among themselves and charged under a SU(2) gauge symmetry, and hence interacting also with non-Abelian SU(2) gauge fields. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. 
   -->

 <!--
  space
-->

-  Section [Axion-Gauge Dynamics](Axion-Gauge.md) discusses how to simulate scenarios in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ where there are an axion-like particle (ALP) and an Abelian gauge sector, interacting through the coupling $\phi F\tilde F$. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

   
 <!--
  space
-->

-  Section [Cosmic Defects](Defects.md) discusses how the creation of cosmic defects can be studied with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. We explain techniques to accelerate the achievement of {\it scaling} in a cosmic defect network (in particular for cosmic strings and domain walls), and also introduce specific observables for each type of defect. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.
   
 <!--
  space
-->

-  Section [Non-minimal Scalar Fields Dynamics](NMC.md) expands over previous sections on scalar fields, explaining how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can also deal with non-canonical interactions of scalar fields, either non-minimally coupled to gravity via $\phi^2 R$, or with non-minimal kinetic terms, $\mathcal{G}_{ab}\partial_{\mu}\phi_a \partial^{\mu}\phi_b$. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.
   
 <!--
  space
-->

-  Section [Gravitational Wave Dynamics](GW.md) explains how to use the gravitational wave (GW) module of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, so that the production of GWs can be  computed in simulations with scalar and/or gauge fields. While this section is suitable for *versions 1.1, 1.2* and *1.3* of the code, we recommend to use instead $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.
   
 <!--
  space
-->


-  Section [Initial Conditions](IC.md) explains how to set up the initial condition of the different fields that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. This includes arbitrary spectra of scalar and/or gauge fields, and in the case of scalar-gauge theories ([Abelian $U(1)$ Scalar-Gauge Dynamics](My first model of gauge fields.md) and [Non-Abelian $SU(2)$ Scalar-Gauge Dynamics](My first model of gauge fields.md)). This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, though different features or field content requires different code versions, as we will indicate in each case.

   
 <!--
  space
-->

-  Section [Simulations in (2+1) and (1+1) dimensions](2dSims.md) explains how to simulate scalar field dynamics in (2+1) and (1+1) dimensions. This section requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

   
 <!--
  space
-->


-  Section [What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail](What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail.md) elaborates on the physics captured by $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including details on how fields are initialized, how the equations of motion are solved, and what are the relevant observables that can be measured in a run. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. 
   
 <!--
  space
-->

-  Section [Useful features: parallel support, backing up and others](Useful features: parallel support, backing up and others.md) describes some of the technical features implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including its parallel support and back-up options. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. 
    
 <!--
  space
-->

The manual is also complemented with few appendices: 

- [Installation](Installation.md) describes the installation process in detail, of both $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ itself and of different tools and libraries that it uses (some of them compulsory, others optional). 

 <!--
  space
-->

- [Appendix: Parameters](Appendix: Parameters.md), [Appendix: Generic Model variables](Appendix: Generic Model variables.md), [Appendix: CMake Flags](Appendix: CMake Flags.md), and [List of Implemented Functions](List of Implemented Functions.md), which contain, respectively, a list of the most relevant parameters, variables, functions, and CMake flags, used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.










