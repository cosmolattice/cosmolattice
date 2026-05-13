Here <!-- after discussing how to cite $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ on your research,  --> we first break the ice by presenting an [*Overview*][subsec_OV] on $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, where we comment briefly about the basic features of the code. <!-- in Section [*The Numerical Early Universe*][subsec_EU], physically-motivated early universe scenarios, suitable for lattice simulations.  --> In Section [*Purpose, capabilities and structure of CosmoLattice*][subsec_Purpose] we discuss in more detail the purpose and capabilities of CosmoLattice, introducing also its file structure. In Section [*Basic Field Equations implemented (so far) in CosmoLattice*][subsec_BasicEOM] we present the basic field equations that CosmoLattice is ready to solve. <!-- A reader familiar that has read Section 1 of Ref. [@Figueroa_2020rrl], can skip Section [*The Numerical Early Universe*][subsec_EU] and jump directly into Sections [*Purpose, capabilities and structure of CosmoLattice*][subsec_Purpose] or [*Basic Field Equations implemented (so far) in CosmoLattice*][subsec_BasicEOM]. -->

### Overview { #subsec_OV }

CosmoLattice is a program designed to simulate the evolution of interacting fields in an expanding Universe. At the time of writing (June 2026), the public branch of the code, CosmoLattice *version 2.0*, can simulate 

1. *Singlet scalar field dynamics*, 
2. *Abelian $U(1)$ scalar-gauge interactions*, 
3. *Non-Abelian $SU(2)$ scalar-gauge interactions*, 
4. *Axion like particle - Abelian $U(1)$ gauge field interactions*, 
5. *Non-minimally coupled-to-gravity scalar field dynamics*, 
6. *Global cosmic defects (domain walls, strings, monopoles, textures)*,
7. *Local topological defects (local strings)*,
8. *Gravitational waves from scalar field theories*, 
9. *Gravitational waves from Abelian $U(1)$ scalar-gauge theories*, 
10. [...] 

In other words, CosmoLattice can handle scenarios that include interacting singlet scalar fields, charged scalar fields under a $U(1)$ and/or $SU(2)$ gauge symmetry, and the corresponding Abelian and/or non-Abelian gauge vector fields, axion-like-particles (ALP) interacting with gauge sectors, non-minimal scalar field dynamics, and soon enough (see below) fluid sectors. CosmoLattice can simulate the dynamics of such fields either in a flat space-time, or in a homogeneous and isotropic (spatially flat) expanding background. In the latter case, the fields can evolve either over a fixed background (e.g. with a power-law scale factor), or with **self-consistent expansion of the Universe, i.e. with the fields involved in a simulation determining themselves the expansion rate throuigh the Friedmann equations**. 

CosmoLattice provides both symplectic integrators, with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, and non-symplectic integrators with accuracies from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{4})$ (See Sections 3.3, 3.4 and 3.5 of $\mathtt{The~Art-I}$[@Figueroa_2020rrl], for a discussion on integrators and their properties). Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for scalar-gauge theories, either Abelian or non-Abelian, always respect the Gauss constraint to machine precision, independently of the integrator, even in the case of self-consistent expansion. 

CosmoLattice is written in C++, and fully exploits the *object oriented  programming* nature of this language, with a modular structure that separates well all the ingredients involved. This allows CosmoLattice to have a clear separation between the physics and the technical implementation details. **The code is designed so that the user can simulate a given scenario with different parameters, without requiring to re-compile each time that parameter values are changed**. <!-- More importantly, the code allows for an easy implementation of new models with either scalar or gauge interactions.  -->

**CosmoLattice is fully parallelized using *Message Passing Interface* (MPI), and uses a discrete Fourier Transform parallelized in multiple spatial dimensions**. This makes it ideal for probing physical problems with well-separated mass/length scales, running very high resolution simulations, or simply shortening the running time of long simulations. **CosmoLattice is actually a general package that defines field variables and their operations, by introducing its own symbolic language. Once you become familiar with the basic ‘vocabulary’ of the CosmoLattice language, editing the code or implementing your own model (resembling how you would write it in the continuum), should become a simple task.**

CosmoLattice can be used at multiple levels of complexity: 

For instance, a **basic level** user, say someone with no experience at all in parallelization techniques and with little to no experience at all on programming, will be able to run fully parallelized simulations of their favourite models (say using hundreds of cores in a cluster), while being completely oblivious to the technical details of the algorithm implementation or parallelization. CosmoLattice automatically prints a collection of relevant observables, such as volume averages, field spectra, or dynamical constraints, which can be used to monitor the evolution of the system. 

However, an **intermediate level** user, say with certain programming experience, will want to modify the type of output CosmoLattice generates, change the initial condition routine (e.g. turn it into a Monte-Carlo generator for thermal configurations), or even design their own evolution algorithms and add these to the family of integrators available in CosmoLattice. 

Finally, **advanced level** users may want to play as well with the hard core inner parts of CosmoLattice, in an attempt to understand or to improve the most advanced technical aspects of the code. This could include e.g. the handling of the parallelization, which is something that typically will remain as a black box for the majority of users. 

What type of user you want to be is entirely up to you: it simply depends on your programming expertise (or lack of it), and on your will to learn how the code works internally. We have developed CosmoLattice with the intention of providing a new up-to-date, relevant numerical tool for the scientific community working in the physics of the early universe. Presently CosmoLattice is able to simulate canonical scalar-gauge field theories, non-minimal scalar theories, Abelian gauge-ALP interactions, gravitational waves from scalar and/or gauge fields, etc, *i.e.* the code represents already a powerful tool to tackle many physics scenarios. We have conceived CosmoLattice as an evolving package that we plan to upgrade constantly, for instance, by incorporating more evolution algorithms, or more modules dedicated to new physics or to specialized tasks. In that regard, we are constantly developing CosmoLattice new modules, which will be made publicly available in due time. For example, to mention just a few, we are currently (2026) working on enhancing the capabilities of CosmoLattice, to also simulate 

1. *Fluid dynamics (fully relativistic and non-relativistic)*, 
2. *Fluid - Abelian $U(1)$ gauge field interactions*, 
3. *Fluid - scalar field interactions*, 
4. *Non-minimal kinetic scalar theories*,
5. *Gravitational waves from Non-Abelian $SU(2)$ scalar-gauge theories*,
6. *Axion like particle - Non-Abelian $SU(2)$ gauge field interactions*,
7. [...]

CosmoLattice is freely available to anyone who wants to use or modify it, as long as you give us credit for its creation, see **How to cite CL**. If you have any questions or comments about CosmoLattice, please contact us, as we would love to hear how the program performs for you, and we will be happy to assist you with questions you might have, bug reports, suggestions for future improvements, etc. 

<!--You can download CosmoLattice at any time from [ http://www.cosmolattice.net](http://www.cosmolattice.net)-->

!!! note
    **Note:** If you would like to help developing some aspect of CosmoLattice, or even implement your own modules with some new functionality we have not envisaged, please contact us and let us know about your idea(s). CosmoLattice introduces a natural language describing fields and operations between them, so it is a natural platform to implement new libraries (related or not to cosmology).

### The *Numerical* Early Universe { #subsec_EU }

The phenomenology of high energy physics in the early universe is vast and very rich, and it is often characterized by non-linear dynamics. The *numerical early universe*, i.e. the study with numerical techniques of high energy non-linear field theory phenomena in the early universe, is an emerging field increasingly gaining relevance, especially as a methodology to assess our experimental capabilities to constrain the physics of this epoch. The details of nonlinear phenomena are often too difficult, when not impossible, to be described by analytic means. In order to fully understand the non-linearities developed in the dynamics of a given scenario, the use of numerical techniques becomes mandatory. The outcome from non-linear early universe phenomena represents, more and more, an important perspective in determining the best observational strategies to probe the unknown physics from this era. It is therefore crucial to develop numerical techniques, as efficient and robust as possible, to simulate these phenomena. Numerical algorithms developed for this purpose must satisfy a number of physical constraints (e.g. energy conservation), and keep the numerical integration errors under control. It is actually useful to develop as many techniques as possible, to validate and double check results from simulations. Only in this way, we will achieve a certain robustness in the predictions of the potentially probeable implications from these phenomena.

It is precisely because we recognize the importance of the above circumstances, that we have created CosmoLattice, a modern code for lattice simulations of scalar-gauge field theories in an expanding universe. CosmoLattice allows for the simulation of the evolution of interacting (singlet) scalar fields, charged scalar fields under U(1) and/or SU(2) gauge groups, and the corresponding associated Abelian and/or non-Abelian gauge fields. CosmoLattice is capable of solving the evolution of such field $dof$'s whenever their dynamics develop large occupation numbers $n_k \gg 1$, so that the fields' quantum nature can be neglected. When such circumstance is met, classical field theory can be used as a powerful tool to solve complicated field dynamics, including the case when non-linear interactions characterize the field evolution, non-perturbative particle production effects are present, or out-of-equilibrium field distributions are developed.

CosmoLattice is particularly suitable for solving the non-linear dynamics of post-inflationary scenarios like e.g. (p)reheating. Preheating scenarios are actually characterized by non-perturbative particle production mechanisms, which cannot be described with standard perturbative quantum field theory techniques: particle species are typically created far away from thermal equilibrium with exponentially growing occupation numbers, so that when they eventually ‘backreact’ onto the system, the dynamics become non-linear from that moment onward. This is the
case of parametric resonance of scalar fields\footnote{If the species created are fermions, non-perturbative particle production can also take place, see e.g. [@Greene_1998nh;@Greene_2000ew;@Peloso_2000hy;@Berges_2010zv], but no resonance can be developed due to Pauli blocking.} either during preheating [@Traschen_1990sw;@Kofman_1994rk;@Shtanov_1994ce;@Kaiser_1995fb;@Khlebnikov_1996mc;@Prokopec_1996rr;@Kofman_1997yn;@Greene_1997fu;@Kaiser_1997mp;@Kaiser_1997hg] or in other circumstances like the non-perturbative decay of the curvaton [@Enqvist_2008be;@Enqvist_2012tc; @Enqvist_2013qba; @Enqvist_2013gwf] or of the Higgs field of the Standard Model [@Enqvist_2013kaa;@Enqvist_2014tta;@Figueroa_2014aya;@Kusenko_2014lra;@Figueroa_2015rqa;@Enqvist_2015sua;@Figueroa_2016dsc]. Similarly, an explosive decay of a field condensate can take place in theories with flat-directions [@Olive_2006uw;@Basboll_2007vt;@Gumrukcuoglu_2008fk]. In theories with spontaneous symmetry breaking, tachyonic effects can also lead to non-perturbative and out-of-equilibrium particle production, like in tachyonic preheating scenarios after hybrid inflation  [@Felder_2000hj;@Felder_2001kt;@Copeland_2002ku;@GarciaBellido_2002aj], or in preheating after hilltop inflation [@Antusch_2015nla;@Antusch_2015vna;@Antusch_2015ziz]. Preheating effects have also been studied in models with gravitationally non-minimal coupled fields [@Bassett_1997az;@Tsujikawa_1999jh;@Tsujikawa_1999iv;@Tsujikawa_1999me;@Ema_2016dny;@Crespo_2019src;@Crespo_2019mmh], and in particular, recently, in multi-field inflation scenarios [@DeCross_2015uza;@DeCross_2016fdz;@DeCross_2016cbs;@Nguyen_2019kbm;@vandeVis_2020qcp].

The presence of gauge fields has also been considered in multiple scenarios. For instance, if a field enjoys a shift-symmetry, then a coupling $\phi F\tilde F$ between such field and some gauge sector is allowed. Particle production in axion-inflation scenarios, where such topological interaction is present, provide an extremely efficient mechanism to reheat the universe, leading to potentially observable phenomenology [@Adshead_2015pva;@Adshead_2016iae;@Figueroa_2017qmv;@Adshead_2018doq;@Cuissa_2018oiw;@Adshead_2019lbr;@Figueroa_2019jsi;@Adshead_2019igv]. Interactions between a singlet inflaton and an Abelian gauge sector, via $f(\phi)F^2$, or a non-Abelian $SU(2)$ gauge sector, via $f(\phi)\rm{Tr  G^2}$, have also been explored in the context of preheating [@Deskins_2013lfx;@Adshead_2017xll]. In Hybrid preheating scenarios, the excitation of gauge fields have also been addressed extensively, both for Abelian and non-Abelian scenarios, obtaining a very rich phenomenology [@Rajantie_2000nj;@Copeland_2001qw;@Smit_2002yg;@GarciaBellido_2003wd;@Tranberg_2003gi;@Skullerud_2003ki;@vanderMeulen_2005sp;@DiazGil_2007dy;@DiazGil_2008tf;@Dufaux_2010cf;@Tranberg_2017lrx]. Preheating via parametric resonance with an inflaton charged under a gauge symmetry, has been also studied in detail in [@Lozanov_2016pac;@Figueroa_2020rrl], both for Abelian $U(1)$ and non-Abelian $SU(2)$ gauge groups. A natural realization of an inflationary set-up where the inflaton is charged under a gauge group is Higgs-Inflation [@Bezrukov_2007ep;@Bezrukov_2010jz], where the SM Higgs is the inflaton. There the electroweak gauge bosons can experience parametric excitation effects during the oscillations of the Higgs after inflation [@Bezrukov_2008ut;@GarciaBellido_2008ab;@Figueroa_2009jw;@Figueroa_2014aya;@Repond_2016sol;@Ema_2016dny;@Sfakianakis_2018lzf]. If the SM Higgs is rather a spectator field during inflation, the post-inflationary explosive non-perturbative decay of the Higgs into SM fields has also been considered [@Figueroa_2015rqa;@Enqvist_2015sua;@Kohri_2016wof;@Figueroa_2017slm;@Ema_2017loe].

The techniques developed for studying nonlinear dynamics of classical fields are actually common to many other non-linear problems in the early universe, like the production of stochastic gravitational wave backgrounds by parametric effects [@Khlebnikov_1997di;@Easther_2006gt;@Easther_2006vd;@GarciaBellido_2007af;@Dufaux_2007pt;@Dufaux_2008dn;@Dufaux_2010cf;@Zhou_2013tsa;@Bethke_2013aba;@Bethke_2013vca;@Antusch_2016con;@Antusch_2017flz;@Antusch_2017vga;@Liu_2018rrt;@Figueroa_2017vfa;@Fu_2017ero;@Lozanov_2019ylm;@Adshead_2019lbr;@Adshead_2019igv;@Armendariz_Picon_2019csc] (for a review on stochastic backgrounds see [@Caprini_2018mtu]), the dynamics of phase transitions [@Hindmarsh_2001vp;@Rajantie_2000fd;@Hindmarsh_2001vp;@Copeland_2002ku;@GarciaBellido_2002aj;@Figueroa_2017hun;@Brandenburg_2017neh;@Brandenburg_2017rnt;@Figueroa_2019jsi] and their emission of gravitational waves [@Hindmarsh_2013xza;@Hindmarsh_2015qta;@Hindmarsh_2017gnf;@Cutting_2018tjt;@Cutting_2019zws;@Pol_2019yex;@Cutting_2020nla;@Di_2020ivg] (for a review on early universe phase transitions see [@Hindmarsh_2020hop]), cosmic defect formation [@Hindmarsh_2000kd;@Rajantie_2001ps;@Rajantie_2002dw;@Donaire_2004gp;@Dufaux_2010cf;@Hiramatsu_2012sc;@Kawasaki_2014sqa;@Fleury_2016xrz;@Moore_2017ond;@Lozanov_2019jff], their later evolution [@Vincent_1997cx;@Bevis_2006mj;@Hindmarsh_2014rka;@Daverio_2015nva;@Lizarraga_2016onn;@Hindmarsh_2018wkp;@Eggemeier_2019khm;@Hindmarsh_2019csc;@Gorghetto_2018myk;@Gorghetto_2020qws;@Hindmarsh_2021mnl] and gravitational wave emission [@Dufaux_2010cf;@Figueroa_2012kw;@Hiramatsu_2013qaa;@Figueroa_2020lvo;@Gorghetto_2021fsn], axion-like field dynamics [@Kolb_1993hw;@Kitajima_2018zco;@Amin_2019ums;@Buschmann_2019icd;@Hindmarsh_2019csc;@Gorghetto_2018myk;@Gorghetto_2020qws;@Hindmarsh_2021vih;@Buschmann_2021sdq], oscillon dynamics [@Amin_2011hj;@Zhou_2013tsa;@Antusch_2016con;@Antusch_2017flz;@Lozanov_2017hjm;@Amin_2018xfe;@Liu_2018rrt;@Kitajima_2018zco;@Lozanov_2019ylm;@Antusch_2019qrr;@Kasuya_2020szy], the post-inflationary evolution of the equation of state [@Podolsky_2005bw;@Lozanov_2016hid;@Figueroa_2016wxr;@Lozanov_2017hjm;@Krajewski_2018moi;@Antusch_2020iyq], moduli dynamics [@Giblin_2017wlo;@Amin_2019qrx], etc. These techniques can also be used in applications of interest not only to cosmology, but also to other high energy physics areas. For example, classical-statistical simulations have been used to compute quantities such as the sphaleron-rate [@Philipsen_1995sg;@Ambjorn_1995xm;@Arnold_1995bh;@Arnold_1996dy;@Arnold_1997yb;@Moore_1997sn;@Bodeker_1998hm;@Moore_1998zk;@Moore_1999fs;@Bodeker_1999gx;@Arnold_1999uy; @Tang_1996qx;@Ambjorn_1997jz;@Moore_2000mx;@DOnofrio_2012phz;@DOnofrio_2015gop], and to study the Abelian [@Buividovich_2015jfa;@Buividovich_2016ulp;@Figueroa_2017hun;@Figueroa_2019jsi;@Mace_2019cqo;@Mace_2020dkp] and non-Abelian [@Akamatsu_2015kau] dynamics associated to the chiral anomaly. They have also been used to study spectral quantities [@Boguslavski_2018beu;@Schlichting_2019tbr], and some properties of the quark-gluon plasma [@Laine_2009dd;@Laine_2013lia;@Panero_2013pla;@Boguslavski_2020tqz].

In summary the study of non-linear dynamics of early universe high-energy phenomena, represents an important emerging and phenomenologically rich field, which will help to determine best our observational strategies to probe the unknown physics from this era. Its study requires the development of appropriate numerical techniques, as efficient and robust as possible, to simulate such phenomena.

### Purpose, capabilities and structure of CosmoLattice { #subsec_Purpose }

A number of public packages for lattice simulations have appeared over the years, mostly dedicated to the simulation of interacting scalar fields, like `LatticeEasy` [@Felder_2000hq], `ClusterEasy` [@Felder_2007nz],  `Defrost` [@Frolov_2008hy], `CUDAEasy` [@Sainio_2009hm], `HLattice` [@Huang_2011gf],  `PyCOOL` [@Sainio_2012mw] and `GABE` [@Child_2013ria], which use finite difference techniques and a FLRW background metric. Other packages are suitable for full general relativistic evolution, like `GABERel` [@Giblin_2019nuv] or the recent `GRChombo` [@Andrade_2021rbd]. Others use pseudo-spectral techniques, like `PSpectRe` [@Easther_2010qz] and `Stella` [@Amin_2018xfe]. `Latfield2` [@Daverio_2015ryl], on the other hand, is a library in C++ designed to simplify writing parallel codes for solving partial differential equations, and hence can be used for field dynamics as long as the users implement their own lattice equations of motion. Finally, `GFiRe` [@Lozanov_2019jff] is a package dedicated to Abelian gauge theories, and even though the code itself has not been made publicly available yet, their algorithm is clearly spelled out in their publication.

CosmoLattice differs from the above codes in a number of aspects. To begin with, CosmoLattice is ready to simulate not only the evolution of global scalar and Abelian $U(1)$ gauge theories, but also non-Abelian $SU(2)$ gauge theories. More importantly, CosmoLattice has been designed as a 'platform' to implement any system of dynamical equations suitable for discretization on a lattice, i.e. CosmoLattice is not just meant as a code for one type of simulation, but it is rather a more evolved concept. It is a package that introduces its own *symbolic language*, by defining field variables and operations over them. Once the user becomes familiar with the basic 'vocabulary' of the new language, they can write their own code: let it be for the time evolution of the relevant field variables in a given model of interest, or for some other operation, like e.g. a Monte-Carlo generator for thermal configurations. One of the main advantages of CosmoLattice is that it clearly separates the $physics$ (e.g. definition of the field content, operations between fields, evolution equations, etc) from the $implementation details$ (e.g. parallelization aspects, Fourier transforms, etc). For example, let us consider a beginner user with little experience in programming, and with no experience at all in parallelization techniques. With CosmoLattice, they will be able to run a fully parallelized simulation of their favourite model (say using hundreds of processors in a cluster), while being completely oblivious to the technical details. They will just need to write a basic *model file* in the language of CosmoLattice, containing the details of the model being simulated. If, on the contrary, the user is rather an experienced programmer and wants to look inside the core routines of CosmoLattice, and modify, say the MPI-implementation, they can always do so, and perhaps even contribute to their improvement.

CosmoLattice comes with symbolic scalar, complex and $SU(2)$ algebras, which allows to use vectorial and matrix notations without sacrificing performances. The code includes also a *library* of basic field theory equations, as well as routines and field-theoretical operations. At the time of writing (Jan 2021), CosmoLattice is ready to simulate scenarios including singlet scalar fields, scalar fields charged under a $U(1)$ and/or $SU(2)$ gauge symmetry, and the corresponding Abelian and/or non-Abelian gauge vector fields. Simulations can be done either in a flat space-time background, or in a homogeneous and isotropic (spatially flat) expanding FLRW background. In the latter case the fields can evolve either over a fixed background (e.g. with a power-law scale factor), or *self-consistently*, i.e. 'dictating' themselves the expansion of the universe as sourced by their volume averaged energy and pressure densities. CosmoLattice provides symplectic integrators, with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$, to simulate the
non-linear dynamics of the appropriate fields in comoving two- or three-dimensional lattices. Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even down to machine precision in the case of the highest order integrators. Our algorithms for gauge theories, either Abelian or non-Abelian, respect always (independently of the integrator) the Gauss constraint to machine precision, even in the case of self-consistent expansion.  Furthermore, CosmoLattice can use a discrete Fourier Transform parallelized in multiple spatial dimensions [@Pi13], which makes it a very powerful code for probing physical problems with well-separated scales, running very high resolution simulations, or simply very long ones. All the above aspects constitute clear advantages for using CosmoLattice as a platform to implement any scenario desired, over writing your own code from scratch.

CosmoLattice is structured in such a way that all the technicalities, such as memory handling or parallelization tasks, remain mostly hidden to a typical user. These are integrated in a set of libraries called `TempLat`, which in principle, a standard user will never need to edit. In `TempLat` we have implemented a new language that can be used to define new fields and operations between them in a natural way. For example, let us imagine that we have two fields `f` and `g` in a lattice and we want to sum them. Without `TempLat`, we would need to explicitly write a loop that sums the amplitudes of both fields at each node of the lattice. Instead, with `TempLat` we can just write `f + g`, and the hidden structure handles the whole operation of summing their values everywhere on the lattice. At the same time, we have developed another collection of libraries called `CosmoInterface`, where all relevant aspects of the physics of scalar-gauge theories are handled, such as the initialization, evolution equations, or relevant field observables. This makes the physics part of the code easy to understand and well separated from technical details. This separation significantly simplifies the process of writing new operations for your own purposes.

The basic folder tree structure of CosmoLattice is the following:
```math
\begin{align}
{\tt cosmolattice}:
\left\lbrace
\begin{array}{l}
{\it CMakeLists.txt}  {\rm[+ other files]}\\
{\tt dependencies} \\
{\tt docs} \\
{\tt src}:\left\lbrace
\begin{array}{l}
{\it cosmolattice.cpp}  {\rm[+ other files]}\\
{\tt cmake} \\
{\tt models} \\
{\tt tests} \\
{\tt include}:\left\lbrace
\begin{array}{l}
{\tt TempLat}:\left\lbrace
\begin{array}{l}
{\it cosmolattice.h}\\
{\tt fft}\\
{\tt lattice}\\
{\tt parallel}\\
{\tt parameters}\\
{\tt session}\\
{\tt util}
\end{array} \right.\\
{\tt CosmoInterface}:\left\lbrace
\begin{array}{l}
{\it CosmoInterface.h}  {\rm[+ other files]}\\
{\tt definitions}\\
{\tt evolvers}\\
{\tt initializers}\\
{\tt measurements}
\end{array} \right.
\end{array} \right.
\end{array} \right.
\end{array} \right.
\end{align}
```

with the content of each folder summarized as:

| **folder name(s)** | **brief description of each folder**                                                          |
| ------------------ | --------------------------------------------------------------------------------------------- |
| `dependencies`     | scripts to install external libraries                                                         |
| `docs`             | documentation files                                                                           |
| `src`              | source code (contains `cmake, models, tests, include`)                                        |
| `cmake`            | files for compilation                                                                         |
| `models`           | model files                                                                                   |
| `tests`            | files for testing purposes                                                                    |
| `include`          | libraries for lattice operations (`TempLat`) and field dynamics (`CosmoInterface`)            |
| `TempLat`          | library for lattice operations (contains `fft, lattice, parallel, parameters, session, util)` |
| `CosmoInterface`   | library for field dynamics (contains `definitions, evolvers, initializers, measurements)`     |
| `fft`              | library to handle Fourier transformations                                                     |
| `lattice`          | library for basic lattice definitions and field operations                                    |
| `parallel`         | library for parallelization routines                                                          |
| `parameters`       | library for parsing parameters from the command-line/files.                                   |
| `session`          | library for taking care of initialization and destruction of external libraries               |
| `util`             | library for basic useful operations                                                           |
| `definitions`      | library for basic field definitions (EOM terms, energy terms, etc)                            |
| `evolvers`         | library for evolution algorithms                                                              |
| `initializers`     | library for initialization algorithms                                                         |
| `measurements`     | library for observables (energy densities, field spectra, etc)                                |

!!! note
    **Note:** A remarkable feature of CosmoLattice is that the operations and parallelization of `TempLat` can actually work in an arbitrary number of spatial dimensions $d$, including $d < 3$ and $d>3$. This feature is not exploited in `version 1.0` of CosmoLattice, but the `TempLat` library has this capability, which makes it a perfect basis for developing future interfaces dealing with field dynamics on lower- or higher-dimensional lattices. Visit [ https://cosmolattice.net/technicalnotes/](https://cosmolattice.net/technicalnotes/) to check for additional modules incorporated in successive updated versions of CosmoLattice to run in $d \neq 3$ spatial dimensions.

### Basic Field Equations implemented (so far) in CosmoLattice { #subsec_BasicEOM }

Let us consider scalar fields of the type
[](){ #eq_ChargedScalars }
```math
\begin{align} \tag{3}
\begin{array}{c|c|c}
{\rm Singlet} & U(1){\rm-charged} & SU(2){\rm-charged Doublet}
\\\hline   &   &\\
\phi \in \mathcal{R}e &  \varphi \equiv {1\over\sqrt{2}}(\varphi_0 +i\varphi_1) & \Phi = \left(
\begin{array}{c}
\varphi^{(0)} \\ \varphi^{(1)}
\end{array}
\right) =
{1\over\sqrt{2}}
\left(
\begin{array}{c}
\varphi_0 +i\varphi_1 \\ \varphi_2 +i\varphi_3
\end{array}
\right)
\end{array}  ,
\end{align}
```

and define standard *gauge covariant derivatives* $D_{\mu}^{\rm A} \equiv \partial _{\mu} - i Q_A g_{_A}A_\mu $, $D_{\mu} \equiv  \mathcal{I}D^{\rm A}_\mu - i g_B Q_B B_{\mu}^a  T_a$ (here $Q_{A}$ and $Q_B$ denote the Abelian and non-Abelian charges), and *field strength* tensors $F_{\mu \nu} \equiv  \partial_{\mu}  A_{\nu} - \partial_{\nu} A_{\mu}$, and $G_{\mu \nu} \equiv \partial_{\mu} B_{\nu} - \partial_{\nu} B_{\mu} - i[B_\mu,B_\nu]$, where $A_\mu$ and $B_\mu = B_\mu^aT_a$ are Abelian and non-Abelian gauge fields, $\mathcal{I}$ is the $2\times 2$ identity matrix, and $\lbrace T_a \equiv \sigma_a / 2 \rbrace$ ($a=1,2,3$) are the $SU(2)$ group generators, with $\sigma_a$ the *Pauli matrices*. CosmoLattice is then ready to solve the following type of equations (here written in cosmic time, with $a(t)$ the scale factor):
[](){ #eq_singletEOM }
[](){ #eq_higgsU1EOM }
[](){ #eq_higgsSU2EOM }
[](){ #eq_U1EOM }
[](){ #eq_SU2EOM }
```math
\begin{align}
\text{d}ot{\phi} - a^{-2} {\vec\nabla}^{ 2} \hspace{-1mm}\phi + 3 \frac{\dot{a}}{a} \dot{\phi} &= - V_{,\phi}  , \tag{4} \\
\text{d}ot{\varphi} - a^{-2} {\vec D}_{\hspace{-0.5mm}A}^{ 2}\varphi + 3\frac{\dot{a}}{a}  {\dot \varphi} &= - {1\over2}{\varphi \over |\varphi|}V_{,|\varphi|}  , \tag{5}\\
\text{d}ot{\Phi} - a^{-2} {\vec D}^{ 2}\Phi + 3 \frac{\dot a}{a}  {\dot{\Phi}} &= - {1\over2}{\Phi \over |\Phi |}V_{,|\Phi|}  , \tag{6}
\\
\partial_0 F_{0i} - a^{-2} \partial_j F_{ji} +  \frac{\dot a}{a} F_{0i} &=
J^A_i  , \tag{7}
\\
(\mathcal{D}_0 )_{a b} (G_{0i})^b -  a^{-2} ( \mathcal{D}_j )_{a b} (G_{ji} )^b + \frac{\dot a}{a} (G_{0i} )^b &= (J_i)_a  , \tag{8}
\end{align}
```

with as many copies as desired of each type of field\footnote{CosmoLattice version 1.0 allows to simulate field theories with one gauge field of each kind, i.e. one Abelian field $A_{\mu}$ and one non-Abelian field $B_{\mu}^a$, and only one $SU(2)$ doublet when it couples to an $SU(2)$ gauge field. We are currently testing the possibility of simulations with multiple gauge fields in CosmoLattice, so we will make available this option as soon as possible in a future update of the code.}, and where
$V \equiv V(\phi,|\varphi|, |\Phi|)$ is the potential describing the interactions among the scalar fields. The Abelian and non-Abelian currents in the $rhs$ of the gauge field EOM ([*7*][eq_U1EOM])-([*8*][eq_SU2EOM]), correspond to $J_A^\mu \equiv 2 g_AQ_A^{(\varphi)} \mathcal{I}m [ \varphi^{*} ( D_A^{\mu} \varphi )] + 2 g_AQ_A^{(\Phi)} \mathcal{I}m [ \Phi^\dagger (D^{\mu} \Phi  )]$ and $J_a^\mu \equiv 2g_BQ_B\mathcal{I}m [ \Phi^{\dagger} T_a( D^{\mu} \Phi )]$. CosmoLattice guarantees that the constraint equations
[](){ #eq_GaussU1 }
[](){ #eq_GaussSU2 }
```math
\begin{align}\tag{9}
\partial_i F_{0i} &= a^2J^A_0 , \\ \tag{10}
(\mathcal{D}_i )_{a b} (G_{0i})^b &= a^2(J_0)_a ,
\end{align}
```

which represent the $U(1)$ and $SU(2)$ Gauss constraints in an expanding background, are preserved all throughout the evolution.

In the case of self-consistent expansion, CosmoLattice obtains numerically the scale factor $a(t)$ by solving the *Friedmann* equation (here written in cosmic time)
[](){ #eq_Friedmann-full }
```math
\hspace{0.6cm} {\text{d}ot a\over a} = - \frac{1}{6 m_p^2}[ \bar\rho + 3 \bar p ] ,\tag{11}
```

while checking that the other Friedmann equation -- the *Hubble constraint* -- (also written in cosmic time),
[](){ #eq_HC }
```math
\tag{12}
H^2 \equiv \left({\dot a\over a}\right)^2 =  \frac{\bar\rho}{3 m_p^2}  ,
```

is verified throughout the evolution. Here $\bar\rho \equiv \langle \rho \rangle$ and $\bar p \equiv \langle p \rangle $are the background energy and pressure densities, obtained from a volume average of the local expressions contributed by the matter fields (both scalar and gauge fields),
[](){ #eq_rhoLocal }
[](){ #eq_pLocal }
```math
\begin{align}
\rho &= {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} + {G}_{\phi} + {G}_{\varphi} + {G}_{\Phi} + {K}_{U(1)} + {G}_{U(1)} + {K}_{SU(2)} + {G}_{SU(2)} + {V}  ,  \tag{13}\\
p &= {K}_{\phi} + {K}_{\varphi} + {K}_{\Phi} -{1\over3}({G}_{\phi} + {G}_{\varphi} + {G}_{\Phi}) + {1\over3}({K}_{U(1)} + {G}_{U(1)}) + {1\over3}({K}_{SU(2)} + {G}_{SU(2)}) - {V}  ,  \tag{14}
\end{align}
```

with $V$ the interacting scalar potential, and $K_x$ and $G_x$ the kinetic and gradient energy densities of each field species [for their exact expression see Eq. (51) in Ref. [@Figueroa_2020rrl], or  e.g. Eq. ([*67*][eq_energy-contrib]) in this manual]. This procedure determines the evolution of the background metric of the universe within a given volume $L^3$, with $L$ the length scale of the simulation box. As long as $L$ is sufficiently large compared to the typical wavelengths excited in the fields, this procedure should lead to a well-defined notion of a 'homogeneous and isotropic' expanding background, within the given volume $L^3$ of the box.

CosmoLattice can also allow for a fixed expansion rate of the universe, with the scale factor given by a power-law function (again written in cosmic time),
[](){ #eq_aFixed }
```math
\tag{15}
a(t) = a (t_* ) \left(1 + \frac{3 (1 + w)}{2} H (t_*) (t-t_*) \right)^{\frac{2}{3(1 + \omega) }  , }  ,
```

where $a(t_*)$ and $H (t_*)$ are the scale factor and Hubble parameter evaluated at the initial time of the simulation $t = t_*$, and $w$ is the constant equation of state of an external fluid sourcing the expansion (assumed to be energetically dominant with respect to the fields actually being simulated). Eq. ([*15*][eq_aFixed]) acts as an input for the field Eqs. ([*4*][eq_singletEOM])-([*8*][eq_SU2EOM]).

We note that the dynamical equations presented before were expressed in cosmic time just for simplicity. In reality, CosmoLattice can solve them in any time variable of the user's preference, such as conformal time. More importantly,
we note that, of course, CosmoLattice does not really solve exactly the continuum differential equations as formulated in Eqs. ([*4*][eq_singletEOM])-([*8*][eq_SU2EOM]) or Eq. ([*11*][eq_Friedmann-full]), nor it really checks the differential constraint Eqs. ([*9*][eq_GaussU1])-([*10*][eq_GaussSU2]) or Eq. ([*12*][eq_HC]). CosmoLattice rather solves and/or checks a set of finite difference equations -- the *lattice equations* -- that approximate the above equations in the continuum. The lattice equations, and hence their numerical solutions, can reproduce the continuum results with higher or lower accuracy, depending on the integrator algorithm on which the lattice equations are based on. Some algorithms can solve lattice equations with numerical solutions that satisfy the (lattice) constraint equations down to machine precision. For a detailed description of different numerical integration algorithms, we refer the interested reader to Sections 3.3-3.5 of Ref. [@Figueroa_2020rrl]. The detailed implementation of such algorithms in CosmoLattice, as specialized for the dynamics of singlet scalar fields, Abelian-gauge theories and non-Abelian gauge theories, can be found in sections 4, 5, and 6 of Ref. [@Figueroa_2020rrl], respectively.

