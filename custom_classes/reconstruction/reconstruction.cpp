#include <vector>

#include <TRandom3.h>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <TH1D.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TStopwatch.h>

#include "../hit/hit.hpp"
#include "../pointCC/pointCC.hpp"
#include "../vertex/vertex.hpp"
#include "../plotter/plotter.hpp"
#include "../../yaml/Yaml.hpp"
#include "reconstruction.hpp"


/**
 * @brief return z from tracking's line
 * 
 * @param hit1 
 * @param hit2 
 * @return double 
 */
double Reconstruction::recZvert(Hit *hit1,Hit *hit2)
{
    double r1,z1,r2,z2;
    r1=hit1->getRadius();
    r2=hit2->getRadius();
    z1=hit1->getZ();
    z2=hit2->getZ();
    return (z2*r1-z1*r2)/(r1-r2);
}



/**
 * @brief loops on hits points in order to find vertex's Z
 * 
 * @param hitsArray1 
 * @param hitsArray2 
 */
void Reconstruction::vertexReconstruction(TClonesArray *hitsArray1, TClonesArray *hitsArray2)
{
    double phi = 0.;
    double deltaPhi = 0.01; 
    double ztemp = 0;
    vector<double> zTrackVert;
    double binW = 0.5;
    TH1D* histoHit = new TH1D("histoHit","Vertex's z rec",int(60./binW),-60.*binW,60.*binW);
     
    for(int i=0; i<hitsArray1->GetEntries(); i++)                          
    {

        Hit *hitptr=(Hit*)hitsArray1->At(i);
        if ((hitptr->getZ()<13.5)&&(hitptr->getZ()>-13.5))
        {
            phi = hitptr->getPhi();
            
            for(int j=0; j<hitsArray2->GetEntries(); j++)
            {
                Hit *hitptr1=(Hit*)hitsArray2->At(j);
                if((hitptr1->getPhi()<phi+deltaPhi) && (hitptr1->getPhi()>phi-deltaPhi)  && (hitptr1->getZ()<13.5) && (hitptr1->getZ()>-13.5))
                {
                    ztemp = recZvert( hitptr,hitptr1);
                    zTrackVert.push_back(ztemp);
                    histoHit->Fill(ztemp);
                }
                
            }
            
        }
       
    }
    int binmax = histoHit->GetMaximumBin();
    double zMax = histoHit->GetXaxis()->GetBinCenter(binmax);
    int nEvntsMax=histoHit->GetBinContent(binmax);
    int nMaxBin=0;
    int c=0,b=0;
    for(int rr=1;rr<histoHit->GetNbinsX();rr++)
    {
       if(histoHit->GetBinContent(rr)==nEvntsMax)  
       {
         nMaxBin++;
       }
    }
    if(histoHit->GetBinContent(binmax-1)==nEvntsMax) c++;
    if(histoHit->GetBinContent(binmax+1)==nEvntsMax) b++;
    
    delete histoHit;

    vector<double> zTrackVert1;
    if(nMaxBin==1)
    {
        for(unsigned long int aa=0; aa<zTrackVert.size(); aa++)          
        {
            if((zTrackVert[aa]<=zMax+binW/2)&&(zTrackVert[aa]>=zMax-binW/2)) 
            {
                zTrackVert1.push_back(zTrackVert[aa]);
            }
        }
        double som = 0.;
        for(unsigned long int a=0; a<zTrackVert1.size(); a++) som = som+zTrackVert1[a];
         zVertVecRec.push_back(som/zTrackVert1.size());
    }
    else 
    if(binmax>1 && (b>0 || c>0)) 
    {
        for(unsigned long int aa=0; aa<zTrackVert.size(); aa++)          
        {
            if(b>0)
            {
                if((zTrackVert[aa]<=zMax+3*binW/2)&&(zTrackVert[aa]>=zMax-binW/2)) 
                {
                    zTrackVert1.push_back(zTrackVert[aa]);
                }
            }
            if(c>0)
            {
                if((zTrackVert[aa]<=zMax+binW/2)&&(zTrackVert[aa]>=zMax-3*binW/2)) 
                {
                    zTrackVert1.push_back(zTrackVert[aa]);
                }  
            }
        }
        double som = 0.;
        for(unsigned long int a=0; a<zTrackVert1.size(); a++) som = som+zTrackVert1[a];
        zVertVecRec.push_back(som/zTrackVert1.size());
    }
    else zVertVecRec.push_back(1000.);
        
}
    
    //----------------------------------------------------
    /*int size=int(zTrackVert.size());
    
    for(int g=0;g<size;g++)
    {
        
        double  temp=0.;
        for(int w=g+1;w<size;w++)
        {
            
            if(zTrackVert[g]>zTrackVert[w]) 
            {   
                
                temp=zTrackVert[g];
                zTrackVert[g]=zTrackVert[w];
                zTrackVert[w]=temp;
                
            }
        }
    }
    
    if(size>0)
    {   
      int k=0;
      int nummax=0;
      int indmax=0;
      int num=0;
      for(int i=0;i<size;i++)
      {
        k=i;
        num=0;
        
        while((zTrackVert[k]<=zTrackVert[i]+0.5) && (zTrackVert[k]>=zTrackVert[i]))
        {
            num++;
            k++;
        }
        if(num>nummax)
        { 
            nummax=num;
            indmax=i;
        }
      }
        //if((num>0) && (num==nummax)) f++;
        double som=0.;
        for(int g=indmax;g<indmax+nummax;g++)
        {
            som=som+zTrackVert[g];
        }
        zVertVecRec.push_back(som/nummax);
    }
      
        
    else zVertVecRec.push_back(1000.);
}*/

/**
 * @brief read data from tree
 * 
 */
void Reconstruction::runReconstruction()
{ 
    cout << "-------------------------------------------" << endl;
    cout << "Begin reconstruction..." << endl;

    TStopwatch timer;
    timer.Start();
    
    Yaml::Node root;
    Yaml::Parse(root, fConfigFile.c_str());

    TFile hfile(root["tree"]["simulation"]["path"].As<std::string>().c_str());
    TTree *tree = (TTree*)hfile.Get(root["tree"]["simulation"]["name"].As<std::string>().c_str());
    //tree->SetDirectory(0);  // giogio


    const int nlayer = root["n_detectors"].As<int>() - 1; // n_detectors counts beam pipe as well
    
    TBranch *br[nlayer];
    TBranch *bv = tree->GetBranch("Vertex");
    //TH1D* smear = new TH1D("smear","smearing",10000,-0.5,0.5);
    for(int b=0; b<nlayer; b++)
    {
        br[b]=tree->GetBranch(Form("HitsL%d",b+1));
    }

    TClonesArray *hitsArray[nlayer];
    // qui proprio fuori di testa
    for (TClonesArray * &a: hitsArray)  a = new TClonesArray("Hit",100);
    //for(int yy=0; yy<nlayer; yy++)  *hitsArray[yy] = TClonesArray("Hit",100); 
    Vertex *vertex=new Vertex();
    bv->SetAddress(&vertex);

    for(int b=0; b<nlayer; b++)
    {
          br[b]->SetAddress(&hitsArray[b]);
    }

    // questo te lo metto nella stessa logica di quello che abbiamo fatto per plotter
    const int nEvents = tree->GetEntries();
    zVertVec.reserve(nEvents);          // fix vector sizes
    zMoltVec.reserve(nEvents);
    zVertVecRec.reserve(nEvents);

    for(int ev=0; ev<nEvents; ev++)
    {
        double sme=0.;
        if(ev%30000==0)    cout << "Processing event " << ev << "..." << endl;
        tree->GetEvent(ev);
        zVertVec.push_back(vertex->getZ());
        
        zMoltVec.push_back(vertex->getMultiplicity());

        for(int ll=1; ll<nlayer; ll++)
        { 
            const int numHits = hitsArray[ll]->GetEntries();
        
            Detector detector = {root["detectors"][ll]["radius"].As<double>(),
                                 root["detectors"][ll]["width"].As<double>(),
                                 root["detectors"][ll]["lenght"].As<double>(),
                                 root["detectors"][ll]["multiple_scattering"].As<bool>(),};
            
            for(int i=0;i<numHits;i++)//smearing
            {  
                Hit *hitptr2 = (Hit*)hitsArray[ll]->At(i);
                //sme=hitptr2->getPhi();
                hitptr2->smearing();
                //smear->Fill(sme-hitptr2->getPhi());
                
            }
        
            int noi=int(gRandom->Rndm()*10);//add noise
            hitsArray[ll]->Expand(hitsArray[ll]->GetEntries()+noi);
            for(int i=numHits; i<numHits+noi; i++)
            {
                Hit * hit1 = (Hit*)hitsArray[ll]->ConstructedAt(i);
                hit1->noise(detector);               
            }
        }

        vertexReconstruction(hitsArray[0], hitsArray[1]);
        for(int i=0; i<nlayer; i++) hitsArray[i]->Clear();
    }
    hfile.Close(); 

    timer.Stop();  
    cout << endl;
    cout << "Reconstruction ended." << endl;
    cout << "Real time: " << timer.RealTime() << " s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << " s" << endl;

    timer.Reset();
    cout << endl << "-----------------------------" << endl;
    cout << "Drawing plots..." << endl;
    timer.Start();

    Plotter plot;
    plot.addVector(zVertVec,zVertVecRec,zMoltVec);
    plot.runPlots();
    TH1D* histores = new TH1D("histores","Residuii",int(sqrt(nEvents)),-3000.,3000.);
    TH1D* histores1 = new TH1D("histores1","zrec",120,-30.,30.0);
    TH1D* histores2 = new TH1D("histores2","zreal",120,-30.,30.0);
    for(int j=0;j<nEvents;j++)
    {
        if(zVertVecRec[j]<999.) histores->Fill((zVertVecRec[j]*10000-zVertVec[j]*10000));
        if(zVertVecRec[j]<999.) histores1->Fill(zVertVecRec[j]);
        histores2->Fill(zVertVec[j]);
    }
    TCanvas* c41= new TCanvas("c41","residues",80,80,1500,1000);
   c41->cd();
   histores->Draw();
   TCanvas* c51= new TCanvas("c51","rec",80,80,1500,1000);
   c51->cd();
   histores1->Draw();
    TCanvas* c61= new TCanvas("c61","real",80,80,1500,1000);
   c61->cd();
   histores2->Draw();

    timer.Stop();
    cout << endl;
    cout << "Process ended." << endl;
    cout << "Real time: " << timer.RealTime() << " s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << " s" << endl;
}