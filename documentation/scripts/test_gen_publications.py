#!/usr/bin/env python3
"""Tests for gen_publications.py.

Runnable directly (``python3 documentation/scripts/test_gen_publications.py``)
or via pytest. Nothing on disk is modified: the tests operate on in-memory
strings built from the committed publications.yaml.

They verify the guarantees the generator must provide:

1. Idempotency: injecting the generated block twice yields identical output.
2. Prose outside the ``@gen:publications`` / ``@endgen`` markers is byte-identical
   before and after a run — only the marker region changes.
3. Rendering fidelity: escaping is applied, $-math is normalised to \\(..\\),
   and every paper produces one parseable <li>.
"""

import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))

import gen_publications as gen  # noqa: E402  (after sys.path tweak)


def _pubs():
    return gen.yaml.safe_load(gen.YAML_PATH.read_text(encoding="utf-8")) or []


def _strip_marker_region(md_text):
    return gen.MARK.sub(lambda m: f"{m.group(1)}{m.group(3)}", md_text)


def test_idempotent():
    pubs = _pubs()
    md = gen.MD_PATH.read_text(encoding="utf-8")
    once = gen.MARK.sub(lambda _m: gen.build_block(pubs), md, count=1)
    twice = gen.MARK.sub(lambda _m: gen.build_block(pubs), once, count=1)
    assert once == twice, "generator is not idempotent"


def test_prose_untouched():
    pubs = _pubs()
    md = gen.MD_PATH.read_text(encoding="utf-8")
    regenerated = gen.MARK.sub(lambda _m: gen.build_block(pubs), md, count=1)
    assert _strip_marker_region(md) == _strip_marker_region(regenerated), \
        "text outside the markers changed"


def test_one_li_per_paper():
    pubs = _pubs()
    block = gen.render_list(pubs)
    assert block.count("<li value=") == len(pubs)
    assert block.startswith("<ol>") and block.endswith("</ol>")


def test_generator_escapes_html():
    sample = [
        {"arxiv": "2605.04670", "title": "A & B <tag>",
         "authors": [{"name": "Foo Bar"}], "journal": "J. Phys. 1 (2026) 1"},
    ]
    li = gen.render_list(sample)
    assert "&amp;" in li and "&lt;tag&gt;" in li, "HTML not escaped"
    assert "<i>J. Phys. 1 (2026) 1</i>." in li, "journal not rendered"
    assert 'href="https://arxiv.org/abs/2605.04670"' in li


def test_clean_title_normalises_math():
    import clpubs  # pure function, no network
    clean, flagged = clpubs.clean_title("Inflaton $Z_2$-Symmetry and $$E=mc^2$$")
    assert "\\(Z_2\\)" in clean and "\\[E=mc^2\\]" in clean, "single-$ math not normalised"
    assert "$" not in clean, "raw $ delimiters remain"
    assert flagged, "a $-bearing title should be flagged for review"


def test_value_counts_down():
    pubs = _pubs()
    li = gen.render_list(pubs)
    first = li.index('<li value="')
    top_value = li[first:].split('"')[1]
    assert int(top_value) == len(pubs), "top entry should carry the highest value"


def _run():
    fns = [v for k, v in sorted(globals().items()) if k.startswith("test_")]
    for fn in fns:
        fn()
        print("ok  %s" % fn.__name__)
    print("\nall %d publications-generator tests passed" % len(fns))


if __name__ == "__main__":
    _run()
