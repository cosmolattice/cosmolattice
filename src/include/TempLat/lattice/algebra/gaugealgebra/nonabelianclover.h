#ifndef TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_NONABELIANCLOVER_H
#define TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_NONABELIANCLOVER_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2024

#include "TempLat/lattice/algebra/su2algebra/su2shift.h"

namespace TempLat
{

  /** @brief A class which implement the clover discretization of non-abelian magnetic fields.
   *
   *
   * Unit test: ctest -R test-nonabelianclover
   **/
  template <int Mu, int Nu, typename R> auto nonabelianclover(const R &Us, Tag<Mu> mu, Tag<Nu> nu)
  {

    // Clover is
    // <-----^  <----^
    // |  2  |  |  1 |
    // |     |  |    |
    // v---->   v---->
    // <-----^  <----^
    // |  3  |  |  4 |
    // |     |  |    |
    // v---->   v---->
    /*auto plaq1 =  (Us(mu) * shift(Us(nu),mu) ) * ( dagger(shift(Us(mu),nu)) * dagger(Us(nu)) ); //of course we could
    call the plaquette here, but we don't for readability. auto plaq2 =  (Us(nu) * dagger(shift(shift(Us(mu),-mu),nu)))
    * ( dagger(shift(Us(nu),-mu)) * shift(Us(mu), -mu)); auto plaq3 =  (dagger(shift(Us(mu),-mu)) *
    dagger(shift(shift(Us(nu),-mu),-nu))) * ( shift(shift(Us(mu),-mu),-nu) * shift(Us(nu), -nu)); auto plaq4 =
    (dagger(shift(Us(nu),-nu)) * shift(Us(mu),-nu)) * ( shift(shift(Us(nu),-nu),mu) * dagger(Us(mu)));*/

    auto plaq1 = (Us(mu) * sh1<Mu>(Us(nu))) *
                 (dag(sh1<Nu>(Us(mu))) *
                  dag(Us(nu))); // of course we could call the plaquette here, but we don't for readability.
    auto plaq2 = (Us(nu) * dag(sh1<-Mu, Nu>(Us(mu)))) * (dag(sh1<-Mu>(Us(nu))) * sh1<-Mu>(Us(mu)));
    auto plaq3 = (dag(sh1<-Mu>(Us(mu))) * dag(sh1<-Mu, -Nu>(Us(nu)))) * (sh1<-Mu, -Nu>(Us(mu)) * sh1<-Nu>(Us(nu)));
    auto plaq4 = (dag(sh1<-Nu>(Us(nu))) * sh1<-Nu>(Us(mu))) * (sh1<-Nu, Mu>(Us(nu)) * dag(Us(mu)));

    return 0.25 * (plaq1 + plaq2 + plaq3 + plaq4);
  }

  template <typename R> auto B4NA(const R &Us, Tag<1>) { return nonabelianclover(Us, Tag<2>(), Tag<3>()); }

  template <typename R> auto B4NA(const R &Us, Tag<2>) { return nonabelianclover(Us, Tag<3>(), Tag<1>()); }
  template <typename R> auto B4NA(const R &Us, Tag<3>) { return nonabelianclover(Us, Tag<1>(), Tag<2>()); }

} // namespace TempLat

#endif
