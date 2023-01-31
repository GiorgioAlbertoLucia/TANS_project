void runNo3D(const int nEvents=100000, unsigned int seed=5467)
{
    gRandom->SetSeed(seed);

    // SIMULATION
    Simulation * sim = Simulation::getInstance("config/configFile.txt");
    sim->runSimulation(nEvents);

    // RECONSTRUCTION
    Reconstruction * rec = Reconstruction::getInstance("config/configFile.txt", "config/constantsFile.txt");
    rec->runReconstruction();
}