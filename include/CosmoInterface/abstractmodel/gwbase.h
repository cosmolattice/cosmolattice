#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_GWBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_GWBASE_H

#include <memory>
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/parameters/parameterparser.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat {

template <int NDIM, typename T>
class GWBase {
public:
    static constexpr size_t NGWs = 6;

    std::unique_ptr<FieldCollection<Field<T, NDIM>, 6, true>> fldGWs;
    std::unique_ptr<FieldCollection<Field<T, NDIM>, 6, true>> piGWs;

    auto pi_GWtensor(Tag<1>, Tag<1>) { return (*piGWs)(0_c); }
    auto pi_GWtensor(Tag<1>, Tag<2>) { return (*piGWs)(1_c); }
    auto pi_GWtensor(Tag<1>, Tag<3>) { return (*piGWs)(2_c); }
    auto pi_GWtensor(Tag<2>, Tag<1>) { return (*piGWs)(1_c); }
    auto pi_GWtensor(Tag<2>, Tag<2>) { return (*piGWs)(3_c); }
    auto pi_GWtensor(Tag<2>, Tag<3>) { return (*piGWs)(4_c); }
    auto pi_GWtensor(Tag<3>, Tag<1>) { return (*piGWs)(2_c); }
    auto pi_GWtensor(Tag<3>, Tag<2>) { return (*piGWs)(4_c); }
    auto pi_GWtensor(Tag<3>, Tag<3>) { return (*piGWs)(5_c); }

    auto GWtensor(Tag<1>, Tag<1>) { return (*fldGWs)(0_c); }
    auto GWtensor(Tag<1>, Tag<2>) { return (*fldGWs)(1_c); }
    auto GWtensor(Tag<1>, Tag<3>) { return (*fldGWs)(2_c); }
    auto GWtensor(Tag<2>, Tag<1>) { return (*fldGWs)(1_c); }
    auto GWtensor(Tag<2>, Tag<2>) { return (*fldGWs)(3_c); }
    auto GWtensor(Tag<2>, Tag<3>) { return (*fldGWs)(4_c); }
    auto GWtensor(Tag<3>, Tag<1>) { return (*fldGWs)(2_c); }
    auto GWtensor(Tag<3>, Tag<2>) { return (*fldGWs)(4_c); }
    auto GWtensor(Tag<3>, Tag<3>) { return (*fldGWs)(5_c); }

protected:
    GWBase(ParameterParser &parser,
           device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox,
           const LatticeParameters<T> &par)
        : fldGWs(parser.get<bool>("withGWs", false)
                     ? std::make_unique<FieldCollection<Field<T, NDIM>, 6, true>>("fldGWs", toolBox, par)
                     : nullptr),
          piGWs(parser.get<bool>("withGWs", false)
                    ? std::make_unique<FieldCollection<Field<T, NDIM>, 6, true>>("piGWs", toolBox, par)
                    : nullptr) {}
};

} // namespace TempLat

#endif
