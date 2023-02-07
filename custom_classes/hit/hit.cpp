#include <TMath.h>
#include <TRandom3.h>

#include "hit.hpp"
#include "../detector/detector.hpp"

#ifdef MAKECINT
#include <vector>
#pragma link C++ class vector<Hit>+;
#endif

ClassImp(Hit)

/**
 * @brief Adds smearing to the hit (changes the Z and Phi coordinate with random values from a gaussian distribution
 * with mean = 0.).
 * 
 */
void Hit::smearing()
{
    fZ += gRandom->Gaus(0., 0.0120);
    fPhi += gRandom->Gaus(0.,0.003)/fR;
}

/**
 * @brief Initializes the hit as generated by noise in a cylindrical detector of given radius. 
 * (z coordinate is generated from a uniform distribution on the detector lenght, 
 * the azimuthal angle is generated from a uniform distribution in [0, 2π])
 * 
 * @param detector detector on which noise effects generate a hit 
 */
void Hit::noise(Detector& detector)
{
    fZ = gRandom->Rndm()*detector.lenght-(detector.lenght/2.);
    fPhi = gRandom->Rndm()*2*TMath::Pi();
    fR = detector.radius - detector.width/2.;
}
