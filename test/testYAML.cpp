#include <Riostream.h>

#include <TTree.h>
#include <TH1F.h>
#include <TFile.h>

#include "../yaml/Yaml.hpp"
#include "../custom_classes/detector/detector.hpp"

void testYAML()
{
    Yaml::Node root;
    Yaml::Parse(root, "config/simulationCfg.txt");

    TFile file((root["outputPaths"]["treeTestPath"].As<std::string>()).c_str(), "recreate");
    TTree * tree = new TTree((root["outputNames"]["treeSimName"].As<std::string>()).c_str(), 
                             (root["outputNames"]["treeSimName"].As<std::string>()).c_str());
    tree->Write();
    file.Close();

    TFile infile((root["inputPaths"]["distributions"].As<std::string>()).c_str());
    TH1F * th1 = (TH1F*)infile.Get("heta2");
    std::cout << th1->GetRandom() << std::endl;

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

    std::cout << (root["outputPaths"]["treeSimPath"].As<std::string>()).c_str() << std::endl;
    std::cout << root["n_detectors"].As<int>() << std::endl;
    std::cout << root["detectors"][1]["radius"].As<double>() << std::endl;
    std::cout << root["detectors"][1]["multiple_scattering"].As<bool>() << std::endl;

}