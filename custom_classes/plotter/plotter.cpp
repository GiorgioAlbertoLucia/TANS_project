#include <Riostream.h>
#include <string>
#include <vector>
#include <TH1I.h>
#include <TH1F.h>
#include <TFile.h>
#include <TObjArray.h>

#include "plotter.hpp"// è solo un abbozzo




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



void Plotter::residues(TObjArray* arrHisto,int *Molt, int nn) 
{
    double n=nn;
    int nHist=arrHisto->GetEntries();
    
    for(int ab=0;ab<nHist;ab++)
    {
      TH1D* hRes=(TH1D*)arrHisto->At(ab);
      if(ab==0)
      {
       for(int i=0;i<n;i++)
        {
            hRes->Fill(zVertRec[i]*10000-zVertReal[i]*10000);  //microm //ci potrebbero essere delle condizioni da aggiungere perchè così viene un efficineza=1 sempre
        }
      }
      else
      {
           for(int j=0;j<n;j++)
           {
               if((moltReal[j]>Molt[ab]-Molt[ab]*0.1)&&(moltReal[j]<Molt[ab]+Molt[ab]*0.1))  hRes->Fill(zVertRec[j]*10000-zVertReal[j]*10000); 
           }
           
      }
    }
}



Plotter::runPlots()
{
   File* output = new TFile("Reconstruction.root", "recreate"); 
   TObjArray* arrHisto = new TObjArray(); 
   int nMolt=18;
   int Molt[nMolt]={0,1,2,3,4,5,6,7,8,9,10,12,15,20,30,40,50,65};
   int arrN[nMolt]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   double n=zVertRec.size();
   arrN[0]=n;

   for(int gg=1;gg<nMolt;gg++)
    {
        for(int hh=0;hh<n;hh++)
        {
            if(moltReal[hh]>Molt[gg]-Molt[gg]*0.1)&&(moltReal[hh]<Molt[gg]+Molt[gg]*0.1) arrN[gg]++;
        }
    }

   for(int i=0;i<nMolt;i++)
   {
        TH1D* resHisto;
        resHisto =  new TH1D("resHisto",Form("Hist of Zrec-Ztrue Molt_%d",Molt[i]),"Zrec-Ztrue [um]; # entries", int(sqrt(arrN[i])),-2000.,2000.);
        arrHisto->AddAtAndExpand(resHisto)
   }
   plotter.residues(arrHisto,Molt,n);

   nHisto=arrHisto->getEntries();
   double risolution[nHisto];
   for(int j=0;nHisto;j++)
   {
        risolution[j]=arraHisto[j]->GetXaxis()->GetStdDev();
        arrHisto[h]->Write();
        arrHisto[h]->draw("E");
   }

}
