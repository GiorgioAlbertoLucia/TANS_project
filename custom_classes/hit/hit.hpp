#ifndef HIT_H
#define HIT_H

#include <Riostream.h>
#include <string>

#include <TObject.h>

#include "../pointCC/pointCC.hpp"
//

class Hit: public TObject, public PointCC
{
    public:
        Hit(): TObject(), PointCC(0., 0., 0.), fHitLayer(0){};
        Hit(const double X, const double Y, const double Z, const int IL):  
            TObject(), PointCC(X, Y, Z), fHitLayer(IL), fPhi(this->evalPhi()){};
        Hit(const Hit& hit): TObject(), PointCC(hit.getX(), hit.getY(), hit.getZ()), fHitLayer(hit.fHitLayer), fPhi(hit.fPhi){};
        ~Hit(){};

        inline int getHitLayer() const {return fHitLayer;};
        inline double getPhi() const {return fPhi;};

        void smearing();
        void noise();
      

    private:
        int fHitLayer;      // layer 
        double fPhi;        // azimuthal angle of interaction from primary vertex

        ClassDef(Hit, 1)

};

#endif


#ifdef MAKECINT
#include <vector>
#pragma link C++ class vector<Hit>+;
#endif