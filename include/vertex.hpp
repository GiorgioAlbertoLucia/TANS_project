#ifndef VERTEX_H
#define VERTEX_H

#include "intersectionPoint.hpp"

/**
 * @brief Class for vertex points. 
 * 
 */
class Vertex
{
    
    public:
        Vertex(){};
        Vertex(double X, double Y, double Z, int multiplicity): fX(X), fY(Y), fZ(Z), fMultiplicity(multiplicity){};
        ~Vertex(){};

        double getX() const {return fX;};
        double getY() const {return fY;};
        double getZ() const {return fZ;};
        int getMultiplicity() const {return fMultiplicity;};

        //void VertexReconstruction(TBranch1, 2, 3);

    protected:
        //Retta(Punto1, 2) ? -> // returns z coord. of the vertex from two points

    private:
        double fX;
        double fY;
        double fZ;
        int fMultiplicity;
};

#endif