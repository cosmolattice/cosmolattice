
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/complexalgebra/complexfieldshift.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/algebra.h"
#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldconjugate.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"

namespace TempLat
{

  struct ComplexFieldShiftTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexFieldShiftTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);

    /*  using T = ComplexFieldAsFourier<ScalarComplexFieldMultiply<
          Operators::Division<
              int,
              Operators::Addition<
                  Operators::Addition<
                      Operators::PowerN<
                          2, Operators::Subtraction<
                                 Tag<1>, Operators::Cosine<Operators::Multiplication<
                                                      double,
       GetVectorComponentHelper<WaveNumber<3>>>>>>, Operators::PowerN< 2,
       Operators::UnaryMinus<Operators::Sine<Operators::Multiplication< double,
       GetVectorComponentHelper<WaveNumber<3>>>>>>>, Operators::Addition<
                      Operators::Addition<
                          Operators::PowerN<
                              2, Operators::Subtraction<
                                     Tag<1>,
                                     Operators::Cosine<Operators::Multiplication<
                                         double, GetVectorComponentHelper<WaveNumber<3>>>>>>,
                          Operators::PowerN<
                              2,
       Operators::UnaryMinus<Operators::Sine<Operators::Multiplication< double,
       GetVectorComponentHelper<WaveNumber<3>>>>>>>, Operators::Addition<
                          Operators::PowerN<
                              2, Operators::Subtraction<
                                     Tag<1>,
                                     Operators::Cosine<Operators::Multiplication<
                                         double, GetVectorComponentHelper<WaveNumber<3>>>>>>,
                          Operators::PowerN<
                              2,
       Operators::UnaryMinus<Operators::Sine<Operators::Multiplication< double,
       GetVectorComponentHelper<WaveNumber<3>>>>>>>>>>, ComplexFieldConjugate<
              ComplexFieldSubtraction<ComplexFieldWrapper<Tag<1>, ZeroType>,
                                               U1Exponential<Operators::Multiplication<
                                                   double,
       GetVectorComponentHelper<WaveNumber<3>>>>>>>>;*/

    using T = Operators::Division<
        int, Operators::Addition<
                 Operators::Addition<
                     Operators::PowerN<
                         2, Operators::Subtraction<Tag<1>, Operators::Cosine<Operators::Multiplication<
                                                               double, GetVectorComponentHelper<3, WaveNumber<3>>>>>>,
                     Operators::PowerN<2, Operators::UnaryMinus<Operators::Sine<Operators::Multiplication<
                                              double, GetVectorComponentHelper<1, WaveNumber<3>>>>>>>,
                 Operators::Addition<
                     Operators::Addition<
                         Operators::PowerN<2, Operators::Subtraction<
                                                  Tag<1>, Operators::Cosine<Operators::Multiplication<
                                                              double, GetVectorComponentHelper<2, WaveNumber<3>>>>>>,
                         Operators::PowerN<2, Operators::UnaryMinus<Operators::Sine<Operators::Multiplication<
                                                  double, GetVectorComponentHelper<3, WaveNumber<3>>>>>>>,
                     Operators::Addition<
                         Operators::PowerN<2, Operators::Subtraction<
                                                  Tag<1>, Operators::Cosine<Operators::Multiplication<
                                                              double, GetVectorComponentHelper<1, WaveNumber<3>>>>>>,
                         Operators::PowerN<2, Operators::UnaryMinus<Operators::Sine<Operators::Multiplication<
                                                  double, GetVectorComponentHelper<2, WaveNumber<3>>>>>>>>>>;

    using T2 = ComplexFieldConjugate<ComplexFieldSubtraction<
        ComplexFieldWrapper<Tag<1>, ZeroType>,
        U1Exponential<Operators::Multiplication<double, GetVectorComponentHelper<1, WaveNumber<3>>>>>>;
    using T1 = GetVectorComponentHelper<1, WaveNumber<3>>;
    using RES1 = ComplexFieldAsFourier<ScalarComplexFieldMultiply<T1, T2>>;

    tdd.verify(HasEvalMethod<RES1>);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexFieldShiftTester> test;
}
