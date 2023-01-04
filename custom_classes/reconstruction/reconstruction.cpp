#include "reconstruction.hpp"
#include <TRandom3.h>
#include <vector>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <TH1D.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include "../hit/hit.hpp"
#include "../pointCC/pointCC.hpp"
#include "../vertex/vertex.hpp"


double Reconstruction::recZvert(Hit *hit1,Hit *hit2)//return z from tracking's line
{
    double m,n,y = 0.;
    m = hit2->getY()-hit1->getY();
    n = hit2->getZ()-hit1->getZ();
    return n*(y-hit2->getY())/m + hit2->getZ(); //from 3D line equations
}



void Reconstruction::residues() //forse tutta la funzione è da spostare in plotter. *** please help!!!!
{
    double n=zVertVecRec.size();
    TH1D* resHisto;//questo va in Plotter fino a riga 40
    resHisto = new TH1D("resHisto","Histo of Zrec-Ztrue",int(sqrt(n)),-2000.,2000.);
    for(int i=0;i<n;i++)
    {
        resHisto->Fill(zVertVecRec[i]*10000-zVertVec[i]*10000);  //microm //ci potrebbero essere delle condizioni da aggiungere perchè così viene un efficineza=1 sempre
    }
    TCanvas* c1=new TCanvas("c1","Residues",1650,900);  
    c1->cd();
    resHisto->SetLineColor(kBlue);//differenza per tutte le moltiplicità 
    resHisto->Draw("E");
    cout<<"number of events: "<<n<<endl; //controllo per vedere se funziona corretamente
    int nMolt=17;
    int Molt[nMolt]={1,2,3,4,5,6,7,8,9,10,12,15,20,30,40,50,60};
    double sigma[nMolt];
    for(int i=0;i<nMolt;i++)
    {
        if(i<12)
        {
            TH1D* resHistoMolt;
            resHistoMolt = new TH1D("resHisto","Histo of Zrec-Ztrue",int(sqrt(n)),-2000.,2000.);
            for(int j=0;j<n;j++)
            {
                if((zMoltVec[j]>Molt[i]+0.5)&&(zMoltVec[j]<Molt[i]-0.5))  resHistoMolt->Fill(zVertVecRec[j]*10000-zVertVec[j]*10000); 
            }
            TF1* g1=new TF1("g1","gaus",-2000,2000.);//estremi possono essere fatti meglio, ma ho sonno ora
            resHistoMolt->Fit("g1","L");
            sigma[i]=g1->GetParameter(2); //devo aggiungere condizioni su quelli non gaussiani, devo prendere l'RMS dell'histo
        }

        else
        {
            TH1D* resHistoMolt;
            resHistoMolt = new TH1D("resHisto","Histo of Zrec-Ztrue",int(sqrt(n)),-2000.,2000.);
            for(int j=0;j<n;j++)
            {
                if((zMoltVec[j]>Molt[i]+Molt[i]*0.1)&&(zMoltVec[j]<Molt[i]*0.1))  resHistoMolt->Fill(zVertVecRec[j]*10000-zVertVec[j]*10000); 
            }
            TF1* g1=new TF1("g1","gaus",-2000,2000.);//estremi possono essere fatti meglio, ma ho sonno ora
            resHistoMolt->Fit("g1","L");
            sigma[i]=g1->GetParameter(2); // le sigma solo le risoluzioni 
        }
    }
}



void Reconstruction::runReconstruction(TClonesArray hitsArray1, TClonesArray hitsArray2){//loop on points for the vertex's reconstruction
    double phi = 0.;
    double deltaPhi = 0.01; 
    
    double ztemp = 0;
    vector<double> zTrackVert;
    TH1D* histoHit;
    double binW = 0.5;
    histoHit = new TH1D("histoHit","Vertex's z rec",int(27/binW),0.,27.);  
                                                                            
    for(int i=0; i<hitsArray1.GetEntries(); i++)                            
    {
        Hit *hitptr=(Hit*)hitsArray1.At(i);
        phi = hitptr->getPhi();
        for(int j=0; j<hitsArray2.GetEntries(); j++)
        {
            Hit *hitptr1=(Hit*)hitsArray2.At(j);
            if((hitptr1->getPhi()<phi+deltaPhi)&&(hitptr1->getPhi()>phi-deltaPhi))
            {
                ztemp = recZvert( hitptr,hitptr1);
                histoHit->Fill(ztemp);
                zTrackVert.push_back(ztemp);
            }
        }
    }

        int binmax = histoHit->GetMaximumBin();
        double zMax = histoHit->GetXaxis()->GetBinCenter(binmax);

        vector<double> zTrackVert1;
        for(int aa=0; aa<(int)zTrackVert.size(); aa++)          // ti volevo dire che esiste la sintassi (per i vector, non so se anche per array)
                                                                // for(double z: zTrackVert) { if((z < (zMax+binW/2.)) && ... ) ...}  // ou fatti i fatti tuoi, smettila di deprimermi!!!
        {
            if((zTrackVert[aa]<zMax+binW/2)&&(zTrackVert[aa]>zMax-binW/2)) zTrackVert1.push_back(zTrackVert[aa]);
        }
        double som = 0.;
        for(int a=0; a<(int)zTrackVert1.size(); a++) som = som+zTrackVert1[a];

        zVertVecRec.push_back(som/zTrackVert1.size());
    }



void Reconstruction::loadHits()
{ 
  int nlayer = 2;
  TFile hfile("data/simulation.root");
  TBranch *br[nlayer];
  TTree *tree = (TTree*)hfile.Get("simulation");
  TBranch *bv = tree->GetBranch("Vertex");

  for(int b=1; b<3; b++)//DA QUI MI HAI FATTO FARE UN CASINO SENZA SENSO
  {
     br[b-1]=tree->GetBranch(Form("HitsL%d",b));
  }

  TClonesArray hitsArray[nlayer];
  for(int yy=0; yy<nlayer; yy++)  hitsArray[yy] = TClonesArray("Hit",100); 
  Vertex vertex;
  bv->SetAddress(&vertex);

  for(int b=1; b<3; b++)
  {
        br[b-1]->SetAddress(&hitsArray[b-1]);
  }
  for(int ev=0; ev<tree->GetEntries(); ev++)
  {
        tree->GetEvent(ev);
        zVertVec.push_back(vertex.getZ());
        zMoltVec.push_back(vertex.getMultiplicity());
        int numHits[nlayer];
        for(int limmortaccitua=0; limmortaccitua<nlayer; limmortaccitua++)
        { 
            numHits[limmortaccitua] = hitsArray[limmortaccitua].GetEntries();  
            for(int ii=0;ii<numHits[limmortaccitua];ii++)//smearing
                {
                    Hit *hitptr2 = (Hit*)hitsArray[limmortaccitua].At(ii);
                    hitptr2->smearing();
                }
        
            int noi=int(gRandom->Rndm()*50);//add noise
            for(int i=numHits[limmortaccitua]+1; i<numHits[limmortaccitua]+noi+1; i++)
            {
                new(hitsArray[limmortaccitua][i]) Hit();//ODDIO QUANTO TI STO ODIANDO QUI
                Hit * hit1 = (Hit*)hitsArray[limmortaccitua].At(i);  
                hit1->noise();                     
            }
        }

        runReconstruction(hitsArray[0], hitsArray[1]);
        for(int i=0; i<nlayer; i++) hitsArray[i].Clear();
    }
}
    

    



