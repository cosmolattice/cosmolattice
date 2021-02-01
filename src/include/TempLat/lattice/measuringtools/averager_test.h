#ifndef TEMPLAT_LATTICE_MEASUREMENTS_AVERAGER_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_AVERAGER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"

inline void TempLat::AveragerTester::Test(TempLat::TDDAssertion& tdd) {

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
        myTmpStruct() : mt(MemoryToolBox::makeShared(4, 16, 1)) { }
        double get(ptrdiff_t i) {
            return 1.;
        }
        auto getToolBox() { return mt; }
        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
        }
        std::shared_ptr<MemoryToolBox> mt;
        std::string toString() const { return "myTmpStruct"; }
    };

    struct myTmpStructComplex {
        myTmpStructComplex(ptrdiff_t nd, ptrdiff_t ngr) : mt(MemoryToolBox::makeShared(nd, ngr, 1)) { }
        std::complex<double> get(const ptrdiff_t& i) {
            mt->itP()() = i;
            double imagPart = mt->mLayouts.getFourierSpaceLayout().getHermitianPartners()->qualify(mt->itP().getVec()) == HermitianRedundancy::realValued ? 0. : 1.;
            return std::complex<double>(1., imagPart);
        }
        auto getToolBox() { return mt; }
        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
        }
        std::shared_ptr<MemoryToolBox> mt;
    };

    myTmpStruct myInstance;

    auto aget = getAverager(myInstance);

    say << "result of " << aget << ": " << aget.compute() << "\n";

    tdd.verify( AlmostEqual(aget.compute(), 1.));

    auto&& myLambda = [&tdd](ptrdiff_t nd_, ptrdiff_t ngr_ ) {
        myTmpStructComplex myInstanceCp(nd_, ngr_);

        auto agetCp = getAverager(myInstanceCp);

        auto result = agetCp.compute();

        say << result << " vs hypothetical " << std::complex<double>(1., 1.) << "\n";

        tdd.verify( AlmostEqual(result, std::complex<double>(1., 1.)) );
    };

    myLambda(4, 16);
    myLambda(3, 32);
    myLambda(2, 64);
#ifdef NOMPI
    /** this one fails correctly under MPI: each process would do the full 1d rod. */
    myLambda(1, 12);
#endif
}

#endif
