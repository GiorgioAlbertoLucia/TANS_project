#include <Riostream.h>
#include <fstream>

#include <TH1I.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TMath.h>

#include "event.hpp"
#include "../recorder/recorder.hpp"

ClassImp(Event)

/*   PRIVATE   */

Event::Event():
    TObject()
{
    fPrimaryVertex = Vertex(0.,0.,0.,0);
}

Event::~Event()
{

}

/**
 * @brief Generates primary vertex, multiplicity and fill a TClonesArray with generated particles
 * 
 * @param hMultiplicity multiplicity distribution
 * @param hEta eta distribution (pseudorapidity)
 */
Vertex Event::partGeneration(TH1I& hMultiplicity, TH1F& hEta)
{
    // to generate new particles, delete the old ones
    if (fPrimaryVertex.getMultiplicity() > 0)   fParticleArray.clear();      

    const int multiplicity = hMultiplicity.GetRandom();         
    //const int multiplicity = int(gRandom->Rndm()*10);
    fPrimaryVertex = Vertex(gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 5.3), multiplicity);

    fParticleArray.reserve(fPrimaryVertex.getMultiplicity());

    for (int i=0; i<fPrimaryVertex.getMultiplicity(); i++)
    {
        double eta = (double)hEta.GetRandom();        // from given distribution
        fParticleArray.push_back(Particle(2.*TMath::Pi()*gRandom->Rndm(), eta, fPrimaryVertex));
    }   

    return fPrimaryVertex;
}

/**
 * @brief Executes particle transport through a given detector. Writes position of all hits if record is true
 * (this can be used to create 3D models with tracks). Returns a TClonesArray of hits.
 * @param detector 
 * @param record 
 * @param recordFile 
 * @return TClonesArray 
 */
TClonesArray Event::partTransport(Detector& detector, bool rec, string recordFile)
{
    const int multiplicity = fPrimaryVertex.getMultiplicity();

    TClonesArray * ptrhits = new TClonesArray("Hit", multiplicity);
    TClonesArray &hits = * ptrhits;

    double recordArray[multiplicity][3];    // array to record hits in a .txt file

    for(int i=0; i<multiplicity; i++)
    {
        new (hits[i]) Hit();
        Hit * hit = (Hit*)ptrhits->At(i);
        * hit = fParticleArray[i].transport(detector);

        if(rec)
        {
            recordArray[i][0] = hit->getX();
            recordArray[i][1] = hit->getY();
            recordArray[i][2] = hit->getZ();
        }
    }

    if(rec)  
    {
        Recorder * recorder = Recorder::getInstance(recordFile.c_str());
        recorder->recordTracks(recordArray, multiplicity);
        recorder->destroy();
    }

    if (detector.multipleScattering)    for (Particle& part: fParticleArray)     part.multipleScattering();

    return hits;
}


/**
 * @brief Clears Particle Array and sets Primary Vertex to (0., 0., 0., 0)
 * 
 */
void Event::clear()
{
    fPrimaryVertex = Vertex(0.,0.,0.,0);
    fParticleArray.clear(); 
}
