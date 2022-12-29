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

double Reconstruction::recZvert(Hit *hit1,Hit *hit2)//return z of rec vertex
{

}

void Reconstruction::runReconstruction(TClonesArray* hitsArray){
    double phi=0.;//loop on point for the vertex's reconstruction
    double deltaPhi=0.087; // 5 degres
    
    for(int i=0;i<hitsArray->GetEntries();i++)
    {
        Hit *hitptr=(Hit*)hitsArray->At(i);
        if((hitptr->getHitLayer()+1)==1)
        {
            phi=hitptr->getPhi();
            for(int j=0;j<hitsArray->GetEntries();j++)
            {
                Hit *hitptr1=(Hit*)hitsArray->At(j);
                if(((hitptr1->getHitLayer()+1)==2)&&(hitptr1->getPhi()<phi+deltaPhi)&&(hitptr1->getPhi()>phi-deltaPhi))
                recZvert( hitptr,hitptr1);//to add in a histo
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

        for(int ii=0;ii<numHits;ii++)//smearing
        {
            Hit *hitptr2=(Hit*)hitsArray->At(ii);
            hitptr2->smearing();
        }

        int noi=int(gRandom->Rndm()*100);//add noise
        for(int i=numHits+1; i<numHits+noi+1; i++)
        {
            Hitrec=Hit();
            Hitrec.noise();
            new(hitsArray[i])Hitrec;//QUESTO POI LO SISTEMO, è l'unica cosa che non va!!! ORA TRA ARRAY E PUNTATORI STO SMATTANDO QUINDI VADO A FARE CAMPI
        }

        runReconstruction(hitsArray);
        hitsArray->Clear();
    }
        
}
    

    


  
  /*TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);*/
