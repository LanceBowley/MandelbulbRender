//
// Created by lance on 4/5/17.
//

#ifndef RENDER_PATH_H
#define RENDER_PATH_H

#include <iostream>
#include "../../Util.h"
#include "../Filters/Filter1f.h"
#include "../Filters/Filter1f.h"
#include "../Catmul.h"

class Path {
public:

    bool filteringEnabled;
    Filter1f * filter;

    Path();

    float modifyCyclePosition(float _cyclePosition);

    virtual cv::Vec3f getPosition(float cyclePosition) = 0;

private:

};


#endif //RENDER_PATH_H
