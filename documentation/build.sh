#!/bin/bash

base_dir=$(pwd)/
tmp_dir=${base_dir}/tmp/
build_dir=${base_dir}/website/
mkdir -p ${tmp_dir}
mkdir -p ${build_dir}
mkdir -p ${tmp_dir}/code_source

# Use the local cosmolattice_private working tree (tracked files, including
# uncommitted edits) as the source scanned for @label line numbers. The
# displayed code still comes from public CLV2.0Alpha: SOURCE_ROOTS in
# hooks/resolve_emgithub.py builds the emgithub URLs against that branch, so the
# embedded snippets show alpha. NOTE: line numbers are only correct where a
# labeled file matches CLV2.0Alpha line-for-line in its labeled region.
repo_root=${base_dir}/..
mkdir -p ${tmp_dir}/code_source/cosmolattice
(cd ${repo_root} && git ls-files -z | tar --null -T - -cf -) | tar -xf - -C ${tmp_dir}/code_source/cosmolattice

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
