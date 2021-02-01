#ifndef TEMPLAT_PARAMETERS_MATHPARSER_REDUCER_H
#define TEMPLAT_PARAMETERS_MATHPARSER_REDUCER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/parenthesisstripper.h"
#include "TempLat/parameters/mathparser/splitter.h"
#include "TempLat/parameters/parameterparser.h"

namespace TempLat {


    MakeException(UnsupportedUnaryOp);
    MakeException(AtomicExpressionNotValid);

    /** \brief A class which reduce a string expression to a double.
     *
     * 
     * Unit test: make test-reducer
     **/

    class Reducer {
    public:
        /* Put public methods here. These should change very little over time. */
        Reducer() = delete;


        static double reduce(std::string str, ParameterParser& parser) {
            double res;

            ParenthesisStripper::strip(str);

            std::vector<std::string> tmp;

            tmp = Splitter::split(str, '+');
            if (tmp.size() > 1) {
                res = 0;
                for (std::string x : tmp) res += reduce(x,parser);
                //say << "+";
                //for (std::string x : tmp) say << x;
                return res;
            } else {
                tmp = Splitter::split(str, '-');
                if (tmp.size() > 1) {
                    res = reduce(tmp[0],parser);
                    for (size_t i = 1; i < tmp.size(); ++i) res -= reduce(tmp[i],parser);
                    //say << "-";
                    //for (std::string x : tmp) say << x;
                    return res;
                } else {
                    tmp = Splitter::split(str, '*');
                    if (tmp.size() > 1) {
                        res = 1;
                        for (std::string x : tmp) res *= reduce(x,parser);
                        //say << "*";
                        //for (std::string x : tmp) say << x;

                        return res;
                    } else {
                        tmp = Splitter::split(str, '/');
                        if (tmp.size() > 1) {
                            res = reduce(tmp[0],parser);
                            for (size_t i = 1; i < tmp.size(); ++i) res /= reduce(tmp[i],parser);
                            //say << "/";
                            //for (std::string x : tmp) say << x;

                            return res;
                        } else {
                            tmp = Splitter::split(str, '^');
                            if (tmp.size() > 1) {
                                res = reduce(tmp[0],parser);
                                for (size_t i = 1; i < tmp.size(); ++i) res = std::pow(res, reduce(tmp[i],parser));
                                //say << "/";
                                //for (std::string x : tmp) say << x;

                                return res;
                            } else {
                                if(unaryReduce(str,res,parser)) return res;
                                else {
                                    //atomic case
                                    return atomicReduce(str,parser);
                                }
                            }
                        }
                    }
                }
            }
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */


        static bool unaryReduce(std::string str, double& res, ParameterParser& parser){
            size_t tmp = str.find_first_of('(');
            if(tmp != std::string::npos){
                std::string copy = str;
                std::string op = str.erase(tmp);
                copy = copy.erase(0,tmp);
                if(op == "exp") res = exp(reduce(copy,parser));
                else if(op == "ln") res = log(reduce(copy,parser));
                else if(op == "cos") res = std::cos(reduce(copy,parser));
                else if(op == "sin") res = std::sin(reduce(copy,parser));
                else if(op == "abs") res = fabs(reduce(copy,parser));
                else if(op == "-") res = -reduce(copy,parser);
                else throw(UnsupportedUnaryOp("Operator " +op + " is not supported by the math parser."));
                return true;
            }
            else if (str.front() == '-')
            {
                res = -reduce(str.erase(0,0),parser);
                return true;
            }
            else return false;
        }

        static double atomicReduce(std::string str, ParameterParser& params)
        {
            if(str.front() != '\''){
                double res;
                std::istringstream iss(str);
                iss >> std::skipws >> res;
                return res;
            }
            else if(str.back() == '\'')
            {
                double res = params.get<double>(str.substr(1, str.size() - 2));
                return res;
            } else throw(AtomicExpressionNotValid("Your atomic expression " + str +" is not valid. It must be either a number, or a mandatory parameter protected by single quotes \'\'"));
        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/parameters/mathparser/reducer_test.h"
#endif


#endif
