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

/**
 * @brief Set the Graph object
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


void runPlots()
{
   TFile* output = new TFile("Reconstruction.root", "recreate"); 
   TObjArray* arrHisto = new TObjArray(); 
   
   int nMolt=18;
   double Molt[]={1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,12.,15.,20.,30.,40.,50.,65.};
   double errMolt[nMolt]; 
   for(int c=0;c<nMolt;c++)
   {
    if (c==0) errMolt[c]=(Molt[+1]-Molt[c])/2;
    if(c>0) errMolt[c]=(Molt[c]-Molt[c-1])/2;
   }

   double Zvert[]={-20.,-18.,-16.,-14.,-12.,-10}

   int arrN[]={10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
   
   arrN[0]=nEvents;
   int indexh=0;
   int indexh2=0;

   for(int gg=1;gg<nMolt;gg++)
    {
        for(int hh=0;hh<nEvents;hh++)
        {
            if(resVec[hh]<10000 && (moltReal[hh]>Molt[gg]-Molt[gg]*0.1) && (moltReal[hh]<Molt[gg]+Molt[gg]*0.1)) arrN[gg]++;
        }
    }

   for(int i=0;i<nMolt;i++)
   {
        TH1D* resHisto;
        // check
        resHisto =  new TH1D(Form("resHisto%d", i),Form("Hist of Zrec-Ztrue Molt_%4.1f",Molt[i]), int(sqrt(arrN[i])),-2000.,2000.);
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
   effmolt->Draw("ap");
   effmolt->Write();

   TGraphErrors *resmolt = new TGraphErrors(indexh,Molt,resolutionM,errMolt,resolutionErrM);
   setGraph(resmolt, "Resolution vs Multiplicity", "Multiplicity", "Resolution [#mum]", 8, kOrange-3);
   TCanvas* c2= new TCanvas("c2","Resolution vs Multiplicity",80,80,1500,1000);
   c2->cd();
   resmolt->Draw("ap");
   resmolt->Write();//da scivere a MAsera nel readme, a molteplicità 0 ho messo quelli senza distinzione di molteplicità


   arrHisto->Clear();

   const double bW=2.;
   
   TH1D* histoZreal;
   histoZreal = new TH1D("histoZreal","Z of real vertex",int(60/bW),-31.,29.);
   const int nbinsX = histoZreal->GetNbinsX();
   double midZ[nbinsX];

   for(int y=0;y<nEvents;y++) histoZreal->Fill(zVertReal[y]); 
   
    
   double errZmid[nbinsX];
   TObjArray* arrHisto2 = new TObjArray(); 
   for(int j=0;j<nbinsX;j++)
   {
    midZ[j]=histoZreal->GetXaxis()->GetBinCenter(j+1);//provare a togliere +1
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
   effZreal->Draw("ap");
   effZreal->Write();

   TGraphErrors *resZreal = new TGraphErrors(indexh2,midZ,resolutionZ,errZmid,resolutionErrZ);
   setGraph(resZreal, "Resolution vs Vertex Z", "Z_true [cm]", "Resolution [#mum]", 8, kRed);
   TCanvas* c4= new TCanvas("c4","Resolution vs Vertex Z",80,80,1500,1000);
   c4->cd();
   resZreal->Draw("ap");
   resZreal->Write();

   output->Write();
   output->Close();

   
   
     
}