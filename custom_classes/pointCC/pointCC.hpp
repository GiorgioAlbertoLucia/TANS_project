#ifndef POINTCC_H
#define POINTCC_H

#include <cmath>

/**
 * @brief point in a 3d space in cartesian coordinates
 * 
 */
class PointCC
{
    public:
        inline PointCC(): fX(0.), fY(0.), fZ(0.){};
        inline PointCC(const double X, const double Y, const double Z): fX(X), fY(Y), fZ(Z){};
        ~PointCC(){};

        inline double getX() const {return fX;};
        inline double getY() const {return fY;};
        inline double getZ() const {return fZ;};

        inline double evalRadius(){return (sqrt(fX*fX+fY*fY));};
        double evalPhi();
        /**
         * @brief Vector addition
         * 
         * @param point 
         * @return PointCC 
         */
        PointCC operator+(const PointCC& point);

        //void smearing();

    protected:
        double fX;
        double fY;
        double fZ;

};

#endif