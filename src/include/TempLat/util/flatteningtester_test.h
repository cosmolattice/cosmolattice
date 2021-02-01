#ifndef TEMPLAT_UTIL_FLATTENINGTESTER_TEST_H
#define TEMPLAT_UTIL_FLATTENINGTESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

struct dummy1{

    template <int N>
    auto SU2Get(TempLat::Tag<N> t)
    {
        return N;
    }

    static constexpr size_t size = 4;
    using Getter = TempLat::SU2Getter;

};

struct dummy2{

    template <int N>
    auto getComp(TempLat::Tag<N> t)
    {
        return dummy1();
    }
    static constexpr size_t size = 3;
    using Getter = TempLat::GetComponent;


};

struct dummy3{

    template <int N>
    auto getComp(TempLat::Tag<N> t)
    {
        return dummy2();
    }
    static constexpr size_t size = 5;
    using Getter = TempLat::GetComponent;

};


inline void TempLat::FlatteningTester::Test(TempLat::TDDAssertion& tdd) {

    auto dum = dummy3();

    tdd.verify(IsComposite<dummy1>::value == false);
    tdd.verify(IsComposite<dummy2>::value == true);
    tdd.verify(IsComposite<dummy3>::value == true);

    auto testmake = make_tuple_from(dum);

    tdd.verify(TempLat::tuple_size<decltype(testmake)>::value==5);
    tdd.verify(TempLat::tuple_size<decltype(flatten_tuple(testmake))>::value == 60);

}

#endif
