This section summarizes the output results that are obtained from a simulation using $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, including information about the content of the output files and about the parameters that can be used to control the type and format of the output. More concretely, we first cover in [**Average observables**][sec_averages] those observables related to averages of fields or energies, we then comment on the measurements of power spectra in [**Power spectra**][sec_PS], and finally briefly cover snapshots in [**Field and energy snapshots**][sec_snapshots].

We note that most of the information presented here can also be found in other section of this manual. For example [**Scalar-scalar interactions**](My first model of (singlet) scalar fields.md) and [**Scalar-gauge interactions**](My first model of gauge fields.md) contain information about the output from default simulations of theories containing scalar and gauge fields, while information about $\texttt{HDF5}$ output can also be found in [**HDF5: Output and Backups**](HDF5: Output and Backups.md). Other information about module specific observables can be found in the corresponding sections of this manual.

Before moving to the description of the output, we first note some general keywords that make it possible to control the output from the parameter file:

-   `outputfile`: Folder to which all observable output files are saved. Defaults to the folder where the simulation is executed.

-   `print_headers`: If set to `true`, prints headers in the first line of the $\texttt{.txt}$ files, describing the content of each column. Headers are written for both average and power spectra output files. Defaults to `false`. Does not have effect for measurements saved in HDF5 format.

-   `overwriteFiles`: If set to `true` overwrites already existing files from a previous simulation. Otherwise, the simulation will fail if files already exist. Defaults to `false` to prevent the deletion of previous results. Note that the directory does not get completely wiped, only those files that are used by the simulation.

-   `appendToFiles`: Instead of writing new files, appends the results to previously existing ones when set to `true`. If the files don't exist, they are newly created. Defaults to `false`. Cannot be `true` simultaneously with `overwriteFiles`. In that case, the simulation is aborted.

### **Average observables** { #sec_averages }

One of the main results from running a simulation with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ is a series of files that contain the time evolution of averages of field values and energy densities. These files are generated automatically when a simulation is run depending on the matter content of the model, and are measured with a frequency specified by the `tOutputFreq` keyword (indicated in program units, defaults to $10\delta \tilde{\eta}$ if unspecified). Each line of the file corresponds to a different measurement. First we cover those files related to **field averages** and results for the **evolution of the scale factor**. (<span style="color:orange;">**Jorge comment: I need to check that the norm is normalized as stated**</span>)

For a model containing scalar singlet fields, `NScalars > 0`, the simulation will contain the following files:

<div class="grid cards cl-files" markdown>

-   `average_scalar_[nfld].txt`{ .cl-fname }

    For each scalar field species $\texttt{nfld}$, containing the following columns:

    $\tilde{ \eta}$, $\langle \tilde{\phi}_{\texttt{nfld}} \rangle$, $\langle \tilde{\phi}_{\texttt{nfld}}' \rangle$, $\langle \tilde{\phi}_{\texttt{nfld}}^2 \rangle$, $\langle \tilde{\phi}_{\texttt{nfld}}^{'2} \rangle$, $\text{rms} (\tilde{\phi}_{\texttt{nfld}})$, $\text{rms} (\tilde{\phi}_{\texttt{nfld}}')$
    {: .cl-schema }

</div>

If the model contains complex scalar, `NCScalars > 0`, the following measurements are generated:

<div class="grid cards cl-files" markdown>

-   `average_norm_cmplx_scalar_[nfld].txt`{ .cl-fname }

    For each complex scalar species $\texttt{nfld}$. It contains:

    $\tilde{ \eta}$, $\langle |\tilde{\varphi}_{\texttt{nfld}} |\rangle$, $\langle | \tilde{\varphi}_{\texttt{nfld}}' |\rangle$, $\langle |\tilde{\varphi}_{\texttt{nfld}} |^2 \rangle$, $\langle |\tilde{\varphi}_{\texttt{nfld}}'|^{2} \rangle$, $\text{rms} (|\tilde{\varphi}_{\texttt{nfld}}|)$, $\text{rms} (|\tilde{\varphi}_{\texttt{nfld}}'|)$
    {: .cl-schema }

-   `average_[Re/Im]_cmplx_scalar_[nfld].txt`{ .cl-fname }

    Two additional files for each complex field species, containing information about its real ($n=1$) and imaginary  ($n=2$) parts, respectively, $\varphi_{\texttt{nfld}}=\varphi_{\texttt{nfld},1}+i\varphi_{\texttt{nfld},2}$. We note this separation in real and imaginary parts differs by a factor of $\sqrt{2}$ from the canonical normalization, $\varphi_{\texttt{nfld}}=(\phi_{\texttt{nfld},1}+i\phi_{\texttt{nfld},2})/\sqrt{2}$.

    $\tilde{ \eta}$, $\langle \tilde{\varphi}_{\texttt{nfld}} \rangle$, $\langle \tilde{\varphi}'_{\texttt{nfld}} \rangle$, $\langle \tilde{\varphi}_{\texttt{nfld}}^2 \rangle$, $\langle \tilde{\varphi}^{'2}_{\texttt{nfld}} \rangle$, $\text{rms} (\tilde{\varphi}_{\texttt{nfld}})$, $\text{rms} (\tilde{\varphi}'_{\texttt{nfld}})$
    {: .cl-schema }

</div>

For simulations with scalar doublets, `NSU2Doublet > 0`, one also gets:

<div class="grid cards cl-files" markdown>

-   `average_norm_SU2Doublet_[nfld].txt`{ .cl-fname }

    Containing measurements related to the norm of the scalar doublet, $|\Phi_{\texttt{nfld}}|^2=|\Phi_{\texttt{nfld},1}|^2+|\Phi_{\texttt{nfld},2}|^2+|\Phi_{\texttt{nfld},3}|^2+|\Phi_{\texttt{nfld},4}|^2$. One file is created per field species, $\texttt{nfld}$. It contains:

    $\tilde{ \eta}$, $\langle |\tilde{\Phi}_{\texttt{nfld}} |\rangle$, $\langle | \tilde{\Phi}_{\texttt{nfld}}' |\rangle$, $\langle |\tilde{\Phi}_{\texttt{nfld}} |^2 \rangle$, $\langle |\tilde{\Phi}_{\texttt{nfld}}'|^{2} \rangle$, $\text{rms} (|\tilde{\Phi}_{\texttt{nfld}}|)$, $\text{rms} (|\tilde{\Phi}_{\texttt{nfld}}'|)$
    {: .cl-schema }

-   `average_SU2Doublet_[nfld]_[n].txt`{ .cl-fname }

    Measurements related to each component of the scalar doublets. Four files are created per field species. They contain:

    $\tilde{ \eta}$, $\langle \tilde{\Phi}_{\texttt{nfld}} \rangle$, $\langle \tilde{\Phi}'_{\texttt{nfld}} \rangle$, $\langle \tilde{\Phi}_{\texttt{nfld}}^2 \rangle$, $\langle \tilde{\Phi}^{'2}_{\texttt{nfld}} \rangle$, $\text{rms} (\tilde{\Phi}_{\texttt{nfld}})$, $\text{rms} (\tilde{\Phi}'_{\texttt{nfld}})$
    {: .cl-schema }

</div>

For simulations with $\mathrm{U}(1)$ gauge fields `NU1Flds > 0`, the following measurements are also generated. Note that simulations with `NU1Flds > 1` have not been thoroughly tested.

<div class="grid cards cl-files" markdown>

-   `average_norm_U1_[nfld].txt`{ .cl-fname }

    Contains information about the $\textrm{U}(1)$ fields. Again, a different field is generated for each field species, $\texttt{nfld}$, with columns:

    $\tilde{ \eta}$, $\langle {|\vec{\widetilde{ \mathcal E}}^{\mathrm{U}(1)}_{\texttt{nfld}}|} \rangle$, $\langle {|\vec{\widetilde{\mathcal B}}^{\mathrm{U}(1)}_{\texttt{nfld}}|} \rangle$, $\langle {|\vec{\widetilde{ \mathcal E}}^{\mathrm{U}(1)}_{\texttt{nfld}}|^2} \rangle$, $\langle {|\vec{\widetilde{\mathcal B}}^{\mathrm{U}(1)}_{\texttt{nfld}}|^2} \rangle$, $\text{rms} (|\vec{\widetilde{ \mathcal E}}^{\mathrm{U}(1)}_{\texttt{nfld}}|)$, $\text{rms} (|\vec{\widetilde{\mathcal B}}^{\mathrm{U}(1)}_{\texttt{nfld}}|)$
    {: .cl-schema }

-   `average_gauss_U1_[nfld].txt`{ .cl-fname }

    Contains information about the conservation of Gauss law in the $\mathrm{U}(1)$ sector, which is in general obeyed to machine precision in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$. Here $\text{LHS}_{\texttt{nfld}}$ and $RHS_{\texttt{nfld}}$ are the left- and right-hand sides of Eq.$~$(17) from [**Scalar-gauge interactions**](My first model of gauge fields.md).

    $\tilde{\eta}$, $\frac{\langle \sqrt{(\text{LHS}_{\texttt{nfld}} - \text{RHS}_{\texttt{nfld}})^2} \rangle}{\langle \sqrt{(\text{LHS}_{\texttt{nfld}} + \text{RHS}_{\texttt{nfld}})^2} \rangle}$, $\langle \sqrt{(\text{LHS}_{\texttt{nfld}} - \text{RHS}_{\texttt{nfld}})^2} \rangle$, $\langle \sqrt{(\text{LHS}_{\texttt{nfld}} + \text{RHS}_{\texttt{nfld}})^2} \rangle$
    {: .cl-schema }

-   `average_topological_charges.txt`{ .cl-fname }

    Contains information about the topological charges associated to the $\textrm{U}(1)$ fields. A single file is created, and contains two columns per field species:

    $\tilde{ \eta}$, $\langle {\vec{\widetilde{ \mathcal E}}_{0}^{(2)}\cdot \vec{\widetilde{ \mathcal B}_{0}}^{(4)}} \rangle$, $\langle {(\vec{\widetilde{ \mathcal E}}_{0}^{(2)}\cdot \vec{\widetilde{ \mathcal B}}_{0}^{(4)})^2} \rangle$, $\ldots$
    {: .cl-schema }

</div>

Here, the electric and magnetic fields are defined using improved definitions,
```math
\begin{equation}
\begin{array}{rcl}
\widetilde{ \mathcal E}_{i}^{(2)} & \equiv & \displaystyle \frac{1}{2} (\widetilde{ \mathcal E}_{i} + \widetilde{ \mathcal E}_{i,-\hat{\imath}})\,,\\[5pt]
\widetilde{ \mathcal B}_{i}^{(4)} & \equiv & \displaystyle \frac{1}{4} (\widetilde{ \mathcal B}_{i} + \widetilde{ \mathcal B}_{i,-\hat{\jmath}} + \widetilde{ \mathcal B}_{i,-\hat{k}} + \widetilde{ \mathcal B}_{i,-\hat{\jmath}-\hat{k}})\,.
\end{array}
\end{equation}
```
where we have left implicit the species index, for simplicity.


For simulations with a $\mathrm{SU}(2)$ gauge field, `NSU2Flds > 0`, the following output is created:

<div class="grid cards cl-files" markdown>

-   `average_norm_SU2_[nfld].txt`{ .cl-fname }

    Contains averages of the electric and magnetic fields arising from the $\mathrm{SU}(2)$ sector. One file is created per field species $\texttt{nfld}$, with columns:

    $\tilde{ \eta}$, $\sum_i \langle {|\vec{\widetilde{ \mathcal E}}^{\mathrm{SU}(2)}_{\texttt{nfld},i}|} \rangle$, $\sum_i \langle {|\vec{\widetilde{\mathcal B}}^{\mathrm{SU}(2)}_{\texttt{nfld},i}|} \rangle$, $\sum_i \langle {|\vec{\widetilde{ \mathcal E}}^{\mathrm{SU}(2)}_{\texttt{nfld},i}|^2} \rangle$, $\sum_i \langle {|\vec{\widetilde{\mathcal B}}^{\mathrm{SU}(2)}_{\texttt{nfld},i}|^2} \rangle$, $\sum_i \text{rms} (|\vec{\widetilde{ \mathcal E}}^{\mathrm{SU}(2)}_{\texttt{nfld},i}|)$, $\sum_i \text{rms} (|\vec{\widetilde{\mathcal B}}^{\mathrm{SU}(2)}_{\texttt{nfld},i}|)$
    {: .cl-schema }

-   `average_gauss_SU2_[nfld].txt`{ .cl-fname }

    Contains information about the conservation of Gauss law in the $\mathrm{SU}(2)$ sector. Here LHS and RHS are the left- and right-hand sides of Eq.$~$(18) from [**Scalar-gauge interactions**](My first model of gauge fields.md) for each species.

    $\tilde{\eta}$, $\frac{\langle \sqrt{(\text{LHS}_{\texttt{nfld}} - \text{RHS}_{\texttt{nfld}})^2} \rangle}{\langle \sqrt{(\text{LHS}_{\texttt{nfld}} + \text{RHS}_{\texttt{nfld}})^2} \rangle}$, $\langle \sqrt{(\text{LHS}_{\texttt{nfld}} - \text{RHS}_{\texttt{nfld}})^2} \rangle$, $\langle \sqrt{(\text{LHS}_{\texttt{nfld}} + \text{RHS}_{\texttt{nfld}})^2} \rangle$
    {: .cl-schema }

</div>



In addition to these files related to field averages, results for the evolution of the scale factor are also saved with the same frequency for simulations running with self-consistent or fixed expansion, `expansion = true`. This file is not created for simulations without expansion. The file contains:

<div class="grid cards cl-files" markdown>

-   `average_scale_factor.txt`{ .cl-fname }

    By default, contains the following columns:

    $\tilde \eta$, $a$, $a'$, $a' / a$
    {: .cl-schema }

    In addition, an extra column is added at the end for some particular cases. For simulations with non-minimal couplings to gravity, this last column contains the value of the Ricci scalar, $R$, as given in Eq.$~$(5) of [**Non-Minimal Interactions**](NMC.md). On the other hand, for simulations of cosmic defects, the last column contains the value of the fattening factor, see Eq.$~$(16) of [**Cosmic Defects**](Defects.md).

</div>


Finally, **energy averages** are also measured, together with information about the energy conservation in the simulation.

<div class="grid cards cl-files" markdown>

-   `average_energies.txt`{ .cl-fname }

    Contains information about the energy density component of all field types and species. The last column always corresponds to the total matter energy density:

    $\tilde{\eta}$, $\tilde{E}_K^{(\phi, 0)}$, $\tilde{E}_G^{(\phi, 0)}$, $\ldots$, $\tilde{E}_K^{(\phi, N_s-1)}$, $\tilde{E}_G^{(\phi, N_s-1)}$, $\tilde{E}_K^{(\varphi, 0)}$, $\tilde{E}_G^{(\varphi, 0)}$, $\ldots$, $\tilde{E}_K^{(\varphi, N_c-1)}$, $\tilde{E}_G^{(\varphi, N_c-1)}$, $\tilde{E}_K^{(\Phi, 0)}$, $\tilde{E}_G^{(\Phi, 0)}$, $\ldots$, $\tilde{E}_K^{(\Phi, N_d-1)}$, $\tilde{E}_G^{(\Phi, N_d-1)}$, $\tilde{E}_K^{(A, 0)}$, $\tilde{E}_G^{(A, 0)}$, $\ldots$, $\tilde{E}_K^{(A, N_{u1}-1)}$, $\tilde{E}_G^{(A, N_{u1}-1)}$, $\tilde{E}_K^{(B, 0)}$, $\tilde{E}_G^{(B, 0)}$, $\ldots$, $\tilde{E}_K^{(B, N_{s2}-1)}$, $\tilde{E}_G^{(B, N_{s2}-1)}$, $\tilde{E}_V^{(0)}$, $\ldots$, $\tilde{E}_V^{(N_p-1)}$, $\langle \tilde{\rho} \rangle$
    {: .cl-schema }

-   `average_energy_conservation.txt`{ .cl-fname }

    Contains information about energy conservation (which is evaluated using the first Friedman equation in case of a self-consistent expanding background). The columns are:

    If no expansion:

    $\displaystyle\tilde{\eta}$, $1 - \frac{\langle \tilde{\rho} (\tilde{\eta} ) \rangle}{\langle \tilde{\rho} (\tilde{\eta}_*  ) \rangle}$
    {: .cl-schema }

    If self-consistent expansion:

    $\displaystyle \tilde{\eta}$, $\frac{\langle\text{LHS} - \text{RHS}\rangle}{\langle \text{LHS} + \text{RHS}\rangle}$, $\langle  \text{LHS} \rangle$, $\langle \text{RHS} \rangle$
    {: .cl-schema }

    where LHS and RHS here are the left- and right-hand sides of Eq.$~$(24) in [**Scalar-gauge interactions**](My first model of gauge fields.md). For simulations with non-minimally coupled scalars, this also incorporates the additional NMC contribution, see [**Non-Minimal Interactions**](NMC.md). The file is not created for fixed background expansion.

</div>

**Module-specific measurements**

In addition to the averages presented above, which are generated automatically for any model containing the correct field content, some other quantities are measured when using one of the modules implemented in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$.

If simulations are run with **gravitational waves**, indicating `withGWs = true` in the parameter file when running the simulation, an additional file is created containing an estimate of the total energy density of gravitational waves, see [**Gravitational Waves**](GW.md):

<div class="grid cards cl-files" markdown>

-   `average_energies_gws.txt`{ .cl-fname }

    Contains the volume-averaged GW energy density. The columns are

    [](){ #eq_GWenergymeasurements }
    $\tilde{\eta}$, $\tilde{\rho}_{\rm GW}/\tilde{\rho}$, $\tilde{\rho}_{\rm GW}$
    {: .cl-schema }

    where $\rho$ here corresponds to the total energy density of the matter sector.

</div>

For simulations of **cosmic defects**, see [**Cosmic Defects**](Defects.md) for more details, several additional files may be generated, containing information related to the defects. These involve the norm of the real scalar field, regarded as a vector of $N$ scalar fields,
[](){ #eq_globalNorm }
```math
\begin{equation}\label{eq_globalNorm}
|\phi|=\left(\sum_{a=1}^N \phi_a^2\right)^{1/2}\,,
\end{equation}
```
Mainly, we have the following two files:

<div class="grid cards cl-files" markdown>

-   `average_scalar_norm.txt`{ .cl-fname }

    It is generated only for simulations of global defects. It contains information about the norm of the real scalar field $|\phi|$, defined in Eq.$~$\eqref{eq_globalNorm}. In particular, the file contains four columns:

    $\tilde \eta$, $\langle |\tilde{\phi} |\rangle$, $\langle |\tilde{\phi}^2 |\rangle$, $\text{rms} (|\tilde{\phi}|)$
    {: .cl-schema }

-   `average_defects.txt`{ .cl-fname }

    Contains additional information about the defects. If `measureDefectsEnergies = true` is indicated in the parameter file, the energy components of the defects are saved. If `measureDefectsStructure = true` is indicated, estimates of the area of domain walls or the total length of cosmic strings is saved as the last column. See [**Cosmic Defects**](Defects.md) for a more detailed discussion on this output file.

</div>

In addition, if a diffusion phase of the initial conditions is performed, some additional files are generated, measuring similar quantities as discussed above during the diffusion phase. For more information on these, see [**Cosmic Defects**](Defects.md),

**HDF5 measurements**

As an alternative to the use of $\texttt{.txt}$ files, it is possible to save all results in a binary file based on the hdf5 format. To do so, one needs to first compile $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ using the HDF5 library, see [**HDF5: Output and Backups**](HDF5: Output and Backups.md) for more information in this regard. Once this is done, measurements can be saved to a binary field if `hdf5Averages = true` is specified in the parameter file. In this case, all measurements related to averages are saved to the $\texttt{average.h5}$ file. This contains a hdf5 group for each $\texttt{.txt}$ file that would be saved in the standard case, with name equal to the name of such $\texttt{.txt}$ file, with the $\texttt{average_}$ prefix removed. Inside each of these groups, a dataset contains the information for each of the columns of the corresponding $\texttt{.txt}$ file.

It is worth noting that there is an additional dataset inside the hdf5 file, $\texttt{Parameters}$, which contains information about the parameters used to run the simulation, in a similar fashion to the $\texttt{.infos}$ file. Each of the parameters is saved as an attribute of the empty dataset.

### **Power spectra** { #sec_PS }

The second set of observables that get measured in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ are power spectra, usually of field values. These are automatically measured for any simulation, with a frequency specified with the `tOutputInfreq` keyword in the parameter file (defaults to $10^2\delta \tilde{\eta}$ if unspecified). Measurement files are saved to the $\texttt{average_spectra_times.txt}$.

Before summarizing the possible output depending on the field content and the models, we briefly discuss about different options that the user has when running the simulation. For this, we will consider a generic quantity $f$ of which we want to measure the power spectrum, $\Delta_f$, and will later specify which this $f$ may correspond to.

The most basic options related to the power spectra are related to the choice of power spectra type and version. The power spectra type is selected with the `PSType` keywork in the parameter file, and accepts two values: `PS_type = 1` (default) for which the exact multiplicity of each bin, $\#_l$, is used to compute the power spectrum, and `PS_type = 2` for which the number of points per bin is approximated as $\#_l\approx 4\pi \tilde{k}(l)^2$. For each of these cases, the power spectrum version can be specified using the `PS_version` keyword, and accepts three values: `1` (default), `2` and `3`. They control how the momenta associated to each bin is estimated and used to compute the power spectrum. For more information, see the [Technical Note I](../tech_notes/Power%20spectra.md).

Next, we discuss about which quantities get saved. By default, spectra is saved in $\texttt{.txt}$ files, and the printed results can be grouped in three sets: a first set of columns corresponding to the binning, a second set of columns corresponding to the results for the power spectra of one or more quantities, and a final column that indicates the bin multiplicity, $\#_l$, of each bin $l$. Each line corresponds to a different bin, and each time the power spectrum is measured, the new results get appended at the end of the file, with a blank line separating the different measurement times. Note that this means that information about the bins and the bin multiplicity, although invariant, gets repeatedly saved for every measurement time.

What the columns of the first and second sets contain can be chosen by the user using the `spectraVerbosity` keyword in the parameter file, which can take three values. The content of the bin-related columns, for each choice of this parameter, is summarized in the following table:

| **Option** <div style="width:160px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `spectraVerbosity = 0` | Central values. By default, this corresponds to $\tilde{k}(l) = l k_\text{IR}$, although it changes if different bin width is used, as described below.  |
| `spectraVerbosity = 1` | $\langle\tilde{k}\rangle$, $\text{var}(\tilde{k})$, $\text{min}(\tilde{k})$, $\text{max}(\tilde{k})$ |
| `spectraVerbosity = 2` | Central value, $\langle\tilde{k}\rangle$, $\text{var}(\tilde{k})$, $\text{min}(\tilde{k})$, $\text{max}(\tilde{k})$ |

Second, for each quantity $f$ for which the power spectra is measured, the corresponding set of columns in the output file contains,

| **Option** <div style="width:160px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `spectraVerbosity = 0` | $\tilde{\Delta}_\tilde{f}(\tilde{k})$  |
| `spectraVerbosity = 1` | $\tilde{\Delta}_\tilde{f}(\tilde{k})$ |
| `spectraVerbosity = 2` | $\tilde{\Delta}_\tilde{f}(\tilde{k})$, $\text{var}[\tilde{\Delta}_\tilde{f}(\tilde{k})]$, $\text{min}[\tilde{\Delta}_\tilde{f}(\tilde{k})]$, $\text{max}[\tilde{\Delta}_\tilde{f}(\tilde{k})]$  |

In all cases, the last column of the fields corresponds to the bin multiplicity.

As an alternative to the use of $\texttt{.txt}$ files, it is possible to save the spectra in binary files with the hdf5 format. This is especially useful when working with large simulations or finer binning, as discussed below. The use of hdf5 files to save spectra results requires to compile the code with the HDF5 library, see [**HDF5: Output and Backups**](HDF5: Output and Backups.md), and to indicate `hdf5Spectra = true` in the parameter file.

The resulting file, $\texttt{spectra.h5}$, contains a series of groups, one per each spectra measured, which have the same name as the text files outlined below (without the $\texttt{.txt}$ suffix). Each of these groups contains a dataset corresponding to each of the columns of the text files. Results for bins and multiplicity only gets saved once, while for the fields spectra each consecutive measurements is saved in a different row of the dataset (saved in C++/python format). As of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ v2.0, only `spectraVerbosity = 0` and `spectraVerbosity = 1` are supported for HDF5 spectra output.

Finally, the user is also able to control the width of the bins with which the power spectrum is measured, using the `deltaKBin` keyword, which is specified in fractions of $k_\text{IR}$. If no value is indicated, the bin width defaults to $k_\text{IR}$. We note that for very thin binning some of the bins may be empty. These bins do not get printed to the output files. If you want to determine their location, their central momentum is at $k_\mathrm{IR}+n\Delta k_\mathrm{bin} k_\mathrm{IR}$, for some integer $n$.

Another important point to highlight is the initial point of the binning. As there is no value of ${k}$ smaller than ${k}_\text{IR}$, if the specified binning has width bigger that $k_\text{IR}$, the lower end of the first bin is set at $k_\text{IR}/2$. On the other hand, if the binning is smaller than $k_\text{IR}$, the first bin is chosen so that it is centered at $k_\text{IR}$.

Related to the binning, there is the alternative option to generate an unbinned power spectrum. This can be regarded as a power spectrum with a very fine and irregular binning, with a single bin for each different value of $k$ and no empty bin. For example, for the type-I and version 1 power spectrum (other versions and types are defined in analogy to the standard power spectrum), the unbinned result is defined as
```math
\begin{equation}
\Delta_f(l) = \frac{k(l)\delta x}{2\pi N^5 w_l}\#_l\langle|f(\mathbf{k})|^2\rangle_l\,,
\end{equation}
```
where here $l$ labels each bin, associated with a fixed value of $\tilde{k}_l=|\tilde{\mathbf{k}}_l|$, $\langle\cdot\rangle_l$ is the average of the function over all points in Fourier space with the same momentum magnitude, $\tilde{k}_l$, and $\#_l$ is the number of such points. In addition, we introduce a width function $w_l$ that corrects for the effective width of each bin, and which we define as
```math
\begin{equation}
k_\mathrm{IR} w_l=\left\{\begin{array}{lcl}
\displaystyle\frac{k_2-k_1}{2}-\frac{k_\mathrm{IR}}{2}\,, & \quad\quad\quad\quad& l=1\,,\\[7pt]
\displaystyle\frac{k_{l+1}-k_{l-1}}{2}\,, & \quad\quad\quad\quad& 1 < l < l_\mathrm{max}\,,\\[7pt]
k_{l_\mathrm{max}}-k_{l_\mathrm{max}-1}\,, & \quad\quad\quad\quad & l=l_\text{max}\,.
\end{array}\right.
\end{equation}
```
where the width choices for the first and last bins are just conventions. We note that, while $l$ takes continuous integer values, it does not coincide with $k^2/k_\text{IR}^2$ for 3 or less dimensions, as not all integers can be written as the sum of three or less squares (this is possible for four or more dimensions).

To activate the unbinned power spectrum, one needs to compile the code with the HDF5 library active, and indicate `saveUnbinnedSpectra = true` in the parameter file. This option substitutes all the standard spectrum savers for an unbinned version, which gets saved in a file named $\texttt{unbinned_spectra.h5}$. This file contains a group per spectra measured, similar to the case of standard hdf5 spectra, with the prefix $\texttt{unbinned}$. Similarly to standard spectrum, as of $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ v2.0, only `spectraVerbosity = 0` and `spectraVerbosity = 1` are supported for HDF5 unbinned spectra output.

For user's seeking more flexibility to contruct their own power spectra with a custom normalization or binning from the unbinned results, $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ offers the possibility of simply outputing the radial averages, $\langle|f(\mathbk{k})|^2\rangle_l$, for each momentum value. Thi is achieved by setting `PS_type = 0`, option that is obnly valid if `saveUnbinnedSpectra = true`. Note that, in this case, the total energy density of gravitational waves is not computed, this is, the file $\texttt{average_energies_gws.txt}$ is not created, although the result for this quantity can be determined from the user in an a posteriori analysis.

**Power spectrum files**

We now summarize the spectrum files that get measured in $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$, depending on the model, the field content and the choices made in the parameter file. For each case, we indicate the quantities for which the power spectra is measured. We note that for matter fields, a file is generated per species.

<div class="grid cards cl-files" markdown>

-   `spectra_scalar_[nfld].txt`{ .cl-fname }

    For models with `NScalars > 0`, it contains the spectra of $\tilde\phi_\texttt{nfld}$ and $\tilde\phi'_\texttt{nfld}$.

-   `spectra_ON_scalar_[nfld].txt`{ .cl-fname }

    For models with `NScalars > 0` and if `ONflag = true` is indicated in the parameter file, it contains the occupation number of each scalar singlet field. This is defined (for `NDim = 3`) as (<span style="color:orange;">**Jorge comment: Please somebody check**</span>)

</div>

```math
\begin{equation}
\tilde{\Delta}^\mathrm{ON}_{\texttt{nfld}}=\frac{a^2}{3}\left(\frac{\delta \tilde{x}}{N}\right)^3\left(\frac{f_*}{\omega_*}\right)^2\left[\left\langle\left|\tilde{\phi}_{\texttt{nfld}}\right|^2\right\rangle + \left\langle\left|\tilde{\phi}_{\texttt{nfld}}'+\frac{a'}{a}\tilde{\phi}_{\texttt{nfld}}\right|^2\right\rangle\right]\,.
\end{equation}
```

<div class="grid cards cl-files" markdown>

-   `spectra_norm_cmplx_scalar_[nfld].txt`{ .cl-fname }

    For models with `NCScalars > 0`, it contains the spectra of $|\tilde\varphi_\texttt{nfld}|$ and $|\tilde\varphi'_\texttt{nfld}|$.

-   `spectra_norm_SU2Doublet_scalar_[nfld].txt`{ .cl-fname }

    For models with `NSU2Doublet > 0`, it contains the spectra of $|\tilde\Phi_\texttt{nfld}|$ and $|\tilde\Phi'_\texttt{nfld}|$.

-   `spectra_norm_U1_[nfld].txt`{ .cl-fname }

    For models with `NU1Flds > 0`, contains the spectra of $|\widetilde{\mathcal{E}}^{\mathrm{U}(1)}_\texttt{nfld}|$ and $|\widetilde{\mathcal{B}}^{\mathrm{U}(1)}_\texttt{nfld}|$

-   `spectra_norm_SU2_[nfld].txt`{ .cl-fname }

    For models with `NSU2Flds > 0`, contains the spectra of $|\widetilde{\mathcal{E}}^{\mathrm{SU}(2)}_\texttt{nfld}|$ and $|\widetilde{\mathcal{B}}^{\mathrm{SU}(2)}_\texttt{nfld}|$

-   `spectra_energy_gws.txt`{ .cl-fname }

    For models run with `withGWs = true`, contains the fractional energy density power spectrum of GWs. See [**Gravitational Waves**](GW.md) for more details.

-   `spectra_norm.txt`{ .cl-fname }

    For models with `NScalars > 0` and `DefectsModel = true`, measures the spectra of $|\phi|$ as defined in Eq.$~$\eqref{eq_globalNorm}. See [**Cosmic Defects**](Defects.md) for more details.

-   `spectra_chiral_U1_0.txt`{ .cl-fname }

    For models with axion couplings, measures the chiral power spectra of the $\mathrm{U}(1)$ field. See [**Axion-Gauge Interactions**](ALP.md) for more details.

-   `spectra_chiral_Elec_U1_0.txt`{ .cl-fname }

    For models with axion couplings, measures the chiral power spectra of the electric $\mathrm{U}(1)$ field. See [**Axion-Gauge Interactions**](ALP.md) for more details.

</div>

### **Snapshots** { #sec_snapshots }

The last type of observables that can be measured with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ are three-dimensional ($N$-dimensional for simulations in $N$ spacial dimensions) snapshots of the energy components and field amplitudes. To measure these snapshots, one needs to compile the model with the HDF5 library active, see [**HDF5: Output and Backups**](HDF5: Output and Backups.md) for an explanation. The snapshots to be measure are specified as a space-separated list to the `snapshots` keyword, and the frequency of the measurements gets indicated using `tOutputRareFreq`, which defaults to $10^3\delta \tilde{\eta}$. The possible options for the `snapshots` parameter are summarized in the following table.

| **Keyword** <div style="width:80px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `S` | Values of scalar singlet fields, saved to $\texttt{snapshot_scalar_singlet.h5}$. |
| `Snorm` | Norm of scalar singlet fields, as specified in Eq.$~$\eqref{eq_globalNorm}. Snapshots are saved to $\texttt{snapshot_scalar_singlet_norm.h5}$. Only valid for simulations of cosmic defects with `DefectsModel = true`, see [**Cosmic Defects**](Defects.md). |
| `E_S_K` | Kinetic energy of scalar singlet fields, saved to $\texttt{kinetic_energy_snapshot_scalar.h5}$. |
| `E_S_G` | Gradient energy of scalar singlet fields, saved to $\texttt{gradient_energy_snapshot_scalar.h5}$. |
| `CS` | Absolute values of complex scalar fields, saved to $\texttt{snapshot_complex_scalar.h5}$. |
| `E_CS_K` | Kinetic energy of complex scalar fields, saved to $\texttt{kinetic_energy_snapshot_complex_scalar.h5}$. |
| `E_CS_G` | Gradient energy of complex scalar fields, saved to $\texttt{gradient_energy_snapshot_complex_scalar.h5}$. |
| `E_SU2D_K` | Kinetic energy of doublet scalar fields, saved to $\texttt{kinetic_energy_snapshot_SU2_doublet.h5}$. |
| `E_SU2D_G` | Gradient energy of doublet scalar fields, saved to $\texttt{gradient_energy_snapshot_SU2_doublet.h5}$. |
| `E_A_K` | Electric energy of the $\mathrm{U}(1)$ gauge sector, saved to $\texttt{electric_energy_snapshot_U1.h5}$. |
| `E_A_G` | Magnetic energy of the $\mathrm{U}(1)$ gauge sector, saved to $\texttt{magnetic_energy_snapshot_U1.h5}$. |
| `E_B_K` | Electric energy of the $\mathrm{SU}(2)$ gauge sector, saved to $\texttt{electric_energy_snapshot_SU2.h5}$. |
| `E_B_G` | Magnetic energy of the $\mathrm{SU}(2)$ gauge sector, saved to $\texttt{magnetic_energy_snapshot_SU2.h5}$. |
| `E_V` | Potential energy, saved to $\texttt{potential_energy_snapshot.h5}$. |
| `E` | Total energy of the scalar and gauge sectors, saved to $\texttt{total_energy_snapshot.h5}$. |

For example, a valid option for a model of scalar field could be
```text
snapshots = S E_S_K E_V
```
for which snapshots of the field, its kinetic energy density and the total potential energy will be saved.


Each of these snapshots gets saved to a different binary file. For example, if `S` is specified, the potential energy is saved to the `snapshot_scalar_singlet.h5` file, as indicated in the table. This file contains a different hdf5 group for each scalar species, and each group contains a series of three-dimensional datasets named with the time at which the snapshot was taken. We recommend the user to take a look at the generated files to get familiar with these naming conventions.

!!! note Compatibility note
    In CosmoLattice versions previous to v2.0, snapshots were indicated using the `energy_snapshots` keyword. This is still supported for compatibility reasons, but will be removed in a future version.

In addition to saving the three-dimensional distributions, since $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ v2.0 it is possible to save snapshots of subvolumes or of a sparse grid, which may be very handy for visualization of very large simulations. For example, it is possible to only output a two-dimensional slice of the simulation. These options are controlled with the following three keywords, to which a set of $\texttt{NDim}$ integers needs to be indicated:

| **Keyword** <div style="width:150px"> | **Explanation** |
| ---------------------------------------- | --------------- |
| `snap_lowercoord` | Lowest coordinates to output in each dimension, between `0` and `N-1`. Defaults to `0` in all dimensions. |
| `snap_uppercoord` | Upper end of the snapshots. Corresponds to the first non-included coordinate. Defaults to `N` in all dimensions. |
| `snap_stepcoord` | Stepping used for the snapshots. This is, starting from the coordinates indicated in `snap_lowercoord` only one every `snap_stepcoord` points is saved to the snapshots. Defaults to `1` in all dimensions.  |

For example, for a three-dimensional simulation with `N=2048`, one could output a two-dimensional snapshot in a sparse grid using:
```text
snap_lowercoord = 0 0 1023
snap_uppercoord = 2048 2048 1024
snap_stepcoord = 2 2 1
```
In particular, this would generate snapshots of the two-dimensional slice with $n_3=1024$, saving points with even $n_1$ and $n_2$ coordinates.

Finally, we note that in the case of defect models run with an initial phase of diffusion it is possible to also save snapshots of the fields during the diffusion phase. These are controlled using the `snapshots_diffusion` and `tOutputRareFreqDiff` keyword, see [**Cosmic Defects**](Defects.md) for further details.


