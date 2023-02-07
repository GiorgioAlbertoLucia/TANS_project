void drawHist()
{
    //gStyle->SetOptFit(111);

    TFile inFile("output/residuesGraphs.root");

    TH1D * oldHist = (TH1D*)inFile.Get("resHisto9");
    oldHist->SetDirectory(0);
    inFile.Close();

    double xbins[301];
    double xlow = -600.;
    for(int i=0; i<301; i++)   
    {
        xbins[i] = xlow;
        xlow += 4.;
    }
    TH1D * hist = (TH1D*)oldHist->Rebin(300, "hist", xbins);

    hist->SetMarkerColor(kAzure+8);
    hist->SetMarkerStyle(8);
    hist->GetXaxis()->SetLimits(-600, 600);

    hist->SetTitle("Residues, multiplicity=12");
    hist->GetXaxis()->SetTitle("z_{Rec} - z_{True} [#mum]");
    hist->GetYaxis()->SetTitle("Entries");
    
    //hist->Fit("gaus");
    //hist->GetFunction("gaus")->SetLineColor(kBlack);
    //hist->GetFunction("gaus")->SetLineWidth(7);

    TCanvas * canvas = new TCanvas("canvas", "Residues, multiplicity=12", 80, 80, 1500, 1000);
    hist->Draw("E");

    TFile outFile("graphGen/residuesHisto.root", "recreate");
    //hist->Write();
    canvas->Write();
    outFile.Close();
}