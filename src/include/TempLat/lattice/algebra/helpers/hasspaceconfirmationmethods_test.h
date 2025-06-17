#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasSpaceConfirmationMethodsTester::Test(TempLat::TDDAssertion &tdd)
{
  static constexpr size_t NDim = 3;

  struct MyTestOne {
    void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateInterface<NDim>::SpaceType &sType)
    {
      std::cerr << "Hell yeah.\n";
    }
  };

  struct MyTestTwo {
    void notConfirmConfigSpace() { std::cerr << "Hell no.\n"; }
  };

  struct MyTestThree {
    void confirmSpace(Tag<3> i, const LayoutStruct<NDim> &newLayout, const SpaceStateInterface<NDim>::SpaceType &sType)
    {
      std::cerr << "Hell yeah.\n";
    }
  };

  tdd.verify(HasSpaceConfirmationMethods<MyTestOne, NDim> == true);
  tdd.verify(HasSpaceConfirmationMethods<MyTestTwo, NDim> == false);
  tdd.verify(HasSpaceConfirmationMethods<int, NDim> == false);
  tdd.verify(HasSpaceConfirmationMethodsIndexed<3, MyTestOne, NDim> == false);
  tdd.verify(HasSpaceConfirmationMethodsIndexed<3, MyTestThree, NDim> == true);
}

#endif
