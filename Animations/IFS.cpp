//
// Created by lance on 5/2/17.
//

#include "IFS.h"


IFS::IFS()
{
    bool absX = false;
    bool absY = false;
    bool absZ = false;

    scale = new Filter1f(0);
    orientation = new Filter3f(0, 0, 0);
    offset = new Filter3f(0, 0, 0);
    edge = new Filter3f(0, 0, 0);
}