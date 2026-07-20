#!/usr/bin/env bash
#
# check_params_sync.sh — verify the parameter database is internally consistent
# in three directions:
#
#   1. parameters.yaml  ->  Appendix_Parameters.md  (the appendix is generated)
#   2. C++ source       <-> parameters.yaml         (no parameter has drifted)
#   3. models.yaml      <-> models/*.h              (the model registry matches
#                                                    the shipped model headers)
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

# NOTE: failures here are reported as WARNINGS and never abort the build. The
# checks flag drift between parameters.yaml, the generated appendix, and the C++
# call sites, but a docs build should still succeed so the site can be published.
# Run the checks directly (or via CI) when you want a hard pass/fail.
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PYTHON="${PYTHON:-python3}"

warned=0

# Run a check; on non-zero exit, print a warning (plus any extra hint lines
# passed as further arguments) and keep going instead of failing the build.
run_check() {
    local description="$1"; shift
    # Split args into the command (up to the first "--") and trailing hint lines.
    local -a cmd=() hints=()
    local seen_sep=0
    for arg in "$@"; do
        if [ "$arg" = "--" ] && [ "$seen_sep" -eq 0 ]; then
            seen_sep=1
            continue
        fi
        if [ "$seen_sep" -eq 0 ]; then cmd+=("$arg"); else hints+=("$arg"); fi
    done
    if ! "${cmd[@]}"; then
        echo "" >&2
        echo "warning: $description" >&2
        local hint
        for hint in ${hints[@]+"${hints[@]}"}; do
            echo "         $hint" >&2
        done
        warned=1
    fi
}

echo "==> [1/3] Appendix <- parameters.yaml"

echo "    Running generator unit tests"
run_check "generator unit tests failed" \
    "$PYTHON" "$SCRIPT_DIR/test_gen_param_appendix.py"

echo "    Checking the appendix is in sync with parameters.yaml"
run_check "the parameter appendix is out of sync with parameters.yaml." \
    "$PYTHON" "$SCRIPT_DIR/gen_param_appendix.py" --check \
    -- "Run 'make gen-params' in documentation/ and commit the result."

echo "==> [2/3] Code <-> parameters.yaml"

echo "    Running drift-checker unit tests"
run_check "drift-checker unit tests failed" \
    "$PYTHON" "$SCRIPT_DIR/test_check_params_code.py"

echo "    Checking parameters.yaml matches the get<> call sites"
run_check "parameters.yaml does not match the get<> call sites." \
    "$PYTHON" "$SCRIPT_DIR/check_params_code.py"

echo "==> [3/3] Model registry <-> code"

echo "    Checking models.yaml lists exactly the models in the code"
run_check "the model registry (models.yaml) does not match the models in the code." \
    "$PYTHON" "$SCRIPT_DIR/check_models_registry.py" \
    -- "Update documentation/source/data/models.yaml and run 'make gen-model-caps'."

if [ "$warned" -eq 0 ]; then
    echo "OK: parameter database is in sync (appendix, code, and model registry)."
else
    echo "WARNING: parameter database checks reported issues (see above); continuing anyway." >&2
fi

exit 0
