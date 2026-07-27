#ifndef COSMOINTERFACE_DEFINITIONS_WINDINGNUMBER_H
#define COSMOINTERFACE_DEFINITIONS_WINDINGNUMBER_H

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
  class DefectsObservables
  {
  public:
    template <typename Model> static auto computeWindingNumberLengthScalarSinglet(const Model &model)
    {
      // return 2. / 3. * average(WindingNumberLengthScalarSinglet(model.fldS(0_c), model.fldS(1_c)));
      auto f = Complexify(model.fldS(0_c), model.fldS(1_c));
      return 2. / 3. *
             average(abs(arg2(f * conj(shift<1_c>(f))) + arg2(shift<1_c>(f) * conj(shift<2_c>(shift<1_c>(f)))) +
                         arg2(shift<2_c>(shift<1_c>(f)) * conj(shift<2_c>(f))) + arg2(shift<2_c>(f) * conj(f))) +
                     abs(arg2(f * conj(shift<2_c>(f))) + arg2(shift<2_c>(f) * conj(shift<2_c>(shift<3_c>(f)))) +
                         arg2(shift<2_c>(shift<3_c>(f)) * conj(shift<3_c>(f))) + arg2(shift<3_c>(f) * conj(f))) +
                     abs(arg2(f * conj(shift<3_c>(f))) + arg2(shift<3_c>(f) * conj(shift<3_c>(shift<1_c>(f)))) +
                         arg2(shift<3_c>(shift<1_c>(f)) * conj(shift<1_c>(f))) + arg2(shift<1_c>(f) * conj(f)))) /
             static_cast<typename Model::FloatType>(2.) / Constants::pi<typename Model::FloatType> *
             pow<3>(GetNGrid::get(model)) * model.dx;
    }

    template <typename Model> static auto computeWindingNumberLengthComplexScalarU1(const Model &model)
    {
      const auto &f = model.fldCS(0_c);
      const auto &a1 = model.fldU1(0_c)(1_c) * model.dx * model.gQ_CsU1(0_c, 0_c);
      const auto &a2 = model.fldU1(0_c)(2_c) * model.dx * model.gQ_CsU1(0_c, 0_c);
      const auto &a3 = model.fldU1(0_c)(3_c) * model.dx * model.gQ_CsU1(0_c, 0_c);
      const auto &expa1 = complexPhase(model.fldU1(0_c)(1_c) * model.dx * model.gQ_CsU1(0_c, 0_c));
      const auto &expa2 = complexPhase(model.fldU1(0_c)(2_c) * model.dx * model.gQ_CsU1(0_c, 0_c));
      const auto &expa3 = complexPhase(model.fldU1(0_c)(3_c) * model.dx * model.gQ_CsU1(0_c, 0_c));

      return 2. / 3. *
             average(abs(arg2(f * expa1 * conj(shift<1_c>(f))) - a1 +
                         arg2(shift<1_c>(f * expa2) * conj(shift<2_c>(shift<1_c>(f)))) - shift<1_c>(a2) +
                         arg2(shift<2_c>(shift<1_c>(f)) * conj(shift<2_c>(f * expa1))) + shift<2_c>(a1) +
                         arg2(shift<2_c>(f) * conj(f * expa2)) + a2) +
                     abs(arg2(f * expa2 * conj(shift<2_c>(f))) - a2 +
                         arg2(shift<2_c>(f * expa3) * conj(shift<2_c>(shift<3_c>(f)))) - shift<2_c>(a3) +
                         arg2(shift<2_c>(shift<3_c>(f)) * conj(shift<3_c>(f * expa2))) + shift<3_c>(a2) +
                         arg2(shift<3_c>(f) * conj(f * expa3)) + a3) +
                     abs(arg2(f * expa3 * conj(shift<3_c>(f))) - a3 +
                         arg2(shift<3_c>(f * expa1) * conj(shift<3_c>(shift<1_c>(f)))) - shift<3_c>(a1) +
                         arg2(shift<3_c>(shift<1_c>(f)) * conj(shift<1_c>(f * expa3))) + shift<1_c>(a3) +
                         arg2(shift<1_c>(f) * conj(f * expa1)) + a1)) /
             static_cast<typename Model::FloatType>(2.) / Constants::pi<typename Model::FloatType> *
             pow<3>(GetNGrid::get(model)) * model.dx;
    }

    template <typename Model> static auto computeAreaParameter(const Model &model)
    {
      const auto &f = model.fldS(0_c);
      return model.dx * model.dx *
             average(Total(i, 1_c, 3_c, heaviside(-f * shift<i>(f))) * abs(Total(j, 1_c, 3_c, shift<j>(f) - f)) /
                     Total(j, 1_c, 3_c, abs(shift<j>(f) - f))) *
             pow<3>(GetNGrid::get(model));
    }
  };

} // namespace TempLat

#endif
