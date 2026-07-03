#!/usr/bin/env python3
"""Tests for check_params_code.py.

Runnable directly (``python documentation/scripts/test_check_params_code.py``)
or via pytest. They verify that the code <-> YAML drift checker agrees on the
committed tree, detects injected drift, and handles the documented special
cases (comment stripping, dynamic ext_PS key).
"""

import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))

import check_params_code as chk  # noqa: E402  (after sys.path tweak)


def test_tree_is_in_sync():
    """The committed code and parameters.yaml agree (no drift)."""
    drifts = chk.reconcile(chk.extract_code_params(), chk.extract_yaml_params())
    assert drifts == [], f"unexpected code<->YAML drift: {drifts}"


def test_injected_code_only_name_is_detected():
    """A parameter read in code but absent from YAML is reported."""
    code = chk.extract_code_params()
    code["framework"] = set(code["framework"]) | {"__bogus_new_param__"}
    drifts = dict((s, (c, y)) for s, c, y in
                  chk.reconcile(code, chk.extract_yaml_params()))
    assert "framework" in drifts, "injected code-only name not detected"
    assert "__bogus_new_param__" in drifts["framework"][0]


def test_injected_yaml_only_name_is_detected():
    """A parameter in YAML but no longer read in code is reported."""
    yaml_scopes = chk.extract_yaml_params()
    yaml_scopes["framework"] = set(yaml_scopes["framework"]) | {"__removed_param__"}
    drifts = dict((s, (c, y)) for s, c, y in
                  chk.reconcile(chk.extract_code_params(), yaml_scopes))
    assert "framework" in drifts, "injected yaml-only name not detected"
    assert "__removed_param__" in drifts["framework"][1]


def test_comment_call_sites_are_ignored():
    """A get<> written inside a comment is not extracted (NMC 'lambda' case)."""
    sample = '''
        x = parser.get<double>("real_param");
        // example: by calling parser.get<double>("commented_param")
        /* block parser.get<int>("block_param") */
    '''
    names = chk._names_in(sample)
    assert "real_param" in names
    assert "commented_param" not in names
    assert "block_param" not in names


def test_string_literals_survive_comment_stripping():
    """// inside a string literal must not be treated as a comment."""
    sample = 'y = parser.get<std::string>("url_param", "http://x");'
    assert "url_param" in chk._names_in(sample)


def test_framework_count_is_sane():
    """Framework name set is in the expected range (sanity bound vs #52)."""
    n = len(chk.extract_code_params()["framework"])
    assert 40 <= n <= 80, f"framework parameter count {n} outside expected range"


def _run():
    tests = [
        test_tree_is_in_sync,
        test_injected_code_only_name_is_detected,
        test_injected_yaml_only_name_is_detected,
        test_comment_call_sites_are_ignored,
        test_string_literals_survive_comment_stripping,
        test_framework_count_is_sane,
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
