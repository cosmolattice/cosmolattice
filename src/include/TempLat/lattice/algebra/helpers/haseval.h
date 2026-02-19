#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/parallel/device.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat
{
  /** @brief concept to detect if the eval method has been defined in a given class.
   *
   * Unit test: ctest -R test-haseval
   *
   * @tparam U the type to check
   * @tparam IDX indices to check the eval method with.
   **/
  template <typename U, typename... IDX>
  concept HasEval = requires(std::decay_t<U> obj, IDX... idx) { obj.eval(idx...); };

  /**
   * @brief Concept to detect if a type is an arithmetic type or a complex type, in which case it can be evaluated as
   * itself.
   *
   * @tparam U
   */
  template <typename U>
  concept TypeEvalsItself = (std::is_arithmetic_v<std::decay_t<U>> || IsComplexType<std::decay_t<U>>);

  /**
   * @brief Concept to detect if a type has a static value member, which can be evaluated at compile time.
   *
   * @tparam U
   */
  template <typename U>
  concept TypeHasStaticValue = requires { std::is_arithmetic_v<decltype(std::decay_t<U>::value)>; };

  template <class T>
  concept HasEvalMethod =
      HasEval<T, size_t> || HasEval<T, size_t, size_t> || HasEval<T, size_t, size_t, size_t> ||
      HasEval<T, size_t, size_t, size_t, size_t> || HasEval<T, size_t, size_t, size_t, size_t, size_t> ||
      HasEval<T, size_t, size_t, size_t, size_t, size_t, size_t> ||
      HasEval<T, size_t, size_t, size_t, size_t, size_t, size_t, size_t>;
} // namespace TempLat

#endif
