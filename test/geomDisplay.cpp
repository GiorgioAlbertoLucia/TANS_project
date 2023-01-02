#include "../yaml/Yaml.hpp"

#include <TCanvas.h>
#include <TGLViewer.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TPolyLine3D.h>

void geomDisplay()
{
    Yaml::Node root;
    Yaml::Parse(root, "config/simulationCfg.txt");

    TCanvas * canvas = new TCanvas();
    TGeoManager * manager = new TGeoManager();

    TGeoVolume * space = manager->MakeBox("space", NULL, 30., 30., 30.);
    manager->SetTopVolume(space);

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
