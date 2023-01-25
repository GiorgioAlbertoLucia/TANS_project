#include "../custom_classes/plotter/plotter.hpp"
#include "../custom_classes/reconstruction/reconstruction.hpp"
#include "../custom_classes/simulation/simulation.hpp"

#include <TRandom3.h>

void testATA()
{
    gRandom->SetSeed(5467);

    Simulation * sim = new Simulation("simulation", "config/configFile.txt");
    sim->runSimulation(100000);

    Reconstruction * rec = new Reconstruction("config/configFile.txt");
    rec->runReconstruction();
}