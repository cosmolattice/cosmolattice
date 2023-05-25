#ifndef COSMOINTERFACE_HELPERS_PITENSOR_H
#define COSMOINTERFACE_HELPERS_PITENSOR_H
/* File created by Jorge Baeza, Adrien Florio and Nicolas Loayza 2021 */
/* Released under the MIT license, see LICENSE.md. */

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
            return scalarSinglet(model, 1_c, 1_c) + complexscalarSinglet(model, 1_c, 1_c) - (pow<2>(model.omegaStar)/pow<2>(model.fStar)) * (pow(model.aI, -2.0 * model.alpha) * electricFieldU1(model, 1_c, 1_c) + pow(model.aI, -2.0) * magneticFieldU1(model, 0_c));
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<1>)
        {		
            return scalarSinglet(model, 1_c, 2_c) + complexscalarSinglet(model, 1_c, 2_c) - (pow<2>(model.omegaStar)/pow<2>(model.fStar)) * (pow(model.aI, -2.0 * model.alpha) * electricFieldU1(model, 1_c, 2_c) + pow(model.aI, -2.0) * magneticFieldU1(model, 1_c));
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<2>)
        {	
            return scalarSinglet(model, 1_c, 3_c) + complexscalarSinglet(model, 1_c, 3_c) - (pow<2>(model.omegaStar)/pow<2>(model.fStar)) * (pow(model.aI, -2.0 * model.alpha) * electricFieldU1(model, 1_c, 3_c) + pow(model.aI, -2.0) * magneticFieldU1(model, 2_c));
        }
       template<class Model>
        static inline auto totalTensor(Model& model, Tag<3>)
        {	
            return scalarSinglet(model, 2_c, 2_c) + complexscalarSinglet(model, 2_c, 2_c) - (pow<2>(model.omegaStar)/pow<2>(model.fStar)) * (pow(model.aI, -2.0 * model.alpha) * electricFieldU1(model, 2_c, 2_c) + pow(model.aI, -2.0) * magneticFieldU1(model, 3_c));
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<4>)
        {	
            return scalarSinglet(model, 2_c, 3_c) + complexscalarSinglet(model, 2_c, 3_c) - (pow<2>(model.omegaStar)/pow<2>(model.fStar)) * (pow(model.aI, -2.0 * model.alpha) * electricFieldU1(model, 2_c, 3_c) + pow(model.aI, -2.0) * magneticFieldU1(model, 4_c));
        }
        template<class Model>
        static inline auto totalTensor(Model& model, Tag<5>)
        {	
            return scalarSinglet(model, 3_c, 3_c) + complexscalarSinglet(model, 3_c, 3_c) - (pow<2>(model.omegaStar)/pow<2>(model.fStar)) * (pow(model.aI, -2.0 * model.alpha) * electricFieldU1(model, 3_c, 3_c) + pow(model.aI, -2.0) * magneticFieldU1(model, 5_c));
        }
        
        /* Complex Scalars Tensor */
        

		
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
		
		template<class Model, int I,int J>
        static inline auto complexscalarSinglet(Model& model, Tag<I> a, Tag<J> b)
        {
            return Total(i, 0, Model::NCs - 1, 2. * Real(conj(GaugeDerivatives::forwardCovGradientCS(model, i, a)) * GaugeDerivatives::forwardCovGradientCS(model, i, b)));
        }
		
		template<class Model, int I,int J>
        static inline auto electricFieldU1(Model& model, Tag<I> a, Tag<J> b)
        {
            return Total(i, 0, Model::NU1 - 1, pow(model.aI,2*(model.alpha - 1)) * model.piU1(i)(a) * model.piU1(i)(b) );
        }
        
        template<class Model>
        static inline auto magneticFieldU1(Model& model, Tag <0>)
        {
            return Total(i, 0, Model::NU1 - 1, pow<2>(fieldStrength(model.fldU1(i), 2_c , 3_c)));
        }

		template<class Model>
        static inline auto magneticFieldU1(Model& model, Tag <1>)
        {
            return Total(i, 0, Model::NU1 - 1, fieldStrength(model.fldU1(i), 2_c , 3_c) * fieldStrength(model.fldU1(i), 3_c , 1_c));
        }
		
		template<class Model>
        static inline auto magneticFieldU1(Model& model, Tag <2>)
        {
            return Total(i, 0, Model::NU1 - 1, fieldStrength(model.fldU1(i), 2_c , 3_c) * fieldStrength(model.fldU1(i), 1_c , 2_c));
        }
        
        template<class Model>
        static inline auto magneticFieldU1(Model& model, Tag <3>)
        {
            return Total(i, 0, Model::NU1 - 1, pow<2>(fieldStrength(model.fldU1(i), 3_c , 1_c)));
        }
        
        template<class Model>
        static inline auto magneticFieldU1(Model& model, Tag <4>)
        {
            return Total(i, 0, Model::NU1 - 1, fieldStrength(model.fldU1(i), 3_c , 1_c) * fieldStrength(model.fldU1(i), 1_c , 2_c));
        }
        
        template<class Model>
        static inline auto magneticFieldU1(Model& model, Tag <5>)
        {
            return Total(i, 0, Model::NU1 - 1,  pow<2>(fieldStrength(model.fldU1(i), 1_c , 2_c)));
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