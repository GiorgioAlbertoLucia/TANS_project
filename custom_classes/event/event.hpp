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
        Vertex fPrimaryVertex;                      // description for ROOT if this becomes a TObject - probably not necessary
    
        Particle * fParticleArray;
        int fParticleArraySize;

        ClassDef(Event, 1)


};

#endif