#include <string>

#include "../yaml/Yaml.hpp"

#include <TFile.h>
#include <TCanvas.h>
#include <TGLViewer.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TPolyLine3D.h>
#include <TGeoTrack.h>

void geomDisplay()
{
    Yaml::Node root;
    Yaml::Parse(root, "config/configFile.txt");

    cout << endl << "-------------------------------------" << endl;
    cout << "Creating event display..." << endl << endl;

    TCanvas * canvas = new TCanvas();
    TGeoManager * manager = new TGeoManager();

    TGeoVolume * space = manager->MakeBox("space", NULL, 50., 50., 50.);
    manager->SetTopVolume(space);

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

    //TGeoTrack * track = new TGeoTrack();
    //track->AddPoint(0.000852881, 1.18093e-05, -0.0110136, 0.);
    //track->AddPoint(2.55264, 1.49854, 4.77401, 1.);
    //track->AddPoint(3.43937, 2.02259, 6.43989, 2.);
    //track->AddPoint(6.02205, 3.54894, 11.292, 3.);
    //track->SetLineColor(kRed);
    //manager->AddTrack(track);
    //track->Draw();

    Yaml::Node recordRoot;
    Yaml::Parse(recordRoot, "data/recordSimulation.txt");

    // PARTICLE TRAJECTORY (AS LINES)
    const int multiplicity = recordRoot["Vertex"]["multiplicity"].As<int>();




    TPolyLine3D* lineArray[multiplicity];
    for(int i=0; i<multiplicity; i++)
    {
        lineArray[i] = new TPolyLine3D(nDetectors+1);

        // set vertex position
        lineArray[i]->SetPoint(0,
                               recordRoot["Vertex"]["x"].As<double>(),
                               recordRoot["Vertex"]["y"].As<double>(),
                               recordRoot["Vertex"]["z"].As<double>());
        
        // set intersection points
        for(int j=0; j<nDetectors; j++)
        {
            lineArray[i]->SetPoint(j+1,
                                   recordRoot["DetectorLayers"][j]["Particles"][i]["x"].As<double>(),
                                   recordRoot["DetectorLayers"][j]["Particles"][i]["y"].As<double>(),
                                   recordRoot["DetectorLayers"][j]["Particles"][i]["z"].As<double>());
        }

        lineArray[i]->SetLineColor(kRed);
        lineArray[i]->Draw("same");
    }

    TFile outFile("output/example3D.root", "recreate");
    canvas->Write();
    outFile.Close();
    cout << endl;
    cout << "3D model saved in file " << "output/example3D.root" << "." << endl;

}
