#!/usr/bin/env python3
"""Generate the parameter tables in the manual appendix from parameters.yaml.

This script is the table-only generator for the parameter appendix. It reads
``documentation/source/data/parameters.yaml`` (the single source of truth for
every CosmoLattice input parameter), renders one markdown table per parameter
region, and injects each table into ``Appendix_Parameters.md`` **between explicit
markers only**:

    <!-- @gen:params:<KEY> -->
    ... generated table goes here ...
    <!-- @endgen -->

Everything outside the markers (prose, intros, notes) is left byte-for-byte
untouched. The injection is idempotent: running the script twice yields the
exact same file.

Region keys:
  framework:run, framework:IC, framework:output, framework:spectra,
  framework:snapshots, framework:GW, framework:saving, framework:couplings
  models    (emits one "### <model>" heading + table per model)

Usage:
    python documentation/scripts/gen_param_appendix.py [--check]

With ``--check`` the script does not write anything; it exits non-zero if the
target file is not already up to date (useful in CI).
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
DOC_ROOT = SCRIPT_DIR.parent  # documentation/
YAML_PATH = DOC_ROOT / "source" / "data" / "parameters.yaml"
MD_PATH = DOC_ROOT / "source" / "docs" / "Manual" / "Appendix_Parameters.md"

# --- Marker grammar ----------------------------------------------------------
MARK = re.compile(
    r"(<!-- @gen:params:(?P<key>[\w:-]+) -->)(.*?)(<!-- @endgen -->)", re.S
)
OPEN_RE = re.compile(r"<!-- @gen:params:[\w:-]+ -->")
CLOSE_RE = re.compile(r"<!-- @endgen -->")

# Canonical emission order of the framework categories (see parameters.schema.md).
FRAMEWORK_CATEGORIES = [
    "run",
    "IC",
    "output",
    "spectra",
    "snapshots",
    "GW",
    "saving",
    "couplings",
]

TABLE_HEADER = (
    "| Parameter | Type | Default | Units | Description |\n"
    "| --- | --- | --- | --- | --- |"
)


def esc(text):
    """Escape a value for inclusion in a single markdown table cell."""
    if text is None:
        return ""
    return str(text).replace("|", "\\|").replace("\n", " ").strip()


def fmt_type(param):
    """Render the Type cell, annotating non-scalar (vector) parameters with arity."""
    t = param.get("type", "")
    arity = str(param.get("arity", "1"))
    cell = f"`{t}`"
    if arity not in ("1", "", "None"):
        cell += f" ×{arity}"  # e.g. `float` ×2  or  `float` ×NS
    return cell


def fmt_default(param):
    """Render the Default cell."""
    default = param.get("default")
    if default is not None:
        return f"`{esc(default)}`"
    if param.get("required"):
        return "*required*"
    return "—"  # em dash: no default, optional


def fmt_description(param):
    """Render the Description cell, appending allowed values for enums."""
    desc = esc(param.get("description"))
    if param.get("type") == "enum" and param.get("enum_values"):
        values = ", ".join(f"`{esc(v)}`" for v in param["enum_values"])
        desc = f"{desc} Allowed values: {values}."
    return desc


def render_row(param):
    return (
        f"| `{esc(param.get('name'))}` | {fmt_type(param)} | "
        f"{fmt_default(param)} | {esc(param.get('units'))} | "
        f"{fmt_description(param)} |"
    )


def render_table(params):
    """Render a markdown table from a list of parameter dicts.

    Duplicate parameter names within a region (an artefact of a parameter being
    read more than once in the source) are collapsed to their first occurrence
    so the rendered table has one row per parameter name.
    """
    rows = []
    seen = set()
    for p in params:
        name = p.get("name")
        if name in seen:
            continue
        seen.add(name)
        rows.append(render_row(p))
    return "\n".join([TABLE_HEADER, *rows])


def build_tables(parameters):
    """Group parameters by region and render one table per region key.

    Returns a dict ``{key: table_markdown}`` covering every framework region key
    plus the special ``models`` key (which renders ``### <model>`` + table per
    model, in YAML order with the default model first).
    """
    framework = {cat: [] for cat in FRAMEWORK_CATEGORIES}
    models = {}  # model name -> list of params, insertion-ordered
    for p in parameters:
        scope = p.get("scope", "")
        if scope == "framework":
            cat = p.get("category")
            if cat not in framework:
                sys.exit(f"error: unknown framework category {cat!r} for {p.get('name')!r}")
            framework[cat].append(p)
        elif scope.startswith("model:"):
            models.setdefault(scope.split(":", 1)[1], []).append(p)
        else:
            sys.exit(f"error: unrecognized scope {scope!r} for {p.get('name')!r}")

    tables = {}
    for cat in FRAMEWORK_CATEGORIES:
        tables[f"framework:{cat}"] = render_table(framework[cat])

    # Models block: one subsection per model.
    blocks = []
    for model, params in models.items():
        blocks.append(f"### `{model}`\n\n{render_table(params)}")
    tables["models"] = "\n\n".join(blocks)

    return tables


def inject(md_text, tables_by_key):
    """Replace the content between each marker pair with its generated table.

    Errors out if a generated key has no marker in the file, or if the markers
    are unbalanced (an opening marker without a matching ``@endgen`` or vice
    versa).
    """
    n_open = len(OPEN_RE.findall(md_text))
    n_close = len(CLOSE_RE.findall(md_text))
    if n_open != n_close:
        sys.exit(
            f"error: unbalanced markers in {MD_PATH.name}: "
            f"{n_open} '@gen:params' vs {n_close} '@endgen'"
        )

    seen = set()

    def repl(m):
        key = m.group("key")
        if key not in tables_by_key:
            sys.exit(f"error: marker key {key!r} has no generated table")
        seen.add(key)
        return f"{m.group(1)}\n{tables_by_key[key]}\n{m.group(4)}"

    new_text = MARK.sub(repl, md_text)

    missing = sorted(set(tables_by_key) - seen)
    if missing:
        sys.exit(
            f"error: no marker found in {MD_PATH.name} for region(s): "
            + ", ".join(missing)
        )
    return new_text


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check",
        action="store_true",
        help="do not write; exit non-zero if the appendix is out of date",
    )
    args = parser.parse_args(argv)

    data = yaml.safe_load(YAML_PATH.read_text(encoding="utf-8"))
    parameters = data.get("parameters", [])
    tables = build_tables(parameters)

    original = MD_PATH.read_text(encoding="utf-8")
    updated = inject(original, tables)

    if args.check:
        if original != updated:
            sys.exit(
                f"error: {MD_PATH.name} is out of date; "
                "run 'python documentation/scripts/gen_param_appendix.py'"
            )
        print(f"{MD_PATH.name} is up to date.")
        return

    if original != updated:
        MD_PATH.write_text(updated, encoding="utf-8")
        print(f"Updated {MD_PATH.name}.")
    else:
        print(f"{MD_PATH.name} already up to date.")


if __name__ == "__main__":
    main()
