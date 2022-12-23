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

double PointCC::evalPhi()
{
    if(fX>0) return asin(fY/this->evalRadius());
    return (TMath::Pi()-asin(fY/this->evalRadius()));
}