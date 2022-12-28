#include "hit.hpp"
#include <TMath.h>
#include <TRandom3.h>


ClassImp(Hit)


void Hit::smearing()
{
    fZ += gRandom->Gaus(0., 0.0120);
    fPhi = this->evalPhi()+gRandom->Gaus(0.,0.003)/this->evalRadius();
}

void Hit::noise()
{
    fZ=gRandom->Rndm()*27.;
    fPhi=gRandom->Rndm()*2*TMath::Pi();
    double aaa=gRandom->Rndm();         // TE LI CAMBIERÒ TUTTI STI NOMI, ma sono giustiiiiiii
    if(aaa<0.5)  fHitLayer=1;
    else  fHitLayer=2;
}
