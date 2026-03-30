#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_COMPLEXSCALARBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_COMPLEXSCALARBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <string>
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/util/templatarray.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "CosmoInterface/abstractmodel/exceptions.h"
#include "CosmoInterface/couplingsmanager.h"
#include "CosmoInterface/fieldsnumbering.h"
#include "TempLat/parameters/parameterparser.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat
{

  template <int NDIM, typename T, size_t NC, typename CSU1COUPLINGS> class ComplexScalarBase
  {
  public:
    static constexpr size_t NCs = NC;
    using CsU1Couplings = typename CSU1COUPLINGS::template Container<T>;

    // Complex scalar fields
    FieldCollection<ComplexField<T, NDIM>, NC> fldCS;
    FieldCollection<ComplexField<T, NDIM>, NC> piCS;

    // Averages complex scalar
    T CSgrad2AvI, CSgrad2AvSI;
    T CSpi2AvSI, CSpi2AvSIM, CSpi2AvIM, CSpi2AvI;

    // Initial amplitudes (homogeneous modes)
    TempLatArray<ComplexFieldWrapper<T, T>, NC> fldCS0;

    // Initial time-derivatives (homogeneous modes)
    TempLatArray<ComplexFieldWrapper<T, T>, NC> piCS0;

    // Effective masses
    TempLatArray<ComplexFieldWrapper<T, T>, NC> masses2CS;

    // CS-U1 gauge coupling
    CsU1Couplings gQ_CsU1;

    // Potential stubs (to be overridden in derived models)
    template <int N> auto potDerivNormCS(Tag<N>)
    {
      throw(PotentialDerivativeNotDefined("You tried to call potDerivNormCS N = " + std::to_string(N) +
                                          ", which is not defined in your model. Abort."));
      return ZeroType();
    }
    template <int N> auto potDeriv2NormCS(Tag<N>)
    {
      throw(PotentialDerivativeNotDefined("You tried to call potDeriv2NormCS N = " + std::to_string(N) +
                                          ", which is not defined in your model. Abort."));
      return ZeroType();
    }

  protected:
    ComplexScalarBase(ParameterParser &parser, device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox,
                      const LatticeParameters<T> &par)
        : fldCS("cmplx_scalar", toolBox, par), piCS("pi_cmplx_scalar", toolBox, par)
    {
      auto gU1s = parser.get<double, CsU1Couplings::nGauge>("gU1s", 1.0);
      auto CSU1Charges = parser.get<double, CsU1Couplings::howManyCouples()>("CSU1_charges", 1);
      gQ_CsU1.setEffectiveCharges(CSU1Charges, gU1s);
    }
  };

} // namespace TempLat

#endif
