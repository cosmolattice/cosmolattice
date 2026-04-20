#ifndef COSMOINTERFACE_COUPLINGSMANAGER_H
#define COSMOINTERFACE_COUPLINGSMANAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/exception.h"

#include <array>
#include <vector>

namespace TempLat
{
  MakeException(NotEnoughChargesForThisCouplingsManager);
  MakeException(NotEnoughCouplingsForThisCouplingsManager);

  /** @brief A class which stores statically an array of booleans; useful to manage what couples to what.
   *
   * This class stores in its type an arbitrary number of booleans, which are used to define which field couple to which
   * other field. The best way to understand how this class work is to see how it is used in the GaugeDerivatives and
   * MatterCurrent classes.
   *
   * Unit test: ctest -R test-couplingsmanager
   **/
  template <typename FloatType, int NMatter, int NGauge, bool... Bools> class CouplingsManagerContainer
  {
  public:
    static constexpr int nGauge = NGauge;

    CouplingsManagerContainer() = default;

    /**
     * @brief In addition to simply knowing what couples to what, we also store the gauge field coupling g and the
     * effective charge g*Q (Q being the matter charge).
     *
     * @param charges The charges Q, for each matter field that couples to at least one gauge field. The
     * order of the charges should be the same as the order of the non-trivial doesCouples in the template parameters.
     * @param couplings The gauge field couplings g, for each gauge field. The order of the couplings should be the same
     * as the order of the gauge fields in the model.
     */
    template <typename T>
    CouplingsManagerContainer(const std::vector<T> &charges, const std::vector<T> &couplings) : gs(couplings)
    {
      setEffectiveCharges(charges, couplings);
    }

    static constexpr bool active = true;

    void setEffectiveCharges(const std::vector<double> &charges, const std::vector<double> &couplings)
    {
      _setEffectiveCharges(charges, couplings);
    }
    void setEffectiveCharges(const std::vector<float> &charges, const std::vector<float> &couplings)
    {
      _setEffectiveCharges(charges, couplings);
    }

    /**
     * @brief Stores the effective charges, but only between fields that actually are coupled.
     *
     * @param charges The charges Q, for each matter field that couples to at least one gauge field. The
     * order of the charges should be the same as the order of the non-trivial doesCouples in the template parameters.
     * @param couplings The gauge field couplings g, for each gauge field. The order of the couplings should be the same
     * as the order of the gauge fields in the model.
     */
    template <typename T> void _setEffectiveCharges(const std::vector<T> &charges, const std::vector<T> &couplings)
    {
      if ((charges.size() != howManyCouples()) && (couplings.size() != 0))
        throw(NotEnoughChargesForThisCouplingsManager(
            std::to_string(charges.size()) + " charges were specified while " + std::to_string(howManyCouples()) +
            " non-trivial doesCouples are expected. Abort."));
      if (couplings.size() != NGauge && charges.size() != 0)
        throw(NotEnoughCouplingsForThisCouplingsManager(std::to_string(couplings.size()) +
                                                        " couplings were specified while " + std::to_string(NGauge) +
                                                        " gauge fields are present. Abort."));
      if constexpr (NGauge > 0) {
        // Set the gauge field couplings g and the effective charges g*Q for each matter-gauge coupling.
        for (size_t i = 0; i < NGauge && i < couplings.size(); ++i)
          gs[i] = couplings[i];

        size_t count = 0;
        for (size_t i = 0; i < doesCouples.size(); ++i) {
          if (doesCouples[i]) {
            effectiveCharges[i] = charges[count] * couplings[i % NGauge];
            ++count;
          } else
            effectiveCharges[i] = 0;
        }
      }
    }

    /**
     * @brief This function, accessible at compile time, tells how many fields are coupled in this CouplingManager.
     *
     */
    static constexpr size_t howManyCouples()
    {
      size_t c = 0;
      for (size_t i = 0; i < doesCouples.size(); ++i)
        c += doesCouples[i] ? 1 : 0;
      return c;
    }

    /** @brief Access the effective charge g*Q for a specific matter and gauge field.
     *
     * @tparam nmat The matter field index.
     * @tparam ng The gauge field index.
     */
    template <int nmat, int ng> auto operator()(Tag<nmat>, Tag<ng>) const
    {
      static_assert(nmat >= 0 && nmat < NMatter,
                    "Error: trying to access the coupling of a matter field which is not present in this "
                    "CouplingsManager. Abort.");
      static_assert(ng >= 0 && ng < NGauge, "Error: trying to access the coupling of a gauge field which is not "
                                            "present in this CouplingsManager. Abort.");
      if constexpr (sizeof...(Bools) == 0 || NGauge == 0) {
        return ZeroType();
      } else {
        return effectiveCharges[nmat * NGauge + ng];
      }
    }

    /**
     * @brief Access the gauge field coupling g for a specific gauge field.
     *
     * @tparam ng The gauge field index.
     */
    template <int ng> auto coupling(Tag<ng>) const
    {
      static_assert(ng >= 0 && ng < NGauge, "Error: trying to access the coupling of a gauge field which is not "
                                            "present in this CouplingsManager. Abort.");
      return gs[ng];
    }

    /**
     * @brief Accessible at compile time function which tells whether or not a given matter field and a given gauge
     * field couples or not.
     *
     * @tparam nmat The matter field index.
     * @tparam ng The gauge field index.
     */
    template <int nmat, int ng> static constexpr bool couples(Tag<nmat>, Tag<ng>)
    {
      static_assert(nmat >= 0 && nmat < NMatter,
                    "Error: trying to access the coupling of a matter field which is not present in this "
                    "CouplingsManager. Abort.");
      static_assert(ng >= 0 && ng < NGauge, "Error: trying to access the coupling of a gauge field which is not "
                                            "present in this CouplingsManager. Abort.");
      if constexpr (sizeof...(Bools) == 0 || NGauge == 0) {
        return false;
      } else {
        return doesCouples[nmat * NGauge + ng];
      }
    }

  private:
    // Compile-time accessible booleans which tell what couples to what.
    static constexpr std::array<bool, NGauge * NMatter> doesCouples = {Bools...};
    std::array<FloatType, NGauge * NMatter> effectiveCharges;
    std::array<FloatType, NGauge> gs;
  };

  template <int NMatter, int NGauge, bool... Bools> class CouplingsManager
  {
  public:
    template <typename FloatType> using Container = CouplingsManagerContainer<FloatType, NMatter, NGauge, Bools...>;

    static constexpr int nGauge = NGauge;
    static constexpr bool active = (Bools || ...);

    static constexpr size_t howManyCouples()
    {
      return (static_cast<size_t>(Bools) + ...);
    }

    template <int nmat, int ng> static constexpr bool couples(Tag<nmat>, Tag<ng>)
    {
      static_assert(nmat >= 0 && nmat < NMatter);
      static_assert(ng >= 0 && ng < NGauge);
      constexpr std::array<bool, NGauge * NMatter> doesCouples = {Bools...};
      return doesCouples[nmat * NGauge + ng];
    }
  };

  /**
   * @brief This class is a specialization of the CouplingsManager for the case where there are no gauge field
   * couplings.
   *
   */
  template <int NMatter, int NGauge> class CouplingsManager<NMatter, NGauge>
  {
  public:
    template <typename FloatType> using Container = CouplingsManager<NMatter, NGauge>;

    static constexpr int nGauge = 0;

    // Put public methods here. These should change very little over time.
    CouplingsManager() = default;

    template <typename T> CouplingsManager(const std::vector<T> &vec, std::array<T, 0> couplings) {}

    static constexpr bool active = false;

    void setEffectiveCharges(const std::vector<float> &charges, const std::vector<float> &couplings) {}
    void setEffectiveCharges(const std::vector<double> &charges, const std::vector<double> &couplings) {}

    static constexpr size_t howManyCouples() { return 0; }
    template <int nmat, int ng> ZeroType operator()(Tag<nmat>, Tag<ng>) const { return {}; }

    template <int ng> ZeroType coupling(Tag<ng>) const { return {}; }

    template <int nmat, int ng> static constexpr bool couples(Tag<nmat>, Tag<ng>) { return false; }

  private:
    static constexpr std::array<bool, 1> doesCouples = {false};
  };
} // namespace TempLat

#endif
