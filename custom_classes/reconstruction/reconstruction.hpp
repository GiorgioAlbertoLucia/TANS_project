#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H
#include <TObject.h>
#include <vector>
#include "TClonesArray.h"
#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
class Reconstruction

{
    public:
        Reconstruction(){};
        Reconstruction(const char * TreeName, const char * ConfigFile): fTreeName2(TreeName), fConfigFile(ConfigFile){};
        ~Reconstruction(){};        
    
        void runReconstruction();
        void vertexReconstruction(TClonesArray *hitsArray1,TClonesArray *hitsArray2); 
        double recZvert(Hit *hit1,Hit *hit2);
        
        
    private:
        std::string fTreeName2; 
        std::string fConfigFile;
       
        vector<double> zVertVec;
        vector<double> zMoltVec;
        vector<double> zVertVecRec;


        

};

#endif