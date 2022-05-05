#ifndef TEMPLAT_UTIL_STRINGTRIMMER_H
#define TEMPLAT_UTIL_STRINGTRIMMER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


/* from http://stackoverflow.com/a/217605 */
// 2022: Adapted  to lambdas.

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A class which trims strings on all ends.
     * In place: [lr]trim(std::string).
     * Copy: std::string [lr]trimmed(std::string)
     *
     * Unit test: make test-stringtrimmer
     */

    class StringTrimmer {
        public:
        /* Put public methods here. These should change very little over time. */
        static inline void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                            [](int c) {return !std::isspace(c);}));
        }

        // trim from end (in place)
        static inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                                 [](int c) {return !std::isspace(c);}).base(), s.end());
        }

        // trim from both ends (in place)
        static inline void trim(std::string &s) {
            ltrim(s);
            rtrim(s);
        }

        // trim from start (copying)
        static inline std::string ltrimmed(std::string s) {
            ltrim(s);
            return s;
        }

        // trim from end (copying)
        static inline std::string rtrimmed(std::string s) {
            rtrim(s);
            return s;
        }

        // trim from both ends (copying)
        static inline std::string trimmed(std::string s) {
            trim(s);
            return s;
        }



        private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



        public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/stringtrimmer_test.h"
#endif


#endif
