#include <Riostream.h>
#include <string>
#include <vector>
#include <fstream>

#include <TH1I.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <TInterpreter.h>
#include <TStopwatch.h>

#include "simulation.hpp"
#include "../event/event.hpp"
#include "../detector/detector.hpp"
#include "../hit/hit.hpp"
#include "../../yaml/Yaml.hpp"
#include "../recorder/recorder.hpp"

/*   STATIC DATA MEMBER  */
Simulation * Simulation::fInstancePtr = NULL;

/*       PUBLIC    */

/**
 * @brief Create instance of the singleton object.
 * 
 * @param configFile path to the configuration file for the simulation
 * @return Simulation* 
 */
Simulation* Simulation::getInstance(const char * configFile)
{
    if(fInstancePtr == NULL)    fInstancePtr = new Simulation(configFile);
    return fInstancePtr;
}

/**
 * @brief Destroy singleton object
 * 
 */
void Simulation::destroy()
{
    if(fInstancePtr)            delete fInstancePtr;
    fInstancePtr = NULL;
}

/**
 * @brief Runs a simulation for n events. For each event, a vertex is generated and particles are transported through a detector.
 * Specifics of the simulation can be set in the configuration file.
 * 
 * @param nEvents 
 */
void Simulation::runSimulation(const int nEvents = 100000)
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

    // get distributions from files
    cout << "Reading distributions from " << root["input"]["distributions"]["path"].As<std::string>() << endl;
    TFile inFile(root["input"]["distributions"]["path"].As<std::string>().c_str());

    TH1I * hMultiplicity = (TH1I*)inFile.Get(root["input"]["distributions"]["multName"].As<std::string>().c_str());
    TH1F * hEta = (TH1F*)inFile.Get(root["input"]["distributions"]["etaName"].As<std::string>().c_str());
    
    hMultiplicity->SetDirectory(0);
    hEta->SetDirectory(0);
    inFile.Close();
    
    // open file and initialize ttree
    TFile outFile(root["tree"]["simulation"]["path"].As<std::string>().c_str(), "recreate");
    TTree * tree = new TTree(root["tree"]["simulation"]["name"].As<std::string>().c_str(), 
                             root["tree"]["simulation"]["name"].As<std::string>().c_str());
    
    tree->Branch("Vertex", &vertex);

    TClonesArray * hitArrayVector[nDetectors-1];    // skip beam pipe
    for (TClonesArray * &a: hitArrayVector)  a = new TClonesArray("Hit");

    for(int i=0; i<nDetectors-1; i++)   tree->Branch(Form("HitsL%d", i+1), &hitArrayVector[i]);

    // generate events
    for(int ev=0; ev<nEvents; ev++)
    {
        if(ev%50000==0)  cout << "Processing event " << ev << "..." << endl;

        vertex = event->partGeneration(*hMultiplicity, *hEta);

        if(ev==105)        // record hits from single event in a .txt file 
        {
            string recordSimPath = root["recording"]["simulation"]["path"].As<std::string>();
            cout << "\t(Recording single event hits for a 3D model in " << recordSimPath << ")" << endl;

            Recorder * recorder = Recorder::getInstance(recordSimPath.c_str());
            recorder->beginRecordSimulation(vertex, nDetectors);
            recorder->destroy();

            for(int i=0; i<nDetectors; i++) 
            {
                if(i==0)    event->partTransportAndRecording(detectorVector[i], recordSimPath.c_str());
                else        *hitArrayVector[i-1] = event->partTransportAndRecording(detectorVector[i], recordSimPath.c_str());
            }
        }
        else
        {
            for(int i=0; i<nDetectors; i++) 
            {
                if(i==0)    event->partTransport(detectorVector[i]);
                else        *hitArrayVector[i-1] = event->partTransport(detectorVector[i]);
            }
        }   
        
        tree->Fill();
        for(int i=0; i<(nDetectors-1); i++)     hitArrayVector[i]->Clear("C");
        event->clear();
    }

    delete hMultiplicity;
    delete hEta;

    timer.Stop();    
    cout << "Simulation ended." << endl;
    cout << "Real time: " << timer.RealTime() << " s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << " s" << endl;

    timer.Reset();
    
    cout << endl;
    cout << "Writing data in a file..." << endl;
    timer.Start();
    
    outFile.Write();

    timer.Stop();
    cout << "TTree stored in " << root["tree"]["simulation"]["path"].As<std::string>() << endl;
    cout << "Real time: " << timer.RealTime() << " s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << " s" << endl;
    cout << "File size: " << outFile.GetBytesWritten()*1e-6 << " MB" << endl << endl;
    
    delete tree;
    outFile.Close();

    delete event;
}

