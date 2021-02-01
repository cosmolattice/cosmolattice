#ifndef TEMPLAT_LATTICE_FIELD_ASSIGNABLEFIELDCOLLECTION_H
#define TEMPLAT_LATTICE_FIELD_ASSIGNABLEFIELDCOLLECTION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/shiftedindexsequence.h"
#include "TempLat/util/foreach.h"
#include "TempLat/util/concat.h"
#include "TempLat/util/istuplelike.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/lattice/field/assigners/assigner.h"
#include "TempLat/util/tuplemaker.h"
#include "TempLat/util/flattentuple.h"


namespace TempLat {


    /** \brief A class which implements collections of object which are assignable.
     *
     * 
     * Unit test: make test-assignablefieldcollection
     **/

    // if want to try to have dynamical access somehow, see
    // https://www.justsoftwaresolutions.co.uk/cplusplus/getting-tuple-elements-with-runtime-index.html
    // seems hardly compatible with indexed fields though.



    template<class Q, class...Args>
    class AssignableCollectionBase {
    public:
        AssignableCollectionBase(Args... args):  //This constructor is used to create composite structure such as SU(N) matrices, from fields.
                fs(std::make_tuple(args...))
        {
        }

        void operator=(const AssignableCollectionBase<Args...>& other)
        {
            for_in_range<0,size>(
                    [&](auto j){
                        std::get<j>(fs) = std::get<j>(other.fs);
                    });
        }


        template <typename R>
        typename std::enable_if<!std::is_same<R,AssignableCollectionBase<Args...>>::value>::type
        operator=(R&& r)
        {
            //auto tup1 = flatten_tuple(make_tuple_from(static_cast<Q&>(*this)));
            //auto tup2 = flatten_tuple(make_tuple_from(std::forward<R>(r)));
            //Assigner::assign(tup1, tup2);
            using nakedR = typename std::remove_cv<typename std::remove_reference<R>::type>::type;
            for_in_range<0,size>(
                    [&](auto j){
                        std::get<j>(fs) = nakedR::Getter::get(r,j);
                    });
        }

        template<int M>
        auto& operator()(Tag<M> t) //Also work with operator [], but might be confusing that it is NOT an int (cant call f[1] -> f[1_c])
        {
            return std::get<M - Q::SHIFTIND>(fs);
        }


        static constexpr size_t size = tuple_size<std::tuple<Args...>>::value;

    protected:

        std::tuple<Args...> fs;



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
    };

    template<class Q> //Specialise to empty field collection, that does nothing. Useful to define general models for example, which may have some empty collections.
    class AssignableCollectionBase<Q> {
    public:
        /* Put public methods here. These should change very little over time. */
        AssignableCollectionBase() {}
        template<int N>
        int operator()(Tag<N> t){return 0;}
        template<int N>
        int getComp(Tag<N> t){return 0;}
        template<typename R>
        void operator=( R&& g){}
        static constexpr size_t size = 0;

    };



    template<class Q, typename T, CANONICALTYPE ISMOMENTUM, int...I>
    class CollectionBase : public  AssignableCollectionBase<Q,Field<T>>{
    public:
        using AssignableCollectionBase<Q,Field<T>>::operator=;

        /* Put public methods here. These should change very little over time. */
        template<class... Args>
        CollectionBase(std::string name, std::shared_ptr<MemoryToolBox> toolBox, LatticeParameters<T> pLatPar = LatticeParameters<T>()):
                AssignableCollectionBase<Q,Args...>(Args(name + std::to_string(I-std::get<0>(std::make_tuple(I...))),toolBox,pLatPar)...)
        {
        }

        template<class... Args>
        CollectionBase(Args... args):  //This constructor is used to create composite structure such as SU(N) matrices, from fields.
                AssignableCollectionBase<Q,Args...>(args...)
        {
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
    };

    template<class Q, typename T, CANONICALTYPE ISMOMENTUM> //Specialise to empty field collection, that does nothing. Useful to define general models for example, which may have some empty collections.
    class CollectionBase<Q, T, ISMOMENTUM> : public  AssignableCollectionBase<Q> {
    public:
        /* Put public methods here. These should change very little over time. */
        CollectionBase(std::string name, std::shared_ptr<MemoryToolBox> toolBox,
                       LatticeParameters<T> pLatPar = LatticeParameters<T>()) {}
    };

    //Now we want to be able to use it like an array; we need to have a way to unpack an integer into a list.
    //This comes from  http://spraetor.github.io/2016/01/02/template-integer-sequence.html
    //Unpack the FieldCollection<T,N> in CollectionBase<T,0,1,2,...> in Olog(N) compile time.

    template <class Q, typename T, CANONICALTYPE ISMOMENTUM, int Start, int End>
    struct CollectionHelper ;

    template <class Q, typename T, CANONICALTYPE ISMOMENTUM, int Start, int End>
    using MakeSeqImpl_t = typename CollectionHelper<Q, T, ISMOMENTUM, Start, End>::type;

    template <class Q, typename T, CANONICALTYPE ISMOMENTUM,  int N>
    using MakeSeq_t = typename CollectionHelper<Q, T, ISMOMENTUM, 0, N - 1>::type;

    template <class Q, typename T, CANONICALTYPE ISMOMENTUM,  int... I1s, int... I2s>
    struct Concat<CollectionBase<Q, T, ISMOMENTUM, I1s...>, CollectionBase<Q, T, ISMOMENTUM,I2s...>> {
        using type = CollectionBase<Q, T, ISMOMENTUM, I1s..., I2s...>;
    };

    template <class Q, typename T, CANONICALTYPE ISMOMENTUM, int Start, int End>
    struct CollectionHelper  {
        using type = Concat_t<MakeSeqImpl_t<Q,T,ISMOMENTUM, Start, (Start+End)/2>,
                MakeSeqImpl_t<Q,T,ISMOMENTUM, (Start+End)/2+1, End> >;
    };

    // break condition:
    template <class Q, typename T, CANONICALTYPE ISMOMENTUM, int I>
    struct CollectionHelper <Q,T,ISMOMENTUM,I,I> {
        using type = CollectionBase<Q,T,ISMOMENTUM, I>;
    };

    // template <typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT = 0>
    // using FieldCollection = typename CollectionHelper<T,ISMOMENTUM, SHIFT,N+SHIFT-1>::type;

    //This allows to switch between empty and non empty field collection.

    template <class Q, typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT, bool ENABLE>
    struct CollectionSelector{
        typedef CollectionBase<Q,T,ISMOMENTUM> type;
    };
    template <class Q, typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT>
    struct CollectionSelector<Q,T,ISMOMENTUM, N, SHIFT,false>{
        typedef typename CollectionHelper<Q,T,ISMOMENTUM, SHIFT, N + SHIFT - 1>::type type;
    };

    template <class Q, typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT>
    using Collection = typename CollectionSelector<Q,T,ISMOMENTUM,N,SHIFT, N == 0>::type;

    class AssignableFieldCollectionTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/assignablefieldcollection_test.h"
#endif


#endif
