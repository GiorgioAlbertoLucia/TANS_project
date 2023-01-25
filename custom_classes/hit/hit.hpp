#ifndef HIT_H
#define HIT_H

#include <Riostream.h>
#include <string>

#include <TObject.h>

#include "../pointCC/pointCC.hpp"
#include "../detector/detector.hpp"


class Hit: public TObject, public PointCC
{
    public:
        Hit(): TObject(), PointCC(0., 0., 0.){};
        Hit(const double R, const double Phi, const double Z):  TObject(), PointCC(R, Phi, Z){};
        Hit(const Hit& hit): TObject(), PointCC(hit.getRadius(), hit.getPhi(), hit.getZ()){};
        ~Hit(){};

        void smearing();
        void noise(Detector& detector);

        ClassDef(Hit, 1)

};

#endif
