#pragma once

#include "TempLat/lattice/algebra/helpers/iscomplextype.h"
#include <type_traits>

namespace TempLat
{
  template <typename T>
  concept IsArithmetic = (std::is_arithmetic_v<T> || IsComplexType<T>);
}