void run3Danim()
{
    // 3D MODEL
    ModelBuilder* mb1 = new ModelBuilder();

    mb1->initializeTotalSpace(30., 30., 30.);
    mb1->createDetectorLayout("config/configFile.txt");
    mb1->animateParticleTracks("output/anim3Dmodel.gif", "data/recordSimulation.txt", 632);

}