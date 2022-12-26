#include "reconstruction.hpp"
#include <vector>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
 Reconstruction::Reconstruction()
{
  TFile hfile("simulation.root");
  TTree *tree=(TTree*)hfile.Get("OhXmasTTree");
  TBranch *b1=tree->GetBranch("Vertex");
  TBranch *b2=tree->GetBranch("Hits");
  b1->SetAddress("&Vertex");
  b2->SetAddress("&Hits");
  for(int ev=0;ev<tree->GetEntries();ev++){
    tree->GetEvent(ev);
    int numHits=Hits->GetEntries();
    for(int j; j<numHits;j++){
    Hit *hitptr=(Hit*)Hits->At(j);
    hitsVec.push_back(hitptr);
    zVec.push_back(hitptr->getZ());
    }
  }
}
void Reconstruction::runReconstruction()
{
  
  /*int nEvents;
  TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);*/
}