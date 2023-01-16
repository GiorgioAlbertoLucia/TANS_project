#include <Riostream.h>
#include <string>
#include <vector>
#include <TH1I.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TFile.h>
#include <TObjArray.h>

#include "plotter.hpp"// è solo un abbozzo




 void Plotter::addVector(vector<double> zVertReal1, vector<double> zVertRec1, vector<double> moltReal1)
{// fixare size dei vettori
   nEvents=zVertReal1.size();
   for(int i=0;i<nEvents;i++)
   {
        zVertReal.push_back(zVertReal1[i]);
        zVertRec.push_back(zVertRec1[i]); 
        moltReal.push_back(moltReal1[i]);
   }
}



void Plotter::residues(TObjArray* arrHisto,int *Molt, int nn,double *resolution,double *resolutionErr, double *efficiency) 
{
    TFile* output1= new TFile("output/Residues.root", "recreate");
    double n=nn;    // qui n lo stai usando comunque come indice intero, quindi non ho capito
    int nHist=arrHisto->GetEntries();
    double mean[nHist];
    for(int ab=0;ab<nHist;ab++)
    {
      TH1D* hRes=(TH1D*)arrHisto->At(ab);
      if(ab==0)
      {
       for(int i=0;i<n;i++)
        {
            hRes->Fill(zVertRec[i]*10000-zVertReal[i]*10000);  
        }
      }
      else
      {
           for(int j=0;j<n;j++)
           {
               if((moltReal[j]>Molt[ab]-Molt[ab]*0.1)&&(moltReal[j]<Molt[ab]+Molt[ab]*0.1))  hRes->Fill(zVertRec[j]*10000-zVertReal[j]*10000); 
           }
      }
      resolution[ab]=hRes->GetStdDev();
      resolutionErr[ab]=hRes->GetStdDevError();
      mean[ab]=hRes->GetMean();
      hRes->Write();
      hRes->Draw("E");
      double binMax,binMin;
      binMax=hRes->GetBin(mean[ab]+3*resolution[ab]); 
      binMin=hRes->GetBin(mean[ab]-3*resolution[ab]);
      int entriesIn=0; 
      for(int t=binMin;t<BinMax;t++)
      {
        entriesIn=entriesIn+hRes->GetBinContent(t);
      }
      efficiency[ab]=hRes->GetEntries()-entriesIn;
    }
    output1->ls();  // questo non ho capito pure
    // una volta scritti i TH1D sul file credo tu li possa eliminare con delete, altrimenti occupano spazio 
    // (magari fai una prova, non vorrei dire una sciocchezzuola)
}



void Plotter::runPlots()
{
   TFile* output = new TFile("output/Reconstruction.root", "recreate"); 
   TObjArray* arrHisto = new TObjArray(); 
   int nMolt=18;
   int Molt[]={0,1,2,3,4,5,6,7,8,9,10,12,15,20,30,40,50,65};
   int arrN[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};        // se ti vuoi salvare tempo, l'internet dice che puoi fare
                                                            // int arrN[18] = {} e fai la stessa cosa
                                                            // smetto di fare il saputello ciao
   double n=zVertRec.size();
   arrN[0]=n;
   int indexh=0;

   for(int gg=1;gg<nMolt;gg++)
    {
        for(int hh=0;hh<n;hh++)
        {
            if((moltReal[hh]>Molt[gg]-Molt[gg]*0.1)&&(moltReal[hh]<Molt[gg]+Molt[gg]*0.1)) arrN[gg]++;
        }
    }

   for(int i=0;i<nMolt;i++)
   {
        TH1D* resHisto;
        resHisto =  new TH1D("resHisto",Form("Hist of Zrec-Ztrue Molt_%d",Molt[i]), int(sqrt(arrN[i])),-2000.,2000.);
        arrHisto->AddAtAndExpand(resHisto,indexh++);
   }
   double resolution[indexh];
   double resolutionErr[indexh];
   double efficiency[indexh];
   residues(arrHisto,Molt,n,resolution,resolutionErr,efficiency);
   


   


}
