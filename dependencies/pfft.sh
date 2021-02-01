#!/bin/bash


#Actual code.

if [[ "${1}" == "" || "${2}" == "" ]]; then
  echo 'Usage: $0 /path/to/place/where/you/want/it /path/to/fftw'
  exit 1
fi # help message if no arguments are passed to the script. Need to specify where fftw is.

# path at which this script lives:
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

FFTWPATH=$(cd -- "${2}" && pwd)

if [[ ${3} == 1 || "${3}" == "--float" ]]; then
  extraFlag=--enable-float
  echo "fftw3 will be compiled for float."
else
  extraFlag=""
fi #if 1, then we enable mpi for compilation.

export LDFLAGS="-L${FFTWPATH}/lib"
export DYLD_LIBRARY_PATH="${FFTWPATH}/lib"
export LD_LIBRARY_PATH="${FFTWPATH}/lib"

if [[ "${extraFlag}" == "" ]]; then
export LIBS="-lfftw3_mpi -lfftw3"
else
export LIBS="-lfftw3f_mpi -lfftw3f"
fi

LDFLAGS="${LDFLAGS}" \
LIBS="${LIBS}" \
CFLAGS="-g3" \
URL="https://www-user.tu-chemnitz.de/~potts/workgroup/pippig/software/pfft-1.0.8-alpha.tar.gz" \
CONFIGFLAGS="--with-fftw3=${FFTWPATH} --disable-fortran --disable-shared --enable-static ${extraFlag}" \
FORMAT="tgz" \
NAME="pfft-1.0.8-alpha" \
DEST="${1}" \
FOLDER=pfft-1.0.8-alpha \
CMAKEFILE="../src/cmake/auto_install_paths.cmake" \
CXX=mpic++ \
CC=mpicc \
bash \
${DIR}/generic_gnu_config_installer.sh

