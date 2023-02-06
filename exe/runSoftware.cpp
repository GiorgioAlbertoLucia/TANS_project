void runSoftware(const int nEvents=1000000, unsigned int seed=5467)
{
    gRandom->SetSeed(seed);

    // SIMULATION
    Simulation * sim = Simulation::getInstance("config/configFile.txt");
    sim->runSimulation(nEvents);

    // RECONSTRUCTION
    Reconstruction * rec = Reconstruction::getInstance("config/configFile.txt");
    rec->runReconstruction();

    // 3D MODEL
    ModelBuilder* mb = new ModelBuilder();

    mb->initializeTotalSpace(30., 30., 30.);
    mb->createDetectorLayout("config/configFile.txt");
    mb->addParticleTracks("data/recordSimulation.txt", 632);

    mb->draw();
    mb->saveAs("output/3Dmodel.root");
}