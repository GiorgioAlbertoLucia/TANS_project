#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

#include <TH1I.h>
#include <TH1F.h>

class Simulation
{
    public:
        Simulation(){};
        Simulation(const char * TreeName, const char * ConfigFile): fTreeName(TreeName), fConfigFile(ConfigFile){};
        ~Simulation(){};

        inline std::string getTreeName() const {return fTreeName;};
        inline std::string getConfigFile() const {return fConfigFile;};

        void runSimulation(const int nEvents);
        /**
         * @brief version with vectors
         * 
         * @param nEvents 
         */
        void runSimulation2(const int nEvents);

    protected:
        /**
         * @brief Function to call during simulation of a event. Will create a .txt file storing positions of particles
         * during transport. Intended to be used to display the event with TGeoManager
         * 
         */
        void recordTracks();

    private:
        std::string fTreeName;          // description for ROOT if this becomes a TObject - probably not necessary
        std::string fConfigFile;    //


};

#endif