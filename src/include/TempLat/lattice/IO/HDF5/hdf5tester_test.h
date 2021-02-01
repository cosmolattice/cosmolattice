#ifndef TEMPLAT_LATTICE_IO_HDF5_HDF5TESTER_TEST_H
#define TEMPLAT_LATTICE_IO_HDF5_HDF5TESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/IO/HDF5/filesaverhdf5.h"
#include "TempLat/lattice/IO/HDF5/fileloaderhdf5.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/almostequal.h"

inline void TempLat::HDF5Tester::Test(TempLat::TDDAssertion& tdd) {

   /* FileSaverHDF5 fs;
    FileLoaderHDF5 fl;
    ptrdiff_t nGrid = 8, nGhost = 1;

    auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);


    Field<double> phi("phi", toolBox);
    phi = 1;
    class myStruct{
    public:
        myStruct(std::shared_ptr<MemoryToolBox> pToolBox):
                mToolBox(pToolBox)
        {}

        double get(ptrdiff_t i)
        {
            auto tmp = mToolBox -> getCoordConfiguration(i);
            return tmp[0];
        }
        std::string toString() const
        {
            return "myClass(x)";
        }

        std::shared_ptr<MemoryToolBox> getToolBox()
        {
            return mToolBox;
        }
        auto getJumps()
        {
            return mToolBox->mLayouts.getConfigSpaceJumps();
        }


        std::shared_ptr<MemoryToolBox> mToolBox;
    };


    toolBox->setVerbose();

    char t1[256] ("aaa=bbb"), t2[256]("  aa = 25.78"), t3[256]("cc=true"), t4[2056]("d= 45 76 45 745 76 143 24 45 76 143 2443 45 76 3 245 76 1 76 143 24 45 76 143 24 45 76 143 244345 76 3 245 76 1 76 143 24 45 76 143 24 45 76 143 244345 76 3 245 76 1 76 143 24 45 76 143 24 45 76 143 2443"), t5[256]("inpt=../../src/tests/TempLat/parameters/test_filereader.txt");
    char* argv[6];
    argv[0] = t1;
    argv[1] = t1;
    argv[2] = t2;
    argv[3] = t3;
    argv[4] = t4;
    argv[5] = t5;
    ParameterParser test(6, argv);
    ParameterParser readTest(1, argv);




    myStruct ms(toolBox);

    fs.open("FILE.h5");
    fs.save(ms);
    fs.save(0.45,"aDot");
    fs.save(test);
    fs.close();


    Field<double> psi("myClass", toolBox);


    say << "hi" << readTest;


    double aDot;
    fl.open("FILE.h5");
    fl.load(psi);
    fl.load(aDot,"aDot");
    fl.load(readTest);
    fl.close();

    tdd.verify(AlmostEqual(aDot, 0.45));

    say << readTest;


    auto itX = toolBox->itX();

    for(itX.begin(); itX.end(); ++itX)
    {
        auto coords = itX.getVec(itX());
     //   say << coords[0]  << " " << coords[1]  << " " << coords[2]  << " " << psi.get(itX());
    }

    fs.open("FILE2.h5");
    fs.save(0.45,ms,"class");
    fs.save(test);
    fs.close();


    tdd.verify( true );*/

}

#endif
