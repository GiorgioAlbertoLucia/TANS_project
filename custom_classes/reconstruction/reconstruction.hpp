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
        ~Reconstruction(){};        // ti ho aggiunto io le parentesi. Stavo provando a compilare e si 
                                    // lamentava che gli avessi inizializzato il distruttore senza definirlo bene
                                    // (visti i tuoi data memeber in realtà potrebbe essere il caso di fargli cancellare 
                                    // delle cose - non sono sicurissimo però -. Molto probabilmente ci avevi già 
                                    // pensato e volevi farlo poi)
       

        void loadHits();
        void runReconstruction(TClonesArray hitsArray1,TClonesArray hitsArray2);   // sono un rompipalle, ti voglio bene
        double recZvert(Hit *hit1,Hit *hit2);
        
    private:
        std::string fTreeName2; 
        std::string fConfigFile;
        vector<Hit> hitsVec;
        vector<double> zVertVec;


        

};

#endif