#include <TRandom3.h>
#include <vector>
#include "TClonesArray.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <TH1D.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>

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
    double m,n,y = 0.;
    m = hit2->getY()-hit1->getY();
    n = hit2->getZ()-hit1->getZ();
    return n*(y-hit2->getY())/m + hit2->getZ(); //from 3D line equations
}


/**
 * @brief loops on hits points in order to find vertex's Z
 * 
 * @param hitsArray1 
 * @param hitsArray2 
 */
void Reconstruction::vertexReconstruction(TClonesArray *hitsArray1, TClonesArray *hitsArray2){
    double phi = 0.;
    double deltaPhi = 0.01; 
    
    double ztemp = 0;
    vector<double> zTrackVert;

    double binW = 0.5;
    TH1D* histoHit = new TH1D("histoHit","Vertex's z rec",int(60/binW),-30.,30.);  
    //qui arriva                                                                  
    for(int i=0; i<hitsArray1->GetEntries(); i++)        //qui cicla 4 volte                    
    {

        Hit *hitptr=(Hit*)hitsArray1->At(i);
        if ((hitptr->getZ()<13.5)&&(hitptr->getZ()>-13.5))
        {
            phi = hitptr->getPhi();
            
            for(int j=0; j<hitsArray2->GetEntries(); j++)
            {
                Hit *hitptr1=(Hit*)hitsArray2->At(j);
                if((hitptr1->getPhi()<phi+deltaPhi) && (hitptr1->getPhi()>phi-deltaPhi) && (hitptr1->getZ()>-13.5))
                {
                    ztemp = recZvert( hitptr,hitptr1);
                    histoHit->Fill(ztemp);
                    zTrackVert.push_back(ztemp);
                }
                
            }
            
        }
       
    }

    int binmax = histoHit->GetMaximumBin();
    double zMax = histoHit->GetXaxis()->GetBinCenter(binmax);
    delete histoHit;

    vector<double> zTrackVert1;
    for(int aa=0; aa<(int)zTrackVert.size(); aa++)          
    {
        if((zTrackVert[aa]<zMax+binW/2)&&(zTrackVert[aa]>zMax-binW/2)) zTrackVert1.push_back(zTrackVert[aa]);
    }
    double som = 0.;
    for(int a=0; a<(int)zTrackVert1.size(); a++) som = som+zTrackVert1[a];

    zVertVecRec.push_back(som/zTrackVert1.size());
}


/**
 * @brief read data from tree
 * 
 */
void Reconstruction::runReconstruction()
{ 
    cout<<"begin reconstruction..."<<endl;
    Yaml::Node root;
    Yaml::Parse(root, fConfigFile.c_str());

    TFile hfile(root["outputPaths"]["treeSimPath"].As<std::string>().c_str());
    TTree *tree = (TTree*)hfile.Get(root["outputNames"]["treeSimName"].As<std::string>().c_str());
    //tree->SetDirectory(0);  // giogio


    const int nlayer = root["n_detectors"].As<int>() - 1; // n_detectors counts beam pipe as well
    
    TBranch *br[nlayer];
    TBranch *bv = tree->GetBranch("Vertex");
    
    for(int b=1; b<3; b++)
    {
        br[b-1]=tree->GetBranch(Form("HitsL%d",b));
    }

    TClonesArray *hitsArray[nlayer];
    // qui proprio fuori di testa
    for (TClonesArray * &a: hitsArray)  a = new TClonesArray("Hit",100);
    //for(int yy=0; yy<nlayer; yy++)  *hitsArray[yy] = TClonesArray("Hit",100); 
    Vertex vertex;
    bv->SetAddress(&vertex);

    for(int b=1; b<3; b++)
    {
          br[b-1]->SetAddress(&hitsArray[b-1]);
    }

    // questo te lo metto nella stessa logica di quello che abbiamo fatto per plotter
    const int nEvents = tree->GetEntries();
    zVertVec.reserve(nEvents);          // fix vector sizes
    zMoltVec.reserve(nEvents);

    for(int ev=0; ev<nEvents; ev++)
    {
        if(ev%1000==0)    cout << "Processing event " << ev << "..." << endl;
        tree->GetEvent(ev);
        zVertVec.push_back(vertex.getZ());
        zMoltVec.push_back(vertex.getMultiplicity());

        for(int ll=0; ll<nlayer; ll++)
        { 
            const int numHits = hitsArray[ll]->GetEntries();
            
            for(int i=0;ii<numHits;i++)//smearing
            {
                Hit *hitptr2 = (Hit*)hitsArray[ll]->At(i);
                hitptr2->smearing();
                
            }
        
            int noi=int(gRandom->Rndm()*10);//add noise
            hitsArray[ll]->Expand(hitsArray[ll]->GetEntries()+noi);
            for(int i=numHits; i<numHits+noi; i++)
            {
                Hit * hit1 = (Hit*)hitsArray[ll]->ConstructedAt(i);
                hit1->noise(ll+1);               
            }
        }

        vertexReconstruction(hitsArray[0], hitsArray[1]);
        for(int i=0; i<nlayer; i++) hitsArray[i]->Clear();
    }
    hfile.Close();  // giogio
}
    

    



