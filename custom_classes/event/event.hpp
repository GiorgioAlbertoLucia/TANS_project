#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>

#include <TH1I.h>
#include <TH1F.h>
#include <TObject.h>
#include <TClonesArray.h>

#include "../hit/hit.hpp"
#include "../detector/detector.hpp"
#include "../vertex/vertex.hpp"
#include "../particle/particle.hpp"

/**
 * @brief Class to simulate an event 
 * 
 */
class Event: public TObject
{
    public:
        Event();
        virtual ~Event();

        inline Vertex getVertex() const {return fPrimaryVertex;};
        inline Particle getParticle(const int i) const {return fParticleArray[i];};

        Vertex partGeneration(TH1I& hMultiplicity, TH1F& hEta);   
        
        TClonesArray partTransportAndRecording(Detector& detector, const char * recordFile);
        TClonesArray partTransport(Detector& detector);
    
        void clear();

    private:
        Vertex fPrimaryVertex;                      // primary vertex of interaction
    
        Particle * fParticleArray;                  // [fParticleArraySize] array of particles created during the collision
        int fParticleArraySize;                     // size of the array of particles

        ClassDef(Event, 1)


};

#endif