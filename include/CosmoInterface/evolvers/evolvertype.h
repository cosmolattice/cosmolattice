#ifndef COSMOINTERFACE_EVOLVERS_EVOLVERTYPE_H
#define COSMOINTERFACE_EVOLVERS_EVOLVERTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/exception.h"

namespace TempLat
{

  MakeException(NotAnEvolverType);

  /** @brief An enum, listing all available evolver types:
   * - LF: leapfrog
   * - VV2: velocity verlet, order 2
   * - VV4: velocity verlet, order 4
   * - VV6: velocity verlet, order 6
   * - VV8: velocity verlet, order 8
   * - VV10: velocity verlet, order 10
   * - VV6_2: alternative scheme for velocity verlet order 6 (see documentation)
   * - PV2: position verlet, order 2
   * - PV4: position verlet, order 4
   * - PV6: position verlet, order 6
   * - PV8: position verlet, order 8
   * - PV10: position verlet, order 10
   * - PV6_2: alternative scheme for position verlet order 6 (see documentation)
   * - RK2: Runge-Kutta order 2
   * - RK3_3: 3rd order 3 stages low storage Runge-Kutta
   * - RK3_4: 3rd order 4 stages low storage Runge-Kutta
   * - RK4_5: 4th order 5 stages low storage Runge-Kutta
   *
   **/
  enum EvolverType {
    LF,
    VV2,
    VV4,
    VV6,
    VV8,
    VV10,
    VV6_2,
    RK2,
    RK3_3,
    RK3_4,
    RK4_5,
    PV2,
    PV4,
    PV6,
    PV8,
    PV10,
    PV6_2
  };

  std::istream &operator>>(std::istream &in, EvolverType &eType)
  {
    std::string tmp;
    in >> tmp;
    if (tmp == "LF" || tmp == "0")
      eType = LF; // leapfrog
    else if (tmp == "VV2" || tmp == "1")
      eType = VV2; // velocity verlet: order 2
    else if (tmp == "VV4" || tmp == "2")
      eType = VV4; // ...order 4
    else if (tmp == "VV6" || tmp == "3")
      eType = VV6; // ...order 6
    else if (tmp == "VV8" || tmp == "4")
      eType = VV8; // ...order 8
    else if (tmp == "VV10" || tmp == "5")
      eType = VV10; // ...order 10
    else if (tmp == "VV6_2" || tmp == "6")
      eType = VV6_2; // alternative scheme for VV6 (see documentation)
    else if (tmp == "RK2" || tmp == "7")
      eType = RK2; // RK2
    else if (tmp == "RK3_3" || tmp == "8")
      eType = RK3_3; // 3rd order 3 stages low storage RK
    else if (tmp == "RK3_4" || tmp == "9")
      eType = RK3_4; // 3rd order 4 stages low storage RK
    else if (tmp == "RK4_5" || tmp == "10")
      eType = RK4_5; // 4th order 5 stages low storage RK
    else if (tmp == "PV2" || tmp == "11")
      eType = PV2; // position verlet: order 2
    else if (tmp == "PV4" || tmp == "12")
      eType = PV4; // ...order 4
    else if (tmp == "PV6" || tmp == "13")
      eType = PV6; // ...order 6
    else if (tmp == "PV8" || tmp == "14")
      eType = PV8; // ...order 8
    else if (tmp == "PV10" || tmp == "15")
      eType = PV10; // ...order 10
    else if (tmp == "PV6_2" || tmp == "16")
      eType = PV6_2; // alternative scheme for PV6 (see documentation)
    else if (tmp.empty()) {
    } // Otherwise crash for optional parameters.
    else
      throw(NotAnEvolverType(tmp + " is not an evolver type, abort."));
    return in;
  };

  std::string to_string(EvolverType eType)
  {
    if (eType == LF)
      return "LF";
    else if (eType == VV2)
      return "VV2";
    else if (eType == VV4)
      return "VV4";
    else if (eType == VV6)
      return "VV6";
    else if (eType == VV8)
      return "VV8";
    else if (eType == VV10)
      return "VV10";
    else if (eType == VV6_2)
      return "VV6_2";
    else if (eType == RK2)
      return "RK2";
    else if (eType == RK3_3)
      return "RK3_3";
    else if (eType == RK3_4)
      return "RK3_4";
    else if (eType == RK4_5)
      return "RK4_5";
    else if (eType == PV2)
      return "PV2";
    else if (eType == PV4)
      return "PV4";
    else if (eType == PV6)
      return "PV6";
    else if (eType == PV8)
      return "PV8";
    else if (eType == PV10)
      return "PV10";
    else if (eType == PV6_2)
      return "PV6_2";
    else
      return "";
  }
} // namespace TempLat

#endif
