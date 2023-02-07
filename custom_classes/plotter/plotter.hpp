#ifndef PLOTTER_H
#define PLOTTER_H
#include <TObject.h>
#include <vector>
#include "TClonesArray.h"
#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"

/**
 * @brief Class to create resolution and efficiency plots
 * 
 */
class Plotter
{
    public:
        Plotter(){};
        Plotter(const char * configFile): fConfigFile(configFile) {};
        ~Plotter(){}; 

        void addVector(double zVertReal1[], double zVertRec1[], double moltReal1[], const int size);
    
        void runPlots();
        void residues(TObjArray* arrHisto,double *Xarray, int nn, double *resolution,double *resolutionErr,double *efficiency,double *efficiencyErr,bool bol, string outputPath);

    private:
        string fConfigFile;             // path to the configuration file (is used to choose the output location)

        double * zVertReal;             // [fNEvents]
        double * zVertRec;              // [fNEvents]
        double * moltReal;              // [fNEvents]
        double * resVec;                // [fNEvents]

        int fNEvents;                   // number of events (size of all arrays)












};

#endif
