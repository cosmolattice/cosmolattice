This is the user manual for $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, a modern package for lattice simulations of the dynamics of interactive fields in an expanding Universe. **This manual focuses on explaining how to use $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, not on the theory and/or techniques behind the code**. The theoretical basis for the equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can be found, instead, in our monographic series on latttice techniques, *"The art of simulating the early Universe"*: 

<div class="cl-art-grid" markdown>

<div class="cl-art-card" markdown>

*Part I. Integration techniques and canonical cases.* (Ref. [@Figueroa_2020rrl])

</div>

<div class="cl-art-card" markdown>

*Part II. Non-canonical cases and gravitational waves.* (Ref. [@BaezaBallesteros_2025tme])

</div>

<div class="cl-art-card" markdown>

*Part III. Scalar-Gauge-Fluid dynamics.* (Ref. [@Figueroa_2026XYZ])

</div>

</div>

which we refer to, colloquially, as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$ and $\mathtt{The~Art-III}$ monographs. <!--with $\mathtt{X = I, II, III, ...}$   These references  explain the motivation(s) that sustain the equations and techniques used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.--> <!--, particularly for the less experienced user.-->  Reading those monographs is however not necessary in order to follow the manual, which is self-contained. Whenever lattice methods or theoretical results <!-- from Refs. [@Figueroa_2020rrl,@Baeza-Ballesteros:2025tme,@Figueroa_2026XYZ] --> are quoted in this manual without explanation, the user will be referred to the corresponding part(s) of the monographs, where appropriate clarifications and/or demonstrations can be found. Having these monographs at hand, might therefore prove to be useful, so brief descriptions of their content <!-- of our lattice theory monographs -- $\mathtt{The~Art-I}$, $\mathtt{-II}$ and $\mathtt{-III}$ -- --> and links to download them, are given in [**Monographic Reviews**](../th_framework/MonographicReviews.md), inside the [**Theoretical Framework**](../th_framework/TF_readme.md) tab of this website.

??? note "Important Note: CosmoLattice updates & versions"
    $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is an ever-evolving package, and improved features are continuously being added. We typically release publicly a new version of the code whenever either of the following aspects take place: code algorithm improvements, new lattice methods, or new physics modules. Sucessive versions of the code with new lattice methods and/or new physics modules, will always maintain previous lattice methods and physics modules, simply adding the new ingredients. However, updated versions with new algorithmic improvements, might supersede (and hence substitute) previous parts of the code. The latter typically concern the very internal tripes of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, as *e.g.* the Fourier transform or internal communication between cores in a cluster, which the majority of users will never touch. Details of the different versions of the code can be found in the [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Version Guide**](../code/Versions.md). This manual can be approached, in any case, independently of the version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ that the reader is using or intending to use. Most sections of the manual are common to all $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ versions, and whenever new physics modules and/or features presented in the manual require specific versions of the code, this will be clearly specified. We recommend, at any rate, to download and work always with the latest version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ (currently *version 2.0*, released on July 2026).


-----------------

The present manual is structured as follows:

<div class="cl-prose-grid cl-chapters" markdown>

<div class="cl-prose-card cl-start cl-v-all" markdown>

**[Introduction](Introduction to CosmoLattice.md)**
{: .cl-minihead }

Provides an overview on $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, introducing the file structure of the code, and the continuum version of the field equations, as well as features/capabilities of the code, that sucessive versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ have gone incorporating in time. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and **we strongly recommend newcomers to read it**.

</div>

<div class="cl-prose-card cl-start cl-ch-scalar cl-v-all" markdown>

**[Scalar-Scalar Interactions](My first model of (singlet) scalar fields.md)**
{: .cl-minihead }

Presents all necessary steps to run an example model with interacting singlet scalar fields. **This chapter is particularly relevant for a newbie**, as we introduce the concept of *program variables*, relevant for choosing appropriate re-scalings of both field and space-time variables, and we review how to to compile and run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, how to define a new model, how to introduce the different parameters of the simulation, and how to interpret the output produced by the code. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

</div>

<div class="cl-prose-card cl-ch-u1 cl-v-all" markdown>

**[Scalar-Gauge Dynamics](My first model of gauge fields.md)**
{: .cl-minihead }

Explains how to simulate models with scalar fields interacting among themselves and charged under a U(1) gauge symmetry (and hence interacting also with Abelian gauge fields), and under a SU(2) gauge symmetry [and thus interacting also with non-Abelian SU(2) gauge fields]. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

</div>

<div class="cl-prose-card cl-ch-alp cl-v-20" markdown>

**[Axion-Gauge Dynamics](ALP.md)**
{: .cl-minihead }

Discusses how to simulate scenarios in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ where there are an axion-like particle (ALP) and an Abelian gauge sector, interacting through the coupling $\phi F\tilde F$. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

</div>

<div class="cl-prose-card cl-ch-defects cl-v-20" markdown>

**[Cosmic Defects](Defects.md)**
{: .cl-minihead }

Discusses how the creation of cosmic defects can be studied with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. We explain techniques to accelerate the achievement of *scaling* in a cosmic defect network (in particular for cosmic strings and domain walls), and also introduce specific observables for each type of defect. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

</div>

<div class="cl-prose-card cl-ch-nmc cl-v-20" markdown>

**[Non-minimal Scalar Fields Dynamics](NMC.md)**
{: .cl-minihead }

Expands over previous sections on scalar fields, explaining how $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can also deal with non-canonical interactions of scalar fields, either non-minimally coupled to gravity via $\phi^2 R$, or with non-minimal kinetic terms, $\mathcal{G}_{ab}\partial_{\mu}\phi_a \partial^{\mu}\phi_b$. This sections requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

</div>

<div class="cl-prose-card cl-ch-gw cl-v-20r" markdown>

**[Gravitational Wave Dynamics](GW.md)**
{: .cl-minihead }

Explains how to use the gravitational wave (GW) module of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, so that the production of GWs can be  computed in simulations with scalar and/or gauge fields. While this section is suitable for *versions 1.1, 1.2* and *1.3* of the code, we recommend to use instead $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

</div>

<div class="cl-prose-card cl-v-all" markdown>

**[Initial Conditions](IC.md)**
{: .cl-minihead }

Explains how to set up the initial condition of the different fields that $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. This includes arbitrary spectra of scalar and/or gauge fields, and in the case of scalar-gauge theories ([Abelian $U(1)$ Scalar-Gauge Dynamics](My first model of gauge fields.md) and [Non-Abelian $SU(2)$ Scalar-Gauge Dynamics](My first model of gauge fields.md)). This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, though different features or field content requires different code versions, as we will indicate in each case.

</div>

<div class="cl-prose-card cl-v-20" markdown>

**[Simulations in (2+1) and (1+1) dimensions](Scalars_n_dims.md)**
{: .cl-minihead }

Explains how to simulate scalar field dynamics in (2+1) and (1+1) dimensions. This section requires $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *version 2.0* or above.

</div>

<div class="cl-prose-card cl-v-all" markdown>

**[What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail](What CosmoLattice does in detail.md)**
{: .cl-minihead }

Elaborates on the physics captured by $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including details on how fields are initialized, how the equations of motion are solved, and what are the relevant observables that can be measured in a run. This section applies to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

</div>

<div class="cl-prose-card cl-v-all" markdown>

**[Parallelization](Parallelization.md) & [HDF5: Output and Backups](HDF5: Output and Backups.md)**
{: .cl-minihead }

Describe some of the technical features implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including its parallel support and back-up options. These sections apply to all versions of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

</div>

</div>

The manual is also complemented with few appendices: 

<div class="cl-facts" markdown>
[Installation](../code/Installation.md){ .cl-fact }
[Appendix: Parameters](Appendix_Parameters.md){ .cl-fact }
[Appendix: Generic Model variables](Appendix_Generic_Model_variables.md){ .cl-fact }
[Appendix: CMake Flags](Appendix_CMake_Flags.md){ .cl-fact }
[List of Implemented Functions](List of Implemented Functions.md){ .cl-fact }
</div>

??? note "More about the appendices"

    - [Installation](../code/Installation.md) describes the installation process in detail, of both $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ itself and of different tools and libraries that it uses (some of them compulsory, others optional). 

    - [Appendix: Parameters](Appendix_Parameters.md), [Appendix: Generic Model variables](Appendix_Generic_Model_variables.md), [Appendix: CMake Flags](Appendix_CMake_Flags.md), and [List of Implemented Functions](List of Implemented Functions.md), which contain, respectively, a list of the most relevant parameters, variables, functions, and CMake flags, used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.
