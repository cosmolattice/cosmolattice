# Migrating from $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 1.x

If you already have a working 1.x setup, this page is the complete list of what you have to change. Your physics does not change: model files, input files and output formats are essentially the same.

## The 5-minute upgrade

```bash
git clone https://github.com/cosmolattice/cosmolattice.git   # fresh clone, do not pull on top of 1.x
cp /old/cosmolattice/src/models/myModel.h            cosmolattice/models/
# edit myModel.h: one line, see below
cd cosmolattice && mkdir build && cd build
cmake -DMODEL=myModel ..        # add -DMPI=ON -DHDF5=ON as before
make -j8
./myModel input=myModel.in
```

Nothing else is needed if you have not changed $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ internals.

!!! warning "Do not upgrade in place"

    The layout changed (see [below](#folder-structure)) and TempLat is no longer vendored in the repository. Clone v2 fresh and copy your model and input files over.


## 1. Your model file: one required change

The toolbox argument of the model constructor changed type. Replacing it by `auto` is the least intrusive fix:

```diff
-  MODELNAME(ParameterParser& parser, RunParameters<double>& runPar,
-            std::shared_ptr<MemoryToolBox> toolBox):
+  MODELNAME(ParameterParser& parser, RunParameters<double>& runPar,
+            auto toolBox):
   Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL))
```

`MemoryToolBox` is now templated on the number of spatial dimensions and held in a device-aware pointer, so the explicit form is `device::memory::host_ptr<MemoryToolBox<NDim>> toolBox`. Both work; `auto` is shorter and stays correct if the type evolves.

**That is the only mandatory edit.** All members of the model class, as well as the whole symbolic language interface, are unchanged.

??? tip "Migrating larger changes to CosmoInterface"

    If you have modified the CosmoInterface layer, you will have to port those changes manually. CosmoInterface and the code itself have not changed much beyond the abstract model (which has now a modular structure) and the new physics features introduced with $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 2.0. The symbolic language interface is unchanged.

??? tip "Optional: you can now run your model in single precision"

    Add `using FloatType = float;` to your `ModelPars` and replace `double` by `FloatType` throughout the model (including `RunParameters<FloatType>&` and `parser.get<FloatType>(...)`). It halves field memory. Left out, `FloatType` defaults to `double` and everything behaves as in 1.x.

## 2. External libraries: no more shell scripts

Delete your `dependencies/` habits. All library handling is done by CMake.

| $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 1.x               | $\mathcal{C}\mathtt{osmo}\mathcal{L}\mathtt{attice}$ 2.0                                     |
| ---------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| `./dependencies/fetchall.sh <path>`                                    | *(nothing — CMake does it)*                                                                  |
| `./dependencies/fftw3.sh`                                              | system package, or `-DAUTOBUILD_FFTW=ON`                                                     |
| `./dependencies/hdf5.sh`                                               | system package, or `-DHDF5=ON -DAUTOBUILD_HDF5=ON`                                           |
| `./dependencies/pfft.sh`                                               | *(dropped — see [ParaFaFT](#3-pfft-parafaft))*                                               |
| editing `MYFFTW3_PATH` / `MYHDF5_PATH` / `MYPFFT_PATH` in `CMakeLists` | `-DCMAKE_PREFIX_PATH=/path/to/lib` (the two `MY*_PATH` variables still work, with a warning) |

Kokkos, ParaFaFT and TempLat are downloaded and built automatically at configure time — you never install them by hand.
Note that both FFTW and HDF5 can also be built automatically by TempLat's CMake interface, if you do not have them installed system-wide. See [CMake Flags](cmake.md#auto-building-dependencies) for details.

!!! note "Configure needs network access"

    The first `cmake ..` clones TempLat (and Kokkos) from GitHub. On a login node without outbound access, configure once where you do have network and reuse the build tree, or point CMake at a local clone with `-DFETCHCONTENT_SOURCE_DIR_TEMPLAT=/path/to/templat`.

## 3. PFFT → ParaFaFT

PFFT is gone. Parallel FFTs in more than one direction are now provided by [ParaFaFT](https://github.com/cosmolattice/parafaft), which is header-only, built for you, and has `fftw3`, `cuFFT` and `hipFFT` backends.

```diff
-  cmake -DMODEL=myModel -DMPI=ON -DPFFT=ON -DMYPFFT_PATH=/opt/pfft ..
+  cmake -DMODEL=myModel -DMPI=ON ..
```

`-DPARAFAFT=ON` is implied by `-DMPI=ON`; set it explicitly only if you want to be verbose. Passing `-DPFFT=ON` or `-DMYPFFT_PATH=...` is a hard configure error.

## 4. Compiler and CMake minimums

| Requirement  | 1.x   | 2.0                                   |
| ------------ | ----- | ------------------------------------- |
| C++ standard | C++14 | **C++20**                             |
| g++          | 5     | **10.1** (11.4 is the oldest we test) |
| clang++      | —     | **13**                                |
| CMake        | 3.0   | **3.16**                              |

On clusters this is usually the only real obstacle: `module avail gcc` and load a $\ge$ 10.1 toolchain (and an MPI version compatible with the compiler). GPU builds additionally need the CUDA toolkit $\ge$ 12.0 or HIP $\ge$ 6.2.0. Full table in [Requirements](Installation.md#subsec_requirements).

## 5. Folder structure { #folder-structure }

TempLat — the template/lattice engine — is now a [separate repository](https://github.com/cosmolattice/templat) and CosmoLattice is only the physics layer on top of it. Everything moved one level up out of `src/`:

<div class="grid" markdown>

```text
CosmoLattice 1.x
├── src/
│   ├── include/CosmoInterface/
│   ├── include/TempLat/
│   ├── models/
│   │   └── parameter-files/
│   ├── cmake/
│   └── tests/
└── dependencies/
```

```text
CosmoLattice 2.0
├── include/CosmoInterface/
├── models/
│   └── parameter-files/
├── source/cosmolattice.cpp
├── cmake/
└── tests/
        (TempLat: fetched into
         build/_deps/templat-src)
```

</div>

Practical consequences:

- Your model goes in `models/`, your input file in `models/parameter-files/` — so the run command becomes `input=../models/parameter-files/myModel.in` (no `src/`).
- A per-model `myModel.cmake` next to the model file is still picked up automatically.
- **If you patched TempLat in 1.x**, those edits are not carried over. Fork [`cosmolattice/templat`](https://github.com/cosmolattice/templat) and build against it with `-DTEMPLAT_REPO=my-repo`, or against a local clone with `-DFETCHCONTENT_SOURCE_DIR_TEMPLAT=/path/to/templat`. Never edit `build/_deps/templat-src` — it will be overwritten.

## 6. CMake flags

Most old flags still work but warn; `-DFLOATFFT` and `-DTESTING` are silently ignored, so rename those. The ones you actually used:

| 1.x              | 2.0                                                                      |
| ---------------- | ------------------------------------------------------------------------ |
| `-DPFFT=ON`      | *(nothing — implied by `-DMPI=ON`)*                                      |
| `-DFLOATFFT=ON`  | `-DFLOAT=ON`                                                             |
| `-DG++OPT=Ofast` | *(nothing — `Release` is the default)*                                   |
| `-DG++SSE=ON`    | `-DSSE=ON`                                                               |
| `-DG++AVX=mavx2` | `-DAVX=mavx2`                                                            |
| `-DTESTING=ON`   | `-DCOSMOINTERFACE_TEST=ON` (`-DTEMPLAT_TEST=ON` for TempLat's own tests) |

New in 2.0 and worth knowing: `-DCUDA=ON` / `-DHIP=ON` (GPUs), `-DOPENMP=ON` / `-DPTHREADS=ON` / `-DNOTHREADING=ON` (shared memory), `-DNATIVE` (on by default on Linux, replaces the hand-written `-march=native`). The complete list is in [CMake Flags](cmake.md).

## 7. Input files and running

Input files from 1.x are read as-is. Two things to know:

- `OccNumb` was renamed to `flagON`.
- Everything new (`saveUnbinnedSpectra`, `ICtype_S`, `ICtype_U1`, `a0`, GW and defect options, …) is optional and defaults to 1.x behaviour.

Running is unchanged, and `make cosmolattice` still builds the currently selected model:

```bash
./myModel input=../models/parameter-files/myModel.in            # serial
mpirun -n 8 ./myModel input=../models/parameter-files/myModel.in # MPI
```

Simulations parallelised over more than one direction no longer require any extra flag or library — just give `mpirun` more ranks than $N$ allows in one direction.

## What you get for free

Once you build, the same model file also gives you, besides all the new physics features of v2.0, GPUs (`-DCUDA=ON`), OpenMP+MPI hybrid runs, single precision, unbinned spectra, sub-volume snapshots and the new Runge–Kutta integrators. See the [Version Guide](Versions.md#subsec_CLv2p0) for the full list.
