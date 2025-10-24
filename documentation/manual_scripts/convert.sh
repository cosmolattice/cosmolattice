#!/bin/bash

url=https://arxiv.org/src/2102.01031
archive=archive.tar.gz
code_folder="../../"

wget ${url} -O ${archive}
# extract and make a file list
tar xfv ${archive} >file_list.txt
rm ${archive}

# get code files
mkdir -p code_files
cp ${code_folder}/src/models/parameter-files/lphi4SU2U1.in ./code_files/
cp ${code_folder}/src/models/parameter-files/lphi4SU2U1.in ./code_files/
cp ${code_folder}/src/models/lphi4SU2U1.h ./code_files/
cp ${code_folder}/src/models/lphi4.h ./code_files/
cp ${code_folder}/src/cosmolattice.cpp ./code_files/
cp ${code_folder}/src/include/CosmoInterface/evolvers/leapfrog.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/evolvers/evolver.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/evolvers/kernels/complexscalarkernels.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/definitions/gaugederivatives.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/initializers/modelinitializer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/initializers/scalefactorinitializer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/initializers/scalarsingletinitializer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/initializers/fluctuationsgenerator.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/initializers/su2initializer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/measurer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/meansmeasurer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/su2measurer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/energiesmeasurer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/energysnapshotmeasurer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/scalarsingletmeasurer.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/measurementsIO/measurementssaver.h ./code_files/
cp ${code_folder}/src/include/CosmoInterface/measurements/measurementsIO/spectrumsaver.h ./code_files/

# if venv does not exist, create it
if [[ ! -d ".venv" ]]; then
    python -m venv .venv
    source .venv/bin/activate
    pip install regex
    pip install mkdocs-material
    pip install markdown-katex
    pip install mkdocs-bibtex
else
    source .venv/bin/activate
fi

cd ..
rm -rf manual/
mkdocs new manual
cp ./build_manual/mkdocs.yml ./manual/mkdocs.yml
cd -

cp ./mainV2.tex ./info.tx
echo "---------------------------------------------------------------------"
echo "         Splitting the LaTeX manual into markdown files..."
echo "---------------------------------------------------------------------"
python ./split.py ./mainV2.tex ../manual/docs
echo "---------------------------------------------------------------------"
echo "         Done splitting the manual."
echo "---------------------------------------------------------------------"
echo

cd ../manual
echo "---------------------------------------------------------------------"
echo "         Building the bibliography file from LaTeX sources..."
echo "---------------------------------------------------------------------"
echo
# append the nav.yml to mkdocs.yml
cat ./nav.yml >>mkdocs.yml
bash ../build_manual/convertbiblio docs/Bibliography.md >refs.bib
echo "---------------------------------------------------------------------"
echo "         Building the manual with MkDocs..."
echo "---------------------------------------------------------------------"
rm ./nav.yml
rm docs/Bibliography.md
mkdocs build --no-directory-urls
echo "---------------------------------------------------------------------"
echo "         Manual built in the ./site folder."
echo "---------------------------------------------------------------------"
echo
cd -

echo "---------------------------------------------------------------------"
echo "         Cleaning up temporary files."
echo "---------------------------------------------------------------------"
# clean up
rm -rf code_files
# read file_list.txt and remove extracted files
while read -r line; do
    rm -rf "$line"
done <file_list.txt
rm -rf _minted-mainV2
rm file_list.txt
