//
// Created by lance on 5/1/17.
//

#ifndef RENDER_COMPOUNDPATH_H
#define RENDER_COMPOUNDPATH_H


#include <opencv2/core/matx.hpp>
#include "Path.h"

class CompoundPath : public Path {
public:

    CompoundPath();
    void addSubPath(Path * subPath, float subPathCyclePosition, float repetitionsPerCycle = 1);

    cv::Vec3f getPosition(float cyclePosition);

private:

    std::vector<Path *> * paths;
    std::vector<float> subPathCyclePositions;
    std::vector<float> subPathRepetitionsPerCycle;
};


#endif //RENDER_COMPOUNDPATH_H
