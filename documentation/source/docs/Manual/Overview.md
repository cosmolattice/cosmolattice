CosmoLattice is a program designed to simulate the evolution of interacting
fields in an expanding universe. It can simulate (so far) the dynamics of 

1. *global theories*, 
2. *Abelian $U(1)$ gauge theories*, and 
3. *non-Abelian $SU(2)$ gauge theories*, 

i.e. it can handle scenarios
including singlet scalar fields, scalar fields charged under a $U(1)$ and/or $SU(2)$ gauge symmetry, and the corresponding Abelian and/or non-Abelian gauge vector fields.

CosmoLattice can simulate the dynamics of such fields either in a flat space-time background, or in a homogeneous and isotropic (spatially flat) expanding background. In the latter case, the fields can evolve either over a fixed background (e.g. with a power-law scale factor), or *self-consistently* with the fields determining themselves the expansion rate of the universe. 

In all cases CosmoLattice provides symplectic integrators, with accuracy ranging from $\mathcal{O}(\delta t^2)$ up to $\mathcal{O}(\delta t^{10})$. Appropriate observables are also provided for each algorithm, like the energy density components of each field, their relevant spectra, or dynamical constraints. Our algorithms conserve energy up to the accuracy set by the order of the evolution algorithm, reaching even machine precision in the case of the highest order integrators. Notably, our algorithms for gauge theories, either Abelian or non-Abelian, always respect the Gauss constraint to machine precision, independently of the integrator and even in the case of self-consistent expansion.

CosmoLattice is written in C++, and fully exploits the *object oriented  programming* nature of this language, with a modular structure that separates well all the ingredients involved. This allows CosmoLattice to have a clear separation between the physics and the technical implementation details. The code is designed so that we can simulate a given scenario with different parameters, without requiring to re-compile each time. More importantly, the code allows for an easy implementation of new models with either singlet or gauge interactions. 

CosmoLattice is fully parallelized using *Message Passing Interface* (MPI), and uses a discrete Fourier Transform parallelized in multiple spatial dimensions. This makes it ideal for probing physical problems with well-separated mass/length scales, running very high resolution simulations, or simply shortening the running time of long simulations. CosmoLattice is actually a general package that defines field variables and their operations, by introducing its own symbolic language. Once you become familiar with the basic ‘vocabulary’ of the CosmoLattice language, editing the code or implementing your own model (resembling how you would write it in the continuum), should become a simple task.

CosmoLattice can be used at multiple levels of complexity. 

For instance, a *basic level* user, say someone with no experience at all in parallelization techniques and with little to no experience programming in general, will be able to run fully parallelized simulations of their favourite models (say using hundreds of processors in a cluster), while being completely oblivious to the technical details of the algorithm implementation or parallelization. CosmoLattice automatically prints a collection of relevant observables, such as volume averages, field spectra, or dynamical constraints, which can be used to monitor the evolution of the system. 

However, an *intermediate level* user, say with certain programming experience, may want however to modify the type of output CosmoLattice generates, change the initial condition routine (e.g. turn it into a Monte-Carlo generator for thermal configurations), or even design their own evolution algorithms and add these to the family of integrators available in CosmoLattice. 

Finally, *advanced level* users may want to edit the hard core inner parts of CosmoLattice, in an attempt to understand or to improve the most advanced technical aspects of the code. This could include e.g. the handling of the parallelization, which is something that typically will remain as a black box for the majority of users. What type of user you want to be is entirely up to you: it simply depends on your programming expertise (or lack of it), and on your will to learn how the code works internally.

We have developed CosmoLattice with the intention of providing a new up-to-date, relevant numerical tool for the scientific community working in the physics of the early universe. Presently CosmoLattice is able to simulate canonical scalar-gauge field theories in an expanding universe, and as such, it is already an extremely useful tool for many physics scenarios. However, we conceive CosmoLattice as an evolving package that we plan to upgrade constantly, for instance by incorporating new evolution algorithms or new modules dedicated to specialized tasks. In that regard, we plan to further develop new modules for CosmoLattice, that will be made publicly available in due time. For example, to mention just a few, we plan to add the computation of gravitational waves, an initializer for the creation a cosmic defects, the handing of theories with non-canonical kinetic terms, and the inclusion of axion-couplings to $F\tilde F$ of a gauge sector. The interested reader can check Section 9 of Ref. [@Figueroa_2020rrl] for further details on the aspects we plan to implement in the future.

CosmoLattice is freely available to anyone who wants to use or modify it, as long as you give us credit for its creation. If you have any questions or comments about CosmoLattice, please email us (you can find our emails in the front page of this document). We would love to hear how the program performs for you, and we will be happy to assist you with any question you might have, bug reports, suggestions for future improvements, etc. 

We welcome everyone to use CosmoLattice for their own projects. Whenever using CosmoLattice in your research, no matter how much (or little) you modify the code, please cite this manual together with our dissertation *The art of simulating the early Universe* [@Figueroa_2020rrl], where the basic algorithms and techniques implemented in CosmoLattice are explained.

You can download CosmoLattice at any time from [ http://www.cosmolattice.net](http://www.cosmolattice.net)

!!! note
    **Note:** If you would like to help developing some aspect of CosmoLattice, or even implement your own modules with some new functionality we have not envisaged, please contact us and let us know about your idea(s). CosmoLattice introduces a natural language describing fields and operations between them, so it is a natural platform to implement new libraries (related or not to cosmology).

