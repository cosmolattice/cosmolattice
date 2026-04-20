#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_GWBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_GWBASE_H

#include <memory>
#include "TempLat/lattice/algebra/matrix3x3algebra/matrix3x3algebra.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/parameters/parameterparser.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat
{

  template <int NDIM, typename T> class GWBase
  {
  public:

    std::unique_ptr<SymTracelessField<T, NDIM>> fldGWs;
    std::unique_ptr<SymTracelessField<T, NDIM>> piGWs;

  protected:
    GWBase(ParameterParser &parser, device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox,
           const LatticeParameters<T> &par)
        : fldGWs(parser.get<bool>("withGWs", false)
                     ? std::make_unique<SymTracelessField<T, NDIM>>("fldGWs", toolBox, par)
                     : nullptr),
          piGWs(parser.get<bool>("withGWs", false)
                    ? std::make_unique<SymTracelessField<T, NDIM>>("piGWs", toolBox, par)
                    : nullptr)
    {
    }
  };

} // namespace TempLat

#endif
