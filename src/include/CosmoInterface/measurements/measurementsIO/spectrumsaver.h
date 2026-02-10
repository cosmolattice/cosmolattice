#ifndef COSMOINTERFACE_MEASUREMENTS_SPECTRUMSAVER_H
#define COSMOINTERFACE_MEASUREMENTS_SPECTRUMSAVER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "CosmoInterface/measurements/measurementsIO/std/spectrumsaverstd.h"
#ifdef HDF5
#include "CosmoInterface/measurements/measurementsIO/hdf5/spectrumsaverhdf5.h"
#endif

namespace TempLat
{
  /** @brief A class which saves spectra to files.
   *
   * Unit test: ctest -R test-spectrumsaver
   **/
  template <typename T> class SpectrumSaver
  {
  public:
    // Put public methods here. These should change very little over time.

    template <size_t NDim>
    SpectrumSaver(FilesManager<NDim> &fm, std::string fn, bool amIRoot, bool appendMode, const RunParameters<T> &rPar,
                  bool dontCreate = false)
        : useHDF5(fm.getUseHDF5Spectra())
    {
      if (not dontCreate) {
        if (useHDF5) {
#ifdef HDF5
          saverHDF5 = std::make_shared<SpectrumSaverHDF5<T>>(fm, fn, amIRoot, appendMode, rPar);
#else
          throw(UseHDF5ButNotCompiled(
              "Call to use HDF5 for the measurementsIO output, but compiled without HDF5 option."));
#endif
        } else {
          saverStd = std::make_shared<SpectrumSaverStd<T>>(fm, fn, amIRoot, appendMode, rPar);
        }
      }
    }

    template <size_t NDim>
    SpectrumSaver(FilesManager<NDim> &fm, const Field<NDim, T> &fld, bool amIRoot, bool appendMode,
                  const RunParameters<T> &rPar, bool dontCreate = false)
        : useHDF5(fm.getUseHDF5Spectra())
    {

      if (useHDF5) {
#ifdef HDF5
        saverHDF5 = std::make_shared<SpectrumSaverHDF5<T>>(fm, fld, amIRoot, appendMode, rPar);
#else
        throw(
            UseHDF5ButNotCompiled("Call to use HDF5 for the measurementsIO output, but compiled without HDF5 option."));
#endif
      } else {
        saverStd = std::make_shared<SpectrumSaverStd<T>>(fm, fld, amIRoot, appendMode, rPar);
      }
    }

    template <template <typename> class... Spectra> void save(T &t, Spectra<T>... spectra)
    {
      if (useHDF5) {
#ifdef HDF5
        saverHDF5->save(
            std::vector<std::shared_ptr<RadialProjectionResult<T>>>{
                std::make_shared<RadialProjectionResult<T>>(spectra)...},
            t);
#else
        throw(
            UseHDF5ButNotCompiled("Call to use HDF5 for the measurementsIO output, but compiled without HDF5 option."));
#endif
      } else
        saverStd->save(
            std::vector<std::shared_ptr<RadialProjectionResult<T>>>{
                std::make_shared<RadialProjectionResult<T>>(spectra)...},
            t);
    }

    /*template< template<typename> class... Spectra, class Model>
        void saveEnergy(T& t, Spectra<T>... spectra, Model& model){

          saverStd->saveEnergy(std::vector<std::shared_ptr<RadialProjectionResult<T>>>
       {std::make_shared<RadialProjectionResult<T>>(spectra)...}, t, model);
        }*/

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    bool useHDF5;
    std::shared_ptr<SpectrumSaverStd<T>> saverStd;
#ifdef HDF5
    std::shared_ptr<SpectrumSaverHDF5<T>> saverHDF5;
#endif
  };

#ifdef TEMPLATTEST
  struct SpectrumSaverTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif

} // namespace TempLat

#endif
