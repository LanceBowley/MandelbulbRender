//
// Created by lance on 4/6/17.
//

#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H


#include <opencv2/core/matx.hpp>
#include "Paths/Path.h"

class Camera {
public:

    Path * path;
    Path * orientation;

    Camera();
    Camera(Path * _path, Path * _orientation);

    cv::Vec3f getPosition(float cyclePosition);
    cv::Vec3f getOrientation(float cyclePosition);

private:

};


#endif //RENDER_CAMERA_H
