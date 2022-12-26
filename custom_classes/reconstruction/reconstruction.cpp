#include "reconstruction.hpp"
#include <vector>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
 Reconstruction::Reconstruction()// da sistemre nomi dei rami che così non funzia, ma ci sono circa
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
  Hit Hitrec;
  for(int y=0;y<hitsVec.size();y++)
  {

  }
  
  /*TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);*/
}