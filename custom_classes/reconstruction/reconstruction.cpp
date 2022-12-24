#include "reconstruction.hpp"
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

#include <TMath.h>
void Reconstruction::ReadTree()
{
  //wait in order to know the structure of the tree
}
void Reconstruction::RunReconstruction()
{
  int nEvents;
  TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);
}