#!/usr/bin/env python3
"""Check the curated model registry (models.yaml) against the code.

The single source of truth for *which* models ship is the set of headers
``models/*.h``. This script enforces that three views of that set agree:

  1. ``models/*.h``                              — the code (ground truth).
  2. ``source/data/models.yaml`` ``models[].name`` — the curated registry
     (human labels/descriptions; an intentional tripwire on add/rename/remove).
  3. ``parameters.yaml`` ``metadata.models``     — the generated capability block.

The curated labels/descriptions in ``models.yaml`` are *not* derivable from the
headers, so they are maintained by hand; this check only keeps the *set* of
names honest. If the sets disagree it prints the differences and exits non-zero
(without modifying anything).

    python documentation/scripts/check_models_registry.py   # exit 1 on drift
    # or, from documentation/:  make check-models

Override the interpreter with PYTHON=... ; PyYAML must be importable.
"""

import sys
from pathlib import Path

import yaml

SCRIPT_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))

import gen_model_capabilities as g  # noqa: E402  (after sys.path tweak)

DOC_ROOT = SCRIPT_DIR.parent
MODELS_YAML = DOC_ROOT / "source" / "data" / "models.yaml"
PARAMS_YAML = DOC_ROOT / "source" / "data" / "parameters.yaml"


def registry_names(path=MODELS_YAML):
    """Ordered list of ``name`` values from models.yaml (validates the schema)."""
    if not path.is_file():
        sys.exit(f"error: model registry not found: {path}")
    data = yaml.safe_load(path.read_text(encoding="utf-8")) or {}
    models = data.get("models")
    if not isinstance(models, list):
        sys.exit(f"error: {path.name} must have a top-level 'models:' list")
    names = []
    for i, entry in enumerate(models):
        if not isinstance(entry, dict) or "name" not in entry:
            sys.exit(f"error: {path.name} entry #{i} must be a mapping with a 'name'")
        names.append(entry["name"])
    return names


def metadata_model_names(path=PARAMS_YAML):
    """Keys of the generated ``metadata.models`` block in parameters.yaml."""
    if not path.is_file():
        sys.exit(f"error: parameters.yaml not found: {path}")
    data = yaml.safe_load(path.read_text(encoding="utf-8")) or {}
    models = (data.get("metadata") or {}).get("models") or {}
    return list(models.keys())


def _diff(label_a, a, label_b, b):
    """Human-readable set-difference lines between two name collections."""
    lines = []
    only_a = sorted(set(a) - set(b))
    only_b = sorted(set(b) - set(a))
    if only_a:
        lines.append(f"  in {label_a} but not in {label_b}: {', '.join(only_a)}")
    if only_b:
        lines.append(f"  in {label_b} but not in {label_a}: {', '.join(only_b)}")
    return lines


def check():
    """Return 0 if all three views agree, else print the drift and return 1."""
    headers = list(g.extract_capabilities().keys())
    registry = registry_names()
    metadata = metadata_model_names()

    problems = []
    problems += _diff("code (models/*.h)", headers, "models.yaml", registry)
    problems += _diff("code (models/*.h)", headers,
                      "parameters.yaml metadata.models", metadata)

    dups = sorted({n for n in registry if registry.count(n) > 1})
    if dups:
        problems.append(f"  duplicate names in models.yaml: {', '.join(dups)}")

    if problems:
        sys.stderr.write(
            "error: the model registry is out of sync:\n\n"
            + "\n".join(problems)
            + "\n\nUpdate documentation/source/data/models.yaml to list exactly the "
            "models in models/*.h (add a name + label/description for a new model, "
            "remove a deleted one), then run 'make gen-model-caps' to refresh "
            "metadata.models.\n"
        )
        return 1

    print(f"OK: model registry lists all {len(headers)} models "
          "(models.yaml, code, and metadata.models agree).")
    return 0


def main(argv=None):
    return check()


if __name__ == "__main__":
    sys.exit(main())
