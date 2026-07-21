<!-- <div style="text-align: justify;"> -->

In this section we explain, step by step, how to implement a model of interactive scalar fields in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. We focus on scalar fields that are not charged under a gauge symmetry, which will be referred to as *singlet-scalar* fields. <!-- For the dynamics of **charged scalar** fields, let it be under an Abelian or a non-Abelian gauge symmetry, see instead Section [Scalar-Gauge Interactions](My first model of gauge fields.md).  --> **This section is particularly relevant for a newcomer to $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$: besides explaining how to deal with singlet-scalar fields, it also introduces along the way relevant definitions and basic functionalities that are common to other field dynamics**. We introduce all the necessary concepts throughout the implementation of a basic two-field preheating scenario, which serves as a reference example. 

After following this section, the reader will have enough information to implement in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ any model involving interacting singlet-scalar fields. Readers interested in including gauge fields and their interactions with charged scalars, should go to Section [Scalar-gauge interactions](My first model of gauge fields.md). However, we only recommend to jump into scalar-gauge theories at this point, if the reader is already familiar with the lattice formulation of scalar-scalar interactions for singlets, that we provide precisely here below. 

This section is structured as follows.

<div class="grid cards cl-roadmap" markdown>

-   [*Program variables*][subsec_LatticeScalars] first introduces the concept of *program variables* for scalar fields, which are a new set of re-scaled dimensionless variables suitable for their introduction in a computer.

-   [*The model*][sec_ScTheModel] presents our example model and define its corresponding program variables and program potential.

-   [*My first run*][sec_MyFirstRun] explains how to compile and run the code.

-   [*The model file*][sec_TheModelFile] walks the user through the *model file*, where the model details are actually implemented.

-   [*The singlet-scalar physics implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$*][sec_WhatHappensAuto], presents a summarized picture of what happens 'under the hood' in the code, discussing details on how the fields are initialized in a simulation, and how their dynamical evolution is solved.

</div> <!-- and how different measurements are obtained.  -->


<!-- </div> -->

### **Program variables** { #subsec_LatticeScalars }

To describe the expansion of the universe, we consider a flat *Friedmann-Lemâitre-Robertson-Walker* (FLRW) background metric, described by the line element
[](){ #eq_FLRWmetric }
```math
\begin{equation}
\text{d} s^2 = g_{\mu\nu}\text{d} x^\mu\text{d} x^\nu = - a(\eta)^{2 \alpha} \text{d} \eta^2 + a(\eta)^2 \delta_{ij} \text{d} x^i \text{d} x^j  , %\tag{29}
\label{eq_FLRWmetric}
\end{equation}
```

where $a(\eta)$ is the scale factor, $\delta_{ij}$ is the Euclidean metric, and $\alpha$ is a constant parameter that we can freely choose conveniently in each scenario. The choice $\alpha = 0$ identifies $\eta$ with *cosmic time* $t$, whereas $\alpha = 1$ identifies it with *conformal time* $\tau \equiv \int {dt'  a^{-1}(t')}$. Independently of the value of $\alpha$, we will refer to $\eta$, from now on, as the **$\alpha$-time variable**.  

!!! note "Program Variables"
    <!-- **Important:** --> On the lattice, we operate with a particular set of dimensionless field and spacetime variables, $\{\tilde{\phi},\tilde{\eta},\tilde{x}^i\}$, which we call **program variables**. The transformation from 'physical' to program variables, $t \rightarrow \tilde{\eta}$, $x^i \rightarrow \tilde{x}^i$, and $\phi \rightarrow \tilde{\phi}$, is given by the following relations 
    [](){ #eq:FieldSpaceTimeNaturalVariables }
    ```math
    \begin{align}
    %\tag{30}
    \tilde\phi \equiv {\frac{\phi}{f_*}} ,  d\tilde\eta \equiv a^{- \alpha}  \omega_* dt ,  d\tilde x^i \equiv \omega_* dx^i ,
    \label{eq_FieldSpaceTimeNaturalVariables}
    \end{align}
    ```
    
    where $f_*$ and $\omega_*$ are two constants with dimensions of energy. Program variables, and any quantity defined in terms of them, will be tagged in this manual with the *diacritic* symbol '$\sim$'. The program variable re-scalings for space and time variables, $d\tilde\eta, d\tilde x^i$, will be universal in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, independently of the matter field content. Program variable re-scalings for matter fields other than singlet scalar fields, will be introduced later on, when dealing *e.g.* with gauge fields or fluid dynamics. 

**Choosing $f_*$ and $\omega_*$**
{: .cl-minihead }

Before simulating any particular model with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, the user must choose a certain set of values for $\{f_*,\omega_*,\alpha\}$, which will define the program variables used on the lattice via Eq.$~$\eqref{eq_FieldSpaceTimeNaturalVariables}. <!-- Eq. \eqref{eq_FieldSpaceTimeNaturalVariables}. --> The choice of $f_*$ and $\omega_*$ can be made arbitrary, as they only re-scale variables by constant factors. However, if we simulate a scenario in which, e.g. a resonance is triggered by an oscillatory field, it can be convenient to set $f_*$ and $\omega_*$ to the initial amplitude and oscillation frequency of that field respectively. This way, the numbers produced by the code will be close to unity, which will help with the interpretation of results. In general, in every scenario there is always a natural choice (at least of the order of magnitude) of $f_*$ and $\omega_*$, related to the typical field amplitudes and time scales of the problem. Choosing such natural values will help to interpret more easily --in a more intuitive physical manner--, the numbers that the code outputs.

**Choosing $\alpha$**
{: .cl-minihead }

A correct choice of $\alpha$ is perhaps more relevant and subtle. A wrong choice of $\alpha$ could actually spoil the stability of the numerical solution at late times. To see this, let us go back to the case of an oscillating homogeneous scalar field that dominates the energy budget of the Universe. Typical evolution algorithms operate with a constant time step, so it would be a good idea to choose $\alpha$ so that the oscillation frequency of the dominant field species is approximately constant, when expressed in the corresponding $\alpha$-time. In this way, we will be able to resolve each physical oscillation with a similar accuracy. For example, let us consider the common case of an oscillatory field, with monomial potential $V(\phi) \propto \phi^p$, that sources the expansion of the Universe. As described in $\mathtt{The~Art{\text -}I}$[@Figueroa_2020rrl] [see there discussion around $Eqs.\,(457)-(464)$], the oscillation frequency will be initially constant if we choose
[](){ #eq_Alpha-PowLaw }
```math
\begin{equation}
\alpha = 3 \left( \frac{p-2}{p+2} \right)  . %\tag{31}
\label{eq_Alpha-PowLaw}
\end{equation}
```

We therefore recommend to use Eq. \eqref{eq_Alpha-PowLaw} for any scenario where there is an energetically dominant scalar field with potential $V(\phi) \propto \phi^p$. A more convenient choice of $\alpha$ in other scenarios must be done in a case by case basis.

??? note "Observation — relation to LatticeEasy"
    Readers familiar with $\texttt{LatticeEasy}$[@Felder_2000hq], might have notticed that the above transformations are similar to those carried out in that code to define their program variables, $\tilde\phi \equiv a^{r}A\phi, d\tilde\eta \equiv a^{s}Bdt,  d\tilde \equiv x^i B dx^i$, if we set $A = 1/f_*$, $B= \omega_*$, $r=0$, and $s=-\alpha$ in their notation. As we will see, contrary to $\texttt{LatticeEasy}$, the evolution algorithms implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ *don't require a conformal rescaling of the fields*, so we do not need to introduce a parameter analogous to $r$.


### **A simple model** { #sec_ScTheModel }

<div class="cl-facts" markdown>
[example model: lphi4](#sec_TheModelFile){ .cl-fact }
[2 scalar fields](#eq_ScalarActionCont){ .cl-fact }
[2 potential terms](#eq_potentialExampleI){ .cl-fact }
[quartic inflaton + interaction](#eq_potentialExampleI){ .cl-fact }
[preheating scenario](#sec_MyFirstRun){ .cl-fact }
</div>

We will consider a preheating scenario for illustrative purposes, consisting of an inflaton $\phi$ with quartic potential $V(\phi) \propto \phi^4$, coupled to a secondary massless scalar field $\chi$ through a quadratic interaction $\propto  \phi^2\chi^2$. We denote the total number of scalar fields in a model as $N_s$, so in our present example $N_s=2$. The action of the field theory we want so simulate in this case is
[](){ #eq_ScalarActionCont }
[](){ #eq_potentialExampleI }
```math
\begin{align} %\tag{32}
\label{eq_ScalarActionCont}
S_{\rm S} &= - \int d\eta d^3 x a(\eta)^{3+\alpha} \left\{{1\over2} \partial^{\mu} \phi \partial_{\mu}\phi +{1\over2} \partial^{\mu} \chi \partial_{\mu}\chi + V(\phi,\chi)\right\}  , \\
\label{eq_potentialExampleI}
V(\phi,\chi) &\equiv \sum_{m=0}^{N_p-1} V^{(m)} (\phi, \chi) =  \frac{\lambda}{4}\phi^4 +\frac{1}{2}g^2 \phi^2\chi^2 , %\tag{33}
\end{align}
```

where $V(\phi, \chi)$ is the scalar potential, and $\lambda$ and $g^2$ are dimensionless coupling parameters. The potential contains two different terms: the quartic potential of the inflaton and the interaction between both fields, which we denote as $V^{(m)}$ with $m=0,1$ respectively. The total number of potential terms is denoted as $N_p$, as in this case we have $N_p = 2$. Indices are raised/lowered using the FLRW metric defined in Eq. \eqref{eq_FLRWmetric}, e.g. $\partial^{\mu} \phi \partial_{\mu}\phi=g^{\mu\nu}\partial_\mu\phi\partial_\nu\phi$. 

!!! note
    We reserve the symbol $\dot f \equiv {df/dt}$ for derivatives with respect to cosmic time, and $f' \equiv {df/ d\eta}$ for derivatives with respect to $\alpha$-time.

The field equations of motion in $\alpha$-time read <!-- [@Figueroa_2020rrl] -->
[](){ #eq_scEOM }
[](){ #eq_scEOM_II }
```math
\begin{align}\label{eq_scEOM}
\phi'' - a^{-2(1 - \alpha)} {\vec\nabla}^{ 2} \hspace{-1mm}\phi + (3 - \alpha)\frac{{a'}}{a} {\phi'} &= - a^{2 \alpha} V_{,\phi} \equiv - a^{2 \alpha} (\lambda \phi^3 +g^2\phi\chi^2) %\tag{34}\\
, \\
\label{eq_scEOM_II}
\chi'' - a^{-2(1 - \alpha)} {\vec\nabla}^{ 2} \hspace{-1mm}\chi + (3 - \alpha)\frac{{a'}}{a} {\chi'} &= - a^{2 \alpha} V_{,\chi} \equiv - a^{2 \alpha} g^2\phi^2\chi  ,
\end{align}
```

with the evolution of the scale factor $a(\eta)$ given by the Friedmann equations, which take one of two forms depending on the scenario:

=== "Self-consistent expansion"

    If these two fields constitute the only energy sources in the Universe (or at least the dominant ones), either of the Friedmann equations
    [](){ #eq_sfEOM }
    [](){ #eq_sfEOM_II }
    ```math
    \begin{align}\label{eq_sfEOM}
    \mathcal{H}^2 \equiv  \left({a'\over a}\right)^2 &=  \frac{a^{2 \alpha}}{3 m_p^2}\left\langle  {K}  + {G} +  {V} \right\rangle,\\
    \label{eq_sfEOM_II}
    {a''\over a} &= \frac{a^{2 \alpha}}{3 m_p^2}\left\langle (\alpha-2)  {K} + \alpha  {G} + (\alpha + 1)  {V}  \right\rangle  ,  %\tag{35}
    \end{align}
    ```

    can be solved self-consistently, together with the fields' equations of motion. Here $\langle \dots \rangle$ indicates a volume average, and $K$ and $G$ are the total kinetic and gradient energies. All scalar fields contribute to these quantities as $K \equiv \sum_{n=0}^{N_s-1} {K}^{(n)}$ and $G \equiv \sum_{n=0}^{N_s-1} {G}^{(n)}$, with
    [](){ #eq_KG }
    ```math
    \begin{equation}
    {K}^{(n)} = \frac{1}{2 a^{2\alpha} } \phi_n^{'2}  ~, \hspace{0.4cm} {G}^{(n)} = \frac{1}{2 a^2} \sum_i (\nabla_i \phi_n)^2  ~, %\tag{36}
    \label{eq_KG}
    \end{equation}
    ```
    where $\phi_0 \equiv \phi$ and $\phi_1 \equiv \chi$.

=== "Fixed-background expansion"

    In other scenarios, one could have the expansion of the Universe to be fixed by an external, energetically-dominant fluid with (constant) equation of state $w$. In this case, the evolution of the scale factor and the Hubble parameter in program variables is given by the following functions,
    [](){ #eq_ScaleFactorPowerLaw }
    ```math
    \begin{equation}
    %\tag{37}
    a(\tilde \eta) = a (\tilde \eta_* ) \left(1 + \frac{1}{p}\mathcal{H}_* (\tilde \eta- \tilde\eta_*) \right)^p  ,\hspace{0.3cm} \mathcal{H}(\eta) = {\mathcal{H}_*\over \left(1 + \frac{1}{p}\mathcal{H}_* (\tilde \eta- \tilde \eta_*) \right)}   ,\hspace{0.5cm} p \equiv \frac{2}{3(1 + \omega) - 2 \alpha }  .
    \label{eq_ScaleFactorPowerLaw}
    \end{equation}
    ```

!!! note
    The volume average in Eqs. \eqref{eq_sfEOM}, \eqref{eq_sfEOM_II} must be considered over length scales sufficiently large compared to the excited wavelengths in the matter fields. Only then we obtain a well-defined notion of a ’homogeneous and isotropic’ expanding background, within the given volume.

As mentioned before, numerical simulations are carried out in the dimensionless program variables defined in Eq. \eqref{eq_FieldSpaceTimeNaturalVariables}. Therefore, we need to choose appropriate values for $\{ f_*, \omega_*, \alpha\}$ in this model. We take them as follows,
[](){ #eq_lphi4-ProgVar }
```math
\begin{align} %\tag{38}
\label{eq_lphi4-ProgVar}
f_*=\overline{\phi}_{*}~ ,~ ~ \omega_*=\lambda^{1/2}  \overline{\phi}_*~,  ~~\alpha=1\,,
\end{align}
```

where $\overline{\phi}_{*}$ and $\lambda^{1/2} \overline{\phi}_*$ are the amplitude and oscillation frequency of the inflaton at the end of inflation, see *e.g.* Ref. [@Greene_1997fu]. The constant $\alpha$ was chosen according to Eq. \eqref{eq_Alpha-PowLaw}, which guarantees that the oscillation frequency remains approximately constant as long as the oscillatory inflaton field $\phi$ dominates the energy budget.

In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, any theory involving scalar fields is implemented by means of the *program potential*, which is a dimensionless quantity defined in terms of the program variables. In our present example, it is written as 
[](){ #eq_PotNat }
```math
\begin{align}%\tag{39}
\label{eq_PotNat}
\widetilde V( \tilde\phi, \tilde\chi ) \equiv \frac{1}{f_*^2 \omega_*^2}V(f_*\tilde \phi,f_*\tilde\chi) = \frac{1}{4}\tilde\phi^4 +\frac{1}{2}\frac{g^2}{\lambda}\tilde\phi^2\tilde\chi^2  .
\end{align}
```

Similarly, we define each of the individual contributions of this quantity as $\widetilde{V}^{(m)} \equiv {V}^{(m)} /(f_*^2 \omega_*^2)$, with $m = 0, ..., N_p-1$, with $N_p$ the number of potential terms. In our example, $N_p = 2$, with ${V}^{(0)} \equiv \frac{1}{4}\tilde\phi^4$ and ${V}^{(1)} \equiv \frac{1}{2}\frac{g^2}{\lambda}\tilde\phi^2\tilde\chi^2$. We also define the following dimensionless variables for the *pressure* and the *energy densities* as
[](){ #eq_rhoProg }
```math
\begin{align}
\label{eq_rhoProg}
\widetilde{\rho} \equiv \frac{\rho}{f_*^2 \omega_*^2} = \widetilde{K} + \widetilde{G}  + \widetilde V  ~, ~~   \widetilde{p} \equiv \frac{p}{f_*^2 \omega_*^2} = \widetilde{K} - \frac{1}{3} \widetilde{\rm G} - \widetilde V  , %\tag{40}
\end{align}
```

with $\widetilde{V} = \widetilde{V}^{(0)} + \widetilde{V}^{(1)}$ given by Eq. \eqref{eq_PotNat}, and where we have introduced the following *program kinetic and gradient energies* as $\widetilde{K} \equiv \sum_{n=0}^{N_s-1} \widetilde{K}^{(n)}$ and $\widetilde{G} \equiv \sum_{n=0}^{N_s-1} \widetilde{G}^{(n)}$, with
[](){ #eq_KandGprogramUnits }
```math
\begin{align}%\tag{41}
\label{eq_KandGprogramUnits}
\widetilde{K}^{(n)} = \frac{1}{2 a^{2\alpha}} ({\tilde \phi_n}')^2   , \hspace{0.4cm}
\widetilde{G}^{(n)} = \frac{1}{2 a^2 }  \sum_{i} (\widetilde\nabla_i \tilde \phi_{n})^2  .
\end{align}
```

We denote the corresponding volume-averaged energy density components as
[](){ #eq_EK_EG_EV }
```math
\begin{align}%\tag{43}
\label{eq_EK_EG_EV}
{\widetilde E}_K \equiv \left\langle \widetilde{K} \right\rangle ,~  {\widetilde E}_G \equiv \left\langle \widetilde{G} \right\rangle ,~  {\widetilde E}_V \equiv \left\langle \widetilde{V} \right\rangle  ,
\end{align}
```

and their partial contributions as ${\widetilde E}_K^{(n)}$, ${\widetilde E}_G^{(n)}$ and ${\widetilde E}_V^{(m)}$ respectively.  Using the above notation, the equations of motion in the continuum, but already expressed in program variables, read

!!! note "Continuum Eq.'s of motion in program variables"
    [](){ #eq_EOMscalarContinuumNat }
    [](){ #eq_EOMscalarContinuumNat_II }
    [](){ #eq_NewFriedmannEQsII }
    [](){ #eq_NewFriedmannEQsI }
    
    ```math
    
    \begin{align}
    \label{eq_EOMscalarContinuumNat}
    \tilde\phi'' - a^{-2 (1  - \alpha )} \tilde\nabla^2 \tilde\phi + (3 - \alpha)\frac{a'}{a} \tilde\phi'_a  &= -  a^{2 \alpha} \widetilde V_{,\tilde\phi} %\tag{41} 
    \equiv -  a^{2 \alpha}\left( \tilde \phi^3 + \frac{g^2}{\lambda}\tilde\phi\tilde\chi^2\right) \\
    \label{eq_EOMscalarContinuumNat_II}
    \tilde\chi'' - a^{-2 (1  - \alpha )} \tilde\nabla^2 \tilde\chi + (3 - \alpha)\frac{a'}{a} \tilde\chi'_a  &= -  a^{2 \alpha} \widetilde V_{,\tilde\chi} 
    \equiv - a^{2 \alpha}  \frac{g^2}{\lambda}\tilde\phi^2\tilde\chi
    \\
    %\tag{42}
    \label{eq_NewFriedmannEQsII}
    {a''\over a} &=  \frac{a^{2\alpha}}{3} \left( \frac{ f_*}{m_p} \right)^2 \Big[ (\alpha - 2){\widetilde E}_{K}  + \alpha {{\widetilde E}_{G}} + (\alpha + 1 ) {{\widetilde E}_V} \Big]  ,\\
    %\tag{43}
    \label{eq_NewFriedmannEQsI}
    a'^{ 2} &= \frac{a^{2\alpha + 2}}{3} \left( \frac{ f_*}{m_p} \right)^2 \Big[ {\widetilde E}_{K}  + {{\widetilde E}_{G}} + {{\widetilde E}_V} \Big]  .
    \end{align}
    
    ```

We note that the numerical schemes implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ use the second-order differential Eq. \eqref{eq_NewFriedmannEQsII} to solve for the scale factor, whereas Eq. \eqref{eq_NewFriedmannEQsI} is used as a *constraint* equation to monitor the accuracy of the obtained solution.

### **My first run** { #sec_MyFirstRun }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ comes with a set of ready-to-run models, which are available in the folder $\texttt{models/}.~$ In particular, the file $\texttt{lphi4.h}$ contains the implementation of the model presented in the previous section, characterized by the potential given in Eq. \eqref{eq_potentialExampleI} [more precisely,  the program potential given in Eq. \eqref{eq_PotNat}]. We now show how to run the code and pass different parameters to the simulation. We also show how to modify/create model files in order to implement other scalar theories.

<!-- #### Compilation -->

<div class="cl-hero" markdown>

**Compilation**

First, we need to choose the location where the code will be compiled. This can be anywhere on your machine. 
 <!-- **except in $\texttt{src/}$ or any of its sub-folders**.--> 
As an example, let us create a $\texttt{build/}$ directory and move inside it, 
```bash
cd cosmolattice
mkdir build
cd build
```
$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ uses CMake for compilation (see Section [CMake Flags](../code/cmake.md) for more details). The model $\texttt{lphi4.h}$ is compiled by typing the following commands (<span style="color:red;">**CHANGE ?**</span>),
```bash
cmake -DMODEL=lphi4 ../
make cosmolattice
```

Some explanations are of order.  The last argument of the `cmake` command is the path to the CMake configuration file, which is located at the root of the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ folders. In our case, its relative path with respect to the $\texttt{build/}$ folder is $\texttt{../}$. The first argument `-DMODEL=lphi4` is passed to CMake, and tells it to compile the model $\texttt{lphi4.h}$. Changing this argument to any other model present inside the folder $\texttt{models/}$ will determine which model will be compiled. Note that this is not a $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$-specific CMake argument, see [CMake Flags](../code/cmake.md) for an exhaustive list.

!!! note "**Important Note**"
    Every time you call CMake, it is a good practice to first remove the $\texttt{CMakeCache.txt}$ file that was previously generated.

At this point, if everything went smoothly, you should have generated an executable named $\texttt{lphi4}$. 

[](){ #subsec_Input-Scalars }
**Running the program with an input parameter file**

Now that we have generated the executable $\texttt{lphi4}$, we are ready to run our first simulation as follows (<span style="color:red;">**CHANGE ?**</span>):
```bash
./lphi4 input=../models/parameter-files/lphi4.in
```

This will launch the model $\texttt{lphi4}$ with the parameters specified in the input file $\texttt{lphi4.in}$ located in $\texttt{models/parameter-files/}$.

</div>

<!-- If this is the case, move on to the next section. If not, continue reading. -->

<!-- #### Troubleshooting

A common problem that will happen to some users at this stage is that CMake does not find your FFTW installation, typically because it is not installed in a standard path. If that is the case, you can indicate the location of FFTW by calling CMake as follows,
```bash
cmake -DMODEL=lphi4 ../   #Does not work because your fftw3 is not found.
rm CMakeCache.txt         #We want to clear the CMake before running it again.
cmake  -DMYFFTW3_PATH="/path/to/fftw3/" -DMODEL=lphi4 ../   #And now this works!
```
with $\texttt{/path/to/fftw3/}$ the path where *fftw3* is located. You can also call `make clean-cmake` to remove the $\texttt{CMakeCache.txt}$ file.

If this solves your problem, you can avoid having to specify the FFTW path each time you compile by modifying line `52` of the $\texttt{CMakeLists.txt}$ file (located at the root of the $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ files), as highlighted below:

<div style="height:5px;"></div>

`CMakeLists.txt:`
@emgithub(CMakeLists.txt:fetchcontent_templat)
where, again $\texttt{/path/to/fftw3/}$ is the path where fftw3 is located.
-->

Let us have a look at the input parameter file:

$\texttt{models/parameter-files/lphi4.in}$

@emgithub(models/parameter-files/lphi4.in)

One of the perks of using $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is its very flexible way of handling parameters. The standard way of passing parameters to the program is to bundle them in an input file such as $\texttt{lphi4.in}$, and indicate its path when calling the program with the `input=...` argument. The structure of the input file is rather straightforward. First, if we want to pass a single parameter, we just write down its name followed by an equal sign, and then define its value. Second, if we are passing parameters that admit multiple values, these must be separated with white spaces, as e.g. in the following lines of $\texttt{lphi4.in}$, where we indicate the initial homogenous values of the field and conjugate momenta for both the inflaton and the daugther field: 

@emgithub(models/parameter-files/lphi4.in:IC)

And third, the character `#` is use for comments, so everything following such character in a given line will be ignored. **Note that the order in which the parameters are specified in an input file does not matter**. To sum up, the way of defining parameters in an input file is
```text
singleParameterName = value
multipleParametersName = value1 value2 value3 ...
```
A convenient feature of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is that we can also pass arguments directly through the console. Moreover, this feature can be used together with an input file: **even if the argument is already specified in the file, it will always be overwritten by the one passed through the command-line**. For instance,
```bash
./lphi4 input=../models/parameter-files/lphi4.in N=64
```
will launch $\texttt{lphi4}$ with the parameters specified in $\texttt{models/parameter-files/lphi4.in}$ except for `N`, which is the size of the lattice and was specified through the command line to be `N=64`. Note that **when passing arguments through the command-line, you should not use spaces around the equal sign**, **i.e. `N=64` is correct, but `N = 64` is not**. If you want to pass arguments that take multiple values, you should protect the values by double quotes as in the following example,
```bash
./lphi4 input=../models/parameter-files/lphi4.in initial_momenta="0 0" N=64
```
In this case, both fields are initialized with zero velocity, and the lattice size is set to `N=64`.

A table of the most important parameters is shown below. Most of these parameters must be specified in program units, *c.f.* Eq. \eqref{eq_FieldSpaceTimeNaturalVariables}, so that variables are dimensionless. For example, one must introduce the length side of the box as $\tilde{L} \equiv L \omega_*$, the infrared cutoff the lattice as $\tilde{k}_{\rm IR} \equiv k_{\rm IR}/\omega_*$, etc.

**Relevant input parameters**:

| **Parameters** <div style="width:150px"> | **Explanation**                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |
| ---------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `N`                                      | Number of lattice points per dimension.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| `kIR`                                    | Infrared cutoff of the lattice **in program units**, i.e. $\tilde{k}_{\rm IR} \equiv k_{\rm IR} / \omega_*$.                                                                                                                                                                                                                                                                                                                                                                                                                                                                |
| `lSide`                                  | Length of the box **in program units**, i.e. $\tilde{L} \equiv L \omega_*$.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| `dt`                                     | Time step of the evolution algorithm **in program units**, i.e. $\delta \tilde{\eta} = \omega_* \delta \eta$.                                                                                                                                                                                                                                                                                                                                                                                                                                                               |
| `expansion`                              | Expanding universe or not. If `false`, the scale factor is fixed to unity and field dynamics occur in Minkowski. If `true` (default value), the scale factor evolves self-consistently according to the Friedmann equations. A fixed background expansion rate can be further specified by the parameter `fixedBackground`, see below.                                                                                                                                                                                                                                      |
| `evolver    `                            | Type of evolution algorithm. For example, options 'VV2', 'VV4', 'VV6', 'VV8', and 'VV10' solve the field equations with the *velocity-verlet* algorithm of order 2, 4, 6, 8, or 10, respectively, while 'LF' solves the equations of motion with the second-order method *staggered-leapfrog*. Other evolvers are 'PV2', 'PV4', 'PV6', 'PV8', and 'PV10' for *position-verlet* and 'RK2', 'RK3', 'RK4' for *Runge-Kutta*. This list might change in time if new evolvers are added. For an exhaustive list of evolvers check (<span style="color:red;">**WHERE ?**</span>). |
| `t0`                                     | Initial time of the simulation **in program units** (set to 0 by default).                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  |
| `tMax`                                   | Final time of the simulation **in program units**.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| `tOutputFreq`                            | Time interval between the printing of *frequent output* **in program units**.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               |
| `tOutputInfreq`                          | Time interval between the printing of *infrequent output* **in program units**.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |
| `tOutputRareFreq`                        | Time interval between the printing of *very infrequent (rare) output* **in program units**.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| `kCutOff`                                | If specified, the given cutoff (**in program units**) is imposed in the spectrum of initial fluctuations for all scalar fields: the amplitude of the field modes at larger momenta is set to zero up to machine precision. Not specifying `kCutOff` implies not having an initial cut-off, whereas `kCutOff = 0` implies initially vanishing fluctuations.                                                                                                                                                                                                                  |
| `fixedBackground`                        | If set to `true`, turns off the self consistent expansion and replace it by a fixed background expansion.                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| `omegaEoS`                               | Barotropic equation of state parameter $\omega\equiv p/\rho$ required for a fixed background expansion. Note that **fractions are not allowed**, so one must write e.g. for a RD universe, '`omegaEoS=0.333`' instead of '`omegaEoS=1/3`'.                                                                                                                                                                                                                                                                                                                                  |
| `H0`                                     | Initial Hubble rate (**in GeV**) used for the fixed background expansion.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |

An exhaustive list of all available parameters is given in [Appendix: Parameters](Appendix_Parameters.md). Let us remark that many of these parameters have default values, so they do not need to be specified unless needed otherwise. However, there are certain parameters that do not have a default value, which we call *mandatory* parameters (such as $N$). These must be always specified.

!!! note "Important"
    The parameters $\lbrace \tilde{k}_{\rm IR}, \tilde{L}, \delta \tilde x \rbrace$ characterize the lattice length scales, but not independently, as they obey the relations $\tilde{k}_{\rm IR} = 2\pi /\tilde L$ and $\tilde L \equiv N\delta \tilde x$. As $N$ must be always specified by the user, only one of the previous three parameters above must be specified by the user for a simulation, whereas the remaining two parameters will be automatically computed by the code. For example, the user could indicate the value of $\tilde{k}_{\rm IR}$ so that the code automatically obtains $\tilde L = 2\pi/\tilde{k}_{\rm IR}$ and $\delta \tilde x = \tilde L / N$. (<span style="color:red;">**is $\delta\tilde x$ really a parameter, or is it always derived ?**</span>)

<!-- #### Outputs -->
[](){ #subsec_Outputs }
**Outputs**

The code generates three different kinds of output files, classified according to the information they contain:

<div class="grid cards cl-outkinds" markdown>

-   **Averages**

    Volume-averages of field quantities (e.g. mean amplitude, variance), or other quantities that are independent of the lattice site (e.g. scale factor). Their printing frequency is controlled by the parameter `tOutputFreq`.

-   **Spectra**

    Binned spectra of fields and other quantities in momentum space. Their printing frequency is controlled by the parameter `tOutputInfreq`. Their computation is generally more time-consuming than averages, as they imply Fourier transforming the whole lattice forth and back.

-   **Snapshots**

    Values of a certain quantity (such as energy components) at all points of the lattice. These files are printed in HDF5 format, and their printing frequency is controlled by the parameter `tOutputRareFreq`. Their computation is also typically  time-consuming, and the produced files are significantly heavier than other files.

</div>

When the simulated model contains only scalar singlets, the files generated by the simulation and the information they contain are the following:

<div class="grid cards cl-files" markdown>

-   `average_energies.txt`{ .cl-fname }

    Energy density volume-averaged components in the following order: (<span style="color:red;">**CHECK**</span>)

    $\tilde{\eta}$, $\tilde{E}_K^{(0)}$, $\tilde{E}_G^{(0)}$, ... , $\tilde{E}_K^{(N_s-1)}$, $\tilde{E}_G^{(N_s-1)}$, $\tilde{E}_V^{(0)}$, ... , $\tilde{E}_V^{(N_p-1)}$, $\langle \tilde{\rho} \rangle$
    {: .cl-schema }

-   `average_energy_conservation.txt`{ .cl-fname }

    If there is no expansion, it prints the relative degree of energy conservation as follows:

    $\tilde{\eta}$, $1 - \frac{\langle \tilde{\rho} (\tilde{\eta} ) \rangle}{\langle \tilde{\rho} (\tilde{\eta}_*  ) \rangle}$
    {: .cl-schema }

    In the case of self-consistent expansion, it prints the degree of relative conservation of the Hubble constraint as follows [here LHS and RHS are the left and hand sides of Eq. \eqref{eq_NewFriedmannEQsI}]:

    $\tilde{\eta}$, $\frac{\langle\text{LHS} - \text{RHS}\rangle}{\langle \text{LHS} + \text{RHS}\rangle}$, $\langle  \text{LHS} \rangle$, $\langle \text{RHS} \rangle$
    {: .cl-schema }

-   `average_scalar_[n].txt`{ .cl-fname }

    One file is produced for each individual scalar field, containing the following averages:

    $\tilde{ \eta}$, $\langle \tilde{\phi}_n \rangle$, $\langle \tilde{\phi}'_n \rangle$, $\langle \tilde{\phi}_n^2 \rangle$, $\langle \tilde{\phi}^{'2}_n \rangle$, $\text{rms} (\tilde{\phi}_n)$, $\text{rms} (\tilde{\phi}'_n)$
    {: .cl-schema }

-   `average_scale_factor.txt`{ .cl-fname }

    Scale factor and their derivatives:

    $\tilde{\eta}$, $a$, $a'$, $a' \over a$
    {: .cl-schema }

-   `spectra_scalar_[nfld].txt`{ .cl-fname }

    One file is produced for each individual scalar field, in which the following data is printed:

    $\tilde{k}$,  $\widetilde{\Delta}_{\tilde \phi} (\tilde k)$, $\widetilde{\Delta}_{\tilde \phi'} (\tilde k)$, ${\tilde n}_{\tilde k}$, $\Delta n_{bin}$ (multiplicity = lattice sites/bin)
    {: .cl-schema }

</div>

where $\tilde k \equiv k/\omega_*$, the dimensionless power spectra are related to their dimensionful counterparts [see Eq. ([*24*][eq_discretePS]) in <span style="color:red;">**WHERE ?**</span>] by $\Delta_{\phi} \equiv \widetilde \Delta_{\tilde\phi}f_*^2$, and $\Delta_{\phi'} \equiv \widetilde\Delta_{\tilde\phi'}f_*^2\omega_*^2$, $\Delta n_{bin}$ is the *multiplicity* (*i.e.* lattice sites/bin), and we have defined a (dimensionless) *lattice occupation number* as
[](){ #eq_OccuppationNum }
```math
\begin{equation}
%\tag{44}
{\tilde n}_{\tilde k}(\tilde{\bf n}) = \frac{a^2\tilde{L}^3}{2N^6} \frac{f_*^2}{\omega_*^2} \left( \tilde{\omega}_{\tilde k(\tilde{\bf n})} \left\langle\Big| \tilde \phi_{\tilde k(\tilde{\bf n})}\Big|^2\right\rangle_{R(\tilde {\bf n})} + \frac{a^{2(1 - \alpha)}}{\tilde{\omega}_{\tilde k(\tilde{\bf n})}} \left\langle\Big| \tilde{\phi}_{\tilde k(\tilde{\bf n})}' + \frac{a'}{a} \tilde{\phi}_{\tilde k(\tilde{\bf n})}  \Big|^2\right\rangle_{R(\tilde {\bf n})} \right)    ,
\label{eq_OccuppationNum}
\end{equation}
```

with $\left\langle ... \right\rangle_{R(\tilde {\bf n})}$ denoting angular averaging within the spherical shells of each bin, and $\tilde{\omega}_{\tilde k(\tilde{\bf n})}^2 \simeq {\tilde k^2(\tilde{\bf n})} + a^{2} \left\langle \frac{\partial^2 \tilde{V} }{\partial \tilde{\phi}^2}\right\rangle_{L^3}$, with $\left\langle ... \right\rangle_{L^3}$ a volume average. We note that the occupation number ${\tilde n}_{\tilde k}(\tilde{\bf n})$ is independent of either $N$ or $\delta \tilde x$ at a given $\tilde n$. However, the total number density of particles $n_\phi \equiv \int {d^3k\over(2\pi)^3} n_k = {\tilde n}_\phi\omega_*^3$ with ${\tilde n}_\phi \simeq {1\over {\tilde L}^3}\sum_{\tilde {\bf n}}{\tilde n}_{\tilde k(\tilde{\bf n})}$, may depend of the choice of $N$ and $\delta \tilde x$, as these determine the infrared and ultraviolet extremes of momenta in the reciprocal lattice.

<div class="grid cards cl-files" markdown>

-   `average_spectra_times.txt`{ .cl-fname }

    List of times at which the above spectra are outputted.

-   `[energy_term]_scalar.h5`{ .cl-fname }

    If indicated in the parameters file, these files contain the entire distribution throughout the lattice of a given energy component (e.g. kinetic, gradient, potential).

-   `[model_name].infos`{ .cl-fname }

    Information about the run, such as parameter values, time of onset and end of the simulation, etc.

</div>

??? note "Definitions may change between versions"
     Definitions of the output variables just defined may vary in successive updates of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. For instance, whereas the form to calculate the scalar power spectrum $\widetilde{\Delta}_{\tilde \phi} (\tilde k)$ was unique in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ `v1.0`, in later versions (starting from $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ `v1.1`) we enabled the option to choose between different versions of a scalar field power spectrum. Check [**Observables**](Observables.md) for up-to-date definitions, and 
     [**$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ Version Guide**](../code/Versions.md) to follow the addition of new features (ranging from new definitions, options, algorithms, interactions, etc), as incorporated in successive versions of the code. 

### **The model file** { #sec_TheModelFile }

To define a model, the only file we really need to modify/create is the corresponding *model file* specified through the `-DMODEL=...` argument of CMake. In the previous example, the model file used was $\texttt{lphi4.h}$. In the following, we will review carefully the contents of such file, so that you can imitate its structure to write a new model file $\texttt{myModel.h}$, for the simulation of any other scenario of (canonically normalized) interacting (singlet) scalar fields.

<!-- #### Definition and declaration of the model { #subsubsec_DefAndDeclModel } -->
[](){ #subsec_DefAndDeclModel }
**Definition and declaration of the model**

The first thing we need to do is to specify the matter content of our theory. In the above model example we have two scalar fields, with a potential composed by the sum of also two terms: the quartic potential of the inflaton, and the quadratic interaction between the inflaton and the daugther field, *c.f.* Eq. \eqref{eq_potentialExampleI}. This is indicated in the following extract of code of the file $\texttt{models/lphi4.h}$:
@emgithub(models/lphi4.h:model_pars)

If we want to include more fields and/or further potential interactions, we simply need to modify the values of `NScalars` and `NPotTerms` of the <code style="color:purple;">ModelPars</code> <!-- `ModelPars`  --> structure accordingly. We can also include other types of fundamental fields by adding extra parameters in this structure, such as gauge fields or complex/doublet scalars, but we wait for Section [Scalar-Gauge Interactions](My first model of gauge fields.md) to explain this. Once `NScalars` and `NPotTerms` are fixed, we give a name to our model:
@emgithub(models/lphi4.h:model_name)

**The name of the model must match the one of the file (without the .h extension)**. Following this prescription, the name of our example model is $\texttt{lphi4}$. This information is then passed to a macro `MakeModel` to generate a customizable skeleton class:
@emgithub(models/lphi4.h:make_model)

Our customized model is then derived from this skeleton, as follows:
@emgithub(models/lphi4.h:class_declaration)

<!-- #### Setting-up the model -->
[](){ #subsec_SetUp }
**Setting-up the model**

The next step is to declare and define some **model specific parameters**, which can be used for example as an input for the different potential terms. In our example, these are mainly the inflaton self-coupling $\lambda$, and the coupling constant of the interaction $g$. This scenario is characterized by parametric resonance of the preheat field, so it is also convenient to introduce an additional third parameter called the *resonance parameter*, defined in terms of the other two as $q \equiv g^2/\lambda$. Parameters are declared in the model file as follows:
@emgithub(models/lphi4.h:private_members)

We now need to assign values to these parameters. We will do it inside the constructor of our model, namely the function in charge of its initialization:
@emgithub(models/lphi4.h:constructor_params)

The two lines starting by `MODELNAME(ParameterParser &parser, ...` and `Model<MODELNAME>(parser, ...`, respectively, are simply the declaration of our constructor. The argument `parser` is the *parameter parser* which we will use to add and get model specific arguments. The argument  `runPar` contains generic parameters such as the lattice spacing and the box size, see [Appendix: Parameters](Appendix_Parameters.md) for more information. The `toolBox` is an object that contains information about the internal mechanics of the library, and of which any model needs to be aware. For example, it is used to instantiate the field variables and perform iterations over the lattice, see Section [**Inside Cosmolattice**](What $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ does in detail.md) for more information. Anyhow, **these two lines should not be modified, as they are only there to declare the constructor**.

Customization starts on line `lambda = parser.get< ...`, where we declare a new parameter to be read either from the input file or the command line. To do so, we use the `parser` object and its `get<double>` function. The specification of `double` means that we are expecting a number with double precision (<span style="color:red;">**FloatType in the example?**</span>). The argument '`lambda`' is the name of the parameter, which is specified as `lambda=...` in the input. The parameter `"q"` on line `q = parser.get< ...` is defined in the same way. On line `g = sqrt(...` we  compute $g$ as a function of $\lambda$ and $q$.

We now need to initialize some generic variables of the skeleton model `Model`. In particular, we need to specify the non-vanishing *initial homogeneous components* of the different scalar fields, the variables \{$\alpha$, $f_*$, $\omega_*$\} that will define our program variables, as well as the initial effective masses of the fields ($i.e.$ second derivatives of the potential).

Regarding the initial homogeneous components of the fields, we also read them from the input file:
@emgithub(models/lphi4.h:initial_conditions)

The variables `fldS0` and `piS0` are arrays of doubles (<span style="color:red;">**floats?**</span>) containing the homogeneous values of the scalar fields and their initial velocities. They are variables declared in the skeleton `Model<MODELNAME>`, see Section [**Inside Cosmolattice**](What CosmoLattice does in detail.md) for more information. We are using the same syntax as above to retrieve parameters from the input parameter file. The only novelty is the fact that now we read parameters that take multiple values. The type of the parameter is passed as the first arguemt, <code style="color:red;">FloatType</code> in this case, whereas the 'size' of the parameter is passed as the second argument, `2` in our case, as there are two fields. The parameter `"initial_amplitudes"` is mandatory, while `"initial_momenta"` is optional, as by default it takes vanishing values if it is not specified. We note that the **field initial amplitudes and initial velocities must be introduced in the parameter file in units of GeV and GeV$^2$, respectively**. In the given example, we will consider from now on the field ''0" to be the inflaton $\phi$, and the field ''1" to be the daughter or preheat field $\chi$.

Next we set up the program variables' re-scalings, as described in Eqs. \eqref{eq_FieldSpaceTimeNaturalVariables},\eqref{eq_Alpha-PowLaw},\eqref{eq_lphi4-ProgVar}:
@emgithub(models/lphi4.h:rescaling)

The code is self-explanatory. The parameters `fStar`, `omegaStar` and `alpha` are declared in the skeleton class.

The last step is to set up the masses.
@emgithub(models/lphi4.h:masses_setup)

Here, we use the default function which sets the masses from the second derivatives of the potential evaluated on the initial homogeneous values of the field, see Section [*The potential and its derivatives*][sec_PotDerivs] below. This function also computes the initial value of the potential, which is useful to initialise the Hubble rate (<span style="color:red;">**This out?**</span>). If the user prefers instead to provide explicitly the initial field masses, the relevant parameter to be set is `masses2S`, which represents an array containing the square masses of the scalar fields (evaluated on the initial homogeneous values of the field) [<span style="color:red;">**This also out?**</span>].

<!-- #### The potential and its derivatives { #sec_PotDerivs } -->
[](){ #sec_PotDerivs }
**The potential and its derivatives**


The last and arguably the most important piece of information missing to be specified is the potential under consideration and its associated field derivatives. Let us start by defining the potential. We split it in `NPotTerms`, as specified on the line `static constexpr size_t NPotTerms = 2;` of the model file, $i.e.$ 2 in our case.
@emgithub(models/lphi4.h:potential_terms)

Here we wrote in dimensionless units, the potential defined in Eq. \eqref{eq_PotNat}, in this example split in two terms. For each term, we need to define a function called `auto potentialTerms(Tag<r>)`, with `r` an integer between $0$ and `NPotTerms-1`. In our case, we define on the line `return 0.25 * pow<4>(fldS(0_c));` as the first potential term (numbered as the 'zeroth' term) the inflaton potential $\frac{1}{4}{\tilde \phi}^4$, whereas and on the line `return 0.5 * q * pow<2>(...` we define the second potential term, describing the interaction between the inflaton and the daughter field, $\frac{1}{2}q\tilde\phi^2\tilde\chi^2$. The object `fldS` contains the scalar fields, so that individual fields are accessed by calling `fldS(s)`, with `s` the number of the scalar field species, running from $0$ to `NScalars - 1`. Fields constitute an object of their own in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, so they can be manipulated with many functions and (differential) operators, see the [**List of Implemented Functions**](List of Implemented Functions.md) for an exhaustive list. Here we use two different such functions, namely multiplication (which can be used between two fields to represent their site-by-site multiplication, or between a field and a number) and an integer power function `pow<n>`, which computes locally the $n^{th}$ integer-power of a field.

The keyword `auto` allows the compiler to automatically deduce the return type. In our case, it is essential as the expression we return are symbolic expression encoded inside the type, through the mechanism known as *''expression templates"*. We refer the interested reader to [Expression Templates](Expression Templates.md) for more information. For related reasons, the syntax `1_c` with the unusual `_c`, is needed as it allows to simply define compile-time integers.

In exactly the same manner, we introduce the potential derivatives:
@emgithub(models/lphi4.h:potential_derivs)

Let us highlight the fact that the numbering of these functions needs to be consistent with your numbering of the fields. By this we mean that the function `auto potentialTerms(Tag<0>)` corresponds to the derivative of the potential with respect to the numbered $0th$ field, `fldS(0_c)` in the code, and so on. The derivatives of the potential are used in the equations of motion.

Finally, we also provide the second derivative of the potential with respect to the scalar field (these are needed to compute the effective masses of the fields):
@emgithub(models/lphi4.h:potential_second_derivs)

With this we end our presentation of the model file. Any model consisting of canonically normalized interacting singlet scalar fields can be constructed in a similar manner.

### **Scalar physics inside $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$** { #sec_WhatHappensAuto }

We discuss now what actions the code executes when running a simulation with a model we have just set up. Our aim here is to provide a short overview of the different parts of the code automatically called when running a simulation, so that the user can have a full picture of what is happening at the physical level. For a deeper understanding on the implementation details of this, we refer the reader to Section [**Inside Cosmolattice**](What CosmoLattice does in detail.md).

<!-- #### Initialization of fluctuations { #sec_InitScalar } -->
[](){ #sec_InitScalar }
**Initialization of fluctuations**


In most applications, on top of the corresponding initial homogeneous modes set up before, we require to initiate as well  a set of fluctuations for each of the scalar fields. An extensive description of how to set initial conditions for scalars fields on a lattice can be found in Section 7 of $\,\texttt{The}\,\texttt{Art}$-$\texttt{I}$ [@Figueroa_2020rrl]. Here we basically summarize the most important results, as well as explain their implementation in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

Whenever considering initial fluctuations $\delta\phi$, these can be characterized in the continuum by their variance
[](){ #eq_SpectrumContinuum }
```math
\begin{align}%\tag{45}
\label{eq_SpectrumContinuum}
\left\langle \delta \phi^2 \right\rangle = \int d\log k~ \Delta_{\delta \phi}(k) ,\hspace{0.6cm} \Delta_{\delta \phi}(k) \equiv {k^3\over 2\pi^2} \mathcal{P}_{\delta \phi}(k) ,\hspace{0.6cm} \left\langle {\delta \phi}_{\bf k}{\delta \phi}_{{\bf k}'} \right\rangle \equiv (2\pi)^3\mathcal{P}_{\delta \phi} (k)\delta(\bf{k}-\bf{k}')  ,
\end{align}
```
where $\langle \cdots \rangle$ represents an ensemble average, and $\Delta_{\delta \phi}(k)$ a power spectrum. If the latter corresponds to quantum fluctuations, then we have
[](){ #eq_QuantumFlucts }
```math
\begin{align}
\label{eq_QuantumFlucts}
\Delta_{\delta \phi}(k) \equiv {k^3\over 2\pi^2} \mathcal{P}_{\delta \phi}(k) ,\hspace{0.6cm}  \mathcal{P}_{\delta \phi} (k) \equiv {1\over 2a^2\omega_{k,\phi}} ,\hspace{0.6cm}  \omega_{k,\phi} \equiv \sqrt{k^2 + a^2m_{\phi}^2}  ,\hspace{0.6cm}  m_{\phi}^2 \equiv \frac{\partial^2 V}{\partial \phi^2}\Big|_{\phi = \bar{\phi}_*}  . %\tag{46}
\end{align}
```

In this expression, $\omega_{k,\phi}$ is the comoving frequency of the mode, and $m_{\phi}$ is the effective mass of the field, evaluated in terms of the initial homogeneous amplitude $\bar{\phi}_*$ of the field.

**Realization on the lattice**
{: .cl-minihead }

In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, (<span style="color:orange;">**Jorge comment: THIS IS NOT TRUE. FOR SCALAR SINGETS COSMOLATTICE SAMPES FROM A GAUSSIAN**</span>) this is represented by imposing a sum of left- and right-moving waves of the field amplitude at each Fourier site ${\bf \tilde n}$,
[](){ #eq_fpr_influct }
[](){ #eq_fpr_influct2 }
```math
\begin{align}
\label{eq_fpr_influct}
\delta \tilde \phi ({  \bf \tilde{n}}) &= \frac{1}{\sqrt{2}} (|\delta \tilde \phi_1 ({  \bf \tilde{n}})|  e^{i \theta_1 ({   \bf \tilde{n}}) } + |\delta \tilde \phi_2 ({   \bf \tilde{n}})| e^{i \theta_2 ({   \bf \tilde{n}}) }   ) %\tag{47}  
, \\
\label{eq_fpr_influct2}
\delta \tilde {\phi}' ({   \bf \tilde{n}}) &= {1\over a^{1-\alpha}}\left[\frac{i \tilde{\omega}_k}{\sqrt{2}}  \left(|\delta \tilde \phi_1 ({   \bf \tilde{n}})| e^{i \theta_1 ({   \bf \tilde{n}}) } - |\delta \tilde \phi_2  ({   \bf \tilde{n}})| e^{i \theta_2 ({   \bf \tilde{n}}) }   \right)\right]  - \tilde{\mathcal{H}}  \delta \tilde \phi ({   \bf \tilde{n}}) , %\tag{48}
\end{align}
```

where $\tilde{\omega}_k \equiv \sqrt{\tilde{k}^2(\tilde{\bf n}) + a^2\tilde{m}_{\phi}^2}$, and $\tilde{\mathcal{H}} \equiv a^\alpha H / \omega_*$. In these expressions, $\theta_1 ({\bf \tilde{n}})$ and $\theta_2 ({\bf \tilde{n}})$ are two random independent phases drawn from a uniform distribution in the range $[0, 2\pi)$, and $|\delta \tilde{\phi}_1 ({\bf \tilde{n}})|$ and $|\delta \tilde{\phi}_2 ({\bf \tilde{n}})|$ are random amplitudes, drawn from a *Rayleigh* distribution with expected square amplitude given by
[](){ #eq_QuantumFlucts2 }
```math
\begin{align} %\tag{49}
\label{eq_QuantumFlucts2}
\left\langle | \delta \tilde \phi (\tilde{\bf n})|^2\right\rangle \equiv \left({\omega_*\over f_*}\right)^2\left({N\over \delta \tilde{x}}\right)^3{1\over 2a^2\sqrt{\tilde{k}^2(\tilde{\bf n}) + a^2\tilde{m}_{\phi}^2}}  , \hspace{0.6cm} \tilde{m}_{\phi}^2 \equiv \frac{\partial^2 \tilde{V}}{\partial \tilde{\phi}^2 }\Big|_{\tilde{\phi} = \tilde{\bar{\phi}}_*}\,.
\end{align}
```

Drawing both phases and modulus amplitudes as above is mathematically equivalent to drawing $\delta \tilde \phi ({  \bf \tilde{n}})$ and $\delta \tilde \phi'({  \bf \tilde{n}})$ as Gaussian random fields. The way in which this initialization is implemented in the code is discussed in more detail in Section [**Initializers**][subsec_Initializers].

<!-- #### Evolution of the system { #eq_evolution-sc } -->
[](){ #eq_evolution-sc }
**Evolution of the system**

Let us now explain how our evolution algorithms solve the field EOM and the evolution of the scale factor. For the purpose of this discussion, we will assume self-consistent expansion of the universe, as the cases of no expansion and fixed background are just a particularization of this. We also consider the case of one single scalar field, as the generalization to multiple fields is immediate.

The equations of motion are solved by using a Hamiltonian evolution scheme, in which we conveniently define the conjugate momenta of $\phi$ and $a$ as $\tilde\pi_\phi \equiv  a^{3-\alpha}\tilde\phi'$ and $b \equiv a'$ respectively. Note that at a given time, $\tilde\pi_\phi$ varies from point to point on the lattice, while $a$ is just a number. The equations of motion \eqref{eq_EOMscalarContinuumNat}, \eqref{eq_EOMscalarContinuumNat_II}, \eqref{eq_NewFriedmannEQsII}, \eqref{eq_NewFriedmannEQsI} can then be written as the following set of four first-order differential equations,
[](){ #eq_HamiltonEOMsinglets }
```math
\begin{align}
\phi' &   =    a^{-(3 - \alpha)} \tilde\pi_\phi  ,  \\
a' &    =    b  ,  \\
(\tilde\pi_\phi)' &  =   \mathcal{K}_{\phi}[a,\tilde\phi]       \equiv     - a^{3 + \alpha} \widetilde V_{,\tilde\phi}  + a^{1 + \alpha} {\widetilde \nabla}^{2} \tilde\phi  , \\
b' &  =    \mathcal{K}_a\hspace{-1mm}\left[a,{\widetilde E}_K^\phi,{\widetilde E}_G^\phi,{\widetilde E}_V\right]
   \equiv    \frac{a^{2\alpha+1}}{3}{f_*^2\over m_p^2}\left[ (\alpha-2) {\widetilde E}_K^\phi + \alpha {\widetilde E}_G^\phi + (\alpha+1) {\widetilde E}_V  \right]  ,
\label{eq_HamiltonEOMsinglets}
\end{align}%\tag{50}
```

where the kinetic, gradient, and potential energies of the field in program variables, are given by
[](){ #eq_Energysinglets }
```math
\begin{equation}
{\widetilde E}_K \equiv \frac{1}{2 a^{6} }\sum_{i}\left\langle \tilde\pi_i^2 \right\rangle ,  {\widetilde E}_G \equiv \frac{1}{2 a^2 }\sum_{i,k} \left\langle ({\widetilde\nabla}_k \tilde \phi_{i})^2 \right\rangle ,  {\widetilde E}_V \equiv \left\langle \widetilde{V}(\lbrace \tilde\phi_j\rbrace) \right\rangle %\tag{51}
\label{eq_Energysinglets}
\end{equation}
```

Above $\mathcal{K}_{\phi}$ and $\mathcal{K}_a$ are the **kernels** of $\phi$ and $a$ respectively. Note that the definition of $\tilde\pi_\phi$ has been chosen so that the kernels are not functions on the time-derivatives of their respective variables.

The current version of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ ($\texttt{v2.0}$) has implemented  different numerical schemes to solve these equations: *staggered leapfrog* (LF), *velocity verlet* (VV), *position verlet* (PV) and *Runge-Kutta* (RK). One difference between these schemes is the moments at which the fields and momenta are defined during the evolution of the system. In LF, fields and momenta are specified at different times, so they must be synchronized each time an output is printed. On the contrary, in VV, PV and RK, fields and momenta are obtained at the same time, so no such synchronization is needed. A detailed account on the properties and ins and outs of how these algorithms work, can be found in Section 3 of The-Art-I [@Figueroa_2020rrl]. A derivation of the adaptation of these algorithms to the particular problem of the dynamics of (canonically normalized) interacting (singlet) scalar fields, can be found in Section 4 of of The-Art-I [@Figueroa_2020rrl]. The reader can also check the details of our evolution algorithms in [**Evolvers**][subsec_Evolvers]

The use of standard $\mathcal{O}(\delta \tilde{\eta}^2)$ accuracy algorithms, you need to choose in the input file `evolver=LF`, `evolver=VV2`, or `evolver=PV2`, respectively. Although both have an accuracy of order $\mathcal{O}(\delta \tilde{\eta}^2)$, `LF` only needs two steps by iteration, while `VV2` and `PV2` need three. Therefore, *verlet* algorithms are slower than *leapfrog*, typically by a factor $\sim$ 30$\%$-50$\%$ for `VV2` $\sim$ 20$\%$-40$\%$  for `PV2`, in our test runs (<span style="color:red;">**NICO confirms?**</span>). The VV and PV algorithms have the advantage, however, that they can be promoted recursively into higher order integrators with accuracies $\mathcal{O}(\delta \tilde{\eta}^4)$, $\mathcal{O}(\delta \tilde{\eta}^6)$, $\mathcal{O}(\delta \tilde{\eta}^8)$, and $\mathcal{O}(\delta \tilde{\eta}^{10})$. These improved algorithms, known as *Yoshida* integrators, are also implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and to use them you simply need to specify in the parameter file, `evolver=VV4`, `VV6`, `VV8`, `VV10`, or `evolver=PV4`, `PV6`, `PV8`, `PV10`, respectively. Such improved algorithms conserve energy much better than `LP`, `VV2` or `PV2`, but they are naturally slower, as they require more steps per iteration (the more the higher the accuracy of the integrator). For a discussion on the construction of all these integrators and dedicated versions of them to the dynamics of (canonically normalized) interacting (singlet) scalar field dynamics, we refer again the reader to Sections 3 and 4 of The-Art-I [@Figueroa_2020rrl], and to the Section [**Evolvers**][subsec_Evolvers].

