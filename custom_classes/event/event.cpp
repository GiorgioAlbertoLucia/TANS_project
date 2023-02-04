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

Event::Event(): TObject(), fParticleArray(NULL), fParticleArraySize(0)
{
    fPrimaryVertex = Vertex(0.,0.,0.,0);
}

Event::~Event()
{
    if(fParticleArraySize > 0)  delete []fParticleArray;
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
    //if (fPrimaryVertex.getMultiplicity() > 0)   fParticleArray.clear();      
    if(fParticleArraySize > 0)  delete []fParticleArray;

    const int multiplicity = hMultiplicity.GetRandom();         
    //const int multiplicity = 1.;
    fPrimaryVertex = Vertex(gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 5.3), multiplicity);

    fParticleArraySize = multiplicity;
    fParticleArray = new Particle[fParticleArraySize];
    //fParticleArray.reserve(fPrimaryVertex.getMultiplicity());

    for (int i=0; i<fPrimaryVertex.getMultiplicity(); i++)
    {
        double eta = (double)hEta.GetRandom();        // from given distribution
        fParticleArray[i] = Particle(2.*TMath::Pi()*gRandom->Rndm(), eta, fPrimaryVertex);
        //fParticleArray.push_back(Particle(2.*TMath::Pi()*gRandom->Rndm(), eta, fPrimaryVertex));
    }   

    return fPrimaryVertex;
}

/**
 * @brief Executes particle transport through a given detector. Returns a TClonesArray of hits.
 * @param detector 
 * @param record 
 * @param recordFile 
 * @return TClonesArray 
 */
//TClonesArray Event::partTransport(const double detectorRadius, const bool multipleScattering)
TClonesArray Event::partTransport(Detector& detector)
{
    const int multiplicity = fPrimaryVertex.getMultiplicity();

    TClonesArray hits = TClonesArray("Hit", multiplicity);
    //TClonesArray * ptrhits = new TClonesArray("Hit", multiplicity);
    //TClonesArray &hits = * ptrhits;

    for(int i=0; i<multiplicity; i++)
    {
        Hit * hit = (Hit*)hits.ConstructedAt(i);
        //new (hits[i]) Hit();
        //Hit * hit = (Hit*)ptrhits->At(i);
        * hit = fParticleArray[i].transport(detector.radius);
        //fParticleArray[i].transport(detectorRadius, *hit);
    }

    if (detector.multipleScattering)    for (int i=0; i<fParticleArraySize; i++)     fParticleArray[i].multipleScattering();
    //if (detector.multipleScattering)    for (Particle& part: fParticleArray)     part.multipleScattering();
    //if (multipleScattering)    for (Particle& part: fParticleArray)     part.multipleScattering();

    return hits;
}

/**
 * @brief Executes particle transport through a given detector. Writes position of all hits
 * (this can be used to create 3D models with tracks). Returns a TClonesArray of hits.
 * @param detector 
 * @param record 
 * @param recordFile 
 * @return TClonesArray 
 */
//TClonesArray Event::partTransportAndRecording(const double detectorRadius, const bool multipleScattering, const char * recordFile)
TClonesArray Event::partTransportAndRecording(Detector& detector, const char * recordFile)
{
    const int multiplicity = fPrimaryVertex.getMultiplicity();

    //TClonesArray * ptrhits = new TClonesArray("Hit", multiplicity);
    //TClonesArray &hits = * ptrhits;
    TClonesArray hits = TClonesArray("Hit", multiplicity);

    double recordArray[multiplicity][3];    // array to record hits in a .txt file

    for(int i=0; i<multiplicity; i++)
    {
        Hit * hit = (Hit*)hits.ConstructedAt(i);
        //new (hits[i]) Hit();
        //Hit * hit = (Hit*)ptrhits->At(i);
        * hit = fParticleArray[i].transport(detector.radius);
        //fParticleArray[i].transport(detectorRadius, *hit);

        recordArray[i][0] = hit->getX();
        recordArray[i][1] = hit->getY();
        recordArray[i][2] = hit->getZ();
    }

    Recorder * recorder = Recorder::getInstance(recordFile);
    recorder->recordTracks(recordArray, multiplicity);
    recorder->destroy();

    if (detector.multipleScattering)    for (int i=0; i<fParticleArraySize; i++)     fParticleArray[i].multipleScattering();
    //if (detector.multipleScattering)    for (Particle& part: fParticleArray)     part.multipleScattering();
    //if (multipleScattering)    for (Particle& part: fParticleArray)     part.multipleScattering();

    return hits;
}


/**
 * @brief Clears Particle Array and sets Primary Vertex to (0., 0., 0., 0)
 * 
 */
void Event::clear()
{
    fPrimaryVertex = Vertex(0.,0.,0.,0);
    //fParticleArray.clear(); 
    if(fParticleArraySize > 0)  delete []fParticleArray;
    fParticleArraySize = 0;
}
