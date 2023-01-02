#include <Riostream.h>
#include <cmath>

#include "particle.hpp"

#include <TMath.h>
#include <TRandom3.h>

/* PROTECTED */

void Particle::updateHitPos(const double X, const double Y, const double Z, const bool updateLayer)
{
    int layer = 0;
    if (updateLayer)    layer = fLastHP.getHitLayer() + 1;
    else                layer = fLastHP.getHitLayer();

    fLastHP = Hit(X, Y, Z, layer);
}

void Particle::rotate(const double phi, const double theta, double (&vec)[3])
{
    double rotMat[3][3] =  {{-sin(phi), -cos(phi)*cos(theta), cos(phi)*sin(theta)},
                            {cos(phi), -sin(phi)*cos(theta), sin(phi)*sin(theta)},
                            {0., sin(theta), cos(theta)}};

    double vecp[3];
    for(int i=0; i<3; i++)  vecp[i] = vec[i];

    for(int i=0; i<3; i++)  for(int j=0; j<3; j++)  vec[i] = rotMat[i][j] * vecp[j];
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
    double theta = this->evalTheta();
    double c1 = sin(theta) * cos(fPhi);
    double c2 = sin(theta) * sin(fPhi);
    double c3 = cos(theta);

    double x0 = fLastHP.getX();
    double y0 = fLastHP.getY();
    double z0 = fLastHP.getZ();
    double R = detector.radius - (detector.width/2);
    double Delta = (x0*c1 + y0*c2)*(x0*c1 + y0*c2) - (c1+c2)*(c1+c2)*(x0*x0 + y0*y0 - R*R);

    double t = (-(x0*c1 + y0*c2) + sqrt(Delta))/(c1*c1 + c2*c2);

    // create a hit
    Hit hit;
    if(z0+c3*t > -(detector.lenght/2) && z0+c3*t < (detector.lenght/2)) hit = Hit(x0+c1*t, y0+c2*t, z0+c3*t, fLastHP.getHitLayer()+1);
    else                                                                hit = Hit(9., 9., 30., fLastHP.getHitLayer()+1);            // choose outliers

    // update last hit position for the particle (outside the detector)
    updateHitPos(x0+c1*t, y0+c2*t, z0+c3*t+(detector.width/2), true);

    // check cout
    // cout << "particle " << hit.getX() << ", " << hit.getY() << ", " << hit.getZ() << endl;

    return hit;
}

void Particle::multipleScattering()
{
    double phiMS = 2. * TMath::Pi() * gRandom->Rndm();
    double thetaMS = 1.;      // from slides

    // direction after MS, with respect to initial particle direction
    double vec[3] = {cos(phiMS)*sin(thetaMS), sin(phiMS)*sin(thetaMS), cos(thetaMS)};

    // return to the initial reference frame
    double theta = this->evalTheta();
    rotate(fPhi, theta, vec);

    // check cout
    //cout << "before MS: phi = " << fPhi << "; eta = " << fEta << endl;
    
    fEta = - log( tan(acos(vec[2])/2.) );
    double newPhi = atan(vec[1]/vec[0]);
    if (newPhi < 0.)    fPhi = newPhi + (2 * TMath::Pi());
    else                fPhi = newPhi;
    
    // check cout
    //cout << " after MS: phi = " << fPhi << "; eta = " << fEta << endl;
}