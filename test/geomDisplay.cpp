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

    TGeoVolume * space = manager->MakeBox("space", NULL, 100., 100., 100.);
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

    Yaml::Parse(root, "data/recordSimulation.txt");

    const int multiplicity = root["Vertex"]["multiplicity"].As<int>();
    cout << "multiplicity is " <<  multiplicity << endl;

    //TPolyLine3D * line = new TPolyLine3D();
    //for(int j=0; j<nDetectors; j++)
    //{
    //    line->SetPoint(j, 
    //                   root["DetectorLayers"][j]["Particles"][0]["x"].As<double>(),
    //                   root["DetectorLayers"][j]["Particles"][0]["y"].As<double>(),
    //                   root["DetectorLayers"][j]["Particles"][0]["z"].As<double>());
    //}
    //line->SetLineColor(kRed);
    //line->Draw("same");

    TPolyLine3D* lineArray[multiplicity];
    for(int i=0; i<multiplicity; i++)
    {
        lineArray[i] = new TPolyLine3D();

        // set vertex position
        lineArray[i]->SetPoint(i*multiplicity,
                               root["Vertex"]["x"].As<double>(),
                               root["Vertex"]["y"].As<double>(),
                               root["Vertex"]["z"].As<double>());
        
        // set intersection points
        for(int j=0; j<nDetectors; j++)
        {
            lineArray[i]->SetPoint(i*multiplicity+j+1,
                                   root["DetectorLayers"][j]["Particles"][i]["x"].As<double>(),
                                   root["DetectorLayers"][j]["Particles"][i]["y"].As<double>(),
                                   root["DetectorLayers"][j]["Particles"][i]["z"].As<double>());
        }

        lineArray[i]->SetLineColor(kRed);
        lineArray[i]->Draw("same");
        
    }

    

}
