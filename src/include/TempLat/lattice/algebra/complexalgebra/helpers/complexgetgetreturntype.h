#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXGETGETRETURNTYPE_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXGETGETRETURNTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** @brief A class which unpack the return type of a complex type.
   *
   * Unit test: ctest -R test-complexgetgetreturntype
   **/
  template <typename T> struct ComplexGetGetReturnType {
    using type = typename GetGetReturnType<
        std::decay_t<decltype(std::declval<T>().ComplexFieldGet(std::declval<Tag<0>>()))>>::type;

    static constexpr bool isComplex = IsComplexType<type>;
  };

} // namespace TempLat

#endif
