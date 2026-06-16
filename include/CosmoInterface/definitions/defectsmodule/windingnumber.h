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
  /** @brief A class which computed the energy of the GWs, using the type 1 (neutral) TT projector.
   *
   * Unit test: ctest -R test-symmetrictracelessfieldmultiply
   **/

  // template <typename R, typename T, typename Model> class WindingNumberLengthScalarSinglet : public TempLat::BinaryOperator<R, T>
  // {
  // public:
  //   // Put public methods here. These should change very little over time.
  //   using TempLat::BinaryOperator<R, T>::mR;
  //   using TempLat::BinaryOperator<R, T>::mT;
  //   using S = Model::FloatType;
  //
  //   WindingNumberLengthScalarSinglet(const R &pR, const T &pT, const Model &model)
  //       : TempLat::BinaryOperator<R, T>(pR, pT)
  //   {
  //   }
  //
  //   static consteval size_t getNDim() { return Model::NDim; }
  //
  //   virtual std::string operatorString() const override { return "WindingNumerLength_scalarSinglet"; }
  //
  //   template <typename... IDX>
  //     requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
  //       requires IsVariadicIndex<IDX...>;
  //       DoEval::eval(r, idx...);
  //       DoEval::eval(t, idx...);
  //     }
  //   DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
  //   {
  //     auto f = complex(DoEval::eval(this->mR, idx...), DoEval::eval(this->mT, idx...));
  //     auto f1 = complex(DoEval::eval(shift<1_c>(this->mR), idx...), DoEval::eval(shift<X>(this->mT), idx...));
  //     auto f2 = complex(DoEval::eval(shift<2_c>(this->mR), idx...), DoEval::eval(shift<Y>(this->mT), idx...));
  //     auto f3 = complex(DoEval::eval(shift<3_c>(this->mR), idx...), DoEval::eval(shift<Y>(this->mT), idx...));
  //     auto f12 = complex(DoEval::eval(shift<1_c,2_c>(this->mR), idx...), DoEval::eval(shift<X,Y>(this->mT), idx...));
  //     auto f23 = complex(DoEval::eval(shift<2_c,3_c>(this->mR), idx...), DoEval::eval(shift<X,Y>(this->mT), idx...));
  //     auto f31 = complex(DoEval::eval(shift<3_c,1_c>(this->mR), idx...), DoEval::eval(shift<X,Y>(this->mT), idx...));
  //
  //     return   ( abs( arg(f*conj(f1)) + arg(f1*conj(f12)) + arg(f12*conj(f2)) + arg(f2*conj(f)) ) +
  //                abs( arg(f*conj(f2)) + arg(f2*conj(f23)) + arg(f23*conj(f3)) + arg(f3*conj(f)) ) +
  //                abs( arg(f*conj(f3)) + arg(f3*conj(f31)) + arg(f31*conj(f1)) + arg(f1*conj(f)) ) ) / S(2.) / Constants::pi<S>;
  //   }
  // };
  //
  // template <typename R, typename Model> class WindingNumberLengthComplexScalar : public TempLat::ComplexFieldUnaryOperator<R>
  // {
  // public:
  //   // Put public methods here. These should change very little over time.
  //   using TempLat::BinaryOperator<R>::mR;
  //   using S = Model::FloatType;
  //
  //   WindingNumberLengthScalarSinglet(const R &pR, const Model &model)
  //       : TempLat::BinaryOperator<R>(pR)
  //   {
  //   }
  //
  //   static consteval size_t getNDim() { return Model::NDim; }
  //
  //   virtual std::string operatorString() const override { return "WindingNumerLength_scalarSinglet"; }
  //
  //   template <typename... IDX>
  //     requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
  //       requires IsVariadicIndex<IDX...>;
  //       DoEval::eval(r, idx...);
  //     }
  //   DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
  //   {
  //     auto f = DoEval::eval(this->mR, idx...);
  //     auto f1 = complex(DoEval::eval(shift<1_c>(this->mR), idx...), DoEval::eval(shift<X>(this->mT), idx...));
  //     auto f2 = complex(DoEval::eval(shift<2_c>(this->mR), idx...), DoEval::eval(shift<Y>(this->mT), idx...));
  //     auto f3 = complex(DoEval::eval(shift<3_c>(this->mR), idx...), DoEval::eval(shift<Y>(this->mT), idx...));
  //     auto f12 = complex(DoEval::eval(shift<1_c,2_c>(this->mR), idx...), DoEval::eval(shift<X,Y>(this->mT), idx...));
  //     auto f23 = complex(DoEval::eval(shift<2_c,3_c>(this->mR), idx...), DoEval::eval(shift<X,Y>(this->mT), idx...));
  //     auto f31 = complex(DoEval::eval(shift<3_c,1_c>(this->mR), idx...), DoEval::eval(shift<X,Y>(this->mT), idx...));
  //
  //     return   ( abs( arg(f*conj(f1)) + arg(f1*conj(f12)) + arg(f12*conj(f2)) + arg(f2*conj(f)) ) +
  //                abs( arg(f*conj(f2)) + arg(f2*conj(f23)) + arg(f23*conj(f3)) + arg(f3*conj(f)) ) +
  //                abs( arg(f*conj(f3)) + arg(f3*conj(f31)) + arg(f31*conj(f1)) + arg(f1*conj(f)) ) ) / S(2.) / Constants::pi<S>;
  //   }
  // };

  template <typename Model> auto computeWindingNumberLengthScalarSinglet(const Model &model)
  {
    // return 2. / 3. * average(WindingNumberLengthScalarSinglet(model.fldS(0_c), model.fldS(1_c)));
    auto f = Complexify(model.fldS(0_c), model.fldS(1_c));
    return 2. / 3. * average( abs( arg(f*conj(shift<1_c>(f))) + arg(shift<1_c>(f)*conj(shift<1_c,2_c>(f))) +
                                   arg(shift<1_c,2_c>(f)*conj(shift<2_c>(f))) + arg(shift<2_c>(f)*conj(f)) ) +
                              abs( arg(f*conj(shift<2_c>(f))) + arg(shift<2_c>(f)*conj(shift<2_c,3_c>(f))) +
                                   arg(shift<2_c,3_c>(f)*conj(shift<3_c>(f))) + arg(shift<3_c>(f)*conj(f)) ) +
                              abs( arg(f*conj(shift<3_c>(f))) + arg(shift<3_c>(f)*conj(shift<3_c,1_c>(f))) +
                                   arg(shift<3_c,1_c>(f)*conj(shift<1_c>(f))) + arg(shift<1_c>(f)*conj(f)) ) ) ) / Model::FloatType(2.) / Constants::pi<Model::FloatType> * pow<3>(model.N) * model.dx;
  }

  template <typename Model> auto computeWindingNumberLengthComplexScalar(const Model &model)
  {
    const auto& f = model.fldCS(0_c);
    return 2. / 3. * average( abs( arg(f*conj(shift<1_c>(f))) + arg(shift<1_c>(f)*conj(shift<1_c,2_c>(f))) +
                                   arg(shift<1_c,2_c>(f)*conj(shift<2_c>(f))) + arg(shift<2_c>(f)*conj(f)) ) +
                              abs( arg(f*conj(shift<2_c>(f))) + arg(shift<2_c>(f)*conj(shift<2_c,3_c>(f))) +
                                   arg(shift<2_c,3_c>(f)*conj(shift<3_c>(f))) + arg(shift<3_c>(f)*conj(f)) ) +
                              abs( arg(f*conj(shift<3_c>(f))) + arg(shift<3_c>(f)*conj(shift<3_c,1_c>(f))) +
                                   arg(shift<3_c,1_c>(f)*conj(shift<1_c>(f))) + arg(shift<1_c>(f)*conj(f)) ) ) / Model::FloatType(2.) / Constants::pi<Model::FloatType> * pow<3>(model.N) * model.dx;
  }

  template <typename Model> auto computeWindingNumberLengthComplexScalarU1(const Model &model)
  {
    const auto& f = model.fldCS(0_c);
    const auto& a1 = model.fldU1(0_c)(1_c) * model.dx * model.gQ_CsU1(0_c,0_c);
    const auto& a2 = model.fldU1(0_c)(2_c) * model.dx * model.gQ_CsU1(0_c,0_c);
    const auto& a3 = model.fldU1(0_c)(3_c) * model.dx * model.gQ_CsU1(0_c,0_c);
    const auto& expa1 = complexPhase(model.fldU1(0_c)(1_c) * model.dx * model.gQ_CsU1(0_c,0_c));
    const auto& expa2 = complexPhase(model.fldU1(0_c)(2_c) * model.dx * model.gQ_CsU1(0_c,0_c));
    const auto& expa3 = complexPhase(model.fldU1(0_c)(3_c) * model.dx * model.gQ_CsU1(0_c,0_c));

    return 2. / 3. * average( abs( arg(f*expa1*conj(shift<1_c>(f))) - a1 + arg(shift<1_c>(f*expa2)*conj(shift<1_c,2_c>(f))) - shift<1_c>(a2) +
                                   arg(shift<1_c,2_c>(f)*conj(shift<2_c>(f*expa1))) + shift<2_c>(a1) + arg(shift<2_c>(f)*conj(f*expa2)) + a2 ) +
                              abs( arg(f*expa2*conj(shift<2_c>(f))) - a2 + arg(shift<2_c>(f*expa3)*conj(shift<2_c,3_c>(f))) - shift<2_c>(a3) +
                                   arg(shift<2_c,3_c>(f)*conj(shift<3_c>(f*expa2))) + shift<3_c>(a2) + arg(shift<3_c>(f)*conj(f*expa3)) + a3 ) +
                              abs( arg(f*expa3*conj(shift<3_c>(f))) - a3 + arg(shift<3_c>(f*expa1)*conj(shift<3_c,1_c>(f))) - shift<3_c>(a1) +
                                   arg(shift<3_c,1_c>(f)*conj(shift<1_c>(f*expa3))) + shift<1_c>(a3) + arg(shift<1_c>(f)*conj(f*expa1)) + a1 ) ) / Model::FloatType(2.) / Constants::pi<Model::FloatType> * pow<3>(model.N) * model.dx;
  }

} // namespace TempLat

#endif
