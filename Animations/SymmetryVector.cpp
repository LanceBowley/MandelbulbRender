//
// Created by lance on 5/2/17.
//

#include "SymmetryVector.h"

SymmetryVector::SymmetryVector()
{
    enabled = true;
    position = new Filter3f("0", "0", "0");
    orientation = new Filter3f("0", "0", "0");
    gamma = new Filter3f("0", "0", "0");
    distance = new Filter1f("0");
    intensity = new Filter1f("0");
}

void SymmetryVector::setPosition(Filter3f* _position)
{
    position = _position;
}

cv::Vec3f SymmetryVector::getPosition(float cyclePosition)
{
    return position->valNormalized(cyclePosition);
}
