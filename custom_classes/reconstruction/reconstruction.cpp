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
double Reconstruction::recZvert(Hit *hit1,Hit *hit2,int ev)
{
    const double m = hit2->getY()-hit1->getY();
    const double n = hit2->getZ()-hit1->getZ();

    double z0 = hit2->getZ() - n/m*hit2->getY();
    //if(ev<3) cout<<"hit1 y="<<hit1->getY()<<"hit1 z="<<hit1->getZ()<<"hit2 y="<<hit2->getY()<<"hit2 z="<<hit2->getZ()<<endl;
    //if(ev<3) cout<<"zret"<<zret<<endl;
    return z0; //from 3D line equations*/
  
}


/**
* @brief Loops on hits points in order to find vertex's Z
* 
* @param hitsArray1 TClonesArray of Hit - all hits on the first layer
* @param hitsArray2 TClonesArray of Hit - all hits on the second layer
*/
void Reconstruction::vertexReconstruction(TClonesArray *hitsArray1, TClonesArray *hitsArray2, int ev)
{    
    vector<double> zTrackVert;      // stores z coordinates after reconstruction

    double binW = .5;
    TH1D* histoHit = new TH1D("histoHit","Vertex's z rec",int(60./binW),-60.*binW,60.*binW);  
                                                                     
    for(int i=0; i<hitsArray1->GetEntries(); i++)                          
    {   
        Hit *hitptr=(Hit*)hitsArray1->At(i);
        if ((hitptr->getZ()<13.5)&&(hitptr->getZ()>-13.5))  // checks if the hit was registered inside the detector lenght
        {   
            for(int j=0; j<hitsArray2->GetEntries(); j++)
            {
                const double phi = hitptr->getPhi();
                const double deltaPhi = 0.01;

                Hit *hitptr1=(Hit*)hitsArray2->At(j);

                if((hitptr1->getPhi()<phi+deltaPhi) && (hitptr1->getPhi()>phi-deltaPhi) && (hitptr1->getZ()>-13.5) && (hitptr1->getZ()<13.5))
                {
                    const double ztemp = recZvert(hitptr, hitptr1, ev);
                    histoHit->Fill(ztemp);
                    zTrackVert.push_back(ztemp);
                }
                
            }
        }
       
    }

    int binmax = histoHit->GetMaximumBin();
    double zMax = histoHit->GetXaxis()->GetBinCenter(binmax);
    int nEvntsMax=histoHit->GetBinContent(binmax);
    int nMaxBin=0;

    if(ev<3)   cout << "binmax = "<< binmax << endl << "nEventsMax = " << nEvntsMax << endl;

    for(int rr=1;rr<histoHit->GetNbinsX();rr++)
    {
       if(histoHit->GetBinContent(rr)==nEvntsMax)  nMaxBin++;
    }

    if(ev<3)
    {
        TFile file(Form("output/hist%d.root", ev), "recreate");
        histoHit->Write();
        file.Close();
    }
    delete histoHit;

    vector<double> zTrackVert1;

    if(nMaxBin==1)
    {
        TH1D * th1 = new TH1D("check", "check", 100, zMax-binW/2, zMax+binW/2);
        for(unsigned long int aa=0; aa<zTrackVert.size(); aa++)          
        {
            if((zTrackVert[aa]<=zMax+binW/2)&&(zTrackVert[aa]>=zMax-binW/2)) 
            {
                zTrackVert1.push_back(zTrackVert[aa]);
                if(ev<3)   th1->Fill(zTrackVert[aa]);
            }
        }
        
        double som = 0.;
        for(unsigned long int a=0; a<zTrackVert1.size(); a++) som = som+zTrackVert1[a];

        if(ev<3)
        {
            cout << "zMax = " << zMax << endl;
            TFile file(Form("output/check%d.root", ev), "recreate");
            th1->Write();
            file.Close();
            cout << "som = " << som/zTrackVert1.size() << endl;
        }
        delete th1;

        zVertVecRec.push_back(som/zTrackVert1.size());
    }
    else zVertVecRec.push_back(1000.);
   
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

    // initialize parser
    Yaml::Node root;
    Yaml::Parse(root, fConfigFile.c_str());

    // initialize TTree 
    TFile hfile(root["outputPaths"]["treeSimPath"].As<std::string>().c_str());
    TTree *tree = (TTree*)hfile.Get(root["outputNames"]["treeSimName"].As<std::string>().c_str());
    //tree->SetDirectory(0);  // giogio


    const int nlayer = root["n_detectors"].As<int>() - 1; // n_detectors counts beam pipe as well
    
    TBranch *br[nlayer];
    TBranch *bv = tree->GetBranch("Vertex");
    
    for(int b=0; b<nlayer; b++)
    {
        br[b]=tree->GetBranch(Form("HitsL%d",b+1));
    }

    TClonesArray *hitsArray[nlayer];
    for (TClonesArray * &a: hitsArray)  a = new TClonesArray("Hit",100);

    Vertex *vertex=new Vertex();
    bv->SetAddress(&vertex);

    for(int b=0; b<nlayer; b++)
    {
          br[b]->SetAddress(&hitsArray[b]);
    }


    const int nEvents = tree->GetEntries();
    zVertVec.reserve(nEvents);          // fix vector sizes
    zMoltVec.reserve(nEvents);
    zVertVecRec.reserve(nEvents);

    for(int ev=0; ev<nEvents; ev++)
    {
        if(ev%30000==0)    cout << "Processing event " << ev << "..." << endl;
        tree->GetEvent(ev);
        zVertVec.push_back(vertex->getZ());
        
        zMoltVec.push_back(vertex->getMultiplicity());

        for(int ll=0; ll<nlayer; ll++)
        { 
            const int numHits = hitsArray[ll]->GetEntries();
            const double detectorRadius = root["detectors"][ll]["radius"].As<double>();
            const double detectorLenght = root["detectors"][ll]["lenght"].As<double>();
            
            for(int i=0;i<numHits;i++)//smearing
            {
                Hit *hitptr2 = (Hit*)hitsArray[ll]->At(i);
                hitptr2->smearing();
                
            }
        
            int noi=int(gRandom->Rndm()*10);//add noise
            hitsArray[ll]->Expand(hitsArray[ll]->GetEntries()+noi);
            for(int i=numHits; i<numHits+noi; i++)
            {
                Hit * hit1 = (Hit*)hitsArray[ll]->ConstructedAt(i);
                hit1->noise(detectorRadius, detectorLenght);       
            }
        }
        
        vertexReconstruction(hitsArray[0], hitsArray[1], ev);
        if(ev<3)     
        {
            cout << "event " << ev << endl;
            cout << "zTrue = " << zVertVec[ev] << endl;
        }

        for(int i=0; i<nlayer; i++) hitsArray[i]->Clear();
    }
    hfile.Close(); 

    timer.Stop();  
    cout << endl;
    cout << "Reconstruction ended." << endl;
    cout << "Real time: " << timer.RealTime() << "s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << "s" << endl;

    timer.Reset();
    cout << endl << "-----------------------------" << endl;
    cout << "Drawing plots..." << endl;
    timer.Start();

    //Plotter plot;
    //plot.addVector(zVertVec,zVertVecRec,zMoltVec);
    //plot.runPlots();
    TH1D* histores = new TH1D("histores","Residuii",int(sqrt(nEvents)),-20000.,20000.);
    //TH1D* histores = new TH1D("histores","Residuii",int(sqrt(nEvents)),-3000.,3000.);
    TH1D* histores1 = new TH1D("histores1","Residuii1",int(sqrt(nEvents)),-3000.,3000.);
    for(int j=0;j<nEvents;j++)
    {
        if(zVertVecRec[j]<999.) histores->Fill((zVertVec[j])*10000);
        if((zVertVecRec[j]*10000-zVertVec[j]*10000)<10000) histores1->Fill(zVertVecRec[j]*10000);
        //if(zVertVecRec[j]<999.) histores->Fill((zVertVecRec[j]-zVertVec[j])*10000);
        //if((zVertVecRec[j]*10000-zVertVec[j]*10000)<10000) histores1->Fill(zVertVecRec[j]*10000-zVertVec[j]*10000);
    }
    TCanvas* c4= new TCanvas("c4","residues",80,80,1500,1000);
    c4->cd();
    histores->Draw();
    TCanvas* c5= new TCanvas("c5","residues1",80,80,1500,1000);
    c5->cd();
    histores1->Draw();

    timer.Stop();
    cout << endl;
    cout << "Process ended." << endl;
    cout << "Real time: " << timer.RealTime() << "s" << endl;
    cout << "CPU time: " << timer.CpuTime()  << "s" << endl;
}

    



