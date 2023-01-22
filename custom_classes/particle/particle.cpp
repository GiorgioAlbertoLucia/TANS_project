#include <Riostream.h>
#include <cmath>

#include <TMath.h>
#include <TRandom3.h>

#include "particle.hpp"

/* PROTECTED */

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
* @param detector 
* @return Hit 
*/
Hit Particle::transport(Detector& detector)
{
    const double theta = this->evalTheta();
    const double c1 = sin(theta) * cos(fPhi);
    const double c2 = sin(theta) * sin(fPhi);
    const double c3 = cos(theta);

    const double x0 = fLastHP.getX();
    const double y0 = fLastHP.getY();
    const double z0 = fLastHP.getZ();
    const double R = detector.radius - (detector.width/2);
    const double Delta = (x0*c1 + y0*c2)*(x0*c1 + y0*c2) - (c1*c1 + c2*c2)*(x0*x0 + y0*y0 - R*R);

    const double t = (-(x0*c1 + y0*c2) + sqrt(Delta))/(c1*c1 + c2*c2);

    // create a hit
    Hit hit;
    if(theta==0)    hit = Hit(0., 0., 14.);
    else            hit = Hit(x0+c1*t, y0+c2*t, z0+c3*t);

    // update last hit position for the particle (outside the detector)
    if(theta==0)    fLastHP = Hit(0., 0., 14.);
    else            fLastHP = Hit(x0+c1*t, y0+c2*t, z0+c3*t+detector.width);

    //if(abs(((x0+c1*t)*(x0+c1*t)+(y0+c2*t)*(y0+c2*t))-(R*R))>0.001)
    //{
    //    cout << "check 1" << endl;
    //    cout << "R = " << R << endl;
    //    cout << "computed R = " << sqrt((x0+c1*t)*(x0+c1*t)+(y0+c2*t)*(y0+c2*t)) << endl;
    //    cout << "detector radius = " << detector.radius << endl << endl;
    //}
    //if(((x0+c1*t)*(x0+c1*t)+(y0+c2*t)*(y0+c2*t))==(R*R))    cout << "giusto" << endl;
    //if(R != fLastHP.evalRadius())
    //{
    //    cout << "check 2" << endl;
    //    cout << "R = " << R << endl;
    //    cout << "computed R = " << fLastHP.evalRadius() << endl;
    //    cout << "detector radius = " << detector.radius << endl << endl;
    //}

    // check cout
    // cout << "particle " << fLastHP.getX() << ", " << fLastHP.getY() << ", " << fLastHP.getZ() << endl;
    // cout << "phi = " << fPhi << "; eta = " << fEta << endl;

    return hit;
}

/**
* @brief Multiple scattering through a detector. In the simplest approximation, only the direction of the particle 
* will be updated. 
* 
* @param detector 
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

    // check cout
    // cout << "before MS: phi = " << fPhi << "; eta = " << fEta << endl;
    
    fEta = - log( tan(acos(vec[2])/2.) );
    double newPhi = atan(vec[1]/vec[0]);
    if((vec[0]/sqrt(1-vec[2]*vec[2]))<0.) newPhi += TMath::Pi();
    if (newPhi < 0.)                      fPhi = newPhi + (2 * TMath::Pi());
    else                                  fPhi = newPhi;
    
    // check cout
    // cout << " after MS: phi = " << fPhi << "; eta = " << fEta << endl;
}