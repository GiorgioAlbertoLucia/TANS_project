#include <Riostream.h>
#include <string>

#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>

#include "../yaml/Yaml.hpp"

void setGraph(TGraph* graph, const char * title, const char * xTitle = "x axis", 
                      const char * yTitle = "y axis", const int markerStyle = 0, const int color = 0)
{
  graph->SetTitle(title);
  graph->GetXaxis()->SetTitle(xTitle);
  graph->GetYaxis()->SetTitle(yTitle);
  graph->SetMarkerStyle(markerStyle);
  graph->SetMarkerColor(color);
}

/**
 * @brief Macro to draw two TGraph on a single canvas and set essential features. Options can be
 * set in a configuration file.
 * 
 * @param configFile path to the configuration file
 */
//void doubleGraph(const char * configFile = "config/configDoubleGraph.txt")
void doubleGraph()
{
    Yaml::Node root;
    Yaml::Parse(root, "config/configDoubleGraph.txt");

    // load graphs from file
    TFile file1(root["graph"][0]["filePath"].As<string>().c_str());
    TGraphAsymmErrors * graph1 = (TGraphAsymmErrors *)file1.Get(root["graph"][0]["name"].As<string>().c_str());
    file1.Close();

    TFile file2(root["graph"][1]["filePath"].As<string>().c_str());
    TGraphAsymmErrors * graph2 = (TGraphAsymmErrors *)file2.Get(root["graph"][1]["name"].As<string>().c_str());
    file2.Close();
    
    // graphs settings
    string title = root["general"]["title"].As<string>();
    string xAxis = root["general"]["xAxis"].As<string>();
    string yAxis = root["general"]["yAxis"].As<string>() + " [#mum]";

    setGraph(graph1, title.c_str(), xAxis.c_str(), yAxis.c_str(), root["graph"][0]["marker"].As<int>(), root["graph"][0]["color"].As<int>());
    setGraph(graph2, title.c_str(), xAxis.c_str(), yAxis.c_str(), root["graph"][1]["marker"].As<int>(), root["graph"][1]["color"].As<int>());
    
    // set legend
    TLegend * leg = new TLegend(root["legend"]["xmin"].As<double>(), root["legend"]["ymin"].As<double>(),
                                root["legend"]["xmax"].As<double>(), root["legend"]["ymax"].As<double>());
    leg->SetHeader(root["legend"]["title"].As<string>().c_str(), "c");
    leg->AddEntry(graph1, root["legend"]["entry1"].As<string>().c_str(), "lep");
    leg->AddEntry(graph2, root["legend"]["entry2"].As<string>().c_str(), "lep");

    // draw on canvas
    TCanvas * canvas = new TCanvas("canvas", "", root["canvas"]["width"].As<double>(), root["canvas"]["height"].As<double>());
    if(root["canvas"]["grid"].As<bool>()) canvas->SetGrid();

    graph2->Draw();
    graph1->Draw("same");
    leg->Draw();

    // save in file
    TFile outputFile(root["outputPath"].As<string>().c_str(), "recreate");
    canvas->Write();
    outputFile.Close();
    //canvas->SaveAs(root["canvas"]["outputPath"].As<string>().c_str());
}