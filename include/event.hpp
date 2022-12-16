#ifndef EVENT_H
#define EVENT_H

#include <string>

#include <TObject.h>
#include <TClonesArray.h>

#include "point.hpp"
#include "detector.hpp"
#include "vertex.hpp"


class Event: public TObject
{
    public:
        Event();
        virtual ~Event();

        Vertex getVertex() const {return fPrimaryVertex;};
        TClonesArray getParticleArray() const {return fParticleArray;};


        /**
         * @brief Generates primary vertex, multiplicity and fill a TClonesArray with generated particles
         * 
         * @param option1 option for multiplicity distr
         * @param option2 option for eta distr (part)
         */
        Vertex PartGeneration(const char * option1, const char * option2);   
        //TClonesArray PartTransport(Detector& detector);                 // arg -> detector


    private:
        Vertex fPrimaryVertex;              // description for ROOT if this becomes a TObject - probably not necessary
        TClonesArray* fParticleArray;       //

    ClassDef(Event, 1);


};

#endif