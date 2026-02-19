
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#ifdef HAVE_HDF5
#include "TempLat/lattice/IO/HDF5/filesaverhdf5.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include <filesystem>

namespace TempLat {

struct FileSaverHDF5Tester {
  static void Test(TDDAssertion &tdd);
};

void FileSaverHDF5Tester::Test(TDDAssertion &tdd)
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

  // Check that the file was created and is not empty.
  std::filesystem::path filePath("./FILE.h5");
  tdd.verify(std::filesystem::exists(filePath));
  tdd.verify(std::filesystem::file_size(filePath) > 0);

  // Run h5dump on the file to check that it contains the expected datasets.
  std::string h5dumpOutput;
  {
    std::array<char, 128> buffer;
    std::string command = "h5dump ./FILE.h5 2>&1";
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      h5dumpOutput += buffer.data();
    }
  }

  // Check that the output contains the expected dataset names.
  tdd.verify(h5dumpOutput.find("phi") != std::string::npos);
  tdd.verify(h5dumpOutput.find("aDot") != std::string::npos);
  tdd.verify(h5dumpOutput.find("42") != std::string::npos);
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FileSaverHDF5Tester> test;
}

#endif