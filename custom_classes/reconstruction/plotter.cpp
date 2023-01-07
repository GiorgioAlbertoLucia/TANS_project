#include <Riostream.h>
#include <string>
#include <vector>
#include <TH1I.h>
#include <TH1F.h>
#include <TFile.h>

#include "plotter.hpp"// è solo un abbozzo
#include "../reconstruction/recosntruction.hpp"

Plotter::Plotter(vector<double> zVertReal1, vector<double> zVertRec1, vector<double> moltReal1)
{
   nEvents=zVertReal.size();
   for(int i=0;i<nEvents;i++)
   {
        zVertReal.push_back(zVertReal1[i]);
        zVertRec.push_back(zVertRec1[i]); 
        moltReal.push_back(moltReal[i]);
   }
   File* output = new TFile("Reconstruction.root", "recreate"); 
}