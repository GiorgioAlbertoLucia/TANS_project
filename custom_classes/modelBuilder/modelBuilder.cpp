#include <Riostream.h>
#include <string>

#include "../../yaml/Yaml.hpp"
#include "modelBuilder.hpp"

#include <TFile.h>
#include <TCanvas.h>
#include <TGLViewer.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TPolyLine3D.h>
#include <TColor.h>
#include <TSystem.h>
#include <TTimer.h>



/*  PUBLIC  */
/**
 * @brief Construct a new Model Builder:: Model Builder object
 * 
 */
ModelBuilder::ModelBuilder(): fSpace(NULL), fLineVector(NULL), fLineVectorSize(0)
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
    if(fCanvas)                 delete fCanvas;
    fCanvas = NULL;
    if(fManager)                delete fManager;
    fManager = NULL;
    if(fSpace)                  delete fSpace;
    fSpace = NULL;

    if(fLineVectorSize != 0)    delete []fLineVector;
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
    fSpace = fManager->MakeBox("space", NULL, x, y, z);
    fSpace->SetLineColor(1);
    fManager->SetTopVolume(fSpace);
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
        const int color = root["detectors"][i]["color"].As<int>();

        detectors[i] = fManager->MakeTube(Form("detector%d", i), NULL, radius-width/2., radius+width/2., lenght);
        detectors[i]->SetLineColor(color);
        fSpace->AddNode(detectors[i], i);    
    }

    fManager->CloseGeometry();
}

/**
 * @brief Adds tracks of particles reading from a .txt configuration file. The file must store
 * particle positions in cartesian coordinates.
 * 
 * @param recordFile file containing tracks information
 * @param color TColor: for reference https://root.cern.ch/doc/master/classTColor.html
 * 
 * kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
 * kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
 * kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
 * 
 * @param style TSyle
 */
void ModelBuilder::addParticleTracks(const char * recordFile, unsigned long int color, unsigned long int style)
{
    Yaml::Node recordRoot;
    Yaml::Parse(recordRoot, recordFile);

    const int multiplicity = recordRoot["Vertex"]["multiplicity"].As<int>();
    const int prevSize = fLineVectorSize;        // previous size of the vector

    if(fLineVectorSize != 0)
    {
        TPolyLine3D* temp = new TPolyLine3D[prevSize];
        for(int i=0; i<prevSize; i++)   temp[i] = fLineVector[i];

        delete []fLineVector;
        const int newSize = prevSize + multiplicity;
        fLineVectorSize = newSize;
        fLineVector = new TPolyLine3D[newSize];
        for(int i=0; i<prevSize; i++)   fLineVector[i] = temp[i];

        delete []temp;
    }
    else
    {
        fLineVectorSize = multiplicity;
        fLineVector = new TPolyLine3D[multiplicity];
    }

    for(int i=prevSize; i<prevSize+multiplicity; i++)
    {
        fLineVector[i] = TPolyLine3D();

        // set vertex position
        fLineVector[i].SetPoint(0,
                                recordRoot["Vertex"]["x"].As<double>(),
                                recordRoot["Vertex"]["y"].As<double>(),
                                recordRoot["Vertex"]["z"].As<double>());
        
        // set intersection points
        const int nDetectors = recordRoot["nLayers"].As<int>();

        for(int j=0; j<nDetectors; j++)
        {
            fLineVector[i].SetPoint(j+1,
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["x"].As<double>(),
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["y"].As<double>(),
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["z"].As<double>());
        }

        fLineVector[i].SetLineColor(color);
        fLineVector[i].SetLineStyle(style);
    }
}

/**
 * @brief Initializes tracks as TPolyLine3D and adds the vertex position
 * 
 * @param recordFile 
 */
void ModelBuilder::addVertexPoint(const char * recordFile, unsigned long int color, unsigned long int style)
{
    Yaml::Node recordRoot;
    Yaml::Parse(recordRoot, recordFile);

    const int multiplicity = recordRoot["Vertex"]["multiplicity"].As<int>();
    const int prevSize = fLineVectorSize;        // previous size of the vector

    if(fLineVectorSize != 0)
    {
        TPolyLine3D* temp = new TPolyLine3D[prevSize];
        for(int i=0; i<prevSize; i++)   temp[i] = fLineVector[i];

        delete []fLineVector;
        const int newSize = prevSize + multiplicity;
        fLineVectorSize = newSize;
        fLineVector = new TPolyLine3D[newSize];
        for(int i=0; i<prevSize; i++)   fLineVector[i] = temp[i];

        delete []temp;
    }
    else
    {
        fLineVectorSize = multiplicity;
        fLineVector = new TPolyLine3D[multiplicity];
    }

    for(int i=prevSize; i<prevSize+multiplicity; i++)
    {
        fLineVector[i] = TPolyLine3D();

        // set vertex position
        fLineVector[i].SetPoint(0,
                                recordRoot["Vertex"]["x"].As<double>(),
                                recordRoot["Vertex"]["y"].As<double>(),
                                recordRoot["Vertex"]["z"].As<double>());
        
        fLineVector[i].SetLineColor(color);
        fLineVector[i].SetLineStyle(style);
    }

}

/**
 * @brief Adds hit points on a given detector
 * 
 * @param recordFile 
 * @param detectorIndex 
 */
void ModelBuilder::addTracksOnDetector(const char * recordFile, const int detectorIndex)
{
    Yaml::Node recordRoot;
    Yaml::Parse(recordRoot, recordFile);

    for(int i=0; i<fLineVectorSize; i++)
    {
        // set intersection points
        fLineVector[i].SetPoint(detectorIndex+1,
                                recordRoot["DetectorLayers"][detectorIndex]["Particles"][i]["x"].As<double>(),
                                recordRoot["DetectorLayers"][detectorIndex]["Particles"][i]["y"].As<double>(),
                                recordRoot["DetectorLayers"][detectorIndex]["Particles"][i]["z"].As<double>());
    }
}

void animTest()
{
    cout << "Up on melancholy hill" << endl;
}

void ModelBuilder::animateParticleTracks(const char * animFile, const char * recordFile, unsigned long int color, unsigned long int style)
{
    TTimer timer(20);

    timer.SetCommand("animTest()");
    timer.TurnOn();
}

/**
 * @brief Adds tracks of particles reading from a .txt configuration file. The file must store
 * particle positions in cartesian coordinates. Creates an animation as a .gif file.
 * 
 * @param animFile path to the file where the animation will be saved
 * @param recordFile file containing tracks information
 * @param color TColor: for reference https://root.cern.ch/doc/master/classTColor.html
 * 
 * kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
 * kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
 * kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
 * 
 * @param style TSyle
 */
/*
void ModelBuilder::animateParticleTracks(const char * animFile, const char * recordFile, unsigned long int color, unsigned long int style)
{
    Yaml::Node recordRoot;
    Yaml::Parse(recordRoot, recordFile);

    const int multiplicity = recordRoot["Vertex"]["multiplicity"].As<int>();
    const int prevSize = fLineVectorSize;        // previous size of the vector

    if(fLineVectorSize != 0)
    {
        TPolyLine3D* temp = new TPolyLine3D[prevSize];
        for(int i=0; i<prevSize; i++)   temp[i] = fLineVector[i];

        delete []fLineVector;
        const int newSize = prevSize + multiplicity;
        fLineVectorSize = newSize;
        fLineVector = new TPolyLine3D[newSize];
        for(int i=0; i<prevSize; i++)   fLineVector[i] = temp[i];

        delete []temp;
    }
    else
    {
        fLineVectorSize = multiplicity;
        fLineVector = new TPolyLine3D[multiplicity];
    }

    // setup for the animation
    TCanvas * canvas = new TCanvas();
    gSystem->Unlink(animFile);

    fSpace->Draw("ogl");

    for(int i=prevSize; i<prevSize+multiplicity; i++)
    {
        fLineVector[i] = TPolyLine3D();
        fLineVector[i].SetLineColor(color);
        fLineVector[i].SetLineStyle(style);

        // set vertex position
        fLineVector[i].SetPoint(0,
                                recordRoot["Vertex"]["x"].As<double>(),
                                recordRoot["Vertex"]["y"].As<double>(),
                                recordRoot["Vertex"]["z"].As<double>());
        fLineVector[i].Draw("same");
    }

    canvas->Update();
    canvas->Print(Form("%s+", animFile));
    
    // set intersection points
    const int nDetectors = recordRoot["nLayers"].As<int>();

    for(int j=0; j<nDetectors; j++)
    {
        for(int i=prevSize; i<prevSize+multiplicity; i++)
        {
            fLineVector[i].SetPoint(j+1,
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["x"].As<double>(),
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["y"].As<double>(),
                                    recordRoot["DetectorLayers"][j]["Particles"][i]["z"].As<double>());
            fLineVector[i].Draw("same");
        }
        canvas->Update();
        canvas->Print(Form("%s+", animFile));
    }    
}
*/

/**
 * @brief Removes all stored tracks
 * 
 */
void ModelBuilder::deleteTracks()
{
    fLineVectorSize = 0;
    delete []fLineVector;
}

void ModelBuilder::draw() const
{
    fCanvas->cd();
    fSpace->Draw("ogl");
    for(int i=0; i<fLineVectorSize; i++)    fLineVector[i].Draw("same");   
    fCanvas->Draw();
}

/**
 * @brief Saves 3D model in a .root file
 * 
 * @param filePath 
 */
void ModelBuilder::saveAs(const char * filePath) const
{
    fCanvas->cd();
    fSpace->Draw("ogl");
    for(int i=0; i<fLineVectorSize; i++)    fLineVector[i].Draw("same");

    TFile outFile(filePath, "recreate");
    fCanvas->Write();
    outFile.Close();
    cout << endl;
    cout << "3D model saved in file " << filePath << "." << endl;
}
