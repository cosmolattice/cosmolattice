#!/usr/bin/env bash
# Launch the CosmoLattice input-file builder GUI.
# Creates a local virtualenv on first run, then opens the webapp in your browser.
set -euo pipefail
cd "$(dirname "$0")"

VENV=".venv"
if [ ! -d "$VENV" ]; then
  echo "First run: creating virtualenv and installing dependencies..."
  python3 -m venv "$VENV"
  "$VENV/bin/pip" install --quiet --upgrade pip
  "$VENV/bin/pip" install --quiet -r requirements.txt
fi

exec "$VENV/bin/streamlit" run app.py
