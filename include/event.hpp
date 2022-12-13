#ifndef EVENT_H
#define EVENT_H

#include <string>

#include <TObject.h>
#include <TClonesArray.h>

#include "point.hpp"

typedef struct
{
    tPoint x;
    int multiplicity;
} Vertex;


class Event: public TObject
{
    public:
        Event();
        virtual ~Event();

        /**
         * @brief Generates primary vertex, multiplicity and fill a TClonesArray with generated particles
         * 
         * @param option1 option for multiplicity distr
         * @param option2 option for eta distr (part)
         */
        void PartGeneration(const char * option1, const char * option2);   
        //TClonesArray PartTransport();               // arg -> detector (class)

    private:
        Vertex fPrimaryVertex;              // description for ROOT if this becomes a TObject - probably not necessary
        TClonesArray* fParticleArray;       //

    ClassDef(Event, 1);


};

#endif