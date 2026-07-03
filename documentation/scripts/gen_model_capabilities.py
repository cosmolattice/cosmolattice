#!/usr/bin/env python3
"""Per-model capability extractor: models/*.h ``ModelPars`` -> parameters.yaml.

The GUI input-builder (and any other consumer) needs to know, per model, which
field sectors and coupling types actually exist — so it can hide parameters a
model does not use (e.g. axion couplings for a model with no axion). The
authoritative record of that is the ``struct ModelPars : public
TempLat::DefaultModelPars`` at the top of each ``models/<stem>.h``: it declares
the field-sector counts (``NScalars``, ``NCScalars``, ``NU1Flds``,
``NSU2Doublet``, ``NSU2Flds``), the lattice dimensionality ``NDim``, and which
``CouplingsManager<...>`` typedefs are *active* (re-declared with a trailing
``true`` boolean, vs. the inactive defaults inherited from ``DefaultModelPars``).

This script parses that struct from every shipped model and writes a generated
``metadata.models`` block into ``parameters.yaml`` (both the ``source/data`` and
``website/data`` copies), between marker comments. It mirrors the code into the
documented database so consumers never have to parse C++ themselves.

Capability schema (per model, zero/false keys omitted to stay compact)::

    <model>: {NScalars: N, NCScalars: N, NU1Flds: N, NSU2Doublet: N,
              NSU2Flds: N, NDim: N, couplings: {csU1, su2dbU1, su2dbSU2,
              axionU1, nonMinimal}}

``NDim`` is always emitted (it is a real value, default 3, that the GUI needs);
the sector counts are emitted only when > 0; ``couplings`` only when at least one
flag is active, listing only the active flags.

Usage
-----
    python documentation/scripts/gen_model_capabilities.py            # write
    python documentation/scripts/gen_model_capabilities.py --check    # no write
    python documentation/scripts/gen_model_capabilities.py --list     # print

``--check`` exits non-zero (without writing) if either YAML copy is out of date
with respect to the headers — drift detection for a pre-commit / CI step.
"""

import argparse
import re
import sys
from pathlib import Path

# --- Paths (resolved relative to this script, so it runs from anywhere) ------
SCRIPT_DIR = Path(__file__).resolve().parent
DOC_ROOT = SCRIPT_DIR.parent            # documentation/
REPO_ROOT = DOC_ROOT.parent             # cosmolattice_private/
MODELS_DIR = REPO_ROOT / "models"
# The committed source of truth (must exist) plus generated build mirrors that
# are kept in sync only when present (website/ is a gitignored docs-build output,
# absent on a fresh checkout).
YAML_PRIMARY = DOC_ROOT / "source" / "data" / "parameters.yaml"
YAML_MIRRORS = [DOC_ROOT / "website" / "data" / "parameters.yaml"]


def target_yamls():
    """Primary YAML (required) plus any build mirrors that currently exist."""
    if not YAML_PRIMARY.is_file():
        sys.exit(f"error: parameters.yaml not found: {YAML_PRIMARY}")
    return [YAML_PRIMARY] + [m for m in YAML_MIRRORS if m.is_file()]

# --- ModelPars grammar -------------------------------------------------------
# Field-sector counts and their DefaultModelPars defaults (include/CosmoInterface/
# abstractmodel.h). Any field a model does not declare inherits the default.
FIELD_DEFAULTS = {
    "NScalars": 0,
    "NCScalars": 0,
    "NU1Flds": 0,
    "NSU2Doublet": 0,
    "NSU2Flds": 0,
    "NDim": 3,
}
# Sector counts emitted only when non-zero; NDim is always emitted.
COUNT_FIELDS = ["NScalars", "NCScalars", "NU1Flds", "NSU2Doublet", "NSU2Flds"]

# CouplingsManager typedef/using alias name -> short capability flag. A coupling
# is *active* iff the model re-declares the alias with at least one `true` among
# the template booleans (the args after the first two integer field counts);
# the DefaultModelPars instantiations carry no booleans and are inactive.
COUPLING_FLAGS = {
    "CsU1Couplings": "csU1",
    "SU2DoubletU1Couplings": "su2dbU1",
    "SU2DoubletSU2Couplings": "su2dbSU2",
    "ScalarU1AxionCouplings": "axionU1",
    "NonMinimalCouplings": "nonMinimal",
}
# Emission order of the coupling flags within the `couplings` mapping.
COUPLING_ORDER = ["csU1", "su2dbU1", "su2dbSU2", "axionU1", "nonMinimal"]

MODELNAME_RE = re.compile(r"#define\s+MODELNAME\s+(\w+)")
COUNT_RE = re.compile(r"static\s+constexpr\s+size_t\s+(\w+)\s*=\s*(\d+)\s*;")
# typedef ... CouplingsManager<ARGS> NAME;
TYPEDEF_RE = re.compile(
    r"typedef\s+(?:\w+::)?CouplingsManager\s*<([^>]*)>\s*(\w+)\s*;"
)
# using NAME = ... CouplingsManager<ARGS>;
USING_RE = re.compile(
    r"using\s+(\w+)\s*=\s*(?:\w+::)?CouplingsManager\s*<([^>]*)>\s*;"
)

# Strip C/C++ comments while preserving string/char literals (same approach as
# check_params_code.py): so a commented-out typedef does not count as active.
_COMMENT_OR_LITERAL = re.compile(
    r"//.*?$|/\*.*?\*/|'(?:\\.|[^\\'])*'|\"(?:\\.|[^\\\"])*\"",
    re.DOTALL | re.MULTILINE,
)

# --- Generated-block markers (YAML comments under `metadata:`) ---------------
BEGIN_MARK = ("  # === BEGIN generated model capabilities — "
              "scripts/gen_model_capabilities.py (do not edit) ===")
END_MARK = "  # === END generated model capabilities ==="
REGION_RE = re.compile(
    r"(?m)^[ ]*# === BEGIN generated model capabilities.*?"
    r"^[ ]*# === END generated model capabilities ===[ ]*\n",
    re.DOTALL,
)


def strip_comments(text):
    """Remove C/C++ comments, leaving string and char literals intact."""
    return _COMMENT_OR_LITERAL.sub(
        lambda m: " " if m.group(0)[0] == "/" else m.group(0), text
    )


def _coupling_is_active(args):
    """True if a CouplingsManager<ARGS> declares an active coupling.

    The first two template args are integer field counts (matter, gauge); any
    further arg that is literally ``true`` activates the coupling.
    """
    parts = [a.strip() for a in args.split(",")]
    return any(p == "true" for p in parts[2:])


def _model_pars_body(text):
    """Return the brace-balanced body of ``struct ModelPars { ... }`` or None.

    Operates on comment-stripped text so braces inside comments are not counted.
    """
    m = re.search(r"struct\s+ModelPars\b", text)
    if not m:
        return None
    open_idx = text.find("{", m.end())
    if open_idx < 0:
        return None
    depth = 0
    for i in range(open_idx, len(text)):
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return text[open_idx + 1:i]
    return None


def parse_header(path):
    """Return ``(model_name, caps_dict)`` for one models/<stem>.h, or None.

    ``caps_dict`` has the resolved field counts (defaults applied), ``NDim``,
    and a ``couplings`` dict of active flags.
    """
    raw = path.read_text(encoding="utf-8", errors="replace")
    name_m = MODELNAME_RE.search(raw)
    if not name_m:
        return None
    model = name_m.group(1)

    body = _model_pars_body(strip_comments(raw))
    if body is None:
        return None

    fields = dict(FIELD_DEFAULTS)
    for m in COUNT_RE.finditer(body):
        if m.group(1) in fields:
            fields[m.group(1)] = int(m.group(2))

    couplings = {}
    for args, alias in TYPEDEF_RE.findall(body):   # typedef <...ARGS> NAME;
        if alias in COUPLING_FLAGS and _coupling_is_active(args):
            couplings[COUPLING_FLAGS[alias]] = True
    for alias, args in USING_RE.findall(body):     # using NAME = <...ARGS>;
        if alias in COUPLING_FLAGS and _coupling_is_active(args):
            couplings[COUPLING_FLAGS[alias]] = True

    caps = {k: fields[k] for k in COUNT_FIELDS}
    caps["NDim"] = fields["NDim"]
    caps["couplings"] = couplings
    return model, caps


def extract_capabilities():
    """Return ``{model: caps}`` for every models/*.h, ordered lphi4-first."""
    if not MODELS_DIR.is_dir():
        sys.exit(f"error: models directory not found: {MODELS_DIR}")
    caps = {}
    for path in sorted(MODELS_DIR.glob("*.h")):
        parsed = parse_header(path)
        if parsed is None:
            continue
        model, c = parsed
        caps[model] = c
    return _ordered(caps)


def _ordered(caps):
    """lphi4 (default model) first, then the rest sorted — matches the GUI."""
    rest = sorted(k for k in caps if k != "lphi4")
    order = (["lphi4"] if "lphi4" in caps else []) + rest
    return {k: caps[k] for k in order}


def _format_caps(caps):
    """One-line YAML flow mapping for a model's capabilities."""
    items = []
    for f in COUNT_FIELDS:
        if caps[f]:
            items.append(f"{f}: {caps[f]}")
    items.append(f"NDim: {caps['NDim']}")
    active = [fl for fl in COUPLING_ORDER if caps["couplings"].get(fl)]
    if active:
        inner = ", ".join(f"{fl}: true" for fl in active)
        items.append(f"couplings: {{{inner}}}")
    return "{" + ", ".join(items) + "}"


def render_block(caps):
    """Render the full marker-delimited ``models:`` block (trailing newline)."""
    lines = [BEGIN_MARK, "  models:"]
    for model, c in caps.items():
        lines.append(f"    {model}: {_format_caps(c)}")
    lines.append(END_MARK)
    return "\n".join(lines) + "\n"


def inject(yaml_text, block):
    """Replace the marked region with ``block``, or insert it before parameters."""
    if REGION_RE.search(yaml_text):
        return REGION_RE.sub(lambda _m: block, yaml_text)
    m = re.search(r"(?m)^parameters:", yaml_text)
    if not m:
        sys.exit("error: could not find top-level 'parameters:' key in YAML")
    return yaml_text[:m.start()] + block + yaml_text[m.start():]


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--check", action="store_true",
                   help="do not write; exit non-zero if any YAML copy is out of date")
    g.add_argument("--list", action="store_true",
                   help="print the extracted capability block and exit (no write)")
    args = ap.parse_args(argv)

    caps = extract_capabilities()
    block = render_block(caps)

    if args.list:
        sys.stdout.write(block)
        return 0

    stale = []
    changed = []
    for path in target_yamls():
        original = path.read_text(encoding="utf-8")
        updated = inject(original, block)
        if original == updated:
            continue
        if args.check:
            stale.append(path)
        else:
            path.write_text(updated, encoding="utf-8")
            changed.append(path)

    if args.check:
        if stale:
            names = ", ".join(str(p.relative_to(REPO_ROOT)) for p in stale)
            print(f"error: model-capability block is out of date in: {names}",
                  file=sys.stderr)
            print("       run 'python documentation/scripts/gen_model_capabilities.py'",
                  file=sys.stderr)
            return 1
        print(f"model capabilities up to date ({len(caps)} models).")
        return 0

    if changed:
        names = ", ".join(str(p.relative_to(REPO_ROOT)) for p in changed)
        print(f"Updated model-capability block ({len(caps)} models) in: {names}")
    else:
        print(f"model capabilities already up to date ({len(caps)} models).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
