#include "../include/event.hpp"
#include "../include/particle.hpp"

#include <TRandom3.h>
#include <TMath.h>

ClassImp(Event)

Event::Event():
    TObject(), 
    fPrimaryVertex(), 
    fParticleArray(NULL)
{
    fPrimaryVertex = {tPoint(0., 0., 0.), 0};
}

Event::~Event()
{
    if (fPrimaryVertex.multiplicity > 0)    delete fParticleArray;
}

void Event::PartGeneration(const char * option1, const char * option2)
{
    if (fPrimaryVertex.multiplicity > 0)    delete fParticleArray;      // to generate new particles, delete the old ones

    const int multiplicity = 4;         // from given distr
    fPrimaryVertex = {tPoint(gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 5.3)), multiplicity};

    fParticleArray = new TClonesArray("Particle", fPrimaryVertex.multiplicity);
    TClonesArray &PartArray = *fParticleArray;

    for (int i=0; i<fPrimaryVertex.multiplicity; i++)
    {
        double eta = 0.;        // from given distribution
        new (PartArray[i]) Particle(2.*TMath::Pi()*gRandom->Rndm(), eta);
    }   
}