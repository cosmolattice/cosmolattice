#ifndef COSMOINTERFACE_EVOLVERS_POSITIONVERLETPARAMETERS_H
#define COSMOINTERFACE_EVOLVERS_POSITIONVERLETPARAMETERS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Released under the MIT license, see LICENSE.md. */

#include "CosmoInterface/evolvers/evolvertype.h"
#include "TempLat/util/exception.h"

namespace TempLat
{
  MakeException(NotPVEvolver);

  /** @brief A class which stores the composition parameters for position Verlet schemes.
   *
   *
   **/
  template <typename T> struct PositionVerletParameters {
    static std::vector<T> getCoeffs(EvolverType ev)
    {
      std::vector<T> res;
      if (ev == PV2) { // composition parameters for position verlet, 2nd order
        res = {1.0};
      } else if (ev == PV4) { // ...4th order
        res = {1.351207191959657771818, -1.702414403875838200264};
      } else if (ev == PV6) { // ...6th order
        res = {0.78451361047755726382, 0.23557321335935813368, -1.1776799841788710069, 1.3151863206839112189};
      } else if (ev == PV6_2) { // ...6th order (alternative scheme)
        res = {0.39216144400731413928, 0.33259913678935943860, -0.70624617255763935981, 0.082213596293550800230,
               0.79854399093482996340};
      } else if (ev == PV8) { // ...8th order
        res = {0.74167036435061295345, -0.40910082580003159400, 0.19075471029623837995, -0.57386247111608226666,
               0.29906418130365592384, 0.33462491824529818378,  0.31529309239676659663, -0.79688793935291635402};
      } else if (ev == PV10) { // ... 10th order
        res = {-0.48159895600253002870, 0.0036303931544595926879, 0.50180317558723140279,  0.28298402624506254868,
               0.80702967895372223806,  -0.026090580538592205447, -0.87286590146318071547, -0.52373568062510581643,
               0.44521844299952789252,  0.18612289547097907887,   0.23137327866438360633,  -0.52191036590418628905,
               0.74866113714499296793,  0.066736511890604057532,  -0.80360324375670830316, 0.91249037635867994571};
      } else {
        throw(NotPVEvolver("You tried to get the composition parameters of a non-position verlet evolver, abort."));
      }
      return res;
    }

    // Returns accuracy order of the position-verlet algorithm
    static size_t getOrder(EvolverType ev)
    {
      size_t res = 0;

      if (ev == PV2)
        res = 2;
      else if (ev == PV4)
        res = 4;
      else if (ev == PV6 or ev == PV6_2)
        res = 6;
      else if (ev == PV8)
        res = 8;
      else if (ev == PV10)
        res = 10;
      else {
        throw(NotPVEvolver("You tried to get the accuracy order of a non-position verlet evolver, abort."));
      }
      return res;
    }

    // Checks if the evolution algorithm is or isn't position verlet
    static bool isVerlet(EvolverType ev)
    {
      bool res = false;
      if (ev == PV2 or ev == PV4 or ev == PV6 or ev == PV8 or ev == PV10 or ev == PV6_2) res = true;
      return res;
    }
  };
} // namespace TempLat

#endif
