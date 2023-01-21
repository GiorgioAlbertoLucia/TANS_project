#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
#include "../detector/detector.hpp"

class Particle
{
    public:
        Particle(): fPhi(0.), fEta(0.), fLastHP() {};
        Particle(const double Phi, const double Eta, Vertex& vertex);
        Particle(const Particle& particle);
        ~Particle();

        inline double getPhi() const {return fPhi;};
        inline double getEta() const {return fEta;};
        inline Hit getLastHP() const {return fLastHP;};

        inline double evalTheta() const {return 2. * atan(exp(-fEta));};

        Hit transport(Detector& detector);
        void multipleScattering();
        
        
    protected:
        void rotate(const double phiRot, const double thetaRot, double (&vec)[3]);

    private:
        double fPhi;
        double fEta;

        Hit fLastHP;  // last hit position

        

};

#endif