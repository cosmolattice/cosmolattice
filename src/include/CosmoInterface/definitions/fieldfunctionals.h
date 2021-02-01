#ifndef COSMOINTERFACE_HELPERS_COMPOSITEFIELDS_H
#define COSMOINTERFACE_HELPERS_COMPOSITEFIELDS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/algebra/operators/operators.h"
#include "CosmoInterface/definitions/gaugederivatives.h"
#include "TempLat/lattice/algebra/gaugealgebra/forwardcovariantderivative.h"
#include "TempLat/lattice/algebra/su2algebra/su2multiply.h"
#include "TempLat/lattice/algebra/gaugealgebra/fieldstrength.h"
#include "TempLat/lattice/algebra/gaugealgebra/plaquette.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare.h"

namespace TempLat {


    /** \brief A class which stores expressions of field functionals.
     *
     * 
     **/

    class FieldFunctionals {
    public:
        /* Put public methods here. These should change very little over time. */

        FieldFunctionals() = delete;
        
        // By "field functional" we refer to any function of the field variables such as quadratic forms, e.g. <phi^2>, and others.
                
        // The following functions compute the field functionals that appear in the total energy density,
        // but without the corresponding multiplying factors. For example, here we compute <Pi^2> for a scalar singlet, 
        // while in energies.h we multiply it by 1/2*a^(-6) to get the corresponding kinetic energy.

		// --> Scalar singlet:
		
        template<class Model, int I>  // <Grad[f]^2>
        static inline auto grad2S(Model& model, Tag<I> i)
        {
            return Grad2<Model::NDim>(model.fldS(i));
        }

        template<class Model, int I>  // <pi^2>
        static inline auto pi2S(Model& model, Tag<I> i)
        {
            return pow<2>(model.piS(i));
        }

		// --> Complex scalar:
		
        template <class Model, int I>  // <D_i[f]^2> (sum over i)
        static inline auto grad2CS(Model& model, Tag<I> i)
        {
            return Total(j, 1, Model::NDim, norm2( GaugeDerivatives::forwardCovGradientCS(model,i,j)));
        }

        template <class Model, int I>  // <pi^2>
        static inline auto pi2CS(Model& model, Tag<I> i)
        {
            return norm2(model.piCS(i));
        }

		// --> SU2 doublet:
		
        template <class Model, int I>   // <D_i[f]^2> (sum over i)
        static inline auto grad2SU2Doublet(Model& model, Tag<I> i)
        {

            return Total(j, 1, Model::NDim, norm2( GaugeDerivatives::forwardCovGradientSU2Doublet(model,i,j)));
        }

        template <class Model, int I>   // <pi^2>
        static inline auto pi2SU2Doublet(Model& model,Tag<I> i)
        {
            return norm2(model.piSU2Doublet(i));
        }

		// --> U1 gauge sector:

        template <class Model, int A>
        static inline auto B2U1(Model& model, Tag<A> a)  // In 3D, returns F_{21}^2 + F_{31}^2 + F_{32}^2 (necessary to compute the magnetic energy) 
        {
            return   Total(i,1,Model::NDim,
                                        Total(j,1,Model::NDim,
                                              IfElse(IsLess(j,i),
                                                     pow<2>(fieldStrength(model.fldU1(a),i,j)), ZeroType());
                                        ));
        }
        
        template <class Model, int A>    // <pi^2>
        static inline auto pi2U1(Model& model, Tag<A> a)
        {
            return  Total(i,1,Model::NDim,
                                       pow<2>(model.piU1(a)(i))
            );
        }
        
        // --> SU2 gauge sector:

        template <class Model,int A>
        static inline auto B2SU2(Model& model, Tag<A> a)
        {
            return 4.0 / (pow<4>(model.dx) * pow<2>(model.gQ_SU2DblSU2(0_c, a)))*  Total(i,1,Model::NDim,
                                               Total(j,1,Model::NDim,
                                                       IfElse(IsLess(j,i),
                                                        2.0 - trace(plaq(model.fldSU2(a),i,j)), //if
                                                       ZeroType() //else
                                                            );
                                                     )
                                               );
        }
        template <class Model, int A> // <pi^2>
        static inline auto pi2SU2(Model& model, Tag<A> a)
        {

            return Total(i,1,Model::NDim,
                         Total(b,1,3,
                                    pow<2>(model.piSU2(a)(i).SU2LieAlgebraGet(b))
                                )
                     );
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/definitions/fieldfunctionals_test.h"
#endif


#endif
