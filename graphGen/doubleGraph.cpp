#include <Riostream.h>

#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>

void setGraph(TGraph* graph, const char * title, const char * xTitle = "x axis", 
                      const char * yTitle = "y axis", const int markerStyle = 0, const int color = 0)
{
  graph->SetTitle(title);
  graph->GetXaxis()->SetTitle(xTitle);
  graph->GetYaxis()->SetTitle(yTitle);
  graph->SetMarkerStyle(markerStyle);
  graph->SetMarkerColor(color);
}

void doubleGraph()
{
    TFile file1("output/residuesGraphs.root");
    TGraphAsymmErrors * graph1 = (TGraphAsymmErrors *)file1.Get("Graph;1");
    //graph1->SetDirectory(0);
    file1.Close();

    TFile file2("output/residuesGraphs.root");
    TGraphAsymmErrors * graph2 = (TGraphAsymmErrors *)file2.Get("Graph;2");
    //graph2->SetDirectory(0);
    file2.Close();

    setGraph(graph1, "Efficiency vs Multiplicity", "Multiplicity", "Effciency", 8, kBlue);
    setGraph(graph2, "Resolution vs Multiplicity", "Multiplicity", "Resolution [#mum]", 8, kOrange-3);

    TLegend * leg = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg->SetHeader("title", "c");
    leg->AddEntry(graph1, "Efficiency vs Multiplicity", "lep");
    leg->AddEntry(graph2, "Resolution vs Multiplicity", "lep");

    TCanvas * canvas = new TCanvas();
    graph2->Draw();
    graph1->Draw("same");
    leg->Draw();

    TFile outputFile("output/checkDoubleGraph.root", "recreate");
    canvas->Write();
    outputFile.Close();
}