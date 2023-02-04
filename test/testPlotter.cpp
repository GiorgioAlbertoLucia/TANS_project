#include "../custom_classes/plotter/plotter.hpp"
#include "../custom_classes/reconstruction/reconstruction.hpp"

void testPlotter()
{
    Reconstruction * rec = Reconstruction::getInstance("config/configFile.txt","config/constantsFile.txt");
    rec->runReconstruction();
}