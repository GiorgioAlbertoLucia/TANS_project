#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

class Simulation
{
    public:
        Simulation();
        ~Simulation();

        RunSimulation();

    private:
        std::string fTreeName;          // description for ROOT if this becomes a TObject - probably not necessary
        std::string fConfigFile;    //


};

#endif