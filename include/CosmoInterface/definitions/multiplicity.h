#ifndef COSMOINTERFACE_DEFINITIONS_MULTIPLICITY_H
#define COSMOINTERFACE_DEFINITIONS_MULTIPLICITY_H

#include <vector>
#include <cmath>
#include <cstddef>
#include <utility>

namespace TempLat {

    template <typename T, size_t NDim>
    class MultiplicityHelper {
    public:
        MultiplicityHelper(const ptrdiff_t& nGrid, std::vector<int> multi)
            : mnGrid(nGrid), mmulti(std::move(multi))
        { }

        T eval(const size_t& i, const size_t& j, const size_t& k) const
        {
            const size_t half = static_cast<size_t>(mnGrid / 2);

            auto fold = [half, this](size_t x) -> size_t {
                return (x <= half) ? x : static_cast<size_t>(mnGrid) - x;
            };

            const size_t ii = fold(i);
            const size_t jj = fold(j);
            const size_t kk = fold(k);

            T pair = T(1);


            if (ii != 0 || jj != 0 || kk != 0) {
                const auto n2 = ii * ii + jj * jj + kk * kk;
                const int bin = int(std::sqrt(T(n2)) + T(0.5)) - 1;
                pair = T(mmulti[bin]);
            }

            if (ii == half && jj == half && kk == half) {
                pair = T(8);
            }

            return T(1) / pair;
    }

    private:
        ptrdiff_t mnGrid;
        std::vector<int> mmulti;
    };

} /* TempLat */

#endif