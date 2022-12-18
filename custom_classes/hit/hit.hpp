#ifndef HIT_H
#define HIT_H

#include <string>

#include <TObject.h>

#include "../point/point.hpp"

class Hit: public TObject, public Point
{
    public:
        inline Hit(): TObject(), Point(0., 0., 0.), fHitLayer(0){};
        inline Hit(const double X, const double Y, const double Z, const int IL): 
            TObject(), Point(X, Y, Z), fHitLayer(IL){};
        ~Hit(){};

        inline int getHitLayer(){return fHitLayer;};

        //void smearing();

    private:
        int fHitLayer;

        //double phi;
        //double r;

        ClassDef(Hit, 1)

};

#endif