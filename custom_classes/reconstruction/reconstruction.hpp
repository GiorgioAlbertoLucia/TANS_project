#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <vector>

#include <TObject.h>
#include <TH1D.h>
#include "TClonesArray.h"

#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"

/**
 * @brief Singleton class to reconstruct the position of the primary vertex and perform analysis on that
 * process (print efficiency and resolution graphs, ...).
 * 
 */
class Reconstruction
{
    public:
        Reconstruction(const Reconstruction& reconstruction) = delete;  // delete copy constructor for singleton object
        void operator=(const Reconstruction& reconstruction) = delete;  // delete operator= for singleton object

        static Reconstruction * getInstance(const char * configFile);
        static void destroy();
    
        void runReconstruction();

    protected:
        void vertexReconstruction(TClonesArray *hitsArray1,TClonesArray *hitsArray2, TH1D* histoHit, const int ev, const double binW, const double deltaPhi); 
        double recZvert(Hit *hit1,Hit *hit2);
        
        
    private:
        Reconstruction(){};
        Reconstruction(const char * configFile);
        virtual ~Reconstruction(); 

        static Reconstruction* fInstancePtr;

        std::string fConfigFile;
       
        //vector<double> zVertVec;
        //vector<double> zMoltVec;
        //vector<double> zVertVecRec;

        double * zVertVec;
        double * zMoltVec;
        double * zVertVecRec;
        int fNEvents;                // size of all arrays

        

};

#endif