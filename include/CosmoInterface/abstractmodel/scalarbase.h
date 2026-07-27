#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALARBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALARBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <array>
#include <string>
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/util/templatarray.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "CosmoInterface/abstractmodel/exceptions.h"
#include "TempLat/parameters/parameterparser.h"
#include "TempLat/util/constants.h"
#include "CosmoInterface/fieldsnumbering.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat
{

  template <int NDIM, typename T, size_t NS> class ScalarBase
  {
  public:
    static constexpr int NDim = NDIM;
    static constexpr size_t Ns = NS;
    using FloatType = T;

    bool isInitialized;

    // Scalar singlet fields
    FieldCollection<Field<T, NDIM>, NS, true> fldS;
    FieldCollection<Field<T, NDIM>, NS, true> piS;

    // Averages scalar
    T grad2AvI, grad2AvSI;
    T pi2AvSI, pi2AvSIM, pi2AvIM, pi2AvI;

    // Initial amplitudes (homogeneous modes)
    TempLatArray<T, NS> fldS0;

    // Initial time-derivatives (homogeneous modes)
    TempLatArray<T, NS> piS0;

    // Effective masses
    TempLatArray<T, NS> masses2S;

    // External initial power spectra
    std::array<std::string, NS> extPS;

    // Potential stubs (to be overridden in derived models)
    template <int N> auto potentialTerms(Tag<N>)
    {
      throw(PotentialDerivativeNotDefined("You tried to call potentialTerms N = " + std::to_string(N) +
                                          ", which is not defined in your model. Abort."));
      return ZeroType();
    }
    template <int N> auto potDeriv(Tag<N>)
    {
      throw(PotentialDerivativeNotDefined("You tried to call potDeriv N = " + std::to_string(N) +
                                          ", which is not defined in your model. Abort."));
      return ZeroType();
    }
    template <int N> auto potDeriv2(Tag<N>)
    {
      throw(PotentialDerivativeNotDefined("You tried to call potDeriv2 N = " + std::to_string(N) +
                                          ", which is not defined in your model. Abort."));
      return ZeroType();
    }

  protected:
    ScalarBase(device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox, const LatticeParameters<T> &par)
        : isInitialized(toolBox->template initializeFFT<T>()), fldS("scalar", toolBox, par),
          piS("pi_scalar", toolBox, par)
    {
    }

    ScalarBase(ParameterParser &parser, device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox,
               const LatticeParameters<T> &par)
        : isInitialized(toolBox->template initializeFFT<T>()), fldS("scalar", toolBox, par),
          piS("pi_scalar", toolBox, par)
    {
      ForLoop(i, 0, NS - 1,
              { extPS[i] = parser.get<std::string>("ext_PS" + std::to_string(i), Constants::defaultString); });
    }
  };

} // namespace TempLat

#endif
