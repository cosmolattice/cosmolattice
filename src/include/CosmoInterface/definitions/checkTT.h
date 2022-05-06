#ifndef TEMPLAT_COSMOINTERFACE_CHECKTT_H
#define TEMPLAT_COSMOINTERFACE_CHECKTT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "TempLat/util/constants.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/algebra.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/definitions/averages.h"
#include "TempLat/lattice/algebra/complexalgebra/asfourier.h"

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/spatialderivatives/backdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/neutdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/forwdij.h"
#include "TempLat/lattice/algebra/spatialderivatives/neutdij.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"



namespace TempLat {


    /** \brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
     *
     *
     **/

     //MakeException(WrongPSType);


      //WARNING: This class is a slow and memory expensive implementation of the transversality and tracelessness checks of GWs. It can be used by uncommenting the relevant lines in "measure.h" and "gwsmeasurer.h" files. A more optimized version will be included in the future. The output file contains the time, the three components of the transversality check (lambda) and the tracelessness check. For more info see the GW technical note in cosmolattice.net
      class CheckTT{
      public:
        template<typename T>
        CheckTT(const RunParameters<T>& par) :
        PRJType(par.GWprojectorType)
        {}
       
        template<class Model, int I, int J, typename T = double>
        inline auto PrR(Model& model, Tag<I> i, Tag<J> j) {
         
           auto toolBox = model.getToolBox();
    	   WaveNumber ntilde(toolBox);
		   size_t N = model.getToolBox()->mNGridPointsVec[0];
             
             auto keffm = MakeVector(k, 1, Model::NDim, sin(2. * Constants::pi<T> / N * ntilde(k)));
             auto keffm2 = Total(k, 1, Model::NDim, pow<2>(keffm(k)));
        
            return IfElse(IsEqual(i,j), OneType(), ZeroType()) + (keffm(i))*(keffm(j))* (-1.0/keffm2) ;

        }
        
        template<class Model, int I, int J, typename T = double>
        inline auto PrCB(Model& model, Tag<I> i, Tag<J> j) {
         
           auto toolBox = model.getToolBox();
    	   WaveNumber ntilde(toolBox);

		    size_t N = model.getToolBox()->mNGridPointsVec[0]; 
		
				auto expIK = MakeVector(k, 1, Model::NDim, complexPhase(2.0 * Constants::pi<T> / N * ntilde(k))); // e^(-2*pi*k/N)
                auto keffm = MakeVector(k, 1, Model::NDim, 1_c - expIK(k));
            
            auto keffm2 = Total(k, 1, Model::NDim, norm2(keffm(k)));
        
            return asFourier(IfElse(IsEqual(i,j), OneType(), ZeroType()) + conj(keffm(i))*(keffm(j))* (-1.0/keffm2)) ;

        }
		
		template<class Model, int I, int J, typename T = double>
        inline auto PrCF(Model& model, Tag<I> i, Tag<J> j) {
         
           auto toolBox = model.getToolBox();
    	   WaveNumber ntilde(toolBox);

		    size_t N = model.getToolBox()->mNGridPointsVec[0]; 
		
				auto expIK = MakeVector(k, 1, Model::NDim, complexPhase(-2.0 * Constants::pi<T> / N * ntilde(k))); // e^(-2*pi*k/N)
                auto keffm = MakeVector(k, 1, Model::NDim, 1_c - expIK(k));
            
            auto keffm2 = Total(k, 1, Model::NDim, norm2(keffm(k)));
        
            return asFourier(IfElse(IsEqual(i,j), OneType(), ZeroType()) + conj(keffm(i))*(keffm(j))* (-1.0/keffm2)) ;

        }
				
		template<class Model, int I, int J, int L, int M>
        inline auto LambdaR(Model& model, Tag<I> i, Tag<J> j, Tag<L> l, Tag<M> m) {
		
			return PrR(model,i,l)*PrR(model,j,m)- 0.5 * PrR(model,i,j)*(PrR(model,l,m)); 
        }
		
		template<class Model, int I, int J, int L, int M>
        inline auto LambdaC(Model& model, Tag<I> i, Tag<J> j, Tag<L> l, Tag<M> m) {
        
			if (PRJType == 2){
				return PrCB(model,i,l)*conj(PrCB(model,j,m))- 0.5 * PrCB(model,i,j)*conj(PrCB(model,l,m));
			} 
			else
			{
				return PrCF(model,i,l)*conj(PrCF(model,j,m))- 0.5 * PrCF(model,i,j)*conj(PrCF(model,l,m));
			}
        }
		
		template<class Model, int I, int J>
        inline auto LambdaUR(Model& model, Tag<I> i, Tag<J> j) {
		
			return Total(k, 1, Model::NDim, 
						Total(l, 1, Model::NDim,
								   (LambdaR(model,i,j,k,l)) * model.GWtensor(k,l).inFourierSpace()
						)); 
        }
        
        template<class Model, int I, int J>
        inline auto LambdaUC(Model& model, Tag<I> i, Tag<J> j) {
		
			return Total(k, 1, Model::NDim, 
						Total(l, 1, Model::NDim,
								   (LambdaC(model,i,j,k,l)) * model.GWtensor(k,l).inFourierSpace()
						)); 
        }
        
        template<class Model, int I, int J, typename Q = double>
        auto getProjectedFieldR(Model& model, Tag<I> i, Tag<J> j) {
            auto toolBox = model.getOneField().getToolBox();
            Field<Q> AuxFld("AuxFld", toolBox);
            
                AuxFld.inFourierSpace() = LambdaUR(model,i,j);
                AuxFld.inFourierSpace().setZeroMode(0);
            
            return AuxFld;
        }
		
		template<class Model, int I, int J, typename Q = double>
        auto getProjectedFieldC(Model& model, Tag<I> i, Tag<J> j) {
            auto toolBox = model.getOneField().getToolBox();
            Field<Q> AuxFld("AuxFld", toolBox);
            
                AuxFld.inFourierSpace() = LambdaUC(model,i,j);
                AuxFld.inFourierSpace().setZeroMode(0);
            
            return AuxFld;
        }

        
        template<typename Model, int I>
        auto checkTrans(Model& model, Tag<I> i) {
        
            double avgGrad, avgDihij;

            if(PRJType == 2){
                avgGrad = average(abs(backDiff(getProjectedFieldC(model, i, 1_c) ,1_c) +  backDiff(getProjectedFieldC(model, i, 2_c), 2_c) + backDiff(getProjectedFieldC(model, i, 3_c), 3_c)));
            	avgDihij = average(abs(forwDij(getProjectedFieldC(model, i, 1_c) ,1_c) +  forwDij(getProjectedFieldC(model, i, 2_c) ,2_c) + forwDij(getProjectedFieldC(model, i, 3_c) ,3_c)));
            }
            else if(PRJType == 3){
                avgGrad = 2 * average(abs(forwDiff(getProjectedFieldC(model, i, 1_c),1_c)+forwDiff(getProjectedFieldC(model, i, 2_c),2_c)+forwDiff(getProjectedFieldC(model, i, 3_c),3_c)));
            	 avgDihij = average(abs(forwDij(getProjectedFieldC(model, i, 1_c) ,1_c) +  forwDij(getProjectedFieldC(model, i, 2_c) ,2_c) + forwDij(getProjectedFieldC(model, i, 3_c) ,3_c)));
            }
			else{
                avgGrad = 2 * average(abs(neutDiff(getProjectedFieldR(model, i, 1_c),1_c)+neutDiff(getProjectedFieldR(model, i, 2_c),2_c)+neutDiff(getProjectedFieldR(model, i, 3_c),3_c)));
            	avgDihij = average(abs(neutDij(getProjectedFieldR(model, i, 1_c) ,1_c) +  neutDij(getProjectedFieldR(model, i, 2_c) ,2_c) + neutDij(getProjectedFieldR(model, i, 3_c) ,3_c)));
            }
            
        
        	return avgGrad/avgDihij;

		}
		
		template<typename Model>
        auto checkTrace(Model& model) {
			
			double Trhij, Trhij2;
			
			if(PRJType == 2 || PRJType == 3){
            	Trhij = average(abs(getProjectedFieldC(model, 1_c, 1_c) + getProjectedFieldC(model, 2_c, 2_c) + getProjectedFieldC(model, 3_c, 3_c)));
            	Trhij2 = average(abs(getProjectedFieldC(model, 1_c, 1_c)) + abs(getProjectedFieldC(model, 2_c, 2_c))  + abs(getProjectedFieldC(model, 3_c,3_c)));
        	}
        	else{
        		Trhij = average(abs(getProjectedFieldR(model, 1_c, 1_c) + getProjectedFieldR(model, 2_c, 2_c) + getProjectedFieldR(model, 3_c, 3_c)));
            	Trhij2 = average(abs(getProjectedFieldR(model, 1_c, 1_c)) + abs(getProjectedFieldR(model, 2_c, 2_c))  + abs(getProjectedFieldR(model, 3_c,3_c)));
        	}			
            return Trhij/Trhij2;
		}
	
	
	 private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
    
     int PRJType;
        
	};
		
    class CheckTTTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };


} 
#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/checkTT_test.h"
#endif


#endif

/* FCN */
