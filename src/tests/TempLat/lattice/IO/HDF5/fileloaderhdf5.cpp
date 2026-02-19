
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HAVE_HDF5

#include "TempLat/lattice/IO/HDF5/fileloaderhdf5.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct FileLoaderHDF5Tester {
    static void Test(TDDAssertion &tdd);
  };

  void FileLoaderHDF5Tester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FileLoaderHDF5Tester> test;
}

#endif