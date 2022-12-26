#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H
#include <TObject.h>
#include <vector>

#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
class Reconstruction

{
    public:
        Reconstruction();
        ~Reconstruction();

        void runReconstruction();   // sono un rompipalle, ti voglio bene
        
    private:
        std::string fTreeName2; 
        std::string fTreeName3;         
        std::string fConfigFile;
        vector<Hit> hitsVec;
        vector<double> zVec;


        

};

#endif