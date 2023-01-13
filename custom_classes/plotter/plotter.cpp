#include <Riostream.h>
#include <string>
#include <vector>
#include <TH1I.h>
#include <TH1F.h>
#include <TFile.h>
#include <TObjArray.h>

#include "plotter.hpp"// è solo un abbozzo
#include "../reconstruction/recosntruction.hpp"



Plotter::addVector(vector<double> zVertReal1, vector<double> zVertRec1, vector<double> moltReal1)
{
   nEvents=zVertReal.size();
   for(int i=0;i<nEvents;i++)
   {
        zVertReal.push_back(zVertReal1[i]);
        zVertRec.push_back(zVertRec1[i]); 
        moltReal.push_back(moltReal[i]);
   }
}



void Plotter::residues() 
{
    int nMolt=18;
    int Molt[nMolt]={0,1,2,3,4,5,6,7,8,9,10,12,15,20,30,40,50,65};
    int arrN[nMolt]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    TObjArray* arrHisto = new TObjArray();
    double n=zVertRec.size();
    arrN[0]=n;
    for(int gg=1;gg<nMolt;gg++)
    {
        for(int hh=0;hh<n;hh++)
        {
            if(moltReal[hh]>Molt[gg]-Molt[gg]*0.1)&&(moltReal[hh]<Molt[gg]+Molt[gg]*0.1) arrN[gg]++;
        }
    }
    
    for(int ab=0;ab<nMolt;ab++)
    {
      if(ab==0){
        
        TH1D* resHisto;
        resHisto = new TH1D("resHisto","Histo of Zrec-Ztrue",int(sqrt(arrN[ab])),-2000.,2000.);
        for(int i=0;i<n;i++)
        {
            resHisto->Fill(zVertRec[i]*10000-zVertReal[i]*10000);  //microm //ci potrebbero essere delle condizioni da aggiungere perchè così viene un efficineza=1 sempre
        }
        resHisto->SetLineColor(kBlue);//differenza per tutte le moltiplicità 
        resHisto->Draw("E");
      }
      else
       {
            TH1D* resHistoMolt;
            resHistoMolt = new TH1D("resHisto","Histo of Zrec-Ztrue",int(sqrt(n)),-2000.,2000.);
            for(int j=0;j<n;j++)
            {
                if((moltReal[j]>Molt[i]-Molt[i]*0.1)&&(moltReal[j]<Molt[i]+Molt[i]*0.1))  resHistoMolt->Fill(zVertRec[j]*10000-zVertReal[j]*10000); 
            }
            TF1* g1=new TF1("g1","gaus",-2000,2000.);//estremi possono essere fatti meglio, ma ho sonno ora
            resHistoMolt->Fit("g1","L");
            sigma[i]=g1->GetParameter(2); // le sigma solo le risoluzioni 
       }
    }
}



Plotter::runPlots()
{
   File* output = new TFile("Reconstruction.root", "recreate");  
   //gli restituisci l'array conn gli histogrammi e ti prendi 
   
}
