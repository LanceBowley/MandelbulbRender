//
// Created by lance on 5/1/17.
//

#ifndef RENDER_STATICPATH_H
#define RENDER_STATICPATH_H

#include "Path.h"
#include "../Filters/Filter3f.h"

class BasicPath : public Path {
public:

    BasicPath(cv::Vec3f _position);
    BasicPath(Filter3f* _position);
    BasicPath(std::string xF, std::string yF, std::string zF);
    BasicPath(float x, float y, float z);

    void setPosition(cv::Vec3f _position);
    void setPosition(Filter3f* _position);
    cv::Vec3f getPosition(float cyclePosition = 0);

private:

    Filter3f* position;
};

#endif //RENDER_STATICPATH_H
