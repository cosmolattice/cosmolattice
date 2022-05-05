#ifndef TEMPLAT_PARAMETERS_PARAMETERS_H
#define TEMPLAT_PARAMETERS_PARAMETERS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parameters/pairmaker.h"
#include <unordered_map>
#include "TempLat/parameters/stringconverter.h"
#include "TempLat/parameters/filereader.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include <random>
#include <iomanip>

#ifndef NOMPI
  #include <mpi.h> //This is to make getSeed MPI safe.
#endif
//#include "TempLat/parameters/mathparser/reducer.h"


namespace TempLat {

    enum ParameterImportance {Important, Normal} ;


    /** \brief The outside-world interface for parameter parsing. Pass your argc/argv, receive
     *   an object that parsed everything that you put on the command line interface,
     *   where 'everything' means that you pass arguments in the form:
     *
     *   a=1 b=2 c=3
     *
     *   The special keyword 'input' takes a filename as value, pointing to a file
     *   in which you put all the parameters at once.
     *
     *
     * Unit test: make test-parameters
     **/
    MakeException(ParameterParserMissingMandatory);
    MakeException(ParameterParserMismatchSizes);
    MakeException(ParameterParserMismatchDefaultSizes);
    MakeException(ParameterParserDoesNotExist);
    MakeException(ParameterParserIsEmpty);


    class ParameterParser {
    public:

        /* Put public methods here. These should change very little over time. */
        ParameterParser(int argc, char *argv[], bool pVerbose = false) :
                params(),
                pairMaker(),
                mVerbose(pVerbose),
                mOverride(false) {
            for (int i = 1; i < argc; ++i) {
                insert(argv[i]);
            }
            if (params.count("input") > 0) {
                FileReader fr;
                fr(params["input"]);
                for (size_t i = 0; i < fr.size(); ++i) {
                    insert(fr[i]);
                }
                assignImportance("input", Normal);
            }
        }

        void addFromVector(const std::vector<std::string> &vec) {
            for (auto x : vec) {
                insert(x);
            }
        }

        template<class S>
        ParameterGetter<S> get(const std::string &name, ParameterImportance importance = Normal) {
            return this->get<S>(name, true, {S()}, false, Tag<1>(), importance)[0];
        }

        template<class S>
        ParameterGetter<S> get(const std::string &name, S def, ParameterImportance importance = Normal) {
            return this->get<S>(name, false, {def}, false, Tag<1>(), importance)[0];
        }

        template<class S, int N>
        MultipleParameterGetter<S> get(const std::string &name, ParameterImportance importance = Normal) {
            return this->get<S>(name, true, {}, false, Tag<N>(), importance);
        }

        template<class S, int N>
        MultipleParameterGetter<S> get(const std::string &name, std::vector<S> def, ParameterImportance importance = Normal) {
            return this->get<S>(name, false, def, false, Tag<N>(), importance);
        }

        template<class S, int N>
        MultipleParameterGetter<S> get(const std::string &name, S def, ParameterImportance importance = Normal) {
            return this->get<S>(name, false, {def}, false, Tag<N>(), importance);
        }

        template<class S>
        ParameterGetter<S> getOverride(const std::string &name, ParameterImportance importance = Normal) {
            return this->get<S>(name, true, {S()}, mOverride, Tag<1>(), importance)[0];
        }

        template<class S>
        ParameterGetter<S> getOverride(const std::string &name, S def, ParameterImportance importance = Normal) {
            return this->get<S>(name, false, {def}, mOverride, Tag<1>(), importance)[0];
        }

        template<class S, int N>
        MultipleParameterGetter<S> getOverride(const std::string &name, ParameterImportance importance = Normal) {
            return this->get<S>(name, true, {}, mOverride, Tag<N>(), importance);
        }

        template<class S, int N>
        MultipleParameterGetter<S> getOverride(const std::string &name, std::vector<S> def, ParameterImportance importance = Normal) {
            return this->get<S>(name, false, def, mOverride, Tag<N>(), importance);
        }

        std::string getSeed(const std::string &name) {
            std::string tmp = this->get<std::string>(name, "0")();
            if (tmp == "0") {
                std::random_device r;
                unsigned int res = r();
                #ifndef NOMPI
                  MPI_Bcast(&res, 1, MPI_INT, 0,MPI_COMM_WORLD);
                #endif
                tmp = std::to_string(res);
            }
            params[name] = tmp;
            return tmp;
        }


        const auto &getParams() const {
            return params;
        }

        void activateOverride() {
            mOverride = true;
        }

        void
        insert(const std::string &str) { //Insert value of the current PairMaker in the unordered_map. If already exist, skip it.
            pairMaker(str);

            if (params.count(pairMaker.getKey()) == 0) {

                params[pairMaker.getKey()] = pairMaker.getValue();

                if (params[pairMaker.getKey()] == "")
                    throw (ParameterParserIsEmpty("Parameter " + pairMaker.getKey() +
                                                  " is empty. Abort. If it is an optional parameter, remove it or comment it out."));
            } else {
                if (mVerbose)
                    say << "Parameter " + pairMaker.getKey() +
                           " already assigned, skipped. The last value of this parameter which is skipped will be cached to have the potential of overriding it.";
                paramsOverride[pairMaker.getKey()] = pairMaker.getValue(); //Store last of the extra value here.
            }

        }

        void erase(const std::string &key) {
            params.erase(key);
        }


        friend std::ostream &operator<<(std::ostream &os, const ParameterParser &par) {
            std::stringstream importantParams ;
            importantParams << "Important parameters:" << std::endl<< std::endl;
            std::stringstream otherParams ;
            otherParams << "Other parameters:" << std::endl<< std::endl;

            std::string second;

            for (const auto& i: par.params) {
                if(par.paramsImportance.count(i.first)>0) {
                    second = par.paramsOverriden.count(i.first) > 0 ? par.paramsOverriden.at(i.first) : i.second;
                    if (par.paramsImportance.at(i.first) == Important)
                        importantParams << std::right << std::setw(20) << i.first << "       " << std::setw(20)
                                        << std::left << second << std::endl;
                    if (par.paramsImportance.at(i.first) == Normal)
                        otherParams << std::right << std::setw(20) << i.first << "       " << std::setw(20) << std::left
                                    << second << std::endl;
                }
            }


            os << importantParams.str() << std::endl << std::endl << otherParams.str();


            return os;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::unordered_map<std::string, std::string> params;
        std::unordered_map<std::string, std::string> paramsOverride;
        std::unordered_map<std::string, std::string> paramsOverriden; //This is the list of parameters which have actually been overriden

        std::unordered_map<std::string, int> paramsImportance; // just for printing purposes


        PairMaker pairMaker;
        const bool mVerbose;
        bool mOverride;


        template<class S, int N>
        MultipleParameterGetter<S>
        get(const std::string &name, const bool &mandatory, std::vector<S> def, bool override, Tag<N>, ParameterImportance importance = Normal) { //give the templated integer as a tag so we can specialise for N=0.
            MultipleParameterGetter<S> ret;
            StringConverter<S> conv;
            if ((!override || paramsOverride.count(name) == 0) && params.count(name) > 0) {
                conv(params[name], ret, name);
                if (N > 0 && mandatory && ret.size() != N)
                    throw (ParameterParserMismatchSizes(
                            "Parameter " + name + " was provided with an unexpected number of arguments. Abort."));
                if (N > 0 && !mandatory && ret.size() > N)
                    throw (ParameterParserMismatchSizes(
                            "Parameter " + name + " was provided with too many arguments. Abort."));
            } else if (override && paramsOverride.count(name) > 0) {
                conv(paramsOverride[name], ret, name);
                paramsOverriden[name] = paramsOverride[name];
                if (N > 0 && ret.size() != N)
                    throw (ParameterParserMismatchSizes(
                            "Parameter " + name + " was provided with an unexpected number of arguments. Abort."));
            } else if (mandatory)
                throw (ParameterParserMissingMandatory("Mandatory parameter " + name + " was not specified. Abort."));
            else if ((N > 0 && (def.size() != 1 && def.size() != N)) || def.empty())
                throw (ParameterParserMismatchDefaultSizes("Optional parameter " + name +
                                                           " was not provided with the correct number of default values. Abort."));
            //else{
            if (ret.size() != N) {
                std::ostringstream oss;
                bool toInsert(ret.size() == 0);
                oss << name << "=";
                if (def.size() == 1) {
                    for (int i = 0; i < N - 1; ++i) {
                        def.emplace_back(def[0]);
                    }
                }
                for (size_t i = ret.size(); i < (size_t) N; ++i) {
                    ret.push_back(ParameterGetter<S>(def[i], name));
                    oss << std::boolalpha << def[i] << " ";
                }
                //if(mVerbose) say << oss.str();
                if (toInsert)
                    insert(oss.str()); //useful to insert the optionnal params so we can retrieve them later, for saving purposes for example.
            }
            assignImportance(name, importance);
            if (mVerbose) say << "Parameter initialisation: " << ret;
            return ret;
        }

        void assignImportance(std::string name, ParameterImportance importance) {
            if (paramsImportance.count(name) == 0) {
                paramsImportance[name] = importance;
            }
        }


        template<class S>
        MultipleParameterGetter<S>  get(const std::string& name, const bool& mandatory, std::vector<S> def, bool override, Tag<0>){
            MultipleParameterGetter<S>  ret;
            return ret;
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parameters/parameterparser_test.h"
#endif

#endif
