//
// Created by lance on 5/1/17.
//

#ifndef RENDER_PATHANALYZER_H
#define RENDER_PATHANALYZER_H


#include <opencv2/core/matx.hpp>
#include "Path.h"

class PathAnalyzer {
public:

    PathAnalyzer();

    float getVelocity(Filter1f * filter, float cyclePosition1, float cyclePosition2, float nFrames = 1);
    cv::Vec3f getVelocity(Path *path, float cyclePosition1, float cyclePosition2, float nFrames = 1);
    float getVelocityMagnitude(Path *path, float cyclePosition1, float cyclePositon2, float nFrames = 1);

private:

    float error;
};


#endif //RENDER_PATHANALYZER_H
