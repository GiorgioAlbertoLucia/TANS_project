#ifndef HIT_H
#define HIT_H

#include <string>

#include <TObject.h>

#include "../point/point.hpp"

class Hit: public TObject, public Point
{
    public:
        Hit(): TObject(), Point(0., 0., 0.), fHitLayer(0){};
        Hit(const double X, const double Y, const double Z, const int IL):  
            TObject(), Point(X, Y, Z), fHitLayer(IL){};
         ~Hit(){};

        inline int getHitLayer(){return fHitLayer;};
        inline double getPhi(){return fPhi;};

        void smearing();
      

    private:
        int fHitLayer;      // layer 
        double fPhi;    // get in point
        //double rad;    // get in point
        

        ClassDef(Hit, 1)

};

#endif