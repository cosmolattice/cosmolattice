# Clang-Tidy Fixes: Theme 6 — Modernize C++ Syntax

## Overview

Implement fixes for 65 clang-tidy diagnostics across 8 checks, as triaged in
[triage file](../clang_tidy_triage/theme_6.md). All diagnostics are marked **fix** — no
suppressions, deferrals, or investigations.

## Triage Summary

| Check | Diagnostics | Decision | Phase |
|-------|-------------|----------|-------|
| `modernize-return-braced-init-list` | 35 | fix | 1 |
| `modernize-concat-nested-namespaces` | 9 | fix | 2 |
| `modernize-use-nullptr` | 9 | fix | 3 |
| `modernize-use-auto` | 1 | fix | 3 |
| `modernize-use-using` | 6 | fix | 4 |
| `modernize-use-override` | 2 | fix | 4 |
| `modernize-use-std-numbers` | 2 | fix | 4 |
| `modernize-use-integer-sign-comparison` | 1 | fix | 4 |

## What We're NOT Doing

- Nothing deferred or suppressed — all 65 diagnostics are fixed.

---

## Phase 1: Return Braced Init List (35 diagnostics)

### Overview

Replace `return Type(args)` with `return {args}` in return statements where the type is
already known from the function signature. All cases are sentinel types (`ZeroType`,
`OneType`, `IntrinsicScales`) or named types with arguments (`HermitianValueAccounting`).

### Changes

#### `src/include/TempLat/fft/external/fftw/fftwinterface.h:36`

**Diagnostic**: `modernize-return-braced-init-list` — avoid repeating the return type

**Before**:
```cpp
    virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nGridPoints) { return IntrinsicScales(); }
```

**After**:
```cpp
    virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nGridPoints) { return {}; }
```

**Rationale**: Default-constructed return value; type deduced from signature.

---

#### `src/include/TempLat/lattice/algebra/complexalgebra/complexfieldconjugate.h:69,71`

**Diagnostic**: `modernize-return-braced-init-list` — avoid repeating the return type (×2)

**Before**:
```cpp
  DEVICE_FORCEINLINE_FUNCTION
  OneType dagger(OneType) { return OneType(); }
  DEVICE_FORCEINLINE_FUNCTION
  OneType conj(OneType) { return OneType(); }
```

**After**:
```cpp
  DEVICE_FORCEINLINE_FUNCTION
  OneType dagger(OneType) { return {}; }
  DEVICE_FORCEINLINE_FUNCTION
  OneType conj(OneType) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/acos.h:71`

**Before**:
```cpp
  ZeroType acos(OneType a) { return ZeroType(); }
```

**After**:
```cpp
  ZeroType acos(OneType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/add.h:65,69`

**Before**:
```cpp
  DEVICE_FORCEINLINE_FUNCTION auto operator+(const ZeroType a, const ZeroType b) { return ZeroType(); }

  /** @brief Specialize for possible half input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType operator+(const HalfType a, const HalfType b) { return OneType(); }
```

**After**:
```cpp
  DEVICE_FORCEINLINE_FUNCTION auto operator+(const ZeroType a, const ZeroType b) { return ZeroType(); }

  /** @brief Specialize for possible half input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType operator+(const HalfType a, const HalfType b) { return {}; }
```

**Note**: Line 65 returns `auto`, so braced-init-list cannot be used (return type not deducible from `{}`). Only line 69 is changed.

---

#### `src/include/TempLat/lattice/algebra/operators/cosh.h:67`

**Before**:
```cpp
  OneType cosh(ZeroType a) { return OneType(); }
```

**After**:
```cpp
  OneType cosh(ZeroType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/cosine.h:69`

**Before**:
```cpp
  OneType cos(ZeroType a) { return OneType(); }
```

**After**:
```cpp
  OneType cos(ZeroType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/exponential.h:70`

**Before**:
```cpp
  OneType exp(ZeroType a) { return OneType(); }
```

**After**:
```cpp
  OneType exp(ZeroType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/heavisidestepfunction.h:56,60`

**Before**:
```cpp
  OneType heaviside(ZeroType a) { return OneType(); }

  /** @brief Specialize for possible unit input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType heaviside(OneType a) { return OneType(); }
```

**After**:
```cpp
  OneType heaviside(ZeroType a) { return {}; }

  /** @brief Specialize for possible unit input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType heaviside(OneType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/log.h:70`

**Before**:
```cpp
  ZeroType log(OneType a) { return ZeroType(); }
```

**After**:
```cpp
  ZeroType log(OneType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/power.h:99,101`

**Before**:
```cpp
  template <ptrdiff_t N> DEVICE_FORCEINLINE_FUNCTION ZeroType pow(ZeroType) { return ZeroType(); }

  template <typename T> DEVICE_FORCEINLINE_FUNCTION OneType pow(const T &a, ZeroType b) { return OneType(); }
```

**After**:
```cpp
  template <ptrdiff_t N> DEVICE_FORCEINLINE_FUNCTION ZeroType pow(ZeroType) { return {}; }

  template <typename T> DEVICE_FORCEINLINE_FUNCTION OneType pow(const T &a, ZeroType b) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/sine.h:65`

**Before**:
```cpp
  ZeroType sin(ZeroType a) { return ZeroType(); }
```

**After**:
```cpp
  ZeroType sin(ZeroType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/operators/sinh.h:67`

**Before**:
```cpp
  ZeroType sinh(ZeroType a) { return ZeroType(); }
```

**After**:
```cpp
  ZeroType sinh(ZeroType a) { return {}; }
```

---

#### `src/include/TempLat/lattice/algebra/su2algebra/su2generators.h` (16 diagnostics)

**Diagnostic**: `modernize-return-braced-init-list` — lines 26, 28, 30, 32, 40, 42, 44, 46, 55, 57, 59, 61, 70, 72, 74, 76

All follow the same pattern: `return OneType()` → `return {}` and `return ZeroType()` → `return {}`.

**Struct `Sigma0` (lines 26, 28, 30, 32)**:

**Before**:
```cpp
      static constexpr OneType SU2Get(Tag<0> t) { return OneType(); }
      ...
      static constexpr ZeroType SU2Get(Tag<1> t) { return ZeroType(); }
      ...
      static constexpr ZeroType SU2Get(Tag<2> t) { return ZeroType(); }
      ...
      static constexpr ZeroType SU2Get(Tag<3> t) { return ZeroType(); }
```

**After**:
```cpp
      static constexpr OneType SU2Get(Tag<0> t) { return {}; }
      ...
      static constexpr ZeroType SU2Get(Tag<1> t) { return {}; }
      ...
      static constexpr ZeroType SU2Get(Tag<2> t) { return {}; }
      ...
      static constexpr ZeroType SU2Get(Tag<3> t) { return {}; }
```

**Struct `Sigma1` (lines 40, 42, 44, 46)** — same pattern, `{0,1,0,0}` identity matrix row.

**Struct `Sigma2` (lines 55, 57, 59, 61)** — same pattern, `{0,0,1,0}` identity matrix row.

**Struct `Sigma3` (lines 70, 72, 74, 76)** — same pattern, `{0,0,0,1}` identity matrix row.

**Implementation strategy**: Use `replace_all` on the file to replace all `{ return OneType(); }` with `{ return {}; }` and all `{ return ZeroType(); }` with `{ return {}; }`. This covers all 16 diagnostics in one pass.

---

#### `src/include/TempLat/lattice/algebra/su2algebra/su2liealgebrafield.h:46`

**Before**:
```cpp
    DEVICE_FORCEINLINE_FUNCTION ZeroType operator()(Tag<0> t) const { return ZeroType(); }
```

**After**:
```cpp
    DEVICE_FORCEINLINE_FUNCTION ZeroType operator()(Tag<0> t) const { return {}; }
```

---

#### `src/include/TempLat/lattice/memory/memorylayouts/hermitianpartners.h:139,145`

**Before**:
```cpp
        return HermitianValueAccounting(real, imaginary);
      } else // if (mode == HermitianPartnersMode::none)
      {
        device::Idx vol = 1;
        for (device::Idx x : mNGrid)
          vol *= x;
        return HermitianValueAccounting(vol, 0);
```

**After**:
```cpp
        return {real, imaginary};
      } else // if (mode == HermitianPartnersMode::none)
      {
        device::Idx vol = 1;
        for (device::Idx x : mNGrid)
          vol *= x;
        return {vol, 0};
```

---

### Success Criteria

#### Automated Verification
- [x] Project compiles: `cd debug_build && cmake <flags> ../ && make cosmolattice`
- [ ] Re-run clang-tidy for this check shows 0 remaining diagnostics:
      `python3 tools/run_clang_tidy.py --checks 'modernize-return-braced-init-list' --category src`
- [x] No regressions in existing tests: `cd debug_build && ctest`

#### Manual Verification
- [x] Verify `hermitianpartners.h` changes: the `HermitianValueAccounting` constructor takes `(Idx, Idx)`, so `{real, imaginary}` and `{vol, 0}` are correct.
- [x] Verify `add.h:65` was NOT changed (returns `auto`, braced-init-list invalid).

**Pause for review before proceeding to Phase 2.**

---

## Phase 2: Concatenate Nested Namespaces (9 diagnostics)

### Overview

Merge nested `namespace A { namespace B {` declarations into C++17 `namespace A::B {`
syntax. Content indentation is left as-is (standard practice for this mechanical refactoring).

### Changes

#### `src/include/CosmoInterface/evolvers/kernels/kernels.h:16`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::Kernels`

**Before**:
```cpp
namespace TempLat {

    namespace Kernels{
        ...
    }
}
```

**After**:
```cpp
namespace TempLat::Kernels {
        ...
}
```

Three edits:
1. Replace `namespace TempLat {\n\n    namespace Kernels{` with `namespace TempLat::Kernels {`
2. Replace closing `    }\n}` (lines 87-88) with `} // namespace TempLat::Kernels`

---

#### `src/include/CosmoInterface/evolvers/kernels/kernelstypes.h:13`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::KernelsTypes`

**Before**:
```cpp
namespace TempLat {


    /** \brief A mechanism to define different kernels for the same solver.
     ...
     **/

    namespace KernelsTypes {
        ...
    }



} /* TempLat */
```

**After**:
```cpp
namespace TempLat::KernelsTypes {


    /** \brief A mechanism to define different kernels for the same solver.
     ...
     **/

        ...



} // namespace TempLat::KernelsTypes
```

Three edits:
1. Replace `namespace TempLat {\n` with `namespace TempLat::KernelsTypes {\n`
2. Remove `    namespace KernelsTypes {` line (22)
3. Replace closing `    }\n\n\n\n} /* TempLat */` (lines 39-43) with `\n\n\n} // namespace TempLat::KernelsTypes`

---

#### `src/include/TempLat/parallel/device_iteration.h:26`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::device::iteration`

**Before**:
```cpp
namespace TempLat
{
  namespace device
  {
    namespace iteration
    {
      ...
    } // namespace iteration
  } // namespace device
} // namespace TempLat
```

**After**:
```cpp
namespace TempLat::device::iteration
{
      ...
} // namespace TempLat::device::iteration
```

Three edits:
1. Replace lines 26-31 with `namespace TempLat::device::iteration\n{`
2. Replace lines 40-42 with `} // namespace TempLat::device::iteration`

---

#### `src/include/TempLat/parallel/device_memory.h:44`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::device::memory`

**Before** (lines 44-49):
```cpp
namespace TempLat
{
  namespace device
  {
    namespace memory
    {
```

**After**:
```cpp
namespace TempLat::device::memory
{
```

Closing (lines 201-203):
```cpp
    } // namespace memory
  } // namespace device
} // namespace TempLat
```
→
```cpp
} // namespace TempLat::device::memory
```

---

#### `src/include/TempLat/parallel/devices/kokkos/kokkos.h:55`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::device_kokkos`

**Before** (lines 55-58):
```cpp
namespace TempLat
{
  namespace device_kokkos
  {
```

**After**:
```cpp
namespace TempLat::device_kokkos
{
```

Closing (lines 162-163):
```cpp
  } // namespace device_kokkos
} // namespace TempLat
```
→
```cpp
} // namespace TempLat::device_kokkos
```

---

#### `src/include/TempLat/parallel/devices/kokkos/kokkos_iteration.h:12`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::device_kokkos::iteration`

**Before** (lines 12-17):
```cpp
namespace TempLat
{
  namespace device_kokkos
  {
    namespace iteration
    {
```

**After**:
```cpp
namespace TempLat::device_kokkos::iteration
{
```

Closing (lines 85-87):
```cpp
    } // namespace iteration
  } // namespace device_kokkos
} // namespace TempLat
```
→
```cpp
} // namespace TempLat::device_kokkos::iteration
```

---

#### `src/include/TempLat/parallel/devices/kokkos/kokkos_memory.h:18`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::device_kokkos::memory`

**Before** (lines 18-23):
```cpp
namespace TempLat
{
  namespace device_kokkos
  {
    namespace memory
    {
```

**After**:
```cpp
namespace TempLat::device_kokkos::memory
{
```

Closing (lines 174-176):
```cpp
    } // namespace memory
  } // namespace device_kokkos
} // namespace TempLat
```
→
```cpp
} // namespace TempLat::device_kokkos::memory
```

---

#### `src/include/TempLat/parallel/devices/kokkos/session/kokkos_guard.h:13`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::device_kokkos`

**Before** (lines 13-16):
```cpp
namespace TempLat
{
  namespace device_kokkos
  {
```

**After**:
```cpp
namespace TempLat::device_kokkos
{
```

Closing (lines 69-70):
```cpp
  } // namespace device_kokkos
} // namespace TempLat
```
→
```cpp
} // namespace TempLat::device_kokkos
```

---

#### `src/include/TempLat/util/hash/libkeccak_twoendians.h:25`

**Diagnostic**: `modernize-concat-nested-namespaces` — `TempLat::KECCAK`

**Before** (lines 25-28):
```cpp
namespace TempLat
{
  namespace KECCAK
  {
```

**After**:
```cpp
namespace TempLat::KECCAK
{
```

Closing (lines 60-61):
```cpp
  } // namespace KECCAK
} // namespace TempLat
```
→
```cpp
} // namespace TempLat::KECCAK
```

**Note**: The inner namespaces `CLAT_LITTLE_ENDIAN` and `CLAT_BIG_ENDIAN` are NOT merged
(they have intervening comments/preprocessor directives between them).

---

### Success Criteria

#### Automated Verification
- [x] Project compiles: `cd debug_build && cmake <flags> ../ && make cosmolattice`
- [ ] Re-run clang-tidy for this check shows 0 remaining diagnostics:
      `python3 tools/run_clang_tidy.py --checks 'modernize-concat-nested-namespaces' --category src`
- [x] No regressions in existing tests: `cd debug_build && ctest`

#### Manual Verification
- [x] Verify each file's closing brace count matches the opening (one `{` and one `}` per merged namespace)
- [x] Verify `libkeccak_twoendians.h` inner namespaces (`CLAT_LITTLE_ENDIAN`, `CLAT_BIG_ENDIAN`) are untouched

**Pause for review before proceeding to Phase 3.**

---

## Phase 3: Use `nullptr` and `auto` (10 diagnostics)

### Overview

Replace C-style `NULL` and integer `0` with `nullptr` in pointer contexts. Also apply
`modernize-use-auto` at the one location where it overlaps (cast initializer).

### Changes

#### `src/include/TempLat/lattice/IO/HDF5/helpers/hdf5object.h:42,58`

**Diagnostic**: `modernize-use-nullptr` — use nullptr (×2)

**Before** (line 42):
```cpp
        const auto dataspace_id = H5Screate_simple(1, &dim, NULL);
```

**After**:
```cpp
        const auto dataspace_id = H5Screate_simple(1, &dim, nullptr);
```

**Before** (line 58):
```cpp
        const auto dataspace_id = H5Screate_simple(1, &dim, NULL);
```

**After**:
```cpp
        const auto dataspace_id = H5Screate_simple(1, &dim, nullptr);
```

**Rationale**: `H5Screate_simple`'s third parameter is `const hsize_t *maxdims`; `NULL`/`nullptr` means "no maximum dimensions".

---

#### `src/include/TempLat/parallel/mpi/comm/exchange/mpiallreduce.h:41,57,79`

**Diagnostic**: `modernize-use-nullptr` — use nullptr (×3)

The file has `int *error = NULL` as default parameter in at least 13 function signatures.
Only 3 were flagged by clang-tidy (lines 41, 57, 79), but for consistency, use `replace_all`
to replace **all** `= NULL)` with `= nullptr)` in this file.

**Before** (pattern appearing on lines 41, 57, 79, 97, 113, 127, 136, 142, 144, 149, 154, 159, 164):
```cpp
int *error = NULL)
```

**After**:
```cpp
int *error = nullptr)
```

**Implementation**: Use `Edit` with `replace_all: true`, `old_string: "= NULL)"`, `new_string: "= nullptr)"`.

---

#### `src/include/TempLat/parallel/mpi/session/mpiguard.h:64`

**Diagnostic**: `modernize-use-nullptr` + `modernize-use-auto` — use nullptr; use auto when initializing with a cast

**Before**:
```cpp
      MPI_Info empty = (MPI_Info)NULL;
```

**After**:
```cpp
      auto empty = (MPI_Info)nullptr;
```

**Rationale**: Combines two diagnostics — `nullptr` replaces `NULL`, and `auto` replaces
the explicit type since the cast already specifies it.

---

#### `src/include/TempLat/util/debug/cdemangle.h:21`

**Diagnostic**: `modernize-use-nullptr` — use nullptr (×3, same line)

**Before**:
```cpp
    char *begin_name = 0, *begin_offset = 0, *end_offset = 0;
```

**After**:
```cpp
    char *begin_name = nullptr, *begin_offset = nullptr, *end_offset = nullptr;
```

**Rationale**: Integer `0` used as null pointer constant — replace with `nullptr`.

---

### Success Criteria

#### Automated Verification
- [x] Project compiles: `cd debug_build && cmake <flags> ../ && make cosmolattice`
- [ ] Re-run clang-tidy for these checks shows 0 remaining diagnostics:
      `python3 tools/run_clang_tidy.py --checks 'modernize-use-nullptr,modernize-use-auto' --category src`
- [x] No regressions in existing tests: `cd debug_build && ctest`

**Pause for review before proceeding to Phase 4.**

---

## Phase 4: Remaining Modernizations (11 diagnostics)

### Overview

Apply four smaller check groups:
- `modernize-use-using` (6): Replace `typedef` with `using` alias
- `modernize-use-override` (2): Add `override` to virtual destructor overrides
- `modernize-use-std-numbers` (2): Use `std::numbers::pi_v<T>` and `std::numbers::sqrt2`
- `modernize-use-integer-sign-comparison` (1): Fix signed/unsigned loop comparison

### Changes — `modernize-use-using` (6 diagnostics)

#### `src/include/CosmoInterface/abstractmodel.h:50-53`

**Before**:
```cpp
    typedef CouplingsManager<NCScalars, NU1Flds> CsU1Couplings;             // couplings U(1) gauge-complex scalar
    typedef CouplingsManager<NSU2Doublet, NU1Flds> SU2DoubletU1Couplings;   // couplings U(1) gauge-SU2 doublet
    typedef CouplingsManager<NSU2Doublet, NSU2Flds> SU2DoubletSU2Couplings; // couplings SU(2) gauge-SU2 doublet
    typedef CouplingsManager<NScalars, NU1Flds> ScalarU1AxionCouplings;     // couplings U(1) gauge-scalar axion
```

**After**:
```cpp
    using CsU1Couplings = CouplingsManager<NCScalars, NU1Flds>;             // couplings U(1) gauge-complex scalar
    using SU2DoubletU1Couplings = CouplingsManager<NSU2Doublet, NU1Flds>;   // couplings U(1) gauge-SU2 doublet
    using SU2DoubletSU2Couplings = CouplingsManager<NSU2Doublet, NSU2Flds>; // couplings SU(2) gauge-SU2 doublet
    using ScalarU1AxionCouplings = CouplingsManager<NScalars, NU1Flds>;     // couplings U(1) gauge-scalar axion
```

---

#### `src/include/TempLat/util/hash/keccakhash.h:32`

**Before**:
```cpp
    typedef KeccakHashBareClass ResultType;
```

**After**:
```cpp
    using ResultType = KeccakHashBareClass;
```

---

#### `src/models/U1Axion.h:35-36`

**Before**:
```cpp
    typedef TempLat::CouplingsManager<NScalars, NU1Flds, true>
        ScalarU1AxionCouplings; // activates axion coupling U(1)-scalar
```

**After**:
```cpp
    using ScalarU1AxionCouplings = TempLat::CouplingsManager<NScalars, NU1Flds, true>; // activates axion coupling U(1)-scalar
```

---

### Changes — `modernize-use-override` (2 diagnostics)

#### `src/include/TempLat/fft/external/fftw/fftwguard.h:71`

**Diagnostic**: annotate this function with 'override'

Base class `FFTSessionGuard` has `virtual ~FFTSessionGuard() = default;`
(`fftlibraryinterface.h:24`).

**Before**:
```cpp
    ~FFTWGuard()
```

**After**:
```cpp
    ~FFTWGuard() override
```

---

#### `src/include/TempLat/fft/external/kokkosfft/kokkosfftguard.h:41`

**Diagnostic**: annotate this function with 'override'

**Before**:
```cpp
    ~KokkosFFTGuard() = default;
```

**After**:
```cpp
    ~KokkosFFTGuard() override = default;
```

---

### Changes — `modernize-use-std-numbers` (2 diagnostics)

#### `src/include/TempLat/util/constants.h:25-26`

**Diagnostic**: prefer 'std::numbers::pi' to this literal

**Step 1**: Add `#include <numbers>` after the existing includes (after line 10).

**Step 2**: Replace the pi definition.

**Before**:
```cpp
    template <typename T = double>
    constexpr T pi = T(
        3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303820);
```

**After**:
```cpp
    template <typename T = double>
    constexpr T pi = std::numbers::pi_v<T>;
```

**Rationale**: `std::numbers::pi_v<T>` (C++20) provides `pi` for any floating-point type,
matching the existing template parameter usage.

---

#### `src/include/CosmoInterface/initializers/fluctuationsgenerator.h:124`

**Diagnostic**: prefer 'std::numbers::sqrt2' to this formula

**Step 1**: Add `#include <numbers>` after the existing includes (after line 16).

**Step 2**: Replace the `sqrt(2.0)` call.

**Before**:
```cpp
      auto e_basis = make_templatvector(0.25, 0.25, sqrt(2.0)/4.0);
```

**After**:
```cpp
      auto e_basis = make_templatvector(0.25, 0.25, std::numbers::sqrt2/4.0);
```

**Rationale**: `std::numbers::sqrt2` (C++20) is a `constexpr double` constant, replacing
the runtime `sqrt(2.0)` call.

---

### Changes — `modernize-use-integer-sign-comparison` (1 diagnostic)

#### `src/include/TempLat/lattice/field/collections/vectorfieldcollection.h:26`

**Diagnostic**: comparison between 'signed' and 'unsigned' integers

Template parameter `int N` (line 19) is compared with `size_t i`.

**Before**:
```cpp
        if constexpr (N > 0)
          for (size_t i = 0; i < N; ++i) {
```

**After**:
```cpp
        if constexpr (N > 0)
          for (int i = 0; i < N; ++i) {
```

**Rationale**: `N` is `int`; changing `i` to `int` eliminates the signed/unsigned mismatch.
The `if constexpr (N > 0)` guard ensures `N` is positive, so `int i` is safe.

---

### Success Criteria

#### Automated Verification
- [x] Project compiles: `cd debug_build && cmake <flags> ../ && make cosmolattice`
- [ ] Re-run clang-tidy for these checks shows 0 remaining diagnostics:
      `python3 tools/run_clang_tidy.py --checks 'modernize-use-using,modernize-use-override,modernize-use-std-numbers,modernize-use-integer-sign-comparison' --category src`
- [x] No regressions in existing tests: `cd debug_build && ctest`

#### Manual Verification
- [x] Verify `constants.h` still compiles with Kokkos backend (the `#include <numbers>` is a standard C++20 header)
- [x] Verify `fluctuationsgenerator.h` produces identical results (compile-time constant vs. runtime `sqrt`)

---

## Final Step: Update Progress Tracker

After all phases are complete:

1. Update `thoughts/shared/clang_tidy_progress.md`:
   - Set each check's status to `done`
   - Add the commit hash(es)
   - Add any notes about the fixes
2. Update the theme's overall status to `done`

### Changes to `thoughts/shared/clang_tidy_progress.md`:

```markdown
## Theme 6: Modernize C++ syntax {#theme-6}

| Check | Count | Status | Commit | Notes |
|-------|-------|--------|--------|-------|
| `modernize-return-braced-init-list` | 35 | done | <hash> | Mechanical: `Type()` → `{}` |
| `modernize-concat-nested-namespaces` | 9 | done | <hash> | C++17 `A::B::C` syntax |
| `modernize-use-nullptr` | 9 | done | <hash> | Also fixed unflagged `= NULL` in mpiallreduce.h |
| `modernize-use-using` | 6 | done | <hash> | `typedef` → `using` |
| `modernize-use-override` | 2 | done | <hash> | Virtual destructors |
| `modernize-use-std-numbers` | 2 | done | <hash> | Added `#include <numbers>` |
| `modernize-use-auto` | 1 | done | <hash> | Combined with nullptr fix in mpiguard.h |
| `modernize-use-integer-sign-comparison` | 1 | done | <hash> | Changed loop var `size_t` → `int` |
```

---

## Verification Commands Reference

### CMake configuration (debug build)

```bash
cd /Users/aflorio/Library/CloudStorage/ProtonDrive-adrien.florio@proton.me-folder/Documents/Projects/CosmoLattice/cosmolattice_GPU_impl/debug_build
cmake -DSerial=ON -DMPI=ON -DHDF5=ON -DTESTING=ON \
  -DMYHDF5_PATH=/Users/aflorio/Library/CloudStorage/ProtonDrive-adrien.florio@proton.me-folder/Documents/Projects/CosmoLattice/cosmolattice_private/dependencies/MyHDF5Parallel/ \
  -DMYFFTW3_PATH=/Users/aflorio/Library/CloudStorage/ProtonDrive-adrien.florio@proton.me-folder/Documents/Projects/CosmoLattice/cosmolattice_private/dependencies/MyLibs/ \
  -DMODEL=lphi4SU2U1 ../
```

### Build
```bash
make cosmolattice
```

### Run clang-tidy for specific check
```bash
python3 tools/run_clang_tidy.py --checks '<check-name>' --category src
```

### Run all tests
```bash
cd debug_build && ctest
```
