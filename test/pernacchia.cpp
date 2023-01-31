#include "TImage.h"
#include "TCanvas.h"
#include "TArrayD.h"
#include "TROOT.h"
#include "TColor.h"
#include "TAttImage.h"
#include "TEnv.h"

void pernacchia(const bool condition = 1)
{
    if(condition)
    {
        TImage *img = TImage::Open("pernacchia.png");
 
        if (!img) {
           printf("Could not create an image... exit\n");
           return;
        }
 
        img->SetConstRatio(0);
        img->SetImageQuality(TAttImage::kImgBest);
 
        TCanvas * c1 = new TCanvas("canvas", "pernacchione", 760, 900);
        img->Draw("xxx");
    }
}