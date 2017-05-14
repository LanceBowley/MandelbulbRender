//
// Created by lance on 4/7/17.
//

#ifndef RENDER_COLOR_H
#define RENDER_COLOR_H


#include <opencv2/core/types.hpp>
#include "Paths/Path.h"

// This is 8 bit only. Should probably differentiate into 'color' and 'color path'
class Color {
public:

    enum Space
    {
        BGR,
        HSV
    };

    Path * path;

    Color();
    Color(Path * _path);

    void setPathColorSpace(Space _colorSpace);

    // Returns a 8 Bit cv Scalar in order Blue, Green, Red
    cv::Vec3f getColor(float cyclePosition, Space _colorSpace = BGR);
    std::string rgbToHex(cv::Vec3f color);

private:

    Space pathColorSpace;
    cv::Vec3f cropColor(cv::Vec3f color, Space _colorSpace);
    cv::Vec3f changeColorSpace(cv::Vec3f color, Space intialColorSpace, Space finalColorSpace);
};


#endif //RENDER_COLOR_H
