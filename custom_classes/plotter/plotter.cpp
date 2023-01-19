#include <Riostream.h>
#include <string>
#include <vector>
#include <TH1I.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TFile.h>
#include <TObjArray.h>
#include<TGraphErrors.h>

#include "plotter.hpp" 



/**
 * @brief  copy vectors in plotter DM
 * 
 * @param zVertReal1 
 * @param zVertRec1 
 * @param moltReal1 
 */
 void Plotter::addVector(vector<double> &zVertReal1, vector<double> &zVertRec1, vector<double> &moltReal1) // e proprio non va provo con i puntatori
{
   nEvents=zVertReal1.size();
   cout<<"nEvents="<<nEvents<<endl;
   zVertReal.reserve(nEvents);
   zVertRec.reserve(nEvents);
   moltReal.reserve(nEvents);
   resVec.reserve(nEvents);

   for(int i=0;i<nEvents;i++)
   {
    if(i==0) cout<<" z reale plotter="<<zVertReal1[i]<<" plotter z rec="<<zVertRec1[i]<<"plotter residuo="<<zVertRec1[i]*10000-zVertReal1[i]*10000<<"plotter molt="<<moltReal1[i]<<endl;
        zVertReal.push_back(zVertReal1[i]);
        zVertRec.push_back(zVertRec1[i]); 
        moltReal.push_back(moltReal1[i]);
        resVec.push_back(zVertRec1[i]*10000-zVertReal1[i]*10000);
   }
}



/**
 * @brief histos for residues, calculation of resolution and efficiency. 
 * 
 * @param arrHisto 
 * @param Xarray 
 * @param nn 
 * @param resolution 
 * @param resolutionErr 
 * @param efficiency 
 * @param efficiencyErr 
 * @param bol 
 */
void Plotter::residues(TObjArray* arrHisto,double *Xarray, int n,double *resolution,double *resolutionErr, double *efficiency, double *efficiencyErr, bool bol) 
{
    int nHist=arrHisto->GetEntries();
    double mean[nHist];
    double bW=2.;
    if(bol==true) TFile* output1= new TFile("Residues.root", "recreate");
    for(int ab=0;ab<nHist;ab++)
    {
      TH1D* hRes=(TH1D*)arrHisto->At(ab);
      if(bol==true)
      {
        if(ab==0)
        {
          for(int i=0;i<n;i++)
          {
            if(i==0) cout<<"residuo2="resVec[i]<<endl;
            hRes->Fill(resVec[i]);  
          }
        }
        else 
        {
           for(int j=0;j<n;j++)
           {
               if((moltReal[j]>Xarray[ab]-Xarray[ab]*0.1)&&(moltReal[j]<Xarray[ab]+Xarray[ab]*0.1))  hRes->Fill(resVec[j]); 
           }
        }
        hRes->Write();
        hRes->Draw("E");
      }
      else
      {
        for(int ii=0;ii<n;ii++)
        {
          if((zVertReal[ii]>Xarray[ab]-bW)&&(zVertReal[ii]<Xarray[ab]+bW))  hRes->Fill(resVec[ii]);
        }
      }
      resolution[ab]=hRes->GetStdDev();
      resolutionErr[ab]=hRes->GetStdDevError();
      mean[ab]=hRes->GetMean();
      double binMax,binMin;
      binMax=hRes->GetBin(mean[ab]+3*resolution[ab]); 
      binMin=hRes->GetBin(mean[ab]-3*resolution[ab]);
      int entriesIn=0; 
      for(int t=binMin;t<binMax;t++)
      {
        entriesIn=entriesIn+hRes->GetBinContent(t);
      }
      efficiency[ab]=entriesIn/hRes->GetEntries();
      efficiencyErr[ab]=0.;
    }
    //if(bol==true) output1->ls();
}



/**
 * @brief create and save Graph of resolution and efficiency vs Ztrue and Moltiplicity of vertex
 * 
 */
void Plotter::runPlots()
{
   TFile* output = new TFile("Reconstruction.root", "recreate"); 
   TObjArray* arrHisto = new TObjArray(); 
   
   int nMolt=18;
   double Molt[]={0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,12.,15.,20.,30.,40.,50.,65.};
   double errMolt[nMolt]; 
   for(int c=0;c<nMolt;c++)
   {
    errMolt[c]=sqrt(molt[c]);
   }

   int arrN[]={10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
   
   arrN[0]=nEvents;
   int indexh=0;

   for(int gg=1;gg<nMolt;gg++)
    {
        for(int hh=0;hh<nEvents;hh++)
        {
            if((moltReal[hh]>Molt[gg]-Molt[gg]*0.1)&&(moltReal[hh]<Molt[gg]+Molt[gg]*0.1)) arrN[gg]++;
        }
    }

   for(int i=0;i<nMolt;i++)
   {
        TH1D* resHisto;
        // check
        resHisto =  new TH1D(Form("resHisto%d", i),Form("Hist of Zrec-Ztrue Molt_%4.1f",Molt[i]), int((arrN[i])),-2000.,2000.);
        //resHisto =  new TH1D(Form("resHisto%d", i),Form("Hist of Zrec-Ztrue Molt_%4.1f",Molt[i]), int(sqrt(arrN[i])),-2000.,2000.);
        arrHisto->AddAtAndExpand(resHisto,indexh++);
   }

   double resolutionM[indexh];
   double resolutionErrM[indexh];
   double efficiencyM[indexh];
   double efficiencyErrM[indexh];
   bool bol=true;
   residues(arrHisto,Molt,nEvents,resolutionM,resolutionErrM,efficiencyM,efficiencyErrM,bol);
  
   TGraphErrors *effmolt = new TGraphErrors(indexh,Molt,efficiencyM,errMolt,efficiencyErrM); //auto gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
   effmolt->SetTitle("Efficiency vs Moltiplicity");
   effmolt->GetXaxis()->SetTitle("Molticplicity");
   effmolt->GetYaxis()->SetTitle("Efficiency");
   effmolt->SetMarkerStyle(8);
   effmolt->SetMarkerColor(kBlue);
   effmolt->Draw();
   effmolt->Write();

   TGraphErrors *resmolt = new TGraphErrors(indexh,Molt,resolutionM,errMolt,resolutionErrM);
   resmolt->SetTitle("Resolution vs Moltiplicity");
   resmolt->GetXaxis()->SetTitle("Molticplicity");
   resmolt->GetYaxis()->SetTitle("Resolution [#mum]");
   resmolt->SetMarkerStyle(8);
   resmolt->SetMarkerColor(kOrange-3);
   resmolt->Draw();
   resmolt->Write();//da scivere a MAsera nel readme, a molteplicità 0 ho messo quelli senza distinzione di molteplicità

   arrHisto->Clear();

   double bW=2;
   double midZ[int(60./bW)];
   TH1D* histoZreal;
   histoZreal = new TH1D("histoZreal","Z of real vertex",int(60/bW),-30.,30.);

   for(int y=0;y<nEvents;y++)
   {
    if (y==0) cout<<"xvertreal="<<zVertReal[y]<<endl;
      histoZreal->Fill(zVertReal[y]); 
   }

   double errZmid[int(60./bW)];
   for(int j=0;j<int(60/bW);j++)
   {
    midZ[j]=histoZreal->GetXaxis()->GetBinCenter(j);
    errZmid[j]=bW/2;
    TH1D* resHisto;
    resHisto =  new TH1D(Form("resHisto%d", j),Form("Hist of Zrec-Ztrue,  Ztrue:_%4.1f",midZ[j]), int(sqrt(histoZreal->GetBinContent(j)+1)),-2000.,2000.);//qui GetBinCintent prende 0
    arrHisto->AddAtAndExpand(resHisto,indexh++);
   }

   bol=false;
   double resolutionZ[indexh];
   double resolutionErrZ[indexh];
   double efficiencyZ[indexh];
   double efficiencyErrZ[indexh];
   
   residues(arrHisto,midZ,nEvents,resolutionZ,resolutionErrZ,efficiencyZ,efficiencyErrZ,bol);

   TGraphErrors *effZreal = new TGraphErrors(indexh,midZ,efficiencyZ,errZmid,efficiencyErrZ); //auto gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
   effZreal->SetTitle("Efficiency vs Vertex Z");
   effZreal->GetXaxis()->SetTitle("Z_true [#mum] ");
   effZreal->GetYaxis()->SetTitle("Efficiency");
   effZreal->SetMarkerStyle(8);
   effZreal->SetMarkerColor(kGreen);
   effZreal->Draw();
   effZreal->Write();

   TGraphErrors *resZreal = new TGraphErrors(indexh,midZ,resolutionZ,errZmid,resolutionErrZ);
   resZreal->SetTitle("Resolution vs Vertex Z");
   resZreal->GetXaxis()->SetTitle("Z_true [#mum] ");
   resZreal->GetYaxis()->SetTitle("Resolution [#mum] ");
   resZreal->SetMarkerStyle(8);
   resZreal->SetMarkerColor(kRed);
   resZreal->Draw();
   resZreal->Write();

   
   
     
}
