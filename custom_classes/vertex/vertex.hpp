#ifndef VERTEX_H
#define VERTEX_H

#include "../hit/hit.hpp"
#include "../pointCC/pointCC.hpp"

/**
 * @brief Class for vertex points. A vertex is a point in cylindrical coordinated associated with the 
 * multiplicity of particle generated in that point.
 * 
 */
class Vertex: public PointCC
{
    public:
        Vertex(): PointCC(0., 0., 0.), fMultiplicity(0){};
        Vertex(const double X, const double Y, const double Z, const int multiplicity): PointCC(X, Y, Z), fMultiplicity(multiplicity){};
        Vertex(PointCC& point, const int multiplicity): PointCC(point), fMultiplicity(multiplicity) {};
        ~Vertex(){};

        int getMultiplicity() const {return fMultiplicity;};

    private:
        int fMultiplicity;          // particle multiplicity
};

#endif