#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/algebra.h"
#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldconjugate.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"

inline void TempLat::ComplexFieldShiftTester::Test(TempLat::TDDAssertion &tdd)
{
  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(true);

  /*  using T = TempLat::ComplexFieldAsFourier<TempLat::ScalarComplexFieldMultiply<
        TempLat::Operators::Division<
            int,
            TempLat::Operators::Addition<
                TempLat::Operators::Addition<
                    TempLat::Operators::PowerN<
                        2, TempLat::Operators::Subtraction<
                               TempLat::Tag<1>, TempLat::Operators::Cosine<TempLat::Operators::Multiplication<
                                                    double,
     TempLat::GetVectorComponentHelper<TempLat::WaveNumber<3>>>>>>, TempLat::Operators::PowerN< 2,
     TempLat::Operators::UnaryMinus<TempLat::Operators::Sine<TempLat::Operators::Multiplication< double,
     TempLat::GetVectorComponentHelper<TempLat::WaveNumber<3>>>>>>>, TempLat::Operators::Addition<
                    TempLat::Operators::Addition<
                        TempLat::Operators::PowerN<
                            2, TempLat::Operators::Subtraction<
                                   TempLat::Tag<1>,
                                   TempLat::Operators::Cosine<TempLat::Operators::Multiplication<
                                       double, TempLat::GetVectorComponentHelper<TempLat::WaveNumber<3>>>>>>,
                        TempLat::Operators::PowerN<
                            2,
     TempLat::Operators::UnaryMinus<TempLat::Operators::Sine<TempLat::Operators::Multiplication< double,
     TempLat::GetVectorComponentHelper<TempLat::WaveNumber<3>>>>>>>, TempLat::Operators::Addition<
                        TempLat::Operators::PowerN<
                            2, TempLat::Operators::Subtraction<
                                   TempLat::Tag<1>,
                                   TempLat::Operators::Cosine<TempLat::Operators::Multiplication<
                                       double, TempLat::GetVectorComponentHelper<TempLat::WaveNumber<3>>>>>>,
                        TempLat::Operators::PowerN<
                            2,
     TempLat::Operators::UnaryMinus<TempLat::Operators::Sine<TempLat::Operators::Multiplication< double,
     TempLat::GetVectorComponentHelper<TempLat::WaveNumber<3>>>>>>>>>>, TempLat::ComplexFieldConjugate<
            TempLat::ComplexFieldSubtraction<TempLat::ComplexFieldWrapper<TempLat::Tag<1>, TempLat::ZeroType>,
                                             TempLat::U1Exponential<TempLat::Operators::Multiplication<
                                                 double,
     TempLat::GetVectorComponentHelper<TempLat::WaveNumber<3>>>>>>>>;*/

  using T = TempLat::Operators::Division<
      int,
      TempLat::Operators::Addition<
          TempLat::Operators::Addition<
              TempLat::Operators::PowerN<
                  2, TempLat::Operators::Subtraction<
                         TempLat::Tag<1>, TempLat::Operators::Cosine<TempLat::Operators::Multiplication<
                                              double, TempLat::GetVectorComponentHelper<3, TempLat::WaveNumber<3>>>>>>,
              TempLat::Operators::PowerN<
                  2, TempLat::Operators::UnaryMinus<TempLat::Operators::Sine<TempLat::Operators::Multiplication<
                         double, TempLat::GetVectorComponentHelper<1, TempLat::WaveNumber<3>>>>>>>,
          TempLat::Operators::Addition<
              TempLat::Operators::Addition<
                  TempLat::Operators::PowerN<
                      2,
                      TempLat::Operators::Subtraction<
                          TempLat::Tag<1>, TempLat::Operators::Cosine<TempLat::Operators::Multiplication<
                                               double, TempLat::GetVectorComponentHelper<2, TempLat::WaveNumber<3>>>>>>,
                  TempLat::Operators::PowerN<
                      2, TempLat::Operators::UnaryMinus<TempLat::Operators::Sine<TempLat::Operators::Multiplication<
                             double, TempLat::GetVectorComponentHelper<3, TempLat::WaveNumber<3>>>>>>>,
              TempLat::Operators::Addition<
                  TempLat::Operators::PowerN<
                      2,
                      TempLat::Operators::Subtraction<
                          TempLat::Tag<1>, TempLat::Operators::Cosine<TempLat::Operators::Multiplication<
                                               double, TempLat::GetVectorComponentHelper<1, TempLat::WaveNumber<3>>>>>>,
                  TempLat::Operators::PowerN<
                      2, TempLat::Operators::UnaryMinus<TempLat::Operators::Sine<TempLat::Operators::Multiplication<
                             double, TempLat::GetVectorComponentHelper<2, TempLat::WaveNumber<3>>>>>>>>>>;

  using T2 = TempLat::ComplexFieldConjugate<
      TempLat::ComplexFieldSubtraction<TempLat::ComplexFieldWrapper<TempLat::Tag<1>, TempLat::ZeroType>,
                                       TempLat::U1Exponential<TempLat::Operators::Multiplication<
                                           double, TempLat::GetVectorComponentHelper<1, TempLat::WaveNumber<3>>>>>>;
  using T1 = TempLat::GetVectorComponentHelper<1, TempLat::WaveNumber<3>>;
  using RES1 = TempLat::ComplexFieldAsFourier<TempLat::ScalarComplexFieldMultiply<T1, T2>>;

  tdd.verify(HasEvalMethod<RES1>);
}

#endif
