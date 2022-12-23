#include <Riostream.h>

#include "particle.hpp"

/* PROTECTED */

void Particle::updateHitPos(const double X, const double Y, const double Z, const bool updateLayer)
{
    int layer = 0;
    if (updateLayer)    layer = fLastHP.getHitLayer() + 1;
    else                layer = fLastHP.getHitLayer();

    fLastHP = Hit(X, Y, Z, layer);
}

/* PUBLIC */

Particle::Particle():
fPhi(0.),
fEta(0.),
fLastHP()
{

}

Particle::Particle(const double Phi, const double Eta, Vertex& vertex): 
fPhi(Phi), 
fEta(Eta)
{
    fLastHP = Hit(vertex.getX(), vertex.getY(), vertex.getZ(), -1);
}

Particle::Particle(const Particle& particle):
fPhi(particle.fPhi),
fEta(particle.fEta),
fLastHP(particle.fLastHP)
{
    
}

Particle::~Particle()
{

}

Hit Particle::transport(Detector& detector)
{
    double theta = 2. * atan(exp(-fEta));
    double c1 = sin(theta) * cos(fPhi);
    double c2 = sin(theta) * sin(fPhi);
    double c3 = cos(theta);

    cout << fPhi << ", " << theta << endl;

    double x0 = fLastHP.getX();
    double y0 = fLastHP.getY();
    double z0 = fLastHP.getZ();
    double R = detector.radius - (detector.width/2);
    double Delta = (x0*c1 + y0*c2)*(x0*c1 + y0*c2) - (c1+c2)*(c1+c2)*(x0*x0 + y0*y0 - R*R);

    double t = (-(x0*c1 + y0*c2) + sqrt(Delta))/(c1*c1 + c2*c2);

    // create a hit
    Hit hit;
    if(z0+c3*t > -(detector.lenght/2) && z0+c3*t < (detector.lenght/2)) hit = Hit(x0+c1*t, y0+c2*t, z0+c3*t, fLastHP.getHitLayer()+1);
    else                                                                hit = Hit(1000., 1000., 1000., fLastHP.getHitLayer()+1);

    // update last hit position for the particle (outside the detector)
    updateHitPos(x0+c1*t, y0+c2*t, z0+c3*t+(detector.width/2), true);

    cout << hit.getX() << ", " << hit.getY() << ", " << hit.getZ() << endl;

    return hit;
}

void Particle::multipleScattering(Detector& detector)
{
    
}