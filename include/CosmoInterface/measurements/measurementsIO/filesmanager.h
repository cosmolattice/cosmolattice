#ifndef COSMOINTERFACE_MEASUREMENTS_FILESMANAGER_H
#define COSMOINTERFACE_MEASUREMENTS_FILESMANAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <filesystem>

#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5group.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"
#ifdef HAVE_HDF5
#include "TempLat/lattice/IO/HDF5/filesaverhdf5.h"
#endif

namespace TempLat
{
  MakeException(UseHDF5ButNotCompiled);
  MakeException(FileAlreadyExistsError);

  /** @brief Interface to switch between hdf5 and std output for measurements. This class hides the polymorphism under
   * the hood.
   *
   *
   * Unit test: ctest -R test-filesmanager
   **/
  template <size_t NDim> class FilesManager
  {
  public:
    FilesManager(ParameterParser &parser, std::string fn, device::memory::host_ptr<MemoryToolBox<NDim>> toolbox,
                 bool pUseHDF5, bool pUseHDF5Spectra, bool pPrintHeaders, bool pAppendMode, bool pOverwriteMode,
                 std::string pTag = "", ptrdiff_t pFlushFreq = 1, ptrdiff_t pNMeas = 0, ptrdiff_t pNMeasInfreq = 0,
                 bool isUnbinned = false)
        : mToolbox(toolbox), mUseHDF5(pUseHDF5), mUseHDF5Spectra(pUseHDF5Spectra || isUnbinned), mPrintHeaders(pPrintHeaders),
          mAppendMode(pAppendMode), mOverwriteMode(pOverwriteMode),
          workingDir(fn), tag(pTag), flushFreq(pFlushFreq), nMeas(pNMeas), nMeasInfreq(pNMeasInfreq)
    {
#ifdef HAVE_HDF5
      auto ensureFreshOrAppend = [&](const std::string &h5fn, bool saveParser) {
        if (!prepareOutputFile(h5fn)) return;
        FileSaverHDF5 fs;
        fs.create(h5fn, Exclusive);
        if (saveParser) fs.save_attr(parser);
        fs.close();
      };
      if (mUseHDF5) ensureFreshOrAppend(getHDF5Fn(), true);
      if (mUseHDF5Spectra && !isUnbinned) ensureFreshOrAppend(getHDF5SpectraFn(), false);
      if (isUnbinned) ensureFreshOrAppend(getHDF5UnbinnedSpectraFn(), false);
#endif
    }

    bool prepareOutputFile(const std::string &fn) const
    {
      const bool exists = std::filesystem::exists(fn);
      if (mAppendMode) return !exists;
      if (exists && mToolbox->amIRoot()) {
        if (mOverwriteMode) std::filesystem::remove(fn);
        else throw(FileAlreadyExistsError(
            "Refusing to overwrite existing output file \"" + fn +
            "\". Set 'appendToFiles = true' to append, or 'overwriteFiles = true' to delete."));
      }
      return true;
    }

    void flush() {}

    bool getUseHDF5() const { return mUseHDF5; }
    bool getUseHDF5Spectra() const { return mUseHDF5Spectra; }
    bool getPrintHeaders() const { return mPrintHeaders; }
    bool getAppendMode() const { return mAppendMode; }
    bool getOverwriteMode() const { return mOverwriteMode; }

    auto getToolBox() const { return mToolbox; }

    std::string getWorkingDir() const { return workingDir; }
    std::string getTag() const { return tag; }
    std::string getHDF5Fn() const { return getWorkingDir() + getTag() + "average.h5"; }
    std::string getHDF5SpectraFn() const { return getWorkingDir() + getTag() + "spectra.h5"; }
    std::string getHDF5UnbinnedSpectraFn() const { return getWorkingDir() + getTag() + "unbinned_spectra.h5"; }

    template <typename T>
    std::string getCurredName(const Field<T, NDim> &fld, bool withDir, std::string nametag = "average")
    {
      std::string name = fld.toString();
      name = name.erase(name.find("(", 3));
      name = withDir ? getWorkingDir() + nametag + "_" + name : nametag + "_" + name;
      return name;
    }

    auto getFlushFreq() const { return flushFreq; }
    auto getNMeas() const { return nMeas; }
    auto getNInfreqMeas() const { return nMeasInfreq; }

  private:
    device::memory::host_ptr<MemoryToolBox<NDim>> mToolbox;
    bool mUseHDF5, mUseHDF5Spectra, mUseHDF5UnbinnedSpectra;
    bool mPrintHeaders;
    bool mAppendMode;
    bool mOverwriteMode;
    std::string workingDir;
    std::string tag;
    ptrdiff_t flushFreq;
    ptrdiff_t nMeas;
    ptrdiff_t nMeasInfreq;
  };

} // namespace TempLat

#endif
