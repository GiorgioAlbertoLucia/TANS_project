#include "event.hpp"

#include <TRandom3.h>
#include <TMath.h>

ClassImp(Event)

Event::Event():
    TObject(), 
    fPrimaryVertex(), 
    fParticleArray()
{
    fPrimaryVertex = Vertex(0., 0., 0., 0);
}

Event::~Event()
{

}

Vertex Event::PartGeneration(/*const char * option1, const char * option2*/)
{
    // to generate new particles, delete the old ones
    if (fPrimaryVertex.getMultiplicity() > 0)   fParticleArray.clear();      

    const int multiplicity = 4;         // from given distr
    fPrimaryVertex = Vertex(gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 5.3), multiplicity);

    fParticleArray.reserve(fPrimaryVertex.getMultiplicity());

    for (int i=0; i<fPrimaryVertex.getMultiplicity(); i++)
    {
        double eta = 0.;        // from given distribution
        fParticleArray.push_back(Particle(2.*TMath::Pi()*gRandom->Rndm(), eta));
    }   

    return fPrimaryVertex;
}

vector<Hit> Event::PartTransport(Detector& detector)
{
    vector<Hit> IPvec;
    IPvec.reserve(fPrimaryVertex.getMultiplicity());

    

    if (detector.multipleScattering)    
    {
        continue;
    }
}
