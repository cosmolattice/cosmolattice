#!/bin/bash

base_dir=$(pwd)/
tmp_dir=${base_dir}/tmp/
mkdir -p ${tmp_dir}

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
else
    source ${tmp_dir}/.venv/bin/activate
fi

cd ${base_dir}
