#include "../custom_classes/modelBuilder/modelBuilder.hpp"

void testDisplay()
{
    ModelBuilder* mb = ModelBuilder::getInstance();

    mb->initializeTotalSpace(30., 30., 30.);
    mb->createDetectorLayout("config/configFile.txt");
    mb->addParticleTracks("data/recordSimulation.txt", 632);
    mb->addParticleTracks("data/recordReconstruction.txt", 860, 2);

    mb->draw();
    mb->saveAs("output/3Dmodel.root");

    //mb->destroy();
}