//
// Created by lance on 5/1/17.
//

#ifndef RENDER_SPLINEPATH_H
#define RENDER_SPLINEPATH_H


#include "../Catmul.h"
#include "Path.h"

class SplinePath : public Path {
public:

    Catmul * spline;
    cv::Vec3f getPosition(float cyclePosition);

private:
};


#endif //RENDER_SPLINEPATH_H
