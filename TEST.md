## Testing

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