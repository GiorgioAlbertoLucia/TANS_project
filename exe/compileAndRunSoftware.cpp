#include <TRandom3.h>

#include "../custom_classes/simulation/simulation.hpp"
#include "../custom_classes/reconstruction/reconstruction.hpp"
#include "../custom_classes/modelBuilder/modelBuilder.hpp"

void runSoftware(const int nEvents=100000, unsigned int seed=5467)
{
    gRandom->SetSeed(seed);

    // SIMULATION
    Simulation * sim = Simulation::getInstance("config/configFile.txt");
    sim->runSimulation(nEvents);

    // RECONSTRUCTION
    Reconstruction * rec = new Reconstruction("config/configFile.txt");
    rec->runReconstruction();

    // 3D MODEL
    ModelBuilder* mb = ModelBuilder::getInstance();

    mb->initializeTotalSpace(30., 30., 30.);
    mb->createDetectorLayout("config/configFile.txt");
    mb->addParticleTracks("data/recordSimulation.txt", 632);
    mb->addParticleTracks("data/recordReconstruction.txt", 860, 2);

    //mb->draw();
    mb->saveAs("output/3Dmodel.root");
}