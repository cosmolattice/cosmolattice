#ifndef TEMPLAT_UTIL_HASH_KECCAKHASHBARECLASS_H
#define TEMPLAT_UTIL_HASH_KECCAKHASHBARECLASS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

/** \file Only keccakhash.h is allowed to include this file, because of
 *        dirty preprocessor macro usage.
 *        This file is included twice, once in a big-endian namespace,
 *        once in a little-endian namespace.
 */

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"

#define INSIDE_TempLat_UTIL_HASH_KECCAKHASHBARECLASS_H
#include "TempLat/util/hash/libkeccak_twoendians.h"
#undef INSIDE_TempLat_UTIL_HASH_KECCAKHASHBARECLASS_H
#include "TempLat/util/endianness.h"


/* \file This file uses libkeccak.h, the stand-alone implementation
 * of the keccak hash algorithm. The reason to use this rather than
 * std::hash, is that std::hash is explicitly not guaranteed to be stable
 * across implementations (compilers, platforms, ...).
 * We insist on having something stable across platforms,
 * such that your hashes which you may use for random seeds
 * are the same, such that the physics you simulate is identical
 * across platforms. */

/* implemented by WV, but following sources from:
 
 Keccak-compact.c, with the following header:
 The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
 Michaël Peeters and Gilles Van Assche. For more information, feedback or
 questions, please refer to our website: http://keccak.noekeon.org/
 
 Implementation by Ronny Van Keer,
 hereby denoted as "the implementer".
 
 To the extent possible under law, the implementer has waived all copyright
 and related or neighboring rights to the source code in this file.
 http://creativecommons.org/publicdomain/zero/1.0/
 *
 *
 
 * CosmoLattice note: this implementation is much older than CosmoLattice. Sorry if it doesn't abide by
 * the golden coding-style rules.
 
 */

// This must be done in the owning header: #include "TempLat/util/hash/libkeccak_twoendians.h"


namespace TempLat {
    MakeException(KeccakHashBareClassReuseException);
    
    /** \brief A class which computes the keccak hash of one string, which is passed to the method compute.
     * Can compute only once, then keeps the state. Throws an exception if compute is called twice.
     *
     * Unit test: make test-keccakhashbareclass
     **/
    class KeccakHashBareClass {
    public:
        /* Put public methods here. These should change very little over time. */
        
        /** \brief Initialize the states, 0 and 0..n */
        KeccakHashBareClass() : hasBeenUsed ( false ) {
            for ( auto&& it : hash) it = 0;
        }
        
        /** \brief Hash an input array. */
        void compute(const std::vector<unsigned char>& data) {
            absorb(data);
        }
        
        /** \brief Hash an input array as String. */
        void compute(const std::string& data) {
            
            /* skip the trailing '\0', if it is there. */
            std::vector<unsigned char> asVec(data.begin(), data.end());
            while ( asVec.size() && asVec.back() == '\0' ) asVec.pop_back();
            /**/
            
            absorb(asVec);
        }
        
        
        std::string toString() const {
            std::vector<char> theHash(hash.size() * 2 + 1, ' ');
            for (unsigned int i = 0; i < hash.size(); ++i) snprintf(theHash.data() + 2*i, 3, "%02x", hash[i]);
            theHash.back() = '\0';
            /* -1: we remove the trailing '\0', which is not by default present in a std::string. Relevant for test comparisons. */
            return std::string(theHash.begin(), theHash.end() - 1);
        }
        operator std::string() const { return toString(); }
        
        friend std::ostream& operator<<( std::ostream& stream, KeccakHashBareClass kck) {
            stream << kck.toString() << " (" << kck.toUnsignedInt64() << ")";
            return stream;
        }
        
        uint64_t toUnsignedInt64() const {
            /* same as a union: we know our state is 25*8 bits,
             so safe to cast as 64 bits data and dereference. */
            return *( (uint64_t*) hash.data() );
        }
        operator uint64_t() const { return toUnsignedInt64(); }
        operator int64_t() const { return toUnsignedInt64(); }
        
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        typedef uint8_t lane;
        std::array<uint8_t, 32> hash;
        Endianness endian;
        bool hasBeenUsed;
        
        void absorb(const std::vector<unsigned char>& data) {
            if ( hasBeenUsed ) throw KeccakHashBareClassReuseException("A KeccakHashBareClass can only compute once, and then has a permanent state. Create a new one.");
            hasBeenUsed = true;
            if ( endian.isLittle() ) {
                KECCAK::CLAT_LITTLE_ENDIAN::TrueKeccak workspace;
                workspace.FIPS202_SHA3_256(data.data(), data.size(), hash.data());
            } else {
                KECCAK::CLAT_BIG_ENDIAN::TrueKeccak workspace;
                workspace.FIPS202_SHA3_256(data.data(), data.size(), hash.data());
            }
        }
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
    
}

#ifdef TEMPLATTEST
#include "TempLat/util/hash/keccakhashbareclass_test.h"
#endif


#endif
