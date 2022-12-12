#ifndef EVENT_H
#define EVENT_H

#include <string>

#include <TObject.h>
#include <TClonesArray.h>

#include "point.hpp"

typedef struct
{
    Point x;
    int multiplicity;
} Vertex;


class Event: public TObject
{
    public:
        Event();
        virtual ~Event();

        void PartGeneration(const char * option);
         PartTransport();

    private:
        Vertex fPrimaryVertex;              // description for ROOT if this becomes a TObject - probably not necessary
        TClonesArray fParticleArray;        //

    Event("Event", 1);


};

#endif