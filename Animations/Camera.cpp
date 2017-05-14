//
// Created by lance on 4/6/17.
//

#include "Camera.h"
#include "Filters/Filter3f.h"
#include "Paths/BasicPath.h"

Camera::Camera()
{
    path = new BasicPath("0", "0", "0");
    orientation = new BasicPath("0", "0", "0");
};

Camera::Camera(Path * _path, Path * _orientation)
{
    path = _path;
    orientation = _orientation;
}

cv::Vec3f Camera::getPosition(float cyclePosition)
{
    return path->getPosition(cyclePosition);
}

cv::Vec3f Camera::getOrientation(float cyclePosition)
{
    return orientation->getPosition(cyclePosition);
}