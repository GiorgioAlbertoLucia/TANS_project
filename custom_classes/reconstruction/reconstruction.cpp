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

double Reconstruction::recZvert(Hit& hit1,Hit& hit2)//return z of rec vertex
{

}

void Reconstruction::runReconstruction(){
    double phi=0.;//loop on point for the vertex's reconstruction
    double deltaPhi=0.087; // 5 degres
    for(int i=0;i<hitsVec.size();i++)
    {
        if((hitsVec[i].getHitLayer()+1)==1)
        {
            phi=hitsVec[i].getPhi();
            for(int j=0;j<hitsVec.size();j++)
            {
                if(((hitsVec[j].getHitLayer()+1)==2)&&(hitsVec[j].getPhi()<phi+deltaPhi)&&(hitsVec[j].getPhi()>phi-deltaPhi))
                recZvert( hitsVec[i], hitsVec[j]);//to add in a histo
            }
        }
    }
}



void Reconstruction::loadHits()
{ 
  TFile hfile("simulation.root");
  TTree *tree=(TTree*)hfile.Get("OhXmasTTree");
  TBranch *b1=tree->GetBranch("Vertex");
  TBranch *b2=tree->GetBranch("Hits");
  TClonesArray *hitsArray = new TClonesArray("Hit",100);
  Hit Hitrec;
  Vertex vertex;
  b1->SetAddress(&vertex);
  b2->SetAddress(&hitsArray);

  for(int ev=0;ev<tree->GetEntries();ev++)
  {
        tree->GetEvent(ev);
        int numHits=hitsArray->GetEntries();
        zVertVec.push_back(vertex.getZ());

        for(int j=0; j<numHits;j++){
        Hit *hitptr=(Hit*)hitsArray->At(j);
        hitsVec.push_back(*hitptr);
        }

        for(int ii=0;ii<hitsVec.size();ii++)
        {
            hitsVec[ii].smearing();
        }

        int noi=int(gRandom->Rndm()*1000);//add noise

        for(int i=0;i<noi;i++)
        {
        Hitrec=Hit();
        Hitrec.noise();
        hitsVec.push_back(Hitrec);
        }
        
        runReconstruction();
    }
}
    


  
  /*TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);*/
