#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_U1BASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_U1BASE_H

#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "CosmoInterface/fieldsnumbering.h"
#include "CosmoInterface/initializers/initialconditionstype.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat
{

  template <int NDIM, typename T, size_t NU1FLDS, size_t NC> class U1Base
  {
  public:
    static constexpr size_t NU1 = NU1FLDS;

    // U(1) gauge fields
    VectorFieldCollection<Field<T, NDIM>, NU1FLDS> fldU1;
    VectorFieldCollection<Field<T, NDIM>, NU1FLDS> piU1;

    // Averages U1
    T U1Mag2AvI, U1Mag2AvSI;
    T U1pi2AvSI, U1pi2AvSIM, U1pi2AvIM, U1pi2AvI;

    InitialConditionsType::U1 getU1IC()
    {
      if (NC > 0)
        return InitialConditionsType::RandomWithMatter;
      else
        return InitialConditionsType::PlaneWavesZeroB;
    }

  protected:
    U1Base(device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox, const LatticeParameters<T> &par)
        : fldU1("U1", toolBox, par), piU1("pi_U1", toolBox, par)
    {
    }
  };

} // namespace TempLat

#endif
