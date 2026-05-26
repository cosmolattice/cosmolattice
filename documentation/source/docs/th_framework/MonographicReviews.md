<!-- ** Theory Monographs Guide. **  -->

The theoretical basis for the equations implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is given in our monographic series on latttice techniques, *"The art of simulating the early Universe"*, 

*Part I. Integration techniques and canonical cases.* (Ref. [@Figueroa_2020rrl])

*Part II. Non-canonical cases and gravitational waves.* (Ref. [@BaezaBallesteros_2025tme]) 

*Part III. Scalar-Gauge-Fluid dynamics.* (Ref. [@Figueroa_2026XYZ]) 

which will be referred to, colloquially, as $\mathtt{The~Art-I}$, $\mathtt{The~Art-II}$ and $\mathtt{The~Art-III}$ monographs.

These monographs [@Figueroa_2020rrl,@BaezaBallesteros_2025tme,@Figueroa_2026XYZ] provide comprehensive discussions of lattice methods for scalar–gauge field theories in an expanding universe ($\mathtt{The~Art-I}$ [@Figueroa_2020rrl]), gravitational waves and non-canonical aspects of field theory - including interactions, initial conditions, dimensionality - ($\mathtt{The~Art-II}$ [@Baeza-Ballesteros:2025tme]), and fluid dynamics as a description of an ensemble of interacting scalar, gauge, and fermion fields ($\mathtt{The~Art-III}$ [@Figueroa_2026XYZ]). 

We recommend users without previous experience on lattice techniques, that before diving into the code, they read at least sections 2, 3 and 4 of Ref. [@Figueroa_2020rrl], in order to have a better understanding of the basic lattice concepts used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. In particular, Section 2 of [@Figueroa_2020rrl] reviews the formulation of scalar and gauge field interactions in the continuum, both in flat and *Friedmann-Lemâitre-Robertson-Walker* (FLRW) backgrounds, whereas Section 3 of [@Figueroa_2020rrl] introduces the basic tools for discretizing any bosonic field theory in an expanding background, including a discussion on *lattice gauge-invariant* techniques for both *Abelian* and *non-Abelian* gauge theories. Section 4 of Ref. [@Figueroa_2020rrl] describes the formulation and properties of the evolution algorithms used in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ to simulate the dynamics of interacting singlet scalar fields. If the user wants to run simulations of the dynamics of Abelian and non-Abelian scalar-gauge theories, we recommend them to read sections 5 and 6 of Ref. [@Figueroa_2020rrl], as we discuss there in detail the different evolution algorithms implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ for dealing with gauge theories.