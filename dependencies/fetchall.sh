## All the dependencies:
## checks for availability of MPI. If not present, asks for confirmation to continue without mpi support. Unless you say '-f', which means no confirmation.
## takes one mandatory argument: the destination path
## optional (self explanatory): --no-hdf5 --no-pfft

if [[ "${1}" == "" ]]; then
  echo 'Usage: $0 /path/to/place/where/you/want/it'
  exit 1
fi

mkdir -p "${1}"

if which mpicc > /dev/null; then
  MPI_ON=1
else
  MPI_OFF=1
  read -p "It seems you have no MPI installed, or not accessible in your PATH. Do you want to continue without MPI support? [y/n] " -n 1 -r
  echo    # (optional) move to a new line
  if [[ $REPLY =~ ^[Yy]$ ]]
  then
      # do dangerous stuff
      echo "OK"
  else
    [[ "$0" = "$BASH_SOURCE" ]] && exit 1 || return 1
  fi
fi

# path at which this script lives:
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

## get FFTW
bash "${DIR}"/fftw3.sh "${1}" ${MPI_ON}

## get HDF5
if [[ ! "$@ " =~ "--no-hdf5 " ]]; then
bash "${DIR}"/hdf5.sh "${1}" ${MPI_ON}
fi

## get PFFT
if [[ ! "$@ " =~ "--no-pfft " ]]; then
if [[ ${MPI_ON} -eq 1 ]]; then
  bash "${DIR}"/pfft.sh "${1}" "${1}"
fi
fi

