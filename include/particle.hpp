#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

#include <TObject.h>

class Particle: public TObject
{
    public:
        inline Particle(double Phi, double Eta): fPhi(Phi), fEta(Eta){};
        ~Particle();

    private:
        double fPhi;
        double fEta;

    ClassDef(Particle, 1);

};

#endif