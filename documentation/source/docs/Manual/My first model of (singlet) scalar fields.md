We now introduce the user to the basic functionalities of CosmoLattice. As an example, we explain  step by step how to implement a specific model of interacting scalar fields in CosmoLattice, for the simulation of a simple preheating scenario.

This section is structured as follows.
In Section [*Program variables*][subsec_LatticeScalars] we first introduce the concept of *program variables* for scalar fields, which are a new set of re-scaled dimensionless variables suitable for their introduction in a computer. 
In Section [*The model*][sec_ScTheModel] we present an example model and define its corresponding program variables and potential specific to it. 
We then explain in Section [*My first run*][sec_MyFirstRun] how to compile and run the code. 
After that, in Section [*The model file*][sec_TheModelFile] we walk the user through the *model file*, where the model details are actually implemented. 
Finally, in Section [*The physics implemented in CosmoLattice*][sec_WhatHappensAuto] we provide a summarized picture of what happens 'under the hood', giving details on how the fields are initialized in the simulation, how their dynamical evolution is solved, and how different measurements are obtained. 

By the end of this chapter, the reader should have enough information to implement any model involving interacting scalar fields. Users interested in including gauge fields in their models should proceed to read Section [My first model of gauge fields](My first model of gauge fields.md).

### Program variables { #subsec_LatticeScalars }

!!! note
    **Important:** On the lattice, we operate with a particular set of dimensionless field and spacetime variables, $\{\tilde{\phi},\tilde{\eta},\tilde{x}^i\}$, which we call *program variables*. The transformation from 'physical' to program variables, $t \rightarrow \tilde{\eta}$, $x^i \rightarrow \tilde{x}^i$, and $\phi \rightarrow \tilde{\phi}$, is given by the following relations
    [](){ #eq_FieldSpaceTimeNaturalVariables }
    
    ```math
    
    \begin{align}
    \tag{29}
    \tilde\phi \equiv {\frac{\phi}{f_*}} ,  d\tilde\eta \equiv a^{- \alpha}  \omega_* dt ,  d\tilde x^i \equiv \omega_* dx^i ,
    \end{align}
    
    ```
    
    where $f_*$ and $\omega_*$ are two constants with dimensions of energy. Program variables will be tagged with the diacritic $\sim$, as well as all quantities defined in terms of them.

The reader familiar with `LatticeEasy`, might have notticed that the above transformations are similar to the ones carried out in that code to define their program variables, if we set $A = 1/f_*$, $B= \omega_*$, $r=0$, and $s=-\alpha$ in their notation [@Felder_2000hq]. As we will see, the main difference is that the evolution algorithms implemented in CosmoLattice *do not require a conformal rescaling of the fields*, so we do not need to introduce a parameter analogous to $r$.

Before simulating a particular model, the user must choose a certain set of values for $\{f_*,\omega_*,\alpha\}$, which will define the program variables used on the lattice via Eq. ([*29*][eq_FieldSpaceTimeNaturalVariables]). The choice of $f_*$ and $\omega_*$ can be made arbitrary, as they only re-scale all numbers by constant factors. However, if we simulate a scenario in which, e.g. a resonance is triggered by an oscillatory field, it can be convenient to set $f_*$ and $\omega_*$ to the initial amplitude and oscillation frequency of that field respectively. This way, the numbers produced by the code will be close to unity, which will help the interpretation of results. In general, in every scenario there is always a natural choice (at least of the order of magnitude) of $f_*$ and $\omega_*$, related to the typical field amplitudes and time scales of the the problem. Choosing those natural values will help us interpret more easily (in a more intuitive physical manner), the numbers that the code outputs.

A correct choice of $\alpha$ is perhaps more relevant, as a wrong choice could spoil the stability of the numerical solution at late times. For example, let us go back to the case of an oscillating homogeneous scalar field dominating the energy budget of the Universe. Our evolution algorithms operate with a constant time step, so it would be a good idea would be to choose $\alpha$ so that the oscillation frequency of the program field variable is approximately constant when expressed in the corresponding $\alpha$-time. In this way, we will be able to resolve each physical oscillation with a similar accuracy. For example, let us consider the common case of an oscillatory field with monomial potential $V(\phi) \propto |\phi|^p$ sourcing the expansion of the Universe. As described extensively in Ref. [@Figueroa_2020rrl], the oscillation frequency will be initially constant if we choose
[](){ #eq_Alpha-PowLaw }
```math
\alpha = 3 \left( \frac{p-2}{p+2} \right)  . \tag{30}
```

We recommend to use Eq. ([*30*][eq_Alpha-PowLaw]) for any scenario where there is an energetically dominant scalar field with potential $V(\phi) \propto \phi^p$. The choice of $\alpha$ for more complex scenarios must be done in a case by case basis.

### The model { #sec_ScTheModel }

We will consider a simple preheating scenario for illustrative purposes, consisting of an inflaton $\phi$ with quartic potential $V(\phi) \propto \phi^4$, coupled to a secondary massless scalar field $\chi$ through a quadratic interaction. Denote the total number of scalar fields in a theory as $N_s$, then $N_s=2$ in our case. To describe the expansion of the universe, we consider a flat *Friedmann-Lemâitre-Robertson-Walker* (FLRW) metric with line element
[](){ #eq_FLRWmetric }
```math
\text{d} s^2 = g_{\mu\nu}\text{d} x^\mu\text{d} x^\nu = - a(\eta)^{2 \alpha} \text{d} \eta^2 + a(\eta)^2 \delta_{ij} \text{d} x^i \text{d} x^j  , \tag{31}
```

where $a(\eta)$ is the scale factor, $\delta_{ij}$ is the Euclidean metric, and $\alpha$ is a constant parameter that will we choose conveniently in a moment. The choice $\alpha = 0$ would identify $\eta$ with *cosmic time* $t$, whereas $\alpha = 1$ would identify it with *conformal time* $\tau \equiv \int {dt'  a^{-1}(t')}$. For now, we will consider $\alpha$ as an unspecified constant, and we will refer to $\eta$ as the *$\alpha$-time variable*.

!!! note
    **Note:** We remind the reader that we reserve the symbol $\dot f \equiv {df/dt}$ for derivatives with respect to cosmic time, and $f' \equiv {df/ d\eta}$ for derivatives with respect to $\alpha$-time.

In this metric, the action of the field theory we want so simulate is the following,
[](){ #eq_ScalarActionCont }
[](){ #eq_potentialExampleI }
```math
\begin{align} \tag{32}
S_{\rm S} &= - \int d\eta d^3 x a(\eta)^{3+\alpha} \left\{{1\over2} \partial^{\mu} \phi \partial_{\mu}\phi +{1\over2} \partial^{\mu} \chi \partial_{\mu}\chi + V(\phi,\chi)\right\}  , \\
V(\phi,\chi) &\equiv \sum_{m=0}^{N_p-1} V^{(m)} (\phi, \chi) =  \frac{\lambda}{4}\phi^4 +\frac{1}{2}g^2 \phi^2\chi^2 , \tag{33}
\end{align}
```

where $V(\phi, \chi)$ is the scalar potential, and $\lambda$ and $g$ are dimensionless parameters. The potential contains two different terms: the quartic potential of the inflaton and the interaction between both fields, which we denote as $V^{(m)}$ with $m=0,1$ respectively. The total number of terms is defined as $N_p$ ($= 2$). Indices are raised/lowered using the FLRW metric defined in Eq. \eqref{eq:FLRWmetric}, e.g. $\partial^{\mu} \phi \partial_{\mu}\phi=g^{\mu\nu}\partial_\mu\phi\partial_\nu\phi$. The field equations of motion in $\alpha$-time read [@Figueroa_2020rrl]
[](){ #eq_scEOM }
```math
\begin{align*}
\phi'' - a^{-2(1 - \alpha)} {\vec\nabla}^{ 2} \hspace{-1mm}\phi + (3 - \alpha)\frac{{a'}}{a} {\phi'} &= - a^{2 \alpha} V_{,\phi}  , \\
&=- a^{2 \alpha} (\lambda \phi^3 +g^2\phi\chi^2) \tag{34}\\
\chi'' - a^{-2(1 - \alpha)} {\vec\nabla}^{ 2} \hspace{-1mm}\chi + (3 - \alpha)\frac{{a'}}{a} {\chi'} &= - a^{2 \alpha} V_{,\chi} \\
&=- a^{2 \alpha} g^2\phi^2\chi  ,
\end{align*}
```

with the evolution of the scale factor $a(\eta)$ given by the Friedmann equations. If these two fields constitute the only energy sources in the Universe (or at least the dominant ones), either of the Friedmann equations
[](){ #eq_sfEOM }
```math
\begin{align*}
\mathcal{H}^2 \equiv  \left({a'\over a}\right)^2 &=  \frac{a^{2 \alpha}}{3 m_p^2}\left\langle  {K}  + {G} +  {V} \right\rangle  ,
\\
{a''\over a} &= \frac{a^{2 \alpha}}{3 m_p^2}\left\langle (\alpha-2)  {K} + \alpha  {G} + (\alpha + 1)  {V}  \right\rangle  ,  \tag{35}
\end{align*}
```

can be solved self-consistently, together with the fields' equations of motion. Here $\langle \dots \rangle$ indicates a volume average, and $K$ and $G$ are the total kinetic and gradient energies. All scalar fields contribute to these quantities as $K \equiv \sum_{n=0}^{N_s-1} {K}^{(n)}$ and $G \equiv \sum_{n=0}^{N_s-1} {G}^{(n)} $, with [$\phi_0 \equiv \phi$, $\phi_1 \equiv \chi$],
```math
{K}^{(n)} = \frac{1}{2 a^{2\alpha} } \phi_n^{'2}  , \hspace{0.4cm} {G}^{(n)} = \frac{1}{2 a^2} \sum_i (\nabla_i \phi_n)^2  .
```

In other scenarios, one could have the expansion of the Universe to be fixed by an external, energetically-dominant fluid with (constant) equation of state $w$. In this case, the evolution of the scale factor and the Hubble parameter in program variables is given by the following functions,
[](){ #eq_ScaleFactorPowerLaw }
```math
\tag{36}
a(\tilde \eta) = a (\tilde \eta_* ) \left(1 + \frac{1}{p}\mathcal{H}_* (\tilde \eta- \tilde\eta_*) \right)^p  ,\hspace{0.3cm} \mathcal{H}(\eta) = {\mathcal{H}_*\over \left(1 + \frac{1}{p}\mathcal{H}_* (\tilde \eta- \tilde \eta_*) \right)}   ,\hspace{0.5cm} p \equiv \frac{2}{3(1 + \omega) - 2 \alpha }  .
```

As mentioned before, numerical simulations are carried out in the dimensionless program variables defined in Eq. ([*29*][eq_FieldSpaceTimeNaturalVariables]). Therefore, we need to appropriately choose values for $\{ f_*, \omega_*, \alpha\}$ in this model. We take them as follows,
[](){ #eq_lphi4-ProgVar }
```math
\begin{align} \tag{37}
f_*=\overline{\phi}_{*} ,  \omega_*=\lambda^{1/2}  \overline{\phi}_*,  \alpha=1
\end{align}
```

where $\overline{\phi}_{*}$ and $\lambda^{1/2} \overline{\phi}_*$ are the amplitude and oscillation frequency of the inflaton at the end of inflation, see e.g. Ref. [@Greene_1997fu]. The constant $\alpha$ was chosen according to Eq. ([*30*][eq_Alpha-PowLaw]), which guarantees that the oscillation frequency remains approximately constant as long as the oscillatory inflaton field $\phi$ dominates the energy budget.

In CosmoLattice, any field theory is implemented by means of the *program potential*, which is a dimensionless quantity defined in terms of the program variables as follows,
[](){ #eq_PotNat }
```math
\begin{align}\tag{38}
\widetilde V( \tilde\phi, \tilde\chi ) \equiv \frac{1}{f_*^2 \omega_*^2}V(f_*\tilde \phi,f_*\tilde\chi) = \frac{1}{4}\tilde\phi^4 +\frac{1}{2}\frac{g^2}{\lambda}\tilde\phi^2\tilde\chi^2  .
\end{align}
```

Similarly, we define each of the individual contributions of this quantity as $\widetilde{V}^{(m)} \equiv {V}^{(m)} /(f_*^2 \omega_*^2)$. We also define the following (dimensionless) *program energy/pressure densities* as
```math
\begin{align}
\tilde\rho \equiv \frac{\rho}{f_*^2 \omega_*^2} = \widetilde{K} + \widetilde{G}  + \widetilde V  ,  ;   \tilde p \equiv \frac{p}{f_*^2 \omega_*^2} = \widetilde{K} - \frac{1}{3} \widetilde{\rm G} - \widetilde V  .
\end{align}
```

with $\tilde V$ given by Eq. ([*38*][eq_PotNat]), and where we have introduced the following *program kinetic and gradient energies* as $\widetilde{K} \equiv \sum_{n=0}^{N_s-1} \widetilde{K}^{(n)}$ and $\widetilde{G} \equiv \sum_{n=0}^{N_s-1} \widetilde{G}^{(n)} $, with
[](){ #eq_KandGprogramUnits }
```math
\begin{align}\tag{39}
\widetilde{K}^{(n)} = \frac{1}{2 a^{2\alpha}} ({\tilde \phi_n}')^2   , \hspace{0.4cm}
\tilde{G}^{(n)} = \frac{1}{2 a^2 }  \sum_{i} (\widetilde\nabla_i \tilde \phi_{n})^2  .
\end{align}
```

We denote the corresponding volume-averaged energy density components as
[](){ #eq_EK_EG_EV }
```math
\begin{align}\tag{40}
{\widetilde E}_K \equiv \left\langle \tilde{K} \right\rangle ,  {\widetilde E}_G \equiv \left\langle \tilde{G} \right\rangle ,  {\widetilde E}_V \equiv \left\langle \tilde{V} \right\rangle  ,
\end{align}
```

and their partial contributions as ${\widetilde E}_K^{(n)}$, ${\widetilde E}_G^{(n)}$ and ${\widetilde E}_V^{(m)}$ respectively.  Using these notations, the equations of motion, still in the continuum but already expressed in program variables, read

!!! note
    [](){ #eq_EOMscalarContinuumNat }
    [](){ #eq_NewFriedmannEQsII }
    [](){ #eq_NewFriedmannEQsI }
    
    ```math
    
    \begin{align*}
    \tilde\phi'' - a^{-2 (1  - \alpha )} \tilde\nabla^2 \tilde\phi + (3 - \alpha)\frac{a'}{a} \tilde\phi'_a  &= -  a^{2 \alpha} \widetilde V_{,\tilde\phi} \tag{41}\\
    &= -  a^{2 \alpha}\left( \tilde \phi^3 + \frac{g^2}{\lambda}\tilde\phi\tilde\chi^2\right) \\
    \tilde\chi'' - a^{-2 (1  - \alpha )} \tilde\nabla^2 \tilde\chi + (3 - \alpha)\frac{a'}{a} \tilde\chi'_a  &= -  a^{2 \alpha} \widetilde V_{,\tilde\chi} \\
    &=- a^{2 \alpha}  \frac{g^2}{\lambda}\tilde\phi^2\tilde\chi
    \\
    \tag{42}
    {a''\over a} &=  \frac{a^{2\alpha}}{3} \left( \frac{ f_*}{m_p} \right)^2 \Big[ (\alpha - 2){\widetilde E}_{K}  + \alpha {{\widetilde E}_{G}} + (\alpha + 1 ) {{\widetilde E}_V} \Big]  ,\\
    \tag{43}
    a'^{ 2} &= \frac{a^{2\alpha + 2}}{3} \left( \frac{ f_*}{m_p} \right)^2 \Big[ {\widetilde E}_{K}  + {{\widetilde E}_{G}} + {{\widetilde E}_V} \Big]  .
    \end{align*}
    
    ```

We note that the numerical schemes implemented in CosmoLattice use exclusively the second-order differential equation ([*42*][eq_NewFriedmannEQsII]) to solve for the scale factor, whereas Eq. \eqref{eq:NewFriedmannEQsI} is used simply as a constraint to monitor the accuracy of the obtained solution.

### My first run { #sec_MyFirstRun }

CosmoLattice comes with a set of ready-to-run models, which are available in the folder `src/models/`. In particular, the file `src/models/lphi4.h` contains the implementation of the model presented in the previous section, characterized by the potential given in Eq. ([*33*][eq_potentialExampleI]). We now show how to run the code and pass different parameters to the simulation. We also show how to modify/create model files in order to implement other scalar theories.

#### Compilation

First, we need to choose the location where the code will be compiled. This can be anywhere on your machine, **except in `src/` or any of its sub-folders**. As an example, let us create a `build/` directory and move inside it,
```bash
cd cosmolattice
mkdir build
cd build
```
CosmoLattice uses CMake for compilation (see Section [Appendix: CMake Flags](Appendix: CMake Flags.md) for more details). The model `lphi4.h` is compiled by typing the following commands,
```bash
cmake -DMODEL=lphi4 ../
make cosmolattice
```

Some explanations are of order.  The last argument of the \textcolor{gray}{`cmake`} command is the path to the CMake configuration file, which is located at the root of the CosmoLattice folders. In our case, its relative path with respect to the `build/` folder is `../`. The first argument \textcolor{gray}{`-DMODEL=lphi4`} is passed to CMake, and tells it to compile the model `lphi4.h`. Changing this argument to any other model present inside the `src/models/` will determine which model is compiled. Note that this is not a CosmoLattice-specific CMake argument, see Appendix [Appendix: CMake Flags](Appendix: CMake Flags.md) for an exhaustive list.

!!! note
    **Important Note:** Every time you call CMake, it is a good practice to first remove the `CMakeCache.txt` file that was previously generated.

At this point, if everything went smoothly, you should have generated an executable named `lphi4`. If this is the case, move on to the next section. If not, continue reading.

#### Troubleshooting

A common problem that will happen to some users at this stage is that CMake does not find your FFTW installation, typically because it is not installed in a standard path. If that is the case, you can indicate the location of FFTW by calling CMake as follows,
```bash
cmake -DMODEL=lphi4 ../   #Does not work because your fftw3 is not found.
rm CMakeCache.txt         #We want to clear the CMake before running it again.
cmake  -DMYFFTW3_PATH="/path/to/fftw3/" -DMODEL=lphi4 ../   #And now this works!
```
with `/path/to/fftw3/` the path where fftw3 is located. You can also call `make clean-cmake` to remove the `CMakeCache.txt` file.

If this solves your problem, you can avoid having to specify the FFTW path each time you compile by modifying line `52` of the `CMakeLists.txt` file (located at the root of the CosmoLattice files), as highlighted below:

`CMakeLists.txt:`

@emgithub(CMakeLists.txt:fetchcontent_templat)
where, again `/path/to/fftw3/` is the path where fftw3 is located.

#### Running the program with an input parameter file { #subsec_Input-Scalars }

Now that we have generated the executable `lphi4`, we are ready to run our first simulation as follows:
```bash
./lphi4 input=../src/models/parameter-files/lphi4.in
```

This will launch the model `lphi4` with the parameters specified in the input file located in \path{src/models/parameter-files/lphi4.in}. Let us have a look at it.

`src/models/parameter-files/lphi4.in:`

@emgithub(models/parameter-files/lphi4.in)

One of the perks of using CosmoLattice is its very flexible way of handling parameters. The standard way of passing parameters to the program is to bundle them in an input file such as `lphi4.in`, and indicate its path when calling the program with the `input=...` argument. The structure of the input file is rather straightforward. First, if we want to pass a single parameter, we just write down its name followed by an equal sign, and then define its value. Second, if we are passing parameters that admit multiple values, these must be separated with white spaces, as e.g. line `25` of `lphi4.in` above. And third, the character `\#` is use for comments, so everything following such character in a given line will be ignored. Note that the order in which the parameters are specified does not matter.  To sum it up, the way of defining parameters in an input file is
```text
singleParameterName = value
multipleParametersName = value1 value2 value3 ...
```
A convenient feature of CosmoLattice is that we can also pass arguments directly through the console. Moreover, this feature can be used together with an input file: even if the argument is already specified in the file, it will always be overwritten by the one passed through the command-line. For instance,
```bash
./lphi4 input=../src/models/parameter-files/lphi4.in N=64
```
will launch `lphi4` with the parameters specified in `src/models/parameter-files/lphi4.in` except for `N`, which is the size of the lattice and was specified through the command line to be $N=64$. Note that **when passing arguments through the command-line, you should not use spaces around the equal sign**, so e.g. `N=64` is correct, but `N = 64` is not. If you want to pass arguments that take multiple values, you should protect the values by double quotes as in the following example,
```bash
./lphi4 input=../src/models/parameter-files/lphi4.in initial_momenta="0 0" N=64
```
In this case, both fields are initialized with zero velocity, and the lattice size is set to $N=64$.

A table of the most important parameters is the following:

| **Parameters** <div style="width:150px"> | **Explanation**                                                                                                                                                                                                                                                                                                                                                 |
| ---------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `N`                                      | Number of lattice points per dimension.                                                                                                                                                                                                                                                                                                                         |
| `kIR`                                    | Infrared cutoff of the lattice **in program units**, i.e. $\tilde{k}_{\rm IR} \equiv k_{\rm IR} / \omega_*$.                                                                                                                                                                                                                                                    |
| `lSide`                                  | Length of the box **in program units**, i.e. $\tilde{L} \equiv L \omega_*$.                                                                                                                                                                                                                                                                                     |
| `dt`                                     | Time step of the evolution algorithm **in program units**, i.e. $\delta \tilde{\eta}$.                                                                                                                                                                                                                                                                          |
| `expansion`                              | Expanding universe or not. If `false`, the scale factor is fixed to unity and field dynamics occur in Minkowski. If `true` (default value), the scale factor evolves self-consistently according to the Friedmann equations. A fixed background expansion rate can be further specified by the parameter `fixedBackground`.                                     |
| `evolver    `                            | Type of evolution algorithm. Options 'VV2', 'VV4', 'VV6', 'VV8', and 'VV10' solve the field equations with the velocity-verlet algorithm of the corresponding order, while 'LF' solves them with the staggered-leapfrog method. Check [ https://www.cosmolattice.net/technicalnotes](https://www.cosmolattice.net/technicalnotes) for addition of new evolvers. |
| `t0`                                     | Initial time of the simulation **in program units** (set to 0 by default).                                                                                                                                                                                                                                                                                      |
| `tMax`                                   | Final time of the simulation **in program units**.                                                                                                                                                                                                                                                                                                              |
| `tOutputFreq`                            | Time interval between the printing of *frequent output* in program units.                                                                                                                                                                                                                                                                                       |
| `tOutputInfreq`                          | Time interval between the printing of *infrequent output* in program units.                                                                                                                                                                                                                                                                                     |
| `tOutputRareFreq`                        | Time interval between the printing of *very infrequent (rare) output* in program units.                                                                                                                                                                                                                                                                         |
| `kCutOff`                                | If specified, the given cutoff (**in program units**) is imposed in the spectrum of initial fluctuations for all scalar fields: the amplitude of the field modes at larger momenta is set to zero up to machine precision. Not specifying `kCutOff` implies not having an initial cut-off, whereas `kCutOff = 0` implies initially vanishing fluctuations.      |
| `fixedBackground`                        | If set to `true`, turns off the self consistent expansion and replace it by a fixed background expansion.                                                                                                                                                                                                                                                       |
| `omegaEoS`                               | Barotropic equation of state parameter $\omega\equiv p/\rho$ required for a fixed background expansion. Note that **fractions are not allowed**, so one must write e.g. for a RD universe, '`omegaEoS=0.333`' instead of '`omegaEoS=1/3`'.                                                                                                                      |
| `H0`                                     | Initial Hubble rate (**in GeV**) used for the fixed background expansion.                                                                                                                                                                                                                                                                                       |

An exhaustive list of all available parameters is given in Appendix [Appendix: Parameters](Appendix: Parameters.md). Let us remark that many of these parameters have default values, so they do not need to be specified unless needed otherwise. However, there are certain parameters that do not have a default value, which we call *mandatory* parameters (such as $N$). These must be always specified.

#### Outputs

The code generates three different kinds of output files, classified according to the information they contain:

-  **Averages:** Volume-averages of field quantities (e.g. mean amplitude, variance), or other quantities that are independent of the lattice site (e.g. scale factor). Their printing frequency is controlled by the parameter `tOutpufFreq`.
-  **Spectra:** Binned spectra of fields and other quantities in momentum space. Their printing frequency is controlled by the parameter `tOutpufInfreq`. Their computation is generally more time-consuming than averages, as they imply Fourier transforming the whole lattice forth and back.
-  **Snapshots:** Values of a certain quantity (such as energy components) at all points of the lattice. These files are printed in HDF5 format, and their printing frequency is controlled by the parameter `tOutputRareFreq`. Their computation is also typically  time-consuming, and the produced files are significantly heavier than other files.

When the simulated model contains only scalar singlets, the files generated by the simulation and the information they contain are the following:

-  `average_energies.txt`: Energy density volume-averaged components in the following order:

$\tilde{\eta}$, $\tilde{E}_K^{(0)}$, $\tilde{E}_G^{(0)}$, ... , $\tilde{E}_K^{(N_s-1)}$, $\tilde{E}_G^{(N_s-1)}$, $\tilde{E}_V^{(0)}$, ... , $\tilde{E}_V^{(N_p-1)}$, $\langle \tilde{\rho} \rangle$.

-  `average_energy_conservation.txt`:

- [$\star$] If there is no expansion, it prints the relative degree of energy conservation as follows:

$\tilde{\eta}$, $1 - \frac{\langle \tilde{\rho} (\tilde{\eta} ) \rangle}{\langle \tilde{\rho} (\tilde{\eta}_*  ) \rangle}$.

- [$\star$]  In the case of self-consistent expansion, it prints the degree of relative conservation of the Hubble constraint as follows [here LHS and RHS are the left and hand sides of Eq. ([*43*][eq_NewFriedmannEQsI])]:

$\tilde{\eta}$, $\frac{\langle\text{LHS} - \text{RHS}\rangle}{\langle \text{LHS} + \text{RHS}\rangle}$, $\langle  \text{LHS} \rangle$, $\langle \text{RHS} \rangle$.

-  `average_scalar_[n].txt`: One file is produced for each individual scalar field, containing the following averages: $\tilde{ \eta}$, $\langle \tilde{\phi}_n \rangle$, $\langle \tilde{\phi}'_n \rangle$, $\langle \tilde{\phi}_n^2 \rangle$, $\langle \tilde{\phi}^{'2}_n \rangle$, $\text{rms} (\tilde{\phi}_n)$, $\text{rms} (\tilde{\phi}'_n)$.

-  `average_scale_factor.txt`: Scale factor and their derivatives: $\tilde{\eta}$, $a$, $a'$, $a' \over a$.

-  `spectra_scalar_[nfld].txt`: One file is produced for each individual scalar field, in which the following data is printed: $\tilde{k}$,  $\widetilde{\Delta}_{\tilde \phi} (\tilde k)$, $\widetilde{\Delta}_{\tilde \phi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, and $\Delta n_{bin}$ (multiplicity = lattice sites/bin), where $\tilde k \equiv k/\omega_*$, the dimensionless power spectra are related to their dimensionful counterparts [see Eq. ([*24*][eq_discretePS])] by $\Delta_{\phi} \equiv \widetilde \Delta_{\tilde\phi}f_*^2$, and $\Delta_{\phi'} \equiv \widetilde\Delta_{\tilde\phi'}f_*^2\omega_*^2$, and we have defined a (dimensionless) *lattice occupation number* as
[](){ #eq_OccuppationNum }
```math
\tag{44}
{\tilde n}_{\tilde k}(\tilde{\bf n}) = \frac{a^2\tilde{L}^3}{2N^6} \frac{f_*^2}{\omega_*^2} \left( \tilde{\omega}_{\tilde k(\tilde{\bf n})} \left\langle\Big| \tilde \phi_{\tilde k(\tilde{\bf n})}\Big|^2\right\rangle_{R(\tilde {\bf n})} + \frac{a^{2(1 - \alpha)}}{\tilde{\omega}_{\tilde k(\tilde{\bf n})}} \left\langle\Big| \tilde{\phi}_{\tilde k(\tilde{\bf n})}' + \frac{a'}{a} \tilde{\phi}_{\tilde k(\tilde{\bf n})}  \Big|^2\right\rangle_{R(\tilde {\bf n})} \right)    ,
```

where $\left\langle ... \right\rangle_{R(\tilde {\bf n})}$ denotes an angular average within the spherical shells of each bin, and we have defined $\tilde{\omega}_{\tilde k(\tilde{\bf n})}^2 \simeq {\tilde k^2(\tilde{\bf n})} + a^{2} \left\langle \frac{\partial^2 \tilde{V} }{\partial \tilde{\phi}^2}\right\rangle_{L^3}$, with $\left\langle ... \right\rangle_{L^3}$ a volume average. We note that the occupation number ${\tilde n}_{\tilde k}(\tilde{\bf n})$ is independent of either $N$ or $\delta \tilde x$ at a given $\tilde n$. However, the total number density of particles $n_\phi \equiv \int {d^3k\over(2\pi)^3} n_k = {\tilde n}_\phi\omega_*^3$ with ${\tilde n}_\phi \simeq {1\over {\tilde L}^3}\sum_{\tilde {\bf n}}{\tilde n}_{\tilde k(\tilde{\bf n})}$, may depend of the choice of $N$ and $\delta \tilde x$, as these determine the infrared and ultraviolet extremes of momenta in the reciprocal lattice.

-  `average_spectra_times.txt`: List of times at which the above spectra are outputted.

-  `[energy_term]_scalar.h5`: If indicated in the parameters file, these files contain the entire distribution throughout the lattice of a given energy component (e.g. kinetic, gradient, potential).

-  `[model_name].infos`: Information about the run, such as parameter values, time of onset and end of the simulation, etc.

!!! note
    **Important Note:** Definitions of the output variables just defined may vary in successive updates of CosmoLattice. For instance, whereas the form to calculate the scalar power spectrum $\widetilde{\Delta}_{\tilde \phi} (\tilde k)$ was unique in CosmoLattice `v1.0`, in CosmoLattice `v1.1` one can choose between multiple options to output different versions of a scalar field power spectrum, see [ `Technical Note I](https://cosmolattice.net/technicalnotes/)` for further details. In general, we invite the user to visit regularly [ https://www.cosmolattice.net/technicalnotes](https://www.cosmolattice.net/technicalnotes) to check for new features (ranging from new definitions, options, algorithms, interactions, etc) incorporated in successive updated versions of CosmoLattice.

### The model file { #sec_TheModelFile }

To define a model, the only file we really need to modify/create is the corresponding *model file* specified through the \textcolor{gray}{`-DMODEL=...`} argument of CMake. In the previous example, the model file used was `src/models/lphi4.h`. In this section, we will review carefully the contents of such file, so that you can imitate its structure to write a new `model.h` file, for the simulation of any other scenario with (canonically normalized) interacting (singlet) scalar fields.

#### Definition and declaration of the model { #subsubsec_DefAndDeclModel }

The first thing we need to do is to specify the matter content of our theory. In our model example we have two scalar fields, with a potential composed by the sum of two terms: the quartic potential of the inflaton, and the quadratic interaction between the inflaton and the preheat field, see Eq. ([*33*][eq_potentialExampleI]). This is indicated in the following extract of code:
@emgithub(models/lphi4.h:model_pars)

If we want to include mode fields with further potential interactions, we simply need to modify the values of `NScalars` and `NPotTerms` of the \textcolor{blue}{`ModelPars`} structure accordingly. We can also include other types of matter fields by adding extra parameters in this structure, such as gauge fields or complex singlets/doublets, but we wait for Section [My first model of gauge fields](My first model of gauge fields.md) to explain this. Once `NScalars` and `NPotTerms` are fixed, we give a name to our model:
@emgithub(models/lphi4.h:model_name)

**The name of the model must match the one of the file (without the .h extension)**. Following this prescription, the name of our example model is `lphi4`. This information is then passed to a macro `MakeModel` to generate a customizable skeleton class:
@emgithub(models/lphi4.h:make_model)

Our customized model is then derived from this skeleton, as follows:
@emgithub(models/lphi4.h:class_declaration)

#### Setting-up the model

The next step is to declare and define some **model specific parameters**, which can be used for example as an input for the different potential terms. In our example, these are mainly the inflaton self-coupling $\lambda$, and the coupling constant of the interaction $g$. This scenario is characterized by parametric resonance of the preheat field, so it is also convenient to introduce an additional third parameter called the *resonance parameter*, defined in terms of the other two as $q \equiv g^2/\lambda$. Parameters are declared in the model file as follows:
@emgithub(models/lphi4.h:private_members)

We now need to assign values to these parameters. We will do it inside the constructor of our model, namely the function in charge of its initialization:
@emgithub(models/lphi4.h:constructor_params)

Lines `67` and `68` are simply the declaration of our constructor. The argument `parser` is the *parameter parser* which we will use to add and get model specific arguments. The argument  `runPar` contains generic parameters such as the lattice spacing and the box size, see Section [Appendix: Parameters](Appendix: Parameters.md) for more information. The `toolBox` is an object that contains information about the internal mechanics of the library, and of which any model needs to be aware. For example, it is used to instantiate the field variables and perform iterations over the lattice, see Section [What CosmoLattice does in detail](What CosmoLattice does in detail.md) for more information. Anyhow, these two lines should not be modified, as they are only there to declare the constructor.

Customization starts on line `75`, where we declare a new parameter to be read either from the input file or the command line. To do so, we use the `parser` object and its `get<double>` function. The specification of `double` means that we are expecting a number with double precision. The argument '`lambda`' is the name of the parameter, which is specified as `lambda=...` in the input. The parameter `"q"` on line `81` is defined in the same way. On line `84` we  compute $g$ as a function of $\lambda$ and $q$.

We now need to initialize some generic variables of the skeleton model `Model`. In particular, we need to specify all the (non-zero) *initial homogeneous components* of the different scalar fields, the variables \{$\alpha$, $f_*$, $\omega_*$\} that will define our program variables, as well as the initial effective masses of the fields.

Regarding the initial homogeneous components of the fields, we also read them from the input file:
@emgithub(models/lphi4.h:initial_conditions)

The variables `fldS0` and `piS0` are arrays of doubles containing the homogeneous values of the scalar fields and their initial velocities. They are variables declared in the skeleton `Model<MODELNAME>`, see Section [What CosmoLattice does in detail](What CosmoLattice does in detail.md) for more information. We are using the same syntax as above to retrieve parameters from the input parameter file. The only novelty is the fact that now we read parameters that take multiple values. The size of the parameter is passed after the argument `double`, `2` in this case. The parameter `"initial_amplitudes"` is mandatory while `"initial_momenta"` is optional, as by default it takes the value zero in its entries. We note that the **field initial amplitudes and initial velocities must be introduced in the parameter file in units of GeV and GeV$^2$, respectively**. In the given example, we will consider from now on the field ''0" to be the inflaton $\phi$, and the field ''1" to be the daughter or preheat field $\chi$.

Next we set up the re-scaling, as described in Eqs. ([*29*][eq_FieldSpaceTimeNaturalVariables]),([*30*][eq_Alpha-PowLaw]),([*37*][eq_lphi4-ProgVar]):
@emgithub(models/lphi4.h:rescaling)

The code is self-explanatory. The parameters `fStar`, `omegaStar` and `alpha` are declared in the skeleton class.
The last step is to set up the masses.
@emgithub(models/lphi4.h:masses_setup)

Here, we use the default function which sets the masses from the second derivatives of the potential evaluated on the initial homogeneous values of the field, see Section [*The potential and its derivatives*][sec_PotDerivs] below. This function also computes the initial value of the potential, which is useful to initialise the Hubble rate. Were we want provide explicitly the initial field masses, the relevant parameter to be set is `masses2S`, which represents an array containing the square masses of the scalar fields.

#### The potential and its derivatives { #sec_PotDerivs }

The last and arguably the most important piece of information missing to be specified is the potential under consideration and its associated field derivatives. Let us start by defining the potential. We split it in `NPotTerms`, as specified on line `26` of the model file, two in our case.
@emgithub(models/lphi4.h:potential_terms)

Here we wrote in dimensionless units, the potential defined in Eq. ([*38*][eq_PotNat]), in this example split in two terms. For each term, we need to define a function called `auto potentialTerms(Tag<r>)`, with `r` an integer between $0$ and `NPotTerms`. In our case, we define on line `143` as the first potential term (numbered as the 'zeroth' term) the inflaton potential $\frac{1}{4}{\tilde \phi}^4$, whereas and on line `156` we define the second potential term describing the interaction between the inflaton and the daughter field $\frac{1}{2}q\tilde\phi^2\tilde\chi^2$. The object `fldS` is the object which contains the scalar fields. Individual fields are accessed using by calling `fldS(s)` with `s` the number of the scalar field species, running from $0$ to `NScalars - 1`. Fields constitute an object of their own in CosmoLattice, so they can be manipulated with many functions and (differential) operators, see Appendix [List of Implemented Functions](List of Implemented Functions.md) for an exhaustive list. Here we use two different such functions, namely multiplication (which can be used between two fields to represent their site-by-site multiplication, or between a field and a number) and an integer power function `pow<n>`, which computes locally the $n^{th}$ integer power of the field.

The `auto` keyword allows the compiler to automatically deduce the return type. In our case, it is essential as the expression we return are symbolic expression encoded inside the type, through the mechanism known as ''expression templates". We defer the interested reader to Appendix [Under the Hood: Expression Templates and CosmoLattice](Under the Hood: Expression Templates and CosmoLattice.md) for more information. For related reasons, the syntax `1_c` with the unusual `"_c"` is needed as it allows to simply defined compile-time integer.

In exactly the same manner, we introduce the potential derivatives:
@emgithub(models/lphi4.h:potential_derivs)

Let us highlight the fact that the numbering of these functions needs to be consistent with your numbering of the fields. By this we mean that the function `auto potentialTerms(Tag<0>)` corresponds to the derivative of the potential with respect to the numbered $0th$ field, `fldS(0_c)` in the code, and so on. The derivatives of the potential are used in the equations of motion.

Finally, we also provide the second derivative of the potential with respect to the scalar field (these are needed to compute the effective masses of the fields):
@emgithub(models/lphi4.h:potential_second_derivs)

With this we end our presentation of the model file. Any model consisting of canonically normalized interacting singlet scalar fields can be constructed in a similar manner.

### The physics implemented in CosmoLattice { #sec_WhatHappensAuto }

We discuss now what actions the code executes when running a simulation with a model we have just set up. Our aim here is to provide a short overview of the different parts of the code automatically called when running a simulation, so that the user can have a full picture of what is happening at the physical level. For a deeper understanding on the implementation details of this, we refer the reader to Section [What CosmoLattice does in detail](What CosmoLattice does in detail.md).

#### Initialization of fluctuations { #sec_InitScalar }

In most applications, on top of the corresponding initial homogeneous modes set up before, we require to initiate as well  a set of fluctuations for each of the simulated scalar fields. An extensive description of how to set initial conditions for scalars field in a lattice can be found in Section 7 of [@Figueroa_2020rrl]. Here we basically summarize the most important results, as well as explain their implementation in CosmoLattice.

Whenever considering initial quantum vacuum fluctuations, these can be written in the continuum as
[](){ #eq_SpectrumContinuum }
```math
\begin{align}\tag{45}
\left\langle \delta \phi^2 \right\rangle = \int d\log k \Delta_{\delta \phi}(k) ,\hspace{0.6cm} \Delta_{\delta \phi}(k) \equiv {k^3\over 2\pi^2} \mathcal{P}_{\delta \phi}(k) ,\hspace{0.6cm} \left\langle {\delta \phi}_{\bf k}{\delta \phi}_{{\bf k}'} \right\rangle \equiv (2\pi)^3\mathcal{P}_{\delta \phi} (k)\delta(\bf{k}-\bf{k}')  ,
\end{align}
```

where $\langle \cdots \rangle$ represents an ensemble average, and the power spectrum is given by
[](){ #eq_QuantumFlucts }
```math
\begin{align}
\Delta_{\delta \phi}(k) \equiv {k^3\over 2\pi^2} \mathcal{P}_{\delta \phi}(k) ,  \mathcal{P}_{\delta \phi} (k) \equiv {1\over 2a^2\omega_{k,\phi}} ,  \omega_{k,\phi} \equiv \sqrt{k^2 + a^2m_{\phi}^2}  ,  m_{\phi}^2 \equiv \frac{\partial^2 V}{\partial \phi^2}\Big|_{\phi = \bar{\phi}_*}  . \tag{46}
\end{align}
```

In this expression, $\omega_{k,\phi}$ is the comoving frequency of the mode, and $m_{\phi}$ is the effective mass of the field, evaluated in terms of the initial homogeneous amplitude $\bar{\phi}_*$ of the field.

In CosmoLattice, this is mimicked by imposing the following sum of left- and right-moving waves to the field amplitude at each lattice point in momentum space,
[](){ #eq_fpr_influct }
[](){ #eq_fpr_influct2 }
```math
\begin{align}
\delta \tilde \phi ({  \bf \tilde{n}}) &= \frac{1}{\sqrt{2}} (|\delta \tilde \phi_1 ({  \bf \tilde{n}})|  e^{i \theta_1 ({   \bf \tilde{n}}) } + |\delta \tilde \phi_2 ({   \bf \tilde{n}})| e^{i \theta_2 ({   \bf \tilde{n}}) }   ) \tag{47}  , \\
\delta \tilde {\phi}' ({   \bf \tilde{n}}) &= {1\over a^{1-\alpha}}\left[\frac{i \tilde{\omega}_k}{\sqrt{2}}  \left(|\delta \tilde \phi_1 ({   \bf \tilde{n}})| e^{i \theta_1 ({   \bf \tilde{n}}) } - |\delta \tilde \phi_2  ({   \bf \tilde{n}})| e^{i \theta_2 ({   \bf \tilde{n}}) }   \right)\right]  - \tilde{\mathcal{H}}  \delta \tilde \phi ({   \bf \tilde{n}}) , \tag{48}
\end{align}
```

where $\tilde{\omega}_k \equiv \sqrt{\tilde{k}^2(\tilde{\bf n}) + a^2\tilde{m}_{\phi}^2}$, and $\tilde{\mathcal{H}} \equiv a^\alpha H / \omega_*$. In these expressions, $\theta_1 ({\bf \tilde{n}})$ and $\theta_2 ({\bf \tilde{n}})$ are two random independent phases (at each Fourier site) drawn from a uniform distribution in the range $[0, 2\pi)$, and $|\delta \tilde{\phi}_1 ({\bf \tilde{n}})|$ and $|\delta \tilde{\phi}_2 ({\bf \tilde{n}})|$ are random amplitudes (set also at each Fourier site), drawn from a *Rayleigh* distribution with expected square amplitude given by
[](){ #eq_QuantumFlucts2 }
```math
\begin{align} \tag{49}
\left\langle | \delta \tilde \phi (\tilde{\bf n})|^2\right\rangle \equiv \left({\omega_*\over f_*}\right)^2\left({N\over \delta \tilde{x}}\right)^3{1\over 2a^2\sqrt{\tilde{k}^2(\tilde{\bf n}) + a^2\tilde{m}_{\phi}^2}}  , \hspace{0.6cm} \tilde{m}_{\phi}^2 \equiv \frac{\partial^2 \tilde{V}}{\partial \tilde{\phi}^2 } (\tilde{\phi} = \tilde{\bar{\phi}}_* )   .
\end{align}
```

Drawing both phases and modulus amplitudes as above is mathematically equivalent to drawing $\delta \tilde \phi ({  \bf \tilde{n}})$ and $\delta \tilde \phi'({  \bf \tilde{n}})$ as Gaussian random fields. The way in which this initialization is implemented in the code is discussed in more detail in Section [*Initializers*][subsec_Initializers].

#### Evolution of the system { #eq_evolution-sc }

Let us now explain how our evolution algorithms solve the field EOM and the evolution of the scale factor. For the purpose of this discussion, we will assume self-consistent expansion of the universe, as the cases of no expansion and fixed background are just a particularization of this. We also consider the case of one single scalar field, as the generalization to multiple fields is immediate.

The equations of motion are solved by using a Hamiltonian evolution scheme, in which we conveniently define the conjugate momenta of $\phi$ and $a$ as $\tilde\pi_\phi \equiv  a^{3-\alpha}\tilde\phi'$ and $b \equiv a'$ respectively. Note that a a given time, $\tilde\pi_\phi$ varies from point to point on the lattice, while $a$ is just a number. The equations of motion ([*41*][eq_EOMscalarContinuumNat]) and ([*42*][eq_NewFriedmannEQsII]) can then be written as the following set of four first-order differential equations,
```math
\begin{align}
\phi' &   =    a^{-(3 - \alpha)} \tilde\pi_\phi  ,  \\
a' &    =    b  ,  \\
(\tilde\pi_\phi)' &  =   \mathcal{K}_{\phi}[a,\tilde\phi]       \equiv     - a^{3 + \alpha} \widetilde V_{,\tilde\phi}  + a^{1 + \alpha} {\widetilde \nabla}^{2} \tilde\phi  , \\
b' &  =    \mathcal{K}_a\hspace{-1mm}\left[a,{\widetilde E}_K^\phi,{\widetilde E}_G^\phi,{\widetilde E}_V\right]
   \equiv    \frac{a^{2\alpha+1}}{3}{f_*^2\over m_p^2}\left[ (\alpha-2) {\widetilde E}_K^\phi + \alpha {\widetilde E}_G^\phi + (\alpha+1) {\widetilde E}_V  \right]  ,
\end{align}
```

where the kinetic, gradient, and potential energies of the field in program variables, are given by
```math
{\widetilde E}_K \equiv \frac{1}{2 a^{6} }\sum_{i}\left\langle \tilde\pi_i^2 \right\rangle ,  {\widetilde E}_G \equiv \frac{1}{2 a^2 }\sum_{i,k} \left\langle ({\widetilde\nabla}_k \tilde \phi_{i})^2 \right\rangle ,  {\widetilde E}_V \equiv \left\langle \widetilde{V}(\lbrace \tilde\phi_j\rbrace) \right\rangle
```

Above $\mathcal{K}_{\phi}$ and $\mathcal{K}_a$ are the *kernels* of $\phi$ and $a$ respectively. Note that the definition of $\tilde\pi_\phi$ has been chosen so that the kernels are not functions on the time-derivatives of their respective variables.

The current version of CosmoLattice has implemented already two different numerical schemes to solve these equations: *staggered leapfrog* and *velocity verlet*. The main difference between both schemes is the times at which the fields and momenta are defined during the evolution of the system. In staggered leapfrog, fields and momenta are specified at different times, so they must be synchronized each time an output is printed. On the contrary, in velocity verlet, fields and momenta are obtained at the same time, so no such synchronization is needed. A detailed account on the properties and ins and outs of how these algorithms work, can be found in Section 3 of [@Figueroa_2020rrl]. A derivation of the adaptation of these algorithms to the particular problem of the dynamics of (canonically normalized) interacting (singlet) scalar fields, can be found in Section 4 of [@Figueroa_2020rrl].

The use of standard $\mathcal{O}(\delta \tilde{\eta}^2)$ accurate staggered leapfrog and velocity verlet algorithms in CosmoLattice, can be specified in the input file as `evolver=LF` or `evolver=VV2`, respectively. Although both have an accuracy of order $\mathcal{O}(\delta \tilde{\eta}^2)$, `LF` only needs two steps by iteration, while `VV2` needs three. Therefore, `VV2` is slower than `LF`, typically by a factor $\sim$ 30$\%$-50$\%$ in our test runs. The velocity verlet algortihm has the advantage, however, that it is really a family of algorithms, which can be implemented with successive improved accuracy, from $\mathcal{O}(\delta \tilde{\eta}^4)$, to $\mathcal{O}(\delta \tilde{\eta}^6)$, $\mathcal{O}(\delta \tilde{\eta}^8)$, and $\mathcal{O}(\delta \tilde{\eta}^{10})$. These improved algorithms are already implemented in CosmoLattice, and to use them you simply need to specify in the parameter file, `evolver=VV4`, `VV6`, `VV8`, or `VV10`, respectively. Such improved algorithms conserve energy much better than `LP` or `VV2`, but they are naturally slower, as they require more steps per iteration (the more the higher the accuracy of the integrator). For a discussion on the construction of all these integrators and dedicated versions of them to the dynamics of (canonically normalized) interacting (singlet) scalar field dynamics, we refer again the reader to Sections 3 and 4 of [@Figueroa_2020rrl].

