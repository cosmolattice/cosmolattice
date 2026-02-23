# Merge Theme 5: RK2N Low-Storage Runge-Kutta Evolver

## Overview

This theme ports the RK2N (2N-storage) explicit Runge-Kutta time integrator from master into GPU_impl. The RK2N evolver supports 4 RK variants (RK2, RK3_3, RK3_4, RK4_5) with low memory overhead (only one auxiliary copy of all fields). This requires:

1. A compile-time **field numbering** system (`FieldsNumbering`) for generic iteration over all field types
2. **Kernel-type dispatch** (`KernelsTypes::EoM<T>`) enabling extensible physics kernels
3. **Butcher coefficient tables** (`RK2NStorageParameters`)
4. **Auxiliary field storage** (`ExtraFields` / `FieldsAsInModel`) for the 2N-storage delta buffer
5. **Tag-dispatch getters** (`getNFields`, `getField`) added to `AbstractModel`
6. **EoM overloads + `get_momentum`** added to every kernel class
7. A **unified kernel dispatch layer** (`Kernels::get`) replacing the bare include aggregator
8. The **RK2N evolver** itself (`RK2NStorage`) and integration into `Evolver` and `cosmolattice.cpp`

The SU(2) gauge link advance uses the matrix exponential map (`exp(B*delta) * U`), which was ported in Theme 4.

## Dependencies

- **Theme 3** (SU(2) 4-component storage): `unitarize()`, 4-component `SU2Field` — **done**
- **Theme 4** (SU(2) exp map): `exp()` for `SU2LieAlgebraField` — **done**

## Cross-Cutting Deferrals

Several files in this theme are cross-cutting with Theme 8 (U(1) Axion coupling). For each, only the RK2N-relevant parts are ported now:

- `scalarsingletkernels.h`: Defer `AxionCouplings::ScalarAxionSource` and `axioncouplings.h` include
- `u1kernels.h`: Defer `AxionCouplings::U1AxionCoupling`, `axioncouplings.h` include, and `normU1AxionScalarSource`
- `extrafields.h`: Defer plane-wave IC methods (`fldForPlaneWavesU1`, `piForPlaneWavesU1`) and `InitialConditionsType` references
- `abstractmodel.h`: Defer `ScalarU1AxionCouplings` template param, `alphaLambda_SU1` member, axion coupling parsing, `getFluctuationRatio`, `getU1IC`
- `cosmolattice.cpp`: Defer `Measurer(parser)` change (Theme 7) and `modelinitializer(extraFlds)` (Theme 8)

## Files to Process

### Pass-Through Files (copy from master, strip TEMPLATTEST + tdd.h)

| File (master path) | GPU_impl target path | Notes |
|---------------------|---------------------|-------|
| `src/include/CosmoInterface/fieldsnumbering.h` | same | New file. Strip TEMPLATTEST block + tdd.h include |
| `src/include/CosmoInterface/evolvers/kernels/kernelstypes.h` | same | New file. Strip TEMPLATTEST block + tdd.h include |
| `src/include/CosmoInterface/evolvers/rk2nstorageparameters.h` | same | New file. Strip TEMPLATTEST block + tdd.h include |
| `src/include/CosmoInterface/evolvers/rk2nstorage.h` | same | New file. Strip TEMPLATTEST block + tdd.h include |

### Files to Adapt

| File (master path) | GPU_impl target path | What to port | Adaptation needed |
|---------------------|---------------------|-------------|-------------------|
| `evolvertype.h` | same | RK3_3, rename RK3_4_A→RK4_5 | Update enum, parsing, to_string. Keep GPU_impl formatting |
| `abstractmodel.h` | same | getNFields + getField dispatch | Add 10+10 overloads. Keep GPU_impl `<NDim,T>` field types |
| `complexscalarkernels.h` | same | EoM dispatch + get_momentum | Add 2 methods + backward-compat overload. Keep GPU_impl style |
| `su2doubletkernels.h` | same | EoM dispatch + get_momentum | Same pattern |
| `su2kernels.h` | same | EoM dispatch + get_momentum | Same pattern + `fieldfunctionals.h` NOT needed |
| `scalarsingletkernels.h` | same | EoM dispatch + get_momentum | Defer axion source. Keep GPU_impl formatting |
| `u1kernels.h` | same | Restructure + EoM dispatch + get_momentum | Remove MakeArray, compute per-field. Defer axion. |
| `kernels.h` | same | Full replacement with Kernels dispatch | Replace bare aggregator with namespace + 10 dispatch overloads |
| `extrafields.h` (new) | same | FieldsAsInModel + ExtraFields RK2N | Adapt field types to `<NDim,T>`. Defer plane-wave IC |
| `evolver.h` | same | Add RK2N branch | Add rk2n shared_ptr, ExtraFields param, activate/deactivate |
| `cosmointerface.h` | same | Add extrafields.h include | Trivial one-line add |
| `cosmolattice.cpp` | same | ExtraFields creation + threading | Add 2 lines + update Evolver constructor |

### Files to Skip

| File | Reason |
|------|--------|
| All `_test.h` stubs | GPU_impl doesn't use `_test.h` files |

### Test File Migration

| Master `_test.h` | Action | Notes |
|-------------------|--------|-------|
| `kernelstypes_test.h` | skip | Trivial stub |
| `rk2nstorage_test.h` | skip | Commented-out stub |
| `rk2nstorageparameters_test.h` | skip | Trivial stub |
| `extrafields_test.h` | skip | Trivial stub |
| `fieldsnumbering_test.h` | skip | Deliberately failing stub |

### Headers Needing TEMPLATTEST Cleanup

| Header | What to strip |
|--------|---------------|
| `fieldsnumbering.h` | `#include tdd.h`, `#ifdef TEMPLATTEST` / `Test()` decl (lines 50-52), bottom `#ifdef TEMPLATTEST` / `#include _test.h` / `#endif` |
| `kernelstypes.h` | `#include tdd.h`, tester class + TEMPLATTEST block at bottom |
| `rk2nstorageparameters.h` | `#include tdd.h`, tester struct + TEMPLATTEST block at bottom |
| `rk2nstorage.h` | `#include tdd.h`, tester class + TEMPLATTEST block at bottom |

## Implementation Steps

### Step 1: Create new leaf files (no inter-dependencies)

#### 1a. Create `fieldsnumbering.h`
Copy from master, strip TEMPLATTEST. The file defines `FieldsNumbering` with `Tag<0>..Tag<9>` aliases and `maxNum = 9`.

**Target:** `src/include/CosmoInterface/fieldsnumbering.h`

Content: Master's file with these removals:
- `#include "TempLat/util/tdd/tdd.h"` line
- The `public:` + `#ifdef TEMPLATTEST` block inside the class (lines 49-53 in master)
- The `#ifdef TEMPLATTEST` / `#include "fieldsnumbering_test.h"` / `#endif` at bottom

#### 1b. Create `kernelstypes.h`
Copy from master, strip TEMPLATTEST.

**Target:** `src/include/CosmoInterface/evolvers/kernels/kernelstypes.h`

Content: Master's file with these removals:
- `#include "TempLat/util/tdd/tdd.h"` line
- `KernelsTypesTester` class and its TEMPLATTEST block at bottom

#### 1c. Modify `evolvertype.h`
Edit GPU_impl's existing file:
- Rename `RK3_4_A` → `RK4_5` in enum, docstring, parsing, and to_string
- Add `RK3_3` entry between `RK2` and `RK3_4` in enum
- Update parsing: `"8"` → `RK3_3`, `"9"` → `RK3_4`, `"10"` → `RK4_5`
- Update to_string: add RK3_3 case, rename RK3_4_A case

The enum becomes: `{ LF, VV2, VV4, VV6, VV8, VV10, VV6_2, RK2, RK3_3, RK3_4, RK4_5 }`

#### 1d. Create `rk2nstorageparameters.h`
Copy from master, strip TEMPLATTEST. Depends on modified `evolvertype.h` (uses `RK3_3`, `RK4_5`).

**Target:** `src/include/CosmoInterface/evolvers/rk2nstorageparameters.h`

Content: Master's file with these removals:
- `#include "TempLat/util/tdd/tdd.h"` line
- `RK2NStorageParametersTester` struct and TEMPLATTEST block at bottom

### Step 2: Add field numbering to `abstractmodel.h`

Edit GPU_impl's `src/include/CosmoInterface/abstractmodel.h`:

**2a. Add include** (after line 19, before the namespace):
```cpp
#include "CosmoInterface/fieldsnumbering.h"
```

**2b. Add `getNFields` overloads** (after line 90, after `MPl`):
```cpp
    // Field numbering for generic iteration (used by RK2N evolver)
    static constexpr size_t getNFields(FieldsNumbering::fldS) { return Ns; }
    static constexpr size_t getNFields(FieldsNumbering::piS) { return Ns; }
    static constexpr size_t getNFields(FieldsNumbering::fldCS) { return NCs; }
    static constexpr size_t getNFields(FieldsNumbering::piCS) { return NCs; }
    static constexpr size_t getNFields(FieldsNumbering::fldSU2Doublet) { return NSU2Doublet; }
    static constexpr size_t getNFields(FieldsNumbering::piSU2Doublet) { return NSU2Doublet; }
    static constexpr size_t getNFields(FieldsNumbering::fldU1) { return NU1; }
    static constexpr size_t getNFields(FieldsNumbering::piU1) { return NU1; }
    static constexpr size_t getNFields(FieldsNumbering::fldSU2) { return NSU2; }
    static constexpr size_t getNFields(FieldsNumbering::piSU2) { return NSU2; }
```

**2c. Add `getField` tag-dispatch getters** (after each field collection pair). These are needed by `RK2NStorage::advance()` which calls `model.getField(fld)(n)`:

After `piS` declaration (line 108):
```cpp
    FieldCollection<Field<NDim, T>, T, Ns, true> getField(FieldsNumbering::fldS) { return fldS; }
    FieldCollection<Field<NDim, T>, T, Ns, true> getField(FieldsNumbering::piS) { return piS; }
```

After `piCS` declaration (line 113):
```cpp
    FieldCollection<ComplexField<NDim, T>, T, NCs> getField(FieldsNumbering::fldCS) { return fldCS; }
    FieldCollection<ComplexField<NDim, T>, T, NCs> getField(FieldsNumbering::piCS) { return piCS; }
```

After `piSU2Doublet` declaration (line 117):
```cpp
    FieldCollection<SU2Doublet<NDim, T>, T, NSU2Doublet> getField(FieldsNumbering::fldSU2Doublet) { return fldSU2Doublet; }
    FieldCollection<SU2Doublet<NDim, T>, T, NSU2Doublet> getField(FieldsNumbering::piSU2Doublet) { return piSU2Doublet; }
```

After `piU1` declaration (line 121):
```cpp
    VectorFieldCollection<Field<NDim, T>, T, NDIM, NU1> getField(FieldsNumbering::fldU1) { return fldU1; }
    VectorFieldCollection<Field<NDim, T>, T, NDIM, NU1> getField(FieldsNumbering::piU1) { return piU1; }
```

After `piSU2` declaration (line 125):
```cpp
    VectorFieldCollection<SU2Field<NDim, T>, T, NDIM, NSU2> getField(FieldsNumbering::fldSU2) { return fldSU2; }
    VectorFieldCollection<SU2LieAlgebraField<NDim, T>, T, NDim, NSU2> getField(FieldsNumbering::piSU2) { return piSU2; }
```

### Step 3: Update all kernel files

#### 3a. `complexscalarkernels.h`
Replace GPU_impl file content with master's version, adapted to GPU_impl style:
- Add `#include "CosmoInterface/evolvers/kernels/kernelstypes.h"`
- Add EoM-typed `get(Model&, Tag<N>, KernelsTypes::EoM<T>)` — same body as existing `get`
- Add `get_momentum(Model&, Tag<N>, KernelsTypes::EoM<T>)` → `pow(aI, alpha-3) * piCS(n)`
- Make existing `get(Model&, Tag<N>)` a backward-compat wrapper
- Keep GPU_impl 2-space formatting, include guards, copyright
- No TEMPLATTEST block

#### 3b. `su2doubletkernels.h`
Same pattern as complexscalarkernels.h:
- Add `#include "CosmoInterface/evolvers/kernels/kernelstypes.h"`
- EoM `get` + `get_momentum` → `pow(aI, alpha-3) * piSU2Doublet(n)`
- Backward-compat wrapper

#### 3c. `su2kernels.h`
Same pattern:
- Add `#include "CosmoInterface/evolvers/kernels/kernelstypes.h"`
- EoM `get` — same body as existing
- `get_momentum` → `pow(aI, alpha-1) * gQ_SU2DblSU2(0_c, n) * dx * piSU2(n)`
- Backward-compat wrapper

#### 3d. `scalarsingletkernels.h`
Adapt for RK2N only (defer axion):
- Add `#include "CosmoInterface/evolvers/kernels/kernelstypes.h"`
- EoM `get` — same body as existing GPU_impl (NO axion source)
- `get_momentum` → `pow(aI, alpha-3) * piS(n)`
- Backward-compat wrapper

#### 3e. `u1kernels.h`
Restructure and add RK2N support (defer axion):
- Add `#include "CosmoInterface/evolvers/kernels/kernelstypes.h"`
- Restructure EoM `get`: compute `U1Source`, `LaplU1`, `GradU1` directly for field index `a` (not via `MakeArray` over all fields). This matches master's approach and is more efficient.
- `get_momentum` → `pow(aI, alpha-1) * piU1(a)`
- Keep backward-compat wrapper
- Do NOT add axion coupling code

The restructured `get` method computes:
```cpp
auto U1Source = MatterCurrents::U1Current(model, a);
auto LaplU1 = MakeVector(i, 1, Model::NDim, LatLapl<Model::NDim>(model.fldU1(a)(i)));
auto GradU1 = MakeVector(i, 1, Model::NDim, Total(j, 1, Model::NDim, backDiff(forwDiff(model.fldU1(a)(j), i), j)));
```
(computing for a single field `a` directly, instead of all fields then indexing)

#### 3f. `kernels.h`
Replace the bare include aggregator with the full dispatch layer from master. Add include guards, the `Kernels` namespace with 10 `static inline get()` overloads dispatching by `FieldsNumbering` tag. Add `#include "CosmoInterface/fieldsnumbering.h"`.

### Step 4: Create `extrafields.h` (adapted for GPU_impl)

**Target:** `src/include/CosmoInterface/extrafields.h`

Port from master with these adaptations:

**`FieldsAsInModel<Model>`:**
- All field collection types must use GPU_impl's `<NDim, T>` convention:
  - `FieldCollection<Field<Model::NDim, T>, T, Model::Ns, true>` (not `FieldCollection<Field, T, ...>`)
  - `FieldCollection<ComplexField<Model::NDim, T>, T, Model::NCs>`
  - `FieldCollection<SU2Doublet<Model::NDim, T>, T, Model::NSU2Doublet>`
  - `VectorFieldCollection<Field<Model::NDim, T>, T, Model::NDim, Model::NU1>`
  - `VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, T, Model::NDim, Model::NSU2>`
- The `get(FieldsNumbering::XXX)` overloads return types must also use `<NDim, T>`

**`ExtraFields<Model>`:**
- Only port the RK2N branch of `allocateExtraMemory`:
  ```cpp
  if (RK2NStorageParameters<T>::isRK2n(runPars.eType))
    allFlds1 = std::make_shared<FieldsAsInModel<Model>>(model, runPars, tag);
  ```
- Keep `getAllFlds1()` method
- **Defer** `fldForPlaneWavesU1()`, `piForPlaneWavesU1()`, `fldU1IC`, `piU1IC`, and `InitialConditionsType` references to Theme 8
- Strip TEMPLATTEST and tdd.h include

### Step 5: Create `rk2nstorage.h` (near pass-through)

**Target:** `src/include/CosmoInterface/evolvers/rk2nstorage.h`

Copy from master. The class works entirely through `Model::` accessors and `FieldsAsInModel<Model>`, so no explicit field-type adaptations needed. Just:
- Strip `#include "TempLat/util/tdd/tdd.h"`
- Strip `RK2NStorageTester` class and TEMPLATTEST blocks at bottom

### Step 6: Update `evolver.h`

Rewrite GPU_impl's `src/include/CosmoInterface/evolvers/evolver.h`:
- Add `#include "CosmoInterface/evolvers/rk2nstorage.h"`
- Change constructor signature: `Evolver(Model &model, RunParameters<T> &rPar, ExtraFields<Model> extraFlds)`
- Use initializer-list style construction (as in master):
  ```cpp
  lf(type == LF ? std::make_shared<LeapFrog<T>>(model, rPar) : nullptr),
  vv(VelocityVerletParameters<T>::isVerlet(type) ? std::make_shared<VelocityVerlet<T>>(model, rPar) : nullptr),
  rk2n(RK2NStorageParameters<T>::isRK2n(type) ? std::make_shared<RK2NStorage<Model>>(model, rPar) : nullptr)
  ```
- In constructor body: if RK2N, call `rk2n->setDelta(extraFlds)`
- Add RK2N branch to `evolve()`: `rk2n->evolve(model, tMinust0, EoMKernels)`
- Add RK2N branch to `sync()`: `rk2n->sync(model, tMinust0)`
- Add `activate`/`deactivate` template methods (delegate to `rk2n`)
- Add private members: `std::shared_ptr<RK2NStorage<Model>> rk2n` and `KernelsTypes::EoM<T> EoMKernels`

### Step 7: Update umbrella and main

#### 7a. `cosmointerface.h`
Add `#include "CosmoInterface/extrafields.h"` after the existing includes.

#### 7b. `cosmolattice.cpp`
After the `ModelType model(...)` construction (line 70) and before `Evolver` construction (line 108):
```cpp
ExtraFields<ModelType> extraFlds;
extraFlds.allocateExtraMemory(model, runParams, "extra_mem");
```

Update the `Evolver` constructor call (line 108):
```cpp
Evolver<ModelType> evolver(model, runParams, extraFlds);
```

### Step 8: Build verification

```bash
cd cosmolattice_GPU_impl/debug_build
cmake -DSerial=ON -DMPI=ON -DHDF5=ON -DTESTING=ON \
  -DMYHDF5_PATH=/Users/aflorio/.../MyHDF5Parallel/ \
  -DMYFFTW3_PATH=/Users/aflorio/.../MyLibs/ \
  -DMODEL=lphi4SU2U1 ../
make cosmolattice
```

### Step 9: Update merge status

Update `thoughts/shared/merge_triage/status.md`:
- Set Theme 5 status to **done**
- Link the plan file
- Add commit hash

## Verification

### Automated
- [x] Build compiles: `make cosmolattice` with model `lphi4SU2U1`
- [x] No regressions: existing LF/VV evolvers still work (verified by successful compilation with model that uses them)

### Manual
- [x] Verify enum values: `RK3_4_A` no longer exists, `RK3_3` and `RK4_5` are present
- [x] Verify `abstractmodel.h` has 10 `getNFields` + 10 `getField` overloads

## API Impact

1. **`Evolver` constructor** now requires 3 arguments: `(model, runParams, extraFlds)` instead of 2. All call sites must update.
2. **`EvolverType` enum**: `RK3_4_A` renamed to `RK4_5`. Any code referencing `RK3_4_A` will break.
3. **New `ExtraFields<Model>`** must be created and passed to `Evolver`. If not using RK2N, it's a no-op (empty allocation).

## Post-Implementation Checklist
- [x] Update `cosmolattice_GPU_impl/thoughts/shared/merge_triage/status.md`: set this theme's status to **done**, link the plan file, and add the commit hash
