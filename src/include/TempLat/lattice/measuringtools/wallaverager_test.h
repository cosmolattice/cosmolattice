#ifndef TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2025

inline void TempLat::WallAveragerTester::Test(TempLat::TDDAssertion& tdd) {

   /* 
    struct myTmpStruct {
        myTmpStruct() : mt(MemoryToolBox::makeShared(4, 16, 1)) { }
        double get(ptrdiff_t i) {

            auto ii = mt->getCoordConfiguration0N(i);

            return ii[0] * pow(16,3) + ii[1]* pow(16,2) + ii[2] * 16 + ii[3];
           //return 1;
        }

        double expectedAnswer(int l)
        {
            return (0.5 * (15) * 16 * ( pow(16,2) +  16 +  1))  + l; // sum_0^N-1 = (N-1)N/2
        }

        auto getToolBox() { return mt; }
        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
        }
        std::shared_ptr<MemoryToolBox> mt;
        std::string toString() const { return "myTmpStruct"; }
    };


    myTmpStruct myInstance;

    auto avs = wallAverager(myInstance);
    avs.compute();
    auto wallt = avs.getWall(3);


    for(int i = 0; i<15; ++i) {
        say << wallt[i];
    }*/

    auto mt = MemoryToolBox::makeShared(3, 4, 1);

    ptrdiff_t i = 0;
    auto& it = mt->itX();

    for(it.begin();it.end();++it)
    {
        i = it();
        auto coord = mt->getCoordConfiguration(i);
        say << coord;
    }

}

#endif
