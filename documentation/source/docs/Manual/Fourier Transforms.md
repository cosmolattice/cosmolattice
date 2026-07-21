Fourier transforms are at the heart of many of the operations performed by the `CosmoInterface`. They are used, among other things, to set the initial fluctuations of the fields at the beginning of a run, and to compute power spectra of the various fields during the evolution. Unlike the local, finite-difference operations (gradients, Laplacians, ...), a Fourier transform is intrinsically *non-local*: every output mode depends on all input sites. This is what makes it the most delicate operation to parallelize, and the reason CosmoLattice relies on dedicated, highly optimized FFT libraries to perform it. Within CosmoLattice these libraries are driven by [`TempLat`](https://github.com/cosmolattice/templat), the lattice library that forms its backbone.

The good news is that, just as with the parallelization strategies described in Section [*Parallelization*][subsubsec_para1D], the user normally does not have to choose an FFT library by hand. CosmoLattice ships support for three backends and automatically builds the appropriate ones for your hardware during the `CMake` configuration.
All the user has to do is enable the relevant capabilities through a few `CMake` flags; the code takes care of the rest, and the same model runs unchanged on a laptop, a multi-core workstation, a GPU, or a large HPC cluster.

!!! note "Fourier conventions"
    The continuum and discrete Fourier transform conventions used throughout CosmoLattice are defined in Section [Conventions and Notation](Conventions and Notation.md) [see Eqs. ([*1*][eq_FTcont]) and ([*2*][eq_FTdiscreteAux])]. The backends documented here only change *how* the transform is computed, never *what* is computed: all of them reproduce, up to round-off, the same discrete transform.

### The three backends { #subsec_fft_backends }

CosmoLattice can compute its Fourier transforms with any of the following libraries:

- **`FFTW`** — the always-present CPU backend. It can be used for both serial runs and for distributed runs parallelized along a single direction. With GPU builds, it can be used, incurs however copy overhead between GPU and CPU RAM, and is therefore not recommended for performance-critical runs.
- **`KokkosFFT`** — the single-node GPU backend. It is pulled in automatically whenever you build for a GPU, and forwards the transform to the vendor library (`cuFFT` on NVIDIA, `hipFFT` on AMD) through [Kokkos](https://kokkos.org).
- **`ParaFaFT`** — the distributed backend for transforms parallelized along *several* directions. It is the modern replacement for the `PFFT` library used in CosmoLattice 1.0 and builds either on top of either `FFTW` on CPUs, or `cuFFT`/`hipFFT` on GPUs. It is only used when the code is built with `MPI` and automatically distributes over $d-1$ spatial dimensions.

The following table summarizes when each backend is used and how it is enabled:

| Backend     | Hardware           | Distributed FFT          | Used when                                 | Enabled by                       |
| ----------- | ------------------ | ------------------------ | ----------------------------------------- | -------------------------------- |
| `FFTW`      | CPU                | along one dimension      | CPU-build without MPI                     | always available                 |
| `KokkosFFT` | GPU (NVIDIA / AMD) | no (single node)         | single-process GPU runs                   | `-DCUDA=ON` or `-DHIP=ON` (auto) |
| `ParaFaFT`  | CPU or GPU         | along several dimensions | multi-process runs in $\geq 2$ dimensions | `-DMPI=ON -DPARAFAFT=ON`         |

### How a backend is chosen { #subsec_fft_selection }

At run time CosmoLattice inspects the build options and the number of MPI processes `nproc` and selects a backend according to the following priority:

1. If the code was built with `MPI` **and** `ParaFaFT`, and you run on more than one process (`nproc > 1`) in two or more spatial dimensions, the distributed **`ParaFaFT`** backend is used.
2. Otherwise, if the code was built for a GPU (so that `KokkosFFT` is available) and you run on a single process, the **`KokkosFFT`** backend is used.
3. In all other cases, CosmoLattice falls back to **`FFTW`**, which is always linked.

!!! note
    Because `FFTW` is always available, a CosmoLattice executable always has a working FFT backend, whatever combination of flags you compiled with. The optional backends simply take over when they are both compiled in and applicable to the run at hand.

The full list of FFT-related `CMake` flags is collected in [CMake flags](../code/cmake.md). We now describe each backend in turn.

### `FFTW`: the CPU backend { #subsec_fft_fftw }

[`FFTW`](https://www.fftw.org) ("Fastest Fourier Transform in the West") is the default backend and is **always linked into CosmoLattice** — you do not need any special flag to use it. It is used for single-node runs and for distributed runs parallelized along a single direction (see Section [*Distributed parallelization in one direction*][subsubsec_para1D]), where its built-in `MPI` transforms split the lattice along one direction.

By default the transforms are computed in double precision. You can switch to single precision — which roughly halves the memory footprint of the transform at the (heavy) cost of precision — by passing
```bash
cmake -DFLOAT=ON -DMODEL=lphi4 ../
make cosmolattice
```
`FFTW` is expected to be available on the system. If it is not, you can let `CMake` download and build it for you with `-DAUTOBUILD_FFTW=ON`. See [Installation](../code/Installation.md) for installation details and guidance on HPC clusters.

### GPU Fourier transforms: `KokkosFFT` { #subsec_fft_kokkos }

When CosmoLattice is compiled for a GPU, single-node Fourier transforms are handled by [`KokkosFFT`](https://github.com/kokkos/kokkos-fft). This keeps the transform on the device — there is no need to copy the fields back to the host — and dispatches it, through `Kokkos`, to the vendor's native FFT library:

- `cuFFT` on **NVIDIA** GPUs (`-DCUDA=ON`),
- `hipFFT` on **AMD** GPUs (`-DHIP=ON`).

You do not enable `KokkosFFT` explicitly: the `-DKOKKOSFFT` flag is switched **on automatically** as soon as a GPU backend is requested (and single-precision `FFTW`, `-DFLOAT`, is enabled along with it). A single-GPU run therefore needs nothing more than the usual GPU build:
```bash
cmake -DCUDA=ON -DMODEL=lphi4 ../   # NVIDIA GPU
cmake -DHIP=ON  -DMODEL=lphi4 ../   # AMD GPU
make cosmolattice
```

!!! note "Single device only"
    `KokkosFFT` handles transforms living entirely on a **single** GPU. To spread a simulation across **several GPUs or nodes**, combine a GPU build with the distributed `ParaFaFT` backend described below, whose `cuFFT`/`hipFFT` backends likewise keep the FFT on the device. The choice between `KokkosFFT` (one device) and `ParaFaFT` (several devices) is then made automatically depending on `nproc`.

### Distributed Fourier transforms: `ParaFaFT` { #subsec_fft_parafaft }

Parallelizing the FFT along a single direction limits the number of processes to the linear lattice size $N$. To scale further, the transform itself must be distributed over more than one direction. CosmoLattice does this with [`ParaFaFT`](https://github.com/cosmolattice/parafaft) (*Parallel Fast Fourier Transform*), a modern, header-only C++ library that **replaces the `PFFT` library used in CosmoLattice 1.0**.

`ParaFaFT` distributes a $D$-dimensional transform over a *pencil* decomposition built on a $(D-1)$-dimensional grid of MPI processes (see the right-hand side of Fig. [*3*][fig_parageo] in Section [*Distributed parallelization in multiple directions*][subsubsec_para2D]). Rather than performing explicit local transposes between the successive 1D transforms, it redistributes the data directly with advanced `MPI` collective operations (`MPI_Alltoallw` over subarray datatypes), following the algorithm of Dalcin, Mortensen & Keyes [@Dalcin_2019]. Like `KokkosFFT`, it offers `fftw3` (CPU), `cuFFT` (NVIDIA) and `hipFFT` (AMD) backends, so the same distributed transform runs on CPUs or on GPUs.

To enable it, pass `-DPARAFAFT=ON`. However, note that with `-DMPI=ON`, `ParaFaFT` is automatically enabled, so the following will use `ParaFaFT` for distributed runs in two or more dimensions:
```bash
cmake -DMPI=ON -DMODEL=lphi4 ../
make cosmolattice
```
and launch the run with `mpirun` (or your cluster's equivalent):
```bash
mpirun -n nproc lphi4 input=...
```

!!! warning "`ParaFaFT` requires `MPI`"
    The `-DPARAFAFT=ON` flag only takes effect together with `-DMPI=ON`. `ParaFaFT` is also a genuinely *multi-dimensional* distributed transform: it is used for runs with at least two spatial dimensions distributed over more than one process. Single-process runs fall back to `FFTW` (or `KokkosFFT` on a GPU) automatically.

!!! note "Migrating from CosmoLattice 1.0"
    Support for the old `PFFT` backend has been discontinued. The flags `-DPFFT` and `-DMYPFFT_PATH` are no longer recognized — use `-DPARAFAFT=ON` instead. See [CMake flags](../code/cmake.md) for the full list of deprecated options and their replacements.

### Constraints and practical notes { #subsec_fft_notes }

A few constraints are worth keeping in mind when planning large or unusual runs:

- **Lattice divisibility.** For distributed runs, the number of sites per dimension $N$ must be divisible by the number of processes used along each parallelized direction. When all directions share the same $N$, this simply means $N$ must be an integer multiple of the number of processes (see Section [*Parallelization*][subsubsec_para2D]).
- **Dimensionality.** `ParaFaFT` is used for lattices with at least two spatial dimensions; in one-dimensional or single-process runs CosmoLattice falls back to `FFTW`. `KokkosFFT` covers the single-node GPU case in up to three dimensions. Note that in 1D, TempLat does not support distributed parallelization.
- **Precision.** Single precision support (`-DFLOAT=ON`) is optional on the CPU but is enabled automatically together with the GPU backends. Note that setting this flag only builds support for single-precision computations - to actually run a model in single precision, you must specify
```c++
  struct ModelPars : public TempLat::DefaultModelPars {
    // ...
    using FloatType = float; // or double
    // ...
  };
```
in your model's `ModelPars` class.

For the broader picture of how these FFT backends fit into CosmoLattice's overall parallelization strategy — distributed (`MPI`) versus shared-memory and GPU parallelism — see Section [Parallelization](Parallelization.md). For the complete reference of compilation flags, see [CMake flags](../code/cmake.md), and for library installation, [Installation](../code/Installation.md).
