//
// Created by franz on 6/4/25.
//

#ifndef SCALARVIEW_H
#define SCALARVIEW_H

#include "TempLat/parallel/kokkos/kokkos.h"
#include <memory>

template<typename T
#ifndef NOKOKKOS
    , typename ExecutionSpace = Kokkos::DefaultExecutionSpace
#endif
>
class ScalarView {
public:
private:
#ifndef NOKOKKOS
    Kokkos::View<T, ExecutionSpace> data;
#else
    std::shared_ptr<T> data;
#endif
};

template<typename T, typename... Args>
ScalarView<T> make_view(Args&&... args) {

}

#endif //SCALARVIEW_H
