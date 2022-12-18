#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>

#include <TObject.h>


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


        /**
         * @brief Generates primary vertex, multiplicity and fill a TClonesArray with generated particles
         * 
         * @param option1 option for multiplicity distr
         * @param option2 option for eta distr (part)
         */
        Vertex PartGeneration(/*const char * option1, const char * option2*/);   
        vector<Hit> PartTransport(Detector& detector);                 // arg -> detector


    private:
        Vertex fPrimaryVertex;                      // description for ROOT if this becomes a TObject - probably not necessary
        std::vector<Particle> fParticleArray;       //

        ClassDef(Event, 1)


};

#endif