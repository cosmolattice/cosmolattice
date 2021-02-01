#ifndef TEMPLAT_UTIL_TDD_TDDASSERTION_H
#define TEMPLAT_UTIL_TDD_TDDASSERTION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <vector>

#include "TempLat/util/log/log.h"

namespace TempLat {

    /** \brief The function to call for the evaluation of each test.
      *
      * Unit test: make test-tddassertion
      */

    class TDDAssertion {
    public:
    /* Put public methods here. These should change very little over time. */
        TDDAssertion() : results(std::make_shared<std::vector<std::pair<bool, std::string>>>()), goodScore(0), badScore(0) { }
        
        /** \brief Call with assert( a == b, "A must equal B."). */
        void operator()(const char *stringifiedExpression, const char *fName, int line, bool result, std::string description = std::string()) {
            results->emplace_back(result, stringifiedExpression);
            RecordSuccess(result);
            SayComplete(fName, line) << (result ? ColouredOK() : ColouredERROR() ) << " " << std::string(stringifiedExpression) << " " << description << "\n";
        }
        
        
        void ASSERT(const char *stringifiedExpression, const char *fName, int line, bool result, std::string description = std::string()) {
            results->emplace_back(result, stringifiedExpression);
            RecordSuccess(result);
            SayComplete(fName, line) << (result ? ColouredOK() : ColouredERROR() ) << " " << std::string(stringifiedExpression) << " " << description << "\n";
        }
        
        /** \brief Overload for Throws. */
        void ASSERT(const char *stringifiedExpression, const char *fName, int line, std::pair<bool, std::string> result, std::string description = std::string()) {
            ASSERT(result.second.c_str(), fName, line, result.first, description);
        }
        
        friend std::ostream& operator<< ( std::ostream& stream, const TDDAssertion& ass) {
            stream << (ass.badScore > 0 ? ColouredERROR() : ColouredOK() ) << " Passed " << ass.goodScore << " and failed " << ass.badScore << " tests.";
            return stream;
        }
        
        int FailCount() { return badScore; }
        int PassCount() { return goodScore; }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        std::shared_ptr<std::vector<std::pair<bool, std::string>>> results;
        int goodScore, badScore;
        
        void RecordSuccess(const bool& success ) {
            success ? ++goodScore : ++badScore;
        }



    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/tdd/tddassertion_test.h"
#endif


#endif
