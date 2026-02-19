
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/log.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct LogTester {
    static void Test(TDDAssertion &tdd);
  };

  void LogTester::Test(TDDAssertion &tdd)
  {

    class myClass
    {
    public:
      DEVICE_FUNCTION
      myClass(int b) : a(b) {}

      DEVICE_FORCEINLINE_FUNCTION
      auto eval(const double &i) const { return a; }

    private:
      double a;
    };

    myClass a(3);

    tdd.verify(log(a).eval(0) == std::log(3));
    //  std::cerr << log(a).d(a).toString() << "\n";

    /* Default is to fail: to remind yourself to implement something here. */
    //    tdd.verify( true );
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::LogTester> test;
}
