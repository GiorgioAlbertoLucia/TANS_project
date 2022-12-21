#include "hit.hpp"
#include <TMath.h>
#include <TRandom3.h>

ClassImp(Hit)


void Hit::smearing()
{
    fZ += gRandom->Gaus(0., 0.0120);
    double phism = this->evalPhi()+gRandom->Gaus(0.,0.003)/this->evalRadius();
}
