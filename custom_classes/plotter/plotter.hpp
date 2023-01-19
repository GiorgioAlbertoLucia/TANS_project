#ifndef PLOTTER_H
#define PLOTTER_H
#include <TObject.h>
#include <vector>
#include "TClonesArray.h"
#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
#include "../reconstruction/reconstruction.hpp"

class Plotter
{
    public:
        Plotter(){};
        
        ~Plotter(){}; 

        void addVector(vector<double> &zVertReal1, vector<double> &zVertRec1, vector<double> &moltReal1); //provare con i puntatori se va male
        void runPlots();
        void residues(TObjArray* arrHisto,double *Xarray, int nn, double *resolution,double *resolutionErr,double *efficiency,double *efficiencyErr,bool bol);
        

    private:
        vector <double> zVertReal;
        vector <double> zVertRec;
        vector <double> moltReal;
        vector <double> resVec;
        int nEvents;












};

#endif
