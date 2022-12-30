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

void Reconstruction::runReconstruction(TClonesArray* hitsArray1, TClonesArray* hitsArray2){//non devi cambiarla... questa devo cambiarla tutta, TI ODIO SEMPRE DI PIù
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
  int nlayer=2;
  TFile hfile("simulation.root");
  TBranch *br[nlayer];
  TTree *tree=(TTree*)hfile.Get("simulation");
  TBranch *bv=tree->GetBranch("Vertex");
  for(int b=1;b<3;b++)//DA QUI MI HAI FATTO FARE UN ACASINO SENZA SENSO
  {
     br[b-1]=tree->GetBranch(Form("HitsL%d",b));
  }
  TClonesArray *hitsArray[nlayer] = new TClonesArray("Hit",100);
  Hit Hitrec;
  Vertex vertex;
  bv->SetAddress(&vertex);
  for(int b=1;b<3;b++)
  {
  br[b-1]->SetAddress(&hitsArray[b-1]);
  }
  for(int ev=0;ev<tree->GetEntries();ev++)
  {
        tree->GetEvent(ev);
        zVertVec.push_back(vertex.getZ());
        int numHits[nlayer];
        for(int assoreta=0; assoreta<nlayer ; assoreta++)
        {
            int numHits[assoreta]=hitsArray[assoreta]->GetEntries();  
            for(int ii=0;ii<numHits[assoreta];ii++)//smearing
                {
                    Hit *hitptr2=(Hit*)hitsArray[assoreta]->At(ii);
                    hitptr2->smearing();
                }
        
        int noi=int(gRandom->Rndm()*50);//add noise
        for(int i=numHits[assoreta]+1; i<numHits[assoreta]+noi+1; i++)
        {
            new(hitsArray[assoreta][i]) Hit();//ODDIO QUANTO TI STO ODIANDO QUI
            Hit * hit1=(Hit*)hitsArray[assoreta]->At(i);  
            * hit1->noise();                      // https://www.deviantart.com/lucdof1/art/Pikachu-with-Chainsaw-390060014
        }
  }

        runReconstruction(hitsArray[0],hitsArray[1]);
        for(int i=0;i<nlayer;i++) hitsArray[i]->Clear();
    }
        
}
    

    


  
  /*TFile hfile("recTree.root","RECREATE");
  TTree* recTree=new TTree(fTreeName2.c_str(),"tree of reconstruction");
  recTree->Branch("Phi",&Phi);
  recTree->Branch("Layer",&Li);
  recTree->Branch("Z",&Z);*/
