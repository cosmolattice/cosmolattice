#!/usr/bin/env bash
#
# check_params_sync.sh — verify the parameter database is internally consistent
# in both directions:
#
#   1. parameters.yaml  ->  Appendix_Parameters.md  (the appendix is generated)
#   2. C++ source       <-> parameters.yaml         (no parameter has drifted)
#
# Run this locally before committing changes to parameters.yaml, the appendix,
# or any get<>/getOverride<>/getSeed call site:
#
#     bash documentation/scripts/check_params_sync.sh
#     # or, from documentation/:  make check-params
#
# Direction 1 runs the generator unit tests and a no-write `--check`: it exits
# non-zero (without modifying any file) if the committed appendix would change.
# Direction 2 runs the code<->YAML drift checker (check_params_code.py) and its
# tests. The script is git-independent and self-contained, so it can later be
# dropped into CI as-is.
#
# Override the interpreter with PYTHON=... (defaults to python3); PyYAML must be
# importable by it (pip install pyyaml).

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PYTHON="${PYTHON:-python3}"

echo "==> [1/2] Appendix <- parameters.yaml"

echo "    Running generator unit tests"
"$PYTHON" "$SCRIPT_DIR/test_gen_param_appendix.py"

echo "    Checking the appendix is in sync with parameters.yaml"
if ! "$PYTHON" "$SCRIPT_DIR/gen_param_appendix.py" --check; then
    echo "" >&2
    echo "error: the parameter appendix is out of sync with parameters.yaml." >&2
    echo "       Run 'make gen-params' in documentation/ and commit the result." >&2
    exit 1
fi

echo "==> [2/2] Code <-> parameters.yaml"

echo "    Running drift-checker unit tests"
"$PYTHON" "$SCRIPT_DIR/test_check_params_code.py"

echo "    Checking parameters.yaml matches the get<> call sites"
"$PYTHON" "$SCRIPT_DIR/check_params_code.py"

echo "OK: parameter database is in sync (appendix and code)."
