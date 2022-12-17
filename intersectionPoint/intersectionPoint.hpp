#ifndef INTERSECTIONPOINT_H
#define INTERSECTIONPOINT_H

#include <string>

#include <TObject.h>

#include "../include/point.hpp"

class IntersectionPoint: public TObject, public Point
{
    public:
        inline IntersectionPoint(): TObject(), Point(0., 0., 0.), fIntersectionLayer(0){};
        inline IntersectionPoint(const double X, const double Y, const double Z, const int IL): 
            TObject(), Point(X, Y, Z), fIntersectionLayer(IL){};
        ~IntersectionPoint(){};

        inline int getIntersectionLayer(){return fIntersectionLayer;};

        //void smearing();

    private:
        int fIntersectionLayer;

        //double phi;
        //double r;

    ClassDef(IntersectionPoint, 1);

};

#endif