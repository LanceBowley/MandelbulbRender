//
// Created by lance on 5/1/17.
//

#include "SplinePath.h"

cv::Vec3f SplinePath::getPosition(float cyclePosition)
{
    cyclePosition = modifyCyclePosition(cyclePosition);
    return spline->getPoint(cyclePosition);
}