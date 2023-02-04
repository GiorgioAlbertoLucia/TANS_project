#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

#include <TH1I.h>
#include <TH1F.h>
#include <TClonesArray.h>

#include "../vertex/vertex.hpp"

/**
 * @brief Singleton class to simulate multiple events and store relevant informations in a TTree
 * 
 */
class Simulation
{
    public:
        Simulation(const Simulation& simulation) = delete; // delete copy constructor for a singleton class
        void operator=(const Simulation& simulation) = delete;  // delete operator= for singleton object

        static Simulation* getInstance(const char * configFile);
        static void destroy();

        inline std::string getConfigFile() const {return fConfigFile;};

        void runSimulation(const int nEvents);

    private:
        Simulation(){};
        Simulation(const char * configFile): fConfigFile(configFile){};
        ~Simulation(){};

        static Simulation *fInstancePtr;    // static pointer to the instance of the class

        std::string fConfigFile;            //  path to the configuration file used


};

#endif