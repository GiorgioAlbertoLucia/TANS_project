void distrGen()
{
    TFile inFile("data/kinem.root");
    TH1F * hEta = (TH1F*)inFile.Get("heta2");
    hEta->SetDirectory(0);
    inFile.Close();

    TRandom3 * ran = new TRandom3(123);

    const float bin_width = .5;
    TH1F * hMult = new TH1F("hm", "hm", (int)(68/bin_width), 2, 70);
    for(int i=0; i<1000000; i++)    hMult->Fill(68*ran->Rndm()+2);

    TFile outFile("data/kinemUnif.root", "recreate");
    hMult->Write();
    hEta->Write();
    outFile.Close();

}