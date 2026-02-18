#ifndef TEMPLAT_UTIL_STATICIF_H
#define TEMPLAT_UTIL_STATICIF_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  class StaticIfTester
  {
#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };

#define IfElse(condition, ifExpr, elseExpr)                                                                              \
  [&]() { if constexpr (condition) { return ifExpr; } else { return elseExpr; } }()
#define If(condition, ifExpr)                                                                                            \
  [&]() { if constexpr (condition) { return ifExpr; } else { return ZeroType(); } }()

#define IfElseStatement(condition, ifExpr, elseExpr)                                                                     \
  [&]() { if constexpr (condition) { ifExpr; } else { elseExpr; } }()
#define IfStatement(condition, ifExpr)                                                                                   \
  if constexpr (condition) { ifExpr; }
} // namespace TempLat

#endif
