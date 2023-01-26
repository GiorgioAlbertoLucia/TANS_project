#include <string>
#include <fstream>

#include <TClonesArray.h>
#include <TRandom3.h>

#include "../pointCC/pointCC.hpp"
#include "../hit/hit.hpp"
#include "../vertex/vertex.hpp"
#include "recorder.hpp"

Recorder * Recorder::fInstancePtr = NULL;

/*  PROTECTED   */
double Recorder::recZvert(Hit *hit1,Hit *hit2) const
{
    const double r1 = hit1->getRadius();
    const double r2 = hit2->getRadius();
    const double z1 = hit1->getZ();
    const double z2 = hit2->getZ();
    return (z2*r1-z1*r2)/(r1-r2);
}

/*  PUBLIC  */

/**
 * @brief Create instance of the singleton object. 
 * 
 * @param fFilePath file path where event information will be stored
 * @return Recorder* 
 */
Recorder* Recorder::getInstance(const char * fFilePath)
{
    if(fInstancePtr == NULL) 
    {
        fInstancePtr = new Recorder(fFilePath);
    }
    return fInstancePtr;
}

/**
 * @brief Destroy singleton object
 * 
 */
void Recorder::destroy()
{
    if(fInstancePtr)    delete fInstancePtr;
    fInstancePtr = NULL;
}

/**
 * @brief Records hits of all particles through a detector in a .txt file. It can be used to create a 
 * 3D model with tracks
 * 
 * @param fFilePath 
 * @param recordArray 
 * @param multiplicity 
 */
void Recorder::recordTracks(double recordArray[][3], const int multiplicity) const
{
    ofstream file(fFilePath, std::ios::app);
    
    for(int i=0; i<multiplicity; i++)
    {
        if(i==0)    file << "    " << "-   Particles:" << endl;;
        file << "            -   " << "x: " << recordArray[i][0] << " #cm" << endl;
        file << "                " << "y: " << recordArray[i][1] << " #cm" << endl;
        file << "                " << "z: " << recordArray[i][2] << " #cm" << endl;
        file << endl;
    }
    
    file.close();
}

/**
 * @brief Records hits of all particles through a detector in a .txt file. It can be used to create a 
 * 3D model with tracks
 * 
 * @param recordArray 
 */
void Recorder::recordTracks(vector<Hit> recordArray) const
{
    ofstream file(fFilePath, std::ios::app);
    
    for(unsigned long int i=0; i<recordArray.size(); i++)
    {
        if(i==0)    file << "    " << "-   Particles:" << endl;;
        file << "            -   " << "x: " << recordArray[i].getX() << " #cm" << endl;
        file << "                " << "y: " << recordArray[i].getY() << " #cm" << endl;
        file << "                " << "z: " << recordArray[i].getZ() << " #cm" << endl;
        file << endl;
    }
    
    file.close();
}

void Recorder::recordVertex(Vertex& vertex) const
{
    ofstream file(fFilePath, std::ios::app);

    file << "Vertex:" << endl;
    file << "    " << "x: " << vertex.getX() << " #cm" << endl;
    file << "    " << "y: " << vertex.getY() << " #cm" << endl;
    file << "    " << "z: " << vertex.getZ() << " #cm" << endl;
    file << "    " << "multiplicity: " << vertex.getMultiplicity() << endl;

    file.close();
}

/**
 * @brief Creates a .txt file and records vertex position
 * 
 * @param vertex 
 */
void Recorder::beginRecordSimulation(Vertex& vertex, const int nDetectors) const
{
    // create output file
    ofstream createFile(fFilePath.c_str());
    createFile << "# Simulated hit points and vertex for a single event " << endl;
    createFile.close();

    recordVertex(vertex);

    ofstream file(fFilePath.c_str(), std::ios::app);
    file << endl << endl << "nLayers:    " << nDetectors << endl;
    file << "DetectorLayers:" << " # beam pipe is included" << endl;
    file.close();
}

/**
 * @brief Records reconstructed vertex and trajectories. Trajectories are chose with pair of points 
 * leading to the reconstructed vertex with a 5mm tolerance on the z coordinate.
 * 
 * @param hitsArray1 
 * @param hitsArray2 
 * @param zRec reconstructed z coordinate of the vertex [cm]
 */
void Recorder::recordReconstruction(TClonesArray * hitsArray1, TClonesArray * hitsArray2, const double zRec) const
{
    vector<Hit> HitL1, HitL2;               // will be filled with ordinated pair of point
    const double deltaPhi = 0.01;           // angle tolerance

    for(int i=0; i<hitsArray1->GetEntries(); i++)                          
    {   
        Hit *hit1 = (Hit*)hitsArray1->At(i);
        
        if ((hit1->getZ()<13.5)&&(hit1->getZ()>-13.5))  // checks if the hit was registered inside the detector lenght
        {   
            for(int j=0; j<hitsArray2->GetEntries(); j++)
            {
                Hit *hit2 = (Hit*)hitsArray2->At(j);
                HitL1.reserve(hitsArray1->GetEntries());
                HitL2.reserve(hitsArray2->GetEntries());

                if((hit2->getPhi()<hit1->getPhi()+deltaPhi) && (hit2->getPhi()>hit1->getPhi()-deltaPhi) 
                    && (hit2->getZ()>-13.5) && (hit2->getZ()<13.5))
                {
                    const double z = recZvert(hit1, hit2);
                    if(abs(z-zRec)<1.)    
                    {
                        HitL1.push_back(*hit1);
                        HitL2.push_back(*hit2);
                    }
                }  
            }
        }
    }

    // create output file
    ofstream createFile(fFilePath.c_str());
    createFile << "# Reconstructed hit points and vertex for a single event " << endl;
    createFile.close();

    // x and y coordinates of the vertex are generated from a gaussian distribution
    Vertex vertex(gRandom->Gaus(0., 0.01), gRandom->Gaus(0., 0.01), zRec, int(HitL1.size()));
    recordVertex(vertex);

    ofstream file(fFilePath.c_str(), std::ios::app);
    file << endl << endl << "nLayers:    2" << endl;
    file << "DetectorLayers:" << " # beam pipe is excluded" << endl;
    file.close();

    recordTracks(HitL1);
    recordTracks(HitL2);    
}

