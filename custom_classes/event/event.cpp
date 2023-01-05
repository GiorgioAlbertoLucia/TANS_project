#include <Riostream.h>

#include <TH1I.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TMath.h>

#include "event.hpp"

ClassImp(Event)

Event::Event():
    TObject()
{
    std::vector<Particle> temp;
    fParticleArray = temp;
    fPrimaryVertex = Vertex(0.,0.,0.,0);
}

Event::~Event()
{

}

// ADD CUSTOM DISTR
Vertex Event::partGeneration(TH1I& hMultiplicity, TH1F& hEta)
{
    // to generate new particles, delete the old ones
    if (fPrimaryVertex.getMultiplicity() > 0)   fParticleArray.clear();      

    //const int multiplicity = hMultiplicity.GetRandom();         // from given distr
    const int multiplicity = 4;
    fPrimaryVertex = Vertex(gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 5.3), multiplicity);

    fParticleArray.reserve(fPrimaryVertex.getMultiplicity());

    for (int i=0; i<fPrimaryVertex.getMultiplicity(); i++)
    {
        double eta = (double)hEta.GetRandom();        // from given distribution
        fParticleArray.push_back(Particle(2.*TMath::Pi()*gRandom->Rndm(), eta, fPrimaryVertex));
    }   

    return fPrimaryVertex;
}

// ADD MULTIPLE SCATTERING
vector<Hit> Event::partTransport(Detector& detector)
{
    vector<Hit> IPvec;
    IPvec.reserve(fPrimaryVertex.getMultiplicity());

    for (Particle& part: fParticleArray)    IPvec.push_back(part.transport(detector));

    if (detector.multipleScattering)    for (Particle& part: fParticleArray)     part.multipleScattering();

    return IPvec;
}

TClonesArray Event::partTransport2(Detector& detector)
{
    TClonesArray * ptrhits = new TClonesArray("Hit", fPrimaryVertex.getMultiplicity());
    TClonesArray &hits = * ptrhits;

    for(int i=0; i<fPrimaryVertex.getMultiplicity(); i++)
    {
        new (hits[i]) Hit();
        Hit * hit = (Hit*)ptrhits->At(i);
        * hit = fParticleArray[i].transport(detector);
        
        // check cout
        // cout << "event " << hit->getX() << ", " << hit->getY() << ", " << hit->getZ() << endl;
    }   

    if (detector.multipleScattering)    for (Particle& part: fParticleArray)     part.multipleScattering();

    return hits;
}

void Event::clear()
{
    fPrimaryVertex = Vertex(0.,0.,0.,0);
    fParticleArray.clear(); 
}
