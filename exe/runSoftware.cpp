void runSoftware(const int nEvents=100000, unsigned int seed=5467)
{
    gRandom->SetSeed(seed);

    // SIMULATION
    Simulation * sim = new Simulation("simulation", "config/configFile.txt");
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

    mb->draw();
    mb->saveAs("output/3Dmodel.root");
}