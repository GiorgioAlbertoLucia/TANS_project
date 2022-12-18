#include "particle.hpp"

Particle::Particle():
fPhi(0.),
fEta(0.),
fLastIP(Hit(0., 0., 0., 0))
{

}

/**
 * @brief Whenever a particle is created, its last intersection point is considered to be the vertex position
 * 
 * @param Phi 
 * @param Eta 
 * @param vertex 
 */
Particle::Particle(const double Phi, const double Eta, Vertex& vertex): 
fPhi(Phi), 
fEta(Eta), 
fLastIP(Hit(vertex.getX(), vertex.getY(), vertex.getZ(), -1))
{

}

Particle::~Particle()
{

}

void Particle::updateHitPos(const double X, const double Y, const double Z, const bool updateLayer)
{
    if (updateLayer)    int layer = fIntersectionPoint.getHitLayer() + 1;
    else                int layer = fIntersectionPoint.getHitLayer();

    fLastHP = Hit(X, Y, Z, layer);
}

Hit transport(Detector& detector)
{
    //Point hitPos = 
}