#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_FINITEDIFFVSFFT_TEST_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_FINITEDIFFVSFFT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

using namespace TempLat;

template<ptrdiff_t nDim>
void myLittleLambda(TempLat::TDDAssertion& tdd, ptrdiff_t nGrid, ptrdiff_t nGhost, double prec = 1e-10) {

    auto toolBox = MemoryToolBox::makeShared(nDim, nGrid, nGhost);

    double dx = 1. / nGrid;

    double pi = Constants::pi<double>;

    /* create the random field once, keep in memory. Trade off between RAM use and redundant computations. */
    Field<double> gaussian("gaussian", toolBox);
    gaussian.inFourierSpace() = 1* RandomGaussianField<double>("hoi",toolBox) ;


    WaveNumber k(toolBox);

    /* first the Fourier versions */
    std::vector<Field<double>> gradientFFT;

    for ( ptrdiff_t i = 0; i < nDim; ++i ) {
        gradientFFT.emplace_back("fft_grad_" + std::to_string(i), toolBox);
        // You wish... FFT gradient is k phi_k, but that is not what we should compare to.
        // Finite difference in configuration space has FFT (1 - exp(i k)) phi)k.
        //gradientFFT.back().inFourierSpace() = std::complex<double>(0, 1) * k[i]/nGrid * delta_x * gaussian.inFourierSpace();
        gradientFFT.back().inFourierSpace() = (exp(std::complex<double>(0, 1) * k[i] * 2 * pi * dx) - 1.) / dx * gaussian.inFourierSpace();
        gradientFFT.back().getMemoryManager()->confirmConfigSpace();
    }




    /* continue using gaussian, let it go to configuration space. */


    std::vector<Field<double>> gradientFiniteDiff;

    for ( ptrdiff_t i = 0; i < nDim; ++i ) {

        //gaussian.getMemoryManager()->confirmConfigSpace();
        gradientFiniteDiff.emplace_back("fd_grad_" + std::to_string(i), toolBox);
        gradientFiniteDiff.back() = LatForwardGrad<nDim>(gaussian)[i] / dx;
    }


    /** don't bother with atomic, if it gets set by anyone, it will be to false. No racing. */
    std::vector<bool> gradientAllEqual(nDim, true);


    auto it = toolBox->itX();

    for(it.begin(); it.end(); ++it){
        bool thisResult = true;
        for ( ptrdiff_t i = 0; i < nDim; ++i) {
            bool thisResultPart =  AlmostEqual( gradientFiniteDiff[i].get(it()) , gradientFFT[i].get(it()), prec);

            if ( ! thisResultPart ){
              auto coords = toolBox->getCoordConfiguration(it());
              std::string coordsStr("(");
              for(size_t j = 0; j<coords.size() ; ++j) coordsStr+=std::to_string(coords[j])+" ";
              coordsStr+=")";

              say << "grad " << i <<", at " << coordsStr << ": " << gradientFiniteDiff[i].get(it()) << " vs " << gradientFFT[i].get(it()) << "\n";
            }

            thisResult = thisResult && thisResultPart;
            if ( ! thisResultPart ) gradientAllEqual[i] = false;
        }
      }

    for ( ptrdiff_t i = 0; i < nDim; ++i) {
        tdd.verify( toolBox->mGroup.getBaseComm().computeAllAnd((bool) gradientAllEqual[i]) );
    }
}



inline void TempLat::FiniteDiffVSFFT::Test(TempLat::TDDAssertion& tdd) {

    myLittleLambda<3>(tdd, 32, 1); //Template the number of dims for gradients
    //myLittleLambda<5>(tdd, 32, 1, 1e-6); // Can do 5D field theory, fun! Lose a bit of precision here, mostly becuse you get small numbers and huge volume.
                                           // Commented cause requires a lot of RAM.


}

#endif
