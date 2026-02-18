---
date: 2026-02-18
author: adrien
branch: GPU_impl
topic: "Plan: Externalize all embedded Test methods to dedicated TesterClass"
tags: [plan, testing, tdd, refactoring, TesterClass]
status: draft
research: thoughts/shared/research/2026-02-18-test-method-vs-tester-class-audit.md
---

# Externalize Embedded Test Methods to TesterClass

## Overview

~160 production classes across TempLat and CosmoInterface have `static inline void Test(TDDAssertion& tdd)` declared **inside** their class body (Pattern B, "embedded"). This plan replaces them all with a separate `FooTester` class declared **outside** the production class in the same header (Pattern A, "TesterClass"). The test body in `foo_test.h` and the registration `.cpp` are updated accordingly.

The transformation is fully mechanical. A Python transformation script is written in Phase 1 and then applied subsystem by subsystem in Phases 2–6, with ctest verification after each phase.

## Current State Analysis

Two patterns co-exist in the codebase. Only Pattern A is desired going forward.

**Pattern A — TesterClass (correct, ~24 classes)**
```cpp
// leapfrog.h — after class LeapFrog closes
#ifdef TEMPLATTEST
  class LeapFrogTester { public: static inline void Test(TDDAssertion &tdd); };
#endif

// leapfrog_test.h
inline void TempLat::LeapFrogTester::Test(TempLat::TDDAssertion &tdd) { ... }

// src/tests/.../leapfrog.cpp
TempLat::TDDContainer<TempLat::LeapFrogTester> test;
```

**Pattern B — Embedded (to remove, ~160 classes)**
```cpp
// containsspace.h — inside class ContainsSpace body
#ifdef TEMPLATTEST
public:
  static inline void Test(TDDAssertion &tdd);   // ← inside production class
#endif

// containsspace_test.h
inline void TempLat::ContainsSpace::Test(TempLat::TDDAssertion &tdd) { ... }

// src/tests/.../containsspace.cpp
TempLat::TDDContainer<TempLat::ContainsSpace> test;
```

## Desired End State

- Every production class uses Pattern A
- `grep -rn "TEMPLATTEST" src/include/` returns only TesterClass declarations positioned **after** the production class `};`
- All ctest targets pass (zero regressions)
- Lint check: `grep -rn "static inline void Test" src/include/ | grep -v "Tester"` returns empty

## What We're NOT Doing

- Modifying test body **content** — only the wrapper class changes
- Renaming any files (`foo_test.h`, `foo.cpp` keep their names)
- Touching classes already using Pattern A (listed in research doc §"Classes Already Conforming")
- Touching `integrationtesting/` helpers (`RandomField`, `VerifyHermitianLayout`, `FiniteDiffVSFFT`, `GradientComponentTest`)
- Touching classes already named `*Tester`

## Transformation Rules

### Design Decisions (resolved)

1. **Templated production classes** (e.g. `template<size_t NDim> class FFTNormalization`):
   Use a **non-templated `struct FooTester`** (consistent with the already-correct `RandomUniformTester`). The test body instantiates the production class with a concrete type. Single `TDDContainer<FooTester>` in `.cpp`.

2. **Integration testing helpers** (`integrationtesting/` subdirectory):
   Skipped — these are already test-only code.

### The Three-File Transformation

For every class `Foo` in file `foo.h`:

#### File 1: `src/include/.../foo.h`

**Remove** the embedded block from inside the production class body:
```cpp
// DELETE these lines (inside class Foo { ... }):
#ifdef TEMPLATTEST
public:
  static inline void Test(TDDAssertion &tdd);
#endif
```

**Add** the TesterClass immediately before the closing `} // namespace TempLat`:
```cpp
// INSERT before } // namespace TempLat:
#ifdef TEMPLATTEST
  class FooTester
  {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
```
For templated production classes, use `struct FooTester` instead of `class FooTester`.

#### File 2: `src/include/.../foo_test.h`

Change the function qualifier from `Foo::Test` to `FooTester::Test`:
```cpp
// Before:
inline void TempLat::Foo::Test(TempLat::TDDAssertion &tdd)

// After:
inline void TempLat::FooTester::Test(TempLat::TDDAssertion &tdd)
```

#### File 3: `src/tests/.../foo.cpp`

Change the container template argument:
```cpp
// Before:
TempLat::TDDContainer<TempLat::Foo> test;

// After:
TempLat::TDDContainer<TempLat::FooTester> test;
```

---

## Phase 1: Transformation Script + ContainsSpace Proof of Concept

### Overview

Write a Python script that automates the 3-file transformation. Validate it manually on `ContainsSpace` (simplest possible case) and run its test before proceeding.

### Script: `tools/externalize_test.py`

The script accepts a list of class descriptors and applies the transformation to all three files. Each descriptor specifies:
- `class_name`: `"ContainsSpace"`
- `tester_name`: `"ContainsSpaceTester"` (always `class_name + "Tester"`)
- `header`: `"src/include/TempLat/util/containsspace.h"`
- `test_header`: `"src/include/TempLat/util/containsspace_test.h"`
- `registration_cpp`: `"src/tests/TempLat/util/containsspace.cpp"`
- `use_struct`: `false` (true for templated production classes)
- `namespace`: `"TempLat"` (always `TempLat` for all classes in this audit)

**Algorithm for `foo.h` transformation:**

1. Read file content.
2. Locate the `#ifdef TEMPLATTEST` block inside the class body. It always matches:
   ```
   \n#ifdef TEMPLATTEST\npublic:\n  static inline void Test(TDDAssertion &tdd);\n#endif\n
   ```
   (with possible whitespace/indentation variants). Remove it.
3. Locate the insertion point: the line matching `} // namespace TempLat` (the namespace-closing brace).
4. Insert the TesterClass block on the line immediately before the namespace close:
   ```cpp
   #ifdef TEMPLATTEST
     class FooTester
     {
     public:
       static inline void Test(TDDAssertion &tdd);
     };
   #endif

   ```
5. Write the modified content back.

**Algorithm for `foo_test.h` transformation:**

Replace the first occurrence of `TempLat::<ClassName>::Test(` with `TempLat::<TesterName>::Test(`.

**Algorithm for `foo.cpp` transformation:**

Replace `TDDContainer<TempLat::<ClassName>>` with `TDDContainer<TempLat::<TesterName>>`.

**Edge cases to handle:**

- `#ifdef TEMPLATTEST` indentation may vary (inside `private:` vs `public:` section)
- Some headers end with `} // namespace TempLat\n\n#endif` — insert the TesterClass before the namespace closing `}`
- Classes inside a namespace with a comment variant like `} // namespace` — use flexible matching: last `}` before `#endif` at end of file

### Proof of Concept: ContainsSpace

Apply the script to `ContainsSpace` manually first, diff the three files to verify correctness, then run:

```
ctest --test-dir debug_build -R test-containsspace
```

Expected: test still passes with zero failures.

Additionally, grep to confirm the embedded pattern is gone:
```
grep -n "TEMPLATTEST" src/include/TempLat/util/containsspace.h
```
Must show only the `LeapFrogTester`-style external declaration, never inside the class body.

### Success Criteria

#### Automated Verification:
- [x] Script exists at `tools/externalize_test.py` and accepts a class descriptor list
- [x] `ContainsSpace` transformed: `grep` confirms no `TEMPLATTEST` inside class body
- [x] `ctest --test-dir debug_build -R test-containsspace` passes

#### Manual Verification:
- [x] Diff of `containsspace.h` matches expected Pattern A structure
- [x] Diff of `containsspace_test.h` shows only the function qualifier change
- [x] Diff of `containsspace.cpp` shows only the template argument change

---

## Phase 2: TempLat/util/ Subsystems

### Overview

Apply the script to all classes in `TempLat/util/`, `TempLat/parameters/`, and `TempLat/session/`. These are simple, low-dependency utility classes — lowest risk.

### Classes to Transform (32 total)

**TempLat/util/** (15 classes):
| Class | Header | Test Header |
|---|---|---|
| `ContainsSpace` | `util/containsspace.h` | `util/containsspace_test.h` |
| `GetCPPTypeName` | `util/getcpptypename.h` | `util/getcpptypename_test.h` |
| `PrettyToString` | `util/prettytostring.h` | `util/prettytostring_test.h` |
| `NumericalIntegrator` | `util/numericalintegrator.h` | `util/numericalintegrator_test.h` |
| `FileToString` | `util/filetostring.h` | `util/filetostring_test.h` |
| `FloatToString` | `util/floattostring.h` | `util/floattostring_test.h` |
| `StringTrimmer` | `util/stringtrimmer.h` | `util/stringtrimmer_test.h` |
| `ParenthesisStripper` | `util/parenthesisstripper.h` | `util/parenthesisstripper_test.h` |
| `Endianness` | `util/endianness.h` | `util/endianness_test.h` |
| `CStyleTime` | `util/cstyletime.h` | `util/cstyletime_test.h` |
| `LoadBalance` | `util/loadbalance.h` | `util/loadbalance_test.h` |
| `Factorize` | `util/factorize.h` | `util/factorize_test.h` |
| `IsInContainer` | `util/isincontainer.h` | `util/isincontainer_test.h` |
| `Exception` | `util/exception.h` | `util/exception_test.h` |
| `StaticWarning` | `util/staticwarning.h` | `util/staticwarning_test.h` |

**TempLat/util/debug/** (3 classes):
`PoorMansProfile`, `MPIDebuggerHanger`, `Stacktrace`

**TempLat/util/hash/** (2 classes):
`KeccakHashBareClass`, `KeccakHash`

**TempLat/util/json/** (1 class):
`FileToJSON`

**TempLat/util/conditionaloutput/** (4 classes — all templated, use `struct`):
`ConditionalFileStream`, `ConditionalStream`, `ConditionalSayShort`, `OutputStream<R>` → `OutputStreamTester`

**TempLat/util/random/** (1 class):
`RandomGaussian`

**TempLat/parameters/** (5 classes — `ParameterGetter<T>`, `MultipleParameterGetter` are templated, use `struct`):
`PairMaker`, `FileReader`, `ParameterParser`, `ParameterGetter`, `MultipleParameterGetter`

**TempLat/session/** (1 class):
`SessionGuard`

### Procedure

1. Add all 32 descriptors to the script's input list.
2. Run `python3 tools/externalize_test.py --phase 2`.
3. Spot-check 3–5 transformed headers visually.

### Success Criteria

#### Automated Verification:
- [x] `ctest --test-dir debug_build -L util` passes (all util tests)
- [x] `ctest --test-dir debug_build -L parameters` passes
- [x] `ctest --test-dir debug_build -L session` passes
- [x] No embedded Test patterns found in Phase 2 classes (verified with Python check)

---

## Phase 3: TempLat/parallel/

### Overview

13 classes spanning MPI and device parallelism infrastructure.

### Classes to Transform (13 total)

| Class | Notes |
|---|---|
| `ThreadSettings` | non-templated |
| `MPIGuard` | non-templated |
| `MPITagsJustCompile` | non-templated |
| `MPITypeConstants` | non-templated |
| `MPIDomainSplit` | non-templated |
| `MPICommReference` | non-templated |
| `MPIAllReduce` | templated → `struct MPIAllReduceTester` |
| `MPISendReceive` | templated → `struct MPISendReceiveTester` |
| `MPICartesianNeighboursSingleDimension` | non-templated |
| `MPICartesianNeighbours` | non-templated |
| `MPICartesianGroup` | non-templated |
| `MPICartesianExchange` | templated → `struct MPICartesianExchangeTester` |
| `DeviceGuard` | non-templated |

### Procedure

Run `python3 tools/externalize_test.py --phase 3`. Spot-check MPI classes since they have slightly different structures.

### Success Criteria

#### Automated Verification:
- [ ] `ctest --test-dir debug_build -L parallel` passes
- [ ] `grep -rn "static inline void Test" src/include/TempLat/parallel/ | grep -v "Tester"` returns empty

---

## Phase 4: TempLat/fft/

### Overview

22 FFT classes, all templated on `NDim`. Per the design decision, all use non-templated `struct FooTester`. The test body in `_test.h` will reference a concrete NDim (e.g. 3) rather than a template parameter.

### Classes to Transform (22 total)

| Class | File |
|---|---|
| `FFTNormalization` | `fft/fftnormalization.h` |
| `FFTMPIDomainSplit` | `fft/fftmpidomainsplit.h` |
| `FFTSessionGuard` | `fft/fftlibraryinterface.h` |
| `FFTWTranspositionFlags` | `fft/external/fftw/fftwtranspositionflags.h` |
| `FFTWHermitianPartners` | `fft/external/fftw/fftwhermitianpartners.h` |
| `FFTMallocFree` | `fft/types/fftmallocfree.h` |
| `FFTWMemoryLayout` | `fft/external/fftw/fftwmemorylayout.h` |
| `FFTWInterface` | `fft/external/fftw/fftwinterface.h` |
| `FFTWGuard` | `fft/external/fftw/fftwguard.h` |
| `FFTWPlanHolder` | `fft/external/fftw/fftwplanholder.h` |
| `FFTWPlanner` | `fft/external/fftw/fftwplanner.h` |
| `ParafaftGuard` | `fft/external/parafaft/parafaftguard.h` |
| `ParafaftInterface` | `fft/external/parafaft/parafaftinterface.h` |
| `ParafaftPlanHolder` | `fft/external/parafaft/parafaftplanholder.h` |
| `ParafaftMemoryLayout` | `fft/external/parafaft/parafaftmemorylayout.h` |
| `ParafaftTranspositionFlags` | `fft/external/parafaft/parafafttranspositionflags.h` |
| `ParafaftPlanner` | `fft/external/parafaft/parafaftplanner.h` |
| `KokkosFFTGuard` | `fft/external/kokkosfft/kokkosfftguard.h` |
| `KokkosFFTInterface` | `fft/external/kokkosfft/kokkosfftinterface.h` |
| `KokkosFFTPlanHolder` | `fft/external/kokkosfft/kokkosfftplanholder.h` |
| `KokkosFFTMemoryLayout` | `fft/external/kokkosfft/kokkosfftmemorylayout.h` |
| `KokkosFFTTranspositionFlags` | `fft/external/kokkosfft/kokkosffttranspositionflags.h` |
| `KokkosFFTPlanner` | `fft/external/kokkosfft/kokkosfftplanner.h` |

Note: `FFTSessionGuard` lives in `fftlibraryinterface.h` — the TesterClass is named `FFTSessionGuardTester`.

### Success Criteria

#### Automated Verification:
- [ ] `ctest --test-dir debug_build -L fft` passes
- [ ] `grep -rn "static inline void Test" src/include/TempLat/fft/ | grep -v "Tester"` returns empty

---

## Phase 5: TempLat/lattice/

### Overview

The largest subsystem — ~63 classes across memory management, ghost cells, field algebra, IO, and measuring tools.

### Subsystem Breakdown

**TempLat/lattice/memory/** (14 classes):
`MemoryBlock`, `MemoryManager`, `MemoryToolBox`, `MemoryLayoutState`, `VerbosityLevels`, `TripleStateLayouts`, `FFTLayoutStruct`, `LayoutStructGlobal`, `LayoutStructLocal`, `LayoutStructLocalTransposed`, `LayoutStruct`, `HermitianPartners`, `HermitianValueAccounting`, `TranspositionMap`

**TempLat/lattice/ghostcells/** (5 classes):
`GhostStateKeeper`, `GhostSubArrayMap`, `GhostSubArray`, `GhostUpdater`, `GhostBuster`

**TempLat/lattice/field/** (4 production classes — skip `integrationtesting/`):
`Field`, `FieldViewConfig`, `FieldViewFourier`, `Flattener`

**TempLat/lattice/IO/** (10 classes):
`FileIO`, `FileLoaderHDF5`, `FileSaverHDF5`, `HDF5Object`, `HDF5Dataset`, `HDF5Group`, `HDF5File`, `HDF5Attribute`, `FileLoaderPureMPI`, `FileSaverPureMPI`

**TempLat/lattice/measuringtools/** (9 classes):
`AveragerHelper`, `ListAverager`, `RadialBinComputer`, `KBins`, `RadialProjectionResult`, `RadialProjectionRebinner`, `RadialProjectionSingleBinAndValue`, `RadialProjectionSingleQuantity`, `RadialProjectionSingleDatum`

**TempLat/lattice/algebra/helpers/** (15 classes):
`DoEval`, `GetEval`, `GetNGrid`, `GetNDim`, `GetDx`, `GetKIR`, `GetDeriv`, `GetVectorSize`, `GetVectorComponent`, `GetString`, `GetToolBox`, `PostGet`, `PreGet`, `GhostsHunter`, `ConfirmSpace`, `ConfirmGhosts`

**TempLat/lattice/algebra/coordinates/** (3 classes, all templated → use `struct`):
`SpatialCoordinate`, `WaveNumber`, `DimensionCountRecorder`

**TempLat/lattice/algebra/operators/** (3 classes):
`BinaryOperator`, `UnaryOperator`, `Shift`

**TempLat/lattice/algebra/listoperators/** (4 classes):
`ListBinaryOperator`, `ListUnaryOperator`, `Derivatives`, `VectorDotter`

**TempLat/lattice/algebra/spatialderivatives/** (8 classes, most templated → use `struct`):
`LatticeLaplacian`, `BackDiff`, `ForwDiff`, `ForwDij`, `NeutDiff`, `NeutDij`, `NormGradientSquare`
Note: `LatticeForwardGradient` is already done (uses `LatticeForwardGradientTester<NDim>`).

**TempLat/lattice/algebra/complexalgebra/** (4 classes):
`ComplexField`, `ComplexFieldOperator`, `ComplexFieldBinaryOperator`, `ComplexFieldUnaryOperator`

**TempLat/lattice/algebra/su2algebra/** (7 classes):
`SU2Operator`, `SU2Field`, `SU2DoubletOperator`, `SU2DoubletUnaryOperator`, `SU2DoubletBinaryOperator`, `PauliVectorsAlgebra`, `ScalarSU2Multiplication`

**TempLat/lattice/** top-level (1 class):
`LatticeParameters`

### Procedure

Run the script in sub-batches by subdirectory. After each sub-batch, compile with `make -C debug_build test-<representative>` to catch issues early.

### Success Criteria

#### Automated Verification:
- [ ] `ctest --test-dir debug_build -L lattice` passes (all lattice subsystem tests)
- [ ] `grep -rn "static inline void Test" src/include/TempLat/lattice/ | grep -v "Tester"` returns empty

---

## Phase 6: CosmoInterface Layer

### Overview

28 production classes in the high-level CosmoInterface layer. Several are templated on the model type.

### Classes to Transform (28 total)

| Class | File | Notes |
|---|---|---|
| `AbstractModel` | `CosmoInterface/abstractmodel.h` | templated → `struct` |
| `RunParameters` | `CosmoInterface/runparameters.h` | templated → `struct` |
| `SimulationManager` | `CosmoInterface/simulationmanager.h` | templated → `struct` |
| `SU2Initializer` | `CosmoInterface/initializers/su2initializer.h` | templated → `struct` |
| `ScaleFactorInitializer` | `CosmoInterface/initializers/scalefactorinitializer.h` | templated → `struct` |
| `ScalarSingletInitializer` | `CosmoInterface/initializers/scalarsingletinitializer.h` | templated → `struct` |
| `U1Initializer` | `CosmoInterface/initializers/u1initializer.h` | templated → `struct` |
| `HubbleConstraint` | `CosmoInterface/definitions/hubbleconstraint.h` | |
| `MatterCurrents` | `CosmoInterface/definitions/mattercurrents.h` | |
| `GaugeDerivatives` | `CosmoInterface/definitions/gaugederivatives.h` | |
| `FieldFunctionals` | `CosmoInterface/definitions/fieldfunctionals.h` | |
| `Averages` | `CosmoInterface/definitions/averages.h` | |
| `Energies` | `CosmoInterface/definitions/energies.h` | |
| `Potential` | `CosmoInterface/definitions/potential.h` | templated → `struct` |
| `GaussLaws` | `CosmoInterface/definitions/gausslaws.h` | templated → `struct` |
| `SU2Kernels` | `CosmoInterface/evolvers/kernels/su2kernels.h` | |
| `SU2DoubletKernels` | `CosmoInterface/evolvers/kernels/su2doubletkernels.h` | |
| `ScaleFactorKernels` | `CosmoInterface/evolvers/kernels/scalefactorkernels.h` | |
| `ScalarSingletKernels` | `CosmoInterface/evolvers/kernels/scalarsingletkernels.h` | |
| `ComplexScalarKernels` | `CosmoInterface/evolvers/kernels/complexscalarkernels.h` | |
| `GWsKernels` | `CosmoInterface/evolvers/kernels/gwskernels.h` | |
| `U1Kernels` | `CosmoInterface/evolvers/kernels/u1kernels.h` | |
| `MeansMeasurer` | `CosmoInterface/measurements/meansmeasurer.h` | templated → `struct` |
| `EnergySnapshotMeasurer` | `CosmoInterface/measurements/energysnapshotmeasurer.h` | templated → `struct` |

### Success Criteria

#### Automated Verification:
- [ ] `ctest --test-dir debug_build -L CosmoInterface` passes
- [ ] `grep -rn "static inline void Test" src/include/CosmoInterface/ | grep -v "Tester"` returns empty

---

## Phase 7: Final Verification

### Overview

Full regression check to confirm no embedded Test patterns remain anywhere and all tests pass.

### Checks

#### Automated Verification:
- [ ] Full ctest run: `ctest --test-dir debug_build` passes with no failures
- [ ] Global lint: `grep -rn "static inline void Test" src/include/ | grep -v "Tester"` returns empty
- [ ] `grep -rn "TDDContainer<TempLat::[A-Z]" src/tests/ | grep -v "Tester"` returns only already-known Pattern A entries (confirm zero remaining Pattern B `.cpp` registrations)
- [ ] Script's `--dry-run` reports 0 remaining classes to transform

---

## Testing Strategy

### Per-phase verification

After each phase, run the ctest label filter for the affected subsystem. Because CMake labels the tests by folder path (set in `src/tests/CMakeLists.txt`), filters are reliable:

```bash
# Examples:
ctest --test-dir debug_build -L util         # all util tests
ctest --test-dir debug_build -L parallel     # parallel tests
ctest --test-dir debug_build -L fft          # FFT tests
ctest --test-dir debug_build -L lattice      # all lattice tests
ctest --test-dir debug_build -L CosmoInterface  # CosmoInterface tests
```

Individual test:
```bash
ctest --test-dir debug_build -R test-containsspace
```

### Regression check (run at any point)

```bash
ctest --test-dir debug_build --output-on-failure
```

### Structural check (no embedded patterns remain)

```bash
# Must return empty when refactoring is complete:
grep -rn "static inline void Test" src/include/ | grep -v Tester
```

---

## Edge Cases and Implementation Notes

1. **`FFTSessionGuard` lives in `fftlibraryinterface.h`** — the file does not follow `foo.h` naming. The script descriptor must specify the exact header path explicitly.

2. **`ParameterGetter` and `MultipleParameterGetter` have no class line listed** in the audit (the `—` entries). This means their class declaration may be spread differently. Inspect these two files before running the script on them.

3. **Classes with multiple namespaces** (if any): verify the namespace structure before running the script. All known classes are in `namespace TempLat`.

4. **`OutputStream<R>`** and other classes where `Test` is in `private:` section (not `public:`): the embedded block begins with just `#ifdef TEMPLATTEST\npublic:\n` — the script regex must match both the `public:` variant and any alternative guard structure.

5. **`StaticIf`** is not in the audit (it was already refactored via `StaticIfTester` per `thoughts/shared/plans/2026-02-18-static-if-to-if-constexpr.md`).

---

## References

- Research document: `thoughts/shared/research/2026-02-18-test-method-vs-tester-class-audit.md`
- Original task: `refactor_tests.md`
- Reference implementation (already correct): `src/include/CosmoInterface/evolvers/leapfrog.h:276`
- Reference test body: `src/include/CosmoInterface/evolvers/leapfrog_test.h`
- Reference `.cpp`: `src/tests/CosmoInterface/evolvers/leapfrog.cpp`
- TDD infrastructure: `src/include/TempLat/util/tdd/tddcontainer.h`
