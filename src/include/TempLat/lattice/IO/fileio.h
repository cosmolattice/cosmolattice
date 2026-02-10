#ifndef TEMPLAT_LATTICE_IO_FILEIO_H
#define TEMPLAT_LATTICE_IO_FILEIO_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/tuplemaker.h"

#include "TempLat/util/tdd/tdd.h"

#ifdef HDF5
#include "TempLat/lattice/IO/HDF5/filesaverhdf5.h"
#include "TempLat/lattice/IO/HDF5/fileloaderhdf5.h"
#else
#include "TempLat/lattice/IO/PureMPI/fileloaderpurempi.h"
#include "TempLat/lattice/IO/PureMPI/filesaverpurempi.h"
#endif

namespace TempLat
{

  /** @brief A class which make the choice between pure mpi or highfive saving happen.
   *
   *
   *
   * Unit test: ctest -R test-fileio
   **/
  template <size_t NDim = 3> class FileIO
  {
  public:
    // Put public methods here. These should change very little over time.
    FileIO() {}

    template <class R>
      requires HasStaticGetter<typename std::decay_t<R>>
    void save(R &&r)
    {
      using nakedR = std::decay_t<R>;
      for_in_range<number_to_skip_as_tuple<nakedR>::value, nakedR::size>(
          [&](auto i) { save(nakedR::Getter::get(r, i)); });
    }

    template <class R>
      requires(!HasStaticGetter<typename std::decay_t<R>>)
    void save(R &&r)
    {
      saver.save(r);
    }

    template <class R>
      requires HasStaticGetter<typename std::decay_t<R>>
    void load(R &&r)
    {
      using nakedR = std::decay_t<R>;
      for_in_range<number_to_skip_as_tuple<nakedR>::value, nakedR::size>(
          [&](auto i) { load(nakedR::Getter::get(r, i)); });
    }

    template <class R>
      requires(!HasStaticGetter<typename std::decay_t<R>>)
    void load(R &&r)
    {
      loader.load(r);
    }

#ifdef HDF5
    FileSaverHDF5 saver;
    FileLoaderHDF5 loader;
#else
    FileSaverPureMPI<NDim> saver;
    FileLoaderPureMPI<NDim> loader;
#endif

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
