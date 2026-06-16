#ifndef COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_AREAPARAMETER_H
#define COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_AREAPARAMETER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
 *   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
 *   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026

#include "TempLat/util/constants.h"

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"

namespace TempLat
{
  /** @brief A class which computed the energy of the GWs, using the type 1 (neutral) TT projector.
   *
   * Unit test: ctest -R test-symmetrictracelessfieldmultiply
   **/



  template <typename Model> auto computeWindingNumberLengthScalarSinglet(const Model &model)
  {
    const auto& f = model.fldS(0_c);
    return model.dx * model.dx * average( Total(i, 1_c, 3_c, Heaviside(-f*shift<i>(f))) *
                  abs( Total(j, 1_c, 3_c, shift<j>(f) - f)) / Total(j, 1_c, 3_c, abs(shift<j>(f) - f)) ) * pow<3>(model.N);
  }

} // namespace TempLat

#endif
