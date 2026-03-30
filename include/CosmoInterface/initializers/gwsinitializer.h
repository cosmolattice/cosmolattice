#ifndef COSMOINTERFACE_INITIALIZERS_GWSINITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_GWSINITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "TempLat/util/rangeiteration/for_in_range.h"
#include "CosmoInterface/initializers/fluctuationsgenerator.h"

namespace TempLat
{
  /** @brief A class which is used to initialize the gravitational waves fields.
   *
   *
   * Unit test: ctest -R test-scalarinitializer
   **/
  class GWsInitializer
  {
  public:
    // Put public methods here. These should change very little over time.
    GWsInitializer() = delete;

    template <class Model> static void initializeGWs(Model &model)
    {
      ForLoop(i, 0_c, 4_c, (*model.fldGWs)(i) = 0.;
                           (*model.piGWs)(i) = 0.;
      );
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };
} // namespace TempLat

#endif
