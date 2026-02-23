#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5ATTRIBUTE_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5ATTRIBUTE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HAVE_HDF5

#include <string>
#include <utility>
#include <hdf5.h>

namespace TempLat
{
  /** @brief A class which overloads hdf5 attrbute.
   *
   *
   * Unit test: ctest -R test-hdf5attribute
   **/
  class HDF5Attribute
  {
  public:
    // Put public methods here. These should change very little over time.
    HDF5Attribute() = default;

    HDF5Attribute(const hid_t &id) : attr_id(id) {}

    HDF5Attribute(hid_t &&id) : attr_id(std::move(id)) {}

    ~HDF5Attribute() { this->close(); }

    operator hid_t &() { return attr_id; }

    operator const hid_t &() const { return attr_id; }

    void close() { H5Aclose(attr_id); }

    template <typename T> void create(std::string name, T value) // single value attribute
    {
    }

  private:
    // Put all member variables and private methods here. These may change arbitrarily.

    hid_t attr_id;
  };
} // namespace TempLat

#endif
#endif
