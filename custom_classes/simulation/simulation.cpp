#include <Riostream.h>
#include <string>
#include <vector>
#include <fstream>

#include <TH1I.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TInterpreter.h>

#include "simulation.hpp"
#include "../event/event.hpp"
#include "../detector/detector.hpp"
#include "../hit/hit.hpp"
#include "../../yaml/Yaml.hpp"

/*      PROTECTED   */

/**
 * @brief Creates a .txt file containing information on a vertex
 * 
 * @param vertex 
 * @param filePath 
 */
void Simulation::recordVertex(Vertex* vertex, const char * filePath) const
{
    ofstream file(filePath);

    file << "Vertex:" << endl;
    file << "    " << "x: " << vertex->getX() << " #cm" << endl;
    file << "    " << "y: " << vertex->getY() << " #cm" << endl;
    file << "    " << "z: " << vertex->getZ() << " #cm" << endl;
    file << "    " << "multiplicity: " << vertex->getMultiplicity() << endl;

    file << endl;
    file << "DetectorLayers:" << " # first layer is the beam pipe" << endl;

    file.close();
}

/*       PRIVATE    */

/**
 * @brief Runs a simulation for n events. For each event, a vertex is generated and particles are transported through a detector.
 * Specifics of the simulation can be set in the configuration file.
 * 
 * @param nEvents 
 */
void Simulation::runSimulation(const int nEvents)
{
    cout << "--------------------------------------" << endl;
    cout << "Begin simulation..." << endl;

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
    TTree * tree = new TTree(root["outputNames"]["treeSimName"].As<std::string>().c_str(), 
                             root["outputNames"]["treeSimName"].As<std::string>().c_str());
    
    tree->Branch("Vertex", &vertex);

    vector<TClonesArray> hitArrayVector;
    hitArrayVector.reserve(nDetectors);

    for(int i=0; i<nDetectors; i++)
    {
        TClonesArray hitArray("Hit");    
        hitArrayVector.push_back(hitArray);
        tree->Branch(Form("HitsL%d", i), &hitArrayVector[i]);
    }

    // read input distributions
    TFile inFile(root["inputPaths"]["distributions"].As<std::string>().c_str());

    TH1I * hMultiplicity = new TH1I("hMultiplicity", "check", 100, 0, 10);
    TH1F * hEta = (TH1F*)inFile.Get("heta2");
    
    hMultiplicity->SetDirectory(0);
    hEta->SetDirectory(0);
    inFile.Close();

    // process events (vertex generation and particle transport through all layers of the detector)
    for(int ev=0; ev<nEvents; ev++)
    {
        if(ev%1000==0)  cout << "Processing event " << ev << "..." << endl;

        vertex = event->partGeneration(*hMultiplicity, *hEta);

        if(ev==0)        // record hits from first event in a .txt file 
        {
            cout << "Recording first event hits for a 3D model..." << endl;

            this->recordVertex(&vertex, root["outputPaths"]["recordSimPath"].As<std::string>().c_str());
            for(int i=0; i<nDetectors; i++) 
            {hitArrayVector[i] = event->partTransport2(detectorVector[i], true, root["outputPaths"]["recordSimPath"].As<std::string>());}
        }
        else    for(int i=0; i<nDetectors; i++) hitArrayVector[i] = event->partTransport2(detectorVector[i], false);
        
        tree->Fill();
        for(TClonesArray clone: hitArrayVector) clone.Clear();
        event->clear();
    }

    delete hMultiplicity;
    delete hEta;
    
    
    TFile outFile(root["outputPaths"]["treeSimPath"].As<std::string>().c_str(), "recreate");
    tree->Write();
    outFile.Close();
    cout << "TTree stored in " << root["outputPaths"]["treeSimPath"].As<std::string>() << endl;

    delete event;
    delete tree;
}

void Simulation::runSimulation2(const int nEvents)
{
    
    Event * event = new Event();
    Vertex vertex;
    Detector detector1 = {3., 0.08, 27., false};

    //TTree * tree = new TTree(fTreeName.c_str(), "Simulation");
    TTree * tree = new TTree("OhXmasTTree", "OhXmasTTree");
    tree->Branch("Vertex", &vertex);

    gInterpreter->GenerateDictionary("vector<Hit>", "vector");
    vector<Hit> hits;
    tree->Branch("Hits", &hits);
    
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
