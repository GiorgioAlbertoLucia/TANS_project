#include "pointCC.hpp"

#include <TMath.h>

PointCC PointCC::operator+(const PointCC& point)
{
    PointCC temp;
    temp.fX = fX + point.fX;
    temp.fY = fY + point.fY;
    temp.fZ = fZ + point.fZ;
    return temp;
}

double PointCC::getPhi()
{
   
         if(fY>0)    return acos(fX/this->evalRadius());
    return (2. * TMath::Pi() - acos(fX/this->evalRadius()));

}