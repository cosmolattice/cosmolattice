# Merge NMC (Group 1) + tMinust0 Refactor (Group 3) into cosmolattice_GPU_impl

## Overview

Port the Non-Minimal Coupling to gravity (NMC) feature and the `tMinust0` kernel refactor from `cosmolattice_private` branch `U1AxionNMCS` into `cosmolattice_GPU_impl`. Explicitly **excludes** all Group 2 (Chiral Projector / Bunch-Davis IC) changes.

**Source**: `cosmolattice_private/src/include/` (branch `U1AxionNMCS`, commit `81c610a7`)
**Target**: `cosmolattice_GPU_impl/include/` (and `models/`)

## Current State Analysis

### What exists in GPU impl (target):
- All core CosmoInterface files exist but lack NMC support
- `AbstractModel` has no `NonMinimalCouplings` typedef, no NMC averages, no `piAI`, no `xis`
- `CouplingsManager` lacks compile-time guards for empty coupling lists (needed for `CouplingsManager<NScalars, 1>`)
- `EoM<T>` class has no `tMinust0` member or `cache(model, tIn)` overload
- `rk2nstorage.h` has commented-out NMC code in `advanceScaleFactor()`
- `ScalarAxionSource` and `U1AxionCoupling` don't accept `tMinust0` parameter
- No `nonminimalcoupling.h`, `lphi4NonMinimal.h`, or `lphi4NonMinimal.in`

### Key GPU impl differences from private repo:
- Field types are templated on `NDim`: `Field<T, NDim>` vs `Field` (private uses old `Field<T>` and passes it as template param)
- `MemoryToolBox` is templated: `MemoryToolBox<NDim>` with `device::memory::host_ptr<>` wrapper
- `FilesManager` is templated: `FilesManager<Model::NDim>`
- Uses `device::memory::getAtOnePoint()` / `device::memory::setAtOnePoint()` instead of `GetValue::get()` / `.getSet()`
- Uses `if constexpr` more consistently
- Missing `TDD` test infrastructure (no `TEMPLATTEST` guards, no `_test.h` includes in many files)
- `AbstractModel` has `t0` and `t` members (not in private)
- `AbstractModel` has `InverseAxionLambda` member
- `AbstractModel` includes `initialconditionstype.h` and `forwardcovariantderivative.h`
- `AbstractModel` has deleted copy constructor/assignment

## Desired End State

After this plan is complete:
1. NMC feature fully functional: models can define `NonMinimalCouplings` typedef, specify `xis` couplings, and the Ricci scalar R is computed and used in scale factor evolution
2. `tMinust0` properly threaded through kernel evaluation chain for time-dependent coupling switches
3. `lphi4NonMinimal` model available as a test/example model
4. All existing models compile and run identically (zero physics change for non-NMC models)

### How to verify:
- Build `lphi4` model → compiles and runs identically to before
- Build `lphi4NonMinimal` model → compiles and runs with NMC enabled
- Build any axion-U1 model → compiles with new `tMinust0` threading

## What We're NOT Doing

- **No Group 2 features**: No chiral projector, no Bunch-Davis IC for gauge fields, no `fasepi2.h`, `multiplicity.h`, `interpolator.h`, `spline.h`, `chiralprojector.h`
- **No `flagChiralPS`** parameter (Group 2)
- **No `fldS` IC type enum** or `initializeScalarsExternalPS` (Group 2)
- **No `NonLinearSwitch`** in axioncouplings.h (Group 2 physics — we DO port the `tMinust0` signature change but keep the physics as-is)
- **No `tNonLinearAxionU1`** member in abstractmodel (Group 2)
- **No `extPS`** array in abstractmodel (Group 2 — external power spectrum)
- **No `BunchDavisTransverseU1`** IC type (Group 2)
- **No changes** to `fluctuationsgenerator.h`, `scalarsingletinitializer.h`, `u1initializer.h`, `u1measurer.h`, `extrafields.h`, `initialconditionstype.h`

## Implementation Approach

We implement in 3 phases:
1. **Infrastructure**: Core type system changes (`CouplingsManager`, `KernelsTypes`, `AbstractModel`)
2. **Physics**: NMC computation, scale factor kernels, scalar kernels, energies, averages, evolution
3. **Model + Measurements**: New model file, energy/scale-factor measurement output, initializers

Files are modified in dependency order to ensure the code compiles at each phase boundary. **Within Phase 2**, note that `nonminimalcoupling.h` (2.1) depends on `rhoMinimal`/`pMinimal` from `energies.h` (2.3). Implement 2.3 before 2.1 during actual coding, or implement the full phase before attempting compilation.

---

## Phase 1: Infrastructure — Type System & Core Plumbing

### Overview
Add the `NonMinimalCouplings` template parameter to the model system, add `tMinust0` to the kernel types, and fix `CouplingsManager` for empty coupling lists.

### Changes Required:

#### 1.1 `include/CosmoInterface/couplingsmanager.h` — Add compile-time guards

The GPU impl version already has the `if constexpr (NGauge > 0)` guard in `setEffectiveCharges()` (line 50), but is **missing** the guards in `operator()` and `couples()` that the private repo added. These guards are needed because `CouplingsManager<NScalars, 1>` (used for NMC) will be instantiated with `sizeof...(Bools) == 0` when no NMC couplings are specified in `DefaultModelPars`.

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/couplingsmanager.h`

**Change 1** — `operator()` (lines 73-76): Add `sizeof...(Bools) == 0 || NGauge == 0` guard:
```cpp
    template <int nmat, int ng> double operator()(Tag<nmat>, Tag<ng>) const
    {
      if constexpr (sizeof...(Bools) == 0 || NGauge == 0) {
        return 0.0;
      } else {
        return effectiveCharges[nmat * NGauge + ng];
      }
    }
```

**Change 2** — `couples()` (lines 83-86): Add same guard:
```cpp
    template <int nmat, int ng> static constexpr bool couples(Tag<nmat>, Tag<ng>)
    {
      if constexpr (sizeof...(Bools) == 0 || NGauge == 0) {
        return false;
      } else {
        return doesCouples[nmat * NGauge + ng];
      }
    }
```

#### 1.2 `include/CosmoInterface/evolvers/kernels/kernelstypes.h` — Add `tMinust0` member

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/evolvers/kernels/kernelstypes.h`

Add `tMinust0` member to `EoM<T>` and a `cache(Model&, T tIn)` overload. The existing `cache(Model&)` must be preserved for backward compatibility.

Replace the `EoM` class (lines 21-25) with:
```cpp
        template<typename T> class EoM{
        public:
            T tMinust0 = T(0);

            template<class Model>
            void cache(Model& model, T tIn) {
                tMinust0 = tIn;
            }

            template<class Model>
            void cache(Model & model) {}
        };
```

#### 1.3 `include/CosmoInterface/abstractmodel.h` — Add NMC template parameter & members

This is the most complex change. The GPU impl's `AbstractModel` needs:
1. A new `NonMinimalCouplings` typedef in `DefaultModelPars`
2. An additional template parameter `NONMINCOUPLINGS` in `AbstractModel`
3. Updated macros to pass the new parameter
4. New member variables for NMC averages
5. New `xis` coupling manager instance
6. Parsing of `xis` in constructor
7. `IsNonMinimallyCoupled` constexpr

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/abstractmodel.h`

**Change 1** — Add `NonMinimalCouplings` typedef to `DefaultModelPars` (after line 53):
```cpp
    using NonMinimalCouplings = CouplingsManager<NScalars, 1>;   // Non-minimal coupling to gravity of scalars.
```

**Change 2** — Update `MakeAbstractModelTemplateArgs` macro (lines 63-67) to include `NonMinimalCouplings`:
```cpp
#define MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, _FloatType)                                          \
  _ModelName, _ModelParsType::NPotTerms, _ModelParsType::NScalars, _ModelParsType::NCScalars, _ModelParsType::NU1Flds, \
      _ModelParsType::NSU2Doublet, _ModelParsType::NSU2Flds, typename _ModelParsType::CsU1Couplings,                   \
      typename _ModelParsType::SU2DoubletU1Couplings, typename _ModelParsType::SU2DoubletSU2Couplings,                 \
      typename _ModelParsType::ScalarU1AxionCouplings, typename _ModelParsType::NonMinimalCouplings,                    \
      _FloatType, _ModelParsType::NDim
```

**Change 3** — Add `NONMINCOUPLINGS` template parameter to `AbstractModel` class (lines 77-80):
```cpp
  template <class R, size_t NPOTTERMS, size_t NS, size_t NC, size_t NU1FLDS, size_t NSU2DOUBLET, size_t NSU2FLDS,
            typename CSU1COUPLINGS, typename SU2DOUBLETU1COUPLINGS, typename SU2DOUBLETSU2COUPLINGS,
            typename SCALARU1AXIONCOUPLINGS, typename NONMINCOUPLINGS, typename T = double, int NDIM = 3>
  class AbstractModel
```

**Change 4** — Add `NonMinimalCouplings` using declaration and `IsNonMinimallyCoupled` constexpr after `ScalarU1AxionCouplings` (after line 111):
```cpp
    using NonMinimalCouplings = NONMINCOUPLINGS;
    static constexpr bool IsNonMinimallyCoupled = NonMinimalCouplings::howManyCouples() > 0;
```

**Change 5** — Add NMC average members after the `SU2pi2AvSI...` block (after line 175, before `potAvI`):
```cpp
    // --> Averages needed for the non-minimal coupling
    T RI;
    TempLatArray<T, Ns> fld2AvSI_i;
    TempLatArray<T, Ns> grad2AvSI_i;
    TempLatArray<T, Ns> pi2AvSI_i;
    TempLatArray<T, Ns> fldPiAvSI;
    TempLatArray<T, Ns> fldVpAvSI;

    T piAI;
```

**Change 6** — Add `NonMinimalCouplings xis;` member after `ScalarU1AxionCouplings alphaLambda_SU1;` (after line 204):
```cpp
    NonMinimalCouplings xis;
```

**Change 7** — In constructor (around line 268), add parsing of `xis` and `setEffectiveCharges`:
After the line `alphaLambda_SU1.setEffectiveCharges(AxionU1Charges, gAxionU1);` add:
```cpp
      auto xiCouplings = parser.get<T, NonMinimalCouplings::howManyCouples()>("xis", 1);
      xis.setEffectiveCharges(xiCouplings, {1});
```

**`getU1IC()` is NOT modified** — the private repo's version references `BunchDavisTransverseU1` which is a Group 2 addition. We keep the existing GPU impl version. Models that need axion U1 coupling can override `getU1IC()` in the model file.

### Success Criteria:

#### Automated Verification:
- [x] `cmake -DMODEL=lphi4 ../ && make cosmolattice` compiles successfully (existing model still works with new template parameter)

#### Manual Verification:
- [x] Review that `DefaultModelPars::NonMinimalCouplings` defaults to `CouplingsManager<NScalars, 1>` where `NScalars=0` in `DefaultModelPars`, so inherited typedef gives `CouplingsManager<0, 1>` with `howManyCouples() == 0` and `IsNonMinimallyCoupled == false` for all existing models (even though lphi4 overrides `NScalars=2`, the typedef is evaluated in the base class scope)

**Implementation Note**: After completing this phase and all automated verification passes, pause here for manual confirmation from the human that the manual testing was successful before proceeding to the next phase.

---

## Phase 2: Physics — NMC Computation, Kernels, Energies, Averages, Evolution

### Overview
Add the core NMC physics: Ricci scalar computation, modified Friedmann equation in scale factor kernels, NMC source term in scalar EOM, NMC energy densities, NMC averages, and modified scale factor evolution (piAI).

### Changes Required:

#### 2.1 New file: `include/CosmoInterface/definitions/nonminimalcoupling.h`

**Action**: Copy from private repo and adapt to GPU impl conventions.

Create file `cosmolattice_GPU_impl/include/CosmoInterface/definitions/nonminimalcoupling.h` with the content from the private repo. Key adaptations:
- Remove `#include "TempLat/util/tdd/tdd.h"` (GPU impl doesn't use TDD)
- Remove `TEMPLATTEST` guard and `_test.h` include at bottom
- The physics code (the `F()` and `R()` static methods) is pure template algebra and should work as-is

```cpp
#ifndef COSMOINTERFACE_DEFINITIONS_NONMINIMALCOUPLING_H
#define COSMOINTERFACE_DEFINITIONS_NONMINIMALCOUPLING_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

#include "CosmoInterface/definitions/energies.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/util/templatvector.h"

namespace TempLat {

    class NonMinimalCoupling {
    public:
        NonMinimalCoupling() = delete;

        template<class Model>
        static auto F(Model& model) {
            auto sum = Total(i, 0, Model::Ns - 1,
              IfElse(Model::NonMinimalCouplings::couples(i, Tag<0>()),
                (6.0 * model.xis(i, 0_c) - 1.0) * model.xis(i, 0_c) * pow<2>(model.fStar / model.MPl) * model.fld2AvSI_i(i),
                ZeroType()
            ));
            return 1.0 / (1.0 + sum);
        }

        template<class Model>
        static auto R(Model& model) {
            auto norm = pow<2>(model.fStar / Model::MPl) * NonMinimalCoupling::F(model);
            auto minimalContribution = Energies::rhoMinimal(model) - 3.0 * Energies::pMinimal(model);
            auto nonMinimalContribution = Total(i, 0, Model::Ns - 1,
                IfElse(Model::NonMinimalCouplings::couples(i, Tag<0>()),
                  12 * model.xis(i, 0_c) * (model.pi2AvSI_i(i) - model.grad2AvSI_i(i)) - 6.0 * model.xis(i, 0_c) * model.fldVpAvSI(i),
                  ZeroType())
            );
            return norm * (minimalContribution + nonMinimalContribution);
        }
    };

} /* TempLat */

#endif
```

#### 2.2 `include/CosmoInterface/definitions/averages.h` — Add NMC averages block

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/definitions/averages.h`

After the `NSU2Doublet` block (after line 116, before the closing `}` of `setAllAverages`), add:
```cpp
      if constexpr (Model::IsNonMinimallyCoupled) {
        ForLoop(i, 0, Model::Ns - 1,
          model.fld2AvSI_i(i) = average(pow<2>(model.fldS(i)));
          model.grad2AvSI_i(i) = average(FieldFunctionals::grad2S(model, i));
          model.pi2AvSI_i(i) = average(FieldFunctionals::pi2S(model, i));

          model.grad2AvSI_i(i) = 0.5 * pow<-2>(model.aI) * model.grad2AvSI_i(i);
          model.pi2AvSI_i(i) = 0.5 * pow<-6>(model.aI) * model.pi2AvSI_i(i);

          model.fldPiAvSI(i) = average(model.fldS(i) * model.piS(i));
          model.fldVpAvSI(i) = average(model.fldS(i) * Potential::derivS(model, i));
        );
      }
```

#### 2.3 `include/CosmoInterface/definitions/energies.h` — Add NMC energy densities

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/definitions/energies.h`

After the `magneticSU2SI` function (after line 151), add the following new methods:

```cpp
    template <class Model> static inline auto rhoNMCAv1(Model &model)
    {
      auto rhoNMC1 = Total(i, 0, Model::Ns - 1,
          IfElse(Model::NonMinimalCouplings::couples(i, 0_c),
            3 * model.xis(i, 0_c) *
             (pow(model.aI, -2 * model.alpha - 2) * pow(model.aDotI, 2) * model.fld2AvSI_i[i]),
            0));
      return rhoNMC1;
    }

    template <class Model> static inline auto rhoNMCAv2(Model &model)
    {
      auto rhoNMC2 = Total(i, 0, Model::Ns - 1,
          IfElse(Model::NonMinimalCouplings::couples(i, 0_c),
            3 * model.xis(i, 0_c) *
             (2 * pow(model.aI, -model.alpha - 4) * model.aDotI * model.fldPiAvSI[i]),
            0));
      return rhoNMC2;
    }

    template <class Model> static inline auto rhoNMCAv(Model &model)
    {
      return rhoNMCAv1(model) + rhoNMCAv2(model);
    }
```

Modify the existing `rho()` method (lines 153-168) to include the NMC energy:

Replace the return statement (lines 167-168) with:
```cpp
      auto ENMC = [&]() {
        if constexpr (Model::IsNonMinimallyCoupled) return rhoNMCAv(model);
        else return ZeroType();
      }();

      return (Eks + Ekcs + EkSU2Dbl + Egs + Egcs + EgSU2Dbl + EelU1 + EmagU1 + EelSU2 + EmagSU2 +
              ENMC + model.potAvI);
```

Add `rhoMinimal()` and `pMinimal()` methods (needed by `NonMinimalCoupling::R()`):

```cpp
    template <class Model> static inline auto rhoMinimal(Model &model)
    {
      Averages::setAllAverages(model);
      auto Eks = (model.Ns > 0 ? kineticS(model) : 0);
      auto Ekcs = (model.NCs > 0 ? kineticCS(model) : 0);
      auto EkSU2Dbl = (model.NSU2Doublet > 0 ? kineticSU2Doublet(model) : 0);
      auto Egs = (model.Ns > 0 ? gradientS(model) : 0);
      auto Egcs = (model.NCs > 0 ? gradientCS(model) : 0);
      auto EgSU2Dbl = (model.NSU2Doublet > 0 ? gradientSU2Doublet(model) : 0);
      auto EelU1 = (model.NU1 > 0 ? electricU1(model) : 0);
      auto EmagU1 = (model.NU1 > 0 ? magneticU1(model) : 0);
      auto EelSU2 = (model.NSU2 > 0 ? electricSU2(model) : 0);
      auto EmagSU2 = (model.NSU2 > 0 ? magneticSU2(model) : 0);

      return (Eks + Ekcs + EkSU2Dbl + Egs + Egcs + EgSU2Dbl + EelU1 + EmagU1 + EelSU2 + EmagSU2 +
              model.potAvI);
    }

    template <class Model> static inline auto pMinimal(Model &model)
    {
      Averages::setAllAverages(model);
      auto Eks = (model.Ns > 0 ? kineticS(model) : 0);
      auto Ekcs = (model.NCs > 0 ? kineticCS(model) : 0);
      auto EkSU2Dbl = (model.NSU2Doublet > 0 ? kineticSU2Doublet(model) : 0);
      auto Egs = (model.Ns > 0 ? gradientS(model) : 0);
      auto Egcs = (model.NCs > 0 ? gradientCS(model) : 0);
      auto EgSU2Dbl = (model.NSU2Doublet > 0 ? gradientSU2Doublet(model) : 0);
      auto EelU1 = (model.NU1 > 0 ? electricU1(model) : 0);
      auto EmagU1 = (model.NU1 > 0 ? magneticU1(model) : 0);
      auto EelSU2 = (model.NSU2 > 0 ? electricSU2(model) : 0);
      auto EmagSU2 = (model.NSU2 > 0 ? magneticSU2(model) : 0);

      return (Eks + Ekcs + EkSU2Dbl - 1.0/3.0 * (Egs + Egcs + EgSU2Dbl) +
              1.0/3.0 * (EelU1 + EmagU1 + EelSU2 + EmagSU2) - model.potAvI);
    }
```

#### 2.4 `include/CosmoInterface/evolvers/kernels/scalefactorkernels.h` — NMC branching in `get()`

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/evolvers/kernels/scalefactorkernels.h`

Add `#include "CosmoInterface/definitions/nonminimalcoupling.h"` after the `energies.h` include (line 10).

Replace the `get()` method body (lines 26-67) with:

```cpp
    template <class Model> static auto get(Model &model)
    {
      using T = decltype(model.aI);

      if constexpr (not Model::IsNonMinimallyCoupled) {
        // The kernel of the scale factor is sourced by the different volume-averaged energy densities:
        T Eks = 0;
        T Egs = 0;
        T Ekcs = 0;
        T Egcs = 0;
        T EkSU2Dbl = 0;
        T EgSU2Dbl = 0;
        T EmagU1 = 0;
        T EelU1 = 0;
        T EmagSU2 = 0;
        T EelSU2 = 0;

        if (model.Ns > 0) {
          Eks = Energies::kineticS(model);
          Egs = Energies::gradientS(model);
        }
        if (model.NCs > 0) {
          Ekcs = Energies::kineticCS(model);
          Egcs = Energies::gradientCS(model);
        }
        if (model.NSU2Doublet > 0) {
          EkSU2Dbl = Energies::kineticSU2Doublet(model);
          EgSU2Dbl = Energies::gradientSU2Doublet(model);
        }
        if (model.NU1 > 0) {
          EelU1 = Energies::electricU1(model);
          EmagU1 = Energies::magneticU1(model);
        }
        if (model.NSU2 > 0) {
          EelSU2 = Energies::electricSU2(model);
          EmagSU2 = Energies::magneticSU2(model);
        }

        return pow(model.aI, 2 * model.alpha + 1) / 3.0 * pow<2>(model.fStar / Model::MPl) *
               ((model.alpha - 2) * (Eks + Ekcs + EkSU2Dbl) + model.alpha * (Egs + Egcs + EgSU2Dbl) +
                (model.alpha - 1) * (EelU1 + EmagU1 + EelSU2 + EmagSU2) + (model.alpha + 1) * model.potAvI);
      } else {
        model.RI = NonMinimalCoupling::R(model);
        return pow(model.aI, 2 + model.alpha) / 6.0 * model.RI;
      }
    }
```

**Note**: We use `if constexpr` here to match the private repo's pattern and to avoid calling `NonMinimalCoupling::R(model)` for non-NMC models (where the NMC averages like `fld2AvSI_i` would be uninitialized). Although `model.RI` exists as a member in all models (declared unconditionally in `AbstractModel`), the NMC branch should only execute when NMC is actually active.

#### 2.5 `include/CosmoInterface/definitions/axioncouplings.h` — Add `tMinust0` parameter (Group 3 plumbing only)

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/definitions/axioncouplings.h`

This is a shared file between Groups 2 and 3. We port ONLY the Group 3 change: adding `tMinust0` as a template parameter to `ScalarAxionSource` and `U1AxionCoupling`. We do NOT add the `NonLinearSwitch` logic (Group 2).

The existing functions are **replaced** (not overloaded) with new signatures that accept `tMinust0`. The `tMinust0` parameter is accepted but NOT used in the physics (no `NonLinearSwitch` — that's Group 2). This is pure signature plumbing for forward compatibility.

**Change 1** — Replace `ScalarAxionSource` (existing 2-arg) with a 3-arg version + backward-compatible 2-arg wrapper:
```cpp
        // New primary signature with tMinust0
        template<class Model, int N, typename TT>
        static auto ScalarAxionSource(Model& model, Tag<N> n, TT tMinust0) {
          return  Total(a,0,Model::NU1-1,
                        IfElse(Model::ScalarU1AxionCouplings::couples(Tag<N>(),a),
                                pow<2>(model.omegaStar)/(model.fStar * Model::MPl) * pow(model.aI,model.alpha - 1) * model.alphaLambda_SU1(n,a) * Total(i, 1, Model::NDim, electricField2(model.piU1(a), i) * magneticField4(magneticField(model.fldU1(a), i), i)),
                        ZeroType()
                        ));
        }

        // Backward-compatible wrapper (calls the 3-arg version with tMinust0=0)
        template<class Model, int N>
        static auto ScalarAxionSource(Model& model, Tag<N> n) {
          return ScalarAxionSource(model, n, 0.0);
        }
```

**Change 2** — Replace `U1AxionCoupling` (3-arg with spatial index) with 4-arg version + backward-compatible 3-arg wrapper:
```cpp
        // New primary signature with tMinust0
        template<class Model, int N, int I, typename TT>
        static auto U1AxionCoupling(Model& model, Tag<N> n, Tag<I> i, TT tMinust0) {
            // ... keep existing physics body UNCHANGED (AxionCoupl1, AxionCoupl2 computation) ...
            return (pow(model.aI, model.alpha - 3) * AxionCoupl1 + pow(model.aI, -1 + model.alpha) * AxionCoupl2);
        }

        // Backward-compatible wrapper
        template<class Model, int N, int I>
        static auto U1AxionCoupling(Model& model, Tag<N> n, Tag<I> i) {
            return U1AxionCoupling(model, n, i, 0.0);
        }
```

**Change 3** — Replace `U1AxionCoupling` (2-arg vector) with 3-arg version + backward-compatible 2-arg wrapper:
```cpp
        // New primary signature with tMinust0
        template<class Model, int N, typename TT>
        static auto U1AxionCoupling(Model& model, Tag<N> t, TT tMinust0)
        {
            return MakeVector(i,1,Model::NDim,U1AxionCoupling(model,t,i,tMinust0));
        }

        // Backward-compatible wrapper
        template<class Model, int N>
        static auto U1AxionCoupling(Model& model, Tag<N> t)
        {
            return U1AxionCoupling(model, t, 0.0);
        }
```

#### 2.6 `include/CosmoInterface/evolvers/kernels/scalarsingletkernels.h` — NMC source term + tMinust0

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/evolvers/kernels/scalarsingletkernels.h`

Replace the `get()` method (lines 30-36) to:
1. Extract `tMinust0` from `eom` (Group 3)
2. Add NMC source term `ξ·φ·R` in the potential derivative (Group 1)
3. Pass `tMinust0` to `ScalarAxionSource` (Group 3)

```cpp
    template <class Model, int N, class T> static auto get(Model &model, Tag<N> n, KernelsTypes::EoM<T> eom)
    {
      auto tMinust0 = eom.tMinust0;

      return (pow(model.aI, 1 + model.alpha) * LatLapl<Model::NDim>(model.fldS(n)) -
              pow(model.aI, 3 + model.alpha) * (Potential::derivS(model, n)
              + IfElse(Model::NonMinimalCouplings::couples(Tag<N>(), Tag<0>()), model.xis(n, 0_c) * model.fldS(n) * model.RI, ZeroType())) +
              AxionCouplings::ScalarAxionSource(model, n, tMinust0));
    }
```

**IMPORTANT — Prefactor convention**: In the GPU impl, `ScalarAxionSource` already includes the `pow<2>(omegaStar)/(fStar*MPl) * pow(aI, alpha-1)` prefactor inside it (see `axioncouplings.h` line 47). In the private repo, this prefactor was moved to the caller — but that's a Group 2 change we're NOT porting. So we keep the GPU impl convention: no prefactor in the caller, prefactor stays inside `ScalarAxionSource`. The code above is correct as written.

#### 2.7 `include/CosmoInterface/evolvers/kernels/u1kernels.h` — Pass `tMinust0` (Group 3)

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/evolvers/kernels/u1kernels.h`

Modify `get()` to extract `tMinust0` from `eom` and pass it to `AxionCouplings::U1AxionCoupling`:

```cpp
    template <class Model, int N, class T> static auto get(Model &model, Tag<N> a, KernelsTypes::EoM<T> eom)
    {
      auto tMinust0 = eom.tMinust0;

      auto U1Source = MatterCurrents::U1Current(model, a);
      auto LaplU1 = MakeVector(i, 1, Model::NDim, LatLapl<Model::NDim>(model.fldU1(a)(i)));
      auto GradU1 =
          MakeVector(i, 1, Model::NDim, Total(j, 1, Model::NDim, backDiff(forwDiff(model.fldU1(a)(j), i), j)));

      auto AxionScalarSource = AxionCouplings::U1AxionCoupling(model, a, tMinust0);
      auto normU1AxionScalarSource = (model.fStar / Model::MPl);

      auto normU1Source = pow(model.aI, 1 + model.alpha);
      auto normGrad = pow(model.aI, -1 + model.alpha);

      return normGrad * (LaplU1 - GradU1) - normU1Source * U1Source + normU1AxionScalarSource * AxionScalarSource;
    }
```

#### 2.8 `include/CosmoInterface/evolvers/rk2nstorage.h` — Pass `tMinust0` to cache + NMC scale factor evolution

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/evolvers/rk2nstorage.h`

**Change 1** — In `evolve()`, change `kt.cache(model)` to `kt.cache(model, tMinust0)` (lines 79 and 109):

Line 79: `kt.cache(model);` → `kt.cache(model, tMinust0);`
Line 109: `kt.cache(model);` → `kt.cache(model, tMinust0);`

**Change 2** — In `advanceScaleFactor()` (lines 129-138), replace the commented-out NMC block with the active NMC code:

```cpp
        void advanceScaleFactor(Model& model, size_t i){
            if(sfDefined) model.aI += Bs[i] * deltaA;

            if constexpr (not Model::IsNonMinimallyCoupled) {
                if(sfDefined) model.aDotI += Bs[i] * deltaADot;
            }
            else {
                if(sfDefined) model.piAI += Bs[i] * deltaADot;
                if(sfDefined) model.aDotI = model.piAI * pow(model.aI, model.alpha - 1);
            }
        }
```

### Success Criteria:

#### Automated Verification:
- [x] `cmake -DMODEL=lphi4 ../ && make cosmolattice` compiles successfully
- [x] Run `./lphi4 input=../models/parameter-files/lphi4.in` — produces identical output to before (regression check)

#### Manual Verification:
- [x] Review NMC physics equations match the private repo
- [x] Verify `if constexpr` is used everywhere that accesses NMC-only members

**Implementation Note**: After completing this phase and all automated verification passes, pause here for manual confirmation from the human that the manual testing was successful before proceeding to the next phase.

---

## Phase 3: Model, Measurements, Initializers & RunParameters

### Overview
Add the `lphi4NonMinimal` example model, update energy/scale-factor measurement output for NMC, update the initializers for NMC-aware Hubble rate, and add the `a0` run parameter.

### Changes Required:

#### 3.1 `include/CosmoInterface/runparameters.h` — Add `a0` parameter

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/runparameters.h`

**Change 1** — Add `a0` parameter to the constructor initializer list. After `H0(...)` (line 57), add:
```cpp
          a0(par.get<T>("a0", 1.0)), // Initial scale factor (default 1.0)
```

**Note**: The private repo uses `a0(fixedBackground ? par.get<T>("a0") : 1.0)` which makes `a0` required (no default) when `fixedBackground` is true. However, `a0` is only actually *used* in `scalefactorinitializer.h` inside the self-consistent (non-fixedBackground) branch, where it always defaults to 1.0. We simplify to always have a default of 1.0, making it optional in all modes.

**Change 2** — Add `a0` member declaration. After `H0` (line 165):
Change `T omegaEoS, H0;` to `T omegaEoS, H0, a0;`

#### 3.2 `include/CosmoInterface/initializers/scalefactorinitializer.h` — NMC-aware Hubble rate

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/initializers/scalefactorinitializer.h`

Add `#include "CosmoInterface/definitions/nonminimalcoupling.h"` after the existing includes.

In the self-consistent expansion branch, **replace** the existing line 50:
```cpp
        model.aDotI = model.fStar / Model::MPl * sqrt((kin + model.pot0) / 3.0); // 1st Friedmann eqn
```

with the following block (introduces `rhoMC` intermediate, `a0` support, and NMC quadratic formula):

```cpp
        auto rhoMC = kin + model.pot0;

        model.aI = rPar.a0;
        model.aDotI = model.fStar / Model::MPl * sqrt(rhoMC / 3.0); // 1st Friedmann eqn

        if constexpr (Model::IsNonMinimallyCoupled) {
          auto A = 1.0 - Total(i, 0, Model::Ns - 1,
              IfElse(Model::NonMinimalCouplings::couples(i, 0_c),
                pow<2>(1.0 / Model::MPl) * model.xis(i, 0_c) * pow<2>(model.fldS0(i)), 0));
          auto B = -2.0 * Total(i, 0, Model::Ns - 1,
              IfElse(Model::NonMinimalCouplings::couples(i, 0_c),
                pow<2>(1.0 / Model::MPl) / model.omegaStar * model.xis(i, 0_c) * model.fldS0(i) * model.piS0(i), 0));
          auto C = -pow<2>(model.fStar / Model::MPl) / 3.0 * rhoMC;

          auto Delta = pow<2>(B) - 4 * A * C;
          auto H1 = (-B + sqrt(Delta)) / 2.0 / A;

          model.aI = rPar.a0;
          model.aDotI = H1;
          model.piAI = model.aDotI;
          model.RI = NonMinimalCoupling::R(model);
        }
```

The existing comments (lines 51-53) about initial gradients being zero should remain after this block.

#### 3.3 `include/CosmoInterface/initializers/modelinitializer.h` — Compute R before Hubble constraint

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/initializers/modelinitializer.h`

Add `#include "CosmoInterface/definitions/nonminimalcoupling.h"` to includes.

After `Averages::setAllAverages(model);` (line 59) and before the Hubble constraint correction, add:
```cpp
        if constexpr (Model::IsNonMinimallyCoupled) model.RI = NonMinimalCoupling::R(model);
```

#### 3.4 `include/CosmoInterface/measurements/energiesmeasurer.h` — NMC energy columns

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/measurements/energiesmeasurer.h`

After the potential energy block (after line 85 `Etot += potTerm;`), add NMC energy output **only for NMC models**:
```cpp
      if constexpr (Model::IsNonMinimallyCoupled) {
        auto rhoNMC1 = Energies::rhoNMCAv1(model);
        auto rhoNMC2 = Energies::rhoNMCAv2(model);
        auto rhoNMC = Energies::rhoNMCAv(model);
        energies.addAverage(rhoNMC1);
        energies.addAverage(rhoNMC2);
        energies.addAverage(rhoNMC);
        Etot += rhoNMC;
      }
```

In `getEnergyHeaders()`, add NMC column headers before `E_tot` **only for NMC models** (matching the conditional data output above so non-NMC models don't get extra zero columns):
```cpp
      if constexpr (Model::IsNonMinimallyCoupled) {
        ret.emplace_back("rhoNMC1");
        ret.emplace_back("rhoNMC2");
        ret.emplace_back("rhoNMC");
      }
```

#### 3.5 `include/CosmoInterface/measurements/scalefactormeasurer.h` — R column

**File**: `cosmolattice_GPU_impl/include/CosmoInterface/measurements/scalefactormeasurer.h`

In constructor, change the column headers to conditionally include `R`:
```cpp
          scaleOut(filesManager, "scale_factor", model.getToolBox()->amIRoot(), append,
                   Model::IsNonMinimallyCoupled
                     ? std::vector<std::string>{"t", "a", "aDot", "H", "R"}
                     : std::vector<std::string>{"t", "a", "aDot", "H"},
                   !expansion)
```

In `measure()`, after the Hubble parameter line (line 40: `scaleOut.addAverage(model.aDotI / model.aI);`) but still **inside** the `if (expansion)` block, add:
```cpp
        if constexpr (Model::IsNonMinimallyCoupled) scaleOut.addAverage(model.RI);
```

#### 3.6 New file: `models/lphi4NonMinimal.h`

**Action**: Copy from private repo and adapt to GPU impl conventions.

**File**: `cosmolattice_GPU_impl/models/lphi4NonMinimal.h`

The model file is relatively self-contained. Key adaptation: GPU impl uses `#include "CosmoInterface/cosmointerface.h"` — need to check if this path exists in GPU impl. Check the existing `lphi4.h` model for the correct include path.

```cpp
#ifndef LPHI4NONMINIMAL_H
#define LPHI4NONMINIMAL_H

#include "CosmoInterface/cosmointerface.h"

namespace TempLat
{
    struct ModelPars : public TempLat::DefaultModelPars {
        static constexpr size_t NScalars = 2;
        static constexpr size_t NPotTerms = 2;

        // Non-minimal coupling: only the second scalar (index 1) couples to gravity
        typedef CouplingsManager<NScalars, 1, false, true> NonMinimalCouplings;
    };

  #define MODELNAME lphi4NonMinimal

  template<class R>
  using Model = MakeModel(R, ModelPars);

  class MODELNAME : public Model<MODELNAME>
  {
  private:
    double g, lambda, q;

  public:
    // NDim must be extracted before use in constructor signature (matches lphi4.h pattern)
    static constexpr size_t NDim = Model<MODELNAME>::NDim;

    MODELNAME(ParameterParser& parser, RunParameters<double>& runPar,
              device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL))
    {
        lambda = parser.get<double>("lambda");
        q = parser.get<double>("q");
        g = sqrt(q * lambda);

        fldS0 = parser.get<double, 2>("initial_amplitudes");
        piS0 = parser.get<double, 2>("initial_momenta", {0, 0});

        alpha = 1;
        fStar = fldS0[0];
        omegaStar = sqrt(lambda) * fStar;

        setInitialPotentialAndMassesFromPotential();
    }

    auto potentialTerms(Tag<0>)
    {
        return 0.25 * pow<4>(fldS(0_c));
    }

    auto potentialTerms(Tag<1>)
    {
        return 0.5 * q * pow<2>(fldS(0_c) * fldS(1_c));
    }

    auto potDeriv(Tag<0>)
    {
        return pow<3>(fldS(0_c)) + q * fldS(0_c) * pow<2>(fldS(1_c));
    }

    auto potDeriv(Tag<1>)
    {
        return q * fldS(1_c) * pow<2>(fldS(0_c));
    }

    auto potDeriv2(Tag<0>)
    {
        return 3 * pow<2>(fldS(0_c)) + q * pow<2>(fldS(1_c));
    }

    auto potDeriv2(Tag<1>)
    {
        return q * pow<2>(fldS(0_c));
    }
  };
}

#endif
```

**NOTE**: The constructor signature matches the GPU impl convention from `lphi4.h`: uses `device::memory::host_ptr<MemoryToolBox<NDim>>` (not `std::shared_ptr<MemoryToolBox>`), and extracts `NDim` as a static constexpr before the constructor.

#### 3.7 New file: `models/parameter-files/lphi4NonMinimal.in`

**File**: `cosmolattice_GPU_impl/models/parameter-files/lphi4NonMinimal.in`

Copy directly from private repo:
```ini
#Output
outputfile = ./

#Evolution
expansion = true
evolver = RK2
xis =10.0

#Lattice
N = 32
dt = 0.01
kIR = 0.75

#Times
tOutputFreq = 0.1
tOutputInfreq = 1
tMax = 50

#IC
baseSeed = 10
kCutOff = 1.75
initial_amplitudes = 7.42675e18 0
initial_momenta = -6.2969e30 0

#Model Parameters
lambda = 9e-14
q = 100
```

### Success Criteria:

#### Automated Verification:
- [x] `cmake -DMODEL=lphi4 ../ && make cosmolattice` compiles and runs (regression test)
- [x] `cmake -DMODEL=lphi4NonMinimal ../ && make cosmolattice` compiles
- [x] `./lphi4NonMinimal input=../models/parameter-files/lphi4NonMinimal.in` runs without crashes

#### Manual Verification:
- [x] Check `energies` output file has `rhoNMC1`, `rhoNMC2`, `rhoNMC` columns
- [x] Check `scale_factor` output file has `R` column for NMC model
- [x] Verify energy conservation check passes for `lphi4NonMinimal` (Friedmann equation satisfied — relative error ~10⁻⁶ early, ~3×10⁻⁴ at t=50, normal for RK2 with dt=0.01)
- [ ] Compare NMC results with private repo output for same parameters

**Implementation Note**: After completing this phase and all automated verification passes, pause here for manual confirmation from the human that the manual testing was successful before proceeding.

---

## Summary: All Files to Modify/Create

### New Files (3)
| File                                                      | Source                        |
| --------------------------------------------------------- | ----------------------------- |
| `include/CosmoInterface/definitions/nonminimalcoupling.h` | Adapted from private repo     |
| `models/lphi4NonMinimal.h`                                | Adapted from private repo     |
| `models/parameter-files/lphi4NonMinimal.in`               | Direct copy from private repo |

### Modified Files (15)
| File                                                             | Phase | Changes                                                              |
| ---------------------------------------------------------------- | ----- | -------------------------------------------------------------------- |
| `include/CosmoInterface/couplingsmanager.h`                      | 1     | Add `sizeof...(Bools)==0` guards in `operator()` and `couples()`     |
| `include/CosmoInterface/evolvers/kernels/kernelstypes.h`         | 1     | Add `tMinust0` member + `cache(model, tIn)` overload to `EoM<T>`     |
| `include/CosmoInterface/abstractmodel.h`                         | 1     | Add `NonMinimalCouplings` template param, NMC members, `xis` parsing |
| `include/CosmoInterface/definitions/averages.h`                  | 2     | Add NMC per-field averages block                                     |
| `include/CosmoInterface/definitions/energies.h`                  | 2     | Add `rhoNMCAv1/2/Av`, `rhoMinimal`, `pMinimal`, modify `rho()`       |
| `include/CosmoInterface/evolvers/kernels/scalefactorkernels.h`   | 2     | NMC branch in `get()` using `NonMinimalCoupling::R()`                |
| `include/CosmoInterface/definitions/axioncouplings.h`            | 2     | Add `tMinust0` parameter to signatures (keep physics unchanged)      |
| `include/CosmoInterface/evolvers/kernels/scalarsingletkernels.h` | 2     | Add NMC source term + pass `tMinust0`                                |
| `include/CosmoInterface/evolvers/kernels/u1kernels.h`            | 2     | Extract and pass `tMinust0`                                          |
| `include/CosmoInterface/evolvers/rk2nstorage.h`                  | 2     | Pass `tMinust0` to `cache()`, activate NMC `advanceScaleFactor`      |
| `include/CosmoInterface/runparameters.h`                         | 3     | Add `a0` parameter                                                   |
| `include/CosmoInterface/initializers/scalefactorinitializer.h`   | 3     | NMC Hubble rate quadratic formula + `a0`                             |
| `include/CosmoInterface/initializers/modelinitializer.h`         | 3     | Compute `RI` before Hubble constraint                                |
| `include/CosmoInterface/measurements/energiesmeasurer.h`         | 3     | Add NMC energy columns                                               |
| `include/CosmoInterface/measurements/scalefactormeasurer.h`      | 3     | Add `R` column                                                       |

### Files NOT modified (Group 2 exclusions)
- `initialconditionstype.h` — No `BunchDavisTransverseU1` or `fldS` enum
- `fluctuationsgenerator.h` — No Bunch-Davis or external PS IC
- `scalarsingletinitializer.h` — No external PS dispatch
- `u1initializer.h` — No Bunch-Davis U1 init
- `u1measurer.h` — No chiral projector
- `extrafields.h` — No BunchDavis extra field allocation
- `m2phi2_axionU1.h` — No `tNonLinearAxionU1`

## Performance Considerations

- NMC adds per-field averages (`fld2AvSI_i`, `grad2AvSI_i`, etc.) which require additional lattice reductions. For non-NMC models, these are skipped via `if constexpr`.
- The `NonMinimalCoupling::R()` computation is called once per RK stage during scale factor evolution.
- No additional memory allocation for non-NMC models (the `TempLatArray<T, Ns>` members exist but are zero-sized when `Ns=0`).

## References

- Merge guide: `cosmolattice_private/thoughts/shared/research/2026-03-10-branch-divergence-manual-merge-guide.md`
- Source branch: `U1AxionNMCS` (commits `716e6a5f..81c610a7`)
- CosmoLattice NMC theory: ξRφ² coupling to gravity
