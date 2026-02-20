# Merge Status: master -> GPU_impl

> Baseline: `16ea955f` -> Target: `a802776d` (45 commits, 121 files)

| # | Theme | Status | Triage | Plan | Commit | Notes |
|---|-------|--------|--------|------|--------|-------|
| 1 | Build system / config | **done** | [theme_1.md](theme_1.md) | n/a | n/a | All 6 files skipped — GPU_impl build system fully diverged |
| 2 | Utilities | **done** | [theme_2.md](theme_2.md) | [plan](../plans/2026-02-20-merge-theme-2-utilities.md) | `903e6783` | stdatomictype.h, flattenstd.h, floattostring.h format_more() |
| 3 | SU(2) 4-component storage | **done** | [theme_3.md](theme_3.md) | [plan](../plans/2026-02-20-merge-theme-3-su2-4component.md) | `1dd14f6d` | unitarize(), 4-component SU2Field storage |
| 4 | SU(2) exp map & algebra | **done** | [theme_4.md](theme_4.md) | [plan](../plans/2026-02-20-merge-theme-4-su2-expmap.md) | `320ee096` | exp/expinv, acos operator, symbols, expmap_inplace |
| 5 | RK2N evolver | **done** | [theme_5.md](theme_5.md) | [plan](../plans/2026-02-20-merge-theme-5-rk2n-evolver.md) | `a07982f9` | fieldsnumbering, kernelstypes, rk2nstorageparameters, rk2nstorage, extrafields, kernel EoM dispatch, evolver RK2N |
| 6 | HDF5 overhaul | pending | - | - | - | Independent |
| 7 | Measurement refactoring | pending | - | - | - | Depends on 6 |
| 8 | U(1) Axion coupling | pending | - | - | - | Depends on 5, 6, 7 |
| 9 | Test infrastructure | pending | - | - | - | Last |
