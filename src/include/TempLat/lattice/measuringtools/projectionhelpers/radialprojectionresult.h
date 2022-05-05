#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONRESULT_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONRESULT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsinglebinandvalue.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsinglequantity.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionrebinner.h"

namespace TempLat {

    MakeException(RadialProjectionResultSizeException);
    MakeException(RadialProjectionResultFinalizationException);

    /** \brief A class which holds the result of a radial projection, with per bin
     *  the average position of each entry (the horizontal axis in your plot..),
     *  the variance in the position,
     *  the average value,
     *  the sample variance (= <f^2> - <f>^2 ) of the values,
     *  and the number of entries in a bin (multiplicity).
     *
     *  Upon call to finalize, constructs the parent vector, which you can use as you wish.
     *
     *  See `RadialProjectionSingleDatum` for the single data points in the vector.;
     *
     *  Also serves as the workspace during the integration, can to the
     *  data transposition after the integration is done.
     *
     *
     * Unit test: make test-radialprojectionresult
     **/

    template <typename T = double>
    class RadialProjectionResult : public std::vector<RadialProjectionSingleBinAndValue<T>> {
    public:

        typedef typename GetFloatType<T>::type floatType;

        /* Put public methods here. These should change very little over time. */
        RadialProjectionResult(ptrdiff_t nBins, bool pUseBinCentralValues = false, bool pIsInFourier = false) :
        std::vector<RadialProjectionSingleBinAndValue<T>>(),
        finalizedOnce(false),
        mNBins(nBins),
        mValues(mNBins),
        mBinBounds(mNBins),
        mMultiplicities(mNBins, 0),
        unset(false),
        mUseBinCentralValues(pUseBinCentralValues),
        mIsInFourier(pIsInFourier)
        {

        }



        /** \brief operator+= is a requirement for use as a workspace in FieldlessGetteration. */
        RadialProjectionResult<T> operator+=(const RadialProjectionResult& other) {
            if(this->mNBins != other.mNBins) throw RadialProjectionResultSizeException("RadialProjectionResults are of different size! Cannot add things of different size... Abort.");
            if(unset){
                for(size_t i = 0; i < this->size(); ++i){
                    (*this)[i] = RadialProjectionSingleBinAndValue<T>(other[i].getBin(), other[i].getValue());
                }
                unset = false;
            }
            for(size_t i = 0; i < this->size(); ++i){
                (*this)[i].getValue().average += other[i].getValue().average;
            }
            return *this;
        }

        /** \brief Decrease the number of bins on demand. */
        RadialProjectionResult& rebin(ptrdiff_t nBins, T customRange = -1) {
            std::vector<RadialProjectionSingleBinAndValue<T>>::operator=(
                                                                         RadialProjectionRebinner<T>::rebin(
                                                                                                            *this,
                                                                                                            nBins,
                                                                                                            centralBinBounds,
                                                                                                            customRange
                                                                                                            )
            );
            return *this;
        }


        /** \brief Rescale the results with a function of x or k (bin location),
         *  using for now a simple lambda function of single float parameter, which
         *  is your x in f(x). In other words, you give f(x).
         */
        template <typename LL>
        RadialProjectionResult& rescale(LL rescaler) {
            //for (auto&& it : *this) {
            for (size_t i = 0; i< this->size(); ++i)
            {
                if(mUseBinCentralValues) (*this)[i].getValue() *= rescaler( this->centralBinBounds[i] );
                else (*this)[i].getValue() *= rescaler( (*this)[i].getBin().average );
            }
            return *this;
        }

        auto energy(int PSVersion) {
            auto energy = 0.;
            auto kIR = (*this).getCentralBinBounds()[0];
            switch (PSVersion){
                case 1:
                    for (size_t i = 0; i < this->size(); ++i) {
                        energy +=  (*this)[i].getValue().average / (i + 1) ;
                    }
                    break;
                case 2:
                case 3:
                    for (auto&& it : *this) {
                        energy +=  it.getValue().average * kIR / it.getBin().average ;
                    }
                    break;
                default:
                    energy = -1.;
            }
            return energy;
        }


        /** \brief Rescale the bin positions with a normalization (for example dimensionful).
         */
        RadialProjectionResult& rescaleBins(T scale) {
            for (auto&& it : *this) {
                it.getBin() *= scale;
            }
            for(T& b : centralBinBounds) {
                 b *= scale;
            }
            return *this;
        }

        RadialProjectionResult& sumInsteadOfAverage() {
          floatType intMultiplicity = 0;

            /*for (auto&& it : *this) {
              intMultiplicity = 2 * it.getBin().multiplicity; // *2 is to get the full number of modes, against original design
              it.getValue() *= intMultiplicity;
              //it.getValue().sumInsteadOfAverage();
            }*/
            for (size_t i = 0; i< this->size(); ++i)
            {
              intMultiplicity = (*this)[i].getBin().multiplicity * (mIsInFourier ? 2 : 1); // Multiply by 2 if in Fourier space (only half of the last coordinate is iterated over
                                                                                          // in this case because of reflection symmetry for real data).
              (*this)[i].getValue() *= intMultiplicity;
            }
            return *this;
        }


        std::string toString(int verbosity = 0) const {
            if ( (ptrdiff_t) this->size() < 1 ) return "";
            std::stringstream sstream;
            sstream << this->front().getHeader(verbosity) << "\n";
            for ( auto&& it : *this) {
                sstream << it.toString(verbosity) << "\n";
            }
            return sstream.str();
        }



        friend
        std::ostream& operator<< (std::ostream& ostream, const RadialProjectionResult& rpr) {
            ostream << rpr.toString() << "\n";
            return ostream;
        }

        std::vector<T>& getCentralBinBounds()
        {
          return centralBinBounds;
        }

        template <typename S>
        friend class RadialProjector;

        template <typename S>
        friend class RadialProjectorGW;

        template <typename S>
        friend RadialProjectionResult<S> operator+(const RadialProjectionResult<S>& a, const RadialProjectionResult<S>& b);

            private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        bool finalizedOnce;
        ptrdiff_t mNBins;
        RadialProjectionSingleQuantity<T> mValues, mBinBounds;
        std::vector<T> centralBinBounds; // Naive central values of the bin. Does not need to be set.

        std::vector<floatType> mMultiplicities;

        bool unset;
        bool mUseBinCentralValues;
        bool mIsInFourier;


        void add(ptrdiff_t i, const T& value, const T& position, const T& weight = (T) 1) {
            mValues.add(i, value, weight);
            mBinBounds.add(i, position, weight);
            mMultiplicities[i] += weight;
        }

        /** \brief RadialProjector calls this as the last step, does the transposition of the result vecotrs into one vector of RadialProjectionSingleBinAndValue<T>. */
        RadialProjectionResult& finalize(MPICommReference comm) {

            if ( finalizedOnce ) throw RadialProjectionResultFinalizationException("Can only finalize once per instance.");

            finalizedOnce = true;

            comm.Allreduce(&mMultiplicities, MPI_SUM);

            mValues.finalize(comm);
            mBinBounds.finalize(comm);

            auto& mFullResult = *this;

            mFullResult.clear();
            for ( ptrdiff_t i = 0; i < mNBins; ++i) {
                if ( mMultiplicities[i] > 0 ) {

                    RadialProjectionSingleBinAndValue<T> next(
                        mBinBounds.getFinal(i, mMultiplicities[i]),
                        mValues.getFinal(i, mMultiplicities[i])
                    );
                    this->push_back(next);
                }
            }
            mMultiplicities.clear();
            mValues.clear();
            mBinBounds.clear();
            return *this;
        }


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



  template<typename T>
  RadialProjectionResult<T> operator+(const RadialProjectionResult<T>& a, const RadialProjectionResult<T>& b) {
      RadialProjectionResult<T> res(a);

      for(size_t i = 0; i < a.size(); ++i){
          res[i].getValue().average += b[i].getValue().average;
      }
      return res;
  }

    template<typename T, class R>
    RadialProjectionResult<T> operator*(R&& func, const RadialProjectionResult<T>& obj)
    {
        RadialProjectionResult<T> res(obj);
        return res.rescale(func);
    }

    template<typename T>
    RadialProjectionResult<T> operator*(double scale, const RadialProjectionResult<T>& obj)
    {
        auto func = [&](auto x){return scale;};
        return func * obj;
    }

    template<typename T>
    RadialProjectionResult<T> operator*(float scale, const RadialProjectionResult<T>& obj)
    {
        auto func = [&](auto x){return scale;};
        return func * obj;
    }

    template<typename T>
    RadialProjectionResult<T> operator*(int scale, const RadialProjectionResult<T>& obj)
    {
        auto func = [&](auto x){return scale;};
        return func * obj;
    }
}



#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult_test.h"
#endif


#endif

