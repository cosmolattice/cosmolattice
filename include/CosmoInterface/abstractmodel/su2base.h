#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SU2BASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SU2BASE_H

#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "CosmoInterface/fieldsnumbering.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat
{

  template <int NDIM, typename T, size_t NSU2FLDS> class SU2Base
  {
  public:
    static constexpr size_t NSU2 = NSU2FLDS;

    // SU(2) gauge fields
    VectorFieldCollection<SU2Field<T, NDIM>, NSU2FLDS> fldSU2;
    VectorFieldCollection<SU2LieAlgebraField<T, NDIM>, NSU2FLDS> piSU2;

    // Averages SU2
    T SU2Mag2AvI, SU2Mag2AvSI;
    T SU2pi2AvSI, SU2pi2AvSIM, SU2pi2AvIM, SU2pi2AvI;

  protected:
    SU2Base(device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox, const LatticeParameters<T> &par)
        : fldSU2("SU2Fld", toolBox, par), piSU2("pi_SU2Fld", toolBox, par)
    {
    }
  };

} // namespace TempLat

#endif
