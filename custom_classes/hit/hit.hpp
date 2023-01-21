#ifndef HIT_H
#define HIT_H

#include <Riostream.h>
#include <string>

#include <TObject.h>

#include "../pointCC/pointCC.hpp"


class Hit: public TObject, public PointCC
{
    public:
        Hit(): TObject(), PointCC(0., 0., 0.){};
        Hit(const double X, const double Y, const double Z):  TObject(), PointCC(X, Y, Z){};
        Hit(const Hit& hit): TObject(), PointCC(hit.getX(), hit.getY(), hit.getZ()){};
        ~Hit(){};

        void smearing();
        void noise(const double detectorRadius, const double detectorLenght);

        ClassDef(Hit, 1)

};

#endif
