#include "point.hpp"

#include <TMath.h>

Point Point::operator+(const Point& point)
{
    Point temp;
    temp.fX = fX + point.fX;
    temp.fY = fY + point.fY;
    temp.fZ = fZ + point.fZ;
    return temp;
}

double Point::evalPhi()
{
    if(fX>0) return asin(fY/this->evalRadius());
    return (TMath::Pi()-asin(fY/this->evalRadius()));
}