# CosmoLattice input-file builder

A lightweight, locally-hosted GUI for generating CosmoLattice `.in` parameter
files. It reads the curated parameter database
(`documentation/source/data/parameters.yaml`) directly, so it never goes stale:
add or edit a parameter there and the form updates automatically.

## Quick start

```bash
./run.sh
```

On first run this creates a local `.venv/` and installs `streamlit` + `PyYAML`,
then opens the app in your browser (default <http://localhost:8501>).

Or manually:

```bash
pip install -r requirements.txt
streamlit run app.py
```

## How it works

1. Pick a **model** in the sidebar.
2. Fill in the **required** fields (marked `*`) and any **commonly-tuned**
   parameters shown directly.
3. Optional, less-common parameters live under per-section **Advanced**
   expanders — tick *set* to override their default.
4. The generated input file updates live in the right-hand column and stays
   pinned in view as you scroll; **Download .in file** from the button above it.
   Drag the divider between the two columns to resize them (the split is
   remembered between sessions). On narrow screens the preview drops below the
   form instead of sitting beside it.

Each field shows its description, units, default, allowed values (for enums),
and any constraints on hover/help.

## Notes

- The model is a compile-time choice in CosmoLattice, so the generated file
  contains no `model =` line — the selector only decides which model-specific
  parameters to show.
- Vector parameters (arity ≠ 1, e.g. `initial_amplitudes`, or symbolic arities
  like `NS`, `#U1`) are entered as space-separated values.
- Symbolic defaults (e.g. `10000*dt`) are shown as placeholders; leave the field
  blank to let CosmoLattice apply its default.
