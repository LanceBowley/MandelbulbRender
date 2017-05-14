//
// Created by lance on 5/2/17.
//

#ifndef RENDER_IFS_H
#define RENDER_IFS_H


#include "Filters/Filter1f.h"
#include "SymmetryVector.h"

class IFS {
public:

    IFS();

    bool absX;
    bool absY;
    bool absZ;

    Filter1f* scale;
    Filter3f* orientation;
    Filter3f* offset;
    Filter3f* edge;
};


#endif //RENDER_IFS_H
