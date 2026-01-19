#!/bin/bash

base_dir="$(pwd)/"
source "${base_dir}/setup_python.sh"

manual_name="Manual"
tmp_dir="${base_dir}/tmp/manual/"
doc_dir="${base_dir}/website/docs/${manual_name}/"
mkdir -p "${tmp_dir}"
mkdir -p "${doc_dir}"
mkdir -p "${doc_dir}/assets"

url=https://arxiv.org/src/2102.01031
archive=manual_archive.tar.gz
code_dir="${base_dir}/../"

echo "---------------------------------------------------------------------"
echo "         Downloading LaTeX source..."
echo "---------------------------------------------------------------------"
wget -q --show-progress ${url} -O ${tmp_dir}/${archive}
# extract and make a file list
mkdir -p "${tmp_dir}/source"
cd "${tmp_dir}/source"
tar xfv "${tmp_dir}/${archive}" >"${tmp_dir}/file_list.txt"
rm "${tmp_dir}/${archive}"

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

echo "---------------------------------------------------------------------"
echo "         Splitting the LaTeX manual into markdown files..."
echo "---------------------------------------------------------------------"

cp -r "${base_dir}/scripts/tex" "${tmp_dir}/tex"
# cp -r "${tmp_dir}/source/*" "${tmp_dir}/tex/"

cd "${base_dir}/scripts/"
python "${base_dir}/scripts/split_manual.py" "${tmp_dir}/source/mainV2.tex" "${doc_dir}" "${tmp_dir}/" "${manual_name}"

echo "---------------------------------------------------------------------"
echo "         Moving assets..."
echo "---------------------------------------------------------------------"

# convert all pdfs to pngs and move to assets
cp -r "${tmp_dir}/assets/"* "${doc_dir}/assets/"
