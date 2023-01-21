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
void setGraph(TGraph* graph, const char * title, const char * xTitle = "x axis", 
                      const char * yTitle = "y axis", const int markerStyle = 0, const int color = 0)
{
  graph->SetTitle(title);
  graph->GetXaxis()->SetTitle(xTitle);
  graph->GetYaxis()->SetTitle(yTitle);
  graph->SetMarkerStyle(markerStyle);
  graph->SetMarkerColor(color);
}


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
      TH1D* hRes=(TH1D*)arrHisto->At(ab);//fai un check sull'array di hist che passi qui
      if(bol==true)
      {
        if(ab==0)
        {
          for(int i=0;i<n;i++) hRes->Fill(resVec[i]); 
        }

        else 
        {
           for(int j=0;j<n;j++) if((moltReal[j]>Xarray[ab]-Xarray[ab]*0.1)&&(moltReal[j]<Xarray[ab]+Xarray[ab]*0.1)) hRes->Fill(resVec[j]); //provato con u cout dopo e lo stampa quindi questo non dà pronlemi
        }
        hRes->Draw("E");
        hRes->Write();
      }

      else
      {
        for(int ii=0;ii<n;ii++)
        { 
          
          if((zVertReal[ii]>Xarray[ab]-bW)&&(zVertReal[ii]<Xarray[ab]+bW)) hRes->Fill(resVec[ii]);
        }
      }
    
      resolution[ab]=hRes->GetStdDev();
      cout<<"risoluzione "<<ab<<" = "<<resolution[ab]<<endl;
      resolutionErr[ab]=hRes->GetStdDevError();
      cout<<"risoluzione err "<<ab<<" = "<<resolutionErr[ab]<<endl;
      mean[ab]=hRes->GetMean();
      cout<<"media "<<ab<<" = "<<mean[ab]<<endl;
      const int binMax=hRes->GetBin(mean[ab]+3*resolution[ab]); 
      const int binMin=hRes->GetBin(mean[ab]-3*resolution[ab]);
      cout<<"bin max="<<binMax<<" binmin="<<binMin <<endl;
      int entriesIn=0; 
      if (ab==nHist-1) cout<<"controllo 3"<<endl;
      for(int t=binMin;t<binMax;t++)
      {
          entriesIn=entriesIn+hRes->GetBinContent(t);
           
      }
      if(hRes->GetEntries()>0.) efficiency[ab]=entriesIn/hRes->GetEntries();
      else efficiency[ab]=0.;
      cout<<"efficienza="<<efficiency[ab]<<endl;
      if (hRes->GetEntries()>0.) efficiencyErr[ab]=sqrt(entriesIn/(hRes->GetEntries()*hRes->GetEntries()) + entriesIn*entriesIn/(hRes->GetEntries()*hRes->GetEntries()*hRes->GetEntries()) ); //poisson and propagation
      else efficiencyErr[ab]=0.;
      cout<<"efficienza err="<<efficiencyErr[ab]<<endl;
    }
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
    errMolt[c]=sqrt(Molt[c]);
   }

   int arrN[]={10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
   
   arrN[0]=nEvents;
   int indexh=0;
      int indexh2=0;

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
  
   double errEffMhigh[indexh];
   for(int i=0;i<indexh;i++)
   {
    if(efficiencyM[i]+efficiencyErrM[i]>=1) errEffMhigh[i]=0.;
    else errEffMhigh[i]=efficiencyErrM[i];
   } 

   TGraphAsymmErrors *effmolt = new TGraphAsymmErrors(indexh,Molt,efficiencyM,errMolt,errMolt,efficiencyErrM,errEffMhigh);
   setGraph(effmolt, "Efficiency vs Multiplicity", "Multiplicity", "Effciency", 8, kBlue);
   TCanvas* c1= new TCanvas("c1","Efficiency vs Multiplicity",80,80,1500,1000);
   c1->cd();
   effmolt->Draw();
   effmolt->Write();

   TGraphErrors *resmolt = new TGraphErrors(indexh,Molt,resolutionM,errMolt,resolutionErrM);
   setGraph(resmolt, "Resolution vs Multiplicity", "Multiplicity", "Resolution [#mum]", 8, kOrange-3);
   TCanvas* c2= new TCanvas("c2","Resolution vs Multiplicity",80,80,1500,1000);
   c2->cd();
   resmolt->Draw();
   resmolt->Write();//da scivere a MAsera nel readme, a molteplicità 0 ho messo quelli senza distinzione di molteplicità


   arrHisto->Clear();

   const double bW=2.;
   
   TH1D* histoZreal;
   histoZreal = new TH1D("histoZreal","Z of real vertex",int(60/bW),-30.,30.);
   const int nbinsX = histoZreal->GetNbinsX();
   double midZ[nbinsX];

   for(int y=0;y<nEvents;y++) histoZreal->Fill(zVertReal[y]); 
   
    
   double errZmid[nbinsX];
   TObjArray* arrHisto2 = new TObjArray(); 
   for(int j=0;j<nbinsX;j++)
   {
    midZ[j]=histoZreal->GetXaxis()->GetBinCenter(j+1);
    errZmid[j]=bW/2;
    TH1D* resHisto2 =  new TH1D(Form("resHisto%d", j),Form("Hist of Zrec-Ztrue,  Ztrue:_%4.1f",midZ[j]), int(sqrt(histoZreal->GetBinContent(j+1))+1),-2000.,2000.);//qui GetBinCintent prende 0
    arrHisto2->AddAtAndExpand(resHisto2,indexh2++);
   }

   bol=false;
   double resolutionZ[indexh2];
   double resolutionErrZ[indexh2];
   double efficiencyZ[indexh2];
   double efficiencyErrZ[indexh2];
   residues(arrHisto2,midZ,nEvents,resolutionZ,resolutionErrZ,efficiencyZ,efficiencyErrZ,bol);
  
   cout<<"controllo fuori da funzione"<<endl;
   for(int i=0;i<indexh2;i++)
   {
    cout<<"eff n "<<i+1<<" = "<<efficiencyZ[i]<<endl;
   }

   double errEffZhigh[indexh2];
   for(int i=0;i<indexh2;i++)
   {
    if(efficiencyZ[i]+efficiencyErrZ[i]>=1) errEffZhigh[i]=1-efficiencyZ[i];
    else errEffZhigh[i]=efficiencyErrZ[i];
   } 
   TGraphAsymmErrors *effZreal = new TGraphAsymmErrors(indexh2,midZ,efficiencyZ,errZmid,errZmid,efficiencyErrZ,errEffZhigh); 
   setGraph(effZreal, "Efficiency vs Vertex Z", "Z_true [cm]", "Efficiency", 8, kGreen);
   TCanvas* c3= new TCanvas("c3","Efficiency vs Vertex Z",80,80,1500,1000);
   c3->cd();
   effZreal->Draw();
   effZreal->Write();

   TGraphErrors *resZreal = new TGraphErrors(indexh2,midZ,resolutionZ,errZmid,resolutionErrZ);
   setGraph(resZreal, "Resolution vs Vertex Z", "Z_true [cm]", "Resolution [#mum]", 8, kRed);
   TCanvas* c4= new TCanvas("c4","Resolution vs Vertex Z",80,80,1500,1000);
   c4->cd();
   resZreal->Draw();
   resZreal->Write();

   output->Write();
   output->Close();

   
   
     
}

   
   
     

