#ifndef TEMPLAT_UTIL_STATICIF_H
#define TEMPLAT_UTIL_STATICIF_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which implement a static if container.
     *
     * 
     * Unit test: make test-staticif
     **/

    template<typename  F, typename G, bool B>
    class static_if_helper {
    public:
        static_if_helper(const F& pF, const G& pG) : f(pF), g(pG) {}

        F operator()(){
            return f;
        }
    private:
        F f;
        G g;
    };

    template<typename  F, typename  G>
    class static_if_helper<F,G,false> {
    public:
        static_if_helper(const F& pF, const G& pG) : f(pF), g(pG) {}

        G operator()(){
            return g;
        }
    private:
        F f;
        G g;
    };
    template< bool B, class F, class G>
    auto static_if(const F& f, const G& g)
    {
        return TempLat::static_if_helper<F,G,B>(f,g)();
    }


    template<typename  F, typename G, bool B>
    class static_if_statement_helper {
    public:
        static_if_statement_helper(const F& pF, const G& pG) : f(pF), g(pG) {}

        void operator()(){
             f();
        }
    private:
        F f;
        G g;
    };

    template<typename  F, typename  G>
    class static_if_statement_helper<F,G,false> {
    public:
        static_if_statement_helper(const F& pF, const G& pG) : f(pF), g(pG) {}

        void operator()(){
            g();
        }
    private:
        F f;
        G g;
    };
    template< bool B, class F, class G>
    auto static_if_statement(const F& f, const G& g)
    {
        return TempLat::static_if_statement_helper<F,G,B>(f,g)();
    }

    class StaticIfTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



#define IfElse(condition, ifExpr, elseExpr) static_if<condition>(ifExpr,elseExpr)
#define If(condition, ifExpr) static_if<condition>(ifExpr,ZeroType())

#define IfElseStatement(condition, ifExpr, elseExpr) static_if_statement<condition>([&](){ifExpr;},[&](){elseExpr;})
#define IfStatement(condition, ifExpr) static_if_statement<condition>([&](){ ifExpr;},[&](){})


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/staticif_test.h"
#endif


#endif
