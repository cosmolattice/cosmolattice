#ifndef TEMPLAT_UTIL_IDEATESTER_TEST_H
#define TEMPLAT_UTIL_IDEATESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <cstddef>
#include <tuple>
#include <utility>
#include <bits/shared_ptr.h>
#include "TempLat/util/rangeiteration/tagliteral.h"



struct Mother{
    static constexpr int a = 3;
    static constexpr int b = 3;
};

struct Daughter : public Mother{
    static constexpr int a = 5;
};

inline void TempLat::IdeaTester::Test(TempLat::TDDAssertion& tdd) {




    say << Daughter::a;
    say << Mother::a;
    say << Daughter::b;

/*
    using namespace HighFive;
    // Create a new file using the default property lists.
    int mpi_rank, mpi_size;

    // initialize MPI
   // MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    using namespace HighFive;


        // open a new file with the MPI IO driver for parallel Read/Write
        File file("mytest.h5", File::ReadWrite | File::Create | File::Truncate,
                  MPIOFileDriver(MPI_COMM_WORLD, MPI_INFO_NULL));

    DataSetCreateProps props;
    props.add(Chunking(std::vector<hsize_t>{1, 3}));
        // Create the dataset. Look at extensible one!

    DataSpace dataspace = DataSpace({1, 3}, {DataSpace::UNLIMITED, 3});

    Group group = file.createGroup("mother");
    DataSet dataset = group.createDataSet<double>("2",  dataspace, props);
    dataset.select({0, 0}, {1, 3}).write(std::array<int,3>{12, 13, 14});
    auto currDim = dataset.getDimensions();
    dataset.resize({currDim[0]+1,currDim[1]});
    dataset.select({1, 2}, {1, 1}).write(11);*/


}

#endif
