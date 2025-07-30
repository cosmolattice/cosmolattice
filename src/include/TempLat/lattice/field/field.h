#ifndef TEMPLAT_LATTICE_FIELD_FIELD_H
#define TEMPLAT_LATTICE_FIELD_FIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/constants/halftype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/field/views/fieldviewconfig.h"
#include "TempLat/lattice/field/views/fieldviewfourier.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  // MakeException(FieldViewFourierWrongSpaceConfirmation);
  // MakeException(FieldViewConfigWrongSpaceConfirmation);

  /** \brief A class which is a classical field on your n-dimensional equisized grid.
   * You use it as a scalar field, a vector component, whatever.
   * Template parameter is your type of floating point precision: float or double. Default: double.
   *
   *  Implements a get method, and is hence suitable for all algebra.
   *
   * Unit test: make test-field
   **/
  template <size_t NDim, typename T> class Field : public ConfigView<NDim, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using ConfigView<NDim, T>::mManager;

    Field(std::string name, std::shared_ptr<MemoryToolBox<NDim>> toolBox,
          LatticeParameters<T> pLatPar = LatticeParameters<T>())
        : ConfigView<NDim, T>(name, toolBox, pLatPar), mFourierView(*this)
    {
    }

    Field() : ConfigView<NDim, T>("", nullptr, LatticeParameters<T>()), mFourierView(*this) {}

    template <typename R> void operator=(R &&g) { ConfigView<NDim, T>::operator=(g); }

    void operator=(const Field<NDim, T> &other)
    { // overwrite the default = operator.
      operator=(1 * other);
    }

    FourierView<NDim, T> &inFourierSpace()
    {
      mManager->confirmFourierSpace();
      return mFourierView;
    }

    template <typename S>
    inline typename std::enable_if<!std::is_same<Field<NDim, T>, S>::value, ZeroType>::type d(const S &other) const
    {
      return ZeroType();
    }

    /** \brief The real overload: is it a Field, then we must compare. */
    inline ptrdiff_t d(const Field<NDim, T> &other) const { return *this == other ? 1 : 0; }

    friend bool operator==(const Field<NDim, T> &a, const Field<NDim, T> &b) { return a.mManager == b.mManager; }

    /* template <typename S>
     inline
     typename std::enable_if<!std::is_same<Field<NDim, T>, S>::value, ZeroType>::type
     d(const S& other) const {
         return ZeroType();
     }

     template <typename S>
     inline
     typename std::enable_if<std::is_same<Field<NDim, T>, S>::value, OneType>::type
     d(const S& other) const {
         return OneType();
     }*/

    //    static constexpr int FLDNumber = FLDN;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    FourierView<NDim, T> mFourierView;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
