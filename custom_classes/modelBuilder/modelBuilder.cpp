#include <string>

#include "../yaml/Yaml.hpp"

#include <TFile.h>
#include <TCanvas.h>
#include <TGLViewer.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TPolyLine3D.h>

ModelBuilder* ModelBuilder::fInstancePtr = NULL;
int fNPoints = 0;

/*  PRIVATE */
/**
 * @brief Construct a new Model Builder:: Model Builder object
 * 
 */
ModelBuilder::ModelBuilder()
{
    fCanvas = new TCanvas();
    fManager = new TGeoManager();
    fNPoints = 0;
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

/**
 * @brief Initializes top space as a box with dimensions (2x, 2y, 2z)
 * 
 * @param x 
 * @param y 
 * @param z 
 */
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
    fCanvas->cd();
    space->Draw("ogl");

}

/**
 * @brief Adds tracks of particles reading from a .txt configuration file. The file must store
 * particle positions in cartesian coordinates.
 * 
 * @param configFile 
 * @param color TColor: for reference https://root.cern.ch/doc/master/classTColor.html
 * 
 * kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
 * kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
 * kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
 */
void ModelBuilder::addParticleTracks(const char * configFile, unsigned long int color)
{
    Yaml::Node root;
    Yaml::Parse(root, configFile);

    const int multiplicity = recordRoot["Vertex"]["multiplicity"].As<int>();
    const int prevSize = fLineVector.size();        // previous size of the vector
    fLineVector.resize(previousSize+multiplicity);

    TPolyLine3D* lineArray[multiplicity];
    for(int i=prevSize; i<prevSize+multiplicity; i++)
    {
        TPolyLine3D line;
        fLineVector.push_back(line);

        // set vertex position
        fLineVector[i].SetPoint(fNPoints,
                                recordRoot["Vertex"]["x"].As<double>(),
                                recordRoot["Vertex"]["y"].As<double>(),
                                recordRoot["Vertex"]["z"].As<double>());
        fNPoints++;
        
        // set intersection points
        for(int j=0; j<nDetectors; j++)
        {
            fLineVector[i].SetPoint(fNPoints,
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["x"].As<double>(),
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["y"].As<double>(),
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["z"].As<double>());
            fNPoints++;
        }


        fLineVector[i].SetLineColor(color);
        fCanvas->cd();
        fLineVector[i].Draw("same");
    }
}

/**
 * @brief Removes all stored tracks
 * 
 */
void ModelBuilder::deleteTracks()
{
    fNPoints = 0;
    fLineVector.clear();
}

/**
 * @brief Saves 3D model in a .root file
 * 
 * @param filePath 
 */
void ModelBuilder::saveAs(const char * filePath)
{
    TFile outFile(filePath, "recreate");
    canvas->Write();
    outFile.Close();
    cout << endl;
    cout << "3D model saved in file " << filePath << "." << endl;
}
