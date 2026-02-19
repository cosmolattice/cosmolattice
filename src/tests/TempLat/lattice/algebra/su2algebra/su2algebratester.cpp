
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2algebratester.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/complexalgebra/real.h"
#include "TempLat/lattice/algebra/complexalgebra/imag.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/algebra/su2algebra/su2doublet.h"
#include "TempLat/lattice/algebra/su2algebra/su2generators.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/gaugealgebra/u1exponential.h"
#include "TempLat/parallel/device_memory.h"

namespace TempLat
{

  struct SU2AlgebraTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2AlgebraTester::Test(TDDAssertion &tdd)
  {
    using namespace Constants;

    tdd.verify(HasSU2DoubletGet<decltype(sigma3)> == false);
    tdd.verify(HasSU2Get<decltype(sigma3)> == true);
    tdd.verify(HasSU2DoubletGet<decltype(sigma1)> == false);
    tdd.verify(HasSU2Get<decltype(sigma1)> == true);

    auto test1 = i_sigma(1_c) * i_sigma(3_c) * i_sigma(1_c) * i_sigma(3_c);

    tdd.verify(test1.SU2Get(0_c).eval(0) == -1);
    tdd.verify(test1.SU2Get(1_c).eval(0) == 0);
    tdd.verify(test1.SU2Get(2_c).eval(0) == 0);
    tdd.verify(test1.SU2Get(3_c).eval(0) == 0);

    auto test2 = i_sigma(1_c) * i_sigma(3_c);

    tdd.verify(test2.SU2Get(0_c).eval(0) == 0);
    tdd.verify(test2.SU2Get(1_c).eval(0) == 0);
    tdd.verify(test2.SU2Get(2_c).eval(0) == 1);
    tdd.verify(test2.SU2Get(3_c).eval(0) == 0);

    tdd.verify(dagger(i_sigma(3_c)).SU2Get(3_c).eval(0) == -1);

    struct MySU2Doublet {
      MySU2Doublet(double i, double j, double k, double l) : arr{{i, j, k, l}} {}

      DEVICE_FORCEINLINE_FUNCTION
      double SU2DoubletGet(Tag<0> t) const { return arr[0]; }
      DEVICE_FORCEINLINE_FUNCTION
      double SU2DoubletGet(Tag<1> t) const { return arr[1]; }
      DEVICE_FORCEINLINE_FUNCTION
      double SU2DoubletGet(Tag<2> t) const { return arr[2]; }
      DEVICE_FORCEINLINE_FUNCTION
      double SU2DoubletGet(Tag<3> t) const { return arr[3]; }

      std::string toString() const
      {
        return "MySU2Doublet(" + std::to_string(arr[0]) + "," + std::to_string(arr[1]) + "," + std::to_string(arr[2]) +
               "," + std::to_string(arr[3]) + ")";
      }

      device::array<double, 4> arr;
    };

    auto test3 = scalar_prod(MySU2Doublet(2, 0, 1, 1), dagger(i_sigma(1_c)) * MySU2Doublet(1, 1, 1, -1));
    tdd.verify(Real(test3).eval(0) == -2);
    tdd.verify(Imag(test3).eval(0) == -4);
    tdd.verify(HasComplexFieldGet<decltype(test3)> == true);

    auto test4 = test3 * MySU2Doublet(1, 1, 1, 1);
    tdd.verify(test4.SU2DoubletGet(0_c).eval(0) == 2);
    tdd.verify(test4.SU2DoubletGet(1_c).eval(0) == -6);
    tdd.verify(test4.SU2DoubletGet(2_c).eval(0) == 2);
    tdd.verify(test4.SU2DoubletGet(3_c).eval(0) == -6);

    auto toolBox = MemoryToolBox<3>::makeShared(32, 1);

    // Multiplication, test again.

    SU2Field<3, double> A("A", toolBox, LatticeParameters<double>());
    SU2Field<3, double> B("B", toolBox, LatticeParameters<double>());
    SU2DoubletBase<3, double> C("C", toolBox, LatticeParameters<double>());

    A = SU2Wrap(1., 0.2, 0.13, 0.14);
    B = SU2Wrap(1., 0.2, 0.13, 0.14);
    C = SU2DoubletWrap(1., 0.2, 0.13, 0.14);

    // zero is 1, because of the ghosts!
    const device::IdxArray<3> zero{{1, 1, 1}};

    auto D = Complexify(1.0, 2.0) * C;

    // Multiplication by a complex number
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(0_c), zero), 0.6));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(0_c), zero), 0.6));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(0_c), zero), 0.6));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(1_c), zero), 2.2));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(1_c), zero), 2.2));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(2_c), zero), -0.15));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(2_c), zero), -0.15));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(3_c), zero), 0.4));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint(D.SU2DoubletGet(3_c), zero), 0.4));

    auto A2 = A * A;
    auto A3 = A * A * A;

    say << A2.SU2Get(1_c);

    say << (A * B).SU2Get(0_c);
    say << (A2 * A2 * A2).SU2Get(0_c);
    say << (A3 * A3).SU2Get(0_c);
    say << ((A * A) * (A * A) * (A * A)).SU2Get(0_c);

    say << "Notice: \n " << (A * A * B * B).SU2Get(0_c) << " \n  vs \n " << ((A * A) * (B * B)).SU2Get(0_c) << "\n";

    auto B2 = A2 * A2 * A2;
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint((B2).SU2Get(2_c), zero),
                           device::memory::getAtOnePoint((A3 * A3).SU2Get(2_c), zero)));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint((B2).SU2Get(2_c), zero),
                           device::memory::getAtOnePoint(((A * A * A * A) * (A * A)).SU2Get(2_c), zero)));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint((B2).SU2Get(2_c), zero),
                           device::memory::getAtOnePoint(((A * A) * (A * A) * (A * A)).SU2Get(2_c), zero)));

    // Funny stuff: uncomment to get a segfault! Morale: use too much memory on the stack and it crashes
    // say << (A * A * A * A * A * A).SU2Get(0_c);
    // say << (A * A * A * A * A * A * A * A).SU2Get(0_c);
    // say << (A*A*A*A*A*A*A*A).SU2Get(0_c).get(tmp);
    // say << ((A * A * A * A) * (A * A * A * A)).SU2Get(0_c);
    // say << ((A*A*A*A)*(A*A*A*A)).SU2Get(0_c).get(tmp);

    // commutator
    B = SU2Wrap(sqrt(1 - pow<2>(0.56) - pow<2>(0.1) - pow<2>(0.01)), 0.56, 0.1, 0.01);

    auto comm = A * B - B * A;
    say << (comm).SU2Get(0_c);
    say << (comm).SU2Get(1_c);
    say << (comm).SU2Get(2_c);
    say << (comm).SU2Get(3_c);

    auto commutator2 = commutator(A, B);
    say << (commutator2).SU2Get(0_c);
    say << (commutator2).SU2Get(1_c);
    say << (commutator2).SU2Get(2_c);
    say << (commutator2).SU2Get(3_c);

    tdd.verify(AlmostEqual(device::memory::getAtOnePoint((comm).SU2Get(0_c), zero),
                           device::memory::getAtOnePoint((commutator2).SU2Get(0_c), zero)));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint((comm).SU2Get(1_c), zero),
                           device::memory::getAtOnePoint((commutator2).SU2Get(1_c), zero)));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint((comm).SU2Get(2_c), zero),
                           device::memory::getAtOnePoint((commutator2).SU2Get(2_c), zero)));
    tdd.verify(AlmostEqual(device::memory::getAtOnePoint((comm).SU2Get(3_c), zero),
                           device::memory::getAtOnePoint((commutator2).SU2Get(3_c), zero)));

    // For my sanity: checking that some complicated expressions just work.

    static_assert(
        HasSU2DoubletGet<
            ComplexFieldSU2DoubletMultiplication<U1Exponential<Operators::Multiplication<double, Field<3, double>>>,
                                                 SU2DoubletShifterByOne<SU2DoubletBase<3, double>, 1>>>);

    static_assert(
        HasSU2DoubletGet<SU2SU2DoubletMultiplication<
            SU2FieldBase<3, double>,
            ComplexFieldSU2DoubletMultiplication<U1Exponential<Operators::Multiplication<double, Field<3, double>>>,
                                                 SU2DoubletShifterByOne<SU2DoubletBase<3, double>, 1>>>>);

    static_assert(HasSU2DoubletGet<SU2DoubletSubtract<SU2DoubletBase<3, double>, SU2DoubletBase<3, double>>>);

    static_assert(HasSU2DoubletGet<
                  SU2DoubletSubtract<SU2DoubletBase<3, double>,
                                     SU2SU2DoubletMultiplication<SU2FieldBase<3, double>, SU2DoubletBase<3, double>>>>);

    static_assert(
        HasSU2DoubletGet<SU2DoubletSubtract<
            SU2DoubletBase<3, double>,
            ComplexFieldSU2DoubletMultiplication<U1Exponential<Operators::Multiplication<double, Field<3, double>>>,
                                                 SU2DoubletBase<3, double>>>>);

    static_assert(HasSU2DoubletGet<SU2DoubletShifterByOne<SU2DoubletBase<3, double>, 1>>);

    static_assert(HasSU2DoubletGet<
                  SU2DoubletSubtract<SU2DoubletBase<3, double>, SU2DoubletShifterByOne<SU2DoubletBase<3, double>, 1>>>);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2AlgebraTester> test;
}
