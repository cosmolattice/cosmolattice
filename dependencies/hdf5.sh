#!/bin/bash

# This what you may need to change, if you want the newest version for instance. You can check if something newer was released
# or if the url is broken https://www.hdfgroup.org/downloads/hdf5/. You can also just download manually the archive and place
# it where you want it to be installed (first argument of this script).

CURURL="https://www.hdfgroup.org/package/hdf5-1-12-0-tar-gz/?wpdmdl=14582&refresh=60058b7b074801610976123"
CURNAME="hdf5-1.12.0"
CURFOLDER=hdf5-1.12.0


# Actual code

if [[ "${1}" == "" ]]; then
  echo 'Usage: $0 /path/to/place/where/you/want/it [1], where 1 is your optional flag to specify that you DO have mpi installed.'
  exit 1
fi # help message if no arguments are passed to the script.

if [[ ${2} == 1 || "${2}" == "--parallel" ]]; then
  MPIOPTION=--enable-parallel
  COMPILER=mpicc
  echo "hdf5 will be compiled with parallel flags. If you want to have at the same time a serial version, DO NOT install it in the same place."
else
  MPIOPTION=""
  COMPILER=gcc
fi #if 1, then we enable mpi for compilation.

# path at which this script lives:
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# We call the generic installer for download, uncompressing and compilation.
CC=${COMPILER} \
URL=${CURURL} \
CONFIGFLAGS="${MPIOPTION}" \
FORMAT="tgz" \
NAME=${CURNAME} \
DEST="${1}" \
FOLDER=${CURFOLDER} \
CMAKEFILE="../src/cmake/auto_install_paths.cmake" \
bash \
${DIR}/generic_gnu_config_installer.sh
