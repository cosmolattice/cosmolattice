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
    pip install -q regex
    pip install -q mkdocs-material
    pip install -q markdown-katex
    pip install -q mkdocs-autorefs
    pip install -q mkdocs-bibtex
    pip install -q mkdoxy
    pip install -q mkdocs-gen-files
else
    source ${tmp_dir}/.venv/bin/activate
fi

cd ${base_dir}
