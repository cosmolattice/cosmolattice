#ifndef TEMPLAT_UTIL_RANGEITERATION_TAGLIST_H
#define TEMPLAT_UTIL_RANGEITERATION_TAGLIST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/concat.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class which implements an enumerated list.
     *
     *
     * Unit test: make test-taglist
     **/
    template<int...I>
    class TagListBase {
    public:
        /* Put public methods here. These should change very little over time. */
        TagListBase() :
        tags(std::make_tuple(Tag<I>()...))
        {

        }

    //private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        std::tuple<Tag<I>...> tags;


    };




    //This comes from  http://spraetor.github.io/2016/01/02/template-integer-sequence.html
    //Unpack the TagList<T,N> in TagListBase<T,0,1,2,...> in Olog(N) compile time.



    template <int Start, int End>
    struct TagListHelper ;

    template <int Start, int End>
    using MakeSeqImplTagList_t = typename TagListHelper< Start, End>::type;

    template <int N>
    using MakeSeqTagList_t = typename TagListHelper<0, N-1>::type;


    template <int... I1s, int... I2s>
    struct Concat<TagListBase<I1s...>, TagListBase<I2s...>> {
      using type = TagListBase<I1s..., I2s...>;
    };

    template <int Start, int End>
    struct TagListHelper  {
      using type = Concat_t<MakeSeqImplTagList_t<Start, (Start+End)/2>,
                            MakeSeqImplTagList_t<(Start+End)/2+1, End> >;
    };

  // break condition:
    template <int I>
    struct TagListHelper <I,I> {
      using type = TagListBase<I>;
    };

    template <int START, int END>
    using TagList = typename TagListHelper<START, END>::type;



} /* TempLat */



#endif
