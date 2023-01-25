#ifndef POINTCC_H
#define POINTCC_H

#include <cmath>

/**
 * @brief point in a 3d space in cylindric coordinates
 * 
 */
class PointCC
{
    public:
        inline PointCC(): fR(0.), fPhi(0.), fZ(0.){};
        inline PointCC(const double R, const double Phi, const double Z): fR(R), fPhi(Phi), fZ(Z){};
        ~PointCC(){};

        inline double getRadius() const {return fR;};
        inline double getPhi() const {return fPhi;};
        inline double getZ() const {return fZ;};

        inline double getX() const {return (fR * cos(fPhi));};
        inline double getY() const {return (fR * sin(fPhi));};

        /**
         * @brief Vector addition
         * 
         * @param point 
         * @return PointCC 
         */
        //PointCC operator+(const PointCC& point);

    protected:
        double fR;
        double fPhi;
        double fZ;

};

#endif