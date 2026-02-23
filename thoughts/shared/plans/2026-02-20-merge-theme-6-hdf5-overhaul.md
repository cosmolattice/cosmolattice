# Merge Theme 6: HDF5 Output System Overhaul

## Overview

Major rework of the I/O layer to support **buffered HDF5 output** for both averaged measurements and spectra. Replaces the previous per-timestep group creation in spectra with efficient time-series storage. Adds compact binary RNG state persistence. Introduces `HDF5TimeSeries<T>` as a new reusable building block, and a new `MeasurementsSaverHDF5<T>` backend for averaged measurements.

The end result: when the user sets `hdf5Averages=true` in the parameter file, all averaged measurements (field means, energies, scale factor, etc.) are written to a single shared HDF5 file with buffered flush, alongside the already-existing HDF5 spectra path which is also upgraded to buffered time-series.

## Dependencies

- **Theme 2 (Utilities)**: `stdatomictype.h` and `flattenstd.h` are needed by `hdf5dataset.h` and `hdf5timeseries.h`. Already merged (commit `903e6783`).
- **Theme 3 (SU2 4-comp)**: No direct dependency.
- **Theme 4 (SU2 exp map)**: No direct dependency.
- **Theme 5 (RK2N)**: Independent — HDF5 changes are orthogonal to the evolver.

## Files to Process

### Pass-Through Files

| File (master path) | GPU_impl target path | Notes |
|---------------------|---------------------|-------|
| `src/include/TempLat/lattice/IO/HDF5/helpers/hdf5timeseries.h` | Same | New file. Strip `#ifdef TEMPLATTEST` block. Wrap in `#ifdef HAVE_HDF5`. |

### Files to Adapt

| File (master path) | GPU_impl target path | What to port | Adaptation needed |
|---------------------|---------------------|-------------|-------------------|
| `helpers/hdf5type.h` | Same | `int`, `int*` specializations | Keep `HAVE_HDF5` macro |
| `helpers/hdf5dataset.h` | Same | `doIWrite` param, `getSizes()`, `reopen()`, `writeElement` fix, `std_atomic_type` | Keep C++20 `requires` constraints, `HAVE_HDF5`/`HAVE_MPI` |
| `helpers/hdf5file.h` | Same | MPI barriers, `H5Lexists` in `createOrOpenGroup` | Keep `HAVE_HDF5`/`HAVE_MPI`, keep `getHandle()` |
| `helpers/hdf5group.h` | Same | `createTimeSeries`, `reopenDataset`, `print_dataset_name` | Keep existing group-level methods (don't comment out), keep C++20 `requires` |
| `filesaverhdf5.h` | Same | `saveRNGStateBinary`, `createOrOpenGroup` | Keep GPU_impl's existing `save(double,...)`, `save(string,...)`, `savePerRank(...)`, device API. Use `HAVE_MPI` macro. |
| `fileloaderhdf5.h` | Same | `loadRNGStateBinary` | Keep GPU_impl's existing `load(double&,...)`, `load(string&,...)`, `loadPerRank(...)`, device API. Use `HAVE_MPI`. |
| `measurementsIO/filesmanager.h` | Same | `ParameterParser&` param, HDF5 file creation, `getUseHDF5()` returning real value, `getHDF5Fn()`, `getHDF5SpectraFn()`, `getCurredName()`, `getFlushFreq()`, `getNMeas()`, `getNInfreqMeas()` | Keep `template<size_t NDim>`, `device::memory::host_ptr<MemoryToolBox<NDim>>`. |
| `measurementsIO/measurementssaver.h` | Same | HDF5 dispatch, `save(bool lastMeas = false)` | Keep `template<size_t NDim>` constructors, `Field<NDim,T>` |
| `measurementsIO/spectrumsaver.h` | Same | Add `bool lastMeas` parameter to `save()` | Add overload with `lastMeas`, keep old signature as wrapper calling `save(false, t, spectra...)`. Backward-compatible. |
| `measurementsIO/std/measurementssaverstd.h` | Same | Use `getCurredName` from `FilesManager` | Keep `template<size_t NDim>` |
| `measurementsIO/hdf5/measurementssaverhdf5.h` | Same | New file — HDF5 backend for averaged measurements | Adapt to `FilesManager<NDim>&`, `Field<NDim,T>&`. No `#ifdef HAVE_HDF5` guard (matches master pattern). |
| `measurementsIO/hdf5/spectrumsaverhdf5.h` | Same | Rewrite from per-timestep groups to buffered time-series | Keep `HAVE_HDF5` guard, keep `template<size_t NDim>` constructors. Accept loss of verbosity 2 (throws). |
| `runparameters.h` | Same | Add `hdf5Averages`, `hdf5FlushFreq`, `getFlushFreq()` | Theme 6 portion only. Other themes add further changes. |
| `measurer.h` | Same | Add `ParameterParser&` to constructor, forward to `FilesManager` with HDF5 config | Minimal change — only constructor signature and `filesManager` init. Theme 7 adds `AbstractMeasurer`, `lastMeas`, `topologicalChargesMeasurer`. |
| `cosmolattice.cpp` | Same | Pass `parser` to `Measurer` constructor | One-line change. |

### Files to Skip

| File | Reason |
|------|--------|
| `helpers/hdf5timeseries_test.h` | Trivial `tdd.verify(true)` stub — GPU_impl uses .cpp test files |
| `measurementsIO/hdf5/measurementssaverhdf5_test.h` | Trivial `tdd.verify(true)` stub |

### Test File Migration

| Master `_test.h` | Action | GPU_impl `.cpp` target | Notes |
|-------------------|--------|----------------------|-------|
| `filesaverhdf5_test.h` | move logic to .cpp | `src/tests/TempLat/lattice/IO/HDF5/filesaverhdf5.cpp` | 3 real RNG binary round-trip tests. Needs adaptation to GPU_impl test pattern. |
| `hdf5tester_test.h` | move logic to .cpp | `src/tests/TempLat/lattice/IO/HDF5/hdf5tester.cpp` | Time series integration test. Fix `flush()` call (needs `amIRoot` arg). |

### Headers Needing TEMPLATTEST Cleanup

> All ported headers must have their `#ifdef TEMPLATTEST` / `#include "_test.h"` block removed.

| Header | Lines to strip |
|--------|---------------|
| `hdf5timeseries.h` | Bottom TEMPLATTEST guard (if present in master) |
| `measurementssaverhdf5.h` | Bottom unconditional `_test.h` include (master includes `measurementssaverhdf5_test.h` unconditionally — do NOT include it in GPU_impl) |

## Implementation Steps

### Step 1: HDF5 type additions — `hdf5type.h`

Add two specializations after the existing `float*` specialization:

```cpp
template <> struct HDF5Type<int>
{
  hid_t type = H5T_NATIVE_INT;
  void close() {}
};

template <> struct HDF5Type<int *>
{
  hid_t type = H5T_NATIVE_INT;
  void close() {}
};
```

### Step 2: HDF5 dataset enhancements — `hdf5dataset.h`

Add the following to the existing `HDF5Dataset` class:

1. **Add `#include "TempLat/util/stdatomictype.h"`** at the top (after existing includes).

2. **Add `bool doIWrite = true` parameter to `writeSlices`**: When `doIWrite == false`, select none in memspace (`H5Sselect_none(memspace_id)`) so the rank participates in collective I/O without writing.

3. **Fix `writeElement`**: Change `H5Dwrite(..., data, ...)` to `H5Dwrite(..., &data, ...)` (pointer to data).

4. **Add `getSizes()` method**: Queries the dataset's dataspace dimensions via `H5Dget_space` + `H5Sget_simple_extent_dims` and returns them as `vector<hsize_t>`.

5. **Add `reopen(hid_t parentId)` method**: Closes current dataset if open, then `mId = H5Dopen2(parentId, name, H5P_DEFAULT)`. Requires storing the dataset name — add a `std::string mName` member, set in `HDF5Dataset` constructor.

6. **Use `std_atomic_type<T>::type`** for HDF5 type resolution in write/read methods where appropriate (particularly in `writeSlices` and `readSlices`).

### Step 3: HDF5 file MPI barriers and H5Lexists — `hdf5file.h`

1. **Add MPI barriers** around file `create()` and `open()`:
   ```cpp
   #ifdef HAVE_MPI
   MPI_Barrier(MPI_COMM_WORLD);
   #endif
   // ... existing create/open code ...
   #ifdef HAVE_MPI
   MPI_Barrier(MPI_COMM_WORLD);
   #endif
   ```

2. **Replace `H5Gget_objinfo` with `H5Lexists`** in `createOrOpenGroup`:
   ```cpp
   if (H5Lexists(file_id, gn.c_str(), H5P_DEFAULT) > 0)
     return HDF5Group(H5Gopen(file_id, gn.c_str(), H5P_DEFAULT));
   else
     return createGroup(gn);
   ```

### Step 4: HDF5 group extensions — `hdf5group.h`

Add to the existing `HDF5Group` class:

1. **`createTimeSeries<T>(name, dims, chunks)`**: Creates an extendible dataset with `H5S_UNLIMITED` max dimensions and chunked storage. Returns `HDF5TimeSeries<T>` (new class from step 5). Uses C++20 `requires` constraint on dims/chunks parameter types.

2. **`reopenDataset(name)`**: Returns an `HDF5Dataset` opened via `H5Dopen2(mId, name.c_str(), H5P_DEFAULT)`.

3. **`print_dataset_name(hid_t id)`**: Free function (debug utility) that iterates group contents using `H5Literate` and prints dataset names. Place outside the class in the namespace.

4. **Replace `H5Gget_objinfo` with `H5Lexists`** in `createOrOpenGroup` (same fix as hdf5file.h).

5. **Add include** for `hdf5timeseries.h` (forward dependency).

### Step 5: New file — `hdf5timeseries.h`

Copy from master and adapt:
- Replace `#ifdef HDF5` with `#ifdef HAVE_HDF5`
- Remove `#ifdef TEMPLATTEST` block and `_test.h` include at the bottom
- Remove any `#include "TempLat/util/tdd/tdd.h"` that existed only for the test guard
- The class inherits from `HDF5Dataset` and adds:
  - `push(T)`: appends scalar to internal cache
  - `push(vector<U>)`: flattens and appends via `flatten()`
  - `flush(bool amIRoot)`: writes cache to dataset via `writeSlices`, clears cache, updates offset
  - `extend(hsize_t n)`: grows first dimension of the dataset
  - Internal `cache` (vector), `count`, `curOffset`
- Add `#include "TempLat/util/flattenstd.h"` for the flatten utility
- Add `#include "TempLat/util/stdatomictype.h"` for atomic type resolution

### Step 6: RNG binary persistence — `filesaverhdf5.h` and `fileloaderhdf5.h`

**In `filesaverhdf5.h`**, add after the existing `savePerRank(double,...)` method:

1. **`saveRNGStateBinary(const std::string& textState, const std::string& name, int mpiRank, int nRanks)`**: Parses text RNG state into `vector<uint64_t>`, creates 2D dataset `[nRanks, stateSize]` with `H5T_NATIVE_UINT64`, writes per-rank row via hyperslab with `H5FD_MPIO_INDEPENDENT`. Handles both `RandomUniform` (313 values) and `RandomGaussian` (316 values with cached double via `memcpy`).

2. **`createOrOpenGroup(const std::string& name)`**: Delegates to `mFile.createOrOpenGroup(name)`. Returns `HDF5Group`.

Add includes: `<cstring>` (for `memcpy`), `<sstream>`, `<vector>`.

**In `fileloaderhdf5.h`**, add after the existing `loadPerRank(double&,...)` method:

1. **`loadRNGStateBinary(std::string& textState, const std::string& name, int mpiRank)`**: Opens 2D `uint64_t` dataset, selects per-rank row, reconstructs text string. Handles 313-value (RandomUniform), 316-value (RandomGaussian with double reconstruction via `memcpy`), and unknown sizes.

Add includes: `<cstring>`, `<iomanip>`, `<sstream>`, `<vector>`.

### Step 7: RunParameters HDF5 additions — `runparameters.h`

Add to the constructor initialization list:
```cpp
hdf5Averages(par.get<bool>("hdf5Averages", false)),
hdf5FlushFreq(par.get<ptrdiff_t>("hdf5FlushFreq", 10)),
```

Add member declarations:
```cpp
const bool hdf5Averages;
const ptrdiff_t hdf5FlushFreq;
```

Add accessor:
```cpp
ptrdiff_t getFlushFreq() const { return hdf5FlushFreq; }
```

**Naming note from triage**: Use lowercase `hdf5FlushFreq` for the member name to match the existing `hdf5Spectra` convention. The parser string can be `"hdf5FlushFreq"` to be clear.

### Step 8: FilesManager expansion — `filesmanager.h`

Major rework of the 66-line GPU_impl file. Add:

1. **Constructor signature change**: Add `ParameterParser& parser` as the first parameter. Add `ptrdiff_t pFlushFreq = 1`, `ptrdiff_t pNMeas = 0`, `ptrdiff_t pNMeasInfreq = 0` parameters.

2. **Constructor body**: If `mUseHDF5`, create `FileSaverHDF5`, call `fs.create(getHDF5Fn(), Exclusive)`, `fs.save_attr(parser)`, `fs.close()`. Same for `mUseHDF5Spectra` with spectra file.

3. **New methods**:
   - `getHDF5Fn()`: returns `workingDir + tag + "average.h5"`
   - `getHDF5SpectraFn()`: returns `workingDir + tag + "spectra.h5"`
   - `getCurredName(const Field<NDim, T>& fld, bool withDir, std::string nametag = "average")`: extracts field name (strip after `(`), builds file path
   - `getFlushFreq()`: returns `flushFreq`
   - `getNMeas()`: returns `nMeas`
   - `getNInfreqMeas()`: returns `nMeasInfreq`

4. **Fix `getUseHDF5()`**: Replace `return false;` with `return mUseHDF5;`.

5. **New includes**: `hdf5group.h`, `hdf5file.h`, `field.h`, `filesaverhdf5.h` (all under `#ifdef HAVE_HDF5` or unconditional as appropriate).

6. **New private members**: `bool mUseHDF5`, `ptrdiff_t flushFreq`, `ptrdiff_t nMeas`, `ptrdiff_t nMeasInfreq`.

### Step 9: MeasurementsSaverStd cleanup — `measurementssaverstd.h`

In the second constructor (the one taking `Field<NDim, T>&`), replace the 3-line manual name extraction with a call to `fm.getCurredName(fld, true)`.

### Step 10: New file — `measurementssaverhdf5.h`

Create `src/include/CosmoInterface/measurements/measurementsIO/hdf5/measurementssaverhdf5.h`.

Adapt from master:
- **No `#ifdef HAVE_HDF5` guard** (matches master pattern — the file is included conditionally by `measurementssaver.h`)
- Template constructors take `FilesManager<NDim>&` and `Field<NDim, T>&` (GPU_impl convention)
- Opens shared HDF5 file, creates group, creates `HDF5TimeSeries<T>` per column header with chunk size 4096
- Pre-extends to `fm.getNMeas()` slots
- `addAverage(T)`: pushes to current series (round-robin)
- `save(bool lastMeas)`: flushes if `lastMeas` or `flushCount % flushFreq == 0`; opens file, iterates averages, reopen dataset, flush, close
- Do NOT include `measurementssaverhdf5_test.h` at the bottom

### Step 11: MeasurementsSaver HDF5 dispatch — `measurementssaver.h`

1. **Add include**: `#include "CosmoInterface/measurements/measurementsIO/hdf5/measurementssaverhdf5.h"`
2. **Add `useHDF5` member** and `shared_ptr<MeasurementsSaverHDF5<T>> ms5` member
3. **Both constructors**: Branch on `fm.getUseHDF5()`:
   - `false` → create `MeasurementsSaverStd<T>` (existing)
   - `true` → create `MeasurementsSaverHDF5<T>`
4. **`addAverage()`**: Dispatch to `ms` or `ms5` based on `useHDF5`
5. **`save(bool lastMeas = false)`**: Dispatch; std path calls `ms->save()`, HDF5 path calls `ms5->save(lastMeas)`. The default `= false` preserves backward compatibility with existing callers.

### Step 12: SpectrumSaverHDF5 rewrite — `spectrumsaverhdf5.h`

Complete rewrite of the existing GPU_impl file:

1. **Add includes**: `hdf5timeseries.h`
2. **Add `NotImplementedInHDF5` exception**
3. **Constructor changes**: Store `flushFreq`, `nMeas` from `FilesManager`. Set `uninitialized = true` for deferred initialization. Don't create the file upfront (it was created by `FilesManager`).
4. **`save(bool lastMeas, vector<shared_ptr<RadialProjectionResult<T>>> arr, T t)`**: New signature with `lastMeas`. On first call, initializes time series datasets. Pushes spectra data. Flushes on `lastMeas` or `flushCount % flushFreq == 0`.
5. **`initialize(arr)`**: Creates `HDF5TimeSeries` for multiplicity, bin averages, bin centers, and per-spectrum values. Throws `NotImplementedInHDF5` for verbosity 2.
6. **`push_spectra(arr, t)`**: Pushes data into time series caches.
7. **`flush_spectra(group)`**: Reopens datasets, flushes each time series, closes.
8. **Private members**: Replace per-timestep state with time-series state (`shared_ptr<HDF5TimeSeries<T>>` for each dataset type, flush counters).

### Step 13: SpectrumSaver lastMeas parameter — `spectrumsaver.h`

Add a new overload with `lastMeas` and keep the old signature as a backward-compatible wrapper:

```cpp
template<template<typename> class... Spectra>
void save(bool lastMeas, T& t, Spectra<T>... spectra) {
  // existing body, but forward lastMeas to HDF5 backend
}

template<template<typename> class... Spectra>
void save(T& t, Spectra<T>... spectra) {
  save(false, t, spectra...);
}
```

Also update `SpectrumSaverHDF5::save()` call to pass `lastMeas`.

### Step 14: Measurer minimal update — `measurer.h`

1. **Constructor signature**: Add `ParameterParser &parser` parameter:
   ```cpp
   Measurer(Model &model, const RunParameters<T> &par, ParameterParser &parser)
   ```

2. **`filesManager` initialization**: Update from:
   ```cpp
   filesManager(SimulationManager<Model::NDim>::base_filename(par, model), model.getToolBox(), false,
                par.hdf5Spectra, par.printHeaders)
   ```
   to:
   ```cpp
   filesManager(parser, SimulationManager<Model::NDim>::base_filename(par, model), model.getToolBox(),
                par.hdf5Averages, par.hdf5Spectra, par.printHeaders, "",
                par.getFlushFreq(),
                static_cast<ptrdiff_t>(round((par.tMax - par.t0) / par.tOutFreq)),
                static_cast<ptrdiff_t>(round((par.tMax - par.t0) / par.tOutInfreq)))
   ```

3. **Add include**: `#include "TempLat/parameters/parameterparser.h"` if not already present.

### Step 15: cosmolattice.cpp — pass parser to Measurer

Change line 119 from:
```cpp
Measurer<ModelType, double> measurer(model, runParams);
```
to:
```cpp
Measurer<ModelType, double> measurer(model, runParams, parser);
```

### Step 16: Test files

**Create `src/tests/TempLat/lattice/IO/HDF5/filesaverhdf5.cpp`** (if it doesn't already exist, or update if it does):
- Port the 3 RNG binary round-trip tests from master's `filesaverhdf5_test.h`
- Follow GPU_impl test pattern (inline test body in .cpp, no `_test.h`)

**Update `src/tests/TempLat/lattice/IO/HDF5/hdf5tester.cpp`** (if it exists):
- Port the time series integration test from master's `hdf5tester_test.h`
- Fix `dataset.flush()` to pass `amIRoot` argument (master test file omits it but `flush(bool)` requires it)

### Step 17: Update merge status

Update `thoughts/shared/merge_triage/status.md`: set Theme 6 to **done**, link this plan file, add the commit hash.

## Verification

### Automated
- [x] Build compiles: `cd cosmolattice_GPU_impl/debug_build && cmake -DSerial=ON -DMPI=ON -DHDF5=ON -DTESTING=ON -DMYHDF5_PATH=.../MyHDF5Parallel/ -DMYFFTW3_PATH=.../MyLibs/ -DMODEL=lphi4SU2U1 ../`
- [x] `make cosmolattice` succeeds
- [x] `make test-filesaverhdf5 && ctest -R filesaverhdf5` passes (RNG round-trip tests)
- [x] `make test-hdf5tester && ctest -R hdf5tester` passes (time series test)
- [x] `ctest -R "hdf5|filesaver|fileloader"` runs all HDF5 tests without regression (10/10 pass)

### Manual
- [ ] Run a short simulation with `hdf5Averages=true` in parameter file and verify `.h5` output is produced
- [ ] Verify `hdf5Spectra=true` still works with the rewritten time-series spectra saver

## API Impact

1. **`Measurer` constructor** now requires `ParameterParser&` as third argument. All instantiations of `Measurer` need updating (only one site: `cosmolattice.cpp`).
2. **`FilesManager` constructor** now requires `ParameterParser&` as first argument plus 3 new trailing parameters. Internal to Measurer — no external API impact.
3. **`SpectrumSaver::save()`** gains a new overload with `bool lastMeas` leading parameter. Old signature preserved for backward compatibility.
4. **`MeasurementsSaver::save()`** gains `bool lastMeas = false` parameter. Backward-compatible via default.
5. **`RunParameters`** gains `hdf5Averages` and `hdf5FlushFreq` members. No breaking change.
6. **`SpectrumSaverHDF5`** completely rewritten — internal to SpectrumSaver, no external API impact.
7. **`HDF5Dataset::writeSlices`** gains optional `bool doIWrite = true` parameter. Backward-compatible.

## Post-Implementation Checklist
- [x] Update `cosmolattice_GPU_impl/thoughts/shared/merge_triage/status.md`: set this theme's status to **done**, link the plan file (commit hash to be added after commit)
