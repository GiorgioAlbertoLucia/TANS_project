#include <Riostream.h>

#include <TTree.h>
#include <TH1F.h>
#include <TFile.h>

#include "../yaml/Yaml.hpp"
#include "../custom_classes/detector/detector.hpp"

void testYAML()
{
    Yaml::Node root;
    Yaml::Parse(root, "data/recordSimulation.txt");

    std::cout << root["Vertex"]["x"].As<double>() << std::endl;
    std::cout << root["Vertex"]["multiplicity"].As<int>() << std::endl;
    //std::cout << root["DetectorLayers"][0]["Particles"][0]["x"].As<double>() << std::endl;
    //std::cout << root["DetectorLayers"][0]["Particles"][0]["y"].As<double>() << std::endl;
    //std::cout << root["DetectorLayers"][0]["Particles"][1]["x"].As<double>() << std::endl;

}