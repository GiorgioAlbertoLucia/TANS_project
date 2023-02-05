#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <vector>

#include <TObject.h>
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

        static Reconstruction * getInstance(const char * configFile, const char * constantsFile);
        static void destroy();
    
        void runReconstruction();

    protected:
        void vertexReconstruction(TClonesArray *hitsArray1,TClonesArray *hitsArray2, const int ev); 
        double recZvert(Hit *hit1,Hit *hit2);
        
        
    private:
        Reconstruction(){};
        Reconstruction(const char * configFile, const char * constantsFile);
        virtual ~Reconstruction(); 

        static Reconstruction* fInstancePtr;

        std::string fConfigFile;
        std::string fConstantsFile;
       
        //vector<double> zVertVec;
        //vector<double> zMoltVec;
        //vector<double> zVertVecRec;

        double * zVertVec;
        int zVertVecSize;
        double * zMoltVec;
        int zMoltVecSize;
        double * zVertVecRec;
        int zVertVecRecSize;

        

};

#endif