#include "../custom_classes/plotter/plotter.hpp"
#include "../custom_classes/reconstruction/reconstruction.hpp"
#include "../custom_classes/simulation/simulation.hpp"

#include <TRandom3.h>

void testATA(const int nEvents=100000, unsigned int seed=5467)
{
    gRandom->SetSeed(seed);

    Simulation * sim = new Simulation("simulation", "config/configFile.txt");
    sim->runSimulation(nEvents);

    Reconstruction * rec = new Reconstruction("config/configFile.txt");
    rec->runReconstruction();
}