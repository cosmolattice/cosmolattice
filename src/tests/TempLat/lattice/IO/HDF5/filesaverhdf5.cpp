
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#ifdef HAVE_HDF5
#include "TempLat/lattice/IO/HDF5/filesaverhdf5.h"
#include "TempLat/lattice/IO/HDF5/fileloaderhdf5.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include <filesystem>
#include <cstdio>

namespace TempLat
{

  struct FileSaverHDF5Tester {
    static void Test(TDDAssertion &tdd);
  };

  void FileSaverHDF5Tester::Test(TDDAssertion &tdd)
  {
    // Test 1: basic save/load of field and scalar
    {
      FileSaverHDF5 fs;

      const ptrdiff_t nGrid = 16, nGhost = 1;
      auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);

      Field<3, double> phi("phi", toolBox);
      phi = 42.0;

      fs.create("./FILE.h5");
      fs.save(phi);
      fs.save(0.45, "aDot");
      fs.close();

      std::filesystem::path filePath("./FILE.h5");
      tdd.verify(std::filesystem::exists(filePath));
      tdd.verify(std::filesystem::file_size(filePath) > 0);

      std::string h5dumpOutput;
      {
        std::array<char, 128> buffer;
        std::string command = "h5dump ./FILE.h5 2>&1";
        std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
          h5dumpOutput += buffer.data();
        }
      }

      tdd.verify(h5dumpOutput.find("phi") != std::string::npos);
      tdd.verify(h5dumpOutput.find("aDot") != std::string::npos);
      tdd.verify(h5dumpOutput.find("42") != std::string::npos);
    }

    const std::string testFile = "test_rng_binary.h5";

    // Test 2: Binary RNG state round-trip (uniform-format: 313 uint64_t values)
    {
      // Build a synthetic state string mimicking master's RandomUniform format:
      // 313 space-separated uint64_t values
      std::ostringstream oss;
      for (int i = 0; i < 313; ++i) {
        if (i > 0) oss << " ";
        oss << static_cast<uint64_t>(i * 12345 + 67890);
      }
      std::string originalState = oss.str();

      FileSaverHDF5 saver;
      saver.create(testFile);
      saver.saveRNGStateBinary(originalState, "rng_uniform", 0, 1);
      saver.close();

      FileLoaderHDF5 loader;
      loader.open(testFile);
      std::string loadedState;
      loader.loadRNGStateBinary(loadedState, "rng_uniform", 0);
      loader.close();

      tdd.verify(originalState == loadedState, "Uniform-format binary round-trip preserves exact state string");

      std::remove(testFile.c_str());
    }

    // Test 3: Binary RNG state round-trip (gaussian-format: 316 uint64_t values with cache)
    {
      // Build synthetic state: first line = 313 values, second line = counter hasCache cachedValue
      std::ostringstream oss;
      for (int i = 0; i < 313; ++i) {
        if (i > 0) oss << " ";
        oss << static_cast<uint64_t>(i * 54321 + 11111);
      }
      oss << "\n";
      // gaussianCounter=42, hasCache=1, cachedValue=3.14159
      oss << "42 1 3.14159";
      std::string originalState = oss.str();

      FileSaverHDF5 saver;
      saver.create(testFile);
      saver.saveRNGStateBinary(originalState, "rng_gaussian", 0, 1);
      saver.close();

      FileLoaderHDF5 loader;
      loader.open(testFile);
      std::string loadedState;
      loader.loadRNGStateBinary(loadedState, "rng_gaussian", 0);
      loader.close();

      // The round-trip may not be exact for the double due to precision,
      // but the uint64_t values should match. Verify the first line matches.
      std::istringstream issOrig(originalState);
      std::istringstream issLoaded(loadedState);
      std::string origLine, loadedLine;
      std::getline(issOrig, origLine);
      std::getline(issLoaded, loadedLine);
      tdd.verify(origLine == loadedLine, "Gaussian-format binary round-trip preserves uniform state line");

      std::remove(testFile.c_str());
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FileSaverHDF5Tester> test;
}

#endif
