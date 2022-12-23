#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

class Simulation
{
    public:
        Simulation(const char * TreeName, const char * ConfigFile): fTreeName(TreeName), fConfigFile(ConfigFile){};
        ~Simulation(){};

        /**
         * @brief Run a simulation for n events. Results (hits and vertex positions) are stored in a TTree (.root file).
         * 
         * @param nEvents 
         */
        void runSimulation(const int nEvents);

    private:
        std::string fTreeName;          // description for ROOT if this becomes a TObject - probably not necessary
        std::string fConfigFile;    //


};

#endif