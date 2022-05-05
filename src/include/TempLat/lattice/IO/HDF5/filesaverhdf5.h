#ifndef TEMPLAT_LATTICE_IO_HDF5_FILESAVERHDF5_H
#define TEMPLAT_LATTICE_IO_HDF5_FILESAVERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HDF5

#include "TempLat/util/prettytostring.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/parameters/parameterparser.h"

#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"

namespace TempLat {


    /** \brief A class which implements saving in pure HDF5.
     *
     *
     * Unit test: make test-filesaverhdf5
     **/

    MakeException(StringIsTooLong);

    class FileSaverHDF5 {
    public:
        /* Put public methods here. These should change very little over time. */
        FileSaverHDF5() {

        }

        void open(std::string fn) {
            mFile.open(fn);
        }

        void create(std::string fn) {
            mFile.create(fn);
        }

        void close()
        {
            mFile.close();
        }
        void reset()
        {
            this->close();
        }

        void save(ParameterParser& r){ //Conceptually, may be better as attributes? But nightmare to save vector of strings, did nt manage to do it in a finite amount of time.
            std::ostringstream oss;

            std::vector<std::string> parStr;
            std::string tmp;
            for(auto x : r.getParams() ){
                if(x.second != "inf") {
                    parStr.emplace_back(x.first + "=" + x.second);
                }
            }
            mDataset = mFile.createDataset<const char*>("Parameters", std::vector<hsize_t>(1,parStr.size()));
            for(size_t i = 0; i < parStr.size(); ++i){
                if(parStr[i].size() > HDF5TypeConstant::FixedSizeStringLength) throw StringIsTooLong("Well, that's a bit embarassing. One of your parameters contains too many characters (the total string should be smaller than "+std::to_string(HDF5TypeConstant::FixedSizeStringLength)+" char by default, for our hdf5). If you managed to make HDF5 with variable string length, please let us know! If you just want to change the hardcoded number, look in the file TempLat/lattice/IO/HDF5/helpers/hdf5type.h .");
                mDataset.writeElement(parStr[i].c_str(),std::vector<hsize_t>(1,i));
            }
            mDataset.close();
        }

        template<typename R>
        void save(R r){ //used to store an entity directly to a dataset, using it's own name.
            typedef typename GetGetReturnType<R>::type vType;
            ConfirmSpace::apply(r,r.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);
            GhostsHunter::apply(r);
            mDataset = mFile.createDataset<vType>(GetString::get(r), r.getToolBox()->mNGridPointsVec);
            saveDim(r,0,{});
            mDataset.close();
        }

        template<typename R, typename T>
        void save(T t, R r, std::string name){ //used to store an entity in a time series. The name is the one of the group, data set labelled by t.
            typedef typename GetGetReturnType<R>::type vType;
            ConfirmSpace::apply(r,r.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);
            GhostsHunter::apply(r);
            mDataset = mFile.createOrOpenGroup(name).createDataset<vType>(PrettyToString::get(t,10), r.getToolBox()->mNGridPointsVec);
            saveDim(r,0,{});
            mDataset.close();
        }


        template<typename R>
        void save(R t, std::string name){ //used to store a number. The name is the one of the dataset which contains this number.
            typedef typename GetGetReturnType<R>::type vType;
            mDataset = mFile.createDataset<vType>(name, std::vector<hsize_t>(1,1));
            mDataset.writeElement(&t, std::vector<hsize_t>(1,0));
            mDataset.close();
        }

        //To save our fields, we use the fact that the last dimension is not parallelised.
        //We iterate over the first N-1 dimensions, and for each of these we save the whole
        //last dimension to file.
        template<typename R>
        void saveDim(R r, int dim,  std::vector<ptrdiff_t> coords)
        {
            auto toolBox = r.getToolBox();

            auto itX = toolBox->itX();

            auto starts =  toolBox->mLayouts.getConfigSpaceStarts(); //Local mpi offset.
            auto sizes =  toolBox->mLayouts.getConfigSpaceSizes(); //Local mpi sizes.

            if(dim == toolBox->mNDimensions-1 ) //Last dimension, saved as a full rod.
            {
                coords.emplace_back(0); //look at index 0 in the last dimension. The next nGrid[last dimension] points are stored continuously.
                ptrdiff_t offset = r.getJumps().getTotalOffsetFromSpatialCoordinates(coords);

                std::vector<hsize_t> subdims(toolBox->mNDimensions,1); //for hdf5, tell it we want to store a sub array of size (1,1,1...,nGrid[last dimension]).
                subdims.back() = toolBox-> mNGridPointsVec[dim];
                std::vector<hsize_t> offsets; // at position (i,j,k,...,0) in the global lattice file.
                for(size_t i = 0; i < coords.size() ; ++i) offsets.emplace_back( coords[i]);
                offsets.back() = 0;

                typedef typename GetGetReturnType<R>::type vType;

                std::vector<vType> sdata(toolBox->mNGridPointsVec[dim]);
                for(size_t i = 0; i< sdata.size() ; ++i)
                {
                    sdata[i] = r.get(offset + i);
                }

                mDataset.writeSlices(sdata, subdims, offsets);
            }
            else{
                for(int i = 0; i < sizes[dim] ;++i){  //Recursive call to loop over an arbitrary number of dimensions.
                    std::vector<ptrdiff_t> newCoords(coords);
                    newCoords.emplace_back(starts[dim] + i);
                    saveDim(r, dim + 1, newCoords);
                }
            }
        }



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        HDF5File mFile;
        HDF5Dataset mDataset;

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/filesaverhdf5_test.h"
#endif

#endif //HDF5

#endif
