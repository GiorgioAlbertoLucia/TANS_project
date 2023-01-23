#include <string>

#include "../yaml/Yaml.hpp"

#include <TFile.h>
#include <TCanvas.h>
#include <TGLViewer.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TPolyLine3D.h>

ModelBuilder* ModelBuilder::fInstancePtr = NULL;

/*  PRIVATE */
/**
 * @brief Construct a new Model Builder:: Model Builder object
 * 
 */
ModelBuilder::ModelBuilder()
{
    fCanvas = new TCanvas();
    fManager = new TGeoManager();
}

/**
 * @brief Destroy the Model Builder:: Model Builder object
 * 
 */
ModelBuilder::~ModelBuilder()
{

}

/*  PUBLIC  */

/**
 * @brief Create an instance of the singleton object
 * 
 */
ModelBuilder* ModelBuilder::getInstance()
{
    if(fInstancePtr == NULL)    fInstancePtr = new ModelBuilder();
    return fInstancePtr;    
}

/**
 * @brief Destroy singleton object
 * 
 */
void ModelBuilder::destroy()
{
    if(fCanvas)                 delete fCanvas;
    fCanvas = NULL;
    if(fManager)                delete fManager;
    fManager = NULL;
    if(fSpace)                  delete fSpace;
    fSpace = NULL;

    if(fInstancePtr)            delete fInstancePtr;
    fInstancePtr = NULL;
}

void ModelBuilder::initializeTotalSpace(const double x, const double y, const double z)
{
    if(fSpace)                  delete fSpace;
    fSpace = manager->MakeBox("space", NULL, x, y, z);
    manager->SetTopVolume(space);
}

/**
 * @brief Creates detectors in the 3D space reading instructions from a .txt file
 * 
 * @param configFile 
 */
void ModelBuilder::createDetectorLayout(const char * configFile)
{
    Yaml::Node root;
    Yaml::Parse(root, configFile);

    // DETECTORS
    const int nDetectors = root["n_detectors"].As<int>();
    TGeoVolume * detectors[nDetectors];

    for(int i=0; i<nDetectors; i++)
    {
        const double radius = root["detectors"][i]["radius"].As<double>();
        const double width = root["detectors"][i]["width"].As<double>();
        const double lenght = root["detectors"][i]["lenght"].As<double>();

        detectors[i] = manager->MakeTube(Form("detector%d", i), NULL, radius-width/2., radius+width/2., lenght);
        space->AddNode(detectors[i], i);    
    }

    manager->CloseGeometry();
    space->Draw("ogl");

}

