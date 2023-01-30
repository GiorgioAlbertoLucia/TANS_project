void run3Dmodel()
{
    // 3D MODEL
    ModelBuilder* mb1 = new ModelBuilder();

    mb1->initializeTotalSpace(30., 30., 30.);
    mb1->createDetectorLayout("config/configFile.txt");
    mb1->addParticleTracks("data/recordSimulation.txt", 632);

    mb1->draw();
    mb1->saveAs("output/3DmodelSim.root");

    ModelBuilder* mb2 = new ModelBuilder();

    mb2->initializeTotalSpace(30., 30., 30.);
    mb2->createDetectorLayout("config/configFile.txt");
    mb2->addParticleTracks("data/recordReconstruction.txt", 860);

    mb2->draw();
    mb2->saveAs("output/3DmodelRec.root");
}