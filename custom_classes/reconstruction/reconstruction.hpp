#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H
#include <TObject.h>
#include <vector>

#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
class Reconstruction

{
    public:
        Reconstruction(){};
        Reconstruction(const char * TreeName, const char * ConfigFile): fTreeName2(TreeName), fConfigFile(ConfigFile){};
        ~Reconstruction();
       

        void readTree();
        void runReconstruction();   // sono un rompipalle, ti voglio bene
        
    private:
        std::string fTreeName2; 
        std::string fConfigFile;
        vector<Hit> hitsVec;
        vector<double> zVertVec;


        

};

#endif