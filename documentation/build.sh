#!/bin/bash

base_dir=$(pwd)/
tmp_dir=${base_dir}/tmp/
build_dir=${base_dir}/website/
mkdir -p ${tmp_dir}
mkdir -p ${build_dir}
mkdir -p ${tmp_dir}/code_source

# Code source for the API reference (mkdoxy), @label scanning
# (hooks/resolve_emgithub.py) and the parameter/model drift checks.
#
# Default: shallow clone of the public cosmolattice repo at CL_CODE_BRANCH,
# so the build needs no access to any private repo and only public headers
# can ever be rendered. Developers with a local checkout can override with
#   CL_CODE_SOURCE=/path/to/cosmolattice bash build.sh
# which stages that checkout's tracked files (including uncommitted edits).
# NOTE: @emgithub line numbers are only correct where a labeled file matches
# CL_CODE_BRANCH line-for-line in its labeled region.
CL_CODE_BRANCH=${CL_CODE_BRANCH:-CLV2.0Alpha}
code_dir=${tmp_dir}/code_source/cosmolattice
if [ -n "${CL_CODE_SOURCE:-}" ]; then
    rm -rf ${code_dir}
    mkdir -p ${code_dir}
    (cd ${CL_CODE_SOURCE} && git ls-files -z | tar --null -T - -cf -) | tar -xf - -C ${code_dir}
elif [ -d ${code_dir}/.git ]; then
    (cd ${code_dir} && git fetch --depth 1 origin ${CL_CODE_BRANCH} && git checkout -q FETCH_HEAD)
else
    rm -rf ${code_dir}
    git clone --depth 1 --branch ${CL_CODE_BRANCH} https://github.com/cosmolattice/cosmolattice.git ${code_dir}
fi

# Only pull templat if it's not already present
if [ ! -d ${tmp_dir}/code_source/templat ]; then
    git clone https://github.com/cosmolattice/templat.git ${tmp_dir}/code_source/templat
else
    # otherwise, git pull to update it
    (cd ${tmp_dir}/code_source/templat && git pull)
fi

source ./setup_python.sh
cd ${base_dir}

echo "---------------------------------------------------------------------"
echo "         Checking the parameter appendix is in sync with the YAML..."
echo "---------------------------------------------------------------------"

# The parameter appendix (source/docs/Manual/Appendix_Parameters.md) is generated
# from source/data/parameters.yaml. Drift between the YAML, the appendix, and the
# C++ call sites is reported as a WARNING only: the script never fails the build,
# so the site still publishes. Run check_params_sync.sh directly for a hard check.
bash ${base_dir}/scripts/check_params_sync.sh

echo "---------------------------------------------------------------------"
echo "         Checking the publications list is in sync with the YAML..."
echo "---------------------------------------------------------------------"

# The Using CL list in source/docs/publications/Publications.md is generated from
# source/data/publications.yaml by scripts/gen_publications.py. Reported as a
# WARNING only: the site still publishes if it has drifted.
"${PYTHON:-python3}" ${base_dir}/scripts/gen_publications.py --check || \
  echo "warning: Publications.md is out of sync with publications.yaml (run 'make gen-pubs' and commit)."

# now get the latex stuff
# bash ${base_dir}/convert_arxiv_manual.sh

# copy source files to build directory
cp -r ${base_dir}/source/* ${build_dir}/

echo "---------------------------------------------------------------------"
echo "         Setting up new mkdocs project..."
echo "---------------------------------------------------------------------"

cd ${base_dir}
mkdocs new ${build_dir}
cd ${build_dir}

echo "---------------------------------------------------------------------"
echo "         Building the bibliography file from LaTeX sources..."
echo "---------------------------------------------------------------------"

# source bib
# cp ${base_dir}/source/refs.bib refs.bib

# append the generated nav.yml and bibliography from converted latex
# manual
# cat ${tmp_dir}/manual/nav.yml >>mkdocs.yml
# I did a manual merge of the bibliographies.
# cat ${tmp_dir}/manual/Bibliography.tex >>${tmp_dir}/manual/Bibliography.tex

# convert the bibliography to bib file
# bash ${base_dir}/scripts/convertbiblio ${tmp_dir}/manual/Bibliography.tex >>refs.bib

echo "---------------------------------------------------------------------"
echo "         Building the manual with MkDocs..."
echo "---------------------------------------------------------------------"

mkdocs build
