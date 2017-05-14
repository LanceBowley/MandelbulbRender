//
// Created by lance on 5/1/17.
//

#include "BasicPath.h"

BasicPath::BasicPath(cv::Vec3f _position)
{
    setPosition(_position);
}

BasicPath::BasicPath(Filter3f* _position)
{
    setPosition(_position);
}

BasicPath::BasicPath(std::string xF, std::string yF, std::string zF)
{
    setPosition(new Filter3f(xF, yF, zF));
}

BasicPath::BasicPath(float xF, float yF, float zF)
{
    setPosition(cv::Vec3f(xF, yF, zF));
}

void BasicPath::setPosition(cv::Vec3f _position)
{
    position = new Filter3f(_position.val[0], _position.val[1], _position.val[2]);
}

void BasicPath::setPosition(Filter3f* _position)
{
    position = _position;
}

cv::Vec3f BasicPath::getPosition(float cyclePosition)
{
    return position->val(modifyCyclePosition(cyclePosition));
}
