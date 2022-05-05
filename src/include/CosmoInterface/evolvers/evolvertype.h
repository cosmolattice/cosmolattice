#ifndef COSMOINTERFACE_EVOLVERS_EVOLVERTYPE_H
#define COSMOINTERFACE_EVOLVERS_EVOLVERTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"


namespace TempLat {


    /** \brief An enum for the different evolvers.
     *
     *
     **/

    MakeException(NotAnEvolverType);


    enum EvolverType {LF, VV2, VV4, VV6, VV8, VV10, VV6_2, RK2, RK3_4, RK3_4_A};

    std::istream& operator>>(std::istream& in, EvolverType& eType){
        std::string tmp;
        in>>tmp;
        if(tmp=="LF"||tmp=="0") eType=LF; // leapfrog
        else if(tmp=="VV2"||tmp=="1") eType=VV2;  // velocity verlet: order 2
        else if(tmp=="VV4"||tmp=="2") eType=VV4;  // ...order 4
        else if(tmp=="VV6"||tmp=="3") eType=VV6;  // ...order 6
        else if(tmp=="VV8"||tmp=="4") eType=VV8;  // ...order 8
        else if(tmp=="VV10"||tmp=="5") eType=VV10;  // ...order 10
        else if(tmp=="VV6_2"||tmp=="6") eType=VV6_2;  // alternative scheme for VV6 (see documentation)
        else if(tmp=="RK2"||tmp=="7") eType=RK2;  // RK2
        else if(tmp=="RK3_4"||tmp=="8") eType=RK3_4;  // 3rd order 4 stages low storage RK
        else if(tmp=="RK3_4_A"||tmp=="9") eType=RK3_4_A;  // 3rd order 4 stages adaptative low storage RK
        else if(tmp.empty()){} //Otherwise crash for optional parameters.
        else throw(NotAnEvolverType(tmp +" is not an evolver type, abort."));
        return in;
    };

    struct EvolverTypeTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/evolvers/evolvertype_test.h"
#endif


#endif

