#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
#include "../detector/detector.hpp"

class Particle
{
    public:
        Particle();
        /**
        * @brief Whenever a particle is created, its last intersection point is considered to be the vertex position
        * 
        * @param Phi 
        * @param Eta 
        * @param vertex 
        */
        Particle(const double Phi, const double Eta, Vertex& vertex);
        ~Particle();

        double getPhi() const {return fPhi;};
        double getEta() const {return fEta;};
        Hit getLastHP() const {return *fLastHP;};

        /**
        * @brief Creates a Hit (position and layer) of this particle on given detector assuming the particle will
        * move along a straight line from its starting position (in fLastHP)
        * 
        * @param detector 
        * @return Hit 
        */
        Hit transport(Detector& detector);
        /**
         * @brief multiple scattering through a detector. In the simplest approximation, only the direction of the particle 
         * will be updated.
         * 
         * @param detector 
         */
        void multipleScattering(Detector& detector);
        
        
    protected:
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

        Hit* fLastHP;  // last hit position

        

};

#endif