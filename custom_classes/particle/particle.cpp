#include <Riostream.h>
#include <cmath>

#include <TMath.h>
#include <TRandom3.h>

#include "particle.hpp"

/* PROTECTED */

/**
 * @brief Rotates a vector in cartesian coordinate in the 3D space by an azimuthal angle phi and a polar angle theta
 * 
 * @param phi 
 * @param theta 
 * @param vec 
 */
void Particle::rotate(const double phi, const double theta, double (&vec)[3])
{
    double rotMat[3][3] =  {{-sin(phi), -cos(phi)*cos(theta),   cos(phi)*sin(theta)},
                            {cos(phi),  -sin(phi)*cos(theta),   sin(phi)*sin(theta)},
                            {0.,        sin(theta),             cos(theta)}};

    double vecp[3];
    for(int i=0; i<3; i++)  vecp[i] = vec[i];

    for(int i=0; i<3; i++)
    {
        vec[i] = 0;
        for(int j=0; j<3; j++)  vec[i] += rotMat[i][j] * vecp[j];
    }
}

/* PUBLIC */

/**
* @brief Whenever a particle is created, its last intersection point is considered to be the vertex position
* 
* @param Phi 
* @param Eta 
* @param vertex 
*/
Particle::Particle(const double Phi, const double Eta, Vertex& vertex): 
fPhi(Phi), 
fEta(Eta)
{
    fLastHP = Hit(vertex.getX(), vertex.getY(), vertex.getZ());
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

/**
* @brief Creates a Hit (position and layer) of this particle on given detector assuming the particle will
* move along a straight line from its starting position (in fLastHP)
* 
* @param R radius of the detector
* @return Hit 
*/
Hit Particle::transport(const double R)
{
    const double theta = this->evalTheta();
    const double c1 = sin(theta) * cos(fPhi);
    const double c2 = sin(theta) * sin(fPhi);
    const double c3 = cos(theta);

    const double x0 = fLastHP.getX();
    const double y0 = fLastHP.getY();
    const double z0 = fLastHP.getZ();

    const double Delta = (x0*c1 + y0*c2)*(x0*c1 + y0*c2) - (c1*c1 + c2*c2)*(x0*x0 + y0*y0 - R*R);

    const double t = (-(x0*c1 + y0*c2) + sqrt(Delta))/(c1*c1 + c2*c2);
    const double x = x0+c1*t;
    const double y = y0+c2*t;
    
    double phi = atan(y/x);
    if(x < 0)   phi += TMath::Pi();
    if(phi < 0) phi += 2. * TMath::Pi();

    // create a hit
    Hit hit(sqrt(x*x + y*y), phi, z0+c3*t);

    // update last hit position for the particle
    fLastHP = Hit(sqrt(x*x + y*y), phi, z0+c3*t);
    
    return hit;
}

/**
* @brief Multiple scattering through a detector. In the simplest approximation, only the direction of the particle 
* will be updated. 
*  
*/
void Particle::multipleScattering()
{
    double phiMS = 2. * TMath::Pi() * gRandom->Rndm();
    double thetaMS = gRandom->Gaus(0, 0.001);               // theta0 rms = 1 mrad

    // direction after MS, with respect to initial particle direction
    double vec[3] = {cos(phiMS)*sin(thetaMS), sin(phiMS)*sin(thetaMS), cos(thetaMS)};

    // return to the initial reference frame
    double theta = this->evalTheta();
    rotate(fPhi, theta, vec);
    
    fEta = - log( tan(acos(vec[2])/2.) );
    double newPhi = atan(vec[1]/vec[0]);
    if((vec[0]/sqrt(1-vec[2]*vec[2]))<0.) newPhi += TMath::Pi();
    if (newPhi < 0.)                      fPhi = newPhi + (2 * TMath::Pi());
    else                                  fPhi = newPhi;
    
}