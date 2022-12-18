#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"

class Particle
{
    public:
        Particle();
        Particle(const double Phi, const double Eta, Vertex vertex);
        ~Particle();

        double getPhi() const {return fPhi;};
        double getEta() const {return fEta;};

        Hit transport(Detector& detector);
        /**
         * @brief Update last hit position with given coordinates
         * 
         * @param X 
         * @param Y 
         * @param Z 
         * @param updateLayer choose if it should update the last layer index
         * @return * void 
         */
        void updateHitPos(const double X, const double Y, const double Z, const bool updateLayer=true);

    private:
        double fPhi;
        double fEta;

        Hit fLastHP;  // last hit position

        

};

#endif