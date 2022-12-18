#include "../include/point.hpp"

Point Point::operator+(Point& point)
{
    fX += point.fX;
    fY += point.fY;
    fZ += point.fZ;
}