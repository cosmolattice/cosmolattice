#!/bin/bash

base_dir=$(pwd)/
tmp_dir=${base_dir}/tmp/
build_dir=${base_dir}/website/
mkdir -p ${tmp_dir}
mkdir -p ${build_dir}
mkdir ${tmp_dir}/code_source

git clone -b CLV2.0Alpha https://github.com/cosmolattice/cosmolattice.git ${tmp_dir}/code_source/cosmolattice
git clone https://github.com/cosmolattice/templat.git ${tmp_dir}/code_source/templat

source ./setup_python.sh
cd ${base_dir}

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
