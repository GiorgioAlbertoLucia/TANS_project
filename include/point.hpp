#ifndef POINT_H
#define POINT_H

class Point:
{
    public:
        inline Point(): fX(0.), fY(0.), fZ(0.){};
        inline Point(const double X, const double Y, const double Z): fX(X), fY(Y), fZ(Z){};
        ~Point(){};

        inline double getX(){return fX;};
        inline double getY(){return fY;};
        inline double getZ(){return fZ;};

        Point operator+(const Point& point);

        //void smearing();

    private:
        double fX;
        double fY;
        double fZ;

};

#endif