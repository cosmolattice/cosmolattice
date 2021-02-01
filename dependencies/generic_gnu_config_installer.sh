#!/bin/bash

# script by Wessel Valkenburg, 2017
#
# call with your settings as local environment variables upon calling. For example:
#
# URL="ftp://ftp.fftw.org/pub/fftw/fftw-3.3.4.tar.gz" \
# CONFIGFLAGS="--enable-mpi --enable-threads --enable-sse2 --enable-avx" \
# FORMAT="tgz" \
# NAME="FFTW" \
# FOLDER=fftw-3.3.4
# DEST="/your/path" \
# ./generic_gnu_config.sh
#
# where:
# URL is the url to fetch
# CONFIGFLAGS are the optional flags to pass to the gnu configure script
# PLAINMAKE should be set to some value if this is not a gnu configure script, rather only takes a plain 'make' command.
# FORMAT is either tgz (for a .tar.gz file) or zip. Other formats will be implemented whenever we need them.
# NAME is the name you want to use for this library
# FOLDER is the folder that lives in the archive and in which we will build stuff. No, this is not found automatically, you have to specify it.
# DEST is the path in which you want the library to be compiled and installed.

if [[ "${CUSTOMMAKE}" != "" ]]; then
  # switch off the './configure' and 'make install' steps.
  PLAINMAKE=1;
else
  # If it is a plain make without a custom make command, this is the make command:
  CUSTOMMAKE="make -j 8";
fi



if [[ "${DEST}" == "" ]]; then
  echo "Please specify a path in which to download and install ${NAME}."
  exit
fi

if [ ! -f "${DEST}" ]; then
  mkdir -p "${DEST}"
fi

ORIGINPATH=`pwd`
ABSPATH=$(cd -- "${DEST}" && pwd)


# Manually change this url to reflect new releases.. unfortunately no "current"-url available.
RCONFIGFLAGS=" --prefix=${ABSPATH} ${CONFIGFLAGS}"

if [[ ${FORMAT} == "tgz" ]]; then
  TGZ=${NAME}.tar.gz
  UNPACK="tar -zxvf"
elif [[ ${FORMAT} == "zip" ]]; then
  TGZ=${NAME}.zip
  UNPACK="unzip"
fi

if [[ "${FOLDER}" == "" ]]; then
  FOLDER=${NAME}
fi

function myexit {
  echo "Automated ${NAME} fetch-and-compile script finished."
  if [[ ! $error == 0 ]]; then
    echo "COMPILATION FAILED."
  fi
  exit $error
}

error=0





cd "${ABSPATH}"

if ! which wget > /dev/null ; then wget=0; else wget=1; fi

echo ${TGZ}

if [[ ! -f "${TGZ}" ]]
then
  if [[ ${wget} == 1 ]]
  then
    if ! wget -O "${TGZ}" "${URL}"
    then
      echo "Failed to fetch archive file for ${NAME}."
      error=1
      myexit
    fi
  else
    if ! curl -L -o "${TGZ}" "${URL}"
    then
      echo "Failed to fetch archive file for ${NAME}."
      error=1
      myexit
    fi
  fi
fi

if ! ${UNPACK} "${TGZ}"
then
  echo "Failed to unpack file for ${NAME}."
  error=2
  myexit
fi

cd "${FOLDER}"
pwd

if [[ "${PLAINMAKE}" == "" ]]; then
  if ! ./configure ${RCONFIGFLAGS}
  then
    echo "Failed to configure ${NAME}."
    error=3
    myexit
  fi
fi

if ! ${CUSTOMMAKE}
then
  echo "Failed to build ${NAME}."
  error=4
  myexit
fi

if [[ "${PLAINMAKE}" == "" ]]; then
  if ! make install
  then
    echo "Failed to copy build products of ${NAME} to final destination."
    error=5
    myexit
  fi

fi


# Add the path to some CMake file (optional)

if [[ "${CMAKEFILE}" != "" ]]; then
cd ${ORIGINPATH}
if ! grep -q "${ABSPATH}" ${CMAKEFILE}
then
	echo  'set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};'"${ABSPATH};"'")' >> ${CMAKEFILE}
fi
fi


# clean up intermediate build products, leave the tar ball there:
# rm -rf "${ABSPATH}"/"${FOLDER}"

if [[ "${PLAINMAKE}" == "" ]]; then
  # safe guard for systems that create architecture-dependent library folders:
  mkdir -p "${ABSPATH}/lib"
  cd "${ABSPATH}/lib"
  find "${ABSPATH}/" -name lib\*.a | grep -v "${ABSPATH}/lib/" | while read -r y; do ln -s "${y}" .; done

fi




myexit

