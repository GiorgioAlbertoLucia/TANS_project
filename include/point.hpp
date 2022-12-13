#ifndef POINT_H
#define POINT_H

#include <string>

#include <TObject.h>

class tPoint: public TObject
{
    public:
        inline tPoint(){};
        inline tPoint(double X, double Y, double Z): fX(X), fY(Y), fZ(Z){};
        ~tPoint(){};

    private:
        double fX;
        double fY;
        double fZ;

        //double phi;
        //double r;

    ClassDef(tPoint, 1);

};

#endif