#ifndef TEMPLAT_LATTICE_FIELD_HELPERS_HASASTUPLECAT_H
#define TEMPLAT_LATTICE_FIELD_HELPERS_HASASTUPLECAT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <type_traits>
#include <utility>

namespace TempLat
{
  /** @brief A class which detects whether or not an object has the hasTuplCat method.
   *
   *
   * Unit test: ctest -R test-hasastuplecat
   **/
  template <class, class = std::void_t<>> struct HasAsTupleCat : std::false_type {
  };

  // specialization recognizes types that do have a nested ::type member:
  template <class T> struct HasAsTupleCat<T, std::void_t<decltype(std::declval<T>().asTupleCat())>> : std::true_type {
  };
} // namespace TempLat

#endif
