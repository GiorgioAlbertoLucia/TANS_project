#ifndef PLOTTER_H
#define PLOTTER_H
#include <TObject.h>
#include <vector>
#include "TClonesArray.h"
#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
//#include "../reconstruction/reconstruction.hpp"

class Plotter
{
    public:
        Plotter(){};
        Plotter(const char * configFile): fConfigFile(configFile) {};
        
        ~Plotter(){}; 

        void addVector(double zVertReal1[], double zVertRec1[], double moltReal1[], const int size);
        //void addVector(vector<double> &zVertReal1, vector<double> &zVertRec1, vector<double> &moltReal1); //provare con i puntatori se va male
        void runPlots();
        void residues(TObjArray* arrHisto,double *Xarray, int nn, double *resolution,double *resolutionErr,double *efficiency,double *efficiencyErr,bool bol, string outputPath);

    private:
        string fConfigFile;             // path to the configuration file (is used to choose the output location)

        //vector <double> zVertReal;
        //vector <double> zVertRec;
        //vector <double> moltReal;
        //vector <double> resVec;

        double * zVertReal;             // [fNEvents]
        double * zVertRec;              // [fNEvents]
        double * moltReal;              // [fNEvents]
        double * resVec;                // [fNEvents]

        int fNEvents;                   // number of events (size of all arrays)












};

#endif
