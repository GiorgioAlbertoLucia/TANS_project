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
#include<TGraphErrors.h>

#include "../hit/hit.hpp"
#include "../pointCC/pointCC.hpp"
#include "../vertex/vertex.hpp"
#include "../plotter/plotter.hpp"
#include "../recorder/recorder.hpp"
#include "../../yaml/Yaml.hpp"
#include "reconstruction.hpp"

/*  STATIC DATA MEMBER  */
Reconstruction * Reconstruction::fInstancePtr = NULL;

/*       PRIVATE        */

/**
 * @brief Construct a new Reconstruction:: Reconstruction object
 * 
 */
Reconstruction::Reconstruction(const char * configFile):
    fConfigFile(configFile), zVertVec(NULL), zMoltVec(NULL), zVertVecRec(NULL), fNEvents(0)
{

}

/**
 * @brief Destroy the Reconstruction:: Reconstruction object
 * 
 */
Reconstruction::~Reconstruction()
{
    if(fNEvents > 0)        delete []zVertVec;
    if(fNEvents > 0)        delete []zMoltVec;
    if(fNEvents > 0)     delete []zVertVecRec;
}

/*      PROTECTED       */

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
    r1 = hit1->getRadius();
    r2 = hit2->getRadius();
    z1 = hit1->getZ();
    z2 = hit2->getZ();
    return (z2*r1-z1*r2)/(r1-r2);
}

/**
 * @brief loops on hits points in order to find vertex's Z
 * 
 * @param hitsArray1 
 * @param hitsArray2 
 */
void Reconstruction::vertexReconstruction(TClonesArray *hitsArray1, TClonesArray *hitsArray2, TH1D* histoHit, const int ev, const double binW, const double deltaPhi)
{
    double phi = 0.;
    double ztemp = 0;
    
    vector<double> zTrackVert;
    zTrackVert.reserve(hitsArray1->GetEntries());
     
    for(int i=0; i<hitsArray1->GetEntries(); i++)                          
    {
        Hit * hitptr = (Hit*)hitsArray1->At(i);
        if ((hitptr->getZ()<13.5) && (hitptr->getZ()>-13.5))
        {
            phi = hitptr->getPhi();
            
            for(int j=0; j<hitsArray2->GetEntries(); j++)
            {
                Hit * hitptr1 = (Hit*)hitsArray2->At(j);
                if((hitptr1->getPhi() < phi + deltaPhi) && (hitptr1->getPhi() > phi - deltaPhi) && (abs(hitptr1->getZ()) < 13.5) && (hitptr1->getZ() > -13.5))
                {
                    ztemp = recZvert(hitptr, hitptr1);
                    zTrackVert.push_back(ztemp);
                    histoHit->Fill(ztemp);
                }

            }
            
        }
        
    }


    if(zTrackVert.size() == 0)
    {
        zVertVecRec[ev] = 1000.;
        histoHit->Reset();
        return;
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
    histoHit->Reset();
  

    double som = 0.;
    int numin = 0;

    if(nMaxBin==1)
    {
        for(unsigned long int aa=0; aa<zTrackVert.size(); aa++)          
        {
            if((zTrackVert[aa]<=zMax+binW/2)&&(zTrackVert[aa]>=zMax-binW/2)) 
            {
                som = som+zTrackVert[aa];
                numin++;
            }
        }
        //zVertVecRec.push_back(som/zTrackVert1.size());
        zVertVecRec[ev] = som/numin;
        return;
    }
    else 
    {
        if((nMaxBin==2) && (b==1 || c==1)) 
        {
            for(unsigned long int aa=0; aa<zTrackVert.size(); aa++)          
            {
                if(b==1)
                {
                    if((zTrackVert[aa]<=zMax+3*binW/2)&&(zTrackVert[aa]>=zMax-binW/2)) 
                    {
                        som = som+zTrackVert[aa];
                        numin++;
                    }
                }
                if(c==1)
                {
                    if((zTrackVert[aa]<=zMax+binW/2)&&(zTrackVert[aa]>=zMax-3*binW/2)) 
                    {
                        som = som+zTrackVert[aa];
                        numin++;
                    }  
                }
            }
            //zVertVecRec.push_back(som/zTrackVert1.size());
            zVertVecRec[ev] = som/numin;
            return;
        }
        else
        {
            //zVertVecRec.push_back(1000.);
            zVertVecRec[ev] = 1000.;
        }
    }
        
}
    
/*      PUBLIC      */

/**
 * @brief Create an instance of the singleton object
 * 
 * @param configFile 
 * @param constantsFile
 * @return Reconstruction* 
 */
Reconstruction * Reconstruction::getInstance(const char * configFile)
{
    if(fInstancePtr == NULL)    fInstancePtr = new Reconstruction(configFile);
    return fInstancePtr;
}

/**
 * @brief Destroy the instance of the singleton object
 * 
 */
void Reconstruction::destroy()
{
    if(fInstancePtr)            delete fInstancePtr;
    fInstancePtr = NULL;
}

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
    
    
    const int nlayer = root["n_detectors"].As<int>() - 1; // n_detectors counts beam pipe as well
    
    TBranch *br[nlayer];
    TBranch *bv = tree->GetBranch("Vertex");
    
    for(int b=0; b<nlayer; b++)
    {
        br[b]=tree->GetBranch(Form("HitsL%d",b+1));
    }

    TClonesArray *hitsArray[nlayer];
    for (TClonesArray * &a: hitsArray)  a = new TClonesArray("Hit",100);
    
    Vertex *vertex = new Vertex();
    bv->SetAddress(&vertex);

    for(int b=0; b<nlayer; b++)
    {
          br[b]->SetAddress(&hitsArray[b]);
    }

    fNEvents = tree->GetEntries();
    //zVertVec.reserve(nEvents);          // fix vector sizes
    //zMoltVec.reserve(nEvents);
    //zVertVecRec.reserve(nEvents);

    zVertVec = new double[fNEvents];
    zMoltVec = new double[fNEvents];
    zVertVecRec = new double[fNEvents];

    Detector detectorVector[nlayer];
    for(int i=0; i<nlayer; i++)
    {   
        detectorVector[i] = {root["detectors"][i+1]["radius"].As<double>(),
                             root["detectors"][i+1]["width"].As<double>(),
                             root["detectors"][i+1]["lenght"].As<double>(),
                             root["detectors"][i+1]["multiple_scattering"].As<bool>()};
    }

    const double binW = root["recTolerance"]["zBinWidth"].As<double>();
    const double deltaPhi = root["recTolerance"]["deltaPhi"].As<double>(); 
    const int noiseMax = root["noise"]["nPoints"].As<int>();
    TH1D * histoHit = new TH1D("histoHit","Vertex's z rec",int(60./binW),-60.*binW,60.*binW);

    for(int ev=0; ev<fNEvents; ev++)
    {
        if(ev%50000==0)    cout << "Processing event " << ev << "..." << endl;
        tree->GetEvent(ev);
        //zVertVec.push_back(vertex->getZ());
        //zMoltVec.push_back(vertex->getMultiplicity());

        zVertVec[ev] = vertex->getZ();
        zMoltVec[ev] = vertex->getMultiplicity();

        for(int ll=0; ll<nlayer; ll++)
        { 
            const int numHits = hitsArray[ll]->GetEntries();
            
            for(int i=0;i<numHits;i++)//smearing
            {  
                Hit *hitptr2 = (Hit*)hitsArray[ll]->At(i);
                hitptr2->smearing();
            }

            const int noi = int(gRandom->Rndm()*noiseMax);//add noise
            hitsArray[ll]->Expand(hitsArray[ll]->GetEntries()+noi);
            for(int i=numHits; i<numHits+noi; i++)
            {
                Hit * hit1 = (Hit*)hitsArray[ll]->ConstructedAt(i);
                //hit1->noise(detector);               
                hit1->noise(detectorVector[ll]);               
            }
        }

        vertexReconstruction(hitsArray[0], hitsArray[1], histoHit, ev, binW, deltaPhi);
        if(ev==105)   
        {   
            Recorder * recorder = Recorder::getInstance(root["recording"]["reconstruction"]["path"].As<std::string>().c_str());
            recorder->recordReconstruction(hitsArray[0], hitsArray[1], zVertVec[ev]);
            recorder->destroy();
        }
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

    TH1D* histores = new TH1D("histores","Residuii",int(sqrt(fNEvents)),-3000.,3000.);
    TH1D* histores1 = new TH1D("histores1","zrec",120,-30.,30.0);
    TH1D* historeal = new TH1D("historeal","zreal",120,-30.,30.0);
        for(int j=0;j<fNEvents;j++)
    {
        if(zVertVecRec[j]<999.) histores->Fill((zVertVecRec[j]*10000-zVertVec[j]*10000));
        if(zVertVecRec[j]<999.) histores1->Fill(zVertVecRec[j]);
        historeal->Fill(zVertVec[j]);
    }
    
    double sigma=historeal->GetRMS();
    double mean=historeal->GetMean();
    for(int j=0;j<fNEvents;j++)
    {
        if(abs(zVertVec[j]-mean)>3*sigma) zVertVecRec[j]=2000.;
    }

    Plotter plot(fConfigFile.c_str());
    plot.addVector(zVertVec, zVertVecRec, zMoltVec, fNEvents);
    plot.runPlots();
    
 
    TCanvas* c41= new TCanvas("c41","residues",80,80,1500,1000);
   c41->cd();
   histores->Draw();
   TCanvas* c51= new TCanvas("c51","rec",80,80,1500,1000);
   c51->cd();
   histores1->Draw();
    TCanvas* c61= new TCanvas("c61","real",80,80,1500,1000);
   c61->cd();
   historeal->Draw();

    timer.Stop();
    cout << endl;
    cout << "Process ended." << endl;
    cout << "Real time: " << timer.RealTime() << " s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << " s" << endl;
}