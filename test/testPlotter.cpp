#include "../custom_classes/plotter/plotter.hpp"
#include "../custom_classes/reconstruction/reconstruction.hpp"

void testPlotter()
{
    Reconstruction * rec = new Reconstruction("config/configFile.txt");
    rec->runReconstruction();
}