#include "../include/particle.hpp"

Particle::Particle():
fPhi(0.),
fEta(0.),
fLastIP(IntersectionPoint(0., 0., 0., 0))
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
fLastIP(IntersectionPoint(vertex.getX(), vertex.getY(), vertex.getZ(), -1))
{

}

Particle::~Particle()
{

}

void Particle::updateIntPoint(const double X, const double Y, const double Z)
{
    int layer = fIntersectionPoint.getIntersectionLayer() + 1;
    fIntersectionLayer = IntersectionPoint(X, Y, Z, layer);
}