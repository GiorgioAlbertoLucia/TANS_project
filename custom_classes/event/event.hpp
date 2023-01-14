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
        
        vector<Hit> partTransport(Detector& detector);  
        TClonesArray partTransport2(Detector& detector, bool record, string filePath = "");

        /**
         * @brief clears fParticleArray and sets fPrimaryVertex to (0., 0., 0., 0)
         * 
         */
        void clear();

    protected:
        void recordTracks(const char * filePath, double recordArray[][3], const int multiplicity) const;


    private:
        Vertex fPrimaryVertex;                      // description for ROOT if this becomes a TObject - probably not necessary
        std::vector<Particle> fParticleArray;       //

        ClassDef(Event, 1)


};

#endif