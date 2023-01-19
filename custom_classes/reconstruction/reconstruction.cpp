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
    TH1D* histoHit;
    double binW = 0.5;
    histoHit = new TH1D("histoHit","Vertex's z rec",int(60/binW),-30.,30.);  
                                                                            
    for(int i=0; i<hitsArray1->GetEntries(); i++)                            
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
                delete hitptr1; // giogio
            }
        }
        delete hitptr; // giogio
    }

    int binmax = histoHit->GetMaximumBin();
    double zMax = histoHit->GetXaxis()->GetBinCenter(binmax);

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
        cout << "event " << ev << endl;
        tree->GetEvent(ev);
        zVertVec.push_back(vertex.getZ());
        zMoltVec.push_back(vertex.getMultiplicity());
        int numHits[nlayer];    // scusa, ma se invece inizializzi la variabile dentro il for loop?
        for(int ll=0; ll<nlayer; ll++)
        { 
            cout << "layer " << ll << endl; 
            // tipo così
            // const int numHits = hitsArray[ll]->GetEntries();
            // e usi questa per ciclare dentro i loop annidati
            numHits[ll] = hitsArray[ll]->GetEntries();  
            cout << "entries " << hitsArray[ll]->GetEntries() << ", " << numHits[ll] << endl;
            for(int ii=0;ii<numHits[ll];ii++)//smearing
            {
                cout << "smearing " << ii << endl;
                Hit *hitptr2 = (Hit*)hitsArray[ll]->At(ii);
                hitptr2->smearing();
            }
        
            int noi=int(gRandom->Rndm()*10);//add noise
            for(int i=numHits[ll]+1; i<numHits[ll]+noi+1; i++)
            {
                cout << "noise " << i << endl;
                new(&hitsArray[ll][i]) Hit();
                Hit * hit1 = (Hit*)hitsArray[ll]->At(i);  
                cout << "test " << i << endl;
                hit1->noise();               
                cout << "test " << i << endl;      
            }
        }

        //vertexReconstruction(hitsArray[0], hitsArray[1]);
        for(int i=0; i<nlayer; i++) hitsArray[i]->Clear();
    }
    hfile.Close();  // giogio
}
    

    



