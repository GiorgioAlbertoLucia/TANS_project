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
        Reconstruction(){};
        Reconstruction(const char * ConfigFile): fConfigFile(ConfigFile){};
        virtual ~Reconstruction(){};        
    
        void runReconstruction();

    protected:
        void vertexReconstruction2(TClonesArray *hitsArray1, TClonesArray *hitsArray2, int ev);
        void vertexReconstruction(TClonesArray *hitsArray1,TClonesArray *hitsArray2, int ev); 
        double recZvert(Hit *hit1,Hit *hit2,int ev);
        
        
    private:
        std::string fConfigFile;
       
        vector<double> zVertVec;
        vector<double> zMoltVec;
        vector<double> zVertVecRec;


        

};

#endif