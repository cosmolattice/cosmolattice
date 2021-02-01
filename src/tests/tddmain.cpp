/* File created by Wessel Valkenburg, 2019 */
/* Released under the MIT license, see LICENSE.md. */
/** \file The main function for the testing of a single unit testing. Link against all object files that you want to test. */

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/session/sessionguard.h"

int main (int argc, char* argv[] ) {
    TempLat::SessionGuard guard(argc, argv);
    return TempLat::TDDRegister::run();
};

