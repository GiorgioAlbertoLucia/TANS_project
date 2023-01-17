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

        void addVector(vector<double> zVertReal1, vector<double> zVertRec1, vector<double> moltReal1);
        void runPlots();
        void residues(TObjArray* arrHisto,double *Molt, int nn, double *resolution,double *resolutionErr,double *efficiency);
        

    private:
        vector <double> zVertReal;
        vector <double> zVertRec;
        vector <double> moltReal;
        double nEvents;












};

#endif
