#include <Riostream.h>
#include <string>
#include <vector>

#include <TH1I.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>

#include "simulation.hpp"
#include "../event/event.hpp"
#include "../detector/detector.hpp"
#include "../hit/hit.hpp"
#include "../../yaml/Yaml.hpp"

void Simulation::runSimulation(const int nEvents)
{
    // initialize parser and event
    Yaml::Node root;
    Yaml::Parse(root, fConfigFile.c_str());

    Event * event = new Event();

    //  initialize vertex and detectors
    Vertex vertex;
    
    const int nDetectors = root["n_detectors"].As<int>();
    vector<Detector> detectorVector;
    detectorVector.reserve(nDetectors);

    for(int i=0; i<nDetectors; i++)
    {
        Detector detector = {root["detectors"][i]["radius"].As<double>(),
                             root["detectors"][i]["width"].As<double>(),
                             root["detectors"][i]["lenght"].As<double>(),
                             root["detectors"][i]["multiple_scattering"].As<bool>(),};
        detectorVector.push_back(detector);
    }

    // initialize ttree
    TTree * tree = new TTree((root["outputNames"]["treeSimName"].As<std::string>()).c_str(), 
                             (root["outputNames"]["treeSimName"].As<std::string>()).c_str());
    
    tree->Branch("Vertex", &vertex);

    vector<TClonesArray> hitArrayVector;
    hitArrayVector.reserve(nDetectors);
    char name[50];

    for(int i=0; i<nDetectors; i++)
    {
        TClonesArray hitArray("Hit");    
        hitArrayVector.push_back(hitArray);

        sprintf(name, "HitsL%d", i);
        tree->Branch(name, &hitArrayVector[i]);
    }

    // read input distributions
    TFile inFile((root["inputPaths"]["distributions"].As<std::string>()).c_str());

    TH1I * hMultiplicity = new TH1I("hMultiplicity", "check", 100, 0, 10);
    TH1F * hEta = (TH1F*)inFile.Get("heta2");
    
    hMultiplicity->SetDirectory(0);
    hEta->SetDirectory(0);
    inFile.Close();

    for(int ev=0; ev<nEvents; ev++)
    {
        vertex = event->partGeneration(*hMultiplicity, *hEta);
        for(int i=0; i<nDetectors; i++)     hitArrayVector[i] = event->partTransport2(detectorVector[i]);

        tree->Fill();
        for(TClonesArray clone: hitArrayVector) clone.Clear();
        event->clear();
    }

    

    TFile outFile((root["outputPaths"]["treeSimPath"].As<std::string>()).c_str(), "recreate");
    tree->Write();
    outFile.Close();

    //delete event;
    //delete tree;
}


void Simulation::runSimulation2(const int nEvents)
{
    
    Event * event = new Event();
    Vertex vertex;
    Detector detector1 = {3., 0.08, 27., false};

    //TTree * tree = new TTree(fTreeName.c_str(), "Simulation");
    TTree * tree = new TTree("OhXmasTTree", "OhXmasTTree");
    tree->Branch("Vertex", &vertex);

    vector<Hit> hits;
    vector<Hit> *ptrhits = &hits;
    
    // input distributions
    TFile inFile("data/kinem.root");
    TH1I * hMultiplicity = new TH1I("hMultiplicity", "check", 100, 0, 10);
    TH1F * hEta = (TH1F*)inFile.Get("heta2");
    hMultiplicity->SetDirectory(0);
    hEta->SetDirectory(0);
    inFile.Close();

    for (int i=0; i<nEvents; i++)
    {
        vertex = event->partGeneration(*hMultiplicity, *hEta);
        hits.reserve(event->getVertex().getMultiplicity());
        for (int j=0; j<event->getVertex().getMultiplicity(); j++)  hits.push_back(event->partTransport(detector1)[j]);

        tree->Fill();
        hits.clear();
        event->clear();
    }

    TFile file("data/simulation.root", "recreate");
    tree->Write();
    file.Close();

    //delete event;
    //delete tree;
    
}
