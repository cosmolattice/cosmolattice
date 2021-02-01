#ifndef TEMPLAT_UTIL_FLOATTOSTRING_H
#define TEMPLAT_UTIL_FLOATTOSTRING_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>
#include <cstring>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/stringtrimmer.h"

namespace TempLat {
    
    /** \brief A class which holds one static method (format) which returns
     *  a nicer string representation of a floating point value,
     *  for display purposes (not too many significant digits).
     *
     * Unit test: make test-floattostring
     **/
    
    class FloatToString {
    public:
        
        template <typename T>
        static inline
        typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
        format(T value) {
            char buffer[128];
            std::memset(buffer, 0, 128);
            if ( (std::abs(value) > 1e3 || std::abs(value) < 1e-2) && std::abs(value) != 0 ) {
                snprintf(buffer, 128, "%11.2e", value);
                stripTrailingZeros(buffer, findThe_E(buffer), '*');
                squashChars(buffer, '*');
            } else {
                if (std::abs(value) < 1e-1) snprintf(buffer, 128, "%.3f", value);
                else if (std::abs(value) < 1) snprintf(buffer, 128, "%.2f", value);
                else snprintf(buffer, 128, "%.1f", value);
                stripTrailingZeros(buffer, buffer + 128, '\0');
            }
            return StringTrimmer::trimmed(buffer);
        }
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        
        /** \brief Replace zeros and optional dot by `replacement`, counting back from rend up to rbegin. */
        static inline void stripTrailingZeros(char *begin, char *end, char replacement) {

            if (end <= begin ) return;

            bool goOn = true;
            for (char *i = end - 1; i >= begin && goOn; --i) {
                switch (*i) {
                    case '\0':
                        break;
                    case '0':
                        /* cut trailing zeros. */
                        *i = replacement;
                        break;
                    case '.':
                        /* cut trailing dot. and stop. */
                        *i = replacement;
                        goOn = false;
                        break;
                    default:
                        /* anything other than zero or dot? Done with the cutting. */
                        goOn = false;
                        break;
                }
            }
        }
        
        /** \brief Finds the E/e character in a scientifically formatted float. Expects a C-style zero-terminated string. */
        static inline char *findThe_E(char *string) {
            while (*string != '\0' && *string != 'e' && *string != 'E') ++string;
            return string;
        }
        
        /** \brief Removes characters matching `replacement`, by moving the following characters forward. Expects a C-style zero-terminated string. */
        static inline void squashChars(char *string, char replacement) {

            bool insideBatch = false;

            char *batchStart = string;

            char *iEnd = string;

            /* note that with this definition iEnd is a valid pointer, to the trailing '\0'-char. */
            while (*iEnd != '\0' ) ++iEnd;

            for ( char *i = string; i < iEnd; ++i) {

                if ( *i == replacement ) {
                    if ( ! insideBatch ) batchStart = i;
                    insideBatch = true;
                } else {
                    if ( insideBatch ) {
                        insideBatch = false;
                        /* squash everything between batchStart and i  */
                        ptrdiff_t batchSize = iEnd - i + 1; /* +1, because iEnd is a valid pointer to the trailing '\0'-char. */
                        std::memmove( batchStart, i, batchSize);
                    }
                }
            }
        }
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/floattostring_test.h"
#endif


#endif
