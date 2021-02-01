#ifndef TEMPLAT_SESSION_SESSIONGUARD_TEST_H
#define TEMPLAT_SESSION_SESSIONGUARD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::SessionGuard::Test(TempLat::TDDAssertion& tdd) {

    if ( SessionGuard::InstanceCounter() < 1 ) {
        SessionGuard guard(0, NULL, true);
    } else {
        /* there is an instance of MPIGuard in the calling main, which is a good thing. Then we can test if the multiple instantiation protection works. */
        tdd.verify( Throws<SessionGuardInstantiationException>(
                   []() {
                       SessionGuard guard(0, NULL, true);
                   }
        ) );

    }

}

#endif
