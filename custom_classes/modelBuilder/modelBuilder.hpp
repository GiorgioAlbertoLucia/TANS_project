#ifndef MODELBUILDER_H
#define MODELBUILDER_H

#include <string>

#include "../yaml/Yaml.hpp"

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
        void createDetectorLayout(const char * configFile) const;
        void addParticleTracks(const char * configFile) const;

        void saveAs(const char * fFilePath) const;



    private:
        // private data members
        static ModelBuilder* fInstancePtr;
        
        TCanvas * fCanvas;

        TGeoManager * fManager;             // manager to create 3D model
        TGeoVolume * fSpace;                // total volume of the model
        


        // private methods
        Recorder();
        virtual ~Recorder(); 

};

#endif