#include <Riostream.h>
#include <string>
#include <vector>
#include <fstream>

#include <TH1I.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TInterpreter.h>
#include <TStopwatch.h>

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
    cout << "Begin simulation..." << endl << endl;

    TStopwatch timer;
    timer.Start();

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
    hitArrayVector.reserve(nDetectors-1);   // skip beam pipe

    for(int i=0; i<nDetectors-1; i++)         // nDetectors-1: skip beam pipe
    {
        TClonesArray hitArray("Hit");    
        hitArrayVector.push_back(hitArray);
        tree->Branch(Form("HitsL%d", i+1), &hitArrayVector[i]);
    }

    cout << "Reading distributions from " << root["inputPaths"]["distributions"].As<std::string>() << endl;
    TFile inFile(root["inputPaths"]["distributions"].As<std::string>().c_str());

    TH1I * hMultiplicity = (TH1I*)inFile.Get("hmul");
    TH1F * hEta = (TH1F*)inFile.Get("heta");
    
    hMultiplicity->SetDirectory(0);
    hEta->SetDirectory(0);
    inFile.Close();


    for(int ev=0; ev<nEvents; ev++)
    {
        if(ev%10000==0)  cout << "Processing event " << ev << "..." << endl;

        vertex = event->partGeneration(*hMultiplicity, *hEta);

        if(ev==0)        // record hits from first event in a .txt file 
        {
            cout << "\t(Recording first event hits for a 3D model)" << endl;
            string recordSimPath = root["outputPaths"]["recordSimPath"].As<std::string>();

            this->recordVertex(&vertex, recordSimPath.c_str());
            for(int i=0; i<nDetectors; i++) 
            {
                if(i==0)    event->partTransport2(detectorVector[i], true, recordSimPath);
                else        hitArrayVector[i-1] = event->partTransport2(detectorVector[i], true, recordSimPath);
            }
        }
        else
        {
            for(int i=0; i<nDetectors; i++) 
            {
                if(i==0)    event->partTransport2(detectorVector[i], false);
                else        hitArrayVector[i-1] = event->partTransport2(detectorVector[i], false);
            }
        }   
        
        tree->Fill();
        for(TClonesArray clone: hitArrayVector) clone.Clear();
        event->clear();
    }

    delete hMultiplicity;
    delete hEta;

    timer.Stop();    
    cout << "Simulation ended." << endl;
    cout << "Real time: " << timer.RealTime() << "s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << "s" << endl;

    timer.Reset();
    
    cout << endl;
    cout << "Writing data in a file..." << endl;
    timer.Start();
    
    TFile outFile(root["outputPaths"]["treeSimPath"].As<std::string>().c_str(), "recreate");
    tree->Write();

    timer.Stop();
    cout << "TTree stored in " << root["outputPaths"]["treeSimPath"].As<std::string>() << endl;
    cout << "Real time: " << timer.RealTime() << "s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << "s" << endl;
    cout << "File size:" << outFile.GetBytesWritten()*1e-6 << " MB" << endl;

    outFile.Close();

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

    //gInterpreter->GenerateDictionary("vector<Hit>", "vector");
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
