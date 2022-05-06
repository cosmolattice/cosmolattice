#ifndef TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2021

#include "TempLat/util/almostequal.h"

inline void TempLat::MaximumTester::Test(TempLat::TDDAssertion& tdd) {

    struct myWorkspace {
        myWorkspace() : value(0.) { }
        myWorkspace& operator+=(const myWorkspace &other) {
            value += other.value;
            return *this;
        }
        myWorkspace& operator+=(const double &other) {
            value += other;
            return *this;
        }
        std::string toString() const { return "myWorkspace"; }
        double value;
    };

    struct myTmpStruct {
        myTmpStruct() : mt(MemoryToolBox::makeShared(3, 4, 0)) { }
        double get(ptrdiff_t i) {
            return i > 40 ? 0 : i;
        }
        auto getToolBox() { return mt; }
        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
        }
        std::shared_ptr<MemoryToolBox> mt;
        std::string toString() const { return "myTmpStruct"; }
    };


    myTmpStruct myInstance;

    auto aget = max(myInstance);

    say << "result of max : " << aget << "\n";

    //tdd.verify( aget == 40);


}

#endif
