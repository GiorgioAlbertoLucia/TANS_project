#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <vector>

#include <TObject.h>
#include "TClonesArray.h"

#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"

class Reconstruction
{
    public:
        Reconstruction(const Reconstruction& reconstruction) = delete;  // delete copy constructor for singleton object
        void operator=(const Reconstruction& reconstruction) = delete;  // delete operator= for singleton object

        static Reconstruction * getInstance(const char * configFile, const char * constantsFile);
        static void destroy();
    
        void runReconstruction();

    protected:
        void vertexReconstruction(TClonesArray *hitsArray1,TClonesArray *hitsArray2); 
        double recZvert(Hit *hit1,Hit *hit2);
        
        
    private:
        Reconstruction(){};
        Reconstruction(const char * configFile, const char * constantsFile): 
                fConfigFile(configFile), fConstantsFile(constantsFile) {};
        virtual ~Reconstruction(){}; 

        static Reconstruction* fInstancePtr;

        std::string fConfigFile;
        std::string fConstantsFile;
       
        vector<double> zVertVec;
        vector<double> zMoltVec;
        vector<double> zVertVecRec;


        

};

#endif