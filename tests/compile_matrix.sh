#!/usr/bin/env bash
#
# compile_matrix.sh -- compile every CosmoLattice model against every meaningful
# permutation of the PARAFAFT / MPI / HDF5 options.
#
# PARAFAFT=ON with MPI=OFF is not a real configuration: TempLat warns and drops
# ParaFaFT, so it would just duplicate the PARAFAFT=OFF build. Those two
# combinations are excluded, leaving 6 permutations rather than 8.
#
# The matrix is walked permutation-major: for each flag combination a single
# throwaway build directory is configured once (so Kokkos, TempLat, ParaFaFT and
# HDF5 are built once per permutation) and then every model is compiled in it by
# re-running cmake with a new -DMODEL. All build directories live under one
# temporary root which is removed on exit.
#
# Logs are written to a separate directory that survives the cleanup whenever
# something fails, so a failing build can still be diagnosed.
#
# Usage: tests/compile_matrix.sh [options]
# Run with --help for the full list of options.

set -uo pipefail

# ##############################################################################
# Defaults
# ##############################################################################

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
SRC_DIR=$(cd -- "${SCRIPT_DIR}/.." && pwd)

JOBS=$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)
BUILD_TYPE=Release
AUTOBUILD_HDF5=ON
KEEP_BUILDS=0
DRY_RUN=0
LIST_ONLY=0
LOG_ROOT=""
MODEL_FILTER=""
PERM_FILTER=""
EXTRA_CMAKE_ARGS=()

# ##############################################################################
# Pretty printing
# ##############################################################################

if [[ -t 1 ]]; then
  C_RESET=$'\033[0m'; C_BOLD=$'\033[1m'; C_DIM=$'\033[2m'
  C_RED=$'\033[31m'; C_GREEN=$'\033[32m'; C_YELLOW=$'\033[33m'; C_BLUE=$'\033[34m'
else
  C_RESET=""; C_BOLD=""; C_DIM=""; C_RED=""; C_GREEN=""; C_YELLOW=""; C_BLUE=""
fi

info()  { printf '%s==>%s %s\n' "${C_BLUE}${C_BOLD}" "${C_RESET}" "$*"; }
warn()  { printf '%s[warn]%s %s\n' "${C_YELLOW}" "${C_RESET}" "$*" >&2; }
die()   { printf '%s[error]%s %s\n' "${C_RED}${C_BOLD}" "${C_RESET}" "$*" >&2; exit 2; }

usage() {
  cat <<EOF
${C_BOLD}compile_matrix.sh${C_RESET} -- compile every model against every PARAFAFT/MPI/HDF5 permutation.

PARAFAFT=ON with MPI=OFF is skipped: TempLat ignores ParaFaFT without MPI, so it
is not a distinct configuration. That leaves 6 permutations.

${C_BOLD}Options:${C_RESET}
  -j, --jobs N            Parallel compile jobs per build (default: ${JOBS})
  -m, --models LIST       Comma-separated model names to build
                          (default: every models/*.h in the repo)
  -p, --perms LIST        Comma-separated 3-char permutation masks to run,
                          in PARAFAFT/MPI/HDF5 order. Use the flag letter for ON
                          and '-' for OFF, e.g. '--perms PMH,-M-,---'
      --no-autobuild-hdf5 Do not pass -DAUTOBUILD_HDF5=ON. HDF5=ON permutations
                          will then fail unless HDF5 is installed system-wide.
      --build-type TYPE   CMAKE_BUILD_TYPE (default: ${BUILD_TYPE})
  -D<var>=<value>         Extra argument forwarded verbatim to every cmake
                          configure call (repeatable)
      --log-dir DIR       Write logs here instead of a temporary directory.
                          This directory is never deleted.
  -k, --keep              Keep the temporary build root instead of deleting it
  -n, --dry-run           Print what would be built, run nothing
  -l, --list              List the discovered models and permutations, then exit
  -h, --help              Show this help

${C_BOLD}Exit status:${C_RESET} 0 if every attempted build succeeded, 1 otherwise.
EOF
}

# ##############################################################################
# Argument parsing
# ##############################################################################

while [[ $# -gt 0 ]]; do
  case "$1" in
    -j|--jobs)            JOBS="${2:?--jobs needs a value}"; shift 2 ;;
    -m|--models)          MODEL_FILTER="${2:?--models needs a value}"; shift 2 ;;
    -p|--perms)           PERM_FILTER="${2:?--perms needs a value}"; shift 2 ;;
    --no-autobuild-hdf5)  AUTOBUILD_HDF5=OFF; shift ;;
    --build-type)         BUILD_TYPE="${2:?--build-type needs a value}"; shift 2 ;;
    --log-dir)            LOG_ROOT="${2:?--log-dir needs a value}"; shift 2 ;;
    -k|--keep)            KEEP_BUILDS=1; shift ;;
    -n|--dry-run)         DRY_RUN=1; shift ;;
    -l|--list)            LIST_ONLY=1; shift ;;
    -h|--help)            usage; exit 0 ;;
    -D*)                  EXTRA_CMAKE_ARGS+=("$1"); shift ;;
    *)                    die "unknown option '$1' (try --help)" ;;
  esac
done

[[ "${JOBS}" =~ ^[0-9]+$ && "${JOBS}" -gt 0 ]] || die "--jobs must be a positive integer, got '${JOBS}'"
command -v cmake >/dev/null || die "cmake not found in PATH"

# ##############################################################################
# Discover models
# ##############################################################################

ALL_MODELS=()
for f in "${SRC_DIR}"/models/*.h; do
  [[ -e "$f" ]] || continue
  ALL_MODELS+=("$(basename "$f" .h)")
done
((${#ALL_MODELS[@]})) || die "no models found in ${SRC_DIR}/models/"

MODELS=()
if [[ -n "${MODEL_FILTER}" ]]; then
  IFS=',' read -r -a requested <<<"${MODEL_FILTER}"
  for want in "${requested[@]}"; do
    want="${want// /}"
    [[ -n "${want}" ]] || continue
    [[ -f "${SRC_DIR}/models/${want}.h" ]] || die "no such model: models/${want}.h"
    MODELS+=("${want}")
  done
  ((${#MODELS[@]})) || die "--models matched nothing"
else
  MODELS=("${ALL_MODELS[@]}")
fi

# ##############################################################################
# Build the permutation list
#
# Each permutation is encoded as "P:M:H" with 0/1 values, plus a 3-char display
# mask. PARAFAFT=ON with MPI=OFF is not generated at all: TempLat warns and
# disables ParaFaFT in that case (see the `elseif(PARAFAFT)` branch in TempLat's
# CMakeLists.txt), so the build would be identical to the PARAFAFT=OFF one.
# ##############################################################################

PERM_KEYS=(); PERM_MASKS=()
for p in 0 1; do
  for m in 0 1; do
    # ParaFaFT is meaningless without MPI.
    [[ $p == 1 && $m == 0 ]] && continue
    for h in 0 1; do
      mask=""
      [[ $p == 1 ]] && mask+="P" || mask+="-"
      [[ $m == 1 ]] && mask+="M" || mask+="-"
      [[ $h == 1 ]] && mask+="H" || mask+="-"
      if [[ -n "${PERM_FILTER}" && ",${PERM_FILTER}," != *",${mask},"* ]]; then
        continue
      fi
      PERM_KEYS+=("${p}:${m}:${h}")
      PERM_MASKS+=("${mask}")
    done
  done
done
# Report a mistyped mask explicitly rather than letting it silently select nothing.
if [[ -n "${PERM_FILTER}" ]]; then
  IFS=',' read -r -a wanted_masks <<<"${PERM_FILTER}"
  for want in "${wanted_masks[@]}"; do
    want="${want// /}"
    [[ -n "${want}" ]] || continue
    [[ "${want}" =~ ^[P-][M-][H-]$ ]] \
      || die "invalid permutation mask '${want}': expected 3 characters in PARAFAFT/MPI/HDF5 order, e.g. 'PMH', '-M-', '---'"
    if [[ "${want}" == P-* ]]; then
      die "permutation '${want}' is not built: ParaFaFT requires MPI, so TempLat would silently disable it. Did you mean 'PM${want:2:1}' (ParaFaFT with MPI) or '--${want:2:1}' (neither)?"
    fi
  done
fi
((${#PERM_KEYS[@]})) || die "no permutations selected (check --perms)"

if [[ ${LIST_ONLY} == 1 ]]; then
  printf '%sModels (%d):%s\n' "${C_BOLD}" "${#MODELS[@]}" "${C_RESET}"
  printf '  %s\n' "${MODELS[@]}"
  printf '%sPermutations (%d), mask is PARAFAFT/MPI/HDF5:%s\n' \
    "${C_BOLD}" "${#PERM_MASKS[@]}" "${C_RESET}"
  printf '  %s\n' "${PERM_MASKS[@]}"
  printf '  %s(PARAFAFT=ON with MPI=OFF is excluded: ParaFaFT requires MPI.)%s\n' \
    "${C_DIM}" "${C_RESET}"
  printf '%sTotal builds: %d%s\n' "${C_BOLD}" "$(( ${#MODELS[@]} * ${#PERM_MASKS[@]} ))" "${C_RESET}"
  exit 0
fi

# ##############################################################################
# Pre-flight checks (advisory only -- cmake gives the authoritative answer)
# ##############################################################################

needs_mpi=0
for key in "${PERM_KEYS[@]}"; do
  [[ "${key}" == *":1:"* ]] && needs_mpi=1
done
if [[ ${needs_mpi} == 1 ]] && ! command -v mpicxx >/dev/null 2>&1; then
  warn "mpicxx not found in PATH; the MPI=ON permutations will probably fail to configure."
fi
if [[ ${AUTOBUILD_HDF5} == OFF ]]; then
  warn "AUTOBUILD_HDF5 is off; HDF5=ON permutations require a system HDF5 install."
fi

# ##############################################################################
# Temporary directories and cleanup
# ##############################################################################

BUILD_ROOT=$(mktemp -d "${TMPDIR:-/tmp}/cl-compile-matrix.XXXXXX") \
  || die "could not create a temporary build root"

LOG_ROOT_IS_TEMP=0
if [[ -z "${LOG_ROOT}" ]]; then
  LOG_ROOT=$(mktemp -d "${TMPDIR:-/tmp}/cl-compile-matrix-logs.XXXXXX") \
    || die "could not create a temporary log directory"
  LOG_ROOT_IS_TEMP=1
else
  mkdir -p "${LOG_ROOT}" || die "could not create log directory ${LOG_ROOT}"
  LOG_ROOT=$(cd -- "${LOG_ROOT}" && pwd)
fi

FAILURES=0

cleanup() {
  # Guard against ever running rm -rf on an empty or unexpected path.
  if [[ ${KEEP_BUILDS} == 1 ]]; then
    info "Keeping build root: ${BUILD_ROOT}"
  elif [[ -n "${BUILD_ROOT}" && -d "${BUILD_ROOT}" && "${BUILD_ROOT}" == *"cl-compile-matrix."* ]]; then
    rm -rf -- "${BUILD_ROOT}"
  fi

  if [[ ${LOG_ROOT_IS_TEMP} == 1 && ${FAILURES} == 0 \
        && -n "${LOG_ROOT}" && -d "${LOG_ROOT}" && "${LOG_ROOT}" == *"cl-compile-matrix-logs."* ]]; then
    rm -rf -- "${LOG_ROOT}"
  elif [[ -d "${LOG_ROOT}" ]]; then
    info "Logs kept in: ${LOG_ROOT}"
  fi
}
trap cleanup EXIT
trap 'echo; warn "interrupted"; exit 130' INT TERM

# ##############################################################################
# Run the matrix
# ##############################################################################

# macOS ships bash 3.2, which has no associative arrays, so the result grid is a
# flat indexed array addressed by cell() = perm_index * NMODELS + model_index.
# Values are: ok | FAIL | cfg-FAIL | dry
NMODELS=${#MODELS[@]}
cell() { echo $(( $1 * NMODELS + $2 )); }

RESULTS=()
DURATIONS=()
FAILED_LIST=()

TOTAL=$(( ${#MODELS[@]} * ${#PERM_KEYS[@]} ))
COUNTER=0
MATRIX_START=${SECONDS}

info "Source dir : ${SRC_DIR}"
info "Build root : ${BUILD_ROOT}"
info "Log dir    : ${LOG_ROOT}"
info "Matrix     : ${#MODELS[@]} models x ${#PERM_KEYS[@]} permutations = ${TOTAL} builds, -j${JOBS}"
echo

for i in "${!PERM_KEYS[@]}"; do
  IFS=':' read -r p m h <<<"${PERM_KEYS[$i]}"
  mask="${PERM_MASKS[$i]}"
  [[ $p == 1 ]] && parafaft=ON || parafaft=OFF
  [[ $m == 1 ]] && mpi=ON       || mpi=OFF
  [[ $h == 1 ]] && hdf5=ON      || hdf5=OFF

  build_dir="${BUILD_ROOT}/cfg_${mask//-/x}"
  log_dir="${LOG_ROOT}/${mask//-/x}"
  mkdir -p "${build_dir}" "${log_dir}"

  cmake_args=(
    -S "${SRC_DIR}" -B "${build_dir}"
    "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
    "-DPARAFAFT=${parafaft}"
    "-DMPI=${mpi}"
    "-DHDF5=${hdf5}"
  )
  [[ ${hdf5} == ON && ${AUTOBUILD_HDF5} == ON ]] && cmake_args+=("-DAUTOBUILD_HDF5=ON")
  ((${#EXTRA_CMAKE_ARGS[@]})) && cmake_args+=("${EXTRA_CMAKE_ARGS[@]}")

  printf '%s[%s]%s PARAFAFT=%-3s MPI=%-3s HDF5=%-3s\n' \
    "${C_BOLD}" "${mask}" "${C_RESET}" "${parafaft}" "${mpi}" "${hdf5}"

  if [[ ${DRY_RUN} == 1 ]]; then
    printf '   %scmake %s -DMODEL=<each model>%s\n' "${C_DIM}" "${cmake_args[*]}" "${C_RESET}"
    for j in "${!MODELS[@]}"; do
      RESULTS[$(cell "$i" "$j")]="dry"
      COUNTER=$((COUNTER + 1))
    done
    echo
    continue
  fi

  # First configure of this permutation: this is where Kokkos, TempLat,
  # ParaFaFT and (optionally) HDF5 get fetched and built.
  cfg_start=${SECONDS}
  if ! cmake "${cmake_args[@]}" -DMODEL="${MODELS[0]}" \
        >"${log_dir}/configure.log" 2>&1; then
    warn "configure failed for [${mask}] -- see ${log_dir}/configure.log"
    tail -n 15 "${log_dir}/configure.log" | sed 's/^/     /'
    for j in "${!MODELS[@]}"; do
      RESULTS[$(cell "$i" "$j")]="cfg-FAIL"
      FAILED_LIST+=("${mask}  ${MODELS[$j]}  (configure)")
      FAILURES=$((FAILURES + 1))
      COUNTER=$((COUNTER + 1))
    done
    echo
    continue
  fi
  printf '   %sdependencies configured in %ds%s\n' \
    "${C_DIM}" "$((SECONDS - cfg_start))" "${C_RESET}"

  for j in "${!MODELS[@]}"; do
    model="${MODELS[$j]}"
    COUNTER=$((COUNTER + 1))
    printf '   %s(%*d/%d)%s %-24s ' \
      "${C_DIM}" "${#TOTAL}" "${COUNTER}" "${TOTAL}" "${C_RESET}" "${model}"

    log="${log_dir}/${model}.log"
    # Drop any executable left over from an earlier run so that a silent
    # no-op build cannot be mistaken for a success.
    rm -f "${build_dir}/${model}"

    build_start=${SECONDS}
    ok=1
    # -UMODELS_CMAKE: CMakeLists.txt looks up an optional per-model
    # models/<MODEL>.cmake via find_file, which caches a successful hit. Since
    # the build dir is shared by every model in this permutation, that cached
    # path would otherwise leak into the next model.
    cmake "${cmake_args[@]}" -UMODELS_CMAKE -DMODEL="${model}" >"${log}" 2>&1 || ok=0
    if [[ ${ok} == 1 ]]; then
      cmake --build "${build_dir}" --target cosmolattice -j "${JOBS}" >>"${log}" 2>&1 || ok=0
    fi
    # The executable is named after the model (MODEL_LABEL in CMakeLists.txt).
    [[ -x "${build_dir}/${model}" ]] || ok=0
    elapsed=$((SECONDS - build_start))
    DURATIONS[$(cell "$i" "$j")]=${elapsed}

    if [[ ${ok} == 1 ]]; then
      RESULTS[$(cell "$i" "$j")]="ok"
      printf '%sok%s   %ss\n' "${C_GREEN}" "${C_RESET}" "${elapsed}"
    else
      RESULTS[$(cell "$i" "$j")]="FAIL"
      FAILED_LIST+=("${mask}  ${model}")
      FAILURES=$((FAILURES + 1))
      printf '%sFAIL%s %ss  %s%s%s\n' "${C_RED}${C_BOLD}" "${C_RESET}" "${elapsed}" \
        "${C_DIM}" "${log}" "${C_RESET}"
    fi
  done
  echo

  # Reclaim the disk as soon as a permutation is done -- a full matrix with
  # HDF5 autobuilt is several GB if all eight are kept around at once.
  if [[ ${KEEP_BUILDS} == 0 ]]; then
    rm -rf -- "${build_dir}"
  fi
done

# ##############################################################################
# Report
# ##############################################################################

TOTAL_TIME=$((SECONDS - MATRIX_START))

echo "${C_BOLD}Compile matrix (rows: models, columns: PARAFAFT/MPI/HDF5)${C_RESET}"
printf '%-24s' ""
for mask in "${PERM_MASKS[@]}"; do printf ' %-8s' "${mask}"; done
echo
printf '%-24s' "$(printf '%.0s-' {1..24})"
for _ in "${PERM_MASKS[@]}"; do printf ' %-8s' "--------"; done
echo

for j in "${!MODELS[@]}"; do
  printf '%-24s' "${MODELS[$j]}"
  for i in "${!PERM_MASKS[@]}"; do
    r="${RESULTS[$(cell "$i" "$j")]:-?}"
    case "${r}" in
      ok)       printf ' %s%-8s%s' "${C_GREEN}" "ok" "${C_RESET}" ;;
      FAIL)     printf ' %s%-8s%s' "${C_RED}${C_BOLD}" "FAIL" "${C_RESET}" ;;
      cfg-FAIL) printf ' %s%-8s%s' "${C_RED}${C_BOLD}" "cfgFAIL" "${C_RESET}" ;;
      dry)      printf ' %s%-8s%s' "${C_DIM}" "-" "${C_RESET}" ;;
      *)        printf ' %-8s' "${r}" ;;
    esac
  done
  echo
done
echo

# Machine-readable results for CI.
{
  printf 'mask\tmodel\tresult\tseconds\n'
  for i in "${!PERM_MASKS[@]}"; do
    for j in "${!MODELS[@]}"; do
      printf '%s\t%s\t%s\t%s\n' "${PERM_MASKS[$i]}" "${MODELS[$j]}" \
        "${RESULTS[$(cell "$i" "$j")]:-?}" "${DURATIONS[$(cell "$i" "$j")]:-0}"
    done
  done
} >"${LOG_ROOT}/results.tsv"

if [[ ${DRY_RUN} == 1 ]]; then
  info "Dry run: ${TOTAL} builds would have been attempted."
  exit 0
fi

if [[ ${FAILURES} -gt 0 ]]; then
  printf '%s%d of %d builds failed:%s\n' "${C_RED}${C_BOLD}" "${FAILURES}" "${TOTAL}" "${C_RESET}"
  printf '  %s\n' "${FAILED_LIST[@]}"
  echo
  printf 'Total time: %dm%02ds\n' $((TOTAL_TIME / 60)) $((TOTAL_TIME % 60))
  exit 1
fi

printf '%sAll %d builds succeeded.%s  Total time: %dm%02ds\n' \
  "${C_GREEN}${C_BOLD}" "${TOTAL}" "${C_RESET}" $((TOTAL_TIME / 60)) $((TOTAL_TIME % 60))
exit 0
