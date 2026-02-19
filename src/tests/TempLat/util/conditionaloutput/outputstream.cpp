
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/conditionaloutput/outputstream.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct OutputStreamTester {
    static void Test(TDDAssertion &tdd);
  };

  void OutputStreamTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    /*
    OutputStream<int> st("testOut.txt",true);
    st.savetxt(1,2,3,4,5,56);
    st.savetxt(1,1,2,3,1,4,5,6,7,3,4,5,56);
    st.savetxt(1,2,6);
    tdd.verify( true );*/
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::OutputStreamTester> test;
}
