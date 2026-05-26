#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_KERNELSTYPES_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_KERNELSTYPES_H
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio  Year: 2024

#include "CosmoInterface/definitions/gausslaws.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"

namespace TempLat::KernelsTypes
{
  /** @brief A mechanism to define different kernels for the same solver.
   *
   **/
  template <typename Model> class EoM
  {
    using T = typename Model::FloatType;

  public:
    T tMinust0 = T{};

    void cache(Model &model, T tIn) { tMinust0 = tIn; }

  private:
    void cache(Model &model) {}
  };

} // namespace TempLat::KernelsTypes

#endif
