#include "reconstruction.hpp"
#include <TRandom3.h>
#include <vector>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include "../hit/hit.hpp"
#include "../pointCC/pointCC.hpp"
#include "../vertex/vertex.hpp"

 Reconstruction::Reconstruction()// da sistemre nomi dei rami che così non funzia, ma ci sono circa
{
  TFile hfile("simulation.root");
  TTree *tree=(TTree*)hfile.Get("OhXmasTTree");
  TBranch *b1=tree->GetBranch("Vertex");
  TBranch *b2=tree->GetBranch("Hits");
  TClonesArray *hitsArray = new TClonesArray("Hit",100);
  Vertex vertex;
  b1->SetAddress(&vertex);
  b2->SetAddress(&hitsArray);
  for(int ev=0;ev<tree->GetEntries();ev++){
      tree->GetEvent(ev);
      int numHits=hitsArray->GetEntries();

      for(int j=0; j<numHits;j++){
      Hit *hitptr=(Hit*)hitsArray->At(j);
      hitsVec.push_back(*hitptr);
    }

   zVertVec.push_back(vertex.getZ());
    
  }
}

void Reconstruction::runReconstruction()
{
    Hit Hitrec;

    for(int j=0;j<hitsVec.size();j++)
    {
        Hitrec = Hit(hitsVec[j].getX(), hitsVec[j].getY(), hitsVec[j].getZ(), hitsVec[j].getHitLayer()+1);
        Hitrec.smearing();
        hitsVec[j]=Hitrec;
    }

    int noi=int(gRandom->Rndm()*1000);
    for(int i=0;i<noi;i++)
    {
        Hitrec=Hit();
        Hitrec.noise();
        hitsVec.push_back(Hitrec);
    }

  
  /*TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);*/
}