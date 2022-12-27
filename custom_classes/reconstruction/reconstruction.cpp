#include "reconstruction.hpp"
#include "hit.hpp"
#include "pointCC.hpp"
#include <vector>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
 Reconstruction::Reconstruction()// da sistemre nomi dei rami che così non funzia, ma ci sono circa
{
  TFile hfile("simulation.root");
  TTree *tree=(TTree*)hfile.Get("OhXmasTTree");
  TBranch *b1=tree->GetBranch("Vertex");
  TBranch *b2=tree->GetBranch("Hits");
  TClonesArray *hitsArray = new TClonesArray("Hit",100);
  TClonesArray *vertexArray = new TClonesArray("Vertex",100);
  b1->SetAddress("&vertexArray");
  b2->SetAddress("&hitsArray");
  for(int ev=0;ev<tree->GetEntries();ev++){
    tree->GetEvent(ev);
    int numHits=hitsArray->GetEntries();
    int numVert=vertexArray->GetEntries();

    for(int j=0; j<numHits;j++){
    Hit *hitptr=(Hit*)hitsArray->At(j);
    hitsVec.push_back(hitptr);
    }
    for (int i=0;i<numVert;i++)
    {
     zVertVec.push_back(vertexArray[i].fZ)
    }
  }
}
void Reconstruction::runReconstruction()
{
<<<<<<< Updated upstream
  Hit Hitrec;
  for(int y=0;y<hitsVec.size();y++)
  {

  }
  
  /*TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);*/
=======
  TClonesArray *ptrhits = new TClonesArray("hit",100);
  int nEvents;
  TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree("Trec","tree of reconstruction");
  recTree->Branch("hits",&ptrhits);
>>>>>>> Stashed changes
}