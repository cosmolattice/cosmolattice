#!/usr/bin/env python3
"""Tests for gen_param_appendix.py.

Runnable directly (``python documentation/scripts/test_gen_param_appendix.py``)
or via pytest. The tests operate on a temporary copy of the appendix so the
checked-in file is never modified.

They verify the two guarantees the generator must provide:

1. Idempotency: generating twice yields a byte-identical file.
2. Prose outside the ``@gen:params`` / ``@endgen`` markers is byte-identical
   before and after a run -- only the marker regions change.
"""

import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))

import gen_param_appendix as gen  # noqa: E402  (after sys.path tweak)


def _strip_marker_regions(md_text):
    """Return the text with every marker region's inner content blanked out.

    Markers themselves are kept; only the generated body between a
    ``@gen:params`` opener and its ``@endgen`` closer is removed. Two documents
    that share this projection have identical out-of-marker prose.
    """
    return gen.MARK.sub(lambda m: f"{m.group(1)}{m.group(4)}", md_text)


def _tables():
    data = gen.yaml.safe_load(gen.YAML_PATH.read_text(encoding="utf-8"))
    return gen.build_tables(data.get("parameters", []))


def test_idempotent():
    """Injecting twice produces identical output."""
    tables = _tables()
    original = gen.MD_PATH.read_text(encoding="utf-8")
    once = gen.inject(original, tables)
    twice = gen.inject(once, tables)
    assert once == twice, "generator is not idempotent"


def test_prose_outside_markers_unchanged():
    """Only marker regions change; all surrounding prose is byte-identical."""
    tables = _tables()
    original = gen.MD_PATH.read_text(encoding="utf-8")
    updated = gen.inject(original, tables)
    assert _strip_marker_regions(original) == _strip_marker_regions(updated), (
        "prose outside the markers changed during generation"
    )


def test_committed_file_is_up_to_date():
    """The checked-in appendix matches what the generator would produce."""
    tables = _tables()
    original = gen.MD_PATH.read_text(encoding="utf-8")
    assert gen.inject(original, tables) == original, (
        "Appendix_Parameters.md is out of date; run gen_param_appendix.py"
    )


def test_all_regions_have_markers():
    """Every generated region key has a corresponding marker in the file."""
    tables = _tables()
    original = gen.MD_PATH.read_text(encoding="utf-8")
    keys_in_file = {m.group("key") for m in gen.MARK.finditer(original)}
    assert set(tables) == keys_in_file, (
        f"region/marker mismatch: tables={sorted(tables)} "
        f"markers={sorted(keys_in_file)}"
    )


def _run():
    tests = [
        test_idempotent,
        test_prose_outside_markers_unchanged,
        test_committed_file_is_up_to_date,
        test_all_regions_have_markers,
    ]
    failures = 0
    for t in tests:
        try:
            t()
        except AssertionError as exc:
            failures += 1
            print(f"FAIL {t.__name__}: {exc}")
        else:
            print(f"ok   {t.__name__}")
    if failures:
        sys.exit(f"{failures} test(s) failed")
    print("all tests passed")


if __name__ == "__main__":
    _run()
