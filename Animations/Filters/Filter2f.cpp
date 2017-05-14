//
// Created by lance on 5/1/17.
//

#include "Filter2f.h"

Filter2f::Filter2f(float valX, float valY)
{
    xFilter = new Filter1f(valX);
    yFilter = new Filter1f(valY);
}

Filter2f::Filter2f(std::string valX, std::string valY)
{
    xFilter = new Filter1f(valX);
    yFilter = new Filter1f(valY);
}

void Filter2f::setTruncation(float _lowerBound, float _upperBound)
{
    for (int i = 0; i < 2; i++)
    {
        (*this)[i]->setTruncation(_lowerBound, _upperBound);
    }
}

cv::Vec2f Filter2f::val(float cyclePosition)
{
    cv::Vec2f v = cv::Vec2f((*this)[0]->val(cyclePosition), (*this)[1]->val(cyclePosition));
    return v;
}
