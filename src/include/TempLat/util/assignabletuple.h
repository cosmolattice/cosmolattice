#ifndef TEMPLAT_UTIL_ASSIGNABLETUPLE_H
#define TEMPLAT_UTIL_ASSIGNABLETUPLE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/concat.h"

#include "TempLat/lattice/field/assignablefieldcollection.h"

namespace TempLat {


    /** \brief A class which makes tuple out of things that have an operator equal. Dispatch the equal.
     *
     * 
     * Unit test: make test-assignabletuple
     **/

    template <int Shift, class... Args>
    class AssignableTuple : public AssignableCollectionBase<AssignableTuple<Shift, Args...>, Args...>{
    public:
        using AssignableCollectionBase<AssignableTuple<Shift, Args...>, Args...>::operator=;
        using AssignableCollectionBase<AssignableTuple<Shift, Args...>, Args...>::size;

        /* Put public methods here. These should change very little over time. */
        AssignableTuple(Args... args) :
        AssignableCollectionBase<AssignableTuple<Shift,Args...>, Args...>(args...){

        }

        template<int N>
        auto& getComp(Tag<N> t)
        {
            return (*this)(t+Tag<Shift>()); //get component runs from 0 to N-1 always. (internal getter).
        }

        auto getComp(ptrdiff_t i)
        {
            typedef typename GetGetReturnType<decltype((*this)(std::declval<Tag<0>>()))>::type SV;
            std::array<SV,size> ret;
            for_in_range<0,size>(
                    [&](auto j)
                    {
                        ret[j] = GetValue::get(getComp(j),i);
                    }
                    );
            return ret;
        }

        using Getter = GetComponent;
        static constexpr int SHIFTIND = Shift;


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    };

    struct AssignableTupleTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<class... Args>
    auto make_list(Args... args)
    {
        return AssignableTuple<0,Args...>(args...);
    }
    template<class... Args>
    auto make_vector(Args... args)
    {
        return AssignableTuple<1,Args...>(args...);
    }


    template <typename T, class... Args>
    auto make_list_from_array_impl(std::array<T,1> arr, Args... args)
    {
        return make_list(arr[0], args...);
    }

    template <typename T, size_t N, class... Args>
    auto make_list_from_array_impl(std::array<T,N> arr, Args... args)
    {
        std::array<T,N-1> nArr;
        for( int i = 0; i < (int)N-1 ;++i) nArr[i] = arr[i];
        return make_list_from_array_impl(nArr,arr[N-1], args...);
    }

    template <typename T, size_t N>
    auto make_list_from_array(std::array<T,N> arr)
    {
        return make_list_from_array_impl(arr);
    }

    template <typename T>
    int make_list_from_array(std::array<T,0> arr)
    {
        return 0;
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/assignabletuple_test.h"
#endif


#endif
