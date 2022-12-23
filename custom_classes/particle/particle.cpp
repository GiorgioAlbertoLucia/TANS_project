#include "particle.hpp"

/* PROTECTED */

void Particle::updateHitPos(const double X, const double Y, const double Z, const bool updateLayer)
{
    int layer = 0;
    if (updateLayer)    layer = fLastHP->getHitLayer() + 1;
    else                layer = fLastHP->getHitLayer();

    delete fLastHP;
    fLastHP = new Hit(X, Y, Z, layer);
}

/* PUBLIC */

Particle::Particle():
fPhi(0.),
fEta(0.),
fLastHP(NULL)
{

}

Particle::Particle(const double Phi, const double Eta, Vertex& vertex): 
fPhi(Phi), 
fEta(Eta)
{
    fLastHP = new Hit(vertex.getX(), vertex.getY(), vertex.getZ(), -1);
}

Particle::~Particle()
{
    delete fLastHP;
}

Hit Particle::transport(Detector& detector)
{
    double theta = 2. * atan(exp(-fEta));
    double c1 = sin(theta) * cos(fPhi);
    double c2 = sin(theta) * sin(fPhi);
    double c3 = cos(theta);

    Hit &lastHP = *fLastHP;
    double x0 = lastHP.getX();
    double y0 = lastHP.getY();
    double z0 = lastHP.getZ();
    double R = detector.radius - (detector.width/2);
    double Delta = (x0*c1 + y0*c2)*(x0*c1 + y0*c2) - (c1+c2)*(c1+c2)*(x0*x0 + y0*y0 - R*R);

    double t = (-(x0*c1 + y0*c2) + sqrt(Delta))/(c1*c1 + c2*c2);

    // create a hit
    Hit hit;
    if(z0+c3*t > -(detector.lenght/2) && z0+c3*t < (detector.lenght/2)) hit = Hit(x0+c1*t, y0+c2*t, z0+c3*t, lastHP.getHitLayer()+1);
    else                                                                hit = Hit(1000., 1000., 1000., lastHP.getHitLayer()+1);

    // update last hit position for the particle (outside the detector)
    updateHitPos(x0+c1*t, y0+c2*t, z0+c3*t+(detector.width/2), true);

    return hit;
}

void Particle::multipleScattering(Detector& detector)
{
    
}