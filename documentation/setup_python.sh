#!/bin/bash

base_dir=$(pwd)/
tmp_dir=${base_dir}/tmp/
mkdir -p ${tmp_dir}

pythonexec=""
if command -v python3 &>/dev/null; then
    pythonexec=$(command -v python3)
elif command -v python &>/dev/null; then
    pythonexec=$(command -v python)
else
    echo "Python is not installed. Please install Python3 and try again."
    exit 1
fi

# if venv does not exist, create it
cd ${tmp_dir}

if [[ ! -d "${tmp_dir}/.venv" ]]; then
    echo "---------------------------------------------------------------------"
    echo "         Building virtual environment..."
    echo "---------------------------------------------------------------------"
    python3 -m venv ${tmp_dir}/.venv
    source ${tmp_dir}/.venv/bin/activate
    pip install -q --upgrade pip
else
    source ${tmp_dir}/.venv/bin/activate
fi

# Pinned dependency set; idempotent and fast once installed, and also brings
# pre-existing venvs up to the pinned versions.
pip install -q -r ${base_dir}/requirements.txt

cd ${base_dir}
