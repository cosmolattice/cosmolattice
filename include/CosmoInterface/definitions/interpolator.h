#ifndef COSMOINTERFACE_DEFINITIONS_INTERPOLATOR_H
#define COSMOINTERFACE_DEFINITIONS_INTERPOLATOR_H

#include <vector>
#include <algorithm>
#include <fstream>

#include "TempLat/lattice/algebra/coordinates/wavenumber.h"   // WaveNumber + FourierSite alias
#include "TempLat/lattice/memory/memorytoolbox.h"            // MemoryToolBox<NDim>
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h" // IsVariadicNDIndex
#include "CosmoInterface/definitions/spline.h"               // tk::spline wrapper

namespace TempLat {

template <size_t NDim, typename T>
class InterpolatorFunction {
public:
  using ToolBoxPtr = device::memory::host_ptr<MemoryToolBox<NDim>>;

  InterpolatorFunction(const std::vector<T>& kIn,
                       const std::vector<T>& psIn,
                       ToolBoxPtr toolBox,
                       T kIRIn)
    : mToolBox(toolBox)
    , ntilde(toolBox)
    , kIR(kIRIn)
    , kMin(kIn.empty() ? T(0) : kIn.front())
    , kMax(kIn.empty() ? T(0) : kIn.back())
  {
    std::vector<double> kd; kd.reserve(kIn.size());
    std::vector<double> pd; pd.reserve(psIn.size());
    for (auto v : kIn)  kd.push_back(static_cast<double>(v));
    for (auto v : psIn) pd.push_back(static_cast<double>(v));
    splineObject.set_points(kd, pd);
  }

  // This is what TempLat operators (safeSqrt, +, *, etc.) want.
  template <typename... IDX>
    requires IsVariadicNDIndex<NDim, IDX...>
  DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX&... idx) const
  {
    auto kSite = ntilde.norm().eval(idx...) * kIR;   // k = |ntilde| * kIR
    T kEval = clampK(static_cast<T>(kSite));
    return static_cast<T>(splineObject(static_cast<double>(kEval)));
  }

  ToolBoxPtr getToolBox() const { return mToolBox; }

  std::string operatorString() const { return "Interpolator"; }

private:
  tk::spline splineObject;
  ToolBoxPtr mToolBox;
  FourierSite<NDim> ntilde;
  T kIR, kMin, kMax;

  T clampK(T k) const
  {
    if (k < kMin) return kMin;
    if (k > kMax) return kMax;
    return k;
  }
};

} 
#endif