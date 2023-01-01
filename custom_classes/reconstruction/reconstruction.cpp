#include "reconstruction.hpp"
#include <TRandom3.h>
#include <vector>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <TH1D.h>
#include "../hit/hit.hpp"
#include "../pointCC/pointCC.hpp"
#include "../vertex/vertex.hpp"


void Reconstruction::runReconstruction(TClonesArray hitsArray1, TClonesArray hitsArray2){//loop on points for the vertex's reconstruction
    double phi=0.;
    double deltaPhi=0.087; // 5 degres, not sure
    double ztemp=0;
    vector<double> zTrackVert;
    TH1D* histoHit;
    double binW=0.5;
    histoHit=new TH1D("histoHit","Vertex's z rec",int(27*binW),0.,27.); //bin's number maybe has to be changed

    for(int i=0;i<hitsArray1.GetEntries();i++)
    {
        Hit *hitptr=(Hit*)hitsArray1.At(i);
            phi=hitptr->getPhi();
            for(int j=0;j<hitsArray2.GetEntries();j++)
            {
                Hit *hitptr1=(Hit*)hitsArray2.At(j);
                if((hitptr1->getPhi()<phi+deltaPhi)&&(hitptr1->getPhi()>phi-deltaPhi))
                {
                    ztemp=recZvert( hitptr,hitptr1);
                    histoHit->Fill(ztemp);
                    zTrackVert.push_back(ztemp);
                }
            }
        }

        int binmax=histoHit->GetMaximumBin();
        double zMax=histoHit->GetXaxis()->GetBinCenter(binmax);

        vector<double> zTrackVert1;
        for(int aa=0;aa<zTrackVert.size();aa++)
        {
            if((zTrackVert[aa]<zMax+binW/2)&&(zTrackVert[aa]>zMax-binW/2)) zTrackVert1.push_back(zTrackVert[aa]);
        }
        double som=0.;
        for(int a=0;a<zTrackVert1.size();a++) som=som+zTrackVert1[a];

        zVertVecRec.push_back(som/zTrackVert1.size());
    }



void Reconstruction::loadHits()
{ 
  int nlayer=2;
  TFile hfile("simulation.root");
  TBranch *br[nlayer];
  TTree *tree=(TTree*)hfile.Get("simulation");
  TBranch *bv=tree->GetBranch("Vertex");

  for(int b=1;b<3;b++)//DA QUI MI HAI FATTO FARE UN CASINO SENZA SENSO
  {
     br[b-1]=tree->GetBranch(Form("HitsL%d",b));
  }

  TClonesArray hitsArray[nlayer];
  for(int yy=0;yy<nlayer;yy++)  hitsArray[yy] = TClonesArray("Hit",100); // bho non so che dare e tu su quetso sei + skillato fa così: error: no viable overloaded '='
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
        for(int limmortaccitua=0; limmortaccitua<nlayer ; limmortaccitua++)
        { 
            numHits[limmortaccitua]=hitsArray[limmortaccitua].GetEntries();  
            for(int ii=0;ii<numHits[limmortaccitua];ii++)//smearing
                {
                    Hit *hitptr2=(Hit*)hitsArray[limmortaccitua].At(ii);
                    hitptr2->smearing();
                }
        
        int noi=int(gRandom->Rndm()*50);//add noise
        for(int i=numHits[limmortaccitua]+1; i<numHits[limmortaccitua]+noi+1; i++)
        {
            new(hitsArray[limmortaccitua][i]) Hit();//ODDIO QUANTO TI STO ODIANDO QUI
            Hit * hit1=(Hit*)hitsArray[limmortaccitua].At(i);  
            hit1->noise();                     
        }
  }

        runReconstruction(hitsArray[0],hitsArray[1]);
        for(int i=0;i<nlayer;i++) hitsArray[i].Clear();
    }
        
}
    

    



