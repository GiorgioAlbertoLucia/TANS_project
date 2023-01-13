void Reconstruction::runReconstruction(TClonesArray hitsArray1, TClonesArray hitsArray2){//loop on points for the vertex's reconstruction
    double phi = 0.;
    double deltaPhi = 0.01; 
    
    double ztemp = 0;
    vector<double> zTrackVert;
    TH1D* histoHit;
    double binW = 0.5;
    histoHit = new TH1D("histoHit","Vertex's z rec",int(27/binW),0.,27.);  
                                                                            
    for(int i=0; i<hitsArray1.GetEntries(); i++)                            
    {
        Hit *hitptr=(Hit*)hitsArray1.At(i);
        phi = hitptr->getPhi();
        for(int j=0; j<hitsArray2.GetEntries(); j++)
        {
            Hit *hitptr1=(Hit*)hitsArray2.At(j);
            if((hitptr1->getPhi()<phi+deltaPhi)&&(hitptr1->getPhi()>phi-deltaPhi))
            {
                ztemp = recZvert( hitptr,hitptr1);
                histoHit->Fill(ztemp);
                zTrackVert.push_back(ztemp);
            }
        }
    }

        int binmax = histoHit->GetMaximumBin();
        double zMax = histoHit->GetXaxis()->GetBinCenter(binmax);

        vector<double> zTrackVert1;
        for(int aa=0; aa<(int)zTrackVert.size(); aa++)          // ti volevo dire che esiste la sintassi (per i vector, non so se anche per array)
                                                                // for(double z: zTrackVert) { if((z < (zMax+binW/2.)) && ... ) ...}  // ou fatti i fatti tuoi, smettila di deprimermi!!!
        {
            if((zTrackVert[aa]<zMax+binW/2)&&(zTrackVert[aa]>zMax-binW/2)) zTrackVert1.push_back(zTrackVert[aa]);
        }
        double som = 0.;
        for(int a=0; a<(int)zTrackVert1.size(); a++) som = som+zTrackVert1[a];

        zVertVecRec.push_back(som/zTrackVert1.size());
    }






void Reconstruction::loadHits()
{ 
    Yaml::Node root;
    Yaml::Parse(root, fConfigFile.c_str());

    const int nlayer = root["n_detectors"].As<int>() - 1; // n_detectors counts beam pipe as well

    TFile hfile(root["inputPaths"]["distributions"].As<std::string>().c_str());
    TTree *tree = (TTree*)hfile.Get(root["outputNames"]["treeSimName"].As<std::string>().c_str());

    // initialize TBranches
    TBranch *bVertex = tree->GetBranch("Vertex");
    TBranch *bHits[nlayer];            
    for(int b=1; b<(nlayer+1); b++)     bHits[b-1]=tree->GetBranch(Form("HitsL%d",b));
    
    // initialize objects to link to TBranches
    Vertex vertex;
    TClonesArray hitsArray[nlayer];
    for(int i=0; i<nlayer; i++)         hitsArray[i] = TClonesArray("Hit",100); 

    // link objects to TBranches
    bVertex->SetAddress(&vertex);
    for(int b=1; b<(nlayer+1); b++)      bHits[b-1]->SetAddress(&hitsArray[b-1]);

    for(int ev=0; ev<tree->GetEntries(); ev++)
    {
        tree->GetEvent(ev);

        zVertVec.push_back(vertex.getZ());
        zMoltVec.push_back(vertex.getMultiplicity());

        // for each layer, add noise and smearing to the detector response
        int numHits[nlayer];
        for(int i=0; i<nlayer; i++)
        { 
            numHits[i] = hitsArray[i].GetEntries();  
            
            // add smearing
            for(int j=0; j<numHits[i]; j++)
            {
                Hit *hitptr2 = (Hit*)hitsArray[i].At(j);
                hitptr2->smearing();
            }
        
            // add noise
            int noi=int(gRandom->Rndm()*50);
            for(int j=numHits[i]+1; j<numHits[i]+noi+1; j++)
            {
                new(hitsArray[i][j]) Hit();
                Hit * hit1 = (Hit*)hitsArray[i].At(j);  
                hit1->noise();                     
            }
        }

        runReconstruction(hitsArray[0], hitsArray[1]);
        for(int i=0; i<nlayer; i++) hitsArray[i].Clear();
    }
}