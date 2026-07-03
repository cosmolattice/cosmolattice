#!/usr/bin/env python3
"""Code <-> YAML parameter drift checker.

`parameters.yaml` is the documented single source of truth for every
CosmoLattice input parameter. The parameters it documents are actually *read*
from the input file / command line by the C++ via call sites of the form

    parser.get<TYPE>("name", ...)
    parser.getOverride<TYPE>("name", ...)
    parser.getSeed("name")

This script scans those call sites in ``include/`` (framework) and
``models/*.h`` (one scope per model), extracts the set of parameter names per
scope, and compares it against the names listed in ``parameters.yaml``. It
fails (exit 1) if the two disagree, so a parameter added/removed/renamed in the
code can never silently drift out of sync with the documented database.

What it checks: the **set of parameter names per scope**. What it deliberately
does not check: descriptions, units, constraints (these live only in the YAML /
human knowledge, never in the code).

Scope mapping
-------------
* Any read under ``include/``                -> scope ``framework``
* A read in ``models/<stem>.h``              -> scope ``model:<stem>``

Special cases handled
---------------------
* **Comments** are stripped before scanning, so an example call written inside a
  ``//`` or ``/* */`` comment (e.g. the explanatory ``get<double>("lambda")`` in
  ``NMC_tanh4_w_mass_P.h``) is not mistaken for a real read.
* **Dynamically-named keys** such as ``parser.get<...>("ext_PS" + std::to_string(i))``
  contribute their literal prefix (``ext_PS``); the YAML documents the same base
  name ``ext_PS``, so they line up.
* **Duplicate reads** of one key collapse naturally (set semantics).
* Genuine, intentional code-only / YAML-only names go in :data:`ALLOWLIST`.

Usage
-----
    python documentation/scripts/check_params_code.py [--list]

``--list`` prints the full extracted name set per scope and exits 0 (no check).
"""

import argparse
import re
import sys
from pathlib import Path

try:
    import yaml
except ImportError:  # pragma: no cover - dependency hint
    sys.exit("error: PyYAML is required (pip install pyyaml)")

# --- Paths (resolved relative to this script, so it runs from anywhere) ------
SCRIPT_DIR = Path(__file__).resolve().parent
DOC_ROOT = SCRIPT_DIR.parent            # documentation/
REPO_ROOT = DOC_ROOT.parent             # cosmolattice_private/
YAML_PATH = DOC_ROOT / "source" / "data" / "parameters.yaml"
INCLUDE_DIR = REPO_ROOT / "include"
MODELS_DIR = REPO_ROOT / "models"

# --- Call-site grammar -------------------------------------------------------
# parser.get<...>("name"  /  parser.getOverride<...>("name"
GET_RE = re.compile(r'\.(?:get|getOverride)\s*<[^>]*>\s*\(\s*"([^"]+)"')
# parser.getSeed("name")
SEED_RE = re.compile(r'\.getSeed\s*\(\s*"([^"]+)"')

# Strip C/C++ comments while preserving string/char literals: matches a
# comment OR a literal; comments are replaced by a space, literals are kept.
_COMMENT_OR_LITERAL = re.compile(
    r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
    re.DOTALL | re.MULTILINE,
)

# Intentional exceptions, keyed by scope. Names listed here are excused from the
# corresponding mismatch list (kept empty by default — the tree is fully in
# sync). Example:
#   ALLOWLIST = {"framework": {"code_only": {"someInternalKey"}}}
ALLOWLIST = {
    # "<scope>": {"code_only": {...}, "yaml_only": {...}},
}


def strip_comments(text):
    """Remove C/C++ comments, leaving string and char literals intact."""
    return _COMMENT_OR_LITERAL.sub(
        lambda m: " " if m.group(0)[0] == "/" else m.group(0), text
    )


def _names_in(text):
    text = strip_comments(text)
    names = set()
    for m in GET_RE.finditer(text):
        names.add(m.group(1))
    for m in SEED_RE.finditer(text):
        names.add(m.group(1))
    return names


def extract_code_params():
    """Return ``{scope: set(names)}`` read from the C++ source.

    ``include/`` (recursively, ``.h``/``.cpp``) maps to the ``framework`` scope;
    each ``models/<stem>.h`` maps to ``model:<stem>``.
    """
    code = {}
    if not INCLUDE_DIR.is_dir():
        sys.exit(f"error: include directory not found: {INCLUDE_DIR}")
    if not MODELS_DIR.is_dir():
        sys.exit(f"error: models directory not found: {MODELS_DIR}")

    framework = set()
    for path in INCLUDE_DIR.rglob("*"):
        if path.suffix in (".h", ".cpp"):
            framework |= _names_in(path.read_text(encoding="utf-8", errors="replace"))
    code["framework"] = framework

    for path in sorted(MODELS_DIR.glob("*.h")):
        code[f"model:{path.stem}"] = _names_in(
            path.read_text(encoding="utf-8", errors="replace")
        )
    return code


def extract_yaml_params():
    """Return ``{scope: set(names)}`` documented in parameters.yaml."""
    data = yaml.safe_load(YAML_PATH.read_text(encoding="utf-8"))
    scopes = {}
    for prm in data.get("parameters", []):
        scopes.setdefault(prm["scope"], set()).add(prm["name"])
    return scopes


def reconcile(code, yaml_scopes):
    """Compare per-scope name sets; return a sorted list of drift records.

    Each record is ``(scope, code_only, yaml_only)`` with the ALLOWLIST applied.
    Only scopes with a remaining discrepancy are returned.
    """
    drifts = []
    for scope in sorted(set(code) | set(yaml_scopes)):
        cc = code.get(scope, set())
        yy = yaml_scopes.get(scope, set())
        allow = ALLOWLIST.get(scope, {})
        code_only = sorted((cc - yy) - set(allow.get("code_only", ())))
        yaml_only = sorted((yy - cc) - set(allow.get("yaml_only", ())))
        if code_only or yaml_only:
            drifts.append((scope, code_only, yaml_only))
    return drifts


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--list",
        action="store_true",
        help="print the extracted parameter names per scope and exit (no check)",
    )
    args = parser.parse_args(argv)

    code = extract_code_params()

    if args.list:
        for scope in sorted(code):
            print(f"{scope} ({len(code[scope])}):")
            for name in sorted(code[scope]):
                print(f"  {name}")
        return 0

    yaml_scopes = extract_yaml_params()
    drifts = reconcile(code, yaml_scopes)

    if not drifts:
        n = sum(len(v) for v in code.values())
        print(f"OK: code and parameters.yaml agree ({n} reads across {len(code)} scopes).")
        return 0

    print("error: parameters.yaml is out of sync with the code:\n", file=sys.stderr)
    for scope, code_only, yaml_only in drifts:
        print(f"  [{scope}]", file=sys.stderr)
        if code_only:
            print(
                f"    read in code but MISSING from YAML: {', '.join(code_only)}",
                file=sys.stderr,
            )
        if yaml_only:
            print(
                f"    in YAML but NOT read in code:       {', '.join(yaml_only)}",
                file=sys.stderr,
            )
    print(
        "\nUpdate documentation/source/data/parameters.yaml to match the code "
        "(then run 'make gen-params'), or add a justified entry to ALLOWLIST in "
        "this script.",
        file=sys.stderr,
    )
    return 1


if __name__ == "__main__":
    sys.exit(main())
