#!/bin/bash

echo """
============================================================================================================================
This script will build the project, and then run massif to profile the memory usage of the project.
With N=64 we expect to see memory usage around 64^3 * 2 * 2 * 8 bytes = 8MiB 
(64^3 grid points, 2 fields with pi/phi each, 8 bytes per double).
           THIS WILL TAKE A WHILE
============================================================================================================================
"""

# script dir
scrdir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cldir="${scrdir}/../"

mkdir -p ${scrdir}/massif/build
cd ${scrdir}/massif/build/

# Notice:
# - sanitizer will cause massif to fail
# - avx512 will cause massif to fail (so, to be safe, we kill all the simd by just doing a debug build)
cmake ${cldir} -DCMAKE_BUILD_TYPE=Debug -DNOTHREADING=ON -DNOSANITIZER=ON &>${scrdir}/massif/massif_config.log
make -j &>${scrdir}/massif/massif_build.log

# run massif,
valgrind --tool=massif ./lphi4 input=${scrdir}/lphi4.in expansion=false N=64 tMax=0.02 &>${scrdir}/massif/massif_run.log

# get the latest massif output file
massif_file=$(ls -t massif.out.* | head -n 1)

massif-visualizer ${massif_file} &>/dev/null
