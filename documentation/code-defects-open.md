# Open code defects from the documentation review

Companion to `physics-review-findings.md`. That document's §5 listed six defects in the C++
and input files. Current status:

| §5 item | Defect | Status |
|---|---|---|
| 1 | `defectsmeasurer.h:49` variance is negated | **open** — §1 below |
| 2 | Effective-charge keys silently ignored in three input files | **fixed** — `8b4ff6d5`, `1f722466` |
| 3 | `potDeriv2NormCS` missing its `\|Φ\|²` factor — *disputed* | **reverted on instruction** — §2 below |
| 4 | `//` comment markers in `defects_U1LocalStrings.in` | **fixed** — `1f722466` |
| 5 | Duplicated `@label` markers in `su2initializer.h` | **open** — §3 below, attempted and reverted |
| 6 | `scalefactorinitializer.h` defect-model guard over-triggers | **open** — §4 below |

Fixes are on branch `fix/review-code-defects` (off `main`, code only). Reading the source
directly while writing this corrected two things the automated reviewers had reported; both
corrections are flagged inline.

---

## 1. `defectsmeasurer.h:49` — the variance column is negated

```cpp
T norm, norm2, varnorm;
norm  = average(sqrt(Total(i, 0, Model::Ns-1, pow<2>(model.fldS(i));)));
norm2 = average(Total(i, 0, Model::Ns-1, pow<2>(model.fldS(i));));
varnorm = pow<2>(norm) - norm2;          // <-- operands reversed
```

`varnorm` evaluates to `⟨|φ|⟩² − ⟨|φ|²⟩`, the **negative** of the variance. By Jensen's
inequality it is ≤ 0 for every configuration, so the fourth column of `average_norm.txt`
and `average_norm_diffusion.txt` is non-positive throughout every run.

`MeansMeasurer` gets the order right (`meansmeasurer.h:41`):

```cpp
auto rmsPhi2 = phi2 - pow<2>(phi);
```

so the two measurers disagree on sign for the same quantity.

**Why this is not a one-character fix.** Three things are entangled:

1. **What should the column be?** The saver header calls it `var(norm(phi))`, but
   `Observables.md:213` and `Defects.md:241` both document it as an **rms**. Swapping the
   operands fixes the sign but leaves the docs wrong — unless rms was the intent, in which
   case the fix is `sqrt(norm2 - pow<2>(norm))`.
2. **`MeansMeasurer` has the same confusion in the other direction**: its variable is called
   `rmsPhi2` but it computes a variance. Whichever convention you pick should probably apply
   to both; this is plausibly where the docs' rms/var muddle originated.
3. **It changes published output.** Existing `average_norm.txt` files get a column that flips
   sign (or changes units, for rms). Probably worth a note in `Versions.md`.

**Suggested fix**, once decided — either

```cpp
varnorm = norm2 - pow<2>(norm);        // variance, matches the saver header
```

or

```cpp
varnorm = sqrt(norm2 - pow<2>(norm));  // rms, matches Observables.md and Defects.md
```

then align the header string, `Observables.md:213` and `Defects.md:241` with the choice.

---

## 2. `models/lphi4SU2U1.h:203` — `potDeriv2NormCS` missing `|Φ|²`

**Applied, then reverted at the maintainer's instruction. No reason was given, and none
should be inferred from this entry.** The analysis below is the reviewer's, and it may
simply be wrong — a maintainer declining the change is itself evidence worth weighing
against it. Treat this as an open question, not as a confirmed defect awaiting a fix.

```cpp
auto potDeriv2NormCS(Tag<0>)
// 2nd derivative with respect complex scalar norm
{
  return 4 * qH;
}
```

From the potential defined in the same file,

```
V = |Φ|⁴ + qG |Φ|² φ² + 2 qH |Φ|² |φ_CS|²
```

so `∂V/∂|φ_CS| = 4 qH |Φ|² |φ_CS|` — which is exactly what `potDerivNormCS` (line 178)
returns — and therefore `∂²V/∂|φ_CS|² = 4 qH |Φ|²`, not `4 qH`. The sibling `potDeriv2`
(line 197) carries the same `pow<2>(norm(fldSU2Doublet(0_c)))` factor, so the omission is
inconsistent within the file.

The change that was applied and reverted was:

```cpp
return 4 * qH * pow<2>(norm(fldSU2Doublet(0_c)));
```

Open questions a maintainer would need to answer before this goes anywhere:

- Is the derivation above actually right? It assumes `potDeriv2NormCS` means the plain
  second derivative of the program potential with respect to `|φ_CS|`. If these functions
  carry a different convention — a coefficient with the field factor supplied by the caller,
  say — then the current code is correct and `potDeriv2` (line 197) is the odd one out
  instead.
- If the derivation is right, is changing it wanted? It feeds
  `setInitialPotentialAndMassesFromPotential`, so it moves the initial masses of the
  `lphi4SU2U1` model, and the function is the worked example embedded in
  `My first model of gauge fields.md`.

Nobody has answered either question. Recorded so the analysis is available if someone wants
to take it up, not as a pending task.

---

## 3. `su2initializer.h` — duplicated `@label` markers

> **Two corrections to the earlier report.** The reviewers found two duplicated labels;
> there are **three**. And the framing "just delete the duplicate" is wrong — see below.

### What is there

```
line  38:    // @label:su2initializer_initsu2          outer, wraps initializeSU2
line  42:      // @label:su2initializer_initsu2        same name again
line  44:      // @endlabel
line  71:        // @label:su2_gauss_solve
line  86:        // @endlabel
line  89:    // @endlabel

line  93:    // @label:su2initializer_doublet          outer, wraps initializeSU2Doublet
line  98:      // @label:su2initializer_doublet        same name again
line 109:      // @endlabel
line 111:    // @endlabel

line 114:    // @label:su2initializer_fluctuations     outer, wraps addFluctuations...
line 129:      // @label:su2initializer_fluctuations   same name again
line 148:      // @endlabel
line 151:        // @label:su2_doublet_constrained_phases
line 158:        // @endlabel
line 161:        // @label:su2_doublet_modes
line 185:        // @endlabel
line 187:    // @endlabel
```

The file is written as though labels nest: an outer label wrapping a whole function, with
inner labels marking sub-blocks of it.

### Root cause: nesting is unsupported by design

`label_scanner.py:19` keeps a single `open_label`, not a stack, and lines 29-34 warn and
**implicitly close** whatever is open as soon as a new label opens.
`test_label_scanner.py:113` is named `test_nested_label_warns` and asserts exactly that. So
this is a designed contract, not a bug in the scanner.

`Manual/What CosmoLattice does in detail.md` nevertheless embeds **both** kinds from the
same regions — `su2initializer_initsu2` *and* `su2_gauss_solve`; `su2initializer_fluctuations`
*and* `su2_doublet_constrained_phases` / `su2_doublet_modes`. **The page asks for something
the scanner is designed not to provide.** That is the actual defect; the duplicated names
are a symptom.

### What the reader currently sees

| Label | Resolves to | Should be |
|---|---|---|
| `su2initializer_initsu2` | `L43-L43` (one line) | the `initializeSU2` body, ~39-88 |
| `su2initializer_fluctuations` | `L130-L147` | the `addFluctuations...` body, ~115-186 |
| `su2initializer_doublet` | truncated the same way | the `initializeSU2Doublet` body, ~94-110 |

So `What CosmoLattice does in detail.md:248` describes Gauss-law inversion and link
unitarisation that the snippet beside it does not show, and `:256` describes constrained
right-moving phases that its snippet stops short of.

### An attempted fix, and why it failed

Renaming the three inner duplicates to distinct names (`su2_init_doublet_call`,
`su2_doublet_setup`, `su2_doublet_amplitudes`) was tried and **reverted**. The build then
reported:

```
WARNING - label 'su2initializer_initsu2' not found in
          'include/CosmoInterface/initializers/su2initializer.h'.
          Available labels: su2_init_doublet_call, su2_gauss_solve, su2_doublet_setup,
          su2_doublet_amplitudes, su2_doublet_constrained_phases, su2_doublet_modes
```

— i.e. all three page snippets disappeared entirely, which is worse than truncated.

The reason is worth recording: renaming does not un-nest anything. Opening *any* label still
implicitly closes the outer one, so the outer span is never recorded at all. Under the
current scanner **the duplicated name is load-bearing** — it is the only reason the outer
label resolves to anything, because the inner span gets registered under the outer's name.
Removing the collision removes the entry.

### Scope

A scan of the whole staged tree with the real scanner gives **7 warnings in 2 files**:

- `include/CosmoInterface/initializers/su2initializer.h` — 6 (the three nestings)
- `models/lphi4.h:74` — 1 stray `@endlabel` with no preceding `@label` (separate, unexamined)

No other file nests. No `duplicate label` or `unclosed label` warnings anywhere.

### The three options

1. **Delete the inner label/endlabel pairs.** Outer spans then cover the whole functions.
   Cost: the page loses `su2_gauss_solve`, `su2_doublet_constrained_phases` and
   `su2_doublet_modes`.
2. **Delete the outer label/endlabel pairs.** Inner spans work as-is. Cost: the three
   `@emgithub` directives in `What CosmoLattice does in detail.md` must be repointed, and the
   surrounding prose rewritten, since it currently describes the whole function.
3. **Give the scanner a stack so labels nest.** The only option that delivers what the page
   already asks for, and it touches no C++. Cost: it reverses a tested contract —
   `test_nested_label_warns` would have to be rewritten — and the three inner labels would
   still need distinct names, since a stack does not resolve a name colliding with an
   already-open one.

Option 3 is the only one that leaves both the source and the page saying what their authors
evidently intended, but it is a design decision about the docs tooling and belongs to a
maintainer, not to a review.

---

## 4. `scalefactorinitializer.h:33` — the defect-model guard over-triggers

```cpp
if (!rPar.fixedBackground && Model::DefectsModel)
  throw(RunParametersInconsistent("Running a defects model with self-consistent expansion "
        "is not tested, and features such as (extra)fattening may not work correctly. "
        "If you really want to run this option, comment out this exception in "
        "scalefactorinitializar.h"));
```

> **Correction to the earlier report.** A reviewer called this "an unintended exclusion".
> That is wrong — the message shows the guard is deliberate and documents its own bypass.
> The defect is narrower: it catches a second case its author did not intend.

The condition tests `!fixedBackground`, but that flag is not independent of `expansion`.
From `runparameters.h:81`:

```cpp
fixedBackground(expansion ? par.get<bool>("fixedBackground", false) : ...)
```

so `expansion = false` forces `fixedBackground = false`, which trips the guard:

| `expansion` | `fixedBackground` | Guard fires? | Intended? |
|---|---|---|---|
| `true` | `true` | no | correct |
| `true` | `false` | yes | correct — the untested self-consistent case |
| `false` | forced `false` | yes | **no** — this is a flat-background run |

A flat-background defect run is therefore rejected, though `Defects.md:75` says "only
simulations with fixed expansion **or flat background** are allowed" and `Defects.md:327`
repeats it for local strings.

**Suggested fix** — narrow the guard:

```cpp
if (rPar.expansion && !rPar.fixedBackground && Model::DefectsModel)
```

**Why not applied.** Whether flat-background defect runs are actually *supported* is a
physics question. The guard warns that "(extra)fattening may not work correctly", and
`runparameters.h:129` forces `doResolutionPreserving = false` whenever `fixedBackground` is
false — so on a flat background the resolution-preserving machinery is silently disabled.
That may be precisely why the author wanted these runs blocked, or may be harmless because a
flat background needs no resolution preservation. It needs someone who knows the defect
module.

If the answer is "flat background is fine", also fix the typo `scalefactorinitializar.h` →
`scalefactorinitializer.h` in the message.

---

## Verification notes for the two applied fixes

- **Charge keys.** All four assignments across `lphi4SU2U1.in`, `lphi4U1.in` and
  `defects_U1LocalStrings.in` now match the strings the parser reads
  (`complexscalarbase.h:72`, `su2doubletbase.h:75,80`), checked by grepping both sides.
- **Comment markers.** `filereader.h:30` takes `char comment = '#'` and line 38 keeps any
  line whose first character is not the comment char — so the two `//` lines really were
  being handed to the parameter parser. No `//` remains in any `.in` file, and a docs
  rebuild confirms the snippet still resolves to `L37-L41`.

**Neither fix was compiled or run.** Both are verified by reading the source and by
re-rendering the documentation, not by building CosmoLattice.
