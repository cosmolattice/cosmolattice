#!/bin/bash

# This what you may need to change, if you want the newest version for instance. You can check if something newer was released
# or if the url is broken http://www.fftw.org/download.html . You can also just download manually the archive and place
# it where you want it to be installed (first argument of this script).


CURURL="http://fftw.org/fftw-3.3.9.tar.gz"
CURNAME="fftw-3.3.9"
CURFOLDER=fftw-3.3.9

#Actual code

if [[ "${1}" == "" ]]; then
  echo 'Usage: $0 /path/to/place/where/you/want/it [1], where 1 is your optional flag to specify that you DO have mpi installed.'
  exit 1
fi # help message if no arguments are passed along.

if [[ ${2} == 1 || "${2}" == "--parallel" ]]; then
  MPIOPTION=--enable-mpi
  echo "fftw3 will be compiled with parallel flags."
else
  MPIOPTION=""
fi #if 1, then we enable mpi for compilation.

if [[ ${3} == 1 || "${3}" == "--float" ]]; then
  FLOATOPTION=--enable-float
  echo "fftw3 will be compiled for float."
else
  FLOATOPTION=""
fi #if 1, then we enable mpi for compilation.

# path at which this script lives:
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# We call the generic installer for download, uncompressing and compilation.
URL=${CURURL} \
CONFIGFLAGS="${MPIOPTION} --enable-threads --enable-sse2 --enable-avx --disable-shared --enable-static ${FLOATOPTION}" \
FORMAT="tgz" \
NAME=${CURNAME} \
DEST="${1}" \
FOLDER=${CURFOLDER} \
CMAKEFILE="../src/cmake/auto_install_paths.cmake" \
bash \
${DIR}/generic_gnu_config_installer.sh

