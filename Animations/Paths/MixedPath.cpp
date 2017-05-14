//
// Created by lance on 5/1/17.
//

#include "MixedPath.h"

MixedPath::MixedPath(Path* _path1, Path* _path2, Filter1f* _biasFilter)
{
    path1 = _path1;
    path2 = _path2;
    biasFilter = _biasFilter;
}

cv::Vec3f MixedPath::getPosition(float cyclePosition)
{
    cyclePosition = modifyCyclePosition(cyclePosition);
    cv::Vec3f p1 = (1 - biasFilter->val(cyclePosition)) * path1->getPosition(cyclePosition);
    cv::Vec3f p2 = biasFilter->val(cyclePosition) * path2->getPosition(cyclePosition);
    return p1 + p2;
}
