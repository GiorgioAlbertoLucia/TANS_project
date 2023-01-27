#ifndef MODELBUILDER_H
#define MODELBUILDER_H

#include <string>
#include <vector>

#include <TFile.h>
#include <TCanvas.h>
#include <TGLViewer.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TPolyLine3D.h>

/**
 * @brief Singleton class to create a 3D event display using ROOT Framework.
 * Information of detectors and particle tracks should be passed as configuration files.
 * 
 */
class ModelBuilder
{
    public:
        static ModelBuilder * getInstance();
        static void destroy();

        void initializeTotalSpace(const double x, const double y, const double z);
        void createDetectorLayout(const char * configFile);
        void addParticleTracks(const char * recordFile, unsigned long int color, const int style);
        void deleteTracks();

        void draw() const;
        void saveAs(const char * filePath) const;



    private:
        // private data members
        static ModelBuilder* fInstancePtr;
        
        static TCanvas * fCanvas;

        static TGeoManager * fManager;              // manager to create 3D model
        static TGeoVolume * fSpace;                 // total volume of the model
        
        static TPolyLine3D* fLineVector;            // [fLineVectorSize] vector with lines
        static int fLineVectorSize;


        static int fNPoints;                        // number of points set for tracks
        


        // private methods
        ModelBuilder();
        virtual ~ModelBuilder(); 

};

#endif