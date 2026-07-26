## Testing

### The compile matrix

`tests/compile_matrix.sh` compiles every model in `models/` against every meaningful permutation of
the `PARAFAFT`, `MPI` and `HDF5` options. It is a pure build check — nothing is executed — meant to
catch model code that only compiles under one particular set of options.

```bash
tests/compile_matrix.sh              # the full 9 models x 6 permutations matrix
tests/compile_matrix.sh --list       # show what would be built, then exit
tests/compile_matrix.sh -n           # dry run: print every cmake invocation
```

The script exits non-zero if any build fails and prints a pass/fail matrix at the end:

```
Compile matrix (rows: models, columns: PARAFAFT/MPI/HDF5)
                         ---      --H      -M-      -MH      PM-      PMH
------------------------ -------- -------- -------- -------- -------- --------
lphi4                    ok       ok       ok       ok       ok       ok
tanh2                    ok       ok       ok       ok       FAIL     ok
```

Column headers are three-character masks in `PARAFAFT`/`MPI`/`HDF5` order: the flag's letter means
`ON`, `-` means `OFF`.

There are 6 columns rather than 8 because `PARAFAFT=ON` with `MPI=OFF` is not a real configuration:
TempLat warns and disables ParaFaFT, so those two builds would just duplicate their `PARAFAFT=OFF`
counterparts. Asking for them explicitly (`-p P--`) is rejected with an error rather than silently
producing a misleading `ok`.

**How it works.** Builds are grouped permutation-major. Each permutation gets one throwaway build
directory under a temporary root, configured once so that Kokkos, TempLat, ParaFaFT and HDF5 are
built a single time; every model is then compiled in that directory by re-running `cmake` with a new
`-DMODEL`. Each build directory is deleted as soon as its permutation finishes, and the temporary
root is removed on exit (including on `Ctrl-C`). Logs go to a *separate* directory that is kept
whenever anything fails, along with a machine-readable `results.tsv`.

The `HDF5=ON` permutations pass `-DAUTOBUILD_HDF5=ON`, so HDF5 2.0.0 is built from source when it is
not installed system-wide. This dominates the runtime of those three permutations. Use
`--no-autobuild-hdf5` to require a system HDF5 instead.

**Useful options** (see `--help` for the full list):

```bash
tests/compile_matrix.sh -m lphi4,tanh2      # only these models
tests/compile_matrix.sh -p ---,PMH          # only these permutations
tests/compile_matrix.sh -j 8                # parallel compile jobs per build
tests/compile_matrix.sh --log-dir ./cmlogs  # keep logs somewhere permanent
tests/compile_matrix.sh -k                  # keep the temporary build root
tests/compile_matrix.sh -DNOTHREADING=ON    # forward extra flags to every cmake call
```

### Enable testing

To enable testing, add the flag `-DTESTING=ON` when configuring the project with CMake. 

You can then run the full test suite using CTest:
```bash
ctest --output-on-failure
```
If you want to run tests in parallel, you can specify the number of parallel jobs with the `-j` option:
```bash
ctest --output-on-failure -j <num_jobs>
```

### Running specific tests

`ctest` can run tests based on labels or test names. For example, to run all tests labeled with `su2algebra`, you can use:
```bash
ctest -L su2algebra --output-on-failure
```
All tests are tagged automatically with the names of the folders they are located in, which is why the `su2algebra` label works in the above example. You can also run a specific test by its name:
```bash
ctest -R test-name --output-on-failure
```
The `-R` flag actually matches all tests whose name contain the given string, so you can do 
```bash
ctest -R su2 --output-on-failure
```
to run all tests whose name contains `su2`.

### Test output

By default, `ctest` only shows the output of failed tests. If you want to see the output of all tests, you can use the `--output-on-failure` flag as shown in the examples above. This will print the output of each test, regardless of whether it passed or failed.
To see the full output of all tests, you can use the `--verbose` flag:
```bash
ctest --verbose
```

### Rerunning failed tests

If you want to rerun only the tests that failed in the previous run, you can use the `--rerun-failed` flag:
```bash
ctest --rerun-failed --output-on-failure
```
This will rerun only the tests that failed in the last test run and show their output.