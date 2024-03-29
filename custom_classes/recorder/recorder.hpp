#ifndef RECORDER_H
#define RECORDER_H

/**
 * @brief Singleton class to record hits from a simulated or reconstructed event into a .txt file
 * 
 */
class Recorder
{
    public:
        Recorder(const Recorder& recorder) = delete;            // delete copy constructor for a singleton class
        void operator=(const Recorder& recorder) = delete;      // delete operator= for singleton object

        static Recorder* getInstance(const char * filePath);
        static void destroy();
        
        void beginRecordSimulation(Vertex& vertex, const int nDetectors) const;
        void recordReconstruction(TClonesArray * hitsArray1, TClonesArray * hitsArray2, const double zRec) const;

        void recordTracks(vector<Hit> recordArray) const;
        void recordTracks(double recordArray[][3], const int multiplicity) const;
        void recordVertex(Vertex& vertex) const;

    protected:
        double recZvert(Hit * hit1, Hit * hit2) const;

    private:
        // private constructor and destructor for a singleton class
        Recorder(){};
        Recorder(const char * filePath): fFilePath(filePath) {};
        virtual ~Recorder(){};
        
        static Recorder* fInstancePtr;                          // pointer to the instance of the class
        string fFilePath;                                       // file to record into
        

};

#endif