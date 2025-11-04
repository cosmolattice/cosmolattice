#!/bin/bash

base_dir=$(pwd)/
tmp_dir=${base_dir}/tmp/
mkdir -p ${tmp_dir}

url=https://arxiv.org/src/2102.01031
archive=archive.tar.gz
code_dir="${base_dir}/../"

echo "---------------------------------------------------------------------"
echo "         Downloading LaTeX source..."
echo "---------------------------------------------------------------------"
wget -q --show-progress ${url} -O ${tmp_dir}/${archive}
# extract and make a file list
mkdir -p ${tmp_dir}/source
cd ${tmp_dir}/source
tar xfv ${tmp_dir}/${archive} >${tmp_dir}/file_list.txt
#rm ${tmp_dir}/${archive}

# get code files
mkdir -p ${tmp_dir}/code_files
cp ${code_dir}/src/models/parameter-files/lphi4SU2U1.in ${tmp_dir}/code_files/
cp ${code_dir}/src/models/parameter-files/lphi4SU2U1.in ${tmp_dir}/code_files/
cp ${code_dir}/src/models/lphi4SU2U1.h ${tmp_dir}/code_files/
cp ${code_dir}/src/models/lphi4.h ${tmp_dir}/code_files/
cp ${code_dir}/src/cosmolattice.cpp ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/evolvers/leapfrog.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/evolvers/evolver.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/evolvers/kernels/complexscalarkernels.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/definitions/gaugederivatives.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/initializers/modelinitializer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/initializers/scalefactorinitializer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/initializers/scalarsingletinitializer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/initializers/fluctuationsgenerator.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/initializers/su2initializer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/measurer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/meansmeasurer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/su2measurer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/energiesmeasurer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/energysnapshotmeasurer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/scalarsingletmeasurer.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/measurementsIO/measurementssaver.h ${tmp_dir}/code_files/
cp ${code_dir}/src/include/CosmoInterface/measurements/measurementsIO/spectrumsaver.h ${tmp_dir}/code_files/

# if venv does not exist, create it
cd ${tmp_dir}
if [[ ! -d "${tmp_dir}/.venv" ]]; then
  echo "---------------------------------------------------------------------"
  echo "         Building virtual environment..."
  echo "---------------------------------------------------------------------"
  python -m venv ${tmp_dir}/.venv
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

echo "---------------------------------------------------------------------"
echo "         Setting up new mkdocs project..."
echo "---------------------------------------------------------------------"
cd ${base_dir}
rm -rf ${base_dir}/manual/
mkdocs new manual
cp ${base_dir}/manual_scripts/mkdocs.yml ${base_dir}/manual/mkdocs.yml

echo "---------------------------------------------------------------------"
echo "         Splitting the LaTeX manual into markdown files..."
echo "---------------------------------------------------------------------"

cp -r ${base_dir}/manual_scripts/tex ${tmp_dir}/tex
cp -r ${tmp_dir}/source/* ${tmp_dir}/tex/

cd ${base_dir}/manual_scripts/
python ${base_dir}/manual_scripts/split.py ${tmp_dir}/source/mainV2.tex ${base_dir}/manual/docs/ ${tmp_dir}/

echo "---------------------------------------------------------------------"
echo "         Moving assets..."
echo "---------------------------------------------------------------------"
# convert all pdfs to pngs and move to assets
mv ${tmp_dir}/assets ${base_dir}/manual/docs/assets

cd ${base_dir}/manual
echo "---------------------------------------------------------------------"
echo "         Building the bibliography file from LaTeX sources..."
echo "---------------------------------------------------------------------"

# append the nav.yml to mkdocs.yml
cat ${tmp_dir}/nav.yml >>mkdocs.yml
bash ${base_dir}/manual_scripts/convertbiblio ${tmp_dir}/Bibliography.tex >refs.bib
echo "---------------------------------------------------------------------"
echo "         Building the manual with MkDocs..."
echo "---------------------------------------------------------------------"
mkdocs build --no-directory-urls

echo "---------------------------------------------------------------------"
echo "         Cleaning up temporary files."
echo "---------------------------------------------------------------------"
rm -rf ${tmp_dir}
