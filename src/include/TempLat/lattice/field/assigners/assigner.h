#ifndef TEMPLAT_LATTICE_FIELD_ASSIGNERS_ASSIGNER_H
#define TEMPLAT_LATTICE_FIELD_ASSIGNERS_ASSIGNER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/rangeiteration/tagliteral.h"


namespace TempLat {


    /** \brief A class which can be used to "outsource" the assignement operator.
     *
     * 
     * Unit test: make test-assigner
     **/

    class Assigner {
    public:

        template<typename Tup1, typename Tup2>
        static void assign( Tup1&& tup1, Tup2&& tup2) //Can do different children classes with different getter to have different algebras.
        {

            constexpr size_t flatSize = tuple_size<typename std::remove_reference<decltype(tup1)>::type>::value;

            for_in_range<0,flatSize>(
                    [&](auto j){
                        std::get<j>(tup1).onBeforeAssignment(std::get<j>(tup2));
                    });

            int i=0;

            auto dirtyHackSoItCompilesWithEmptyTuples = static_if<(flatSize > 0)>(tup1, ZeroType());
            auto& it= GetToolBox::get(GetComponent::get(dirtyHackSoItCompilesWithEmptyTuples,0_c))->itX();
            //auto it = std::get<0>(fs).getToolBox()->mCSpaceItThreaded;

            for(it.begin();it.end();++(it))
            {
                i=it();

                for_in_range<0,flatSize>(
                        //[this,i,&list](auto j){
                        [&](auto j){
                            DoEval::eval(std::get<j>(tup2),i);
                            std::get<j>(tup1).getSet(i) =  GetValue::get( std::get<j>(tup2),i);
                        });
            }
            //auto assigners = make_tuple_tag<0,size>([&](auto j){return MakeRealValueAssigner(std::get<j>(list),std::get<j>(fs));});

            //it.iterate(assigners);

            for_each(tup1,[](auto& x){
                x.setGhostsAreStale();
            });
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/assigners/assigner_test.h"
#endif


#endif
