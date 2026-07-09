<!-- <div style="text-align: justify;"> -->

CosmoLattice makes it possible to simulate and study the dynamics and evolution of networks of cosmic defects. At the time of writing, CosmoLattice incorporates models of global defects arising from theory with $N$ singlet scalar fields and a O($N$) symmetry, and of local cosmic string arising from a theory with a complex scalar field and a U(1) gauge sector. While the simulation of such models follow the same basis as standard models of scalar-singlet and scalar-gauge interactions, they incorporate some additional features related to the initialization of the fields, the use of resolution-preserving techniques to prevent losing resolution of the cosmic strings, and the inclusion of string-related observables.

In this page we summarize the default models that are implemented in CosmoLattice, and explains the parameters that can be used to control different aspects of the simulations of cosmic strings. A more detailed explanation of the techniques used in the simulations can be found in chapter 5 of (Ref. [@BaezaBallesteros_2025tme]) We separate this presentation in two section: [*Simultations of global defects*][sec_simulationsGlobalDefects], focused on the case of global defects, and [*Simultations of local strings*][sec_simulationsLocalStrings], centered in the case of local U(1) strings. We note that, while these are the only two models currently implemented in CosmoLattice, they can serve as a basis for advancd user to build their own models of cosmic defects.

!!! warning "Caution"
    Verion 2.0 of CosmoLattice implements a significant casuistic of choices to simulate cosmic defects. While the most common cases have been properly tested, we use of this software is at responsibility of the user. We recommend you to test the models you want to simulate before running large-scale simulations. If you find any bug or inconsistency, please report it via the ``Issues'' tab in GitHub.

### Simulations of global defects { #sec_simulationsGlobalDefects }

Simulations of global cosmic defects can be performed using the $\texttt{defects_ONGlobal.h}$ model file. This implements a scalar model consisting of $N$ scalar fields with a global O($N$) symmetry. More concretely, the model is characterized by the action
[](){ #eq_globalDefectsAction }
```math
\begin{equation}\label{eq_globalDefectsAction}
S=-\int\text{d}^4x\sqrt{-g}\left\{\frac{1}{2}\sum_{a=1}^N\partial_\mu\phi_a\partial^\mu\phi^a+V_N[\{\phi_a\}+\delta_{N,1}V_\text{bias}[\phi_1]]\right\}\,,
\end{equation}
```
where the potential takes the general form
[](){ #eq_globalDefectsPotential }
```math
\begin{equation}\label{eq_globalDefectsPotential}
V_N[\{\phi_a\}]=\frac{\lambda}{4}\left(\sum_{a=1}^N\phi_a^2-v^2\right)^2\,.
\end{equation}
```
After spontaneous symmetry breaking different types of cosmic defects may originate depending on the number of scalar field species that controls the symmetry breaking pattern. In particular, the phase transition results in topological defects for $N=1$ (domain walls), $N=2$ (cosmic strings) and $N=3$ (monopoles), while for $N\gtreq 4$ the resulting structures, usually knonw as textures, are of non-topological nature. In the case of domain walls, we also allow for a bias term in the potential
[](){ #eq_globalBiasPotential }
```math
\begin{equation}\label{eq_globalBiasPotential}
V_\text{bias}[\{\phi_1\}]=qv\phi_1^3\,.
\end{equation}
```
which is used to break the degeneracy between the two true vacua of the model and leads to an annihilation of the domain walls.

The number of scalar fields to be simulated is specified at the beginning of the model file before compilation, using the `NScalars` variable
@emgithub(models/defects_ONGlobal.h:model_pars)
Note also the presence of the `DefectsModel` flag. This acts as a safeguard to prevent the use of defect-specific techniques for models that do not involve cosmic defects. It needs to be set to `true` in order for the techniques presented in the remaining of the page to be available. Otherwise, irrespective of what is indicated in the parameter file, the model will run as a canonical model of scalar-singlet interactions.

!!! note "Important note"
    While it is possible to turn on the `DefectsModel` flag for other models not involving cosmic defects, the use of defect-specific techniques on those escenarios has not been studied and is highly disencouraged.

The potential of this model is characterized by two main parameters: $v$, which corresponds to the vacuum expectation value of the system (so that the true vacuum of the theory corresponds to $\langle |\phi|^2\rangle = v^2$) and $\lambda$, that characterizes the strength of the interactions.  Their values are indicated in the parameter file using the `vev` and `lambda` keywords, respectively,
@emgithub(models/parameter-files/defects_ONGlobal.in:model_parameters)
These two parameters, in addition, are used to define program variables for these models,
[](){ #eq_globalBiasPotential }
```math
\begin{equation}\label{eq_globalBiasPotential}
f_*=v\,,\quad\quad\quad\quad\omega_*=\sqrt{\lambda}v\,.
\end{equation}
```
In the case of biased domain-wall simulations, the potential includes one additional parameter, $q$, which controls the magnitude of the bias term. This also needs to be indicated in the parameter file using the `qbias` keyword, for example,
```text
#Model Parameters
lambda = 1
vev = 1
qbias = 0.001
```

The potential and its derivatives with respect to the field variables, expressed in program variables, are specified at the end on the model file. In particular, the potential and its first derivative take the form
@emgithub(models/defects_ONGlobal.h:model_potential)
It is worth noting that, since the potential is symmetric in all the scalar fields, we don't need to especify a separate derivative function for each species, but instead we make used of the `Tag<N>` feature of $\texttt{TempLat}$. We also note how the second term, corresponding to the bias potential, is only present if `NScalars = 1`. Finally, we note the present of a `fatteningfactor` variable. This is related to the resolution-preserving techniques discussed below, and will be explained in detail later.

We finally note that simulations of cosmic defects in CosmoLattice are performed by default in conformal time. The associated $alpha$ parameter is set to one in the model file. While this can be changed by the user, the correctness of the defect-specific techniques presented below has not be tested for other values of $\alpha$.

In addition to especifying the model parameters, it is compulsory to especify the same required parameters as for canonical simulations are needed to run a simulation of cosmic defects, and there are no additional compulsory parameters. The only difference compared to canonical scalar-singlet interactions at this point is related to the background expansion. If the `DefectsModel` flag is specified, only simulations with fixed expansion or with flat background are allowed. In addition, if no `H0` keyword is especified or if one sets `H0 = 0.0`, the fixed background expansion will default to
[](){ #eq_genericScaleFactor }
```math
\begin{equation}\label{eq_genericScaleFactor}
a=(\eta/\eta_0)^p\quad\quad\quad \text{with}quad\quad\quad p = \frac{2}{3(1+\omega)-2\alpha}\,,
\end{equation}
```
where recall, $\omega$ is indicated via the `omegaEoS` keyword and $\eta_0$ is the initial time of the simulation. For example, the default parameter file for the \texttt{defects_ONGlobal.h} model contains
@emgithub(models/parameter-files/defects_ONGlobal.in:model_expansion)
for which the scale factor wil evolve as
[](){ #eq_radiationScaleFactor }
```math
\begin{equation}\label{eq_radiationScaleFactor}
a(\eta)=\eta / \eta_0\,.
\end{equation}
```

If no additional flags are indicated in the parameter file, the model will run analogously to a canonical model of scalar-singlet interactions, following the canonical equations of motion
[](){ #eq_globalEOM }
```math
\begin{equation}\label{eq_globalEOM}
(a^2\phi_a')'-a^2\nabla^2\phi_a=-a^4\lambda \phi_a\left(\sum_{b=1}^N \phi_b^2 - v^2\right)-3\delta_{N,0}a^4qv\phi_0^2\,.
\end{equation}
```
The only difference are the initial conditions---see the next subsection for further discussion.

#### Initial conditions and diffusion { #sec_ICDiffusionGlobal }

Many different prescriptions to define the initial conditions for simulations of cosmic defects exist in the literature. Most commonly, they are based on emulating a phase transition that naturally gives rise to a network of cosmic defects which eventually approaches the scaling regime, or on generating an initial field configurations that contains a network of defects that is already close to scaling. CosmoLattice makes it possible to simulate these two scenarios via two different sets of initial conditions. In the case of the models of global defects discussed here, these are specified using the `ICtype_S` keyword in the parameter file, which for simulations of cosmic defects must be one of two options: `DefectsWhiteNoise` or `DefectsNetwork`. In both cases, all the species of scalar fields are initialized with a vanishing homogeneous part, with fluctuations that follow a power spectrum that we now detail. The associated conjugate momenta are all set to zero. Note that, if none of this initial conditions is especified, the simulation will fail.

The **first option**, `ICtype_S = DefectsWhiteNoise` initializes all the scalar fields with Gaussian fluctuations that follow a white noise poser spectrum,
[](){ #eq_whiteNoise }
```math
\begin{equation}\label{eq_whiteNoise}
\Delta_{\phi_a}(\tilde{n})=\frac{\delta k^3}{4\pi^2 a^2}\,.
\end{equation}
```
More concretely, the fluctuations are initialized as explained in Eq. (24) [**Scalar-scalar interactions**](My first model of (singlet) scalar fields.md). This emans that field amplitudes are drawn from a Gaussian distribution with expected square amplitude
[](){ #eq_whiteNoiseamplitude }
```math
\begin{equation}\label{eq_whiteNoiseAmplitude}
\left.\langle|\delta \tilde{\phi}(\tilde{n})|^2\rangle\right|_{\tilde{k}<\trilde{k}_\text{cut}}  \equiv \left({\omega_*\over f_*}\right)^2\left({N\over \delta \tilde{x}}\right)^3{\delta\over 2a^2}  \,.
\end{equation}
```
These initial conditions are controlled via two free parameters: the maximum momentum that gets excited in the initial conditions, $k_\cut$, which acts analogously to the case of quantum fluctuations used in preheating, and the amplitude of the fluctuations, $\delta$. These are specified in program units using the `kCutOff` and `deltaNoise` keywords, respectively. Only the former is an optional parameter, which is set to $\tilde{k}_\text{UV}$ if not especified. For example, a valid choice of initial conditions are
```text
#IC
ICtype_S = WhiteNoise
lcorr = 0.01
```

The **second option**, `ICtype_S = DefectsNetwork`, initializes the field following a scale-dependent power spectrum, peaked around a scale $\ell_\text{corr}$. This type of initial conditions are especially useful (when combined with a diffusive phase as described below) to generate a initial configuration consisting of a network of defects. The $\ell_\text{corr}$ parameter then controls the density of the resulting network. In particular, the power spectrum takes the form,
[](){ #eq_defectsNetworkIC }
```math
\begin{equation}\label{eq_defectsNetworkIC}
\Delta_{\phi_a}(\tilde{n})=\frac{k^3 v^2 \ell_\text{corr}^3}{\sqrt{2\pi}}\exp\left(-\frac{1}{2}k^2\ell_\text{corr}^2\right)\,.
\end{equation}
```
so that fluctuations are drawed from a Gaussian distribution with amplitude
[](){ #eq_defectsNetworkICAmplitude }
```math
\begin{equation}\label{eq_defectsNetworkICAmplitude}
\langle|\delta \tilde{\phi}(\tilde{n})|^2\rangle  \equiv \left({2\pi \ell_\text{corr} L\over \delta\tilde{x}^2}\right)^3 \text{exp}\left(-\đrac{\tilde{k}^2}{\ell_\txt{corr}^2}\right)  \,.
\end{equation}
```
In this case, the initial conditions depend on a single parameter, $\ell_\text{corr}$, which is especified in the parameter file (in program units) using the `lcorr` keywork. For example, the default parameter file for this model reads
@emgithub(models/parameter-files/defects_ONGlobal.in:model_IC)

Following the initialization of the field fluctuations, one can decide between two options: allow the field to evolve following the physical equations of motion, or to use an **initial phase of diffusion** to reach a better defined network of defects. In the case of `WhiteNoise` initial conditions, as one wants to mimic the phase transition, it is not recommended to perform a phase of diffusion. However, this is customary when using the `DefectsNetwork` initial conditions, as they allow to get rid of the excess energy of the initial configuration and give rise to an initial network of defects that is close to the scaling regime.
This phase is characterized by an evolution of the field following the first-order diffusion equation
[](){ #eq_diffusion }
```math
\begin{equation}\label{eq_defectsNetworkICAmplitude}
\phi_a'=\nabla^2\phi_a-\phi_a\left(\sum_{b=1}^N\phi_b^2-1\right)\,.
\end{equation}
```

In CosmoLattice, the diffusion phase is controlled via a series of parameters in the input file.
| **Parameters** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `doDiffusion` | Boolean flag indicated if a diffusion phase is to be performed. Defaults to `false`. |
| `tmaxdiff` | Duration of the diffusion phase in program units. We note the diffusion time is different and kept separate from the physical simulation time. |
| `diffType` | Evolver used to evolve the diffusion equation. Must be of a Runge-Kutta type: `RK2`, `RK3_3`, `RK3_4` or `RK4_5`. Default: `RK2`. |
| `dtdiff` | Time step used for the integration. |
In addition to specifying the duration of the diffusion phase and the details of the integrator, CosmoLattice also allows the user to measure the evolution of different observables duringthe diffusion phase. These correspond to the energies of the system, that get saved to the $\texttt{average_energies_diffusion.txt}$ file, and the norm of the scalar field, $|\phi|=\left(\sum_{a=1}^N \phi_a^2\right)^{1/2}$, which is saved to $\texttt{average_norm_diffusion.txt}$. Moreover, it is also possible to save snapshots of the fields during the diffusion phase.
| **Parameters** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `tOutputFreqDiff` | Frequency in which energies and the norm of the scalar field are saved during diffusion. |
| `tOutputRareFreqDiff` | Frequency in which snapshots are save during the diffusion phase. |
| `snapshots_diffusion` | Snapshots to be saved during diffusion. The same options as for the standard `snapshots` keyword are valid. |
For example, the selection of diffusion parameters to be used in a simulation could be
@emgithub(models/parameter-files/defects_ONGlobal.in:diffusion)

It is worth noting that, while the integration of th ediffusion equation makes use of a Runge-Kutta solver, no additional fields are stored in memory. Instead, CosmoLattice makes use of the momentum variables as auxiliary fields, since these are set to zero at the end.

#### Resolution-preserving techniques { #sec_ResolutionPreservingTechniquesGlobal }

Following the initialization of the field configuration, the simulation starts running following the physical equations of motion


#### Defect-specific observables { #sec_ObservablesGlobal }



### Simulations of global defects { #sec_simulationsLocalDefects }

#### Initial conditions and diffusion { #sec_ICDiffusionLocal }

#### Resolution-preserving techniques { #sec_ResolutionPreservingTechniquesLocal }

#### Defect-specific observables { #sec_ObservablesLocal }
