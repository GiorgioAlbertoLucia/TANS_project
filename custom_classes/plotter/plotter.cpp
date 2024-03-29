#include <Riostream.h>
#include <string>
#include <vector>

#include <TH1I.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TFile.h>
#include <TObjArray.h>
#include<TGraphErrors.h>
#include<TGraphAsymmErrors.h>
#include<TCanvas.h>

#include "../../yaml/Yaml.hpp"
#include "plotter.hpp" 

/*    PROTECTED   */
/**
 * @brief Basic settings for a TGraph
 * 
 * @param graph 
 * @param title 
 * @param xTitle 
 * @param yTitle 
 * @param markerStyle 
 * @param color 
 */
void setGraph(TGraph* graph, const char * name, const char * title, const char * xTitle = "x axis", 
                      const char * yTitle = "y axis", const int markerStyle = 0, const int color = 0)
{
  graph->SetName(name);
  graph->SetTitle(title);
  graph->GetXaxis()->SetTitle(xTitle);
  graph->GetYaxis()->SetTitle(yTitle);
  graph->SetMarkerStyle(markerStyle);
  graph->SetMarkerColor(color);
}


/**
 * @brief  Initialize Plotter data member importing vector with data to plot
 * 
 * @param zVertReal1 
 * @param zVertRec1 
 * @param moltReal1 
 */
 void Plotter::addVector(double zVertReal1[], double zVertRec1[], double moltReal1[], const int size) 
{
   fNEvents = size;

   zVertReal = new double[fNEvents];
   zVertRec = new double[fNEvents];
   moltReal = new double[fNEvents];
   resVec = new double[fNEvents];

   for(int i=0;i<fNEvents;i++)
   {
        zVertReal[i] = zVertReal1[i];
        zVertRec[i] = zVertRec1[i]; 
        moltReal[i] = moltReal1[i];
        resVec[i] = zVertRec1[i]*10000-zVertReal1[i]*10000;
   }
}



/**
 * @brief Creates histograms for residues and evaluates resolution and efficiency. 
 * 
 * @param arrHisto array of empty histograms
 * @param Xarray array with values on the x axis 
 * @param n 
 * @param resolution array to store resolution values
 * @param resolutionErr array to store resolution error values
 * @param efficiency array to store efficiency values
 * @param efficiencyErr array to store efficiency error values
 * @param bol true -> Xarray is multiplicity; false-> Xarray is vertex z coordinate
 */
void Plotter::residues(TObjArray* arrHisto,double *Xarray, int n,double *resolution,double *resolutionErr, double *efficiency, double *efficiencyErr, bool bol, string outputPath) 
{
    const int nHist=arrHisto->GetEntries();
    double mean[nHist];
    double bW=2.;

    TFile* output1 = NULL;
    if(bol==true) output1 = new TFile(outputPath.c_str(), "recreate");

    for(int ab=0;ab<nHist;ab++)
    {
      TH1D* hRes=(TH1D*)arrHisto->At(ab);
      double nEventsArr[nHist];
      for(int i=0; i<nHist; i++)  nEventsArr[i] = 0.;
      if(bol==true)
      {
           for(int j=0;j<n;j++)
           { 
              if((moltReal[j]>Xarray[ab]-Xarray[ab]*0.1)&&(moltReal[j]<Xarray[ab]+Xarray[ab]*0.1))
              {
                if(zVertRec[j]<1999) nEventsArr[ab]++;
                if (zVertRec[j]<999.) hRes->Fill(resVec[j]);
              } 
           }
        
        hRes->Draw("E");
        if(bol) output1->cd();
        hRes->Write();
      }

      else
      {
        for(int ii=0;ii<n;ii++)
        { 
          
          if((zVertReal[ii]>Xarray[ab]-bW) && (zVertReal[ii]<Xarray[ab]+bW))
          {
            if(zVertRec[ii]<1999) nEventsArr[ab]++;
            if(zVertRec[ii]<999.) hRes->Fill(resVec[ii]);
          }
        }
      }
  
      resolution[ab]=hRes->GetRMS();
      resolutionErr[ab]=hRes->GetRMSError();
      
      double entriesIn=0.;
      for(int t=1;t<8001;t++)
      {
           entriesIn=entriesIn+hRes->GetBinContent(t);//controllare sta cosa
      }

      if(nEventsArr[ab]>0.) efficiency[ab]=entriesIn/nEventsArr[ab];
      else efficiency[ab]=0.;
      if (nEventsArr[ab]>0.) efficiencyErr[ab]=pow(efficiency[ab]*(1-efficiency[ab])/nEventsArr[ab],0.5);
      else efficiencyErr[ab]=0.;
      if(nEventsArr[ab]>0 && (1/nEventsArr[ab]>efficiencyErr[ab])) efficiencyErr[ab]=1/nEventsArr[ab];


      delete hRes;
    }
    if(bol) 
    {
      output1->Close();
      cout << "Output file has been saved at " << outputPath << endl;
    }
  }



/**
 * @brief Creates and saves TGraphs of resolution and efficiency vs zTrue and multiplicity of the vertex
 * 
 */
void Plotter::runPlots()
{
   Yaml::Node root;
   Yaml::Parse(root, fConfigFile.c_str());

   TFile* output = new TFile(root["output"]["reconstruction"]["path"].As<string>().c_str(), "recreate"); 
   TObjArray* arrHisto = new TObjArray(); 
   
   int nMolt=16;
   double Molt[]={2.,3.,4.,5.,6.,7.,8.,9.,10.,12.,15.,20.,30.,40.,50.,65.};
   double errMolt[nMolt]; 
   for(int c=0;c<nMolt;c++)
   {
    if (c==0) errMolt[c]=(Molt[c+1]-Molt[c])/2;
    if(c>0) errMolt[c]=(Molt[c]-Molt[c-1])/2;
   }
   
   int indexh=0;
   for(int i=0;i<nMolt;i++)
   {
        TH1D* resHisto;
        
        resHisto =  new TH1D(Form("resHisto%d", i),Form("Hist of Zrec-Ztrue Molt_%4.1f",Molt[i]), 8000,-2000.,2000.);
        arrHisto->AddAtAndExpand(resHisto,indexh++);
   }

   string residuesPath = root["output"]["residues"]["path"].As<string>();

   double resolutionM[indexh];
   double resolutionErrM[indexh];
   double efficiencyM[indexh];
   double efficiencyErrM[indexh];
   bool bol=true;
   residues(arrHisto,Molt,fNEvents,resolutionM,resolutionErrM,efficiencyM,efficiencyErrM,bol, residuesPath);
  
   double errEffMhigh[indexh];
   for(int i=0;i<indexh;i++)
   {
    if(efficiencyM[i]+efficiencyErrM[i]>=1) errEffMhigh[i]=1-efficiencyM[i];
    else errEffMhigh[i]=efficiencyErrM[i];
   } 

   TGraphAsymmErrors *effmolt = new TGraphAsymmErrors(indexh,Molt,efficiencyM,errMolt,errMolt,efficiencyErrM,errEffMhigh);
   setGraph(effmolt,"eff_vs_mult",  "Efficiency vs Multiplicity", "Multiplicity", "Effciency", 8, kBlue);
   TCanvas* c1= new TCanvas("c1","Efficiency vs Multiplicity",80,80,1500,1000);
   c1->cd();
   effmolt->Draw("ap");
   output->cd();
   effmolt->Write();

   TGraphErrors *resmolt = new TGraphErrors(indexh,Molt,resolutionM,errMolt,resolutionErrM);
   setGraph(resmolt, "res_vs_mult", "Resolution vs Multiplicity", "Multiplicity", "Resolution [#mum]", 8, kOrange-3);
   TCanvas* c2= new TCanvas("c2","Resolution vs Multiplicity",80,80,1500,1000);
   c2->cd();
   resmolt->Draw("ap");
   output->cd();
   resmolt->Write();
   arrHisto->Clear();
   indexh=0;

   const double bW=2.;
   double midZ[18];
   double errZmid[18];

   for(int j=0;j<18;j++)
   {
    midZ[j]=-17+j*2;
    errZmid[j]=bW/2;
    TH1D* resHisto2 =  new TH1D(Form("resHisto%d", j),Form("Hist of Zrec-Ztrue,  Ztrue:_%4.1f",midZ[j]), 8000,-2000.,2000.);//qui GetBinCintent prende 0
    arrHisto->AddAtAndExpand(resHisto2,indexh++);
   }

   bol=false;
   double resolutionZ[indexh];
   double resolutionErrZ[indexh];
   double efficiencyZ[indexh];
   double efficiencyErrZ[indexh];
   residues(arrHisto,midZ,fNEvents,resolutionZ,resolutionErrZ,efficiencyZ,efficiencyErrZ,bol,residuesPath);
  

   double errEffZhigh[indexh];
   for(int i=0;i<indexh;i++)
   {
    if(efficiencyZ[i]+efficiencyErrZ[i]>=1) errEffZhigh[i]=1-efficiencyZ[i];
    else errEffZhigh[i]=efficiencyErrZ[i];
   } 
   TGraphAsymmErrors *effZreal = new TGraphAsymmErrors(indexh,midZ,efficiencyZ,errZmid,errZmid,efficiencyErrZ,errEffZhigh); 
   setGraph(effZreal, "eff_vs_z", "Efficiency vs Vertex Z", "Z_true [cm]", "Efficiency", 8, kGreen);
   TCanvas* c3= new TCanvas("c3","Efficiency vs Vertex Z",80,80,1500,1000);
   c3->cd();
   effZreal->Draw("ap");
   output->cd();
   effZreal->Write();

   TGraphErrors *resZreal = new TGraphErrors(indexh,midZ,resolutionZ,errZmid,resolutionErrZ);
   setGraph(resZreal, "res_vs_z", "Resolution vs Vertex Z", "Z_true [cm]", "Resolution [#mum]", 8, kRed);
   TCanvas* c4= new TCanvas("c4","Resolution vs Vertex Z",80,80,1500,1000);
   c4->cd();
   resZreal->Draw("ap");
   output->cd();
   resZreal->Write();

   output->Close();
   cout << "Output file has been saved at " << root["output"]["reconstruction"]["path"].As<string>() << endl;


}