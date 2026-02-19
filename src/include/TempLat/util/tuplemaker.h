#ifndef TEMPLAT_UTIL_TUPLEMAKER_H
#define TEMPLAT_UTIL_TUPLEMAKER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/util/iscomposite.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"

namespace TempLat
{
  /** @brief A class which make a tuple from a composite object.
   *
   *
   * Unit test: ctest -R test-tuplemaker
   **/
  template <typename R, class = std::void_t<>> struct number_to_skip_as_tuple {
    static const size_t value = 0;
  };

  template <typename R> struct number_to_skip_as_tuple<R, std::void_t<decltype(R::numberToSkipAsTuple)>> {
    static const size_t value = R::numberToSkipAsTuple;
  };

  template <typename R, bool> struct TupleMakerHelper {
    auto operator()(R &&r)
    {
      using nakedR = typename std::decay_t<R>;
      return make_tuple_tag<number_to_skip_as_tuple<nakedR>::value, tuple_size<nakedR>::value>(
          [&](auto i) { return nakedR::Getter::get(r, i); });
    }
  };

  template <typename R> struct TupleMakerHelper<R, true> {
    using nakedR = typename std::decay_t<R>;
    auto operator()(R &&r)
    {
      return make_tuple_tag<number_to_skip_as_tuple<nakedR>::value, tuple_size<nakedR>::value>([&](auto i) {
        using childR =
            typename std::remove_cv<typename std::remove_reference<decltype(nakedR::Getter::get(r, i))>::type>::type;
        return TupleMakerHelper<childR, IsComposite<childR>::value>()(nakedR::Getter::get(r, i));
      });
    }
  };

  template <typename R> using TupleMaker = TupleMakerHelper<R, IsComposite<R>::value>;

  template <typename R>
    requires HasStaticGetter<std::decay_t<R>>
  auto make_tuple_from(R &&r)
  {
    return TupleMaker<R>()(std::forward<R>(r));
  }

  template <typename R>
    requires(!HasStaticGetter<std::decay_t<R>>)
  auto make_tuple_from(R &&r)
  {
    return std::make_tuple(std::forward<R>(r));
  }
} // namespace TempLat

#endif
