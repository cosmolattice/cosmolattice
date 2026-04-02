#ifndef COSMOINTERFACE_MEASUREMENTS_SPECTRUMSAVER_H
#define COSMOINTERFACE_MEASUREMENTS_SPECTRUMSAVER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "CosmoInterface/measurements/measurementsIO/std/spectrumsaverstd.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/unbinnedradialprojectionresult.h" //Added so that the code compiles without HDF5, even if not used here
#ifdef HAVE_HDF5
#include "CosmoInterface/measurements/measurementsIO/hdf5/spectrumsaverhdf5.h"
#include "CosmoInterface/measurements/measurementsIO/hdf5/unbinnedspectrumsaverhdf5.h"
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
                  bool dontCreate = false):
                  useHDF5(fm.getUseHDF5Spectra()),
                  unbinnedSpectra(rPar.unbinnedSpectra)

    {
      if (not dontCreate) {
        if (unbinnedSpectra) {
#ifdef HAVE_HDF5
          unbinnedSaverHDF5 = std::make_shared<UnbinnedSpectrumSaverHDF5<T>>(fm, fn, amIRoot, appendMode, rPar);
#else
          throw(UseHDF5ButNotCompiled(
              "Unbinned spectra needs HDF5, but compiled without HDF5 option."));
#endif
        }
        else if (useHDF5) {
#ifdef HAVE_HDF5
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
    SpectrumSaver(FilesManager<NDim> &fm, const Field<T, NDim> &fld, bool amIRoot, bool appendMode,
                  const RunParameters<T> &rPar, bool dontCreate = false) :
        useHDF5(fm.getUseHDF5Spectra()),
        unbinnedSpectra(rPar.unbinnedSpectra)
    {

      if (not dontCreate) {
        if(unbinnedSpectra) {
#ifdef HAVE_HDF5
          unbinnedSaverHDF5 = std::make_shared<UnbinnedSpectrumSaverHDF5<T>>(fm, fld, amIRoot, appendMode, rPar);
#else
          throw(UseHDF5ButNotCompiled(
            "Unbinned spectra needs HDF5, but compiled without HDF5 option."));
#endif
        }
        if (useHDF5) {
#ifdef HAVE_HDF5
          saverHDF5 = std::make_shared<SpectrumSaverHDF5<T>>(fm, fld, amIRoot, appendMode, rPar);
#else
          throw(
              UseHDF5ButNotCompiled("Call to use HDF5 for the measurementsIO output, but compiled without HDF5 option."));
#endif
        } else {
          saverStd = std::make_shared<SpectrumSaverStd<T>>(fm, fld, amIRoot, appendMode, rPar);
        }
      }
    }


    template<template<typename> typename Spectrum, template<typename> typename... Spectra>
    void saveV(bool lastMeas, T& t, std::vector<std::shared_ptr<Spectrum<T>>>& v, Spectrum<T> spectrum1, Spectra<T>... spectra){
      v.push_back(std::make_shared<Spectrum<T>>(spectrum1));
      saveV(lastMeas, t, v, spectra...);
    }

    template<template<typename> typename Spectrum>
    void saveV(bool lastMeas, T& t, std::vector<std::shared_ptr<Spectrum<T>>>& v){
      saveSpectra(lastMeas, t, v);
    }

    template<template<typename> typename Spectrum, template<typename> typename... Spectra>
    void save(bool lastMeas, T& t, Spectrum<T> spectrum1, Spectra<T>... spectra){
      std::vector<std::shared_ptr<Spectrum<T>>> v = {std::make_shared<Spectrum<T>>(spectrum1)};
      saveV(lastMeas, t, v, spectra...);
    }

    template<template<typename> typename Spectrum, template<typename> typename... Spectra>
    void save(T& t, Spectrum<T> spectrum1, Spectra<T>... spectra){
      std::vector<std::shared_ptr<Spectrum<T>>> v = {std::make_shared<Spectrum<T>>(spectrum1)};
      saveV(false, t, v, spectra...);
    }

    template<template<typename> typename Spectrum>
    void save(bool lastMeas, T& t, Spectrum<T> spectrum1){
      std::vector<std::shared_ptr<Spectrum<T>>> v = {std::make_shared<Spectrum<T>>(spectrum1)};
      saveSpectra(lastMeas, t, v);
    }

    template<template<typename> typename Spectrum>
    void save(T& t, Spectrum<T> spectrum1){
      std::vector<std::shared_ptr<Spectrum<T>>> v = {std::make_shared<Spectrum<T>>(spectrum1)};
      saveSpectra(false, t, v);
    }

    void saveSpectra(bool lastMeas, T& t, std::vector<std::shared_ptr<UnbinnedRadialProjectionResult<T>>> spectra) {
#ifdef HAVE_HDF5
      unbinnedSaverHDF5->save(lastMeas, spectra, t);
#else
      throw (UseHDF5ButNotCompiled(
        "Unbinned spectra needs HDF5, but compiled without HDF5 option."));
#endif
    }

    void saveSpectra(bool lastMeas, T& t, std::vector<std::shared_ptr<RadialProjectionResult<T>>> spectra){
      if(useHDF5) {
#ifdef HAVE_HDF5
        saverHDF5->save(lastMeas,spectra, t);
#else
        throw (UseHDF5ButNotCompiled(
          "Call to use HDF5 for the measurementsIO output, but compiled without HDF5 option."));
#endif
      }
      else {
        saverStd->save(spectra, t);
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    bool useHDF5, unbinnedSpectra;;
    std::shared_ptr<SpectrumSaverStd<T>> saverStd;
#ifdef HAVE_HDF5
    std::shared_ptr<SpectrumSaverHDF5<T>> saverHDF5;
    std::shared_ptr<UnbinnedSpectrumSaverHDF5<T>> unbinnedSaverHDF5;
#endif
  };

} // namespace TempLat

#endif
