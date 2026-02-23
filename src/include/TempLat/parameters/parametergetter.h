#ifndef TEMPLAT_PARAMETERS_PARAMETERGETTER_H
#define TEMPLAT_PARAMETERS_PARAMETERGETTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <ostream>

namespace TempLat
{
  /** @brief
   *  Class for parameter .handler. Special getter which can directly cast
   * to T.
   *
   * Unit test: ctest -R test-parametergetter
   **/
  template <typename T> class ParameterGetter
  {
  public:
    // Put public methods here. These should change very little over time.
    ParameterGetter(T instance, std::string name) : mName(name), mInstance(instance) {}
    ParameterGetter(const ParameterGetter &) = default;
    const T &operator()() { return this->mInstance; }

    operator T() { return mInstance; }

    std::string toString() const { return this->mName; }
    friend std::ostream &operator<<(std::ostream &os, const ParameterGetter<T> &pm)
    {
      os << pm.mName + "(" + pm.mInstance + ")";
      return os;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string mName;
    T mInstance;
  };
} // namespace TempLat

#endif
