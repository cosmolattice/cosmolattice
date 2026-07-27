#!/bin/bash

echo """
============================================================================================================================
This script will build the project, and then run gprof to profile the project.
           THIS WILL TAKE A WHILE
============================================================================================================================
"""

# script dir
scrdir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cldir="${scrdir}/../"

mkdir -p ${scrdir}/gprof/build
cd ${scrdir}/gprof/build/

# Build with pg for gprof
cmake ${cldir} -DCMAKE_BUILD_TYPE=Release -DNOTHREADING=ON -DCMAKE_CXX_FLAGS="-pg" &>${scrdir}/gprof/gprof_config.log
make -j &>${scrdir}/gprof/gprof_build.log

# run
./lphi4 input=${scrdir}/lphi4.in expansion=false N=128 &>${scrdir}/gprof/gprof_run.log

gprof ${scrdir}/gprof/build/lphi4 gmon.out >${scrdir}/gprof/gprof_output.txt
