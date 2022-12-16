#ifndef VERTEX_H
#define VERTEX_H

#include "point.hpp"

/**
 * @brief Struct with basic info on vertex points
 * 
 * @param[in] leaflist leaflist for TTree::Branch() (https://root.cern.ch/doc/master/classTTree.html#ad10fdaab73d4e756aa785d38b1b1f8d8)
 * 
 */
typedef struct
{
    double X, Y, Z;
    int multiplicity;
    const char * leaflist = "X/D:Y:Z:multiplicity:I";
} Vertex;

#endif