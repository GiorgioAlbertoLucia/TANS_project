void runNo3D(const int nEvents=100000, unsigned int seed=5467)
{
    gRandom->SetSeed(seed);

    // SIMULATION
    Simulation * sim = new Simulation("simulation", "config/configFile.txt");
    sim->runSimulation(nEvents);

    // RECONSTRUCTION
    Reconstruction * rec = new Reconstruction("config/configFile.txt");
    rec->runReconstruction();
}