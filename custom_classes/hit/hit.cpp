#include "hit.hpp"
#include <TMath.h>
#include <TRandom3.h>


ClassImp(Hit)


void Hit::smearing()
{
    fZ += gRandom->Gaus(0., 0.0120);
    fPhi += gRandom->Gaus(0.,0.003)/this->evalRadius();
}

void Hit::noise()
{
    fZ=gRandom->Rndm()*27-13.5;
    fPhi=gRandom->Rndm()*2*TMath::Pi();
    
}
