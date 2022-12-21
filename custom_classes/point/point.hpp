#ifndef POINT_H
#define POINT_H

#include <cmath>

class Point
{
    public:
        inline Point(): fX(0.), fY(0.), fZ(0.){};
        inline Point(const double X, const double Y, const double Z): fX(X), fY(Y), fZ(Z){};
        ~Point(){};

        inline double getX(){return fX;};
        inline double getY(){return fY;};
        inline double getZ(){return fZ;};

        inline double evalRadius(){return (sqrt(fX*fX+fY*fY));};
        double evalPhi();
        /**
         * @brief Vector addition
         * 
         * @param point 
         * @return Point 
         */
        Point operator+(const Point& point);

        //void smearing();

    protected:
        double fX;
        double fY;
        double fZ;

};

#endif