#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include "../intersectionPoint/intersectionPoint.hpp"
#include "../include/vertex.hpp"

class Particle
{
    public:
        Particle();
        Particle(const double Phi, const double Eta, Vertex vertex);
        ~Particle();

        double getPhi() const {return fPhi;};
        double getEta() const {return fEta;};

        void updateIntPoint(const double X, const double Y, const double Z);

    private:
        double fPhi;
        double fEta;

        IntersectionPoint fLastIP;  // 

        

};

#endif