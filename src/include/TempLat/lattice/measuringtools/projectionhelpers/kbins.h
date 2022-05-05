#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_HALKKBINS_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_HALKKBINS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult.h"

namespace TempLat {



    /**
     *  Simple convenience class which maps any bining to one integer ks. Useful for test purposes.
     *
     *
     * Unit test: make test-kbins
     **/
    template<typename T>
    class KBins {
    public:
        /* Put public methods here. These should change very little over time. */
        KBins(const ptrdiff_t& nGrid, const RadialProjectionResult<T>& res, const T& pKir):
        ks((int)(std::pow(3,0.5)/2.0*(T)nGrid)),
        vs((int)(std::pow(3,0.5)/2.0*(T)nGrid)),
        ms((int)(std::pow(3,0.5)/2.0*(T)nGrid)),
        kir(pKir)
         {
          size_t ind=0;
          for(auto&& it : res)
          {
          if(!AlmostEqual(it.getBin().average,0)){
              ind=std::min(std::round((it.getBin().average))-1,ks.size()-1.0);
            //  say<<ind<<"\n";
              vs[ind]+= it.getValue().average * it.getValue().multiplicity;
              ms[ind]+= it.getValue().multiplicity;
            }
          }
          for(size_t i=0; i<ks.size();++i)
          {
            ks[i] = i+1;
            vs[i] /= ms[i];
          }
        }

        friend
        std::ostream& operator<< (std::ostream& ostream, const KBins& rpr) {
            //ostream << "#ks values multiplicities" << "\n";
            for(size_t i=0; i<rpr.ks.size();++i){
              ostream<<rpr.ks[i]*rpr.kir<<" "<<rpr.vs[i]<<" "<<rpr.ms[i]<<"\n";
            }
            //ostream <<"\n";
            return ostream;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::vector<T> ks; //ks
        std::vector<T> vs; //spectrum or whatever is binned
        std::vector<T> ms; //multiplicities, to check
        T kir;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/projectionhelpers/kbins_test.h"
#endif


#endif
