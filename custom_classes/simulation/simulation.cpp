#include <Riostream.h>
#include <vector>

#include <TH1I.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

#include "simulation.hpp"
#include "../event/event.hpp"
#include "../detector/detector.hpp"
#include "../hit/hit.hpp"

void Simulation::runSimulation(const int nEvents)
{
    Event * event = new Event();
    Vertex vertex;
    Detector detector1 = {3., 0.08, 27., false};

    //TTree * tree = new TTree(fTreeName.c_str(), "Simulation");
    TTree * tree = new TTree("OhXmasTTree", "OhXmasTTree");
    tree->Branch("Vertex", &vertex);

    //vector<Hit> hits;
    //vector<Hit> *ptrhits = &hits;
    //vector<double> hits;
    TClonesArray hits("Hit");
    tree->Branch("Hits", &hits);
    //tree->Branch("Hits", &ptrhits);

    TFile inFile("data/kinem.root");
    
    TH1I * hMultiplicity = new TH1I();
    TH1F * hEta = (TH1F*)inFile.Get("heta2");
    hMultiplicity->SetDirectory(0);
    hEta->SetDirectory(0);
    inFile.Close();


    for (int i=0; i<nEvents; i++)
    {
        vertex = event->partGeneration(*hMultiplicity, *hEta);
        //hits.reserve(event->getVertex().getMultiplicity());
        //for (int j=0; j<event->getVertex().getMultiplicity(); j++)  hits.push_back(event->partTransport(detector1)[j]);
        hits = event->partTransport2(detector1);
        //hits = {1., 2.};



        tree->Fill();
        //hits.clear();
        hits.Clear();
    }

    TFile file("data/simulation.root", "recreate");
    tree->Write();
    file.Close();

    //delete event;
    //delete tree;
}


void Simulation::runSimulation2(const int nEvents)
{
    /*
    Event * event = new Event();
    Vertex vertex;
    Detector detector1 = {3., 0.08, 27., false};

    //TTree * tree = new TTree(fTreeName.c_str(), "Simulation");
    TTree * tree = new TTree("OhXmasTTree", "OhXmasTTree");
    tree->Branch("Vertex", &vertex);

    //vector<Hit> hits;
    //vector<Hit> *ptrhits = &hits;
    //vector<double> hits;
    TClonesArray hits("Hit");
    tree->Branch("Hits", &hits);
    //tree->Branch("Hits", &ptrhits);

    for (int i=0; i<nEvents; i++)
    {
        vertex = event->partGeneration();
        //hits.reserve(event->getVertex().getMultiplicity());
        //for (int j=0; j<event->getVertex().getMultiplicity(); j++)  hits.push_back(event->partTransport(detector1)[j]);
        hits = event->partTransport2(detector1);
        //hits = {1., 2.};



        tree->Fill();
        //hits.clear();
        hits.Clear();
    }

    TFile file("data/simulation.root", "recreate");
    tree->Write();
    file.Close();

    //delete event;
    //delete tree;
    */
}
