
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/filetostring.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/namedtmpfile.h"

namespace TempLat
{

  struct FileToStringTester {
    static void Test(TDDAssertion &tdd);
  };

  void FileToStringTester::Test(TDDAssertion &tdd)
  {

    NamedTmpFile ntf;

    std::string payload = "Hello world!";

    ntf << payload;

    ntf.close();

    FileToString fts(ntf.getName());

    std::string ftsRead = fts;

    tdd.verify(ftsRead == payload);

    tdd.verify(ntf.remove() == 0);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FileToStringTester> test;
}
