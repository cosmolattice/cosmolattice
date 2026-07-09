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
[](){ #eq_globalProgramVariables }
```math
\begin{equation}\label{eq_globalProgramVariables}
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
It is worth noting that, since the potential is symmetric in all the scalar fields, we don't need to especify a separate derivative function for each species, but instead we make used of the `Tag<N>` feature of $\texttt{TempLat}$. We also note how the second term, corresponding to the bias potential, is only present if `NScalars = 1`. Finally, we note the present of a `resolutionPreservingFactor` variable. This is related to the resolution-preserving techniques discussed below, and will be explained in detail later.

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

Following the initialization of the field fluctuations, one can decide between two options: allow the field to evolve following the physical equations of motion, or to use an **initial phase of diffusion** to reach a better defined network of defects. In the case of `DefectsWhiteNoise` initial conditions, as one wants to mimic the phase transition, it is not recommended to perform a phase of diffusion. However, this is customary when using the `DefectsNetwork` initial conditions, as they allow to get rid of the excess energy of the initial configuration and give rise to an initial network of defects that is close to the scaling regime.
This phase is characterized by an evolution of the field following the first-order diffusion equation
[](){ #eq_diffusion }
```math
\begin{equation}\label{eq_diffusionGlobal}
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

In addition to specifying the duration of the diffusion phase and the details of the integrator, CosmoLattice also allows the user to measure the evolution of different observables duringthe diffusion phase. These correspond to the energies of the system, that get saved to the $\texttt{average_energies_diffusion.txt}$ file, and the norm of the scalar field, $|\phi|$, as defined below in Eq. \eqref{eq_resolutionPreservingFactor}. This is saved to the $\texttt{average_norm_diffusion.txt}$, toghether with the average of its square and its variance (as is done for any other standard observable in cosmoLattice) . Moreover, it is also possible to save snapshots of the fields during the diffusion phase.

| **Parameters** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `tOutputFreqDiff` | Frequency in which energies and the norm of the scalar field are saved during diffusion. |
| `tOutputRareFreqDiff` | Frequency in which snapshots are save during the diffusion phase. |
| `snapshots_diffusion` | Snapshots to be saved during diffusion. The same options as for the standard `snapshots` keyword are valid. |

For example, the selection of diffusion parameters to be used in a simulation could be
@emgithub(models/parameter-files/defects_ONGlobal.in:diffusion)

It is worth noting that, while the integration of th ediffusion equation makes use of a Runge-Kutta solver, no additional fields are stored in memory. Instead, CosmoLattice makes use of the momentum variables as auxiliary fields, since these are set to zero at the end.

#### Resolution-preserving techniques { #sec_ResolutionPreservingTechniquesGlobal }

Following the initialization of the field configuration, the simulation starts running following the physical equations of motion given in Eq. \eqref{eq_globalEOM}. However, when working in an expanding background, this leads to a loss in resolution of the defects. As the width of the defects if constant in physical units, $w\sim 1/\sqrt{2\lambda} v$, but simulations are performed in comoving units, less and less lattice points will be contained within the defect as the simulation progresses.  This can lead to large discretization errors in the simulation.

To overcome this problem, several resolution-preserving techniques have been proposed in the literature [@Press:1989yh;@Moore:2001px;@Bevis:2006mj], based on evolving the fields using a modified equation of motion of the form
[](){ #eq_resolutionPreservingGlobal }
```math
\begin{equation}\label{eq_resolutionPreservingglobal}
(a^2\phi_a')'-a^2\nabla^2\phi_a=-a^4\left[\frac{a}{a(\tau_\text{RP,0})}\right]^{2(s-1)}\lambda \phi_a\left(\sum_{b=1}^N \phi_b^2 - v^2\right)-3\delta_{N,0}a^4qv\phi_0^2\,.
\end{equation}
```
only during a fixed period of time, followed by standard evolution. Here $t_\text{RP,0}$ indicates the starting time of the resolution-preserving phase and $s$ is a parameter that controls the evolution of the resolution of the defects.In particular, during this unphysical phase the width of the defects scales as
[](){ #eq_resolutionPreservingwidthglobal }
```math
\begin{equation}\label{eq_resolutionPreservingwidthglobal}
r_\text{c}(\tau)= r_0\left(\frac{a_0}{a}\right)^s\,.
\end{equation}
```
so that for $s < 0$, the resoluton of the defects scales, while for $s=0$ it is kept constant. In particular, the cases $s=0$ and $s=-1$ are known as fattening and extra-fattening, respectively.  Typically, the duration of these resolution-presenving phase is fixed to ensure the resolution of the defects is the same at the beginning of this phase and at the end of the simulation,
[](){ #eq_resolutionPreservingdurationglobal }
```math
\begin{equation}\label{eq_resolutionPreservingdurationglobal}
\tau_\text{RP,max} = \tau_\text{RP,0}^{-\frac{s}{1-s}}\tau_\text{max}^{\frac{1}{1-s}}
\end{equation}
```
For example, fattening is set to last during the whole simulation, $\tau_\text{fat,max}=\tau_\text{max}$, while extra-fattening ends after a period $\tau_{e-fat,max}=\sqrt{\tau_0 \tau_\texŧ{max}}$. Note that, while this is the typical choice, other options may also be viable and can be explored using CosmoLattice. Finally, it is worth noting that this resoluton-preserving phase, while unphysical, is similar to the physical evolution, and so it is expected that the defects will slowly approach the scaling regime during this phase. However, this also means that this time needs to be taken into account when deciding the maximum time that the simulation can run for before around-the-world causal effects start to affect the results.

In CosmoLattice, the resoluton preserving phase is implemented via a `resolutionPreservingFactor` variable that multiplies the potential. This takes the values
[](){ #eq_resolutionPreservingFactor }
```math
\begin{equation}\label{eq_resolutionPreservingFactor}
\Lambda_\text{RP}(\tau) = \left\{\begin{array}{lr}
1  & \quad\quad\quad\quad\quad \tau < \tau_\text{RP,0}\,,\\
\left[\frac{a(\tau)}{a(\tau_\text{RP,0})}\right]^{2(s-1)} &  \quad\quad\quad\quad\quad \tau_\text{RP,0} \leq \tau < \tau_{RP, max}\,, \\
\left[\frac{a(\tau_{RP,max})}{a(\tau_\text{RP,0})}\right]^{2(s-1)} &  \quad\quad\quad\quad\quad \tau \geq \tau_{RP, max}\,. \\
\end{array}\right\}
\end{equation}
```
where we note it is kept fixed after the resolution preserving-phase has ended. We note that the value of this parameter is saved as the last column in the $\texttt{average_scale_factor.txt}$ file if a defects model (`DefectsModel = true`) is simulate, irrespective of whether it containes a resolution-preserving phase or not.

To include a resolution-preserving phase in a simulation and specify its details, one needs to indicate the following parametersin the parameter file,

| **Parameters** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `doResolutionPreserving` | Boolean flag indicated if a resoluton-preserving phase is to be performed. Defaults to `false`. |
| `sRP` | Value of the $s$ parameter used in the resolution-preserving techniques. It has to obey $s \leq 0$. |
| `tRP0` | Time at which the resolution-preserving phase starts. Default: `t0`. |
| `tRPmax` | Time at which the resolution-preserving phase ends. Defaults to the results given in Eq. \eqref{eq_resolutionPreservingFactor}. |

For example, a possible value of these inputs can be found in the default parameter file,
@emgithub(models/parameter-files/defects_ONGlobal.in:resolutionpreserving)

Finally, we note a special detail of cosmolattice. Typically, the time at which the resolution-preserving phase is expected to end, $\eta_\text{RP,max}$, does not corresponds to an exact time step of the evolution, this is, $\eta_0 + n\delta\eta < \eta_\text{RP,max} < \eta_0 + (n+1)\delta\eta$ for some integer $n$. to prevent errors coming from this misalignment, CosmoLAttice perfoms an additional time step exactly at $\eta_\text{RP,max}$. Note this does not affect the times at which observables are measured or any other functionality of the code.


#### Defect-specific observables { #sec_ObservablesGlobal }

In addition of additional techniques to generate the initial conditions and prevent the loss of resolution, CosmoLattice also allows to measure different observables related ot the cosmic defects. For the particular case of scalar defects originating in a theory that only contains scalar fields, such as that given in $\texttt{defects_ONGlobal.h}$, CosmoLattice automatically measures the norm of the complex fields,
[](){ #eq_globalNorm }
```math
\begin{equation}\label{eq_globalNorm}
$|\phi|=\left(\sum_{a=1}^N \phi_a^2\right)^{1/2}$
\end{equation}
```
This is saved to the $\texttt{average_norm.txt}$ file, together with the average of its square, $\langle|\phi|^2\rangle$ and its variance. These measurements fall in the frequent category, this is, its frequency is controlled by the `tOutputFreq` keyword. In addition, the power psectrum of the norm is also saved to the $\texttt{spectra_norm.txt}$ file, with a frequency control by the `tOutputInfreq` keyword.

In addition to the norm of the scalar fields, CosmoLattice allows one to measure other observables related to the defects, which are saved to the $\texttt{average_defects.txt}$ file. These fall in two categories that need to be activated by the user via the input file. First, it is possible to measure the energy components of the defects weighted by a weight function that is centered around the defects. In particular, for the scalar model, CosmoLattice allows one to measure
[](){ #eq_globalNorm }
```math
\begin{equation}\label{eq_weightedEnergiesglobal}
\begin{array}{rcl}
E_{K,\str} & = & \displaystyle\frac{\delta x^3}{a^3}\sum_{\mathbf{n}} W[\varphi(\mathbf{n})]|\pi_\varphi(\mathbf{n})|^2\,,\\[10pt]
 E_{G,\str} & = &\displaystyle a \delta x^3\sum_{\mathbf{n}}\sum_i W[\varphi(\mathbf{n})] |\nabla^+_i\varphi(\mathbf{n})|^2\,,\\[10pt]
  E_{V,\str} & = &\displaystyle a^3 \delta x^3 \sum_{\mathbf{n}} W[\varphi(\mathbf{n})] V[\varphi(\mathbf{n})]\,,
\end{array}
\end{equation}
```
where the default weight function used is proportional to the potential and normalized such that it is equal to unitiy at the core of the defects and field values beyond the true vacua are not taken into consideration,
[](){ #eq_globalWeightFunction }
```math
\begin{equation}\label{eq_globalWeightFunction}
W[\varphi]=\frac{4V[\varphi]}{\lambda v^4}\,\Theta\left(\frac{v^2}{2}-|\varphi|^2\right)\,,
\end{equation}
```
One can easily change this in the $\texttt{definitions/defectsmodule/weights.h}$ header file. To measure the weighted energies, one needs to indicate `measureDefectsEnergies = true` in the parameter file.

Second, in the case of domain walls (`NScalars = 1`) or cosmic strings (`NScalars = 2`) one can measure the estimates of the total area of the domain walls or the total length of the strings. In the case of domain walls, the area parameter is defined as
[](){ #areaParameter }
```math
\begin{equation}\label{eq_areaParameter}
\displaystyle A=a^2\delta x^2 \sum_{\mathbf{n}}\sum_{i} \delta_{\mathbf{n},i}\frac{|\nabla \phi(\mathbf{n})|}{\displaystyle\sum_{j}|\nabla_j\phi(\mathbf{n})|}\,,
\end{equation}
```
where $\delta_{\mathbf{n},i}=1$ if $\phi(\mathbf{n})\phi(\mathbf{n}+\hat{\imath})<0$, and 0 otherwise. On the other hand, for global cosmic strings the total length is defined from counting the number of plaquettes with non-zero winding number, $N_\text{w}$,
[](){ #eq_lengthGlobal }
```math
\begin{equation}\label{eq_lengthGlobal}
L_\text{w}=\frac{2}{3}\delta x N_\text{w}\,.
\end{equation}
```
where the $2/3$ prefactor takes into account the so-called Manhattan effect, and the winding number of a plaquette with vertices $\{\mathbf{n}, \mathbf{n}+\hat{\imath}, \mathbf{n}+\hat{\imath}+\hat{\jmath}, \mathbf{n}+\hat{\jmath}\}$ is defined as
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
is the phase variation along each link, which is defined as $\theta(\mathbf{n})=\text{atan}(\phi_2/\phi_1)$, and $[\theta]_\pi$ sets $\theta$ in the range $-\pi <\theta \leq\pi$. To measure either the area parameter ($N=1$) or the string length ($N=2$), one needs to indicate `measureDefectsStructure = true` in the parameter file. These observables are always written as the last column of the output file. Note that, if a simulation is performed with $N>2$, no observable will be measured even if `measureDefectsStructure` is set to `true`.

An example of the observables section in the parameter file can be found in the default parameter file,
@emgithub(models/parameter-files/defects_ONGlobal.in:observables)



### Simulations of local strings { #sec_simulationsLocalStrings }

In addition to models of global defects, CosmoLattice is also prepared to simulate local U(1) strings, using the $\texttt{defects_LocalStrings.h}$ model. This model implements a model with a complex scalar field charged under a U(1) gauge symmetry, with action
[](){ #eq_localStringsAction }
```math
\begin{equation}\label{eq_localStringsAction}
S=-\int\text{d}^4x\sqrt{-g}\left\{(D^A_\mu\varphi)^*D_A^\mu\varphi+\frac{1}{2}F_{\mu\nu}F^{\mu\nu}+V_2[\phi_1, \phi_2]\right\}\,,
\end{equation}
```
where here $\phi_1$ and $\phi_2$ refer to the real and imaginary parts of the complex field, which are defined via, $\varphi=\frac{1}{\sqrt{2}}(\phi_1+i\phi_2)$., and the scalar potential is identical to that in Eq. \eqref{eq_globalDefectsPotential} for $N=2$. This model has a local U(1) symmetry which is spontaneously broken, leading to the formation of local cosmic strings. Their evolution is governed by the dynamics of the underlying fields, which follow the classical equations of motion,
[](){ #eq_localStringsEOM }
```math
\begin{equation}\label{eq_localStringsEOM}
\left.\begin{array}{rcl}
(a^2\varphi')'-a^2 D_A^2\varphi & = & \displaystyle -2a^{4}\lambda\varphi\left(|\varphi|^2-\frac{v^2}{2}\right)\,,\\[10pt]
A_i''-\nabla^2A_i + \nabla_j \nabla_i A_j & = & \displaystyle 2a^{2}e\text{Im}[\varphi^*D_i\varphi]\,.
\end{array}\right.
\end{equation}
```

The model considered here depends on four parameters. In addition to $\lambda$ and the vaccuum expectation value, $v$, the model also depends on the gauge coupling, $g_A$, and the charge of the complex scalar field under the U(1) symmetry, $Q_A^\varphi$. All these need to be specified in the parameter file,
@emgithub(models/parameter-files/defects_localStrings.in:model_pars)
These parameters are used to define program variables analogously as it is done in the case of global defects, see Eq. \eqref{eq_globalProgramVariables}. The particular example presented here corresponds to the so-called critical case, $2\lambda=(Q_a^\varphi g_A)^2$, for which the gauge ans scalar radii of the local strings coincide, although CosmoLattice also allows one to simulate other non-critical cases.

!!! note "Parameter degeneracy"
    As this model contains a single complex scalar field, there is a redundancy between the values of $g_a$ and $Q_A^\varphi$. However, both need to be included in the parameter file.

As in the case of global defects, simulations of local strings in CosmoLattice follow similar lines than those of global defects presented above in [*Simultations of global defects*][sec_simulationsGlobalDefects]. In particular, they can only be simulated on a flat background or on a background with fixed expansion. Beyond that, CosmoLAttice incorporates local versions of the initial conditions, resolution-preserving techniques and observables discussed above, which are controlled by the same input parameters as in the global case. We now briefly discuss the main differences, but refer the reader to [*Initial conditions and diffusion*][sec_ICDiffusionGlobal], [*Resolution-preserving techniques*][sec_ResolutionPreservingTechniquesGlobal] and [*Defect-specific observables*][sec_ObservablesGlobal] for more detailed information about the usage of these techniques.

#### Initial conditions and diffusion { #sec_ICDiffusionLocal }

Initial conditions for local strings in CosmoLattice are specified in a very similar fashion as for their global counterparts now using the `ICtype_U1` keywork in the parameter file. In particular, one can set this to either `DefectsWhiteNoise` or `DefectsNetwork`, which initializes the real and imaginary parts of the complex field with a zero homogeneous background and fluctuations that follow the power spectra given in Eqs. \eqref{eq_whiteNoise} and \eqref{eq_defectsNetworkIC}, respectively. The same parameters as described in [*Initial conditions and diffusion*][sec_ICDiffusionGlobal] need to be specified in either case. In both cases, the gauge field and the conjugate momenta of both the scalar and gauge fields are set to zero.

Following the initialization of the scalar field, a phase of diffusion can also be performed, which is especially recommended when considering `ICtypeU1 0 DefectsNetwork`. In this phase, the fields are evolved following the diffusion equations
[](){ #eq_localStringsDiffusion }
```math
\begin{equation}\label{eq_localStringsDiffusion}
\left.\begin{array}{rcl}
(a^2\varphi')'-a^2 D_A^2\varphi & = & \displaystyle -2a^{4}\lambda\varphi\left(|\varphi|^2-\frac{v^2}{2}\right)\,,\\[10pt]
A_i''-\nabla^2A_i + \nabla_j \nabla_i A_j & = & \displaystyle 2a^{2}e\text{Im}[\varphi^*D_i\varphi]\,.
\end{array}\right.
\end{equation}
```
As in the global case, the evoluton is performed with a Runge-Kuta algorithm, and conjugate momenta, which are set to zero after the end of diffusion, are used as the auxiliary variables to store the intermediate kernels. All the input parameters regarding the diffusive phase and the measurement of observables are the same as in the global case.

#### Resolution-preserving techniques { #sec_ResolutionPreservingTechniquesLocal }

CosmoLattice also allows for the use of resolution-preserving techniques for local strings, analogous to those used for global defects. In aprticular, the equations of motion of the fields are modified to be
[](){ #eq_localFatteningEOM }
```math
\begin{equation}\label{eq_localFatteningEOM}
\left.\begin{array}{rcl}
(a^2 \tilde{\varphi}')'  & = &a^2 \tilde{D}_{A,i}^-\tilde{D}_{A,i}^+ \tilde{\varphi} + \displaystyle -2 a^4 \left(\frac{a}{a_0}\right) ^ {2(s-1)}\tilde{\varphi} \left(|\tilde{\varphi}|^2 -\frac{1}{2}\right)\,,\\[10pt]
\displaystyle \left[\left(\frac{a}{a_0}\right)^{-2(s-1)}\tilde{A}_i'\right]' & = &\displaystyle \left(\frac{a}{a_0}\right)^{-2(s-1)}\left[\tilde{\nabla}_j^-\tilde{\nabla}_j^+\tilde{A}_i - \tilde{\nabla}_j^-\tilde{\nabla}_i^+ \tilde{A}_j \right] +  2 a^2 e \left(\frac{\fstar}{\omegastar}\right)^2 \text{Im}[\tilde{\varphi}^* \tilde{D}_{A,i}^+ \tilde{\varphi}]\,,
\end{array}\right.
\end{equation}
```
Again, this is implemented in the code using the `resolutionPreservingFactor` variable, that takes time dependent values according to Eq. \eqref{eq_resolutionPreservingFactor}. An important difference to the scalar case is that this factor enters the definition of the conjugate momentum of the gauge field,
[](){ #eq_localConjugateMomemtaFattening }
```math
\begin{equation}\label{eq_localFatteningEOM}
\left.\begin{array}{rcl}
(\tilde{\pi}_A)_i=\Lambda_\text{RP}^{-1}\tilde{A}_i'\,,
\end{array}\right.
\end{equation}
```
as well as definitions that involve the electric and magnetic energy of the U(1) field,
[](){ #eq_localConjugateMomemtaFattening }
```math
\begin{equation}\label{eq_localFatteningEOM}
K_{U(1)} & = & \displaystyle\frac{1}{2a^4\Lambda_\text{RP}} \sum_i E_i^2(\mathbf{n})\,,\\[10pt]
K_{U(1)} & = & \displaystyle\frac{1}{2a^4\Lambda_\text{RP}} \sum_i B_i^2(\mathbf{n})\,.
 \end{equation}
```
This factor similarly enters the definition of the anisotropic stress tensor acting as the source of gravitational waves---see .



#### Defect-specific observables { #sec_ObservablesLocal }

Finally, CosmoLattice also incorporates some especific observables to study the dynamics and evolution of local strings. Contrary to the global case, the norm of the complex field and its corresponding power spectrum are measured by default in the $\texttt{average_norm_cmplx_scalar_0.txt}$ and $\texttt{spectra_norm_cmplx_scalar_0.txt}$ files, and so no additional measurement is performed by default. Note however, that the normalization of the field is different that in the global case, and the vacuum corresponds to $\langle |\tilde\varphi|^2\rangle = 1/2.

However, CosmoLattice makes it possible to measure the energy components of the comsic strings and the total length of the strings, which get saved in the $\texttt{average_defects.txt}$ output file. If one sets `measureDefectsEnergies = true` in the input file,this file will contain the average weighted energies,
[](){ #eq_localStringsEnergies }
```math
\begin{equation}\label{eq_localStringsEnergies}
\begin{array}{rcl}
E_{K,\str}&=&\displaystyle\frac{\delta x^3}{a^3}\sum_{\mathbf{n}} W[\varphi(\mathbf{n})] |\pi_\varphi(\mathbf{n})|^2\,,\\[10pt]
 E_{G,\str} & =&\displaystyle a\delta x^3\sum_{\mathbf{n}}\sum_i W[\varphi(\mathbf{n})]|D_{A,i}^+\varphi(\mathbf{n})|^2\,,\\[10pt]
E_{V,\str}&=&\displaystyle a^3\delta x^3 \sum_{\mathbf{n}} W[\varphi(\mathbf{n})] V[\varphi(\mathbf{n})]\,,\\[10pt]
 E_{E,\str} & =&\displaystyle\frac{a}{2}\sum_{{\mathbf{n}}}W[\varphi(\mathbf{n})] \sum_i E_i^2(\mathbf{n})\,,\\[10pt]
 E_{B,\str} & = &\displaystyle \frac{1}{2a}\sum_{\mathbf{n}} W[\varphi(\mathbf{n})] \sum_i B_i^2(\mathbf{n})\,,
\end{array}
\end{equation}
```
where the weight function is analogous to the one used for global defects
[](){ #eq_localWeightFunction }
```math
\begin{equation}\label{eq_localWeightFunction}
W[\varphi]=\frac{4V[\varphi]}{\lambda v^4}\,\Theta\left(\frac{v^2}{2}-|\varphi|^2\right)\,,
\end{equation}
```

Second, if `measureDefectsStructure = true` is indicated in the parameter file, the last column of the $\texttt{average_defects.txt}$ file would correspond to the total length of the strings, measured from the number of pierced plaquettes as indicated in Eq. \eqref{eq_lengthGlobal}. Recall we define a pierced plaquette as that one with a non-zero winding number. In the case of local strings, this is defined as in Eq. (), where now the contribution from each link is
[](){ #eq_phaseVariation }
```math
\begin{equation}\label{eq_phaseVariation-local}
Y_i(\mathbf{n})=\left[g_AQ_A^\varphi \delta x A_i(\mathbf{n})+\theta(\mathbf{n})-\theta(\mathbf{n}+\hat{\imath})\right]_\pi- g_AQ_A^\varphi\delta x A_i(\mathbf{n})\,
\end{equation}
```
with $\theta$ the phase of the complex field.


