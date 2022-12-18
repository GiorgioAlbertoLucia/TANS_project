#ifndef VERTEX_H
#define VERTEX_H

#include "../hit/hit.hpp"
#include "../point/point.hpp"

/**
 * @brief Class for vertex points. 
 * 
 */
class Vertex: public Point
{
    
    public:
        Vertex(): Point(0., 0., 0.), fMultiplicity(0){};
        Vertex(double X, double Y, double Z, int multiplicity): Point(X, Y, Z), fMultiplicity(multiplicity){};
        ~Vertex(){};

        int getMultiplicity() const {return fMultiplicity;};

        //void VertexReconstruction(TBranch1, 2, 3);

    protected:
        //Retta(Punto1, 2) ? -> // returns z coord. of the vertex from two points

    private:
        int fMultiplicity;
};

#endif