# Merge Theme 8: U(1) Axion Coupling

## Overview
This theme adds a new physics feature: coupling between a real scalar (axion) and U(1) gauge fields via an E·B-type interaction term. It introduces two new model files, new lattice operators (averaged electric/magnetic fields), axion coupling definitions, a topological charges measurer, initial condition type dispatch for U(1) plane-wave ICs, and axion source terms in the scalar and U(1) kernels. This is the largest and most cross-cutting theme in the merge.

## Dependencies
Themes 1-7 must be complete (all done).

## Files to Process

### Pass-Through Files (5)
| File (master path) | GPU_impl target path | Notes |
|---------------------|---------------------|-------|
| `src/include/TempLat/lattice/algebra/axionalgebra/electricfield2.h` | same | New file in new directory. Strip `_test.h` include and `tdd.h` include |
| `src/include/TempLat/lattice/algebra/axionalgebra/magneticfield4.h` | same | New file in new directory. Strip `_test.h` include and `tdd.h` include |
| `src/include/CosmoInterface/definitions/axioncouplings.h` | same | New file. Strip TEMPLATTEST block |
| `src/include/CosmoInterface/initializers/initialconditionstype.h` | same | New file. Strip TEMPLATTEST block |
| `src/models/parameter-files/AxionU1.in` | same | Plain text parameter file, copy as-is |

### Files to Adapt (14)
| File (master path) | GPU_impl target path | What to port | Adaptation needed |
|---------------------|---------------------|-------------|-------------------|
| `abstractmodel.h` | same | Axion infrastructure | Add `ScalarU1AxionCouplings` template param, `alphaLambda_SU1`, `InverseAxionLambda`, `getFluctuationRatio`, `getU1IC`, `initialconditionstype.h` include, parse axion charges |
| `fieldfunctionals.h` | same | `EBU1`, `get_SU2_electric`, rewrite `pi2SU2` | Add new methods, new includes |
| `gausslaws.h` | same | Axion Gauss law in `checkU1`, SU2 normalization fix | Add RHSaxion, fix `checkSU2` |
| `fluctuationsgenerator.h` | same | `planeWaves` method | Add ~45-line method with GPU_impl field types |
| `scalarsingletinitializer.h` | same | `getFluctuationRatio` lines | Add 2 lines |
| `u1initializer.h` | same | IC type dispatch, plane-wave init | Restructure for `RandomWithMatter`/`PlaneWavesZeroB` dispatch |
| `modelinitializer.h` | same | `ExtraFields` param, U1 condition | Change signature, pass `extraFields` |
| `topologicalchargesmeasurer.h` | same | New measurer | Strip TEMPLATTEST, adapt `FilesManager<NDim>` |
| `U1Axion.h` | `src/models/U1Axion.h` | New model | Adapt `MemoryToolBox` type, add `NDim` decl |
| `m2phi2_axionU1.h` | `src/models/m2phi2_axionU1.h` | New model | Adapt `MemoryToolBox` type, add `NDim` decl |
| `scalarsingletkernels.h` | same | Axion source term | Add include + 1 term in return |
| `u1kernels.h` | same | Axion coupling term | Add include + ~4 lines |
| `extrafields.h` | same | Plane-wave IC fields | Add `else if` branch, accessors, members |
| `cosmolattice.cpp` | same | Pass `extraFlds` to `initialize` | 1-arg change |

### Files to Adapt (deferred)
| `measurer.h` | same | TopologicalChargesMeasurer integration | Add include, member, constructor init, measure call |

### Files to Skip (4)
| File | Reason |
|------|--------|
| `src/include/CosmoInterface/definitions/mattercurrents.h` | Whitespace-only change |
| `TODOS.md` | Development notes, not needed |
| `src/include/CosmoInterface/runparameters.h` | Only commented-out `U1icType` |
| `src/include/CosmoInterface/measurements/u1measurer.h` | Only commented-out `checkU1axion` diagnostic |

### Auto-Skipped Test Files (5)
| File | Reason |
|------|--------|
| `axioncouplings_test.h` | Trivial stub |
| `initialconditionstype_test.h` | Trivial stub |
| `topologicalchargesmeasurer_test.h` | Trivial stub |
| `electricfield2_test.h` | Empty stub |
| `magneticfield4_test.h` | Empty stub |

### Headers Needing TEMPLATTEST Cleanup
| Header | What to strip |
|--------|---------------|
| `electricfield2.h` | `#include "TempLat/util/tdd/tdd.h"` + trailing `#include "...electricfield2_test.h"` |
| `magneticfield4.h` | `#include "TempLat/util/tdd/tdd.h"` + trailing `#include "...magneticfield4_test.h"` |
| `axioncouplings.h` | `#include "TempLat/util/tdd/tdd.h"` (doesn't exist but check), `public: #ifdef TEMPLATTEST ... #endif` block, trailing `#ifdef TEMPLATTEST ... #endif` |
| `initialconditionstype.h` | `#include "TempLat/util/tdd/tdd.h"`, `InitialConditionsTypeTester` class, trailing `#ifdef TEMPLATTEST ... #endif` |
| `topologicalchargesmeasurer.h` | `#include "TempLat/util/tdd/tdd.h"`, `TopologicalChargesMeasurerTester` class, trailing `#ifdef TEMPLATTEST ... #endif` |

## Implementation Steps

### Step 1: Create `axionalgebra/` directory and pass-through TempLat files

Create the new directory and add the two averaged-field operator files.

**`src/include/TempLat/lattice/algebra/axionalgebra/electricfield2.h`** — Copy from master, with these changes:
- Remove `#include "TempLat/util/tdd/tdd.h"` (line 4 of master)
- Remove trailing `#include "TempLat/lattice/algebra/axionalgebra/electricfield2_test.h"` (last 2 lines before `#endif`)

**`src/include/TempLat/lattice/algebra/axionalgebra/magneticfield4.h`** — Copy from master, with these changes:
- Remove `#include "TempLat/util/tdd/tdd.h"` (line 4 of master)
- Remove trailing `#include "TempLat/lattice/algebra/axionalgebra/magneticfield4_test.h"` (last 2 lines before `#endif`)

### Step 2: Add `initialconditionstype.h`

**`src/include/CosmoInterface/initializers/initialconditionstype.h`** — Copy from master, with these changes:
- Remove `#include "TempLat/util/tdd/tdd.h"`
- Remove the `InitialConditionsTypeTester` class (lines 50-55)
- Remove the trailing `#ifdef TEMPLATTEST ... #endif` block

### Step 3: Add `axioncouplings.h`

**`src/include/CosmoInterface/definitions/axioncouplings.h`** — Copy from master, with these changes:
- Remove the `public: #ifdef TEMPLATTEST ... #endif` block inside the class
- Remove the trailing `#ifdef TEMPLATTEST ... #include ... #endif` block
- Remove `#include "TempLat/util/tdd/tdd.h"` if present (not in master's version — it's not there, but the `staticif.h` include is)

Note: `axioncouplings.h` uses `Field<T>` (master convention) in `complexfieldshift.h` etc. but the actual axion coupling expressions only use `model.fldS(n)`, `model.piU1(n)`, etc. which are already `Field<NDim, T>` in GPU_impl. The file's own templates don't instantiate `Field` directly — they operate on whatever the model provides. So no field-type adaptation is needed in this file itself.

### Step 4: Adapt `abstractmodel.h` (API-breaking)

This is the core structural change. The `AbstractModel` template gains a new `SCALARU1AXIONCOUPLINGS` parameter.

Changes to GPU_impl's `abstractmodel.h`:

1. **Add include** at top: `#include "CosmoInterface/initializers/initialconditionstype.h"`

2. **Add to `DefaultModelPars`** (after `SU2DoubletSU2Couplings` typedef):
   ```cpp
   typedef CouplingsManager<NScalars, NU1Flds> ScalarU1AxionCouplings;
   ```

3. **Update `MakeAbstractModelTemplateArgs` macro** to include `typename _ModelParsType::ScalarU1AxionCouplings` after `SU2DoubletSU2Couplings`:
   ```cpp
   #define MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, _FloatType) \
     _ModelName, _ModelParsType::NPotTerms, _ModelParsType::NScalars, _ModelParsType::NCScalars, \
     _ModelParsType::NU1Flds, _ModelParsType::NSU2Doublet, _ModelParsType::NSU2Flds, \
     typename _ModelParsType::CsU1Couplings, typename _ModelParsType::SU2DoubletU1Couplings, \
     typename _ModelParsType::SU2DoubletSU2Couplings, typename _ModelParsType::ScalarU1AxionCouplings, \
     _FloatType, _ModelParsType::NDim
   ```

4. **Update `AbstractModel` template parameter list** to add `typename SCALARU1AXIONCOUPLINGS` after `SU2DOUBLETSU2COUPLINGS`:
   ```cpp
   template<class R, size_t NPOTTERMS, size_t NS, size_t NC, size_t NU1FLDS, size_t NSU2DOUBLET, size_t NSU2FLDS,
            typename CSU1COUPLINGS, typename SU2DOUBLETU1COUPLINGS, typename SU2DOUBLETSU2COUPLINGS,
            typename SCALARU1AXIONCOUPLINGS, typename T = double, int NDIM = 3>
   ```

5. **Add using alias** after `SU2DoubletSU2Couplings`:
   ```cpp
   using ScalarU1AxionCouplings = SCALARU1AXIONCOUPLINGS;
   ```

6. **Add members** after `gQ_SU2DblSU2`:
   ```cpp
   ScalarU1AxionCouplings alphaLambda_SU1;
   ```
   And after `masses2SU2Doublet`:
   ```cpp
   T InverseAxionLambda;
   ```

7. **Add axion charge parsing** in constructor, after `gQ_SU2DblSU2.setEffectiveCharges(...)`:
   ```cpp
   auto gAxionU1 = parser.get<double, NU1>("gAxionU1", 1.0);
   auto AxionU1Charges = parser.get<double, ScalarU1AxionCouplings::howManyCouples()>("alphaLambda_AxionU1", 1);
   alphaLambda_SU1.setEffectiveCharges(AxionU1Charges, gAxionU1);
   ```

8. **Add `getFluctuationRatio` default** (after `getOneField()` method):
   ```cpp
   template<int N>
   auto getFluctuationRatio(Tag<N>) {
     return OneType();
   }
   ```

9. **Add `getU1IC` method** (after `getFluctuationRatio`):
   ```cpp
   InitialConditionsType::U1 getU1IC() {
     if (NCs > 0) return InitialConditionsType::RandomWithMatter;
     else return InitialConditionsType::PlaneWavesZeroB;
   }
   ```

### Step 5: Adapt `fieldfunctionals.h`

Add to GPU_impl's file:

1. **Add includes** at top (after existing includes):
   ```cpp
   #include "TempLat/lattice/algebra/axionalgebra/electricfield2.h"
   #include "TempLat/lattice/algebra/axionalgebra/magneticfield4.h"
   #include "TempLat/lattice/algebra/gaugealgebra/magneticfield.h"
   ```

2. **Add `EBU1` method** after `pi2U1`:
   ```cpp
   template <class Model, int N>
   static inline auto EBU1(Model &model, Tag<N> n)
   {
     return Total(i, 1, Model::NDim,
                  electricField2(model.piU1(n), i) * magneticField4(magneticField(model.fldU1(n), i), i));
   }
   ```

3. **Add `get_SU2_electric` methods** before `pi2SU2`:
   ```cpp
   template <class Model, int N, int DIR> static auto get_SU2_electric(Model &model, Tag<N> n, Tag<DIR> i)
   {
     auto El = model.piSU2(n)(i);
     return (El);
   }

   template <class Model, int N> static auto get_SU2_electric(Model &model, Tag<N> n)
   {
     return MakeVector(i, 1, Model::NDim, get_SU2_electric(model, n, i));
   }
   ```

4. **Rewrite `pi2SU2`** to use `get_SU2_electric` with factor-of-4 normalization:
   ```cpp
   template <class Model, int A>
   static inline auto pi2SU2(Model &model, Tag<A> a)
   {
     auto El = get_SU2_electric(model, a);
     return Total(i, 1, Model::NDim, Total(b, 1, 3, 4 * pow<2>(El(i)(b))));
   }
   ```

### Step 6: Adapt `gausslaws.h`

1. **Replace includes** — remove `su2generators.h` and `su2wrapper.h`, add:
   ```cpp
   #include "TempLat/lattice/algebra/spatialderivatives/backdiff.h"
   #include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
   #include "TempLat/lattice/algebra/axionalgebra/magneticfield4.h"
   #include "CosmoInterface/definitions/axioncouplings.h"
   ```

2. **Update `checkU1`** — Add `RHSaxion` and include it in diff/normalization. Replace the body of `checkU1` with master's version (adapted: keep GPU_impl's `make_templatvector` return style, incorporate `RHSaxion` into `diff` and `sum` per-component):
   ```cpp
   auto RHSaxion = AxionCouplings::ScalarAxionGaussLaw(model, n);
   auto RHS = -model.dx * MatterCurrents::U1ChargeDensity(model, n);
   auto LHS = Total(i, 1, Model::NDim, model.piU1(n)(i) - shift(model.piU1(n)(i), -i)) / model.dx;
   auto LHS1 = (model.piU1(n)(1_c) - shift(model.piU1(n)(1_c), -1_c)) / model.dx;
   auto LHS2 = (model.piU1(n)(2_c) - shift(model.piU1(n)(2_c), -2_c)) / model.dx;
   auto LHS3 = (model.piU1(n)(3_c) - shift(model.piU1(n)(3_c), -3_c)) / model.dx;

   auto diff = LHS - (RHS + RHSaxion);
   auto sum = pow<2>(LHS1) + pow<2>(LHS2) + pow<2>(LHS3) + pow<2>(RHS + RHSaxion);

   auto avDiff = average(sqrt(pow<2>(diff)));
   auto avSum = average(sqrt(pow<2>(sum)));
   auto avRHS = average(sqrt(pow<2>(RHS + RHSaxion)));
   auto avLHS = average(sqrt(pow<2>(LHS)));

   return make_templatvector(static_cast<double>(avDiff / avSum), static_cast<double>(avLHS), static_cast<double>(avRHS));
   ```

3. **Fix `checkSU2` normalization** — change from `(1.0 / model.dx) * ...` to `model.dx * MatterCurrents::SU2ChargeDensity(...)` for RHS, and remove the `1/dx` from LHS:
   ```cpp
   auto RHS = -model.dx * MatterCurrents::SU2ChargeDensity(model, n);
   auto LHS = Total(i, 1, Model::NDim,
                    (model.piSU2(n)(i) - shift(dagger(model.fldSU2(n)(i)) * model.piSU2(n)(i) * model.fldSU2(n)(i), -i)));
   ```

### Step 7: Adapt `fluctuationsgenerator.h`

Add the `planeWaves` method before the `private:` section (after `omega_k`). Use GPU_impl's `Field<Model::NDim, T>` field types where needed, but the method uses generic `VF` template params:

```cpp
template<class Model, class VF>
void planeWaves(Model& model, VF f, VF p, VF tmpF, VF tmpP, T aDot, T kCutOff) const
{
  ForLoop(i, 1, Model::NDim-1,
    ForLoop(a, 1, 3,
      conjugateGaussianFluctuations(model, tmpF(i), tmpP(i), 0.0, aDot, kCutOff);
      tmpF(i).inFourierSpace().setZeroMode(0);
      tmpP(i).inFourierSpace().setZeroMode(0);
    )
  );

  FourierSite<Model::NDim> ntilde(model.getToolBox());
  size_t N = GetNGrid::get(model);

  auto expIK = MakeVector(i, 1, Model::NDim, complexPhase(-2.0 * Constants::pi<T> / N * ntilde(i)));
  auto keffm = MakeVector(i, 1, Model::NDim, 1_c - expIK(i));
  auto keffm2 = Total(i, 1, Model::NDim, norm2(keffm(i)));

  auto e_basis = make_templatvector(0.25, 0.25, sqrt(2.0)/4.0);
  auto edotk = Total(i, 1, 3, e_basis(i) * keffm(i));

  auto lambda1 = MakeVector(i, 1, 3, e_basis(i) - edotk * (1.0 / keffm2) * conj(keffm(i)));
  auto invNLambda1 = safeDivide(1.0, sqrt(Total(i, 1, Model::NDim, norm2(lambda1(i)))));

  auto lambda2 = MakeVector(i, 1, 3,
    Total(j, 1, 3,
      Total(k, 1, 3,
        Symbols::epsilon(i, j, k) * e_basis(j) * keffm(k)
      )
    )
  );
  auto invNLambda2 = safeDivide(1.0, sqrt(Total(i, 1, Model::NDim, norm2(lambda2(i)))));

  ForLoop(i, 1, Model::NDim,
    f(i).inFourierSpace() = model.fStar / model.omegaStar * (tmpF(1_c).inFourierSpace() * asFourier(invNLambda1 * lambda1(i)) + tmpF(2_c).inFourierSpace() * asFourier(invNLambda2 * lambda2(i)));
    p(i).inFourierSpace() = model.fStar / model.omegaStar * (tmpP(1_c).inFourierSpace() * asFourier(invNLambda1 * lambda1(i)) + tmpP(2_c).inFourierSpace() * asFourier(invNLambda2 * lambda2(i)));
    f(i).inFourierSpace().setZeroMode(0);
    p(i).inFourierSpace().setZeroMode(0);
  );
}
```

Also add needed includes at top:
```cpp
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/complexalgebra/asfourier.h"
#include "TempLat/lattice/algebra/constants/symbols.h"
```

### Step 8: Adapt `scalarsingletinitializer.h`

Add two lines before the homogeneous background lines, inside the `if constexpr (Model::Ns > 0)` block:

```cpp
model.fldS = model.getFluctuationRatio(FieldsNumbering::fldS()) * model.fldS;
model.piS = model.getFluctuationRatio(FieldsNumbering::piS()) * model.piS;
```

These go after the `ForLoop(i, 0, Model::Ns-1, ...)` and before `model.fldS += model.fldS0 / ...`.

Also add include: `#include "CosmoInterface/fieldsnumbering.h"`

### Step 9: Adapt `u1initializer.h`

Major restructuring:

1. **Add includes**: `#include "CosmoInterface/initializers/initialconditionstype.h"`, `#include "CosmoInterface/extrafields.h"`, `#include "CosmoInterface/fieldsnumbering.h"`

2. **Add exception**: `MakeException(U1ICNotImplemented);` before the class

3. **Change `initializeU1` signature** to accept `ExtraFields<Model>`:
   ```cpp
   template <class Model, typename T>
   static void initializeU1(Model &model, FluctuationsGenerator<T> &fg, T kCutOff, ExtraFields<Model> extraFlds)
   ```

4. **Add IC type dispatch** as the body of `initializeU1`:
   ```cpp
   if (model.getU1IC() == InitialConditionsType::RandomWithMatter) initializeRandomWithMatterU1(model, fg, kCutOff);
   else if (model.getU1IC() == InitialConditionsType::PlaneWavesZeroB) initializePlaneWavesZeroBU1(model, fg, kCutOff, extraFlds);
   else throw(U1ICNotImplemented("The initial condition provided for U1 is not implemented."));
   ```

5. **Rename existing `initializeU1` body** to `initializeRandomWithMatterU1` (same content, new name, no `ExtraFields` param).

6. **Add `initializePlaneWavesZeroBU1`** method:
   ```cpp
   template <class Model, typename T>
   static void initializePlaneWavesZeroBU1(Model &model, FluctuationsGenerator<T> &fg, T kCutOff, ExtraFields<Model> extraFlds)
   {
     ForLoop(n, 0, Model::NU1 - 1,
       fg.planeWaves(model, model.fldU1(n), model.piU1(n), extraFlds.fldForPlaneWavesU1(), extraFlds.piForPlaneWavesU1(), model.aDotI, kCutOff);
       model.fldU1(n) = model.getFluctuationRatio(FieldsNumbering::fldU1()) * model.fldU1(n);
       model.piU1(n) = model.getFluctuationRatio(FieldsNumbering::piU1()) * model.piU1(n);
       ForLoop(i, 1, Model::NDim,
         model.fldU1(n)(i) = 0;
       );
     );
   }
   ```

### Step 10: Adapt `modelinitializer.h`

1. **Change `initialize` signature** to accept `ExtraFields<Model>`:
   ```cpp
   template <class Model>
   void initialize(Model &model, RunParameters<T> &rPar, ExtraFields<Model> extraFields)
   ```

2. **Change U1 init condition** from `NCs > 0 || (NSU2Doublet > 0 && NU1 > 0)` to `NCs > 0 || NU1 > 0`:
   ```cpp
   if constexpr (Model::NCs > 0 || Model::NU1 > 0)
     U1Initializer::initializeU1(model, fg, rPar.kCutoff, extraFields);
   ```

### Step 11: Adapt `scalarsingletkernels.h`

1. **Add include**: `#include "CosmoInterface/definitions/axioncouplings.h"`

2. **Append axion source** to `get()` return expression:
   ```cpp
   return (pow(model.aI, 1 + model.alpha) * LatLapl<Model::NDim>(model.fldS(n)) -
           pow(model.aI, 3 + model.alpha) * Potential::derivS(model, n) +
           AxionCouplings::ScalarAxionSource(model, n));
   ```

### Step 12: Adapt `u1kernels.h`

1. **Add include**: `#include "CosmoInterface/definitions/axioncouplings.h"`

2. **Add axion coupling computation** in `get()`, after `GradU1`:
   ```cpp
   auto AxionScalarSource = AxionCouplings::U1AxionCoupling(model, a);
   auto normU1AxionScalarSource = (model.fStar / Model::MPl);
   ```

3. **Append to return expression**:
   ```cpp
   return normGrad * (LaplU1 - GradU1) - normU1Source * U1Source + normU1AxionScalarSource * AxionScalarSource;
   ```

### Step 13: Adapt `extrafields.h`

1. **Add include**: `#include "CosmoInterface/initializers/initialconditionstype.h"`

2. **Add `else if` branch** in `allocateExtraMemory`, after the RK2N check:
   ```cpp
   else if (model.getU1IC() == InitialConditionsType::PlaneWaves || model.getU1IC() == InitialConditionsType::PlaneWavesZeroB) {
     fldU1IC = std::make_shared<VectorField<Field<Model::NDim, T>, T, Model::NDim>>("U1ICfldU1", model.getToolBox(), runPars.getLatParams());
     piU1IC = std::make_shared<VectorField<Field<Model::NDim, T>, T, Model::NDim>>("U1ICpiU1", model.getToolBox(), runPars.getLatParams());
   }
   ```

3. **Add accessor methods**:
   ```cpp
   auto fldForPlaneWavesU1() {
     if (allFlds1 != nullptr) return allFlds1->fldU1(0_c);
     else if (fldU1IC != nullptr) return *fldU1IC;
     else {
       throw(ExtraMemoryNotAllocated("Extra memory required by fldForPlaneWaves is not allocated, will crash."));
       return *fldU1IC;
     };
   }

   auto piForPlaneWavesU1() {
     if (allFlds1 != nullptr) return allFlds1->piU1(0_c);
     else if (piU1IC != nullptr) return *piU1IC;
     else {
       throw(ExtraMemoryNotAllocated("Extra memory required by fldForPlaneWaves is not allocated, will crash."));
       return *piU1IC;
     };
   }
   ```

4. **Add member variables** in `private`:
   ```cpp
   std::shared_ptr<VectorField<Field<Model::NDim, T>, T, Model::NDim>> fldU1IC;
   std::shared_ptr<VectorField<Field<Model::NDim, T>, T, Model::NDim>> piU1IC;
   ```

Note: GPU_impl uses `Field<Model::NDim, T>` while master uses `Field<T>` — all `VectorField` declarations must use the GPU_impl convention.

### Step 14: Add `topologicalchargesmeasurer.h`

**`src/include/CosmoInterface/measurements/topologicalchargesmeasurer.h`** — Copy from master, with these changes:
- Remove `#include "TempLat/util/tdd/tdd.h"`
- Remove `TopologicalChargesMeasurerTester` class
- Remove trailing `#ifdef TEMPLATTEST ... #endif` block
- Change `FilesManager` to `FilesManager<NDim>` in constructor parameter
- Add `static constexpr size_t NDim = Model::NDim;` if needed, or use `FilesManager<Model::NDim>` directly

### Step 15: Adapt `measurer.h`

1. **Add include**: `#include "CosmoInterface/measurements/topologicalchargesmeasurer.h"`

2. **Add constructor init** in initializer list (after `scaleFactorMeasurer`):
   ```cpp
   topologicalChargesMeasurer(model, filesManager, par, par.appendMode),
   ```

3. **Add measure call** in frequent output section (after `scaleFactorMeasurer.measure(...)`):
   ```cpp
   topologicalChargesMeasurer.measure(model, t);
   ```

4. **Add member variable** in private section:
   ```cpp
   TopologicalChargesMeasurer<T> topologicalChargesMeasurer;
   ```

### Step 16: Add model files

**`src/models/U1Axion.h`** — Copy from master, with these changes:
- Change constructor signature: `std::shared_ptr<MemoryToolBox>` → `device::memory::host_ptr<MemoryToolBox<NDim>>`
- Add `static constexpr size_t NDim = Model<MODELNAME>::NDim;` before constructor
- Keep all axion-specific logic (kappa, m2, fluctuationRatio, getFluctuationRatio overloads, InverseAxionLambda)

**`src/models/m2phi2_axionU1.h`** — Copy from master, with these changes:
- Change constructor signature: `std::shared_ptr<MemoryToolBox>` → `device::memory::host_ptr<MemoryToolBox<NDim>>`
- Add `static constexpr size_t NDim = Model<MODELNAME>::NDim;` before constructor

### Step 17: Add parameter file

**`src/models/parameter-files/AxionU1.in`** — Copy from master as-is.

### Step 18: Adapt `cosmolattice.cpp`

Change the initializer call from 2-arg to 3-arg:
```cpp
initializer.initialize(model, runParams, extraFlds);
```

### Step 19: Update merge status

Update `thoughts/shared/merge_triage/status.md` to mark Theme 8 as done.

## Verification

### Automated
- [x] Build compiles: `cd cosmolattice_GPU_impl/debug_build && cmake -DSerial=ON -DMPI=ON -DHDF5=ON -DTESTING=ON -DMYHDF5_PATH=.../MyHDF5Parallel/ -DMYFFTW3_PATH=.../MyLibs/ -DMODEL=lphi4SU2U1 ../ && make cosmolattice`
- [x] Build also compiles with `-DMODEL=lphi4U1` (exercises axion code paths with default non-coupling)

### Manual
- [ ] Verify that existing models (lphi4SU2U1, lphi4U1) still compile — they should, since `DefaultModelPars` provides a default `ScalarU1AxionCouplings` typedef that doesn't couple anything
- [ ] The new model files (U1Axion, m2phi2_axionU1) won't be buildable until added to CMakeLists.txt, which is a separate step the user can do later

## API Impact

1. **`AbstractModel` template parameter list changed** — gains `SCALARU1AXIONCOUPLINGS` parameter. All model files use `MakeModel` macro which handles this automatically via `DefaultModelPars`.
2. **`MakeAbstractModelTemplateArgs` macro changed** — now passes `ScalarU1AxionCouplings` from `ModelPars`.
3. **`modelinitializer.h::initialize()` signature changed** — now takes `ExtraFields<Model>` as 3rd param.
4. **`u1initializer.h::initializeU1()` signature changed** — now takes `ExtraFields<Model>` as 4th param.
5. **`cosmolattice.cpp`** — must pass `extraFlds` to `initializer.initialize()`.
6. **`pi2SU2` formula changed** — now uses factor-of-4 generator normalization via `get_SU2_electric`.

## Post-Implementation Checklist
- [ ] Update `cosmolattice_GPU_impl/thoughts/shared/merge_triage/status.md`: set this theme's status to **done**, link the plan file, and add the commit hash
