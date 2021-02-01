#ifndef TEMPLAT_UTIL_HASH_KECCAKHASH_TEST_H
#define TEMPLAT_UTIL_HASH_KECCAKHASH_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::KeccakHash::Test(TempLat::TDDAssertion& tdd) {

    
    auto&& compare = [&tdd](auto&& a, auto&& b) {
        std::string computed = KeccakHash::compute(a);
        std::string manual = b;
        
        tdd.verify( computed == manual );
    };
    
    compare("", "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a");
    compare("Hallo!0", "9fd74e50fd3c8efa609e61a31ecc40db8fc2cf857c92fb1951fd05ffc05ba231");
    compare("Hallo!1", "72a2a9d73afef64b3a0be1499d824fef86fb8f60de0713df09c5e02023fdb05c");
}

#endif
