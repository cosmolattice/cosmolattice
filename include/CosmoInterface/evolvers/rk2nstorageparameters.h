#ifndef COSMOINTERFACE_EVOLVERS_RK2NSTORAGEPARAMETERS_H
#define COSMOINTERFACE_EVOLVERS_RK2NSTORAGEPARAMETERS_H
/* File created by Adrien Florio, 2024 */
/* Released under the MIT license, see LICENSE.md. */

#include "CosmoInterface/evolvers/evolvertype.h"

namespace TempLat {


    /** \brief A class which stores coefficients for 2N-storage RK.
     *
     *
     **/
     // References:
     // - RK3_3:  https://arxiv.org/pdf/2101.05320
     // - RK3_4:  https://ntrs.nasa.gov/api/citations/19940029698/downloads/19940029698.pdf .
     // - RK4_5:  https://arxiv.org/pdf/2101.05320 RK4CK

    template<typename T>
    class RK2NStorageParameters {
    public:
        /* Put public methods here. These should change very little over time. */
        RK2NStorageParameters() = default;

        static std::vector<T> getAs(EvolverType eType){
            std::vector<T> res;

            if (eType == RK2) res = {0, -0.5};
            else if(eType == RK3_3) res = {0, -5.0/9.0, -153.0/128.0};
            else if(eType == RK3_4) res = {0, -0.7825460361923583, -2.042914325731225, -1.799337253940777};
            else if (eType == RK4_5) res = {0, -567301805773.0/1357537059087.0, -2404267990393.0/2016746695238.0, -3550918686646.0/2091501179385.0,  -1275806237668.0/842570457699.0};

            return res;
        }

        static std::vector<T> getBs(EvolverType eType){
            std::vector<T> res;

            if (eType == RK2) res = {0.5, 1.0};
            else if(eType == RK3_3) res = {1.0/3.0, 15.0/16.0, 8.0/15.0};
            else if(eType == RK3_4) res = {0.06688758201974097, 2.876554598956719, 0.5534657361343982, 0.3912730180961791};
            else if (eType == RK4_5) res = {1432997174477.0/9575080441755.0,5161836677717.0/13612068292357.0,1720146321549.0/2090206949498.0,3134564353537.0/4481467310338.0, 2277821191437.0/14882151754819.0};

            return res;
        }

        static std::vector<T> getCs(EvolverType eType){
            std::vector<T> res;

            if (eType == RK2) res = {0.0, 0.5};
            else if(eType == RK3_3) res = {0.0,1.0/3.0,3.0/4.0};
            else if(eType == RK3_4) res = {0.0,0.06688758201974097,0.6924057816719806,1.0};
            else if (eType == RK4_5) res = {0.0,1432997174477.0/9575080441755.0,0.37040095736420475,0.6222557631344432,0.9582821306746903};

            return res;
        }

        static size_t order(EvolverType eType){
            size_t res = 2;
            if(eType == RK3_3) res = 3;
            if(eType == RK3_4) res = 3;
            if(eType == RK4_5) res = 4;
            return res;
        }


        static bool isRK2n(EvolverType ev){
            bool res = false;
            if(ev == RK2 or ev == RK3_3 or ev == RK3_4 or ev == RK4_5) res = true;
            return res;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
    };

} /* TempLat */

#endif
