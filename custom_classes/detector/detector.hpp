#ifndef DETECTOR_H
#define DETECTOR_H

/**
 * @brief cylindrical particle detecor
 * 
 * @param[in] radius radius of the detector [cm]
 * @param[in] width width of the detector [cm]
 * @param[in] lenght lenght of the detector [cm]
 * @param[in] multipleScattering whether you should consider or not multiple scattering when trasporting through this detector
 *
 */
typedef struct 
{
    double radius, width, lenght;   
    bool multipleScattering;
} Detector;


#endif