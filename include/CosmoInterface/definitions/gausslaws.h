#ifndef COSMOINTERFACE_HELPERS_GAUSSLAWS_H
#define COSMOINTERFACE_HELPERS_GAUSSLAWS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/util/templatvector.h"
#include "CosmoInterface/definitions/mattercurrents.h"
#include "TempLat/lattice/algebra/spatialderivatives/backdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "TempLat/lattice/algebra/axionalgebra/magneticfield4.h"
#include "CosmoInterface/definitions/axioncouplings.h"

namespace TempLat
{
  /** @brief A class that checks the preservation of the Gauss constraints, for both U1 and SU2 gauge sectors.
   *
   *
   **/
  class GaussLaws
  {
  public:
    // Put public methods here. These should change very little over time.
    GaussLaws() = default;

    template <class Model, int N>

    // Checks Gauss constraint for the U(1) gauge sector:
    static inline auto checkU1(Model &model, Tag<N> n)
    {
      auto RHSaxion = AxionCouplings::ScalarAxionGaussLaw(model, n);
      auto RHS = -model.dx * MatterCurrents::U1ChargeDensity(model, n);
      auto LHS = Total(i, 1, Model::NDim, model.piU1(n)(i) - shift(model.piU1(n)(i), -i)) / model.dx;
      auto LHS1 = (model.piU1(n)(1_c) - shift(model.piU1(n)(1_c), -1_c)) / model.dx;
      auto LHS2 = (model.piU1(n)(2_c) - shift(model.piU1(n)(2_c), -2_c)) / model.dx;
      auto LHS3 = (model.piU1(n)(3_c) - shift(model.piU1(n)(3_c), -3_c)) / model.dx;

      auto diff = LHS - (RHS + RHSaxion);
      auto sum = pow<2>(LHS1) + pow<2>(LHS2) + pow<2>(LHS3) + pow<2>(RHS + RHSaxion);

      auto avDiff = average(sqrt(pow<2>(diff)));
      auto avSum = average(sqrt(pow<2>(sum)));
      auto avRHS = average(sqrt(pow<2>(RHS + RHSaxion)));
      auto avLHS = average(sqrt(pow<2>(LHS)));

      return make_templatvector(static_cast<double>(avDiff / avSum), static_cast<double>(avLHS),
                                static_cast<double>(avRHS));
    }

    // Checks Gauss constraint for the SU(2) gauge sector:
    template <class Model, int N> static inline auto checkSU2(Model &model, Tag<N> n)
    {
      // Right hand side (source term). Note there is no factor of dx here, unlike checkU1() above:
      // U1ChargeDensity carries a 2/dx in its normalisation which that dx cancels, whereas
      // SU2ChargeDensity has no 1/dx. Since the LHS below is (1/dx)*sum(...), it must be compared
      // against the bare density -- which is also what the initialiser imposes (su2initializer.h sets
      // j0a = -dx*density, i.e. LHS = -density). With the spurious dx the reported violation was a
      // constant |LHS|/|RHS| = 1/dx and the ratio sat near 0.6 even for an exactly satisfied constraint.
      auto RHS = (-1.0) * MatterCurrents::SU2ChargeDensity(model, n);
      auto LHS = Total(i, 1, Model::NDim,
                       (1 / model.dx) * (model.piSU2(n)(i) -
                                         shift(dagger(model.fldSU2(n)(i)) * model.piSU2(n)(i) * model.fldSU2(n)(i),
                                               -i))); // left hand side

      auto diff = LHS - RHS;
      auto sum = LHS + RHS;

      auto avDiff = Total(i, 1, 3, average(sqrt(pow<2>(diff(i)))));
      auto avSum = Total(i, 1, 3, average(sqrt(pow<2>(sum(i)))));
      auto avRHS = Total(i, 1, 3, average(sqrt(pow<2>(RHS(i)))));
      auto avLHS = Total(i, 1, 3, average(sqrt(pow<2>(LHS(i)))));

      return make_templatvector(avDiff / avSum, avLHS,
                                avRHS); // Returns a vector, whose 0 component is the relative degree of conservation.
    }
  };
} // namespace TempLat

#endif
