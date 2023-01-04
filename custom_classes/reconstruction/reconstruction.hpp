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
    
        void loadHits();
        void runReconstruction(TClonesArray hitsArray1,TClonesArray hitsArray2); 
        double recZvert(Hit *hit1,Hit *hit2);
        void  residues();
        
    private:
        std::string fTreeName2; 
        std::string fConfigFile;
       
        vector<double> zVertVec;
        vector<double> zVertVecRec;


        

};

#endif