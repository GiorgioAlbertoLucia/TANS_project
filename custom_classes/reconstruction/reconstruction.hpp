#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H
#include <TObject.h>

#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
class Reconstruction

{
    public:
        Reconstruction();
        ~Reconstruction();

        void RunReconstruction();
        void ReadTree();
    private:
        std::string fTreeName2; 
        std::string fTreeName3;         
        std::string fConfigFile;   
        

};

#endif