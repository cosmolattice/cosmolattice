#ifndef TEMPLAT_UTIL_RANGEITERATION_TAGLITERAL_H
#define TEMPLAT_UTIL_RANGEITERATION_TAGLITERAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: Library.

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class which implements a litteral to create tags, comes from Boost Hana.
     *
     * 
     * Unit test: make test-tagliteral
     **/

    /*Disclaimer: The following two functions are taken as is from boost::hana from the integral_constants file, credit to Louis Dionne. As such, they are
     * released under the Boost License.
     * */

    constexpr int to_int(char c) {
        int result = 0;

        if (c >= 'A' && c <= 'F') {
            result = static_cast<int>(c) - static_cast<int>('A') + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            result = static_cast<int>(c) - static_cast<int>('a') + 10;
        }
        else {
            result = static_cast<int>(c) - static_cast<int>('0');
        }

        return result;
    }

    template<std::size_t N>
    constexpr long long parse(const char (&arr)[N]) {
        long long base = 10;
        std::size_t offset = 0;

        if (N > 2) {
            bool starts_with_zero = arr[0] == '0';
            bool is_hex = starts_with_zero && arr[1] == 'x';
            bool is_binary = starts_with_zero && arr[1] == 'b';

            if (is_hex) {
                //0xDEADBEEF (hexadecimal)
                base = 16;
                offset = 2;
            }
            else if (is_binary) {
                //0b101011101 (binary)
                base = 2;
                offset = 2;
            }
            else if (starts_with_zero) {
                //012345 (octal)
                base = 8;
                offset = 1;
            }
        }

        long long number = 0;
        long long multiplier = 1;

        for (std::size_t i = 0; i < N - offset; ++i) {
            char c = arr[N - 1 - i];
            if (c != '\'') { // skip digit separators
                number += to_int(c) * multiplier;
                multiplier *= base;
            }
        }

        return number;
    }
    // END disclaimer.

    template <char ...c>
    constexpr auto operator"" _c() { //Adapted from boost::hana as well, but not as such.
        return Tag<parse<sizeof...(c)>({c...})>{};
    }

    struct TagLiteralTester {

#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/rangeiteration/tagliteral_test.h"
#endif


#endif
