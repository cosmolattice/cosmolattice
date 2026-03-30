#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SU2DOUBLETBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SU2DOUBLETBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <string>
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
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

  template <int NDIM, typename T, size_t NSU2DOUBLET, typename SU2DOUBLETU1COUPLINGS, typename SU2DOUBLETSU2COUPLINGS>
  class SU2DoubletSectorBase
  {
  public:
    static constexpr size_t NSU2Doublet = NSU2DOUBLET;
    using SU2DoubletU1Couplings = typename SU2DOUBLETU1COUPLINGS::template Container<T>;
    using SU2DoubletSU2Couplings = typename SU2DOUBLETSU2COUPLINGS::template Container<T>;

    // SU2 doublet fields
    FieldCollection<SU2Doublet<T, NDIM>, NSU2DOUBLET> fldSU2Doublet;
    FieldCollection<SU2Doublet<T, NDIM>, NSU2DOUBLET> piSU2Doublet;

    // Averages SU2 doublet
    T SU2DblGrad2AvI, SU2DblGrad2AvSI;
    T SU2DblPi2AvSI, SU2DblPi2AvSIM, SU2DblPi2AvIM, SU2DblPi2AvI;

    // Initial amplitudes (homogeneous modes)
    TempLatArray<SU2DoubletWrapper<T, T, T, T>, NSU2DOUBLET> fldSU2Doublet0;

    // Initial time-derivatives (homogeneous modes)
    TempLatArray<SU2DoubletWrapper<T, T, T, T>, NSU2DOUBLET> piSU2Doublet0;

    // Effective masses
    TempLatArray<SU2DoubletWrapper<T, T, T, T>, NSU2DOUBLET> masses2SU2Doublet;

    // SU2Doublet-U1 and SU2Doublet-SU2 gauge couplings
    SU2DoubletU1Couplings gQ_SU2DblU1;
    SU2DoubletSU2Couplings gQ_SU2DblSU2;

    // Potential stubs (to be overridden in derived models)
    template <int N> auto potDerivNormSU2Doublet(Tag<N>)
    {
      throw(PotentialDerivativeNotDefined("You tried to call potDerivNormSU2Doublet N = " + std::to_string(N) +
                                          ", which is not defined in your model. Abort."));
      return ZeroType();
    }
    template <int N> auto potDeriv2NormSU2Doublet(Tag<N>)
    {
      throw(PotentialDerivativeNotDefined("You tried to call potDeriv2NormSU2Doublet N = " + std::to_string(N) +
                                          ", which is not defined in your model. Abort."));
      return ZeroType();
    }

  protected:
    SU2DoubletSectorBase(ParameterParser &parser, device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox,
                         const LatticeParameters<T> &par)
        : fldSU2Doublet("SU2Doublet", toolBox, par), piSU2Doublet("pi_SU2Doublet", toolBox, par)
    {
      auto gU1s = parser.get<double, SU2DoubletU1Couplings::nGauge>("gU1s", 1.0);
      auto SU2DoubletU1Charges = parser.get<double, SU2DoubletU1Couplings::howManyCouples()>("SU2DoubletU1_charges", 1);
      gQ_SU2DblU1.setEffectiveCharges(SU2DoubletU1Charges, gU1s);

      auto gSU2s = parser.get<double, SU2DoubletSU2Couplings::nGauge>("gSU2s", 1.0);
      auto SU2DoubletSU2Charges =
          parser.get<double, SU2DoubletSU2Couplings::howManyCouples()>("SU2DoubletSU2_charges", 1);
      gQ_SU2DblSU2.setEffectiveCharges(SU2DoubletSU2Charges, gSU2s);
    }
  };

} // namespace TempLat

#endif
