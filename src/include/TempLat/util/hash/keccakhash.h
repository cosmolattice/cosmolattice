#ifndef TEMPLAT_UTIL_HASH_KECCAKHASH_H
#define TEMPLAT_UTIL_HASH_KECCAKHASH_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
/* \file This file uses libkeccak.h, the stand-alone implementation
 * of the keccak hash algorithm. The reason to use this rather than
 * std::hash, is that std::hash is explicitly not guaranteed to be stable
 * across implementations (compilers, platforms, ...).
 * We insist on having something stable across platforms,
 * such that your hashes which you may use for random seeds
 * are the same, such that the physics you simulate is identical
 * across platforms. */


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/hash/keccakhashbareclass.h"

namespace TempLat {
    
    /** \brief A class which computes (statically) the keccak hash of one string.
     *
     * Unit test: make test-keccakhash
     **/
    
    class KeccakHash {
    public:
        /* Put public methods here. These should change very little over time. */
        
        typedef KeccakHashBareClass ResultType;
        
        template <typename T>
        static ResultType compute(const T& input) {
            ResultType result;
            result.compute(input);
            return result;
        }
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        /** \brief Constructor is private: no need to instantiate, just call the static compute. */
        KeccakHash() { }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/hash/keccakhash_test.h"
#endif


#endif
