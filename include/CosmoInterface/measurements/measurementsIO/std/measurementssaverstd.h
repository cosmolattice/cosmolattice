#ifndef COSMOINTERFACE_MEASUREMENTS_STD_MEASUREMENTSSAVERSTD_H
#define COSMOINTERFACE_MEASUREMENTS_STD_MEASUREMENTSSAVERSTD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/conditionaloutput/outputstream.h"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include "TempLat/lattice/field/field.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"

namespace TempLat
{
  /** @brief A class which is used to save measurements using the standard library.
   *
   *
   * Unit test: ctest -R test-measurementssaverstd
   **/
  template <typename T> class MeasurementsSaverStd
  {
  public:
    // Put public methods here. These should change very little over time.
    template <size_t NDim>
    MeasurementsSaverStd(FilesManager<NDim> &fm, std::string fn, bool amIRoot, bool appendMode,
                         const std::vector<std::string> &headers = {})
        : mMode(!appendMode ? std::ios_base::out : std::ios_base::app),
          outputAv(makeOutputStream(fm.getWorkingDir() + fm.getTag() + "average_" + fn + ".txt", amIRoot, appendMode,
                                    fm.getOverwriteMode())),
          stream(std::make_shared<std::stringstream>()), headerStream(std::make_shared<std::stringstream>()),
          headerSaved(false)
    {
      if (!appendMode && fm.getPrintHeaders())
        for (auto &str : headers)
          addHeader(str);
      else
        headerSaved = true;
    }

    template <size_t NDim>
    MeasurementsSaverStd(FilesManager<NDim> &fm, const Field<T, NDim> &fld, bool amIRoot, bool appendMode,
                         const std::vector<std::string> &headers = {})
        : mMode(!appendMode ? std::ios_base::out : std::ios_base::app), stream(std::make_shared<std::stringstream>()),
          headerStream(std::make_shared<std::stringstream>()), headerSaved(false)
    {
      auto name = fm.getCurredName(fld, true);
      outputAv = makeOutputStream(name + ".txt", amIRoot, appendMode, fm.getOverwriteMode());
      if (!appendMode && fm.getPrintHeaders())
        for (auto &str : headers)
          addHeader(str);
      else
        headerSaved = true;
    }

    virtual ~MeasurementsSaverStd() = default;

    void addAverage(const T &r) {
      if constexpr (std::floating_point<T> && (sizeof(T) == sizeof(float))) (*stream) << std::setprecision(8);
      else (*stream) << std::setprecision(15);
      (*stream) << "    " << r;
      // (*stream) << std::setprecision(15) << "    " << r;
       }

    void addHeader(const std::string &str) { (*headerStream) << "    " << str; }

    void save()
    {
      if (!headerSaved) {
        (*outputAv) << "#" << StringTrimmer::ltrimmed(headerStream->str()) << "\n";
        headerSaved = true;
      }
      (*outputAv) << StringTrimmer::ltrimmed(stream->str()) << "\n";
      stream->str("");
      outputAv->flush();
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::shared_ptr<OutputStream<T>> makeOutputStream(const std::string &fname, bool amIRoot, bool appendMode,
                                                      bool overwriteMode)
    {
      if (!appendMode && std::filesystem::exists(fname) && amIRoot) {
        if (overwriteMode) {
          std::filesystem::remove(fname);
        } else {
          throw(FileAlreadyExistsError(
              "Refusing to overwrite existing output file \"" + fname +
              "\". Set 'appendToFiles = true' to append, or 'overwriteFiles = true' to delete."));
        }
      }
      return std::make_shared<OutputStream<T>>(fname, amIRoot, mMode);
    }

    std::ios_base::openmode mMode;
    std::shared_ptr<OutputStream<T>> outputAv;
    std::shared_ptr<std::stringstream> stream;
    std::shared_ptr<std::stringstream> headerStream;

    bool headerSaved;
  };
} // namespace TempLat

#endif
