#ifndef TEMPLAT_UTIL_LOG_PUTTOSTREAM_H
#define TEMPLAT_UTIL_LOG_PUTTOSTREAM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <array>
#include <vector>
#include <iostream>
#include <tuple>
#include "TempLat/util/tdd/tddmacros.h"

namespace TempLat
{
  template <typename T> std::ostream &PutToStream(std::ostream &stream, const T &vec);

  /* expose these to for vectors of arrays of vectors... */
  template <typename T, typename K> std::ostream &operator<<(std::ostream &stream, const std::pair<T, K> &p)
  {
    stream << "( " << p.first << ", " << p.second << " )";
    return stream;
  };

  template <typename C>
    requires requires(C c) {
      c.begin();
      c.end();
      c.size();
      c[0];
      requires !std::is_same_v<std::string, C>;
    }
  std::ostream &operator<<(std::ostream &stream, const C &vec)
  {
    return PutToStream(stream, vec);
  };

  /** @brief Simple outputing of arrays and vectors: not exposing as operator<<, because below we want to limit it to
   * vectors and arrays, without needing to know the exact number of template parameters for this systems implementation
   * of array and vector. */
  template <typename T> std::ostream &PutToStream(std::ostream &stream, const T &vec)
  {
    stream << "{{ ";
    bool first = true;
    int limiter = 0;
    for (auto &&it : vec) {
      if (++limiter > 10) {
        stream << ", ...";
        break;
      }
      if (first) {
        first = false;
      } else {
        stream << ", ";
      }
      stream << it;
    }
    stream << " }}";
    return stream;
  }
} // namespace TempLat

#endif
