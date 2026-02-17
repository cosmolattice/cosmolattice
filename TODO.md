# Open issues

- There are problems with 1D fourier transforms in `test-normgradientsquare`
- There are problems with 2D fourier transforms in `test-latticelaplacian`
- Correctly implement PostGet/PreGet for other things than just the bare field.
- Offline compilation must work flawlessly
- Hunt down manual copy operations where device::memory::copyDeviceToDevice should be used instead. (Ghostupdater, e.g.)
- Make sure HIP works.
- Get rid of the JumpsHolder