# Cosmolattice online documentation

To build the documentation, simply run in this directory:

```bash
bash build.sh
```

The output will be placed in the `website/site/` folder. You can simply view the documentation by opening `website/site/index.html` in your browser, e.g.

```bash
xdg-open website/site/index.html
```

## Parameter appendix (generated)

The parameter tables in `source/docs/Manual/Appendix_Parameters.md` are **generated**
from the single source of truth `source/data/parameters.yaml` (see
`source/data/parameters.schema.md` for the schema). Only the regions between the
`<!-- @gen:params:KEY -->` / `<!-- @endgen -->` markers are generated; everything
else in the appendix is hand-written prose and is left untouched.

**After editing `parameters.yaml`, regenerate the appendix and commit it:**

```bash
cd documentation
make gen-params      # rewrites the marker regions in Appendix_Parameters.md
```

Useful related targets (run from `documentation/`):

```bash
make check-params       # verify the database is in sync, both directions (no write)
make check-params-code  # only the code<->YAML drift check
make test-params        # run the generator + drift-checker unit tests
make docs               # build the full site (equivalent to bash build.sh)
make help               # list all targets
```

### Keeping `parameters.yaml` honest (two directions)

`make check-params` (also run automatically at the start of `build.sh`) verifies
the parameter database in **both** directions:

1. **Appendix ← YAML** — the generated tables match `parameters.yaml` (runs the
   generator unit tests + a no-write `--check`).
2. **Code ↔ YAML** — every parameter read in the C++ via
   `get<>`/`getOverride<>`/`getSeed` (in `include/` and `models/*.h`) appears in
   `parameters.yaml` with the right scope, and vice versa
   (`scripts/check_params_code.py`). This catches a parameter being
   added/removed/renamed in the code without updating the YAML.

So, before committing changes to `parameters.yaml`, the appendix, or any
`get<>` call site:

```bash
make check-params
# equivalently: bash scripts/check_params_sync.sh
```

If direction 1 fails, run `make gen-params` and commit the result. If direction
2 fails, update `parameters.yaml` to match the code (then `make gen-params`), or
— for an intentional code-only / YAML-only name — add a justified entry to the
`ALLOWLIST` in `scripts/check_params_code.py`. The check is git-independent and
self-contained, so it can be wired into CI unchanged later.