<!-- <div style="text-align: justify;"> -->

Since $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ v2.0, it is possible to simulate and study the dynamics and evolution of networks of cosmic defects.By default, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ includes models of global defects arising from theory with $N$ singlet scalar fields and a $\text{O}(N)$ symmetry, and of local cosmic strings originating in a theory with a complex scalar field and a $\text{U}(1)$ gauge sector. We note that, while these are the only two models currently implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, they can serve as a basis for advancd user to build their own models of cosmic defects. The simulation of such models follow the same basis as standard models of scalar-singlet and scalar-gauge interactions, respectively, they incorporate some additional features related to the initialization of the fields, the use of resolution-preserving techniques to prevent losing resolution of the cosmic strings, and the inclusion of string-specific observables.

In this section, we summarize the defect models that are implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, and describe the defect-especific methods in the code and how they are controlled from the parameter file. A more detailed explanation of the theory underlying these techniques can be found in chapter 5 of Ref. [@BaezaBallesteros_2025tme]. We separate this presentation in two sections: [*Simulations of global defects*][sec_simulationsGlobalDefects], focused on the case of global defects, and [*Simulations of local strings*][sec_simulationsLocalStrings], centered on the case of local $\text{U}(1)$ strings.

Before moving on, we note that simulations of cosmic defects are typically performed in conformal time, $\tau$, and we will be focusing on this case in this section. This means that the associated $\alpha$ parameter in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is set to one in the model file by default, `alpha = 1`. While this can be changed by the user, the correctness of the defect-specific techniques presented below has not be tested for other values of $\alpha$.

!!! warning "Caution"
    Version 2.0 of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ implements a significant casuistic of choices to simulate cosmic defects. While the most common cases have been thoroughly tested, the use of this software is at user's responsibility. We recommend properly testing the models before running large-scale simulations. If you find any bug or inconsistency, please report it via the "Issues" tab in our <a href="https://github.com/cosmolattice/cosmolattice_private" target="_blank"> GitHub repository </a>.


### **Simulations of global defects** { #sec_simulationsGlobalDefects }

Simulations of global cosmic defects can be performed using the $\texttt{defects_ONGlobal}$ model. This implements a scalar model consisting of $N$ real scalar fields with a global $\text{O}(N)$ symmetry, characterized by the action
[](){ #eq_globalDefectsAction }
```math
\begin{equation}\label{eq_globalDefectsAction}
S=-\int\text{d}^4x\sqrt{-g}\left\{\frac{1}{2}\sum_{a=1}^N\partial_\mu\phi_a\partial^\mu\phi^a+V_N[\{\phi_a\}]+\delta_{N,1}V_\text{bias}[\phi_1]\right\}\,,
\end{equation}
```
where the potential takes the general form
[](){ #eq_globalDefectsPotential }
```math
\begin{equation}\label{eq_globalDefectsPotential}
V_N[\{\phi_a\}]=\frac{\lambda}{4}\left(\sum_{a=1}^N\phi_a^2-v^2\right)^2\,.
\end{equation}
```
After spontaneous symmetry breaking different types of cosmic defects may originate depending on the number of scalar field species. In particular, the phase transition results in topological defects for $N=1$ (domain walls), $N=2$ (cosmic strings) and $N=3$ (monopoles), while for $N\geq 4$ the resulting structures, usually known as textures, are of non-topological nature. In the case of domain walls, $N=1$, we also allow for a bias term in the potential
[](){ #eq_globalBiasPotential }
```math
\begin{equation}\label{eq_globalBiasPotential}
V_\text{bias}[\{\phi_1\}]=qv\phi_1^3\,.
\end{equation}
```
which is used to break the degeneracy between the two true vacua of the model and leads to the eventual annihilation of the domain walls.  Independently of the number of scalar fields, the dynamics of the defects are controlled by the evolution of the scalar fields, which follows the canonical equations of motion,
[](){ #eq_globalEOM }
```math
\begin{equation}\label{eq_globalEOM}
(a^2\phi_a')'-a^2\nabla^2\phi_a=-a^4\lambda \phi_a\left(\sum_{b=1}^N \phi_b^2 - v^2\right)-3\delta_{N,1}a^4qv\phi_0^2\,.
\end{equation}
```

The number of scalar fields to be simulated has to be specified within the model file before compilation, using the `NScalars` variable. The following example, in particular, corresponds to the $N=1$ choice.
@emgithub(models/defects_ONGlobal.h:model_pars)
Note also the presence of the `DefectsModel` flag. This acts as a safeguard to prevent the use of defect-specific techniques for models that do not involve cosmic defects. It needs to be set to `true` before compilation in order for the techniques presented in this section to be available. Otherwise, irrespective of what is indicated in the parameter file, the model will run as a canonical model of scalar-singlet interactions.

!!! warning "Warning"
    While it is possible to turn the `DefectsModel` flag on for other models not involving cosmic defects, the use of defect-specific techniques on those scenarios has not been tested and is left at the user's responsibility.

The potential of this model is characterized by two main parameters: the vacuum expectation value, $v$, so that the true vacuum of the theory corresponds to $\langle |\phi|^2\rangle = v^2$, and $\lambda$, that characterizes the strength of the interactions.  Their values are indicated in the parameter file using the `vev` and `lambda` keywords, respectively,
@emgithub(models/parameter-files/defects_ONGlobal.in:model_parameters)
These two parameters, in addition, are used to define program variables for this model,
[](){ #eq_globalProgramVariables }
```math
\begin{equation}\label{eq_globalProgramVariables}
f_*=v\,,\quad\quad\quad\quad\omega_*=\sqrt{\lambda}v\,.
\end{equation}
```
Note that, as $v$ is in general the only dimensionful scale in the model, its actual value is irrelevant in the simulations, as it gets fully reabsorbed in the program variables.


In the case of biased domain-wall simulations, the potential includes one additional parameter, $q$, which controls the magnitude of the bias term. This is also indicated in the parameter file using the `qbias` keyword, in the same physical units as $v$. For example, for a simulation of biased domain walls, one could use
```text
#Model Parameters
lambda = 1
vev = 1
qbias = 0.001
```

The potential and its derivatives with respect to the field variables, expressed in program variables, are specified at the end on the model file. For example, the potential and its first derivative take the form
@emgithub(models/defects_ONGlobal.h:model_potential)
It is worth highlighting that, since the potential is invariant under permutations of the scalar fields, we don't need to specify a separate derivative function for each species, but instead we make used of the `Tag<N>` feature of [`TempLat`](https://github.com/cosmolattice/templat). Also notice how the second term in the potential, corresponding to the bias potential, is only present if `NScalars = 1`. Finally, we note the presence of a `resolutionPreservingFactor` variable. This is related to the resolution-preserving techniques discussed later in [*Resolution-preserving techniques*][sec_ResolutionPreservingTechniquesGlobal].

In addition to specifying the model parameters, it is also compulsory to indicate the standard parameters used for canonical simulations of scalar-singlet interactions. The only difference are some additional restrictions related to the background expansion. If the `DefectsModel` flag is on, `DefectsModel = true`, only simulations with fixed expansion or flat background are allowed. In the former case and specific for defect models, if no `H0` keyword is indicated or if one sets `H0 = 0.0`, the fixed background expansion will default to
[](){ #eq_genericScaleFactor }
```math
\begin{equation}\label{eq_genericScaleFactor}
a=\left(\frac{\tau}{\tau_0}\right)^p\quad\quad\quad \text{with}\quad\quad\quad p = \frac{2}{3(1+\omega)-2\alpha}\,,
\end{equation}
```
where recall, $\omega$ is indicated via the `omegaEoS` keyword and $\tau_0$ is the initial time of the simulation, indicated with $\tau_0$. For example, the default parameter file for the $\texttt{defects_ONGlobal}$ model contains
@emgithub(models/parameter-files/defects_ONGlobal.in:model_expansion)
for which the scale factor wil evolve as $a(\tau)=\tau / \tau_0$.

#### Initial conditions and diffusion { #sec_ICDiffusionGlobal }

In addition to the potential parameters and other parameters related to the basic running of the simulation, the only additional compulsory parameter that needs to be indicated for a simulation of cosmic defects is related to the initial conditions. Many different prescriptions to define the initial conditions for simulations of cosmic defects exist in the literature, most commonly based on generating an initial field configurations that contains a network of defects that is close to the scaling regime , or on emulating a phase transition that naturally gives rise to a network of cosmic defects.

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ makes it possible to simulate these two scenarios via two different sets of initial conditions. In the case of the models of global defects we are currently covering, these are specified using the `ICtype_S` keyword in the parameter file, which for simulations of cosmic defects accepts two values: `ICtype_S = DefectsNetwork` or `ICtype_S = DefectsWhiteNoise`. In both cases, all the species of scalar fields are initialized with a vanishing homogeneous part, with fluctuations that follow a power spectrum that we now detail. The associated conjugate momenta are all set to zero. If none of these two initial conditions is specified, the simulation of cosmic defects will fail.

The **first option**, `ICtype_S = DefectsNetwork`, initializes the field following a scale-dependent power spectrum, peaked around a scale $k\sim \ell_\text{corr}^{-1}$. This type of initial conditions is especially useful, when combined with a diffusive phase as described below, to generate a initial configuration consisting of a network of defects that is close to the scaling regime. The $\ell_\text{corr}$ parameter then controls the density of the resulting network. In particular, the power spectrum takes the form,
[](){ #eq_defectsNetworkIC }
```math
\begin{equation}\label{eq_defectsNetworkIC}
\Delta_{\phi_a}(\tilde{ \bf n})=\frac{k^3 v^2 \ell_\text{corr}^3}{\sqrt{2\pi}}\exp\left(-\frac{1}{2}k^2\ell_\text{corr}^2\right)\,.
\end{equation}
```
so that fluctuations are drawed from a Gaussian distribution with amplitude squared
[](){ #eq_defectsNetworkICAmplitude }
```math
\begin{equation}\label{eq_defectsNetworkICAmplitude}
\langle|\delta \tilde{\phi}(\tilde{n})|^2\rangle  \equiv \left({2\pi \ell_\text{corr} N\over \delta\tilde{x}}\right)^3 \text{exp}\left(-\đrac{\tilde{k}^2}{\ell_\txt{corr}^2}\right)  \,.
\end{equation}
```
In this case, the initial conditions depend on a single parameter, $\ell_\text{corr}$, which is especified in the parameter file (in program units) using the `lcorr` keywork. For example, the default parameter file for this model reads
@emgithub(models/parameter-files/defects_ONGlobal.in:model_IC)

The **second option**, `ICtype_S = DefectsWhiteNoise`, initializes all the scalar fields with Gaussian fluctuations that follow a white noise power spectrum,
[](){ #eq_whiteNoise }
```math
\begin{equation}\label{eq_whiteNoise}
\Delta_{\phi_a}(\tilde{n})=A\frac{ k^3 }{4\pi^2 a^2}\,.
\end{equation}
```
More concretely, the fluctuations are initialized as explained in Eq. (24) of [**Scalar-scalar interactions**](My first model of (singlet) scalar fields.md). This means that field amplitudes are drawn from a Gaussian distribution with expected squared amplitude
[](){ #eq_whiteNoiseamplitude }
```math
\begin{equation}\label{eq_whiteNoiseAmplitude}
\left.\langle|\delta \tilde{\phi}(\tilde{\bf n})|^2\rangle\right|_{\tilde{k}<\tilde{k}_\text{cut}}  \equiv \left({\omega_*\over f_*}\right)^2\left({N\over \delta \tilde{x}}\right)^3{A \over 2a^2}  \,.
\end{equation}
```
These initial conditions are controlled via two free parameters: the maximum momentum to be excited in the initial conditions, $k_\text{cut}$, and the amplitude of the fluctuations, $A. These are specified in program units using the `kCutOff` and `deltaNoise` keywords in the parameter file, respectively.  For example, a valid choice of initial conditions are
```text
#IC
ICtype_S = WhiteNoise
kCutOff = 6
deltaNoise = 0.01
```
The cutoff momentum, $\tilde{k}_\text{cut}$, is an optional parameter, which is set to $\tilde{k}_\text{UV}$ if not especified.

**Diffusion of the initial conditions**

Following the initialization of the field fluctuations, one can decide between two options: to let the field evolve naturally following the physical equations of motion, or to use an **initial phase of diffusion** to reach a better defined network of defects. In the case of `ICtype = DefectsWhiteNoise` initial conditions, as one wants to mimic the phase transition, it is not recommended to perform such a phase of diffusion. However, this is customary when using the `ICtype = DefectsNetwork` option, as the diffusion phase allows one to get rid of the excess energy of the initial configuration and to generate an initial network of defects that is close to the scaling regime.

The diffusion phase is characterized by an evolution of the field following the first-order diffusion equation
[](){ #eq_diffusion }
```math
\begin{equation}\label{eq_diffusionGlobal}
\phi_a'=\nabla^2\phi_a-\phi_a\left(\sum_{b=1}^N\phi_b^2-1\right)\,.
\end{equation}
```
In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, this phase is controlled via a series of parameters in the input file, as presented in the following table. We recall that it is necessary to set `DefectsModel = true` in the model file before compilation for any of these parameters to take effect.

| **Parameters** <div style="width:130px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `doDiffusion` | Boolean flag that indicates if a diffusion phase must be performed to the initial field configuration. Defaults to `false`. |
| `tmaxdiff` | Duration of the diffusion phase in program units. We note the diffusion time is different and kept separate from the physical simulation time. |
| `diffType` | Evolver used to integrate the diffusion equation. Must be of a Runge-Kutta type: `RK2`, `RK3_3`, `RK3_4` or `RK4_5`. Default: `RK2`. |
| `dtdiff` | Time step used for the integration of the diffusion equation. Must obey $\delta\tau_\text{diff}\lesssim \delta x^2 / 3$ (Courant's stability condition). |

Furthermore, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ allows the user to measure the evolution of different observables during the diffusion phase. These correspond to the energies of the system, that get saved to the $\texttt{average_energies_diffusion.txt}$ file, and the norm of the scalar field, $|\phi|$, as defined below in Eq. \eqref{eq_resolutionPreservingFactor}. This is saved to the $\texttt{average_norm_diffusion.txt}$ file, toghether with the average of its square, $\langle|\phi|^2\rangle$, and its variance, $\text{var}(|\phi|)$. Moreover, it is also possible to save snapshots of the fields during the diffusion phase. The frequency of these measurements and the type of snapshots to be saved is controlled via the following keywords. Recall that saving snapshots requires compiling $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ with the $\texttt{HDF5}$ library enabled.

| **Parameters** <div style="width:140px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `tOutputFreqDiff` | Frequency with which averages of energies and the norm of the scalar fields are saved during the diffusion phase. |
| `tOutputRareFreqDiff` | Frequency with which snapshots are saved during the diffusion phase. |
| `snapshots_diffusion` | Snapshots to be saved during diffusion. The same options as for the standard `snapshots` keyword are valid. Requires compiling the model with the HDF5 library. |

For example, the selection of diffusion parameters to be used in a simulation takes the following form in the default parameter file
@emgithub(models/parameter-files/defects_ONGlobal.in:diffusion)
It is worth noting that, while the integration of the diffusion equation makes use of a Runge-Kutta solver, no additional fields are stored in memory. Instead, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ makes use of the momentum variables as auxiliary fields, since these are not used during the diffusion phase and are set to zero at the end.

#### Resolution-preserving techniques { #sec_ResolutionPreservingTechniquesGlobal }

Following the initialization of the fields, the simulation will by default run following the physical equations of motion given in Eq. \eqref{eq_globalEOM}. When working in an expanding background, however, this leads to a loss in resolution of the defects. As the width of the defects is constant in physical units, $w\sim 1/\sqrt{2\lambda} v$, but simulations are performed in comoving spacial coordinates, less and less lattice points are contained within the defects as the simulation progresses.  This can lead to large discretization errors in the simulation.

To overcome this problematic, several resolution-preserving techniques have been proposed in the literature. Possibly the most extended one is the use of a modified modified equation of motion to evolve the fields during some period of the simulation time, which is followed by standard evolution following \eqref{eq_globalEOM}. The modified equation of motion takes the form
[](){ #eq_resolutionPreservingGlobal }
```math
\begin{equation}\label{eq_resolutionPreservingglobal}
(a^2\phi_a')'-a^2\nabla^2\phi_a=-a^4\left[\frac{a}{a(\tau_\text{RP,0})}\right]^{2(s-1)}\lambda \phi_a\left(\sum_{b=1}^N \phi_b^2 - v^2\right)-3\delta_{N,0}a^4qv\phi_0^2\,.
\end{equation}
```
where we keep the dependence of the scale factor on the time variable implicit, $a\equiv a(\tau)$. Here $\tau_\text{RP,0}$ indicates the starting time of the resolution-preserving phase and $s$ is a parameter that controls the evolution of the width of the defects. In particular, during this unphysical phase the comoving width scales as
[](){ #eq_resolutionPreservingwidthglobal }
```math
\begin{equation}\label{eq_resolutionPreservingwidthglobal}
w_\text{c}(\tau)= w_0\left[\frac{a(\tau_\text{RP,0})}{a(\tau)}\right]^s\,.
\end{equation}
```
with $w_0$ the initial width. For $s < 0$, the resolution of the defects grows, while for $s=0$ it remains constant. The cases $s=0$ and $s=-1$ are known as fattening and extra-fattening, respectively.

Typically, the duration of these resolution-presenving phase is fixed to ensure that the resolution of the defects is the same at the beginning of this phase and at the end of the simulation. This corresponds to setting the end-time of this resolution-preserving phase as
[](){ #eq_resolutionPreservingdurationglobal }
```math
\begin{equation}\label{eq_resolutionPreservingdurationglobal}
\displaystyle \tau_\text{RP,max} = \left(\frac{\tau_\text{max}}{\tau_\text{RP,0}^{s}}\right)^{1/(1-s)}\,,
\end{equation}
```
For example, fattening is set to last during the whole simulation, $\tau_\text{fat,max}=\tau_\text{max}$, while extra-fattening ends after a period $\tau_\text{e-fat,max}=\sqrt{\tau_0 \tau_\text{max}}$.

It is worth noting that this resolution-preserving phase, while unphysical, is similar to the physical evolution. Thus, it is expected that the defects will slowly approach some scaling regime during this phase which, while unphysical, is expected to be close to the physical phase. However, this also means that the time spent in this resolution-preserving phase needs to be taken into account when setting the maximum time that the simulation can run for before around-the-world causal effects start to affect the results.

In $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, the resolution-preserving technique outline above is implemented via a `resolutionPreservingFactor` variable, $\Lambda_\text{RP}$, member of the `AbstractModel` class, that multiplies the potential,
```math
\begin{equation}\label{eq_resolutionPotentialResolutionPreserving}
 V_N[\{\phi_a\}]=\frac{\lambda}{4}\Lambda_\text{RP}(\tau)\left(\sum_{a=1}^N\phi_a^2-v^2\right)^2\,,
 \end{equation}
```
where $\Lambda_\text{RP}(\tau)$ takes the values,
[](){ #eq_resolutionPreservingFactor }
```math
\begin{equation}\label{eq_resolutionPreservingFactor}
\Lambda_\text{RP}(\tau) = \left\{\begin{array}{lr}
1  & \quad\quad\quad\quad\quad \tau < \tau_\text{RP,0}\,,\\[7pt]
\displaystyle\left[\frac{a(\tau)}{a(\tau_\text{RP,0})}\right]^{2(s-1)} &  \quad\quad\quad\quad\quad \tau_\text{RP,0} \leq \tau < \tau_{RP, max}\,, \\[7pt]
\displaystyle\left[\frac{a(\tau_{RP,max})}{a(\tau_\text{RP,0})}\right]^{2(s-1)} &  \quad\quad\quad\quad\quad \tau \geq \tau_{RP, max}\,. \\
\end{array}\right.
\end{equation}
```
We note that this factor is kept fixed after the resolution preserving-phase has ended. The value of this parameter is saved as the last column in the $\texttt{average_scale_factor.txt}$ output file if a defect model (`DefectsModel = true`) is simulated, irrespective of whether it contains a resolution-preserving phase or not (in which case $\Lambda_\text{RP}=1$).

To include a resolution-preserving phase in a simulation of cosmic defects and specify its details, one uses the following keywords in the input parameter file,

| **Parameters** <div style="width:160px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `doResolutionPreserving` | Boolean flag that indicates if a resolution-preserving phase is to be performed. Defaults to `false`. |
| `sRP` | Value of the $s$ parameter used in the resolution-preserving phase. It has to obey $s \leq 0$. |
| `tRP0` | Time at which the resolution-preserving phase starts. Default: `t0`. |
| `tRPmax` | Time at which the resolution-preserving phase ends. Defaults to the result given in Eq. \eqref{eq_resolutionPreservingFactor}. |

For example, a possible value of these inputs can be found in the default parameter file for the considered model,
@emgithub(models/parameter-files/defects_ONGlobal.in:resolutionpreserving)


We finally highlight a technical feature of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. Typically, the time at which the resolution-preserving phase is expected to end, $\tau_\text{RP,max}$, does not corresponds to an exact time step of the evolution, this is, $\tau_0 + n\delta\tau < \tau_\text{RP,max} < \tau_0 + (n+1)\delta\tau$ for some integer $n$. To prevent errors coming from this misalignment, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ performs an additional intermediate time step exactly at $\tau_\text{RP,max}$. Note this does not affect the times at which observables are measured or any other functionality of the code.


#### Defect-specific observables { #sec_ObservablesGlobal }

In addition to techniques to generate the initial conditions and prevent the loss of resolution, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ v2.0 allows the user to measure different observables related to the cosmic defects. For the particular case of scalar defects originating in a theory that only contains scalar fields, as in the $\texttt{defects_ONGlobal}$ model, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ can provide up to four different types of additional measurements:

-  The norm of the complex field,
[](){ #eq_globalNorm }
```math
\begin{equation}\label{eq_globalNorm}
|\phi|=\left(\sum_{a=1}^N \phi_a^2\right)^{1/2}\,,
\end{equation}
```
which is saved to the $\texttt{average_norm.txt}$ file, together with the average of its square, $\langle|\phi|^2\rangle$, and its variance, $\text{var}(|\phi|)$. The frequency of these measurements is controlled by the `tOutputFreq` parameter.

-  The power spectrum of the norm, which is saved to the $\texttt{spectra_norm.txt}$ file, with a frequency controlled by the `tOutputInfreq` keyword.

-   If `measureDefectsEnergies = true` is indicated in the parameter file, the energy components of the defects, weighted by a window function that is centered around the defects, are measured. In particular, for the scalar model considered here, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ allows one to measure
[](){ #eq_globalNorm }
```math
\begin{equation}\label{eq_weightedEnergiesglobal}
\begin{array}{rcl}
E_{K,\text{str}} & = & \displaystyle\frac{\delta x^3}{2a^3}\sum_{\mathbf{n}}\sum_{a=1}^N W[\{\phi_b(\mathbf{n})\}]\pi_{\phi_a}(\mathbf{n})^2\,,\\[7pt]
 E_{G,\text{str}} & = &\displaystyle \frac{a \delta x^3}{2}\sum_{\mathbf{n}}\sum_i \sum_{a=1}^N W[\{\phi_b(\mathbf{n})\}] \left[\nabla^+_i\phi_a(\mathbf{n})\right]^2\,,\\[7pt]
  E_{V,\text{str}} & = &\displaystyle a^3 \delta x^3 \sum_{\mathbf{n}} \sum_{\mathbf{n}}\sum_{a=1}^N W[\{\phi_b(\mathbf{n})\}] V[\{\phi_a(\mathbf{n})\}]\,,
\end{array}
\end{equation}
```
These are saved to columns 2-4 of the $\texttt{average_defects.txt}$ file, following the measurement time. The window function implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is proportional to the potential and normalized such that it is equal to unitiy at the core of the defects and field values beyond the true vacua are not taken into consideration,
[](){ #eq_globalWeightFunction }
```math
\begin{equation}\label{eq_globalWeightFunction}
W[\varphi]=\frac{4V_N[\varphi]}{\lambda v^4}\,\Theta\left(v^2-|\phi|^2\right)\,,
\end{equation}
```
with $\Theta$ the Heaviside function. This window function can easily be changed in the $\texttt{definitions/defectsmodule/weights.h}$ header file.

-  For domain walls (`NScalars = 1`) and if `measureDefectsStructure = true` is indicated in the parameter file, an estimate of the area of the domain walls is measured. In particular, the area parameter is defined as
[](){ #areaParameter }
```math
\begin{equation}\label{eq_areaParameter}
\displaystyle A=a^2\delta x^2 \sum_{\mathbf{n}}\sum_{i} \delta_{\mathbf{n},i}\frac{|\nabla \phi_1(\mathbf{n})|}{\displaystyle\sum_{j}|\nabla_j\phi_1(\mathbf{n})|}\,,
\end{equation}
```
where $\delta_{\mathbf{n},i}=1$ if $\phi(\mathbf{n})\phi(\mathbf{n}+\hat{\imath})<0$, and 0 otherwise. This quantity is saved in the last column of the $\texttt{average_defects.txt}$ file.

- For global strings (`NScalars = 2`) and if `measureDefectsStructure = true`, an estimate of the total length of the strings is saved in the last column of the,
[](){ #eq_lengthGlobal }
```math
\begin{equation}\label{eq_lengthGlobal}
L_\text{w}=\frac{2}{3}\delta x N_\text{w}\,.
\end{equation}
```
where the $2/3$ prefactor takes into account the so-called Manhattan effect and $N_\text{w}$ is the total number of plaquettes with non-zero winding number. For a plaquette with vertices $\{\mathbf{n}, \mathbf{n}+\hat{\imath}, \mathbf{n}+\hat{\imath}+\hat{\jmath}, \mathbf{n}+\hat{\jmath}\}$, the winding number is defined as
[](){ #eq_windingNumberGlobal }
```math
\begin{equation}\label{eq_windingNumberGlobal}
W_{ij}(\mathbf{n})=\frac{1}{2\pi}\left(Y_i(\mathbf{n})+Y_j(\mathbf{n}+\hat{\imath})-Y_i(\mathbf{n}+\hat{\jmath})-Y_j(\mathbf{n})\right)\,,
\end{equation}
```
where
[](){ #eq_phaseVariation }
```math
\begin{equation}\label{eq_phaseVariation}
Y_i(\mathbf{n})=\left[\theta(\mathbf{n})-\theta(\mathbf{n}+\hat{\imath})\right]_\pi\,,
\end{equation}
```
is the phase variation along each link, which is defined as $\theta(\mathbf{n})=\text{atan}(\phi_2/\phi_1)$, and $[\varepsilon]_\pi$ sets $\theta$ in the range $-\pi < \varepsilon \leq \pi$.

An example of the observables section can be found in the default parameter file,
@emgithub(models/parameter-files/defects_ONGlobal.in:observables)
Note that, if a simulation is performed with $N>2$, no observable will be measured even if `measureDefectsStructure = true` is specified.

### **Simulations of local strings** { #sec_simulationsLocalStrings }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is also prepared to simulate local $\text{U}(1)$ strings, using the $\texttt{defects_localStrings}$ model. This implements a model with a complex scalar field charged under a U(1) gauge symmetry, with action
[](){ #eq_localStringsAction }
```math
\begin{equation}\label{eq_localStringsAction}
S=-\int\text{d}^4x\sqrt{-g}\left\{(D^A_\mu\varphi)^*D_A^\mu\varphi+\frac{1}{4}F_{\mu\nu}F^{\mu\nu}+V_2[\phi_1, \phi_2]\right\}\,,
\end{equation}
```
where here $\phi_1$ and $\phi_2$ refer to the real and imaginary parts of the complex field, defined via $\varphi=\frac{1}{\sqrt{2}}(\phi_1+i\phi_2)$, and the scalar potential is identical to that in Eq. \eqref{eq_globalDefectsPotential} for $N=2$, see [**Scalar-gauge interactions**](My first model of gauge fields.md) for a definition of the covariant derivative and the field strength tensor.

This model has a local $\text{U}(1)$ symmetry which is spontaneously broken by the true vacuum, leading to the formation of local cosmic strings. Their evolution is governed by the dynamics of the underlying fields, which follow the classical equations of motion,
[](){ #eq_localStringsEOM }
```math
\begin{equation}\label{eq_localStringsEOM}
\left.\begin{array}{rcl}
(a^2\varphi')'-a^2 D_A^2\varphi & = & \displaystyle -2a^{4}\lambda\varphi\left(|\varphi|^2-\frac{v^2}{2}\right)\,,\\[10pt]
A_i''-\nabla^2A_i + \nabla_j \nabla_i A_j & = & \displaystyle 2a^{2}g_AQ_A^\varphi\text{Im}[\varphi^*D_i\varphi]\,.
\end{array}\right.
\end{equation}
```

In addition to $\lambda$ and the vaccuum expectation value, $v$, the model also depends on the gauge coupling, $g_A$, and the charge of the complex scalar field under the $\mathrm{U}(1)$ symmetry, $Q_A^\varphi$. All these need to be specified in the parameter file,
@emgithub(models/parameter-files/defects_U1LocalStrings.in:model_pars)
These parameters are used to define program variables analogously as it is done in the case of global defects, see Eq. \eqref{eq_globalProgramVariables}. The particular example presented here corresponds to the so-called critical case, $2\lambda=(Q_a^\varphi g_A)^2$, for which the gauge ans scalar radii of the local strings coincide, although $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ also allows one to simulate other non-critical cases. Note that as this model only contains one complex scalar field, there is a redundancy between the values of $g_a$ and $Q_A^\varphi$. However, both need to be included in the parameter file to run $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

Simulations of local strings in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ follow similar lines than those of global defects presentedabove in [*Simultations of global defects*][sec_simulationsGlobalDefects]. In particular, they can only be simulated on a flat background or on a background with fixed expansion. $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ also incorporates local versions of the initial conditions, resolution-preserving techniques and observables discussed above, which are controlled by the same input parameters as in the global case. We now briefly discuss the main differences, but refer the reader to the corresponding subsections fromthe global strings model for more detailed information about the usage of these techniques and the associated parameters.

#### Initial conditions and diffusion { #sec_ICDiffusionLocal }

Initial conditions for local strings in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ are specified in a very similar fashion as for their global counterparts now using the `ICtype_U1` keywork in the parameter file. In particular, one can set this to either `ICtype_U1 = DefectsNetwork` or `ICtype_U1 = DefectsWhiteNoise`, which initializes the real and imaginary parts of the complex field with a zero homogeneous background and fluctuations that follow the power spectra given in Eq. \eqref{eq_defectsNetworkIC} or Eq. \eqref{eq_whiteNoise}, respectively. The same parameters as described for global defects in [*Initial conditions and diffusion*][sec_ICDiffusionGlobal] need to be specified in either case. In both cases, the gauge field and the conjugate momenta of both the scalar and gauge fields are set to zero.

**Diffusion of initial conditions**

Following the initialization of the scalar field, a phase of diffusion can also be performed, which is especially recommended when using `ICtypeU1 = DefectsNetwork`. In this phase, the fields are evolved following the diffusion equations
[](){ #eq_localStringsDiffusion }
```math
\begin{equation}\label{eq_localStringsDiffusion}
\left.\begin{array}{rcl}
\varphi' & = & a^2 D_A^2\varphi \displaystyle - 2a^{4}\lambda\varphi\left(|\varphi|^2-\frac{v^2}{2}\right)\,,\\[10pt]
A_i' & = & \nabla^2A_i - \nabla_j \nabla_i A_j + \displaystyle 2a^{2}g_AQ_A^\varphi\text{Im}[\varphi^*D_i\varphi]\,.
\end{array}\right.
\end{equation}
```
As in the global case, the evoluton is performed with a Runge-Kutta algorithm, and conjugate momenta, which are set to zero after the end of diffusion, are used as the auxiliary variables to store the intermediate kernels. All the input parameters regarding the diffusive phase and the measurement of observables are the same as for global defects.

#### Resolution-preserving techniques { #sec_ResolutionPreservingTechniquesLocal }

$\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ allows one to use of resolution-preserving techniques for local strings, similar to the techniques used for global defects. In particular, the equations of motion of the fields are modified to
[](){ #eq_localFatteningEOM }
```math
\begin{equation}\label{eq_localFatteningEOM}
\left.\begin{array}{rcl}
(a^2 \tilde{\varphi}')'  & = &a^2 \tilde{D}_{A,i}^-\tilde{D}_{A,i}^+ \tilde{\varphi} \displaystyle -2 a^4 \left(\frac{a}{a_0}\right) ^ {2(s-1)}\tilde{\varphi} \left(|\tilde{\varphi}|^2 -\frac{1}{2}\right)\,,\\[10pt]
\displaystyle \left[\left(\frac{a}{a_0}\right)^{-2(s-1)}\tilde{A}_i'\right]' & = &\displaystyle \left(\frac{a}{a_0}\right)^{-2(s-1)}\left[\tilde{\nabla}_j^-\tilde{\nabla}_j^+\tilde{A}_i - \tilde{\nabla}_j^-\tilde{\nabla}_i^+ \tilde{A}_j \right] +  2 a^2 g_AQ_A^\varphi \left(\frac{f_*}{\omega_*}\right)^2 \text{Im}[\tilde{\varphi}^* \tilde{D}_{A,i}^+ \tilde{\varphi}]\,,
\end{array}\right.
\end{equation}
```
This is implemented in the code using the `resolutionPreservingFactor` member variable of the `AbstractModel` class, that takes time dependent values according to Eq. \eqref{eq_resolutionPreservingFactor}. An important difference to the scalar case is the fact that this factor enters the definition of the conjugate momentum of the gauge field,
[](){ #eq_localConjugateMomemtaFattening }
```math
\begin{equation}\label{eq_localConjugateMomemtaFattening}
\left.\begin{array}{rcl}
(\tilde{\pi}_A)_i=\Lambda_\text{RP}^{-1}\tilde{A}_i'\,,
\end{array}\right.
\end{equation}
```
as well as definitions that involve the electric and magnetic energy of the U(1) field,
[](){ #eq_localEnergiesFattening }
```math
\begin{equation}\label{eq_localEnergiesFattening}
\begin{array}{rcl}
K_{U(1)} & = & \displaystyle\frac{1}{2a^4\Lambda_\text{RP}} \sum_i E_i^2(\mathbf{n})\,,\\[10pt]
G_{U(1)} & = & \displaystyle\frac{1}{2a^4\Lambda_\text{RP}} \sum_i B_i^2(\mathbf{n})\,.
\end{array}
 \end{equation}
```
This factor similarly enters the definition of the anisotropic stress tensor acting as the source of gravitational waves---see the $\texttt{PITensor.h}$.


#### Defect-specific observables { #sec_ObservablesLocal }

Finally, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ incorporates some especific observables to study the dynamics and evolution of local strings. Contrary to the global case, the norm of the complex field and its corresponding power spectrum are measured by default in the $\texttt{average_norm_cmplx_scalar_0.txt}$ and $\texttt{spectra_norm_cmplx_scalar_0.txt}$ files, respectively, and so no additional measurements are performed by default. Note however, that the normalization of the field is different that in the global case, and the vacuum corresponds to $\langle |\tilde\varphi|^2\rangle = 1/2$.

In the case of local strings, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ allows one to measure two defect-specific observables:

-  If `measureDefectsEnergies = true` is indicated, the energy components of the cosmic strings, measured using a window function centered in the strings, are saved to the $\texttt{defects_observables.txt}$ file, in columns 2-6. These are defined as
[](){ #eq_localStringsEnergies }
```math
\begin{equation}\label{eq_localStringsEnergies}
\begin{array}{rcl}
E_{K,\text{str}}&=&\displaystyle\frac{\delta x^3}{a^3}\sum_{\mathbf{n}} W[\varphi(\mathbf{n})] |\pi_\varphi(\mathbf{n})|^2\,,\\[5pt]
 E_{G,\text{str}} & =&\displaystyle a\delta x^3\sum_{\mathbf{n}}\sum_i W[\varphi(\mathbf{n})]|D_{A,i}^+\varphi(\mathbf{n})|^2\,,\\[5pt]
E_{V,\text{str}}&=&\displaystyle a^3\delta x^3 \sum_{\mathbf{n}} W[\varphi(\mathbf{n})] V[\varphi(\mathbf{n})]\,,\\[5pt]
 E_{E,\text{str}} & =&\displaystyle\frac{a\delta x^3}{2}\sum_{{\mathbf{n}}}W[\varphi(\mathbf{n})] \sum_i E_i^2(\mathbf{n})\,,\\[5pt]
 E_{B,\text{str}} & = &\displaystyle \frac{\delta x^3}{2a}\sum_{\mathbf{n}} W[\varphi(\mathbf{n})] \sum_i B_i^2(\mathbf{n})\,,
\end{array}
\end{equation}
```
where the window function is analogous to the one used for global defects,
[](){ #eq_localWeightFunction }
```math
\begin{equation}\label{eq_localWeightFunction}
W[\varphi]=\frac{4V[\varphi]}{\lambda v^4}\,\Theta\left(\frac{v^2}{2}-|\varphi|^2\right)\,.
\end{equation}
```

-  If `measureDefectsStructure = true` is indicated in the parameter file, an estimate of the total length of the strings is saved as the last column of the $\texttt{average_defects.txt}$ file. This is measured from the number of pierced plaquettes as indicated in Eq. \eqref{eq_lengthGlobal}. Recall we define a pierced plaquette as that one with a non-zero winding number. In the case of local strings, this is defined as in Eq. \eqref{eq_windingNumberGlobal}, where now the contribution from a link between $\mathbf{n}$ and $\mathbf{n}+\hat{\imath}$ is
[](){ #eq_phaseVariationLocal }
```math
\begin{equation}\label{eq_phaseVariationLocal}
Y_i(\mathbf{n})=\left[g_AQ_A^\varphi \delta x A_i(\mathbf{n})+\theta(\mathbf{n})-\theta(\mathbf{n}+\hat{\imath})\right]_\pi- g_AQ_A^\varphi\delta x A_i(\mathbf{n})\,,
\end{equation}
```
with $\theta$ the phase of the complex field.


