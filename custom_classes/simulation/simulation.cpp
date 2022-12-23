#include <Riostream.h>
#include <vector>

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

    TTree * tree = new TTree(fTreeName.c_str(), "Simulation");
    tree->Branch("Vertex", &vertex);

    vector<Hit> hits;
    //vector<double> hits;
    tree->Branch("Hits", &hits);

    for (int i=0; i<nEvents; i++)
    {
        vertex = event->partGeneration();
        hits.reserve(event->getVertex().getMultiplicity());
        hits = event->partTransport(detector1);
        //hits = {1., 2.};

        tree->Fill();
        hits.clear();
    }

    TFile file("data/simulation.root", "recreate");
    tree->Write();
    file.Close();

    //delete event;
    //delete tree;
}