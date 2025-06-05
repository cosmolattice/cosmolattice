//
// Created by franz on 6/5/25.
//

#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEX_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEX_H

#ifndef NOKOKKOS
#include <Kokkos_Complex.hpp>

namespace TempLat {
    template<typename T>
    using complex = Kokkos::complex<T>;
}

#else
#include <complex>

namespace TempLat {
    template<typename T>
    using complex = std::complex<T>;
}

#endif

#endif //COMPLEX_H
