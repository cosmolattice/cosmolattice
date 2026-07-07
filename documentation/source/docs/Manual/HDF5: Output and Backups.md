Besides its [parallel support](Parallelization.md), another useful feature provided by CosmoLattice — through [`TempLat`](https://github.com/cosmolattice/templat), the lattice library that forms its backbone — is the possibility of saving and restarting simulations, as well as the possibility of having an automatic backup every given number of iterations. This feature is enabled through a `CMake` flag; we elaborate about it in Section [*Saving three-dimensional field distributions, backups and other options: HDF5*][subsec_hdf5spec]. Using the same external library (`HDF5`), we also provide the user with the possibility of saving spectra in `HDF5` format, which has the advantage of being more structured than the default text files. We explain this also below.

### Saving three-dimensional field distributions, backups and other options: `HDF5` { #subsec_hdf5spec }

When running long simulations, it may come very handy to be able to stop them and restart them later on, or to have some kind of automatic backup in case some problem happens to the hardware you are using. In order to implement this type of features, we need to be able to save the field distributions to a file. For the sake of portability, the current version of CosmoLattice uses the `HDF5` library to perform this task in a binary format. This means that, if you want to use one of the features that involve saving a three-dimensional distribution of some fields to a file, you will need to have a working `HDF5` library installed (see Appendix [Installation](Installation.md) on how to do this). Assuming you have such installation, activating these features in CosmoLattice is as simple as using another `CMake` flag:
```bash
cmake -DHDF5=ON -DMODEL=lphi4 ../
make cosmolattice
```
We will now survey what features this unlocks.

#### Saving a simulation to disk
After having activated `HDF5`, we can now save runs to disk. This is simply done via the argument `save_dir`, which you can add to your input file, or simply pass it through the command line. For instance,
```bash
./lphi4 input=input.in save_dir=./
```
will save this `lphi4` run at the end in the current folder. It is going to create a file named `lphi4_DATE_d**_m**_y**_TIME_h**_m**_s**.h5`, where `lphi4` is the model name, and the $**$ symbols will be replaced by the actual date and time.

You do not need to know anything about the actual content of the file in order to restart your simulation from there. However, thanks to the standardized `HDF5` format, you can easily go and explore the content of the file with your favorite data visualization tool, be it `gnuplot`, `Mathematica`, `Matlab`, `python`, `Julia` or other, as long as it supports `HDF5`. To simplify, a `HDF5` file mimics a folder/file structure, folders being designated as ''groups" and files as ''datasets". In this case, every field is stored in a separate dataset. For simplicity, we also store the values of the scale factor, its time derivative, and the final time as separate datasets.

#### Restarting a saved simulations

Once a saved simulation file has been created, it is straightforward to restart the simulation from the same time when you stopped it and saved it. To do so, you only need to call your executable with the ''load_dir" parameter set to the simulation file you want to restart from. It will also read the parameters of the previous simulation and use them. Except for the lattice size $N$, the length side $L$, and the infrarred and ultraviolet cutoffs $k_{\rm IR}$ and $k_{\rm UV}$, you can override the other parameters by either specifying them through the console line or in an output file. Not that if you try to override a parameter that you are not allowed to, the program will not crash, but simply ignore your changes.

To be concrete, let us assume that the above simulation was saved at the time `tMax=200`. If we want to continue the simulation, we can simply relaunch it with a different parameter (say  `tMax=500`) as follows,
```bash
./lphi4 load_dir=lphi4_DATE_d**_m**_y****_TIME_h**_m**_s**.h5 tMax=500
```
As mentioned above, you can also use an input file as usual.

Note that when you run in restart mode, assuming you have not moved the previous output files, the new results will be overwrite the previous files. You can change this behavior by setting explicitly the parameters `appendToFiles` to `true`.

#### Automatic backup

With the ''start and stop" mechanism presented in the section above, it is natural to implement an automatic way of backing up the simulation to disk, in order to be able to recover from some hardware failure. This option is turned on by specifying the parameter `tBackupFreq`. Then, every `tBackupFreq` amount of program time, the simulation will write itself to disk in a file name `ModelName.backup` (`lphi4.backup` for instance). If a backup file is already present, it will first rename it to `ModelName.backup` before creating the backup. This extra amount of precaution allows you not to loose the whole simulation in case your hardware crashes while you are backing-up. By default, the backup file is saved in the same folder than the measurements. You can change this behaviour by specifying the `backup_dir` parameter.

*Note:* In the current implementation of CosmoLattice, the saving of three-dimensional field configuration has not particularly been optimized for performances. As such, it is a good idea not to use a backing up frequency that is too high. You can determine what ''too high" means by trial and error, seeing how much the backing up affects performance on your hardware.

#### Saving three dimensional energy distributions

CosmoLattice is also capable of saving three-dimensional distributions of arbitrary observables in a file. At present, the user can save three-dimensional distributions of the various energy components of the system by adding different flags to the `snapshots` parameter in the parameter file (previously named `energy_snapshot`, which still works as a deprecated alias). The different flags are indicated in Appendix [Appendix: Parameters](Appendix_Parameters.md). Let us show an example: suppose we are running the `lphi4SU2U1` gauge model and we want to save to file the scalars kinetic energies and the $SU(2)$ electric energy. We would then run
```bash
./lphi4SU2U1 input=input.in snapshots="E_S_K E_B_K"
```
Again, as usual, this parameter can go in the input file (in which case you do not need the quotes surrounding the arguments).

#### A more user-friendly format for the spectra { #subsubsec_hdf5spectra }

If you want to compute the field spectra with a very fine resolution binning on large lattices, the corresponding text files storing them may occupy a significant amount of disk memory. However, if you compile CosmoLattice with `HDF5`, you have access to a new way of storing the spectra.
In particular, this problem can be mitigated if the spectra are saved in `HDF5` format, which are binary files. Furthermore, thanks to the internal structure of `HDF5` files, spectra at different times can easily be retrieved. Our `HDF5` spectra files are structured as follows. First, every time is its own group (''folder"). Inside this group, there will be a dataset called `momBinAverage`, which contains the average momentum in a bin, as well as another one called `momBinMultiplicity` which tells you how many values where binned in this bin. Then, there is a dataset for each of the $n$ spectra saved in the given file, named `spectAverage_i` with `i$=0,1,\dots,n-1$. This information is always printed for the default spectra verbosity, but if you choose a higher verbosity, you will also get a dataset containing the variance, minimum and maximum values of the momenta and spectra bins.
