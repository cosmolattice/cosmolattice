#ifndef COSMOINTERFACE_HELPERS_PITENSOR_H
#define COSMOINTERFACE_HELPERS_PITENSOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "CosmoInterface/definitions/gaugederivatives.h"
#include "TempLat/lattice/algebra/gaugealgebra/forwardcovariantderivative.h"
#include "TempLat/lattice/algebra/su2algebra/su2multiply.h"
#include "TempLat/lattice/algebra/gaugealgebra/fieldstrength.h"
#include "TempLat/lattice/algebra/gaugealgebra/plaquette.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare.h"
#include "TempLat/lattice/algebra/operators/operators.h"

namespace TempLat {

    /** \brief A class which computes the componentes of the Anisotropic Tensor source of Gravitational Waves.
     *
     * 
     * Unit test: make test-PITensor
     **/

    class PITensor {
    public:
        /* Put public methods here. These should change very little over time. */
        PITensor() = delete;
   

    public:

		template<class Model>
        static inline auto totalTensor(Model& model, Tag<0>)
        {		
            return scalarSinglet(model, 1_c, 1_c);
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<1>)
        {		
            return scalarSinglet(model, 1_c, 2_c);
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<2>)
        {	
            return scalarSinglet(model, 1_c, 3_c);
        }
       template<class Model>
        static inline auto totalTensor(Model& model, Tag<3>)
        {	
            return scalarSinglet(model, 2_c, 2_c);
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<4>)
        {	
            return scalarSinglet(model, 2_c, 3_c);
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<5>)
        {	
            return scalarSinglet(model, 3_c, 3_c);
        }
        
        /* Complexa Scalars Tensor */
        

		
//         template<class Model>
//         static auto PITensorScalarsArray(Model& model)
//         {
//             return IfElse((Model::Ngws>0),MakeArray(i,0,Model::Ngws-1, PITensorScalars(model,i)), ZeroType());
//         }
        
    private:
        template<class Model, int I,int J>
        static inline auto scalarSinglet(Model& model, Tag<I> a, Tag<J> b)
        {
            return Total(i, 0, Model::Ns - 1, forwDiff(model.fldS(i),a) * forwDiff(model.fldS(i),b));
        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* FCN */

#ifdef TEMPLATTEST
#include "CosmoInterface/definitions/PITensor_test.h"
#endif


#endif
