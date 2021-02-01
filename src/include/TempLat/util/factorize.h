#ifndef TEMPLAT_UTIL_FACTORIZE_H
#define TEMPLAT_UTIL_FACTORIZE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <algorithm>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"

namespace TempLat {

    MakeException(FactorizationFailed);

    /** \brief A class which factorizes an integer into its primes.
     *
     * Unit test: make test-factorize
     **/

    class Factorize {
    public:
    /* Put public methods here. These should change very little over time. */
        Factorize(ptrdiff_t total) :
        factors(performFactorization(total))
        {

        }

        /** \brief access */
        const ptrdiff_t& operator[](ptrdiff_t i) const {
            return factors[i];
        }

        /** \brief size */
        size_t size() const {
            return factors.size();
        }

        /** iterate */
        const auto begin() const { return factors.begin(); }

        /** iterate */
        const auto end() const { return factors.end(); }

        /** We are sorted already, but let's just give the caller peace of mind. */
        void sort(bool reverse = false) {
            if ( ! reverse ) {
                std::sort(factors.begin(), factors.end());
            } else {
                std::sort(factors.begin(), factors.end(), [](ptrdiff_t a, ptrdiff_t b) { return a > b; });
            }
        }

        friend std::ostream& operator<<( std::ostream& ostream, Factorize fac) {
            ostream << fac.factors;
            return ostream;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        std::vector<ptrdiff_t> factors;

        std::vector<ptrdiff_t> performFactorization(const ptrdiff_t value) {
            /* just iterate from two up to value/2 + 2 and get the factors. */
            std::vector<ptrdiff_t> result;
            if ( value == 1 ) {
                result.push_back(1);
            } else {
                ptrdiff_t cp = value;
                for ( ptrdiff_t f = 2; f < value + 1; ++f) {
                    while ( cp >= f && cp % f == 0 ) {
                        cp /= f;
                        result.push_back(f);
                    }
                }
            }
            
            /* verify the result */
            ptrdiff_t test = 1;
            for ( auto&& it : result ) test *= it;
            if ( test != value ) {
                throw FactorizationFailed(test, "!=", value, "from", result);
            }
            return result;
        }


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/factorize_test.h"
#endif


#endif
