#ifndef TEMPLAT_UTIL_HASH_LIBKECCAK_TWOENDIANS_H
#define TEMPLAT_UTIL_HASH_LIBKECCAK_TWOENDIANS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
/* \file This file includes the libkeccak.h header twice, in different namespaces.
 Once with little_endian, once with big_endian.
 */

#ifndef INSIDE_TempLat_UTIL_HASH_KECCAKHASHBARECLASS_H
#error Do not include this file anywhere else than in the controlled header keccakhash.h
#endif

// WV:
#include <cstdint>
#include <cstddef>

#include <cstring>


#include "TempLat/util/tdd/tdd.h"

#define INSIDE_TempLat_UTIL_HASH_LIBKECCAK_TWOENDIANS_H

namespace TempLat {
    namespace KECCAK {
        /* prepended CLAT_ in order to avoid conflicts with preprocessor definitions. */
        namespace CLAT_LITTLE_ENDIAN {
#ifndef LITTLE_ENDIAN
/* need to define this if your platform is LITTLE_ENDIAN (most user machines are) when including true keccak */
#define LITTLE_ENDIAN
#define REMOVEENDIANAGAIN
#endif

#include "TempLat/util/hash/libkeccak.h"

#ifdef REMOVEENDIANAGAIN
#undef LITTLE_ENDIAN
#undef REMOVEENDIANAGAIN
#endif

        }
        namespace CLAT_BIG_ENDIAN {
#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#define REINSTATEENDIANAGAIN
#endif

#include "TempLat/util/hash/libkeccak.h"

#ifdef REINSTATEENDIANAGAIN
#define LITTLE_ENDIAN
#undef REINSTATEENDIANAGAIN
#endif
        }
    }
}

#undef INSIDE_TempLat_UTIL_HASH_LIBKECCAK_TWOENDIANS_H

#ifdef TEMPLATTEST
#include "TempLat/util/hash/libkeccak_twoendians_test.h"
#endif

#endif
