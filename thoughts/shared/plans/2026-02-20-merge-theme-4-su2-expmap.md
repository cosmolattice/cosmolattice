# Merge Theme 4: SU(2) Exponential Map & Algebra Utilities

## Overview

This theme adds the SU(2) exponential map (`exp: su(2) → SU(2)`) and its inverse (`log: SU(2) → su(2)`), plus supporting infrastructure: an `acos` operator for the expression template algebra system, and compile-time Kronecker delta / Levi-Civita epsilon symbols. These are needed by the RK2N evolver (Theme 5) for gauge link time-stepping via matrix exponentiation.

## Dependencies

- **Theme 3 (SU(2) 4-component storage)**: Must be complete. ✅ Done (`1dd14f6d`)
- No other theme dependencies.

## Files to Process

### Pass-Through Files

| File (master path) | GPU_impl target path | Notes |
|---|---|---|
| n/a | — | No pure pass-through files in this theme |

### Files to Adapt

| File (master) | GPU_impl target | What to port | Adaptation needed |
|---|---|---|---|
| `helpers/paulivectorsalgebra.h` | same | `expmap_inplace` static method | DEVICE annotations, `device::array` constraint, `device::` math, numeric guard instead of `AlmostEqual` |
| `operators/acos.h` | same (new) | `ACos<T>` unary operator + free function | eval-based pattern (not get-based), DEVICE annotations, C++20 concepts, strip TDD |
| `constants/symbols.h` | same (new) | Kronecker delta + Levi-Civita epsilon | Strip TDD boilerplate, strip unconditional `_test.h` include, add `constexpr` |
| `su2algebra/su2expmap.h` | same (new) | `SU2ExpMap<R>` expression template | Variadic eval (not ptrdiff_t), DEVICE annotations, concepts, `device::array`, strip TDD |
| `su2algebra/su2expmapinv.h` | same (new) | `SU2ExpMapInv<R>` expression template | Same as su2expmap + device-compatible safe-divide guard, strip TDD |
| `su2algebra/su2algebra.h` | same | Add 2 `#include` lines | Trivial append |

### Files to Skip

| File | Reason |
|---|---|
| (none) | |

### Test File Migration

| Master `_test.h` | Action | GPU_impl `.cpp` target | Notes |
|---|---|---|---|
| `paulivectorsalgebra_test.h` | Move expmap_inplace test to existing .cpp | `src/tests/.../helpers/paulivectorsalgebra.cpp` | Add test block after multiply_inplace test |
| `su2expmap_test.h` | Move logic to new .cpp | `src/tests/.../su2algebra/su2expmap.cpp` | Use real SU2LieAlgebraField + SU2Field instead of mock Dummy |
| `su2expmapinv_test.h` | Move logic to new .cpp | `src/tests/.../su2algebra/su2expmapinv.cpp` | Round-trip test with real SU2Field objects |
| `symbols_test.h` | Move logic to new .cpp | `src/tests/.../constants/symbols.cpp` | 7 epsilon assertions using `eval(0)` instead of `get(0)` |
| `acos_test.h` | Move logic to new .cpp | `src/tests/.../operators/acos.cpp` | Mock class uses `eval()` instead of `get()` |

### Headers Needing TEMPLATTEST Cleanup

> Not applicable — all files in this theme are either new (created clean) or already clean in GPU_impl.

## Implementation Steps

### Step 1: Add `expmap_inplace` to `paulivectorsalgebra.h`

**File**: `src/include/TempLat/lattice/algebra/su2algebra/helpers/paulivectorsalgebra.h`

Add a second static method to `PauliVectorsAlgebra`, after the existing `multiply_inplace`. The method computes `exp(i σ_a x_a)` from a 3-component algebra vector to a 4-component group quaternion.

```cpp
template <typename ResArray, typename AlgArray>
  requires requires(ResArray r, AlgArray a) {
    r[0]; r[1]; r[2]; r[3];
    a[0]; a[1]; a[2];
  }
DEVICE_FORCEINLINE_FUNCTION static void expmap_inplace(ResArray &res, const AlgArray &alg)
{
  auto a = device::sqrt(alg[0] * alg[0] + alg[1] * alg[1] + alg[2] * alg[2]);
  res[0] = device::cos(a);
  auto sina = device::sin(a);
  // Guard: sin(a)/a → 1 as a → 0. Use direct comparison (device-compatible).
  if (a > decltype(a)(1e-15)) {
    auto ratio = sina / a;
    res[1] = alg[0] * ratio;
    res[2] = alg[1] * ratio;
    res[3] = alg[2] * ratio;
  } else {
    res[1] = alg[0];
    res[2] = alg[1];
    res[3] = alg[2];
  }
}
```

**Key differences from master**: Two template parameters (ResArray[4], AlgArray[3]) with C++20 concept constraint instead of `std::array<T,4>` / `std::array<T,3>`. Device-compatible `> 1e-15` guard replaces `AlmostEqual(a, 0.0)`. No `power.h` include needed (uses `x*x` instead of `pow<2>`).

### Step 2: Create `acos.h` operator

**File**: `src/include/TempLat/lattice/algebra/operators/acos.h` (new)

Follow `cosine.h` pattern exactly. Key elements:

- `using device::acos;` at namespace scope (brings device-compatible acos into TempLat)
- `Operators::ACos<T> : public UnaryOperator<T>` with:
  - `eval(const IDX&...idx)` returning `acos(DoEval::eval(mR, idx...))` (not `get()`)
  - `d(const U& other)` returning `-GetDeriv::get(mR, other) * (1.0 / sqrt(1 - pow<2>(mR)))` (symbolic derivative)
  - `DEVICE_FUNCTION` constructor, `DEVICE_FORCEINLINE_FUNCTION` eval
- Free function `acos(T a) requires ConditionalUnaryGetter<T>`
- Special case `acos(OneType) → ZeroType()` (since acos(1) = 0)

**Includes needed**: `conditionalunarygetter.h`, `onetype.h`, `zerotype.h`, `multiply.h`, `squareroot.h`, `power.h`, `divide.h`, `unaryoperator.h`, `getderiv.h`.

### Step 3: Create `symbols.h` constants

**File**: `src/include/TempLat/lattice/algebra/constants/symbols.h` (new)

Compile-time Kronecker delta and Levi-Civita epsilon using tag dispatch. Adapted from master with:

- Removed `tdd.h` include
- Removed unconditional `_test.h` include at bottom
- Removed TDD tester class
- Added `constexpr` to all function definitions
- Added `DEVICE_FORCEINLINE_FUNCTION` to all functions (these may be called in device kernels via axion coupling code in Theme 8)

The file defines:
- `Symbols::delta(Tag<I>, Tag<J>) → ZeroType()` (general case)
- `Symbols::delta(Tag<I>, Tag<I>) → OneType()` (diagonal case)
- `Symbols::epsilon(Tag<I>, Tag<J>, Tag<K>) → ZeroType()` (general/degenerate)
- 6 explicit permutation overloads for (1,2,3) → `OneType()` / `-OneType()`

### Step 4: Create `su2expmap.h`

**File**: `src/include/TempLat/lattice/algebra/su2algebra/su2expmap.h` (new)

Expression-template class `SU2ExpMap<R>` mapping su(2) algebra → SU(2) group. Follow `su2dagger.h` as the reference GPU_impl SU2 unary operator.

Structure:
- `class SU2ExpMap : public SU2UnaryOperator<R>`
- `using SV = typename SU2GetGetReturnType<R>::type`
- `using SU2UnaryOperator<R>::mR`
- **Lazy path** (symbolic, used when building expression trees):
  - `SU2Get(Tag<0>)`: `cos(sqrt(pow<2>(mR.SU2Get(1_c)) + pow<2>(mR.SU2Get(2_c)) + pow<2>(mR.SU2Get(3_c))))`
  - `SU2Get(Tag<N>)` for N=1..3: `mR.SU2Get(t) / a * sin(a)` where `a = sqrt(...)`
- **Eager path** (per-site evaluation, used in lattice kernels):
  - `eval(const IDX&...idx)`: calls `DoEval::eval(mR, idx...)` → gets `device::array<SV,4>`, extracts algebra part `{c[1],c[2],c[3]}` into `device::array<SV,3>`, calls `PauliVectorsAlgebra::expmap_inplace(result, cL)`, returns `device::array<SV,4>`
- Free function `exp(const R& r) requires HasSU2Get<R>` → `SU2ExpMap<R>(r)`

**Includes**: `hassu2get.h`, `tagliteral.h`, `paulivectorsalgebra.h`, `cosine.h`, `sine.h`, `squareroot.h`, `power.h`, `divide.h`, `su2unaryoperator.h`, `su2getgetreturntype.h`, `doeval.h`, `isvariadicindex.h`, `device.h`.

### Step 5: Create `su2expmapinv.h`

**File**: `src/include/TempLat/lattice/algebra/su2algebra/su2expmapinv.h` (new)

Expression-template class `SU2ExpMapInv<R>` mapping SU(2) group → su(2) algebra (logarithmic map).

Structure:
- `class SU2ExpMapInv : public SU2UnaryOperator<R>`
- **Lazy path**:
  - `SU2Get(Tag<0>)`: `ZeroType()` (algebra element has c0=0)
  - `SU2Get(Tag<N>)` for N=1..3: `mR.SU2Get(t) * acos(mR.SU2Get(0_c)) / sin(acos(mR.SU2Get(0_c)))`
  - `SU2LieAlgebraGet(Tag<N>)`: `2 * SU2Get(t)` (sigma/2 normalization)
- **Eager path**:
  - `eval(const IDX&...idx)`: calls `DoEval::eval(mR, idx...)`, computes `a = device::acos(c[0])`, `sina = device::sin(a)`, safe-divide guard `(sina*sina > SV(1e-30)) ? a/sina : SV(0)`, returns `{0, c[1]*ratio, c[2]*ratio, c[3]*ratio}`
- `static constexpr size_t numberToSkipAsTuple = 1` (result is algebra, skip c0 in tuple iteration)
- Free function `expinv(const R& r) requires HasSU2Get<R>` → `SU2ExpMapInv<R>(r)`

**Includes**: Same as su2expmap plus `acos.h` and `zerotype.h`.

### Step 6: Update `su2algebra.h` umbrella

**File**: `src/include/TempLat/lattice/algebra/su2algebra/su2algebra.h`

Add two includes at the end of the file:
```cpp
#include "TempLat/lattice/algebra/su2algebra/su2expmap.h"
#include "TempLat/lattice/algebra/su2algebra/su2expmapinv.h"
```

### Step 7: Add expmap_inplace test to paulivectorsalgebra.cpp

**File**: `src/tests/TempLat/lattice/algebra/su2algebra/helpers/paulivectorsalgebra.cpp`

After the existing `multiply_inplace` test block, add:
```cpp
device::array<double, 3> D = {1, 2, 3};
device::array<double, 4> e;
PauliVectorsAlgebra::expmap_inplace(e, D);
tdd.verify(AlmostEqual(e[0], -0.82529906208));
tdd.verify(AlmostEqual(e[1], -0.15092132722));
tdd.verify(AlmostEqual(e[2], -0.30184265444));
tdd.verify(AlmostEqual(e[3], -0.45276398166));
```

### Step 8: Create su2expmap.cpp test

**File**: `src/tests/TempLat/lattice/algebra/su2algebra/su2expmap.cpp` (new)

Test approach: use real `SU2LieAlgebraField` and `SU2Field` objects. Set algebra components to (1.0, 2.0, 3.0) uniformly, compute `result = exp(algebra)`, read back host views and verify against known values.

```
- Create MemoryToolBox<3>(N=16, nGhostCells=1)
- Create SU2LieAlgebraField algebra("test_alg", toolBox, latPar)
- Set algebra(1_c) = 1.0, algebra(2_c) = 2.0, algebra(3_c) = 3.0
- Create SU2Field result("test_res", toolBox)
- Assign result = exp(algebra)
- Get host views for all 4 components
- Verify at one representative site: c0 ≈ -0.82530, c1 ≈ -0.15092, c2 ≈ -0.30184, c3 ≈ -0.45276
```

### Step 9: Create su2expmapinv.cpp test

**File**: `src/tests/TempLat/lattice/algebra/su2algebra/su2expmapinv.cpp` (new)

Test approach: round-trip test. Create SU2Field with known components, apply `exp(expinv(U))`, verify output matches input.

```
- Create MemoryToolBox<3>
- Create SU2Field U with c1=0.23, c2=0.11, c3=0.4, then unitarize() to set c0
- Create SU2Field roundtrip
- Assign roundtrip = exp(expinv(U))
- Verify all 4 components match U at a representative site (within AlmostEqual tolerance)
```

### Step 10: Create symbols.cpp test

**File**: `src/tests/TempLat/lattice/algebra/constants/symbols.cpp` (new)

Port the 7 epsilon assertions from master. Use `eval(0)` instead of `get(0)`:

```cpp
tdd.verify(Symbols::epsilon(Tag<1>(), Tag<2>(), Tag<3>()).eval(0) == 1);
tdd.verify(Symbols::epsilon(Tag<3>(), Tag<3>(), Tag<3>()).eval(0) == 0);
// ... etc for all 7 cases
```

### Step 11: Create acos.cpp test

**File**: `src/tests/TempLat/lattice/algebra/operators/acos.cpp` (new)

Port from master's `acos_test.h`. The mock class uses `eval()` instead of `get()`:

```cpp
class myClass {
public:
  DEVICE_FUNCTION myClass(int b) : a(b) {}
  DEVICE_FORCEINLINE_FUNCTION auto eval(const double &i) const { return a; }
private:
  double a;
};
myClass a(0.3);  // note: constructor takes int, but stores double
tdd.verify(AlmostEqual(acos(a).eval(0), std::acos(0.3)));
```

Wait — master's myClass constructor takes `double`, not `int`. Use `double`:
```cpp
DEVICE_FUNCTION myClass(double b) : a(b) {}
```

### Step 12: Run cmake configuration

```bash
cd cosmolattice_GPU_impl/debug_build
cmake -DSerial=ON -DMPI=ON -DHDF5=ON -DTESTING=ON \
  -DMYHDF5_PATH=.../MyHDF5Parallel/ \
  -DMYFFTW3_PATH=.../MyLibs/ \
  -DMODEL=lphi4SU2U1 ../
```

This picks up the 5 new/modified test files via the test CMakeLists.txt glob.

### Step 13: Build and verify

```bash
make cosmolattice           # model builds
make test-paulivectorsalgebra && ctest -R paulivectorsalgebra
make test-acos              && ctest -R acos
make test-symbols           && ctest -R symbols
make test-su2expmap         && ctest -R su2expmap
make test-su2expmapinv      && ctest -R su2expmapinv
```

### Step 14: Update merge status

Update `thoughts/shared/merge_triage/status.md`: set theme 4 to **done**, link this plan, add commit hash.

## Verification

### Automated
- [x] Full model build: `make cosmolattice` (lphi4SU2U1)
- [x] `ctest -R paulivectorsalgebra` passes (expmap_inplace)
- [x] `ctest -R acos` passes
- [x] `ctest -R symbols` passes
- [x] `ctest -R su2expmap` passes
- [x] `ctest -R su2expmapinv` passes (round-trip)

### Manual
- [ ] Verify `su2expmap.h` lazy SU2Get path compiles (used when building expression trees without immediate evaluation)
- [ ] Verify `su2expmapinv.h` lazy SU2Get path compiles

## API Impact

- New free function `exp(R)` for types satisfying `HasSU2Get<R>` — maps algebra → group
- New free function `expinv(R)` for types satisfying `HasSU2Get<R>` — maps group → algebra
- New `acos(T)` operator for expression template types satisfying `ConditionalUnaryGetter<T>`
- New `Symbols::delta` and `Symbols::epsilon` compile-time functions
- **No breaking changes** — all additions are new functions/types

## Post-Implementation Checklist

- [ ] Update `cosmolattice_GPU_impl/thoughts/shared/merge_triage/status.md`: set Theme 4 status to **done**, link the plan file, and add the commit hash
