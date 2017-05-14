//
// Created by lance on 5/1/17.
//

#include "Filter3f.h"

Filter3f::Filter3f(float valX, float valY, float valZ)
{
    xFilter = new Filter1f(valX);
    yFilter = new Filter1f(valY);
    zFilter = new Filter1f(valZ);
}

Filter3f::Filter3f(std::string valX, std::string valY, std::string valZ)
{
    xFilter = new Filter1f(valX);
    yFilter = new Filter1f(valY);
    zFilter = new Filter1f(valZ);
}

void Filter3f::setTruncation(float _lowerBound, float _upperBound)
{
    for (int i = 0; i < 3; i++)
    {
        (*this)[i]->setTruncation(_lowerBound, _upperBound);
    }
}

cv::Vec3f Filter3f::val(float cyclePosition)
{
    return cv::Vec3f((*this)[0]->val(cyclePosition), (*this)[1]->val(cyclePosition), (*this)[2]->val(cyclePosition));
}

cv::Vec3f Filter3f::valNormalized(float cyclePosition)
{
    float mag = sqrtf(powf(xFilter->val(cyclePosition), 2) + powf(yFilter->val(cyclePosition), 2) + powf(zFilter->val(cyclePosition), 2));
    return val(cyclePosition) / mag;
}

